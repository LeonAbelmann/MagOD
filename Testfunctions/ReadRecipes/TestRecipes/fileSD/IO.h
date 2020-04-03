#ifndef IO_h
#define IO_h

#include <Arduino.h>
#include <SD.h>

class IO
{
 public:
  IO(); //constructor
  /* Initialize serial communication */
  void initSerial();
  /* Message to serial monitor */
  void serialPrint(const char* message);
  void serialPrintln(const char* message);
  /* Check for presence of file */
  bool checkFile(char* filename);
  /* Check if recipefile still has on more character */
  bool recipeFileavailable();
  /* Read next character from recipe file */
  char recipeFileread();
  /* Close the recipefile */
  void recipeFileclose();

 private:
  File recipeFile; // Global variable to remember the file handle
};

#endif
