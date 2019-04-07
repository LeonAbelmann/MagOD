#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include <Wire.h>   
#include <SD.h>
#include <Adafruit_RA8875.h>

#define USE_RA8875_TOUCH

#define RA8875_INT 2 
#define RA8875_CS 5 
#define RA8875_RESET 4

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS,RA8875_RESET);

#define chipSelect 15 //SDcard
int i;

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

  
  i=0;
}

//int A;

void loop()
{ Serial.println(i);
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
  //Write black square
  tft.fillRect(0,0,10+i,10+i, RA8875_BLACK);
  //Check touchscreen
  uint16_t a,b;
  if(tft.touched())
    {
      tft.touchRead(&a,&b);
      Serial.print(a);Serial.print(" : ");Serial.println(b);
    }
  
  tft.textSetCursor(i, 100);
  tft.textTransparent(RA8875_BLACK);
  tft.textWrite("MagOD 2");
  i=i+1;
  tft.textSetCursor(i, 100);
  tft.textTransparent(RA8875_WHITE);
  tft.textWrite("MagOD 2");
  delay(1000);
  if(i>480) i=0;
 /* A++; */
 /* Serial.println(A); */
 /* delay(500); */
}

// This function opens a Windows Bitmap (BMP) file and
// displays it at the given coordinates.  It's sped up
// by reading many pixels worth of data at a time
// (rather than pixel by pixel).  Increasing the buffer
// size takes more of the Arduino's precious RAM but
// makes loading a little faster.  20 pixels seems a
// good balance.

/* #define BUFFPIXEL 20 */

/* void bmpDraw(char *filename, int x, int y) { */
/*   SdFile   bmpFile; */
/*   int      bmpWidth, bmpHeight;   // W+H in pixels */
/*   uint8_t  bmpDepth;              // Bit depth (currently must be 24) */
/*   uint32_t bmpImageoffset;        // Start of image data in file */
/*   uint32_t rowSize;               // Not always = bmpWidth; may have padding */
/*   uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel in buffer (R+G+B per pixel) */
/*   uint16_t lcdbuffer[BUFFPIXEL];  // pixel out buffer (16-bit per pixel) */
/*   uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer */
/*   boolean  goodBmp = false;       // Set to true on valid header parse */
/*   boolean  flip    = true;        // BMP is stored bottom-to-top */
/*   int      w, h, row, col; */
/*   uint8_t  r, g, b; */
/*   uint32_t pos = 0, startTime = millis(); */
/*   uint8_t  lcdidx = 0; */
/*   boolean  first = true; */

/*   if((x >= tft.width()) || (y >= tft.height())) return; */

/*   Serial.println(); */
/*   Serial.print(F("Loading image '")); */
/*   Serial.print(filename); */
/*   Serial.println('\''); */

/*   // Open requested file on SD card */
/*   if (!bmpFile.open(filename, O_READ)) { */
/*     Serial.println(F("File not found")); */
/*     return; */
/*   } */

/*   // Parse BMP header */
/*   if(read16(bmpFile) == 0x4D42) { // BMP signature */
/*     Serial.println(F("File size: "));  */
/*     Serial.println(read32(bmpFile)); */
/*     (void)read32(bmpFile); // Read & ignore creator bytes */
/*     bmpImageoffset = read32(bmpFile); // Start of image data */
/*     Serial.print(F("Image Offset: "));  */
/*     Serial.println(bmpImageoffset, DEC); */

/*     // Read DIB header */
/*     Serial.print(F("Header size: "));  */
/*     Serial.println(read32(bmpFile)); */
/*     bmpWidth  = read32(bmpFile); */
/*     bmpHeight = read32(bmpFile); */

/*     if(read16(bmpFile) == 1) { // # planes -- must be '1' */
/*       bmpDepth = read16(bmpFile); // bits per pixel */
/*       Serial.print(F("Bit Depth: "));  */
/*       Serial.println(bmpDepth); */
/*       if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed */
/*         goodBmp = true; // Supported BMP format -- proceed! */
/*         Serial.print(F("Image size: ")); */
/*         Serial.print(bmpWidth); */
/*         Serial.print('x'); */
/*         Serial.println(bmpHeight); */

