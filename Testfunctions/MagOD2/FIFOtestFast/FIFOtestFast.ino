/*
    Simulate the writing to the SD card on one core at 8Hz and 860Hz, measure the performance
    Have the WiFi accessible SD card on the other core, measure performance
    Run both simulatanuously and measure performance
*/

#include <SD.h>
#include <SPI.h>
#include "freertos/ringbuf.h"

#define measurementFrequency 1000   // In hertz
#define writeBufferSize 512         // In bytes
#define ringBufferSize 32768        // In bytes


char testData[] = "01234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567\r\n";     // dummydata
char bufferData[writeBufferSize+1];      // 1 extra to be able to close off the character stream with a \0
char fileSplit[2];

bool doMeasurementFlag = false;
RingbufHandle_t dataBufferHandle;   // Using the Freertos built in ring buffers

/*
    SD file access
*/

void createDir(const char * path) {
    Serial.printf("[SD] Creating Dir: %s\n", path);
    if (SD.mkdir(path)) {
        Serial.println("[SD] Dir created");
    } else {
        Serial.println("[SD] mkdir failed");
    }
}

void deleteRecursive(String path) {
    File file = SD.open((char *)path.c_str());
    if (!file.isDirectory()) {
        file.close();
        SD.remove((char *)path.c_str());
        Serial.print("[SD] Deleted "); Serial.println(path);
        return;
    }

    file.rewindDirectory();
    while (true) {
        File entry = file.openNextFile();
        if (!entry) {
            break;
        }
        String entryPath = entry.name(); //path + "/" + entry.name();
        if (entry.isDirectory()) {
            entry.close();
            deleteRecursive(entryPath);
        } else {
            entry.close();
            SD.remove((char *)entryPath.c_str());
            Serial.print("[SD] Deleted "); Serial.println(entryPath);
        }
        
        
        yield();
    }

    SD.rmdir((char *)path.c_str());
    Serial.print("[SD] Deleted folder "); Serial.println(path);
    file.close();
}

/*
    Multi core
*/


void TaskMeasurement(void *pvParameters) {
    (void) pvParameters;

    uint16_t measurementCount = 0;
    UBaseType_t res;
    
    for (;;) {
        //loop
        if (doMeasurementFlag == true) {
            doMeasurementFlag = false;

            // Split files every 1000 measurements
            if (measurementCount == 1000) {
                res = xRingbufferSend(dataBufferHandle, fileSplit, sizeof(fileSplit)-1, pdMS_TO_TICKS(1000));   // Add split symbol to ringbuffer, timeout after 1s
                if (res != pdTRUE) {
                    Serial.println("[Buf] Failed to send file split to buffer");
                }
                measurementCount = 0;
            }
            
            //Add to buffer
            res = xRingbufferSend(dataBufferHandle, testData, sizeof(testData)-1, pdMS_TO_TICKS(1000));         // Add data to ringbuffer, timeout after 1s, don't add the \0
            if (res != pdTRUE) {
                Serial.println("[Buf] Failed to send measurement to buffer");
            }

            measurementCount++;

            Serial.println(xRingbufferGetCurFreeSize(dataBufferHandle));                                        // Print currently available space in ringbuffer
        } else {
            vTaskDelay(1);
        }
    }
}

