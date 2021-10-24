/* MagOD.h
 MagOD libary 
 Feb 2021
 Struct definitions, global (external) variables
 Leon Abelmann 
*/
#ifndef _MagOD_h
#define _MagOD_h

/* Define which system you compile for, outcomment only one */
//#define _CEA
#define _BAYREUTH
//#define _ASTON
//#define _KIST
//#define _KIST_1 // MagOD1 system
#include "calibration/calibration.h"


/* Define for which processor/screen we are compling */
#if defined(_KIST_1)
#define _MAGOD1 // Version with Arduino MEGA and ST7735 1.8" 128x160 screen with joystick button
#endif
#if (defined(_KIST) || defined(_CEA) || defined(_BAYREUTH) || defined(_ASTON))
#define _MAGOD2 /*Version with ESP32 Devkit and TFTM050 5" 480x272
screen and capacitive touch */
#endif

/* Declaration of stuctures */

/* Photodiode signals */
struct diodes {
  double Vdiode; /* photodector */
  double Vled; /* photodiode that measures LED directly */
  double Vscat; /* scattering photodiode */
  double Vsplit; /* extra channel */
  /* MagOD1 has a split photodiode */
#if defined(_MAGOD1)
  double Vup; 
  double Vdwn;
#endif
};

/* Diode reference voltages */
struct references {
  double Vref;   // Current value
  double Vred;   // Voltage for red LED
  double Vgreen; // Voltage for green LED
  double Vblue;  // Voltage for blue LED
  double offset; // Offset voltage of electronics (MagOD2)
};

/* Current feedback loop voltages */
struct feedbacks {
  double x;
  double y;
  double z;
};


struct dataPoint {
  int           channel;// Which DAC channel did we read, DIODE etc. see adc.h
  unsigned long time;   // At what time is the point measured (ms)
  double        val;    // What is its value
};

#define GRAPH_LENGTH SCRN_HOR //Maximum plotlength does not have to be
			     //bigger than the number of screen pixels.
struct dataPlot {
  double val; // Value to be plotted in the graph
  int color; // Which color (RED, GREEN, BLUE);
};

/* MagOD libraries, should be src subdirectory MagOD.ino folder */
#include "src/timer/timer.h" // On board timers
#include "src/led/led.h" // Control of three colour LED
#include "src/buttons/buttons.h" //Control of buttons (joystick)
#include "src/field/field.h"  //Field control
#include "src/adc/adc.h" //ADC input control
#include "src/ESP32FtpServer/ESP32FtpServer.h" //Wifi access with File Transfer
#include <time.h> //Network time protocol to obtain real time.

#if defined(_MAGOD1)
#include "src/screen/screen.h" // TFT Screen with button
#elif defined(_MAGOD2)
#include "src/screen/screen_RA8875.h" // RA8875 TFTM050 with touch 
#endif
// Load screen before fileandserial and recipes 
#include "src/fileandserial/fileandserial.h" //File and serial port IO
#include "src/recipes/recipes.h" //User measurement recipe

/* Definition of global (extern) variables */

/* This should not be here, but in timer.h, but I cannot make that compile
   LEON */
#if defined(_MAGOD2)
extern hw_timer_t * timer1;
extern hw_timer_t * timer3;
extern hw_timer_t * timer4;
#endif

extern timer mytimer;
extern screen myscreen;
extern buttons mybuttons;
extern adc myadc;
extern IO *  myIO; /*The exact IO routines depend on platform:
	      ESP32 serial monitor (ESP_PLATFORM)
	      Command line (stdioVersion)) */

/* FTP server variables */
extern FtpServer ftpSrv;
extern bool serverStarted;
extern const char* ssid ;    //WiFi SSID
extern const char* password; //WiFi Password

/* The measured parameters */
extern diodes Vdiodes;     /*Signals of photodiodes */
extern references Vrefs;   /* Reference photodiode signals */
extern feedbacks Currents; /* Settings of current feedback loop */
extern double Temperature; /* Temperature estimated from temperature sensor */

/* Calculated parameters */
extern double OD;  //Optical Density. Calculated in CaldOD()

/* Update frequencies */
extern float freq_meas;   /* Measurement frequency in Hz */
extern float freq_screen; /* Screen update frequency in Hz */

/* Time parameters */
extern unsigned long time_of_start; //Time at which the measurement was started
extern unsigned long time_last_field_change; //Time since the last field step

/* LED parameters */
#define LEDnumber 3 //Number of LEDs available
extern int LEDs[LEDnumber];
//extern int LED_type; //The color of the LED e.g (RED, GREEN, BlUE)
#if defined(_MAGOD2)
extern int LED_intensity[LEDnumber];//The brightness of the leds
#endif
extern bool ref_all_wavelength; //Set this to 1 for specific programs where you work with multiple wavelengths in a single measurement (such that it stores the reference value of all 3 wavelengths). Can be removed, but you need to adapt set_vrefs in adc.cpp.

/* Parameters to control the menu */
extern recipe recipes_array[]; //List of recipes. 
extern int program_nmb; //Length of recipes array [0..program_nmb].
extern int program_cnt; //Current recipe


/* Declare variables to define the field sequence */
#define B_NR_MAX 24 //Max number of elements
extern int B_nr_set; //the number of elements in the array
extern int Nr_cycles; //The number of cycles through the array, a value of 0 means an infinite amound of cycles
extern int Looppar_1; //Looppar_1,2 track at which point of the field-array the program is
extern int Looppar_2; 
extern unsigned long Switching_time[B_NR_MAX]; //the time the program waits before switching to the next value of the magnetic field, in matrix to allow an alterating switching time, keep all values the same to have a constant switching time
extern double B_arrayfield_x[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the x-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_y[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the y-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_z[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the z-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern int LEDColor_array[B_NR_MAX]; // array containing color of the LED (RED, GREEN, BLUE)
extern int LEDInt_array[B_NR_MAX]; // array containing LED intensities (0-255), MagOD2 only
extern bool Gradient_x[B_NR_MAX];  //determines whether both coils must be on or just one of them for the x-direction, 0 is both on, 1 is only one on
extern bool Gradient_y[B_NR_MAX];  //determines whether both coils must be on or just one of them for the y-direction, 0 is both on, 1 is only one on
extern bool Gradient_z[B_NR_MAX];  //determines whether both coils must be on or just one of them for the z-direction, 0 is both on, 1 is only one on

/* Handles for ADC0 ISR and buffer for data */
// Handle to refer to the task that does the ADC measurement
extern TaskHandle_t readADC0Handle; 
// Handle for the buffer
extern QueueHandle_t dataQueueHandle;
// Timer to tell adc::getDataPoint to get ADC inputs. Only ADC1 for MagOD2
extern bool doMeasurementFlag;

/* ADC settings */
#if defined(_MAGOD1)
extern double adsMaxV0;  //max voltage that can be measured by the ADC
#elif defined(_MAGOD2)
extern double adsMaxV0,adsMaxV1;  //max voltages, for two ADCs
#endif


#endif
