/******************************************************************
 This is an example for the Adafruit RA8875 Driver board for TFT displays
 ---------------> http://www.adafruit.com/products/1590
 The RA8875 is a TFT driver for up to 800x480 dotclock'd displays
 It is tested to work with displays in the Adafruit shop. Other displays
 may need timing adjustments and are not guanteed to work.
 
 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source hardware
 by purchasing products from Adafruit!
 
 Written by Limor Fried/Ladyada for Adafruit Industries.
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.
 ******************************************************************/


#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"

#define RA8875_INT 16
#define RA8875_CS 5
#define RA8875_RESET 17

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);
uint16_t tx, ty;

void setup() 
{
  Serial.begin(9600);

   Serial.println("RA8875 start");

  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  
  while (!tft.begin(RA8875_480x272)) {
    Serial.println("RA8875 Not Found!");
    delay(200);
  }
  
  Serial.println("RA8875 found");
  
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK);

  /* Switch to text mode */  
  tft.textMode();
  
 }

void loop() 
{
    /* Set the cursor location (in pixels) */
  tft.textSetCursor(10, 10);
  
  /* Render some text! */
  char string[15] = "Hello, World! ";
  tft.textTransparent(RA8875_WHITE);
  tft.textWrite(string);

  // tft.textColor(RA8875_WHITE, RA8875_RED);
  // tft.textWrite(string);
  // tft.textTransparent(RA8875_CYAN);
  // tft.textWrite(string);
  // tft.textTransparent(RA8875_GREEN);
  // tft.textWrite(string);
  // tft.textColor(RA8875_YELLOW, RA8875_CYAN);
  // tft.textWrite(string);
  // tft.textColor(RA8875_BLACK, RA8875_MAGENTA);
  // tft.textWrite(string);

  // /* Change the cursor location and color ... */  
  // tft.textSetCursor(100, 100);
  // tft.textTransparent(RA8875_RED);
  // /* If necessary, enlarge the font */
  // tft.textEnlarge(1);
  // /* ... and render some more text! */
  // tft.textWrite(string);
  // tft.textSetCursor(100, 150);
  // tft.textEnlarge(2);
  // tft.textWrite(string);
  delay(1000);

}
