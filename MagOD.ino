/* MagOD version 2.0 */
/* Jan 2019 */
/* Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann */
/* Based on original code, modified by Leon for readablity and ease of recipe change */

/* General libraries, should be in your global Arduino directory */
#include <TimerOne.h> //Timer1 used for measurement frequency
#include <TimerThree.h> //Timer3 is used to set the current sensing
#include <TimerFour.h> //Timer4 used for the screen update frequency
#include <TimerFive.h> //This timer is not used, set to zero not to disturb the other timers.
//#include <MemoryFree.h> //For debugging only

/* Class definitions */
#include "MagOD.h"

/* ############## Global variables ########### */

/* Update frequencies */
float freq_meas = 7; //Measurement frequency in Hz, tested up to 7 Hz. Can be 20 Hz without saving data. Can be 30 Hz if we optimize doMeasurement. Can be 100 Hz if we only read one adc. Can be 700 Hz theoretically...

float freq_screen = 2; //Screen update frequency in Hz

/* Program menu settings */
uint16_t program_cnt = 1; //Current program menu, default 1 
const uint16_t program_nmb = 1;//Total number of menus. Right now, there is only one measurement program. When you want more, make sure there is a good menu selection procedure. The joystick on the 1.8" screen suckcs...

/* State variables (global) used in this program */
bool doMeasurementFlag = false; //Read the ADC inputs
bool screenUpdateFlag = false; //Update the screen
bool Updatecurrentflag = false; //Do a current calibration
bool Exit_program_1 = HIGH; //The program should end

/* Status parameters */
bool isRecording = false; //We are measuring
bool SDpresent = false; //There is a readable SD card in the slot
uint8_t prevButton = 0; /* Status of the button */


/* Control parameters */
bool save_extra_parameter = 0; //set this to 1 when you want to store an extra parameter in the header of the .CVS file (this can be any parameter which makes it easier for dataprocessing, for instance which fields are used)

double extra_par = 0.0; //value of this extra parameter (specifiy this in the initialization function)

/* MagOD libraries, should be subdirectory MagOD.ino folder */
#include "src/led/led.h" // Control of three colour LED
#include "src/pins/pins.h" // Definition of Arduino pins 
#include "src/screen/screen.h" // TFT screen layout
#include "src/buttons/buttons.h" //Control of buttons (joystick)
#include "src/field/field.h"  //Field control
#include "src/adc/adc.h" //ADC input control
#include "src/fileandserial/fileandserial.h" //File and serial port IO
#include "src/recipes/recipes.h" //User measurement recipe

/* Init classes */
screen myscreen;
led myled;
pins mypins;
buttons mybuttons;
field myfield;
adc myadc;
fileandserial myfile;
recipes myrecipes;

/* Define variables */
/* The measured parameters */
extern double Vup   = 0; // Signal of top part of split photodiode
extern double Vdwn  = 0; // Signal of bottom part of split photodiode
extern double Vled  = 0; // Signal of reference photodiode monitoring the LED
extern double Vscat = 0; // Signal of side scatter photodiode
extern double Temperature_degrees = 0; //Temperature estimated from temperature sensor
extern references Vrefs = {0,0,0,0}; 

/* Calculated parameters */
extern double Vav = 0; //(Vup+Vdwn)/2
extern double OD = 0;  //Optical Density. Calculated in CaldOD()

/* Time parameters */
extern unsigned long time_of_data_point = 0; //Store time when datapoint was taken
extern unsigned long time_of_start = 0; //Time at which the measurement was started
extern unsigned long time_last_field_change = 0; //Time since the last field step

/* LED parameters */
extern int LED_type = 2; //The color of the LED, 1 = RED, 2 = GREEN, 3 = BlUE
extern int LED_switch_cycles = 0; //The number of cycles after which the LED changes the frequency, when a 0 is entered, the LED keeps the beginning frequency during the complete measurement 
extern int Counter_cycles_led = 1; //counter used to store the amount of complete cycles the LED has had the same colour, to check when the colour has to change (after LED_switch_cycles)
extern bool ref_all_wavelength = 0; //Set this to 1 for specific programs where you work with multiple wavelengths in a single measurement (such that it stores the reference value of all 3 wavelengths)

/* Declare variables to define the field sequence */
extern int B_nr_set = 1; //the number of elements in the array
extern long Nr_cycles = 0; //The number of cycles throught the array, a value of 0 means an infinite amound of cycles
extern unsigned int Looppar_1 = 0; //Looppar_1,2 track at which point of the field-array the program is
extern unsigned int Looppar_2 = 0; 

