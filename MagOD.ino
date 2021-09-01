/* MagOD veresion 2.4 */
/* Jan 2021 */
/* Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann, Marcel
   Welleweerd, Dave van As, Ruthvik Bandaru, Rob Krawinkel */

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
// MagOD2 measures on interupt basis on ADC0. Set ADC clock in adc.h
float freq_meas = 1; /* Measurement frequency for ADC1 (currents,
			temperature) in Hz */
#endif
float freq_screen = 4; //Screen update frequency in Hz

/* Do you want Wifi :). Make sure password.h is present, see under FTP
   server for details */
bool wifi = true;

/* Program menu settings */
recipe recipes_array[MaxRecipes]; // Array of recipes
int    program_cnt = 0; /* Current recipe selected, default 0 */
int    program_nmb = 0; /* Total number of recipes in recipes_array
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


/* Extra control parameters, kind of obsolete */
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
references Vrefs = {1,1,1,1,PDOFFSET}; /* Reference values of photodector
				  signals. Current value and three
				  different colors. Initialize to 1 to
				  get meaningful OD */
feedbacks Currents = {0,0,0};   /* Current feedback values */

/* Parameters for displayed graph */
dataPlot graph[GRAPH_LENGTH] = {0,0};/* Graph with datapoints and
					their color to display */
int graphCount      = 0; // Index counting datapoints in graph
int graphLength     = GRAPH_LENGTH; // Total lengt. <= GRAPH_LENGTH
long startTime      = millis(); /* Time at which the measurement
				   started */
long lengthTimeAxis = 200000; /* x-axis ranges from 0..lengthTimeAxis (ms)

/* Calculated parameters */
double OD = 0;                 /*Optical Density. Calculated in
				 CaldOD() */

/* Time parameters */
unsigned long time_of_start = millis(); /*Time at which the
					measurement was started.*/
unsigned long time_last_field_change = millis(); /*Time since the last
					    field step */

/* LED parameters */
//int LED_type = GREEN; //The color of the LED
int LEDs[3] = {RED, GREEN, BLUE};
#if defined(_MAGOD2)
int LED_intensity[3] = {15,65,95};//Values that don't saturate the photodiode
#endif
// OBSOLETE? LEON
bool ref_all_wavelength = 0; //Set this to 1 for specific programs where you work with multiple wavelengths in a single measurement (such that it stores the reference value of all 3 wavelengths. Can be removed, but you need to rewrite set_vrefs in adc.cpp

/* Declare variables to define the field sequence */
int B_nr_set = 1; //the length of the field sequence array [0..B_nr_set];
int Looppar_1 = 0; //Track at which point of the field-array we are

int Nr_cycles = 0; //The number of cycles throught the array, a value of 0 means an infinite amound of cycles
int Looppar_2 = 0; //Track how many times we cycled through the recipe

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
    strlcpy(myfile.fName_char,"READY",myfile.fN_len);
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

    //Update display value for file
    Serial.println("Updating display");
    myscreen.updateFILE(myfile.fName_char);
    myscreen.setRecButton(true);
    
    //Note starting time, do this BEFORE setting the field
    time_of_start = millis();
    Serial.print("Started measurement at:  ");
    Serial.println(time_of_start);
    
    //start coil actuation
    Serial.println("Setting magnetic field and LEDs at step 0");
    Looppar_1 = 0;
    SetBfield_array(Looppar_1);    

    //Set parameters for the graph display.
    startTime       = time_of_start; /* Time at which the measurement
					started */
    /*Calculate total length of graph time axis from length of recipe
      step:*/
    Serial.print("Emptying buffer");
    while (not myadc.bufferEmpty()) {
      dataPoint trash = myadc.getDataPoint();
      Serial.print(".");
    }
    Serial.println();
    graphCount = 0; // Clear data array

    myscreen.clearGraph(recipes_array, program_cnt);; //Clear graph
    
    //Activate recording mode
    isRecording = true;
  }
}


void stopRec()
//stop the measurement
{ Serial.println("Stop rec");
  Exit_program_1 = HIGH;
  // reset loop counters
  Looppar_2 = 0;
  Looppar_1 = 0;
  // Switch off field
  myfield.Reset_Bfield();
  // Close file
  dataFile.close();
  // De-activate recording mode
  isRecording = false;
  // Update screen
  myscreen.setRecButton(false);
  strlcpy(myfile.fName_char,"DONE",myfile.fN_len);
  myscreen.updateFILE(myfile.fName_char);
}

