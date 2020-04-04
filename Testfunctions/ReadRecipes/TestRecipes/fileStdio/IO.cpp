/* IO functions for command line on PC */

void initSerial(){
  //Nothing to do
}

void serialPrint(const char* message) {
  cout << message;
}

void serialPrintln(const char* message) {
  cout << message\n;
}

/* Test if recipes file is present */
bool checkFile(char* filename) {
  // THIS NEED TO BE DONE:
  //recipeFile = SD.open("/RECIPES.CSV"); // recipeFile is global to IO.h
  if recipeFile {
      return true;
    }
  else {
    return false;
  }
}

/* Check if recipefile still has on more character */
bool recipeFileavailable() {
  // THIS NEED TO BE DONE:
  //return recipeFile.available();
}

/* Read next character from recipe file */
char recipeFileread(){
  // THIS NEED TO BE DONE:
  //return recipeFile.read();
}

/* Close the recipefile */
void recipeFileclose(){
  // THIS NEED TO BE DONE:
  //recipeFile.close();
}
