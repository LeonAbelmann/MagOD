/* MagOD version 2.4 */
/* Dec 2020 */
/* Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann */
/* Based on original code, modified by Leon for readablity and ease of recipe change */

/* Class definitions */
#include "MagOD.h"

/* Version of electronics is defined in MagOD.h */
#if defined(_KIST_1)
#warning "Compiling for KIST MagOD1.Change in MagOD.h"
#elif defined(_KIST)
#warning "Compiling for KIST MAGOD2. Change in MagOD.h"
#elif defined(_CEA)
#warning "Compiling for CEA MAGOD2. Change in MagOD.h"
#elif defined(_BAYREUTH)
#warning "Compiling for BAYREUTH MAGOD2. Change in MagOD.h"
#elif defined(_ASTON)
#warning "Compiling for ASTON MAGOD2. Change in MagOD.h"
#else
#error "No hardware version defined in MagOD.h"
#endif

#if (defined(_MAGOD1) && defined(ARDUINO_AVR_MEGA2560)) || \
    (defined(_MAGOD2) && defined(ARDUINO_ESP32_DEV))


/* ############## Global variables ########### */

/* Update frequencies */
#if defined(_MAGOD1)
float freq_meas = 7; //Measurement frequency in Hz
// For Arduio in MagOD1: tested up to 7 Hz. Can be 20 Hz without saving data. Can be 30 Hz if we optimize doMeasurement. Can be 100 Hz if we only read one adc. Can be 700 Hz theoretically...
#elif defined(_MAGOD2)
// MagOD2 measures on interupt basis on ADC0
float freq_meas = 1; /* Measurement frequency for ADC1 (currents,
			temperature) in Hz */
#endif
float freq_screen = 2; //Screen update frequency in Hz

/* Do you want Wifi */
bool wifi = true;

/* Program menu settings */
recipe recipes_array[MaxRecipes]; // Array of recipes
int    program_cnt = 0; // Current recipe selected, default 0
int    program_nmb = 0; /*Total number of recipes in recipes_array
			  [0..program_nmb] */

/* ADCO works on interupt. Data is stored in buffer. See adc.cpp */
// Handle to refer to the task that does the ADC measurement
TaskHandle_t readADC0Handle; 
// Handle for the buffer
QueueHandle_t dataQueueHandle;

/* ADC settings */
#if defined(_MAGOD1)
double adsMaxV0;  //max voltage that can be measured by the ADC
#elif defined(_MAGOD2)
double adsMaxV0,adsMaxV1;  //max voltages, for two ADCs
#endif

/* State variables (global) used in this program */
bool doMeasurementFlag = false; //Read the ADC inputs. Only ADC1 for MagOD2
bool screenUpdateFlag  = false; //Update the screen
bool Updatecurrentflag = false; //Do a current calibration
bool Exit_program_1    = HIGH;  //The program should end

/* Status parameters */
bool isRecording       = false; //We are measuring
bool SDpresent         = false; //There is a readable SD card in the slot
uint8_t prevButton     = 0;     //Status of the button


/* Control parameters */
bool sendToSerial      = 0;     /*Do we send files during the
				   measurement over serial port? */
bool save_extra_parameter = 0;  /*set this to 1 when you want to store
				  an extra parameter in the header of
				  the .CVS file (this can be any
				  parameter which makes it easier for
				  dataprocessing, for instance which
				  fields are used) */
double extra_par = 0.0;         /*value of this extra parameter
				   (specifiy this in the
				   initialization function) */

/* Init classes */
adc myadc;
screen myscreen;
timer mytimer;
led myled;
buttons mybuttons;
field myfield;
fileandserial myfile;
recipes myrecipes;
IO *  myIO; /* Wraps IO for recipes, used by TestRecipes. Perhaps
	       merge with fileandserial. LEON */
File dataFile; /* file handle for output file */


/* FTP server */
/* To connect to your wifi network, we expect file a 'password.h' in
local directory with contents: 
const char* ssid = "MyWifiNetwork"; //WiFi SSID 
const char* password = "PasswordForThatNetwork"; //WiFi Password
*/
#include "password.h" //expected in local directory
/* hostname that may appear on your network */
const char* host = "magod"; //HERE? LEON
/* FTP server */
FtpServer ftpSrv;
bool serverStarted = false;


