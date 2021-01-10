/* file.cpp
 MagOD libary 
 Juli 2019
 File IO
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "Arduino.h"
#include "fileandserial.h"

/* We should consider saving the files in directories. LEON */


//Constructor
fileandserial::fileandserial(){
};

void writeHeader(File datfile){
  /* Create file and write header line */
  datfile.print("# Time[ms],Looppar,Vdiode[V],Vled[V],Vscat[V],");
  datfile.println("Temperature[C],Ix[A],Iy[A],Iz[A],LED_type");
  Serial.println("Done");  
}

void writeDataPointInFile (File datfile,
			   dataPoint data,
			   int LED_type,
			   int Looppar_1)
{
  /* Data order see writeHeader() : */
  /* Time, Looppar_1, Vdiode, Vled, Vscat, Temperature, Ix, Iy, Yz,
     LED_type */
  int sn = 4; //Significant numbers for printing ADC voltages
  datfile.print(data.time); datfile.print(",");
  datfile.print(Looppar_1); datfile.print(",");
  //Serial.print("writeDataPointInFile: channel: ");Serial.println(data.channel);
  if (data.channel == LED  ) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == SCAT ) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == DIODE) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == NTC  ) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == IX   ) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == IY   ) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == IZ   ) {datfile.print(data.val,sn);};datfile.print(",");
  datfile.println(LED_type);
}

//Saves a settings file with the settings of the current program
void fileandserial::saveSettingsFile(char fName_char[]){
  //Create filename for settings, like f42/sett.csv
  String fName_settings_str = "/f";
    fName_settings_str = fName_settings_str + dir_number +
    "/sett.csv";
  char fName_settings_char[17];
  fName_settings_str.toCharArray(fName_settings_char, 17);
  //Serial.println(fName_settings_str);
  //Serial.println(fName_settings_char);

  //Create file and save settings data
  File f = SD.open(fName_settings_char, FILE_WRITE);
  if(!f)
    {
      Serial.print(fName_settings_char);
      Serial.println(" failed to open.");
    }
  else
    {
#if defined(_MAGOD1)
      f.println("# Version=MAGOD1");
#elif defined(_MAGOD2)
      f.println("# Version=MAGOD2");
#else
      f.println("# No version defined!");
#endif
      /* I think this can be replaced by something like
	 recipes::serialRecipesPrint */
      f.print("# Vref=");
      f.print(Vrefs.Vref,5);
      //store also all references for the specific colors if required
      if (ref_all_wavelength == 1)
	{
	  f.print(",Vrefred=");
	  f.print(Vrefs.Vred,5);
	  f.print(",Vrefgreen=");
	  f.print(Vrefs.Vgreen,5);
	  f.print(",Vrefblue=");
	  f.print(Vrefs.Vblue,5);
	}
      f.println();
      f.print("# Prg=");f.println(program_cnt);
      f.print("# B_nr_set=");f.println(B_nr_set); 
      f.print("# LED_switch_cycles=");
      f.println(LED_switch_cycles);
      f.print("# Nr_cycles=");f.println(Nr_cycles);
      f.println("");
      f.println("# Recipe: ");
      f.print("seq,B_x[mT],B_y[mT],B_z[mT],T_switch[ms]");
      f.print(", LED_color, LED_intensity");
      f.println(",Grad_x,Grad_y,Grad_z");
      for (unsigned int i=0; i<=B_nr_set; i++){
	f.print(i); f.print(",");
	f.print(B_arrayfield_x[i]); f.print(",");
	f.print(B_arrayfield_y[i]); f.print(",");
	f.print(B_arrayfield_z[i]); f.print(",");
	f.print(Switching_time[i]); f.print(",");
	f.print(LEDColor_array[i]); f.print(",");
	f.print(LEDInt_array[i]); f.print(",");
	f.print(Gradient_x[i]); f.print(",");
	f.print(Gradient_y[i]); f.print(",");
	f.println(Gradient_z[i]);
      }
      Serial.print("Closing: ");Serial.println(fName_settings_char);
      f.close();
    }
}

void fileandserial::saveLine(File datfile,dataPoint data,
			     int LED_type,int Looppar_1){
  if (!datfile)
    {
      Serial.println("saveLine: File failed to open.");
    }
  else
    {
      writeDataPointInFile(datfile,data,LED_type,Looppar_1);
    }
}

void fileandserial::file_reset()
{
  SD_file_number_count = 1;
  SD_file_length_count = 0;
}

File fileandserial::file_init(struct references Vref, bool ref_all_wavelength, bool save_extra_parameter, double extra_par, uint16_t program_cnt, screen thescreen)
//Prepare file for saving data
{   //calls fucntion to valid filename
  char message[] = "Choosing...";
  thescreen.updateFILE(message);
    setFileName(fName_char);
    //Save settings file
    Serial.print("Saving settings file ...");
    saveSettingsFile(fName_char);
    Serial.println("Done");   
    /* Open datafile */
    /* To set the timestamp of the file, we can simply interrogate the
       NTP server using getLocalTime(&timeinfo). But if the MagOD does
       not have an internet connection, that fails. Therefore we set
       the date on boot, either from NTP or artificially to 1.1.2021
       0h00. Than we count the time since boot */
    File datfile = SD.open(fName_char, FILE_WRITE);
    if (datfile) {
      thescreen.updateFILE(fName_char);            
      Serial.print("Starting data file with header...");
      //Write headers
      writeHeader(datfile);
    }
    else {
      Serial.println("Fileandserial::file_init: file could not be opened");
    }
    if (not datfile) {
      Serial.println("Fileandserial::file_init:file is not open anymore!");
    }
    else {
      return datfile;
    }
}

