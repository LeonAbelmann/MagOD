/* IO functions for command line on PC */
#include "IO.h"


//Constructor
IO::IO(char * fname){
  //Nothing to do
  recipeFile_.open(fname, ios::in);
}
void IO::initSerial(){
  //Nothing to do
}


void IO::serialPrint(char message[]) {

  printf("%s\n",message);
}

void IO::serialPrintln(char message[]) {
  printf("%s\n",message);
}

/* Test if recipes file is present */
bool IO::checkFile(char* filename) {
  // THIS NEED TO BE DONE:
  //recipeFile = SD.open("/RECIPES.CSV"); // recipeFile is global to IO.h
  if (recipeFile_.good()) {
	  serialPrintln((char*)"File checked");
      return true;
    }
  else {
	  serialPrintln((char*)"File not exists");
    return false;
  }
}

/* Check if recipefile still has on more character */
bool IO::recipeFileavailable() {
  // THIS NEED TO BE DONE:
  return recipeFile_.is_open() && !recipeFile_.eof();
}


/* Read next character from recipe file */
void IO::recipeFileread(char * line){
  // THIS NEED TO BE DONE:
  recipeFile_.getline(line, 150, '\n');
}

/* Close the recipefile */
void IO::recipeFileclose(){
  // THIS NEED TO BE DONE:
  recipeFile_.close();
}