/* Should this be here? LEON*/
#if defined(_MAGOD2)
hw_timer_t * timer1 = NULL;
hw_timer_t * timer3 = NULL;
hw_timer_t * timer4 = NULL;
#endif

/* The measured parameters */
dataPoint nextData = {0,0,0};   /* Next datapoint from buffer */
diodes Vdiodes = {0,0,0};       /* Photodetector signals [V] */
double Temperature = 0;         /*Temperature estimated from
				  temperature sensor*/
references Vrefs = {1,1,1,1};   /* Reference values of photodector
				  signals. Current value and three
				  different colors. Initialize to 1 to
				  get meaningful OD */
feedbacks Currents = {0,0,0};   /* Current feedback values */

/* Calculated parameters */
double OD = 0;                 /*Optical Density. Calculated in
				 CaldOD() */

/* Time parameters */
unsigned long time_of_data_point = 0; /*Store time when datapoint was
					taken*/
unsigned long time_of_start = 0;      /*Time at which the measurement was
					started*/
unsigned long time_last_field_change = 0; /*Time since the
					    last field step */

/* LED parameters */
//int LED_type = GREEN; //The color of the LED
int LEDs[3] = {RED, GREEN, BLUE};
#if defined(_MAGOD2)
int LED_intensity[3] = {15,65,95};//Values that don't saturate the photodiode
#endif
// OBSOLETE? LEON
int LED_switch_cycles = 0; //The number of cycles after which the LED changes the frequency, when a 0 is entered, the LED keeps the beginning frequency during the complete measurement 
int Counter_cycles_led = 1; //counter used to store the amount of complete cycles the LED has had the same colour, to check when the colour has to change (after LED_switch_cycles)
bool ref_all_wavelength = 0; //Set this to 1 for specific programs where you work with multiple wavelengths in a single measurement (such that it stores the reference value of all 3 wavelengths

/* Declare variables to define the field sequence */
unsigned int B_nr_set = 1; //the length of the field sequence array [0..B_nr_set];
long Nr_cycles = 0; //The number of cycles throught the array, a value of 0 means an infinite amound of cycles
unsigned int Looppar_1 = 0; //Looppar_1,2 track at which point of the field-array the program is
unsigned int Looppar_2 = 0;

/* Note, the definitions don't seem to work. Only the first element in the array is set to that value. Instead, I initialize the arrays in recipes.cpp */
double B_arrayfield_x[B_NR_MAX] = {0.0}; //an array containing B_Nr_set elements for the field in the x-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
double B_arrayfield_y[B_NR_MAX] = {0.0}; //Same for y
double B_arrayfield_z[B_NR_MAX] = {0.0}; //Same for z
unsigned long Switching_time[B_NR_MAX] ={1000};
int LEDColor_array[B_NR_MAX] = {GREEN}; // Default color is green
int LEDInt_array[B_NR_MAX] = {0}; // Default intensity is off

bool Gradient_x[B_NR_MAX]={1};  //determines whether both coils must be on or just one of them for the x-direction, 0 is both on, 1 is only one. When the set of coils is connected, set to 1. Note that MagOD2 no longer has relays to do this automatically, you need to manually route the currents through one coil only.
bool Gradient_y[B_NR_MAX] = {1};  //same for y
bool Gradient_z[B_NR_MAX] = {1};  //same for z
int i=0;

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

/* Calculate in which part of the B-field array the program currently is, when the program has a specified number of iterations (0 is infinite) this part will ensure that the program stops after that number.*/
void SetBfield_array(int step) {
  /* Sets the field whenever the program should be running and stores
     the time at which the field is changed (for the waiting period of
     the current sensing) */
  if (Exit_program_1 == LOW){
    myfield.SetBfieldFast(B_arrayfield_x[step],
			  B_arrayfield_y[step],
			  B_arrayfield_z[step],
			  Gradient_x[step],
			  Gradient_y[step],
			  Gradient_z[step]);
    myled.Set_LED_color(LEDColor_array[step],
			LEDInt_array[step]);
    time_last_field_change = millis();
    //prints for debugging
    Serial.print("step = ");Serial.print(step);
    Serial.print(" out of [0..");Serial.print(B_nr_set);Serial.println("]");
    Serial.print("Magnetic field in the x-direction = ");
    Serial.println(B_arrayfield_x[step]);
    Serial.print("Magnetic field in the y-direction = ");
    Serial.println(B_arrayfield_y[step]);
    Serial.print("Magnetic field in the z-direction = ");
    Serial.println(B_arrayfield_z[step]);
    Serial.print("LED color = ");
    Serial.println(LEDColor_array[step]);
    Serial.print("LED Intensity = ");
    Serial.println(LEDInt_array[step]);
    Serial.println();
  }
}

