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
  // File name parameters. 
  static const int fN_len = 17; //Max filename length
  char fName_char[fN_len]; //Array to keep filename (including directory name!)
  
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
  
  //Send file over serial port. Keep for MagOD1
  void sendFileToSerial(char fName_char[]);
  
 private:
  char dirName_char[fN_len]; //Array to keep directory name
  /* Data is stored in a directory f'dir_number'. A new directory is
     made after each start. Files are named f'file_number'. After each
     sequence a new file is made with the origional name added with:
     "_i" where i is 1,2,3,4,5.... */
  int file_number;
  int dir_number;

  
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