// How to write the datafile lines. Used for SD card as well as serial
// Call with writeDataLine(Serial) or
//           writeDataLine(dataFile)

void fileandserial::writeDataLine(Stream &file,
				  unsigned long time_of_data_point,
				  diodes Vdiodes,
				  double Temperature,
				  double OD,
				  int LED_type,
				  int Looppar_1,
				  feedbacks Vfb)
{
  file.print(time_of_data_point); file.print(",");
#if defined(_MAGOD1)
  file.print(Vdiodes.Vup,5); file.print(",");
  file.print(Vdiodes.Vdwn,5); file.print(",");
#elif defined(_MAGOD2) //For backward compatibility
  file.print(Vdiodes.Vdiode,5); file.print(",");
  file.print(Vdiodes.Vdiode,5); file.print(",");
#endif
  file.print(Vdiodes.Vdiode,5); file.print(",");
  file.print(Vdiodes.Vled,5); file.print(",");
  file.print(Vdiodes.Vscat,5); file.print(",");
  file.print(OD,5); file.print(",");
  file.print(Temperature,4); file.print(",");
  file.print(LED_type); file.print(",");
  file.print(Looppar_1);  file.print(",");
  file.print(Vfb.x);   file.print(",");
  file.print(Vfb.y);   file.print(",");
  file.println(Vfb.z);
}


/* finds a directory name that has not been used on the microSD card */
void fileandserial::setDirName(char dirName_char[]){
  int i=0;
  String dirName_str;
  //Keeps looping until found
  while (true){
    dirName_str = "/f" + (String)i; /* Not sure if MagOD1 understands
				       the /, LEON */
    /* Convert string to char array */
    dirName_str.toCharArray(dirName_char, fN_len);

    //prints for debugging
    Serial.print("Directory name = ");
    Serial.println(dirName_char);
    
    if (!SD.exists(dirName_char)){
      SD.mkdir(dirName_char);
      dir_number = i;
      return;
    }
    i++;
  }
}

/* Creates the first file in a new directory on the SD card */
void fileandserial::setFileName(char fName_char[]){
  char dirName_char[fN_len];
  /* Find an directory name that has not yet been used */
  setDirName(dirName_char);
  /* Create file, with same name as directory. Like /f42/f42_0.csv.
     Not sure if MagOD1 understands the /, LEON */
  String fName_str(dirName_char); // Convert char array to string
  fName_str = fName_str + "/f" + dir_number + "_0.csv"; //Add filename
  fName_str.toCharArray(fName_char, fN_len);
  //prints for debugging
  Serial.print("Filename = ");Serial.println(fName_char);
  file_number = 1;
}

/*when a field sequence has finished, a new file is made with the same name + the addition of "_i" with i = 1,2,3,4,5.... */
File fileandserial::newDataFile(File datfile){
  /* Close the previous datafile */
  Serial.print("Closing : ");Serial.println(fName_char);
  datfile.close();
  /* Create a new filename, like /f42/f42_1.cvs */
  String fName_str = "/f"; 
  fName_str = fName_str + dir_number + "/f" + dir_number +"_" +
	  file_number +".csv";
  fName_str.toCharArray(fName_char, fN_len);
  file_number = file_number + 1;
  Serial.print("New datafile: ");Serial.print(fName_str);
  Serial.print(" : ");Serial.println(fName_char);
  SD_file_number_count = SD_file_number_count +1; // Obsolete? LEON

  /* Open file */
  datfile = SD.open(fName_char, FILE_WRITE);
  if (datfile) {
    //Write headers
    writeHeader(datfile);
  }
  else {
    Serial.println("Fileandserial::updateFileName Datafile not open!");
  }
  return datfile;
}


/* OBSOLETE, LEON: */
//void sendFileToSerial(char fName_char[fN_len]) {
void fileandserial::sendFileToSerial(char fName_char[]) {
  File f = SD.open(fName_char);
  // if the file is available, write it to the serial port:
  if (f) {
    // Announce file is coming, and Write filename on first line
    Serial.print("$FILE: ");
    Serial.println(fName_char);
    /* Send data */
    while (f.available()) {
      Serial.write(f.read());
    }
    // Than announce file is done
    Serial.println("$EOF ");
    f.close();
  }
  // if the file is not available error:
  else {
    Serial.print("error opening ");
    Serial.println(fName_char);
  }
}


/* Write data to SD card */
// void fileandserial::saveToFile(char fName_char[fN_len],
// 			       unsigned long time_of_data_point,
// 			       diodes Vdiodes,
// 			       double Temperature,
// 			       double OD,
// 			       int LED_type,
// 			       int Looppar_1,
// 			       feedbacks Vfb)
// {
  
//   File f = SD.open(fName_char, FILE_APPEND);

//   if (!f)
//     {
//       Serial.print(fName_char);
//       Serial.println(" failed to open.");
//     }
//   else
//     {
//       writeDataLine(f, time_of_data_point, Vdiodes,
// 		    Temperature,OD,LED_type,Looppar_1,Vfb);
//       f.close();
//     }
// }
