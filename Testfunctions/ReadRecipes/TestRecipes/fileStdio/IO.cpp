/* IO functions for command line on PC */
#include "IO.h"

//Constructor
IO::IO(){
  //Nothing to do
};

void IO::initSerial(){
  //Nothing to do
}

void IO::serialPrint(char message[]) {
  printf("%s",message);
}

void IO::serialPrintln(char message[]) {
  printf("%s\n",message);
}

/* Test if recipes file is present */
bool IO::checkFile(char* filename) {
  // THIS NEED TO BE DONE:
  //recipeFile = SD.open("/RECIPES.CSV"); // recipeFile is global to IO.h
  if (recipeFile) {
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
  return true;//fix
}

/* Read next character from recipe file */
char* IO::recipeFileread(){
  // THIS NEED TO BE DONE:
  //return recipeFile.read();
  return 0;//fix
}

/* Close the recipefile */
void IO::recipeFileclose(){
  // THIS NEED TO BE DONE:
  //recipeFile.close();
}
