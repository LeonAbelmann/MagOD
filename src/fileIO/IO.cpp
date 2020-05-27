#include "IO.h"

/* ***************************************************************** */

/* IO functions for command line on PC */

//Constructor
IO::IO(char * fname){
	strcpy(my_file, fname);
	#if defined(stdioVersion)
		recipeFile_.open(fname, ios::in);
	#elif defined (ESP_PLATFORM)
	//Nothing to do
	recipeFile = SD.open(fname, FILE_READ);
	#endif
}

void IO::initSerial(){
	#if defined(stdioVersion)
		//#warning "stdioVersion initSerial"
	#elif defined (ESP_PLATFORM)	
		//#warning "SD version initSerial"
		Serial.begin(115200);
		while (!Serial) {
			; // wait for serial port to connect. Needed for native USB port only
		}
		//Serial.println(SD_CS);
		if (!SD.begin(SD_CS)) {
			Serial.println("SD Card not found");
		}
		else
		{
			Serial.println("SD Card Ready");
	
			/* Get some info about the card */
			uint8_t cardType = SD.cardType();
			if(cardType == CARD_NONE)
			{
				Serial.println("Card Type not recognized");
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
			if(!dataFile)
			{
				Serial.println("Failed to open file for writing");
			}
			if(dataFile.print("Hello world"))
			{
				Serial.println("test.txt written");
			} else
			{
				Serial.println("Write test.txt failed");
			}	
			dataFile.close();
		}
	#endif
}

void IO::serialPrint(float message) {
	#if defined(stdioVersion)
		//#warning "Stdio version serialPrint"
		printf("%.1f",message);
	#elif defined(ESP_PLATFORM)
		//#warning "SD version serialPrint"
		Serial.print(message,1);
	#endif
}

void IO::serialPrintln(float message) {
	#if defined(stdioVersion)
		//#warning "Stdio version serialPrintln"
		printf("%.1f\n",message);
	#elif defined(ESP_PLATFORM)
		//#warning "SD version serialPrintln"
		Serial.println(message,1);
	#endif
}

void IO::serialPrintch(char message){
	#if defined(stdioVersion)	
		//#warning "Stdio version serialPrint"
		printf("%c",message);
	#elif defined(ESP_PLATFORM)
		//#warning "SD version serialPrint"
		Serial.print(message);
	#endif
}

void IO::serialPrint(char * message) {
	#if defined(stdioVersion)	
		//#warning "Stdio version serialPrint"
		printf("%s",message);
	#elif defined(ESP_PLATFORM)
		//#warning "SD version serialPrint"
		Serial.print(message);
	#endif
}

void IO::serialPrintln(char * message) {
	#if defined(stdioVersion)
		//#warning "Stdio version serialPrintln"
		printf("%s\n",message);
	#elif defined(ESP_PLATFORM)
		//#warning "SD version serialPrintln"
		Serial.println(message);
	#endif
}

/* Test if recipes file is present */
bool IO::checkFile() {
  // recipeFile is global to IO.h
	bool  check_flag = false;
	#if defined(stdioVersion)
		//#warning "stdioVersion checkFile"
		//recipeFile_.open("RECIPES1.CSV", ios::in);
		if (recipeFile_.good()) {
			//serialPrintln((char*)"****File found****"); 
			check_flag = true;
		}
		else {
			//serialPrintln((char*)"****File not found****");
			check_flag = false;
		}
	#elif defined(ESP_PLATFORM)
		//#warning "SD version checkFile"
		//recipeFile = SD.open("/RECIPES.CSV", FILE_READ); 
		if (recipeFile) {
			//serialPrintln((char*)"****File checked****"); 
			check_flag = true;
		}
		else {
			//serialPrintln((char*)"****File not found****");
			check_flag = false;
		}
	#endif
	return check_flag;
}


/* Check if recipefile still has on more character */
bool IO::recipeFileavailable() {
	bool recipe_flag = false;
	#if defined(stdioVersion)
		//#warning "stdioVersion recipeFileavailable"
		recipe_flag = recipeFile_.is_open() && !recipeFile_.eof();
	#elif defined(ESP_PLATFORM)
		//#warning "SD version recipeFileavailable"
		recipe_flag = recipeFile.available();
	#endif
	return recipe_flag;
}


/* Read next character from recipe file */
char IO::recipeFileread(){
	char c;
	#if defined(stdioVersion)
		//#warning "stdioVersion recipeFileread"
		recipeFile_.get(c);
	#elif defined(ESP_PLATFORM)
		//#warning "SD version recipeFileread"
		c = recipeFile.read();
	#endif
	return c;
}

/* Close the recipefile */
void IO::recipeFileclose(){
  // THIS NEED TO BE DONE:
	#if defined(stdioVersion)
		//#warning "stdioVersion recipeFileclose"
		recipeFile_.close();
	#elif defined(ESP_PLATFORM)
		//#warning "SD version recipeFileclose"
		recipeFile.close();
	#endif	
}


