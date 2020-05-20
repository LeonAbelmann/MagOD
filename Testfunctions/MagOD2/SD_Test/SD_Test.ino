#include <SPI.h>
#include <Wire.h>   
#include <SD.h>

#define chipSelect 15 //SDcard
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
  Serial.println("SD_Test.ino");
  Serial.println("Initializing card ..");
  
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

  /* Initialize file */
  writeFile(SD, "/hello.txt", "Hello ");
}


void loop()
{
}
