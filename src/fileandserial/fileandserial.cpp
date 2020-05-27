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

/*when the file length is longer as the maximum length and a field sequence has finished, a new file is made with the same name + the addition of "_i" with i = 1,2,3,4,5.... */
void fileandserial::updateFileName(char fName_char[]){
  String fName_str;
  fName_str = "/f" + (String)file_number + "_" +
	  (String)SD_file_number_count +".csv";
  fName_str.toCharArray(fName_char, fN_len);
  SD_file_number_count = SD_file_number_count +1;
}


//Saves a settings file with the settings of the current program
void fileandserial::saveSettingsFile(char fName_char[]){

  //Create filename for settings
  String fName_settings_str = "/f" + (String)file_number + "_set.csv";
  char fName_settings_char[fN_len];
  fName_settings_str.toCharArray(fName_settings_char, fN_len);
  //Serial.println(fName_settings_str);
  //Serial.println(fName_settings_char);

  //Create file and save settings data
  dataFile = SD.open(fName_settings_char, FILE_WRITE);
  if(!dataFile)
    {
      Serial.print(fName_settings_char);
      Serial.println(" failed to open.");
    }
  else
    {
#if defined(_MAGOD1)
      dataFile.println("# Version=MAGOD1");
#elif defined(_MAGOD2)
      dataFile.println("# Version=MAGOD2");
#else
      datafile.println("# No version defined!");
#endif
    
      dataFile.print("# Vref=");
      dataFile.print(Vrefs.Vref,5);
      //store also all references for the specific colors if required
      if (ref_all_wavelength == 1)
	{
	  dataFile.print(",Vrefred=");
	  dataFile.print(Vrefs.Vred,5);
	  dataFile.print(",Vrefgreen=");
	  dataFile.print(Vrefs.Vgreen,5);
	  dataFile.print(",Vrefblue=");
	  dataFile.print(Vrefs.Vblue,5);
	}
      dataFile.println();
      dataFile.print("# Prg=");dataFile.println(program_cnt);
      dataFile.print("# B_nr_set=");dataFile.println(B_nr_set); 
      dataFile.print("# LED_switch_cycles=");
      dataFile.println(LED_switch_cycles);
      dataFile.print("# Nr_cycles=");dataFile.println(Nr_cycles);
      dataFile.println("");
      dataFile.println("# Recipe: ");
      dataFile.print("seq,B_x[mT],B_y[mT],B_z[mT],T_switch[ms]");
      dataFile.print(", LED_color, LED_intensity");
      dataFile.println(",Grad_x,Grad_y,Grad_z");
      for (unsigned int i=0; i<=B_nr_set; i++){
	dataFile.print(i); dataFile.print(",");
	dataFile.print(B_arrayfield_x[i]); dataFile.print(",");
	dataFile.print(B_arrayfield_y[i]); dataFile.print(",");
	dataFile.print(B_arrayfield_z[i]); dataFile.print(",");
	dataFile.print(Switching_time[i]); dataFile.print(",");
	dataFile.print(LEDColor_array[i]); dataFile.print(",");
	dataFile.print(LEDInt_array[i]); dataFile.print(",");
	dataFile.print(Gradient_x[i]); dataFile.print(",");
	dataFile.print(Gradient_y[i]); dataFile.print(",");
	dataFile.println(Gradient_z[i]);
      }
      dataFile.close();
    }
  //Send settings file to serial port
  Serial.print("Sending settings file ...");
  sendFileToSerial(fName_settings_char);
  Serial.println("Done");   
}

/* Write data to SD card */
void fileandserial::saveToFile(char fName_char[fN_len],
			       unsigned long time_of_data_point,
			       diodes Vdiodes,
			       double Temperature,
			       double OD,
			       int LED_type,
			       int Looppar_1,
			       feedbacks Vfb)
{
  
  File dataFile = SD.open(fName_char, FILE_APPEND);

  if (!dataFile)
    {
      Serial.print(fName_char);
      Serial.println(" failed to open.");
    }
  else
    {
      writeDataLine(dataFile, time_of_data_point, Vdiodes,
		    Temperature,OD,LED_type,Looppar_1,Vfb);
      dataFile.close();
    }
}

//void sendFileToSerial(char fName_char[fN_len]) {
void fileandserial::sendFileToSerial(char fName_char[]) {
  File dataFile = SD.open(fName_char);
  // if the file is available, write it to the serial port:
  if (dataFile) {
    // Announce file is coming, and Write filename on first line
    Serial.print("$FILE: ");
    Serial.println(fName_char);
    /* Send data */
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    // Than announce file is done
    Serial.println("$EOF ");
    dataFile.close();
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

void fileandserial::file_init(struct references Vref, bool ref_all_wavelength, bool save_extra_parameter, double extra_par, uint16_t program_cnt, screen thescreen)
//Prepare file for saving data
{   //calls fucntion to valid filename
    thescreen.updateFILE("Choosing name");
    setFileName(fName_char);
    thescreen.updateFILE(fName_char);
    
    //Save settings file
    Serial.print("Saving settings file ...");
    saveSettingsFile(fName_char);
    Serial.println("Done");   
    
    Serial.print("Starting data file with header...");
    //Write headers
    writeHeader(fName_char);
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


void fileandserial::writeHeader(char fName_char[]){
  /* Create file and write header line */
  File dataFile = SD.open(fName_char, FILE_WRITE);
  dataFile.print("Time[ms],Vup[V],Vdwn[V],Vdiode[V],Vled[V],Vscat[V],");
  dataFile.print("OD[],Temp[C],Color[],LoopCount[],");
  dataFile.println("Vfb_x[V],Vfb_y[V],Vfb_z[V]");
  dataFile.close();
  Serial.println("Done");  
}