void startRec()
//Start the measurement
{
    //checks whether SD card is present
  if (!SD.begin(SD_CS)) {
    //If card not present, continue without using
    strlcpy(myfile.fName_char,"NO SD",myfile.fN_len);
    myscreen.updateFILE(myfile.fName_char); //Print "NO SD CARD"
    myscreen.setRecButton(false); //Set recording button to stop
    SDpresent = false;
  }
  else
  {
    // Something wrong with string length on display. Added spaces. LEON
    strlcpy(myfile.fName_char,"READY     ",myfile.fN_len);
    myscreen.updateFILE(myfile.fName_char);
    SDpresent = true;
    
#if defined(_MAGOD2)
    // Check if card is actually working
    File f = SD.open("/test.txt", FILE_WRITE);
    if(!f){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(f.print("Hello world")){
        Serial.println("test.txt written");
	SDpresent = true;
    } else {
        Serial.println("Write test.txt failed");
	strlcpy(myfile.fName_char,"FILE ERROR",myfile.fN_len);
	SDpresent = false;
	myscreen.setRecButton(false); //Set recording button to stop
    }
    f.close();
#endif
  }

  //only starts the program when an SD card is present
  if (SDpresent == true)
  //if(false) //For debugging when there is no card
  {
    //Initialize program
    Serial.println("Initializing ");
    Exit_program_1 = LOW;
    Serial.print("program, ");
    myrecipes.program_init(recipes_array,program_cnt);
    Serial.print("datafile and ");
    dataFile = myfile.file_init(Vrefs, ref_all_wavelength,
		     save_extra_parameter, extra_par, program_cnt,
		     myscreen);
    if (not dataFile) {Serial.println("StartRec: file is not open!!");};
    Serial.print("current feedback. ");
    myfield.Init_current_feedback();
    Serial.println("Done");

    //Note starting time
    time_of_start = millis();
    Serial.print("Started measurement at:  ");
    Serial.println(time_of_start);
    
    //start coil actuation
    Serial.println("Setting magnetic field and LEDs at step 0");
    Looppar_1=0;
    SetBfield_array(Looppar_1);

    //Update display
    Serial.println("Updating display");
    myscreen.updateFILE(myfile.fName_char);
    myscreen.setRecButton(true);

    
    //Activate recording mode
    isRecording = true;
  }
}


void stopRec()
//stop the measurement
{
  Exit_program_1 = HIGH;
  myscreen.setRecButton(false);
  //Something wrong with string length. For now added spaces
  strlcpy(myfile.fName_char,"DONE      ",myfile.fN_len);
  myscreen.updateFILE(myfile.fName_char);
  // reset globals
  Looppar_2 = 0;
  Looppar_1 = 0;
  // Reset file counters
  myfile.file_reset();
  // Switch off field
  myfield.Reset_Bfield();
  Serial.println("Stop rec");
  // Close file
  dataFile.close();
  //De-activate recording mode
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
	delay(1000);//Give user time to release the button
      }
      
    //Set reference voltage
      if (buttonPress==BUTTON_LEFT ){
	if (!isRecording){
#if defined(_MAGOD1)
	  myadc.set_vrefs(Vrefs,ref_all_wavelength,myled);
          myled.Set_LED_color(LEDColor_array[Looppar_1]);
#elif defined(_MAGOD2)
	  myadc.set_vrefs(Vrefs,false,myled);
	  myled.Set_LED_color(LEDColor_array[Looppar_1],
			      LEDInt_array[Looppar_1]);
#endif
        }
        //Update screen
        //TODO, make a screen where all refs are shown in case of 3 colour
        myscreen.updateV(Vdiodes, Vrefs, OD, Currents);
#if defined(_MAGOD2)
	/* This should be a function in screen.cpp. Every button could
	   have 2-3 states (Start, Starting..., Stop), (Set Vrefs,
	   Setting...) */
	mybuttons.showButtonArea(2, (char *)"Set Vref",
		       TFTCOLOR_DARKGRAY, TFTCOLOR_YELLOW);
#endif
      }
    }

