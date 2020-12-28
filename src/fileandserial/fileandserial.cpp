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

/* finds a file name that has not been used on the microSD card */
/* I think we want to make a directory here. LEON */
void fileandserial::setFileName(char fName_char[]){
  int i=0;
  String fName_str;
  //Keeps looping until found
  while (true){
    fName_str = "/f" + (String)i + ".csv"; /* Not sure if MagOD1
					      understands the /,
					      LEON */
    fName_str.toCharArray(fName_char, fN_len);

    //prints for debugging
    Serial.print("Filename = ");
    Serial.println(fName_char);
    
    if (!SD.exists(fName_char)){
      file_number = i;
      return;
    }
    i++;
  }
}



//Saves a settings file with the settings of the current program
void saveSettingsFile(char fName_char[]){

  //Create filename for settings
  /* Temp fix */
  String fName_settings_str = "settings.csv";
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
      f.close();
    }
}

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
  Serial.print("writeDataPointInFile: channel: ");Serial.println(data.channel);
  if (data.channel == LED  ) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == SCAT ) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == DIODE) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == NTC  ) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == IX   ) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == IY   ) {datfile.print(data.val,sn);};datfile.print(",");
  if (data.channel == IZ   ) {datfile.print(data.val,sn);};datfile.print(",");
  datfile.print(",");
  datfile.println(LED_type);
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

void fileandserial::file_reset()
{
  SD_file_number_count = 1;
  SD_file_length_count = 0;
}

File fileandserial::file_init(struct references Vref, bool ref_all_wavelength, bool save_extra_parameter, double extra_par, uint16_t program_cnt, screen thescreen)
//Prepare file for saving data
{   //calls fucntion to valid filename
    thescreen.updateFILE("Choosing name");
    setFileName(fName_char);
    //Save settings file
    Serial.print("Saving settings file ...");
    saveSettingsFile(fName_char);
    Serial.println("Done");   
    /* Open datafile */
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

/*when a field sequence has finished, a new file is made with the same name + the addition of "_i" with i = 1,2,3,4,5.... */
void fileandserial::updateFileName(File datfile){
  String fName_str;
  fName_str = "/f" + (String)file_number + "_" +
	  (String)SD_file_number_count +".csv";
  //kan weg? fName_str.toCharArray(fName_char, fN_len);
  SD_file_number_count = SD_file_number_count +1;
  /* Open file */
  datfile = SD.open(fName_char, FILE_WRITE);
  if (datfile) {
    //Write headers
    writeHeader(datfile);
    Serial.print("New datafile: ");Serial.println(fName_str);
  }
  else {
    Serial.println("Fileandserial::updateFileName Datafile not open!");
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