/*analyse the button that is pressed and take the required action */
/* This function has become too long. Take out the different actions. LEON */
bool processButtonPress(){
  bool buttonPressed = false;
  //Check for new button press
  uint8_t buttonPress = mybuttons.readButton();  
  if (buttonPress!=prevButton){
    buttonPressed = true;
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
    } // END BUTTON_SELECT
    
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
    } // End BUTTON_LEFT

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
			    LEDColor_array[Looppar_1], myfile.fName_char);
      }
    } //end if buttonPress!=prevButton

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
			    LEDColor_array[Looppar_1], myfile.fName_char);
	startTime       = millis();
	Serial.print("Emptying buffer");
	while (not myadc.bufferEmpty()) {
	  dataPoint trash = myadc.getDataPoint();
	  Serial.print(".");
	}
	lengthTimeAxis =
	  myrecipes.getSequenceLength(recipes_array,program_cnt);
	graphCount = 0; // Clear data array
	myscreen.clearGraph(recipes_array, program_cnt);; //Clear graph
      }
    } //end if buttonPress!=prevButton

    if (buttonPress == BUTTON_GRAPH){
      myscreen.graphAutoScale(graph, graphCount, graphLength,
			      recipes_array, program_cnt);
    }

#endif
    prevButton = buttonPress;
  } // End buttonPress!=prevButton
  return buttonPressed;
}    


//calculate the optical density value, whenever using three wavelengths, the right reference has to be chosen
double calcOD(struct references Vrefs, double Vdiode)
{
#if defined(_MAGOD1)
  int sign=1; // MagOD1 has a detector that has higher signal for higher intensity
#elif defined(_MAGOD2)
  int sign=-1; //MagOD2 has a detector that has lower signal for higher intensity.
#endif

  if(Vdiode<=0){return 0;}    //Vdiode has to be positive

  
  /* I don't understand why we simply do not calculate OD for LED_type. Why the if statement? Also, with the new recipe, we can have multiple LED colors in one recipe. This needs to be reworked. TODO Leon */
  if (ref_all_wavelength == 0)
  {
    if(Vrefs.Vref<=0)
      {
	return 0; //Vref has to be positive
      }
    else {//otherwise, return correct OD value
      return log10(Vrefs.offset + sign*Vrefs.Vref) -
	     log10(Vrefs.offset + sign*Vdiode) ;  
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
	  return log10(Vrefs.offset + sign*Vrefs.Vred) -
	         log10(Vrefs.offset + sign*Vdiode) ;  

	}
      break;
    case GREEN:
      if (Vrefs.Vgreen <= 0)
	{
	  return 0; //Vref has to be positive
	}
      else
	{
	  return log10(Vrefs.offset + sign*Vrefs.Vgreen) -
	     log10(Vrefs.offset + sign*Vdiode) ;  

	}
      break;
    case BLUE:
      if (Vrefs.Vblue <= 0)
	{
	  return 0; //Vref has to be positive
	}
      else
	{
	  return log10(Vrefs.offset + sign*Vrefs.Vblue) -
	         log10(Vrefs.offset + sign*Vdiode) ;  
	}
      break;
    default:
      return 0;
    } 
  }
}



void writeDataPointToFile(File datfile, dataPoint data){
  /* Write measurements to datafile.*/
  myfile.saveLine(datfile,data,LEDColor_array[Looppar_1],Looppar_1);
  /* update file length */
  //myfile.SD_file_length_count = myfile.SD_file_length_count + 1;
} 

