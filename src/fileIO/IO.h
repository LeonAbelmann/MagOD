#ifndef IO_h
#define IO_h

#include <cstring>
#include <stdio.h>

#if defined(stdioVersion)
/*IO for command line on PC */
#warning "Compiling for command line stdio"
#include<fstream>
using namespace std;

#elif defined(ESP_PLATFORM)
/*IO for serial port on Arduino */
//#warning "Compiling for ESP32 serial"
#include <Arduino.h>
#include <SD.h>
#endif

#define SD_CS   15 //Pin for SD card.


class IO {
 private:
/*   // Quick fix, REMOVE LATER!!! LEON */
/* #if defined(stdioVersion) */
/*   ifstream recipeFile_; // Global variable to remember the file handle */
/* #elif defined(ESP_PLATFORM) */
/*   File recipeFile; */
/* #endif */
  
 public:
  IO(); //constructor
  IO(char * ); //constructor
  /* Initialize serial communication */
  void initSerial();
  char my_file[20];
  /* Message to serial monitor */
  void serialPrint(float message);
  void serialPrintln(float message);
  void serialPrintch(char message);
  void serialPrint(char* message);
  void serialPrintln(char* message);
  /* Check for presence of file */
  bool checkFile(File recipeFile);
  /* Check if recipefile still has on more character */
  bool recipeFileavailable(File recipeFile);
  /* Read next character from recipe file */
  char recipeFileread(File recipeFile);
  /* Close the recipefile */
  void recipeFileclose(File recipeFile);
};

#endif

