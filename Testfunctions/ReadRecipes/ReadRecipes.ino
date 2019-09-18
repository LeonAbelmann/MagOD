/* Read Recipes 0.1 */
/* Sep 2019 */
/* Rutvik, Leon */
/* Test to improve recipe reading from Flash */

/* Class definitions */
#include "ReadRecipes.h"

/* Init classes */
recipes myrecipes;

/* Setup card and load datafile*/
void setup()
{ delay(1000);//Give serial monitor time

  // Setup the SD Card
  // see if the card is present and can be initialized
  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card not found");
  }
  else
  {
    Serial.println("SD Card Ready");

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
    Serial.printf("SD %lluMB\n", cardSize);

    /* Test if file can actually be opened for writing */
    File dataFile = SD.open("/test.txt", FILE_WRITE);
    if(!dataFile){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(dataFile.print("Hello world")){
        Serial.println("test.txt written");
    } else {
        Serial.println("Write test.txt failed");
    }
    dataFile.close();
  }

  /* Read Recipes from file */
  if (myrecipes.LoadRecipes())
    {Serial.println("Recipes loaded fine");}
  else
    {Serial.println("Error loading recipes");}
}

/* Main loop */
void loop()
{
  /* Do nothing anymore, just burn energy*/
}