#if defined(_MAGOD1)  
  // change between the programs, MAGOD1 Version
  if (buttonPress==BUTTON_RIGHT ){
    if (!isRecording){
      if (!SD.begin(SD_CS)) {
	//If card not present, continue without using
	strlcpy(myfile.fName_char,"NO SD",myfile.fN_len);
	myscreen.updateFILE(myfile.fName_char);
	SDpresent = false;
      }
      else
        {
	strlcpy(myfile.fName_char,"READY",myfile.fN_len);
	myscreen.updateFILE(myfile.fName_char);
	SDpresent = true;
        }
      
      program_cnt++;
      if (program_cnt>program_nmb){
	program_cnt=0;
      }
      myled.Set_LED_color(LEDColor_array[Looppar_1]);
      //Update display
      myscreen.updateInfo(Looppar_1, Looppar_2, program_cnt,
			  myfile.fName_char);
    }
    prevButton = buttonPress;
  } //end if buttonPress!=prevButton
}

#elif defined(_MAGOD2)
  // change between the programs, MAGOD2 Version
  if (buttonPress==BUTTON_NEXTRECIPE || buttonPress==BUTTON_PREVRECIPE){
    if (!isRecording){
      if (!SD.begin(SD_CS)) {
	//If card not present, continue without using
	strlcpy(myfile.fName_char,"NO SD",myfile.fN_len);
	myscreen.updateFILE(myfile.fName_char);
	SDpresent = false;
      }
      else
        {
	strlcpy(myfile.fName_char,"READY",myfile.fN_len);
	myscreen.updateFILE(myfile.fName_char);
	SDpresent = true;
        }
      // By pressing next, prev, you rotate through the list of programs
      if (buttonPress==BUTTON_NEXTRECIPE) {
	// Increment recipe number, and reset color of the > button to red
	program_cnt=program_cnt + 1;
	Serial.print("program_cnt : ");Serial.println(program_cnt);
	mybuttons.showButtonArea(BUTTON_NEXTRECIPE, (char *)"Next recipe",
			     TFTCOLOR_RED, TFTCOLOR_BLACK);
      };
      if (buttonPress==BUTTON_PREVRECIPE) {
	program_cnt=program_cnt - 1;
	Serial.print("program_cnt : ");Serial.println(program_cnt);
      	mybuttons.showButtonArea(BUTTON_PREVRECIPE, (char *)"Prev recipe",
			     TFTCOLOR_RED, TFTCOLOR_BLACK);
      };
      
      if (program_cnt>program_nmb){program_cnt=0;}
      if (program_cnt<0){program_cnt=program_nmb;}
      // Highlight the correct recipe on the screen
      myscreen.showRecipes(recipes_array,program_nmb,program_cnt);
      // Load the parameters for that recipe
      myrecipes.program_init(recipes_array,program_cnt);
      // Switch on the LED
      myled.Set_LED_color(LEDColor_array[Looppar_1],
			  LEDInt_array[Looppar_1]);
      //Update display
      myscreen.updateInfo(Looppar_1, Looppar_2, program_cnt,
			  myfile.fName_char);
    }
    prevButton = buttonPress;
  } //end if buttonPress!=prevButton
}
#endif      



//calculate the optical density value, whenever using three wavelengths, the right reference has to be chosen
double calcOD(struct references Vrefs, double Vdiode)
{
#if defined(_MAGOD1)
  int detector=1; // MagOD1 has a detector that has higher signal for higher intensity
#elif defined(_MAGOD2)
  int detector=-1; //MagOD2 has a detector that has lower signal for higher intensity
#endif

  if(Vdiode<=0){return 0;}    //Vdiode has to be positive

  
  /* I don't understand why we simply do not calculate OD for LED_type. Why the if statement? Also, with the new recipe, we can have multiple LED colors in one recipe. This needs to be reworked. TODO Leon */
  if (ref_all_wavelength == 0)
  {
    if(Vrefs.Vref<=0)
      {
	return 0; //Vref has to be positive
      }
    else {
      return detector*log10(Vrefs.Vref/Vdiode);  //otherwise, return correct OD value
    }
  }
  else
  {
    switch (LEDColor_array[Looppar_1]){
    case RED:
      if (Vrefs.Vred <= 0)
	{
	  return 0; //Vref has to be positive
	}
      else
	{
	  return detector*log10(Vrefs.Vred/Vdiode);
	}
      break;
    case GREEN:
      if (Vrefs.Vgreen <= 0)
	{
	  return 0; //Vref has to be positive
	}
      else
	{
	  return detector*log10(Vrefs.Vgreen/Vdiode);
	}
      break;
    case BLUE:
      if (Vrefs.Vblue <= 0)
	{
	  return 0; //Vref has to be positive
	}
      else
	{
	  return detector*log10(Vrefs.Vblue/Vdiode);
	}
      break;
    default:
      return 0;
    } 
  }
}