void TaskPeripheral(void *pvParameters) {
    (void) pvParameters;

    File dataFile;
    size_t dataSize;
    uint16_t nFiles = 1;
    String fileName;
    int16_t splitIndex;

    fileName = "/measurementData/data_" + (String)nFiles + ".txt";
    dataFile = SD.open(fileName, FILE_APPEND);

    for (;;) {
        char *retrievedData = (char *)xRingbufferReceiveUpTo(dataBufferHandle, &dataSize, pdMS_TO_TICKS(1000), writeBufferSize);    // Retrieve at most 512 bytes from the ring buffer
        if (retrievedData != NULL){
            
            splitIndex = -1;
            for (int i = 0; i < dataSize; i++){
                bufferData[i] = *(retrievedData + i);                                       // Copy data to the buffer
                if (bufferData[i] == fileSplit[0]) {                                        // See if the file split is in the buffer
                    splitIndex = i;
                }
            }
            bufferData[dataSize] = '\0';                                                    // Terminate the buffer with a \0
            
            if (splitIndex == -1) {                                                         // If no split character was found
                dataFile.print(bufferData);                                                 // Save buffer to the SD card
                vRingbufferReturnItem(dataBufferHandle, (void *)retrievedData);             // Clear the retrieved data from the buffer
                
            } else {                                                                        // If there was a split character                
                bufferData[splitIndex] = '\0';                                              // Replace the split character by a terminator (null character)
                dataFile.print(bufferData);                                                 // Save buffer to the SD card, it is only read until the first null character
                
                dataFile.close();
                nFiles++;
                fileName = "/measurementData/data_" + (String)nFiles + ".txt";
                dataFile = SD.open(fileName, FILE_APPEND);                                  // Open a new file

                dataFile.print(bufferData + splitIndex + 1);                                // Save the remainder of the buffer, everything after the split character
                
                Serial.println("[Buf] Data was read and written to a new file");
                vRingbufferReturnItem(dataBufferHandle, (void *)retrievedData);             // Clear the retrieved data from the buffer
            }

            
        } else {
            Serial.println("Buffer is empty");
            vTaskDelay(1);
        }
    }
}


/* 
 *  See the IRAM_ATTR this makes sure the interrupt can actually run on while an SPI transfer occurs 
 *  in another task due to flash access being blocked by the SPI transfer this function should 
 *  always live in RAM
 */
 
void IRAM_ATTR measEvent() {        
    doMeasurementFlag = true;
}

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.print("\n");

    while(!SD.begin(SS, SPI, 4000000, "/sd", 5)) {
        Serial.println("[SD] No SD card found");
        delay(3000);
    }
    
    Serial.println("[SD] SD Card initialized");

    deleteRecursive("/measurementData");    //Delete old data on the SD card
    createDir("/measurementData");          //Create folder for new data to be stored in

    // Create Ring buffer
    dataBufferHandle = xRingbufferCreate(ringBufferSize, RINGBUF_TYPE_BYTEBUF);
    if(dataBufferHandle == NULL) {
        Serial.println("[Buf] Fialed to create ring buffer");
    }

    
    *(fileSplit) = 0x1C;        // Set the split character to the ASCII equivalent of file seperator
    *(fileSplit + 1) = '\0';    // Terminate the character string
    
    delay(1000);

    // Now set up peripheral Task
    xTaskCreatePinnedToCore(
        TaskPeripheral
        ,  "TaskPeripheral"     // A name just for humans
        ,  4096                 // This stack size can be checked & adjusted by reading the uxTaskGetStackHighWaterMark();
        ,  NULL
        ,  1                    // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,  NULL
        ,  0);                  // Core

    delay(1000);

    // Now set up measurement Task
    xTaskCreatePinnedToCore(
        TaskMeasurement
        ,  "TaskMeasurement"    // A name just for humans
        ,  4096                 // This stack size can be checked & adjusted by reading the uxTaskGetStackHighWaterMark();
        ,  NULL
        ,  1                    // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
        ,  NULL
        ,  1);                  // Core

    delay(1000);

    // Finally finalize setup for the MagOD2 simulator
    hw_timer_t * timer0 = NULL;                                         // Initialize a timer
    timer0 = timerBegin(0, 80, true);                                   // Attach the timer to hardware timer 0 with prescaler 80
    timerAttachInterrupt(timer0, measEvent, true);                      // Attach interrupt routine to the timer if it triggers
    timerAlarmWrite(timer0, 1000000/measurementFrequency, true);        // Set time between interrupts
    timerAlarmEnable(timer0);                                           // Enable timer

    vTaskDelete(NULL);
}

void loop() {
    vTaskDelete(NULL);
}