extern double B_arrayfield_x[B_NR_MAX] = {0.0}; //an array containing B_Nr_set elements for the field in the x-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_y[B_NR_MAX] = {0.0}; //an array containing B_Nr_set elements for the field in the y-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_z[B_NR_MAX] = {0.0}; //an array containing B_Nr_set elements for the field in the z-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern bool Gradient_x[B_NR_MAX] = {1};  //determines whether both coils must be on or just one of them for the x-direction, 0 is both on, 1 is only one on
extern bool Gradient_y[B_NR_MAX] = {1};  //determines whether both coils must be on or just one of them for the y-direction, 0 is both on, 1 is only one on
extern bool Gradient_z[B_NR_MAX] = {1};  //determines whether both coils must be on or just one of them for the z-direction, 0 is both on, 1 is only one on
extern long Switching_time[B_NR_MAX] ={1000};

/*** Top level functions ***/

/** Timer triggers **/

/* Timer 1 triggers a measurement */
void measEvent()
{
  doMeasurementFlag=true;
}

/* Timer 3 triggers a field correction event */
void Updatecurrent()
{
  Updatecurrentflag=myfield.CheckUpdatedCurrentTime();
  Updatecurrentflag = false;
}

/* Timer 4 triggers a screenupdate */
void screenUpdateEvent()
{
  screenUpdateFlag=true;
}

// Calculate in which part of the B-field array the program currently is, when the program has a specified number of iterations (0 is infinite) this part will ensure that the program stops after that number.
//the program also calculates when to switch the wavelength of the lightsource when this is wanted
void SetBfield_array()
{
  Looppar_1++; //parameter of which element in the B-field array the program currently is
  if (Looppar_1 >= (B_nr_set)) //when larger than total number of values in the array, gos back to first value
    {
      Looppar_1 = 0;
      //this part is responsible for changing the LED color after the predefined number of cycles
      if ((Counter_cycles_led >= LED_switch_cycles) && (LED_switch_cycles != 0))
        {
          Counter_cycles_led = 1;
          LED_type++;
          if (LED_type >= 4)
            {
              LED_type = 1;
            }
          myled.Set_LED_color(LED_type);
        }
      else if (LED_switch_cycles != 0)
        {
          Counter_cycles_led++;
        }
      
      Looppar_2++; // Counts number of times a cycle has completed
      //check whether the program should end if Nr_cylces is set:
      if (Nr_cycles != 0)
      {
        if (Looppar_2 >= Nr_cycles)
        {      
           //stop the recording
           stopRec();  
           
        }
      }
  }
  //Sets the field whenever the program should be running and stores the time at which the field is changed (for the waiting period of the current sensing)
  if (Exit_program_1 == LOW){
    myfield.SetBfieldFast(B_arrayfield_x[Looppar_1],
			  B_arrayfield_y[Looppar_1],
			  B_arrayfield_z[Looppar_1],
			  Gradient_x[Looppar_1],
			  Gradient_y[Looppar_1],
			  Gradient_z[Looppar_1]);
  }
}
 

void startRec()
//Start the measurement
{
    //checks whether SD card is present
  if (!SD.begin(SD_CS)) {
    //If card not present, continue without using
    myscreen.updateFILE("NO SD CARD");
    SDpresent = false;
  }
  else
  {
    myscreen.updateFILE("SD CARD READY");
    SDpresent = true;
  }

  //only starts the program when an SD card is present
  if (SDpresent == true)
  {
    //Initialize program
    Serial.print("Initializing ...");
    delay(1000);
    Exit_program_1 = LOW;
    myrecipes.program_init();
    myfile.file_init(Vrefs, ref_all_wavelength,
		     save_extra_parameter, extra_par, program_cnt,
		     myscreen);
    Serial.println("Done");

    //initialize current feedback. Do this in an init routine in field LEON.
    Serial.print("Initialising current feedback to ");
    Serial.println(1000000);
    Timer3.initialize(1000000);
    time_last_field_change = millis();

    //start coil actuation
    Serial.println("Setting magnetic field");
    myfield.SetBfieldFast(B_arrayfield_x[Looppar_1],
			  B_arrayfield_y[Looppar_1],
			  B_arrayfield_z[Looppar_1],
			  Gradient_x[Looppar_1],
			  Gradient_y[Looppar_1],
			  Gradient_z[Looppar_1]);
    
  
    //Update display
    Serial.println("Updating display");
    myscreen.updateFILE(myfile.fName_char);
    myscreen.setRecButton(true);
  
    //Note starting time
    time_of_start = millis();
    Serial.print("Start measurement");
    Serial.println(time_of_start);
    
    //Activate recording mode
    isRecording = true;
  }
}