/*         // BMP rows are padded (if needed) to 4-byte boundary */
/*         rowSize = (bmpWidth * 3 + 3) & ~3; */

/*         // If bmpHeight is negative, image is in top-down order. */
/*         // This is not canon but has been observed in the wild. */
/*         if(bmpHeight < 0) { */
/*           bmpHeight = -bmpHeight; */
/*           flip      = false; */
/*         } */

/*         // Crop area to be loaded */
/*         w = bmpWidth; */
/*         h = bmpHeight; */
/*         if((x+w-1) >= tft.width())  w = tft.width()  - x; */
/*         if((y+h-1) >= tft.height()) h = tft.height() - y; */

/*         // Set TFT address window to clipped image bounds */

/*         for (row=0; row<h; row++) { // For each scanline... */
/*           // Seek to start of scan line.  It might seem labor- */
/*           // intensive to be doing this on every line, but this */
/*           // method covers a lot of gritty details like cropping */
/*           // and scanline padding.  Also, the seek only takes */
/*           // place if the file position actually needs to change */
/*           // (avoids a lot of cluster math in SD library). */
/*           if(flip) // Bitmap is stored bottom-to-top order (normal BMP) */
/*             pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize; */
/*           else     // Bitmap is stored top-to-bottom */
/*           pos = bmpImageoffset + row * rowSize; */
/*           if(bmpFile.curPosition() != pos) { // Need seek? */
/*             bmpFile.seekSet(pos); */
/*             buffidx = sizeof(sdbuffer); // Force buffer reload */
/*           } */

/*           for (col=0; col<w; col++) { // For each column... */
/*             // Time to read more pixel data? */
/*             if (buffidx >= sizeof(sdbuffer)) { // Indeed */
/*               // Push LCD buffer to the display first */
/*               if(lcdidx > 0) { */
/*                 tft.drawPixel(col+x, row+y, lcdbuffer[lcdidx]); */
/*                 lcdidx = 0; */
/*                 first  = false; */
/*               } */

/*               bmpFile.read(sdbuffer, sizeof(sdbuffer)); */
/*               buffidx = 0; // Set index to beginning */
/*             } */

/*             // Convert pixel from BMP to TFT format */
/*             b = sdbuffer[buffidx++]; */
/*             g = sdbuffer[buffidx++]; */
/*             r = sdbuffer[buffidx++]; */
/*             lcdbuffer[lcdidx] = color565(r,g,b); */
/*             tft.drawPixel(col+x, row+y, lcdbuffer[lcdidx]); */
/*           } // end pixel */

/*         } // end scanline */

/*         // Write any remaining data to LCD */
/*         if(lcdidx > 0) { */
/*           tft.drawPixel(col+x, row+y, lcdbuffer[lcdidx]); */
/*         }  */

/*         Serial.print(F("Loaded in ")); */
/*         Serial.print(millis() - startTime); */
/*         Serial.println(" ms"); */

/*       } // end goodBmp */
/*     } */
/*   } */

/*   bmpFile.close(); */
/*   if(!goodBmp) Serial.println(F("BMP format not recognized.")); */

/* } */

/* // These read 16- and 32-bit types from the SD card file. */
/* // BMP data is stored little-endian, Arduino is little-endian too. */
/* // May need to reverse subscript order if porting elsewhere. */

/* uint16_t read16(SdFile& f) { */
/*   uint16_t result; */
/*   ((uint8_t *)&result)[0] = f.read(); // LSB */
/*   ((uint8_t *)&result)[1] = f.read(); // MSB */
/*   return result; */
/* } */

/* uint16_t read32(SdFile& f) { */
/*   uint32_t result; */
/*   ((uint8_t *)&result)[0] = f.read(); // LSB */
/*   ((uint8_t *)&result)[1] = f.read(); */
/*   ((uint8_t *)&result)[2] = f.read(); */
/*   ((uint8_t *)&result)[3] = f.read(); // MSB */
/*   return result; */
/* } */

/* uint16_t color565(uint8_t r, uint8_t g, uint8_t b) { */
/*   return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3); */
/* } */

/* byte decToBcd(byte val){ */
/*   // Convert normal decimal numbers to binary coded decimal */
/*   return ( (val/10*16) + (val%10) ); */
/* } */