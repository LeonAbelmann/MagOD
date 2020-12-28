#include <SPI.h> // Communication
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_RA8875.h> // TFTM50 screen
#include "src/MagOD_ADS1115/MagOD_ADS1115.h" // Modified Adafruit_ADS1015 lib


// Define the data structure to be used in the buffer
typedef struct
{
    uint8_t ch;                 // Channel that was measured on
    uint16_t val;               // Value that was measured
    unsigned long time;         // Time of the measurement, the millis function returns a 32bit value meaning it will overflow after 49 days
} bufferData_t;


// Create a handle for the buffer
QueueHandle_t dataQueueHandle;              // Handle which will be used to refer to the buffer
BaseType_t xHigherPriorityTaskWokenADC0;    // Variable to store the outcome of a notification in, explained later
bufferData_t readData;                      // Temp variable to store the data in that was just retrieved from the buffer
#define bufferSize 1000                     // Definition of the buffer size, how many elements does it have
uint16_t bufferRemaining;                   // Variable to store the remaining buffer size in


// Initialize the ADCs
#define ADS1015_ADDRESS_0 (0x48)            // 1001 000 (ADDR -> GND)
#define ADS1015_ADDRESS_1 (0x49)            // 1001 001 (ADDR -> VDD)
#define ADS1115_RDY 13                      // ESP32 pin that the ready pin of the ADC is attached to
Adafruit_ADS1115 ads0(ADS1015_ADDRESS_0);   // Initialize adc0-3;
Adafruit_ADS1115 ads1(ADS1015_ADDRESS_1);   // Initialize adc4-7;
#define sampleRateADC0 2                    // Sample rates: 0: 8 SPS, 1: 16 SPS, 2: 32 SPS, 3: 64 SPS, 4: 128 SPS, 5: 250 SPS, 6: 475 SPS 7: 860 SPS
TaskHandle_t readADC0Handle;                // Handle to refer to the task that does the ADC measurement



// Coil variables that define off
// This was copied straight from the ADC test sketch
uint8_t Coil_x = 33;                        // output of the coils in the x direction
uint8_t Coil_y = 26;                        // output of the coils in the y direction
uint8_t Coil_z = 14;                        // output of the coils in the z direction
const int ledChannel_x = 3;                 // 0-15
const int ledChannel_y = 4;                 // 0-15
const int ledChannel_z = 5;                 // 0-15


// Initialize the display
#define RA8875_INT 16                       // Define TFT pins
#define RA8875_CS 5                         // Define TFT pins
#define RA8875_RESET 17                     // Define TFT pins
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);     // Initialize the display
char stringBuffer[15];                      // Declare a buffer for dtostrf to use on the display


// ADC 0 interrupt routine
void IRAM_ATTR adc0ISR() {                  // IRAM_ATTR is used to make sure that the interrupt routine (ISR) can run while SPI or i2c communication is happening
    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#_CPPv418xTaskNotifyFromISR12TaskHandle_t8uint32_t13eNotifyActionP10BaseType_t
    
    xTaskNotifyFromISR(readADC0Handle, 0, eNoAction, &xHigherPriorityTaskWokenADC0);    // Notify task readADC0Handle to start running and don't pass along any data with the notification (0, eNoAction)

    //Handle possible task exception due to the nature of an ISR, see link above for more information
    if ( xHigherPriorityTaskWokenADC0 ) {
        portYIELD_FROM_ISR ();
    }
}


// ADC 0 readTask
// Designed according to example:
// https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#_CPPv411xTaskCreate14TaskFunction_tPCKcK8uint32_tPCv11UBaseType_tPC12TaskHandle_t
void readADC0( void * parameter ) {
    int iChan = 0;                                  // Counter to keep track of the channel that should be measured next
    bufferData_t writeData;                         // Temp variable to store the data from the ADC in temporarily
    BaseType_t queueStatus;                         // Temp variable storing the status of the buffer
    
    for (;;) {                                      // Infinite loop to keep task running
        // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#_CPPv415xTaskNotifyWait8uint32_t8uint32_tP8uint32_t10TickType_t
        if (xTaskNotifyWait(0, 0, 0, pdMS_TO_TICKS(5000)) == pdPASS) {  //Wait at maximum 5000ms for a notification otherwise it will return a fail. Also don't do anything with the notification bytes (0,0,0 ...)
            
            //Save the data to the temporary variable
            writeData.val = ads0.getADC();          // Save ADC value
            writeData.time = millis();              // Save time at which the data was retrieved
            writeData.ch = iChan;                   // Save the channel on which the measurement was done

            // Push data to buffer
            // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#c.xQueueSendToBack
            queueStatus = xQueueSendToBack(dataQueueHandle, &writeData, 0);     // Push the data to buffer dataQueueHandle and do not wait to retry if it was unsuccesfull
            
            //Check if succesfull
            if ( queueStatus != pdTRUE ) {
                Serial.println("Buffer is full!");
            }

            //Increase channel counter and handle overflow
            iChan++;
            if (iChan >= 4) {                       // There are 4 channels to cover 0-3, overflow if it hits 4 or higher
                iChan = 0;
            }

            //Start next measurement
            ads0.startReadADC(iChan, sampleRateADC0);   // Start the next measurement on the ADC by setting the channel and the samplerate
        }
    }
}