void stopRec()
//stop the measurement
{
  Exit_program_1 = HIGH;
  myscreen.setRecButton(false);
  myscreen.updateFILE("Rec finished");
  // reset globals
  Looppar_2 = 0;
  Looppar_1 = 0;
  // Perhaps make a function that resets the file, so that SD_ etc can be private. TODO LEON.
  myfile.SD_file_number_count = 1;
  myfile.SD_file_length_count = 0;
  // Switch off field
  myfield.Reset_Bfield();
  Serial.println("Stop rec");  
  //De-ctivate recording mode
  isRecording = false;
}

//analyse the button that is pressed and take the wanted action
void processButtonPress()
{
  //Check for new button press
  uint8_t buttonPress = mybuttons.readButton();  
  if (buttonPress!=prevButton)
    {
      //Start/stop measurement
      if (buttonPress==BUTTON_SELECT){
	if(!isRecording){
	  Serial.println("Start rec");
	  startRec();
	}
	else{       
	  stopRec();
	}
	delay(1000);//Give use time to release the button
      }
      
    //Set reference voltage
      if (buttonPress==BUTTON_LEFT ){
	if (!isRecording){
	  myadc.set_vrefs(Vrefs,ref_all_wavelength,myled);
	  // ToDo: Perhaps not here, Why don't we give set_Vref the current led color?
	  myrecipes.LED_init();
	  myled.Set_LED_color(LED_type);
        }
        //Update screen
        //TODO, make a screen where all refs are shown in case of 3 colour
        myscreen.updateV(Vav, Vled, Vrefs.Vref, OD);
      }
    }
  
  // change between the programs
  if (buttonPress==BUTTON_RIGHT ){
    if (!isRecording){
      if (!SD.begin(SD_CS)) {
	//If card not present, continue without using 
	myscreen.updateFILE("NO SD CARD");
	SDpresent = false;
      }
      else
        {
          myscreen.updateFILE("SD CARD READY");
          SDpresent = true;
        }
      
      program_cnt++;
      if (program_cnt>program_nmb){
	program_cnt=1;
      }
      myrecipes.LED_init();  //immediatelty change the LED color to led it stabilize 
      myled.Set_LED_color(LED_type);
      
      //Update display
      myscreen.updateInfo(Looppar_1, Looppar_2, program_cnt);
    }
    prevButton = buttonPress;
  } //end if buttonPress!=prevButton
}


//calculate the optical density value, whenever using three wavelengths, the right reference has to be chosen
double calcOD(struct references Vrefs)
{
  if(Vav<=0){return 0;}    //V1 has to be positive
  if(Vrefs.Vref<=0){return 0;}  //Vref has to be positive
  if(Vrefs.Vred<=0){return 0;}  
  if(Vrefs.Vgreen<=0){return 0;}
  if(Vrefs.Vblue<=0){return 0;} 
  /* We are ok, all voltages > 0 */
  /* I don't understand why we simply do not calculate DO for LED_type. Why the if statement? */
  if (ref_all_wavelength == 0)
  {
    return log10(Vrefs.Vref/Vav);  //otherwise, return correct OD value
  }
  else
  {
    switch (LED_type){
    case 1:
      return log10(Vrefs.Vred/Vav);
      break;
    case 2:
      return log10(Vrefs.Vgreen/Vav);
      break;
    default:
      return log10(Vrefs.Vblue/Vav);
      break;
    } 
  }
}