//Initialization of all the timers used in the program
void Init_timers()
{
  //setup timer 1 for measurement
  mytimer.initTimer(1,long(1000000/freq_meas));
  mytimer.attachInterrupt(1, measEvent);
  
  //Setup timer 3 for current compenstation
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

/* If Vdiode was measured, add value to graph display array. Function
   returns updated index of graph array */
/* data : measured datapoint (.channel, .time, .val)  */
/* plot : array of measurement points to be plotted */
/* count : index for graphArray */
/* length: length of graphArray */
/* start : (ms) time at which measurement started (offset for display
   time axis */
/* lengthTime: (ms) total length of x-axis. */
int updateGraphArray(dataPoint data,
		     dataPlot *plot, int count, int length,
		     long start, long lengthTime) {
  if (data.channel == DIODE) {
    /* calculate time passed since start of new sequence */
    long graphTime = data.time - start;
    /* if time goes beyond length of time axis, start again from zero */
    while (graphTime > lengthTime){
      graphTime = graphTime - lengthTime;
    }
    /* round off graphTime to nearest index */
    double fraction = (double)graphTime/(double)lengthTime;
    int index = round(fraction*length); // Array index
    /* Debug:  
    Serial.print("updateGraph: start: ");
    Serial.print(start);
    Serial.print(", lengthTime: ");
    Serial.print(lengthTime);
    Serial.print(", graphTime: ");
    Serial.println(graphTime);
    Serial.print(", fraction: ");
    Serial.print(fraction);
    Serial.print(", length: ");
    Serial.print(length);
    Serial.print(", index: ");
    Serial.println(index);
    /* End debug: */
    /* index should be between 0 and GRAPH_LENGTH. Panic if that is
       not the case */
    if ((index < 0) || (index > GRAPH_LENGTH)) {
      Serial.print("ERROR: Updategraph: index = ");
      Serial.println(index);
      index=0;
    }
    /* if index is smaller than count, apparently we wrapped
       around. Fill the array's up to max and from zero value. Maybe
       interpolate in future versions. LEON*/
    if (index < count) {
      /* Fill last part of array from count to GRAPH_LENGTH */
      for (i=count; i<=GRAPH_LENGTH; i++){
	plot[i].val=data.val;
	plot[i].color=LEDColor_array[Looppar_1];
      }
      /* Fill first part of array from 0 to index */
      for (i=0; i<=index; i++){
	plot[i].val=data.val;
	plot[i].color=LEDColor_array[Looppar_1];
      }
    }
    /* if index is equal to count, we did not proceed more than one
       step in the array. In that case average with value already there */
    if (index == count) {
      plot[index].val=(plot[index].val + data.val)/2;
      plot[index].color=LEDColor_array[Looppar_1];
    }
    /* if index > count add measured value to the graphArray. If there
       are non-used indices before the index, fill them all with same
       value. (Maybe interpolate in later versions. LEON) */
    if (index > count) {
      for (i=count; i<=index; i++){
	plot[i].val=data.val;
	plot[i].color=LEDColor_array[Looppar_1];
	if (i > GRAPH_LENGTH) {
	  Serial.println("updateGraph: index out of bounds!");
	  count = GRAPH_LENGTH;
	  break;
	}
      }
      /* Debug
      Serial.println("updateGraphArray: graphTime: ");
      Serial.print(graphTime);
      Serial.print(" plot array ");
      for (int j=0; j< count; j++) {
	Serial.print(plot[j].val);
	Serial.print(", ");
      }
      Serial.println();
      End debug*/
    }
    count = index; // in any case, count equal index after this.
  } // end if data.channel = DIODE
  return count;
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
  } // end of SD not intialized
  else {
    Serial.println("SD Card Ready");
    strlcpy(myfile.fName_char,"NO SD",myfile.fN_len);
    //myscreen.updateFILE(myfile.fName_char);
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
    //myscreen.updateFILE(myfile.fName_char);

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
#endif //Defined MAGOD2
  } // End of else: SD initialization succesfull
  
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
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      
      ftpSrv.begin("esp32","esp32"); /* username, password for ftp.
					set ports in ESP32FtpServer.h
					(default 21, 50009 for PASV) */      
      serverStarted = true;

      /* Initialize Network Time Protocol */
      const char *ntpServer = "84.245.9.254"; //pool.ntp.org
      const long  gmtOffset_sec = 3600;//UTC +1
      const int   daylightOffset_sec = 3600;//Country obeys summertime
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      struct tm timeinfo;
      Serial.print("Connecting to NTP server : ");
      /* Maybe try several times with time-out, LEON */
      if(!getLocalTime(&timeinfo)){
	Serial.print("Failed to obtain time, ");
	Serial.println("setting time to 1-1-2-2021 0:00");
	struct timeval tv;
	tv.tv_sec = 1609459200;
	settimeofday(&tv, NULL);
      }
      else {
	Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      }
    }
  }
  else {// No Wifi
	Serial.println("No Wifi: setting time to 1-1-2-2021 0:00");
	struct timeval tv;
	tv.tv_sec = 1609459200;
	settimeofday(&tv, NULL);
  }

  //setup the screen
  Serial.println("Initializing screen...");
  double maxTime = (double)lengthTimeAxis/1000;//convert to seconds
  myscreen.setupScreen(0,maxTime,0,adsMaxV0);
  delay(100);
  
  //Setup the buttons or touchscreen
  Serial.println("Initializing buttons or touchscreen");
  mybuttons.initButton();
  
  Serial.println("Updating screen.");
  myscreen.updateInfo(Looppar_1, Looppar_2,
		      program_cnt,LEDColor_array[Looppar_1], " ");
  
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
	Serial.print("Duration of first program: ");
	lengthTimeAxis =
	  myrecipes.getSequenceLength(recipes_array,program_cnt);
	if (lengthTimeAxis <= 0) {
	  Serial.print(" <=0, resetting to :");
	  lengthTimeAxis = 20000;
	}
	Serial.println(lengthTimeAxis);
	myscreen.clearGraph(recipes_array, program_cnt);; //Clear graph
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
    /* If Vdiode was measured, add value to graph display array */
    graphCount = updateGraphArray(nextData, graph, graphCount,
				  graphLength,startTime,lengthTimeAxis);
    /* If we are recording, write datapoint to file */
    if (isRecording){
      /* Write datapoint to file */
      if (not dataFile) {Serial.println("loop: file not open!!!");};
      writeDataPointToFile(dataFile, nextData);
      /* open file at beginning of recipe step, close it after step or
	 every minute or so */
    }
  }

  /* Check if it is time to go to the next step in the measurement loop */
  int currenttime = millis();
  int meastime = currenttime - time_last_field_change;
  if ((meastime >= Switching_time[Looppar_1]) && (Exit_program_1 == LOW)) {
    Looppar_1 = Looppar_1+1;
    // For debugging:
    Serial.println("-------------------------------------------------");
    Serial.print("Looppar 1: ");Serial.println(Looppar_1);
    Serial.print("Total measurement time: ");
    Serial.println(currenttime - time_of_start);
    Serial.print("Time of this step     :");Serial.println(meastime);
    /* when larger than total number of values in the array, wrap
       over */
    if (Looppar_1 > B_nr_set){ 
      Looppar_1 = 0; /* If we go back to 1 here, we have our
			initialization cycle! LEON */
      Looppar_2++; // Counts number of times a cycle has completed
      /* check whether the program should end if Nr_cylces is set: */
      if (Nr_cycles != 0 && Looppar_2 >= Nr_cycles) {
	stopRec();
      }
      else {
	/* create a new data file */
	dataFile = myfile.newDataFile(dataFile);
	/* Update display */
	myscreen.updateFILE(myfile.fName_char);
	/* Reset the graph */
	graphCount      = 0; 
	startTime       = millis(); //Inaccurate, but simple. LEON
      }
    }
    /* Go to next step in field sequence */
    SetBfield_array(Looppar_1); /* Does not do anything if Looppar_2
				   >= Nr_cycles because stopRec sets
				   Exit_program = HIGH */
    
    return;/* Jump to start of loop to make sure we don't miss
	      datapoints */
  }
  
  /* Check for button pressed and act on it */
  if (processButtonPress()){
    Serial.println("Processing button");
    return;/* Jump to start of loop to make sure we don't miss
	      datapoints */
  };

  /* Handle ftp server requests */
  if (serverStarted)  {
    ftpSrv.handleFTP();
  }
  
  /* Update screen if timer4 has triggered flag */
  if(screenUpdateFlag)
    { //Serial.println("Updating screen");
      screenUpdateFlag=false; // reset flag for next time
      //Update measured values
      myscreen.updateV(Vdiodes, Vrefs, OD, Currents);
      //Update graph on screen
      myscreen.updateGraph(graph, graphCount, graphLength,
      			   recipes_array, program_cnt);
      //Update program status:
      myscreen.updateInfo(Looppar_1, Looppar_2, program_cnt,
      			  LEDColor_array[Looppar_1], myfile.fName_char); 
      return;/* Jump to start of loop to make sure we don't miss
		datapoints */
    }
  
  /* Recalibrate current if timer 3 has set the flag. This does not
     work for the moment, needs to be implemented again. LEON */
  if(Updatecurrentflag == true)
    {
      //Reset flag
      Updatecurrentflag = false;
      /* Perform current recalibration procedure. But only if the
	 program has not exit yet and when the PWM value has not too
	 recently changed */
      if ((Exit_program_1 == LOW) &&
	  ((millis() - time_last_field_change) >
	   myfield.Current_wait_time))
	{
	  myfield.Current_feedback();
	  Serial.println("Perform feedback on current");
	}
      return;/* Jump to start of loop to make sure we don't miss
		datapoints */
    }
}
//end of program

#else
 #error "Wrong board set in IDE! "
#endif
