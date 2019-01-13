/* file.cpp
 MagOD2 libary 
 Oct 2018
 File IO
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "Arduino.h"
#include "fileandserial.h"

//Constructor
fileandserial::fileandserial(){
};

/* finds a file name that has not been used on the microSD card */
void fileandserial::setFileName(){
  int i=0;  
  //Keeps looping until found
  while (true){
    fName_str = "f" + (String)i + ".csv";
    fName_str.toCharArray(fName_char, fN_len);
    //prints for debugging
    //Serial.print("Filename = ");
    //Serial.println(fName_char);
    
    if (!SD.exists(fName_char)){
      file_number = i;
      return;
    }
    i++;
  }
}

//Saves a settings file with the settings of the current program
void fileandserial::saveSettingsFile(){

  //Create filename for settings
  String fName_settings_str = "f" + (String)file_number + "_set.csv";
  char fName_settings_char[fN_len];
  fName_settings_str.toCharArray(fName_settings_char, fN_len);
  //Serial.println(fName_settings_str);
  //Serial.println(fName_settings_char);

  /*char testchar[8] = "test.csv";
  dataFile = SD.open(testchar, FILE_WRITE);
  dataFile.print("SomeTextstuff");
  dataFile.close();*/

  //Create file and save settings data
  dataFile = SD.open(fName_settings_char, FILE_WRITE);
  dataFile.println("B_x[mT],B_y[mT],B_z[mT],T_switch[ms],Grad_x,Grad_y,Grad_z,B_nr_set,LED_sw_cy,Nr_cy");
  dataFile.print(B_arrayfield_x[0]); dataFile.print(",");
  dataFile.print(B_arrayfield_y[0]); dataFile.print(",");
  dataFile.print(B_arrayfield_z[0]); dataFile.print(",");
  dataFile.print(Switching_time[0]); dataFile.print(",");
  dataFile.print(Gradient_x[0]); dataFile.print(",");
  dataFile.print(Gradient_y[0]); dataFile.print(",");
  dataFile.print(Gradient_z[0]); dataFile.print(",");
  dataFile.print(B_nr_set); dataFile.print(",");
  dataFile.print(LED_switch_cycles); dataFile.print(",");
  dataFile.print(Nr_cycles); dataFile.print(",");
  for (int i=1; i<B_nr_set; i++){
    dataFile.print(B_arrayfield_x[i]); dataFile.print(",");
    dataFile.print(B_arrayfield_y[i]); dataFile.print(",");
    dataFile.print(B_arrayfield_z[i]); dataFile.print(",");
    dataFile.print(Switching_time[i]); dataFile.print(",");
    dataFile.print(Gradient_x[i]); dataFile.print(",");
    dataFile.print(Gradient_y[i]); dataFile.print(",");
    dataFile.println(Gradient_z[i]);;
  }
  dataFile.close();

  // TODO: send this file over serial
}


//void sendFileToSerial(char fName_char[fN_len]) {
void fileandserial::sendFileToSerial(char fName_char[]) {
  File dataFile = SD.open(fName_char);
  // if the file is available, write it to the serial port:
  if (dataFile) {
    // Announce file is coming, and Write filename on first line
    Serial.print("$FILE: ");
    // Than write the data
    Serial.println(fName_char);
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

void fileandserial::file_init(struct references Vref, bool ref_all_wavelength, bool save_extra_parameter, double extra_par, uint16_t program_cnt, screen thescreen)
//Prepare file for saving data
{   //calls fucntion to valid filename
    thescreen.updateFILE("Choosing name");
    setFileName();
    thescreen.updateFILE(fName_char);
    
    //Save settings file
    Serial.print("Saving settings file ...");
    saveSettingsFile();
    Serial.println("Done");   
    
    //Open data file and write headers
    Serial.print("Saving data file ...");
    dataFile = SD.open(fName_char, FILE_WRITE);
    dataFile.print("Time[ms],Vup[V],Vdwn[V],Vav[V],Vled[V],Vscat[V],OD[],Temp[C],Color[],Output[],Vref=");
    dataFile.print(Vref.Vref,5);
    //store also all references for the specific colors if required
    if (ref_all_wavelength == 1)
        {
          dataFile.print(",Vrefred=");
          dataFile.print(Vref.Vred,5);
          dataFile.print(",Vrefgreen=");
          dataFile.print(Vref.Vgreen,5);
          dataFile.print(",Vrefblue=");
          dataFile.print(Vref.Vblue,5);
        }
        //save extra parameter if required
    if (save_extra_parameter == 1)
    {
      dataFile.print(",extrapar=");
      dataFile.print(extra_par,5);
    }
    dataFile.print(",Prg=");
    dataFile.println(program_cnt);
    dataFile.close();
    Serial.println("Done");  

}

// How to write the datafile lines. Used for SD card as well as serial
// Call with writeDataLine(Serial) or
//           writeDataLine(dataFile)
void fileandserial::writeDataLine(Stream &file)
{
  file.print(time); file.print(",");
  file.print(Vup,5); file.print(",");
  file.print(Vdwn,5); file.print(",");
  file.print(Vav,5); file.print(",");
  file.print(Vled,5); file.print(",");
  file.print(Vscat,5); file.print(",");
  file.print(OD,5); file.print(",");
  file.print(Temperature_degrees,4); file.print(",");
  file.print(LED_type); file.print(",");
  file.println(Looppar_1);
}