//measures all the wanted values (photodiode voltage and temperature) and stores them on the SD-card if a measurement is started
void doMeasurement()
{
  //Readout ADC's for photodiode voltage
  //Shouldn't this be in a ReadADC function or so? LEON. TODO.
  uint16_t adc0, adc1, adc2, adc3;
  double Temperature_voltage;
  adc0 = myadc.ads.readADC_SingleEnded(0);
  adc1 = myadc.ads.readADC_SingleEnded(1);
  adc2 = myadc.ads.readADC_SingleEnded(2);  
  adc3 = myadc.ads.readADC_SingleEnded(3);     
  
  //Convert to voltage and OD
  Vled = double(adc0)/32768*myadc.adsMaxV;
  Vscat = double(adc1)/32768*myadc.adsMaxV;
  Vdwn = double(adc2)/32768*myadc.adsMaxV;
  Vup = double(adc3)/32768*myadc.adsMaxV; ///adc3 goes to upper part of slit photodiode, adc2 to lower part
  // Perhaps do calculations only when you update the screen. TODO. LEON
  Vav = ((Vup + Vdwn)/2.0);    
  OD = calcOD(Vrefs);
  //calculates the temperature from datasheet calibration
  //TODO Hide this in some ReadTemp function. Separate library? Also, why at the same speed at the other values? LEON.
  Temperature_voltage = analogRead(mypins.Temp_read)*5.0/1024.0; 
  Temperature_degrees = 3.4328 * pow(Temperature_voltage,3)-25.099*pow(Temperature_voltage,2)+76.047*Temperature_voltage-61.785;
 
  //Save results, if desired
  if (isRecording)
  {
    //Record time
    time_of_data_point = millis()-time_of_start;
    
    //Save data, if SD card present
    if (SDpresent)
    {
      // Write data to file
      // Should be a function in fileandserial.cpp. TODO LEON
      myfile.dataFile = SD.open(myfile.fName_char, FILE_WRITE);
      myfile.writeDataLine(myfile.dataFile);
      myfile.dataFile.close(); 

      myfile.SD_file_length_count = myfile.SD_file_length_count + 1;
     //when the file length is longer as the maximum length and a field sequence has finished, a new file is made with the same name + the addition of "_i" with i = 1,2,3,4,5.... and for this new file again all the headers are stored, in the exact same way as the origional header

      // This does not work, why? I think it should be B_nr_set-1
      //      if ((SD_file_length_count > SD_file_length_count_max) && (Looppar_1 >= (B_nr_set))){
	if (myfile.SD_file_length_count > myfile.SD_file_length_count_max){
        myfile.SD_file_length_count = 0;
	
	//Send the file to the serial port
	myfile.sendFileToSerial(myfile.fName_char);
	
        myfile.fName_str = "f" + (String)myfile.file_number + "_" +
	  (String)myfile.SD_file_number_count +".csv";
        myfile.fName_str.toCharArray(myfile.fName_char, myfile.fN_len);
        myfile.SD_file_number_count = myfile.SD_file_number_count +1;
        myfile.dataFile = SD.open(myfile.fName_char, FILE_WRITE);
        myfile.dataFile.print("Time[ms],Vup[V],Vdwn[V],Vav[V],Vled[V],Vscat[V],OD[],Temp[C],Color[],Output[],Vref=");
        myfile.dataFile.print(Vrefs.Vref,5);
        if (ref_all_wavelength == 1)
        {
          myfile.dataFile.print(",Vrefred=");
          myfile.dataFile.print(Vrefs.Vred,5);
          myfile.dataFile.print(",Vrefgreen=");
          myfile.dataFile.print(Vrefs.Vgreen,5);
          myfile.dataFile.print(",Vrefblue=");
          myfile.dataFile.print(Vrefs.Vblue,5);
        }
        if (save_extra_parameter == 1)
        {
          myfile.dataFile.print(",extrapar=");
          myfile.dataFile.print(extra_par,5);
        }
        myfile.dataFile.print(",Prg=");
        myfile.dataFile.println(program_cnt);
        myfile.dataFile.close();
	
	//Update display
	myscreen.updateFILE(myfile.fName_char);

      } //if (SD_file_length_count > SD_file_length_count_max)
    } //if (SDpresent)
  } // if (Isrecording)
  //writeDataLine(Serial); for debug
}


//Initialization of all the timers used in the program
void Init_timers()
{
  //setup timer 1 for measurement
  Timer1.initialize(long(1000000/freq_meas));
  Timer1.attachInterrupt(measEvent);
  
  //Setup timer 3 for current sensing
  Timer3.initialize(1000000);
  Timer3.attachInterrupt(Updatecurrent);
  
  //Setup timer 4 for screen updates
  Timer4.initialize(long(1000000/freq_screen));
  Timer4.attachInterrupt(screenUpdateEvent);
}

