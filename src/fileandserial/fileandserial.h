/* file.h
 MagOD2 libary 
 Oct 2018
 File IO
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
#ifndef fileandserial_h
#define fileandserial_h

#include <SD.h>

#include "../globals/globals.h" //Globals
#include "../screen/screen.h" //To be able to update filename on screen

class fileandserial
{
 public:
  fileandserial(); //constructor
  // File name parameters. TODO, do all of these really need to be public? LEON
  static const int fN_len = 17; //Max filename length
  char fName_char[fN_len]; //Array to keep filename
  String fName_str; //Filename
  File dataFile;
  int file_number;
  int SD_file_length_count = 0;
  int SD_file_number_count = 1;
  int SD_file_length_count_max = 1000;     //Approximate of datalines in a datafile. The program will finish a sequence, so the actual number of lines could be B_nr_set longer. The files should not become too long as this will take longer to save so after the file has reached this length, a new file is made with the origional name added with: "_i" where i is 1,2,3,4,5....

  //Define SD card
  Sd2Card card;
  
  /* finds a file name that has not been used on the microSD card */
  void setFileName();
  //Saves a settings file with the settings of the current program
  void saveSettingsFile();
  //Send file over serial port
  void sendFileToSerial(char fName_char[]);
  
  void file_init(struct references Vref, bool ref_all_wavelength, bool save_extra_parameter, double extra_par, uint16_t program_cnt, screen thescreen);
  
  // How to write the datafile lines. Used for SD card as well as serial
  // Call with writeDataLine(Serial) or
  //           writeDataLine(dataFile)
  void writeDataLine(Stream &file);
  
 private:

};

#endif