void tftWriteStatic(){                                  // Write the static portions of the tft display during setup
    tft.fillScreen(RA8875_BLACK);
    tft.textMode();
    tft.textTransparent(RA8875_WHITE);
    tft.textSetCursor(190, 1);
    tft.textWrite("ADCqueueTest.ino");
    tft.textSetCursor(40, 1);
    tft.textWrite("Value");
    tft.textSetCursor(100, 1);
    tft.textWrite("Time (ms)");
    tft.textSetCursor(360, 1);
    tft.textWrite("Buffer:");
    
    for (int i = 0; i < 8 ;i++){
        tft.textSetCursor(1, 20 + i * 15);
        tft.textWrite("ADC");
        tft.textSetCursor(24, 20 + i * 15);
        tft.textWrite(dtostrf(i, 1, 0, stringBuffer), 2);
    }
}

void tftWriteBuffer(uint16_t size){                     // Write the size of the buffer that is still empty
    tft.fillRect(420, 1, 30, 15, RA8875_BLUE);
    tft.textTransparent(RA8875_WHITE);
    tft.textSetCursor(420, 1);
    tft.textWrite(dtostrf(size, 3, 0, stringBuffer), 3);
}

void tftWriteADC(bufferData_t bufferTftData){           // Write the data from the buffer of 1 of the ADCs
    tft.fillRect(40, 20 + bufferTftData.ch * 15, 160, 15, RA8875_BLUE);
    tft.textTransparent(RA8875_WHITE);

    tft.textSetCursor(40, 20 + bufferTftData.ch * 15);
    tft.textWrite(dtostrf(bufferTftData.val, 5, 0, stringBuffer), 6);
    tft.textSetCursor(100, 20 + bufferTftData.ch * 15);
    tft.textWrite(dtostrf(bufferTftData.time, 11, 0, stringBuffer), 12);
}

void setup() {
    // Open the serial interface
    Serial.begin(115200);

    // Switch off the magnets
    // This was copied straight from the ADC test sketch
    ledcSetup(ledChannel_x, 1000, 8);
    ledcSetup(ledChannel_y, 1000, 8);
    ledcSetup(ledChannel_z, 1000, 8);
    ledcAttachPin(Coil_x, ledChannel_x);
    ledcAttachPin(Coil_y, ledChannel_y);
    ledcAttachPin(Coil_z, ledChannel_z);
    ledcWrite(ledChannel_x, 0);
    ledcWrite(ledChannel_y, 0);
    ledcWrite(ledChannel_z, 0);

    //Setup the display
    Serial.println("Trying to find RA8875 screen...");
    if (!tft.begin(RA8875_480x272)) {           // using 'RA8875_480x272' or 'RA8875_800x480'
        Serial.println("RA8875 Not Found!");
        while (1);
    }
    Serial.println("RA8875 Found");
    tft.displayOn(true);
    tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
    tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
    tft.PWM1out(255);
    tftWriteStatic();       // Write the static display elements

    // Create the buffer
    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#c.xQueueCreate
    dataQueueHandle = xQueueCreate(bufferSize, sizeof(bufferData_t));   // Create a buffer with a size of bufferSize and a element size equal to the size of bufferData_t, the custom struct

    //Setup the ADCs
    ads0.setGain(GAIN_ONE);
    ads0.begin();
    ads1.setGain(GAIN_ONE);
    ads1.begin();
    pinMode(ADS1115_RDY, INPUT_PULLUP);                 // ads0 is interupt based. Pin 13 is connect to ALERT/RDY pin of ad0
    attachInterrupt(ADS1115_RDY, adc0ISR, FALLING);     // Interupt when RDY goes down

    // Setup ADC0 Task
    // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#_CPPv423xTaskCreatePinnedToCore14TaskFunction_tPCKcK8uint32_tPCv11UBaseType_tPC12TaskHandle_tK10BaseType_t
    xTaskCreatePinnedToCore(
        readADC0            // Name of the function that will serve as the task
        ,  "readADC0"       // A name just for humans
        ,  4096             // Stack size can be seen as RAM allocated to the task this stack size can be checked & adjusted by reading the uxTaskGetStackHighWaterMark(); it has not been optimized yet. 4096B is equal to that of the void loop task
        ,  NULL             // Extra pointer parameter which is not neccessary
        ,  2                // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,  &readADC0Handle  // Handle to refer to the task
        ,  1);              // Core to run the task on
        
    //Start measurement
    ads0.startReadADC(0, sampleRateADC0);

    //Add delay to force the buffer to fill up initially
    delay(2000);
}

void loop() { 
    bufferRemaining = uxQueueSpacesAvailable(dataQueueHandle);      // Get the amount of free buffer spaces
    if (bufferRemaining < bufferSize) {                             // If what is remaining is smaller than the total size there must be something in the buffer

        // https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#c.xQueueReceive
        xQueueReceive(dataQueueHandle, &readData, 0);               // Retrieve the oldest point from the buffer (FIFO)

        tftWriteBuffer(bufferRemaining);                            // Write to the display what the remaining buffer size is

        // Print the latest data to the Serial monitor, the reason this is commented out is that 
        //   at 860Hz serial printing takes too long causing the buffer to slowly fill up

       Serial.print("Buf: ");
       Serial.print(bufferRemaining);
       Serial.print("\tCh: ");
       Serial.print(readData.ch);
       Serial.print("\tVal: ");
       Serial.print(readData.val);
       Serial.print("\tTime: ");
       Serial.println(readData.time);

        tftWriteADC(readData);                                      // Write the ADC data to the display

    } else {    // Buffer is empty
        vTaskDelay(1);                                              // In case the buffer is empty add a small delay to prevent the task from locking up other tasks
    }
}
