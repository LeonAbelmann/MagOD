#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h> // Communication
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_RA8875.h> // TFTM50 screen
#include <Wire.h> //?
#include <SD.h> //SD Card library
#include <Adafruit_ADS1015.h>  // ADC library

#define USE_RA8875_TOUCH

#define RA8875_INT 16 
#define RA8875_CS 5 
#define RA8875_RESET 17

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS,RA8875_RESET);

#define chipSelect 15 //SDcard

#define ADS1015_ADDRESS_0 (0x48)// 1001 000 (ADDR -> GND)
#define ADS1015_ADDRESS_1 (0x49)// 1001 001 (ADDR -> VDD)
Adafruit_ADS1115 ads0(ADS1015_ADDRESS_0); //adc0-3;
Adafruit_ADS1115 ads1(ADS1015_ADDRESS_1); //adc4-7;



int i;




void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}


void setup () {
  Serial.begin(115200);
  delay(1000);
  
  if (!SD.begin(chipSelect))
    {
      Serial.println("SD Card not found");
    }
  else
    {
      uint8_t cardType = SD.cardType();

      if(cardType == CARD_NONE){
        Serial.println("Card Type not recognized");
        return;
      }

      Serial.print("SD Card Type: ");
      if(cardType == CARD_MMC){
        Serial.println("MMC");
      } else if(cardType == CARD_SD){
        Serial.println("SDSC");
      } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
      } else {
        Serial.println("UNKNOWN");
      }
      
      uint64_t cardSize = SD.cardSize() / (1024 * 1024);
      Serial.printf("SD Card Size: %lluMB\n", cardSize);

        /* Initialize file */
      writeFile(SD, "/hello.txt", "Hello ");
      
    }

  Serial.println("Trying to find RA8875 screen...");
  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_480x272)) {
    Serial.println("RA8875 Not Found!");
   while (1);
  }
  Serial.println("RA8875 Found");

  // Initiliaze display
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLUE);
  tft.textMode();

  //Initialize touch
  Serial.println("Enable the touch screen");
  pinMode(RA8875_INT, INPUT);
  digitalWrite(RA8875_INT, HIGH);
  tft.touchEnable(true);
  /* Clear any previous interrupts to avoid false buffered reads */
  uint16_t x, y;
  tft.touchRead(&x, &y);
  delay(1);

  /* Initialize ADC */
  ads0.setGain(GAIN_ONE);
  ads0.begin();
  ads1.setGain(GAIN_ONE);
  ads1.begin();

  i=0;
}

void loop()
{  uint16_t adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7; //Bit values of adc
  char string[15]; //Buffer holding string for dtostrf
  i=i+1;
  Serial.println(i);

  //Check touchscreen
  uint16_t a,b;
  if(tft.touched())
    {
      tft.touchRead(&a,&b);
      Serial.print(a);Serial.print(" : ");Serial.println(b);
    }

  // /* Read ADC1115 ports */
  // /* ADC 0 */
  adc0=double(ads0.readADC_SingleEnded(0));
  Serial.print("adc0: ");
  Serial.println(adc0);
  
  adc1=double(ads0.readADC_SingleEnded(1));
  Serial.print("adc1: ");
  Serial.println(adc1);
  
  adc2=double(ads0.readADC_SingleEnded(2));
  Serial.print("adc2: ");
  Serial.println(adc2);
  
  adc3=double(ads0.readADC_SingleEnded(3));
  Serial.print("adc3: ");
  Serial.println(adc3);

  // /* ADC 1 */
  adc4=double(ads1.readADC_SingleEnded(0));
  Serial.print("adc4: ");
  Serial.println(adc4);
  
  adc5=double(ads1.readADC_SingleEnded(1));
  Serial.print("adc5: ");
  Serial.println(adc5);
  
  adc6=double(ads1.readADC_SingleEnded(2));
  Serial.print("adc6: ");
  Serial.println(adc6);
  
  adc7=double(ads1.readADC_SingleEnded(3));
  Serial.print("adc7: ");
  Serial.println(adc7);
  
  /* Write to TFT screen */

  //Write black square to erase
  tft.fillRect(40,0,40,120, RA8875_BLACK);   
  tft.textTransparent(RA8875_WHITE);
  
  //Write text
  tft.textSetCursor(1, 1);
  tft.textWrite("ADC0: ");
  tft.textSetCursor(40, 1);
  tft.textWrite(dtostrf(adc0,5,0,string),6);

  tft.textSetCursor(1, 15);
  tft.textWrite("ADC1: ");
  tft.textSetCursor(40, 15);
  tft.textWrite(dtostrf(adc1,5,0,string),6);

  tft.textSetCursor(1, 30);
  tft.textWrite("ADC2: ");
  tft.textSetCursor(40, 30);
  tft.textWrite(dtostrf(adc2,5,0,string),6);

  tft.textSetCursor(1, 45);
  tft.textWrite("ADC3: ");
  tft.textSetCursor(40, 45);
  tft.textWrite(dtostrf(adc3,5,0,string),6);

  tft.textSetCursor(1, 60);
  tft.textWrite("ADC4: ");
  tft.textSetCursor(40, 60);
  tft.textWrite(dtostrf(adc4,5,0,string),6);

  tft.textSetCursor(1, 75);
  tft.textWrite("ADC5: ");
  tft.textSetCursor(40, 75);
  tft.textWrite(dtostrf(adc5,5,0,string),6);

  tft.textSetCursor(1, 90);
  tft.textWrite("ADC6: ");
  tft.textSetCursor(40, 90);
  tft.textWrite(dtostrf(adc6,5,0,string),6);

  tft.textSetCursor(1, 105);
  tft.textWrite("ADC7: ");
  tft.textSetCursor(40, 105);
  tft.textWrite(dtostrf(adc7,5,0,string),6);

  delay(1000);

  appendFile(SD, "/hello.txt", "a");
}