void writeDataPointToFile(File datfile, dataPoint data){
  /* Write measurements to datafile. HIERR*/
  myfile.saveLine(datfile,data,LEDColor_array[Looppar_1],Looppar_1);
  /* myfile.saveToFile(myfile.fName_char,time_of_data_point,
		    Vdiodes,Temperature,OD,
		    LEDColor_array[Looppar_1],Looppar_1,Currents); */
  /* update file length */
  myfile.SD_file_length_count = myfile.SD_file_length_count + 1;

  // This does not work, why? I think it should be B_nr_set-1
  //      if ((SD_file_length_count > SD_file_length_count_max) && (Looppar_1 >= (B_nr_set))){
  // if (myfile.SD_file_length_count > myfile.SD_file_length_count_max){
  //   /* reset file length counter */
  //   myfile.SD_file_length_count = 0;
  //   /* Send the file to the serial port */
  //   if (sendToSerial)
  //     {
  // 	myfile.sendFileToSerial(myfile.fName_char);
  //     }    
  // } //if (SD_file_length_count > SD_file_length_count_max)
} 

//Initialization of all the timers used in the program
void Init_timers()
{
  //setup timer 1 for measurement
  mytimer.initTimer(1,long(1000000/freq_meas));
  mytimer.attachInterrupt(1, measEvent);
  
  //Setup timer 3 for current sensing
  mytimer.initTimer(3,1000000);
  mytimer.attachInterrupt(3, Updatecurrent);
  
  //Setup timer 4 for screen updates
  mytimer.initTimer(4,long(1000000/freq_screen));
  mytimer.attachInterrupt(4, screenUpdateEvent);
}

// Update the values in the datafields from the datapoint in nextData
void updateValues(dataPoint data, diodes &V, double &OD,
		  feedbacks &Cur, double &Temp)
{
  switch (data.channel) { 
  case LED:
    V.Vled = data.val;
    break;
  case SPLIT:
    V.Vsplit = data.val;
    break;
  case SCAT:
    V.Vscat = data.val;
    break;
  case DIODE:
    V.Vdiode = data.val;
    OD = calcOD(Vrefs, V.Vdiode);// Calculate OD
    break;
  case IX:
    Cur.x = data.val;
    break;
  case IY:
    Cur.y = data.val;
    break;
  case IZ:
    Cur.z = data.val;
    break;
  case NTC:
    Temp = data.val;
    break;
  default:
    Serial.println("UpdateValues: data.channel not recognized");
    break;
  }
}

