#include <stdio.h> // For printing output on commandline
// This does not work. recipes depends on SD.h, which is native Arduino:
//#include "../src/recipes/recipes.h"

// Print to commandline, to mimic Serial.print
void serialPrint(const char* message) {
  printf("%s",message);
}

// Print to commandline, to mimic Serial.println (add linefeed)
void serialPrintln(const char* message) {
  printf("%s\n",message);
}


int main(){
  serialPrintln("TestRecipes, expects RECIPES.CSV in current directory");

  // This cone is copied from ReaRecipes.cpp:
  // 	   // Setup the SD Card

// 	 // see if the card is present and can be initialized
// 	 if (!SD.begin(SD_CS)) {
//     Serial.println("SD Card not found");
//   }
//   else
//   {
//     Serial.println("SD Card Ready");

//     /* Get some info about the card */
//     uint8_t cardType = SD.cardType();
//     if(cardType == CARD_NONE)
//       {
//         Serial.println("Card Type not recognized");
//         return;
//       }

//     Serial.print("SD Card Type: ");
//     if(cardType == CARD_MMC){
//         Serial.println("MMC");
//     } else if(cardType == CARD_SD){
//         Serial.println("SDSC");
//     } else if(cardType == CARD_SDHC){
//         Serial.println("SDHC");
//     } else {
//         Serial.println("UNKNOWN");
//     }

//     uint64_t cardSize = SD.cardSize() / (1024 * 1024);
//     Serial.printf("SD %lluMB\n", cardSize);

//     /* Test if file can actually be opened for writing */
//     File dataFile = SD.open("/test.txt", FILE_WRITE);
//     if(!dataFile)
//       {
//         Serial.println("Failed to open file for writing");
//         return;
//       }
//     if(dataFile.print("Hello world"))
//       {
//         Serial.println("test.txt written");
//       } else
//       {
//         Serial.println("Write test.txt failed");
//       }
//     dataFile.close();
    

//     /* Test if recipes file can be read */
//     File recipeFile = SD.open("/RECIPES.CSV");
//     if(!recipeFile)
//       {
// 	Serial.println("Failed to open file for writing recipes file");
// 	return;
//       }
//     /* Read file contents character by character and display on serial
//        monitor */
//     // while(recipeFile.available())
//     //   {
//     // 	Serial.write(recipeFile.read());
//     //   }
//     // recipeFile.close();
    
//     /* Read Recipes from file */
//     int numRecipes= myrecipes.LoadRecipes(recipeFile,myrecipes.recipes_array);
//     Serial.print("Number of recipes found : ");
//     Serial.println(numRecipes);
//   }
// }	 
}

