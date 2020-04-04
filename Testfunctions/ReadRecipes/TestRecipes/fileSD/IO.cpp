/* IO functions for Arduino */

void initSerial(){
  Serial.begin(9600);
}

void serialPrint(const char* message) {
  Serial.print(message);
}

void serialPrintln(const char* message) {
  Serial.println(message);
}

/* Test if recipes file is present */
bool checkFile(char* filename) {
  recipeFile = SD.open("/RECIPES.CSV"); // recipeFile is global to IO.h
  if recipeFile {
      return true;
    }
  else {
    return false;
  }
}
/* Check if recipefile still has on more character */
bool recipeFileavailable() {
  return recipeFile.available();
}

/* Read next character from recipe file */
char recipeFileread(){
  return recipeFile.read();
}

/* Close the recipefile */
void recipeFileclose(){
  recipeFile.close();
}