void setup()
{
  delay(1000);//Give serial monitor time

  /* ######################### Initialize boards ########################### */
  // A lot of this should be done in the libraries. LEON. TODO.
  //starts the serial connection for debugging and file transfer
  Serial.begin(115200);
  Serial.println("MagOD *************************************************");
  Serial.println("Initializing all");
  //initialize timers 5, to not affect other timers
  #if defined(_MAGOD1)
  mytimer.initTimer(5, 100000);
  #endif  

  // Initialize field control
  Serial.println("Init field");
  myfield.Init_field();/*Sets the pins in the correct status*/
  myfield.Reset_Bfield();
 
  // Init led
  Serial.println("Init led");
#if defined(_MAGOD1)
  myled.Set_LED_color(LEDColor_array[Looppar_1]);
#elif defined(_MAGOD2)
  myled.Set_LED_color(LEDColor_array[Looppar_1],LEDInt_array[Looppar_1]);
#endif

  //Initialize ADC(s)
  Serial.println("Init ADC");
  myadc.initADC();
  
  //setup the screen
  Serial.println("Initializing screen...");
  myscreen.setupScreen();
  delay(100);

  //Setup the buttons or touchscreen
  Serial.println("Initializing buttons or touchscreen");
  mybuttons.initButton();


  // Setup the SD Card
  // see if the card is present and can be initialized
  Serial.println("Initializing SD Card");
  if (!SD.begin(SD_CS)) {
    Serial.println("SD Card not found");
    strlcpy(myfile.fName_char,"NO SD",myfile.fN_len);
    myscreen.updateFILE(myfile.fName_char);
    delay(100);
#if defined(_MAGOD1)
    // More testing, works only for MagOD1 because card is defined.
    Serial.println("Retrying");
    if (!myfile.card.init(SPI_HALF_SPEED, SD_CS)) {
	Serial.println("card initialization failed");
	//If card not present, continue without using 
	strlcpy(myfile.fName_char,"NO SD",myfile.fN_len);
	myscreen.updateFILE(myfile.fName_char);
	SDpresent = false;
      }
    else {
      if (!SD.begin(SD_CS)) {
	  Serial.println("card still not working");
	  //If card not present, continue without using 
	  strlcpy(myfile.fName_char,"NO SD",myfile.fN_len);
	  myscreen.updateFILE(myfile.fName_char);
	  SDpresent = false;
	}
      else {
	Serial.println("ok!");
	strlcpy(myfile.fName_char,"READY",myfile.fN_len);
	myscreen.updateFILE(myfile.fName_char);
	SDpresent = true;
      }
    }
#endif //defined(MAGOD1)
#if defined(_MAGOD2) // Card is not defined for EPS32
    Serial.println("card initialization failed");
    strlcpy(myfile.fName_char,"NO SD",myfile.fN_len);
    myscreen.updateFILE(myfile.fName_char);
#endif
  }
  else
  {
    Serial.println("SD Card Ready");
    strlcpy(myfile.fName_char,"NO SD",myfile.fN_len);
    myscreen.updateFILE(myfile.fName_char);
    SDpresent = true;

#if defined(_MAGOD2)
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("Card Type not recognized");
	strlcpy(myfile.fName_char,"NO SD",myfile.fN_len);
	myscreen.updateFILE(myfile.fName_char);
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD %lluMB\n", cardSize);
    /* It would be nice to print the filesize on the screen. LEON */
    strlcpy(myfile.fName_char,"READY",myfile.fN_len);
    myscreen.updateFILE(myfile.fName_char);

    /* Test if file can actually be opened for writing */
    File f = SD.open("/test.txt", FILE_WRITE);
    if(!f){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(f.print("Hello world")){
        Serial.println("test.txt written");
    } else {
        Serial.println("Write test.txt failed");
	strlcpy(myfile.fName_char,"FILE ERROR",myfile.fN_len);
    }
    f.close();

    /* Wifi */
    if (wifi) {
      Serial.println("Initializing Wifi ..");
      WiFi.mode(WIFI_STA);
      Serial.print("Connecting to ");
      Serial.print(ssid);
      WiFi.begin(ssid, password);
      int startTime = millis();
      while ((WiFi.status() != WL_CONNECTED) &&
	     (millis() - startTime < 10000) ){
	delay(500);
	Serial.print(".");
      }
      if (WiFi.status() != WL_CONNECTED) {
	Serial.println(" Failed on time-out");
	  }
      else {
	Serial.println(" OK");
	// Print local IP address and start ftp server
	Serial.println("WiFi connected.");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	
	ftpSrv.begin("esp32","esp32"); /* username, password for ftp.
				       set ports in ESP32FtpServer.h
				       (default 21, 50009 for PASV) */
	serverStarted = true;
      }
    }
#endif //Defined MAGOD2

  }

  
  Serial.println("Updating screen.");
  myscreen.updateInfo(Looppar_1, Looppar_2, program_cnt," ");


  Serial.println("Intializing current feedback");
  myfield.Init_current_feedback();
  Init_timers();
  Looppar_1 = 0;
  Looppar_2 = 0;
  //sets the initial field to start the measurement, depending on the program used

  // Load recipes from RECIPES.CSV file on the Flash card
  Serial.println("Setup: load recipes");
  if (SDpresent) {
    File recipeFile = SD.open("/RECIPES.CSV");
    if (recipeFile){
      program_nmb = myrecipes.LoadRecipes(recipeFile, recipes_array);
      Serial.print("Number of Recipes loaded: ");
      Serial.println(program_nmb+1);
      if (program_nmb < 1) {
	Serial.println("Loading recipe file failed");
      }
      else {
	myscreen.showRecipes(recipes_array,program_nmb,program_cnt);
	Serial.println("Loading first program");
	myrecipes.program_init(recipes_array,program_cnt);
	Serial.println("Switching on LED");
	myled.Set_LED_color(LEDColor_array[Looppar_1],
			    LEDInt_array[Looppar_1]);
      }
    }
    else {
      Serial.println("RECIPES.CSV not found");
    }
  }

  Serial.println("Initialization finished");
}

/* Main loop */
void loop()
{
  /* Check if there are datapoints in the buffer */
  if (not myadc.bufferEmpty()) {
    /* get the next datapoint */
    nextData = myadc.getDataPoint();
    /* Update values for display */
    updateValues(nextData, Vdiodes, OD, Currents, Temperature);
    /* If we are recording, write datapoint to file */
    if (isRecording){
      /* Write datapoint to file */
      if (not dataFile) {Serial.println("loop: file not open!!!");};
      writeDataPointToFile(dataFile, nextData);
      /* open file at beginning of recipe step, close it after step or
	 every minute or so */
    }
  }

  /* Perhaps better if we check for recipe next step first and than return. */
  
  /* Check for button pressed and act on it */
  /* This should have a return ! */
  processButtonPress();

  /* Handle ftp server requests */
  /* maybe timer based ??? */
  if (serverStarted)  {
    ftpSrv.handleFTP();
  }
  
  /* Update screen if timer4 has triggered flag */
  if(screenUpdateFlag)
    { //Serial.println("Updating screen");
      screenUpdateFlag=false; // reset flag for next time
      myscreen.updateV(Vdiodes, Vrefs, OD, Currents); //Update values
      myscreen.updateGraph(Vdiodes.Vdiode,LEDColor_array[Looppar_1]); //Update graph
      myscreen.updateInfo(Looppar_1, Looppar_2, program_cnt,
      			  myfile.fName_char); //Update program status
      return;/* Jump to start of loop to make sure we don't miss datapoints */
    }
  
  /* Recalibrate current if timer 3 has set the flag */
  if(Updatecurrentflag == true)
    {
      //Reset flag
      Updatecurrentflag = false;
      //Perform current recalibration procedure. But only if the program has not exit yet and when the PWM value has not too recently changed
      if ((Exit_program_1 == LOW) &&
	  ((millis() - time_last_field_change) > myfield.Current_wait_time))
	{
	  myfield.Current_feedback();
	  Serial.println("Perform feedback on current");
	}
      return;/* Jump to start of loop to make sure we don't miss datapoints */
    }
  
  /* Check if it is time to go to the next step in the measurement loop */
  int currenttime = millis();
  int meastime = currenttime - time_last_field_change;
  if ((meastime >= Switching_time[Looppar_1]) && (Exit_program_1 == LOW)) {
    Looppar_1 = Looppar_1+1;
    if (Looppar_1 > B_nr_set){ /* when larger than total number of values in
				  the array, gos back to first value */
      Looppar_1 = 0; /* If we go back to 1 here, we have our
			initialization cycle! LEON */
      /* create a new data file */
      myfile.updateFileName(dataFile);
      /* Update display */
      myscreen.updateFILE(myfile.fName_char);

      Looppar_2++; // Counts number of times a cycle has completed
      //check whether the program should end if Nr_cylces is set:
      if (Nr_cycles != 0) {
	if (Looppar_2 >= Nr_cycles) {      
	  stopRec();
	}
      }
    }
    
    // For debugging:
    Serial.println("-------------------------------------------------");
    Serial.print("Looppar 1: ");Serial.println(Looppar_1);
    Serial.print("Total measurement time: ");
    Serial.println(currenttime - time_of_start);
    Serial.print("Time of this step     :");Serial.println(meastime);
    // Go to next step in field sequence:
    SetBfield_array(Looppar_1);
  }
}
//end of program

#else
 #error "Wrong board set in IDE! "
#endif
