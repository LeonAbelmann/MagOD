/* file.h
 MagOD2 libary 
 Oct 2018
 File IO
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
#ifndef fileandserial_h
#define fileandserial_h

#include <SD.h>

#include "../../MagOD.h" //Globals

class fileandserial
{
 public:
  fileandserial(); //constructor
  // File name parameters. TODO, do all of these really need to be public? LEON
  static const int fN_len = 17; //Max filename length
  char fName_char[fN_len]; //Array to keep filename
  //String fName_str; //Filename
  File dataFile;
  int file_number;
  int SD_file_length_count = 0;
  int SD_file_number_count = 1;
  int SD_file_length_count_max = 1000;     //Approximate of datalines in a datafile. The program will finish a sequence, so the actual number of lines could be B_nr_set longer. The files should not become too long as this will take longer to save so after the file has reached this length, a new file is made with the origional name added with: "_i" where i is 1,2,3,4,5....

  /* SD2Card only defined for Arduino's */
#if defined(_MAGOD1)
  //Define SD card
  #define SD_CS      4 //Pin for SD card
  Sd2Card card;
#elif defined(_MAGOD2)
  #define SD_CS      15 //Pin for SD card.
#endif

  /* SD.h for Arduino does not discriminate between write and append? */
#if defined(_MAGOD1)
#define FILE_APPEND FILE_WRITE
#elif defined(_MAGOD2)
#endif

  
  /* finds a base file name that has not been used on the microSD card */
  void setFileName(char fName_char[]);

  /* increase the file number f42_1, f42_2 etc.*/
  void updateFileName(char fName_char[]);
  
  //Saves a settings file with the settings of the current program
  void saveSettingsFile(char fName_char[]);

  //Write header to datafile
  void writeHeader(char fName_char[]);
  
  //Send file over serial port
  void sendFileToSerial(char fName_char[]);

  //Init the data file (write header)
  void file_init(struct references Vref,
		 bool ref_all_wavelength,
		 bool save_extra_parameter,
		 double extra_par, uint16_t program_cnt,
		 screen thescreen);

  void file_reset(); //Reset counters

  /* Save line of data to file */
  void saveToFile(char fName_char[],
		  unsigned long time_of_data_point,
		  diodes Vdiodes,
		  double Temperature,
		  double OD,
		  int LED_type,
		  int Looppar_1,
		  feedbacks Vfb);
   
 private:
  
  /* Write a single line of data */
  
  void writeDataLine(Stream &file,
		     unsigned long time_of_data_point,
		     diodes Vdiodes,
		     double Temperature,
		     double OD,
		     int LED_type,
		     int Looppar_1,
		     feedbacks Vfb);
};

#endif