void setup()
{
/* ######################### Initialize boards ########################### */
  // A lot of this should be done in the libraries. LEON. TODO.
  //starts the serial connection for debugging and file transfer
  Serial.begin(115200);
  //initialize timers 5, to not affect other timers
  Timer5.initialize(); 

  // Should be in a function init_pins in pins.cpp. LEON. TODO
  myfield.Init_field();/*Sets the pins in the correct status*/
  
  //sets the LEDs as outputs
  pinMode(LED_red, OUTPUT);
  pinMode(LED_green, OUTPUT);
  pinMode(LED_blue, OUTPUT);

  //initialization functions
  myfield.Reset_Bfield();
  myrecipes.LED_init();
  myled.Set_LED_color(LED_type);

  //This should be a function init_adc in adc.cpp. LEON. TODO.
  //set gain of the adc
  //Create serial connection
  //Initialize the adc gain and thereby the maximal detectable values
  //ads.setGain(GAIN_TWOTHIRDS); adsMaxV=6.144; // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE); adsMaxV=4.096; // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO); adsMaxV=2.048; // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
   myadc.ads.setGain(GAIN_FOUR); myadc.adsMaxV=1.024; // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT); adsMaxV=0.512; // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN); adsMaxV=0.256 // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  myadc.ads.begin();


  //setup the screen, first to a black screen
  myscreen.setupScreen();

  // see if the card is present and can be initialized
  if (!SD.begin(SD_CS)) {
    // More testing
    if (!myfile.card.init(SPI_HALF_SPEED, SD_CS)) {
	Serial.println("card initialization failed");
	//If card not present, continue without using 
	myscreen.updateFILE("NO SD CARD");
	SDpresent = false;
      }
    else {
      if (!SD.begin(SD_CS)) {
	  Serial.println("card still not working");
	  //If card not present, continue without using 
	  myscreen.updateFILE("NO SD CARD");
	  SDpresent = false;
	}
      else {
	Serial.println("ok!");
	myscreen.updateFILE("SD CARD READY");
	SDpresent = true;
      }
    }
  }
  else
  {
    myscreen.updateFILE("SD CARD READY");
    SDpresent = true;
  }

  myscreen.updateInfo(Looppar_1, Looppar_2, program_cnt);
  
  myfield.Init_current_feedback();
  Init_timers();
  Looppar_1 = 0;
  Looppar_2 = 0;
  //sets the initial field to start the measurement, depending on the program used
}

/* Main loop */
void loop()
{
  delay(1);
  /* Check for button pressed */
  processButtonPress();
  
  /* Perform measurement if timer1 has triggered flag */
  if(doMeasurementFlag)
    {
      doMeasurementFlag=false; // reset flag for next time
      doMeasurement();
    }
  
  /* Update screen if timer4 has triggered flag */
  if(screenUpdateFlag)
    {
      screenUpdateFlag=false; // reset flag for next time
      myscreen.updateV(Vav, Vled, Vrefs.Vref, OD); //Update values
      myscreen.updateGraph(Vav,LED_type); //Update graph
      myscreen.updateInfo(Looppar_1, Looppar_2, program_cnt); //Update program status
    }
  
  /* Recalibrate current if timer 3 has set the flag */
  if(Updatecurrentflag == true)
    {
      //Reset flag
      Updatecurrentflag = false;
      //Perform current recalibration procedure. But only if the program has not exit yet and when the PWM value has not too recently changed
      if ((Exit_program_1 == LOW) && ((millis() - time_last_field_change) > myfield.Current_wait_time))
	{
	  myfield.Current_feedback();
	  Serial.println("Perform feedback on current");
	}
    }
  
  /* Check if it is time to go to the next step in the measurement loop */
  if (((millis() - time_last_field_change) > (Switching_time[Looppar_1]-1)) && (Exit_program_1 == LOW))
    {
      time_last_field_change = millis();
      SetBfield_array();
      
      //prints for debugging
      Serial.print("The time of field change is: ");
      Serial.println(time_last_field_change);
      Serial.print("B_nr_set = ");Serial.println(B_nr_set);
      Serial.print("Looppar = ");Serial.println(Looppar_1);
      Serial.print("The magnetic field in the x-direction = ");
      Serial.println(B_arrayfield_x[Looppar_1]);
      Serial.print("The magnetic field in the y-direction = ");
      Serial.println(B_arrayfield_y[Looppar_1]);
      Serial.print("The magnetic field in the z-direction = ");
      Serial.println(B_arrayfield_z[Looppar_1]);
    }
}
//end of program
