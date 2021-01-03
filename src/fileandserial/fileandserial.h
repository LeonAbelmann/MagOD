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
  char fName_char[fN_len]; //Array to keep filename (including directory name!)
  char dirName_char[fN_len]; //Array to keep directory name
  //String fName_str; //Filename
  int file_number;
  int dir_number;
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
  
  /* finds a directory name that has not been used on the microSD card */
  void setDirName(char dirName_char[]);
  /* set the first file name in that directory */
  void setFileName(char fName_char[]);
  /* increase the file number  of that file name f42_1, f42_2 etc.*/
  File newDataFile(File datfile);
  /* Save the settings file in the data directory */
  void saveSettingsFile(char fName_char[]);
  
  /* Init the data file (write header) */
  File file_init(struct references Vref,
		 bool ref_all_wavelength,
		 bool save_extra_parameter,
		 double extra_par, uint16_t program_cnt,
		 screen thescreen);
  
  /* Save line of data to file */
  void saveLine(File datfile,dataPoint data,
		int LED_type,int Looppar_1);

  /* reset counters */
  void file_reset(); 

  /* Save line of data to file */
  /* void saveToFile(char fName_char[],
		  unsigned long time_of_data_point,
		  diodes Vdiodes,
		  double Temperature,
		  double OD,
		  int LED_type,
		  int Looppar_1,
		  feedbacks Vfb); */
  
  //Send file over serial port. KAN WEG LEON
  void sendFileToSerial(char fName_char[]);
  
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
