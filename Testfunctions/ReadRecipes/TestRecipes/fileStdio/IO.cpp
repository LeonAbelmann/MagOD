/* IO functions for command line on PC */

void IO::initSerial(){
  //Nothing to do
}

void IO::serialPrint(char message[]) {
  cout << message;
}

void IO::serialPrintln(char message[]) {
  cout << message\n;
}

/* Test if recipes file is present */
bool IO::checkFile(char* filename) {
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
bool IO::recipeFileavailable() {
  // THIS NEED TO BE DONE:
  //return recipeFile.available();
}

/* Read next character from recipe file */
char* IO::recipeFileread(){
  // THIS NEED TO BE DONE:
  //return recipeFile.read();
}

/* Close the recipefile */
void IO::recipeFileclose(){
  // THIS NEED TO BE DONE:
  //recipeFile.close();
}
