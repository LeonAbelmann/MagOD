/* MagOD.h
 MagOD libary 
 July 2019
 Struct definitions, global (external) variables
 Leon Abelmann 
*/
#ifndef _MagOD_h
#define _MagOD_h

/* Define for which version of MagOD meter you are compiling 
_MAGOD1: Version with Arduino MEGA and ST7735 1.8" 128x160 screen with joystick button
/_MAGOD2: Version with ESP32 Devkit and TFTM050 5" 480x272 screen and
capacitive touch */
#define _MAGOD2

/* Declaration of stuctures */

/* Photodiode signals */
struct diodes {
  double Vdiode; /* photodector */
  double Vled; /* photodiode that measures LED directly */
  double Vscat; /* scattering photodiode */
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
};

/* Current feedback loop voltages */
struct feedbacks {
  double x;
  double y;
  double z;
};

/* MagOD libraries, should be src subdirectory MagOD.ino folder */
#include "src/timer/timer.h" // On board timers
#include "src/led/led.h" // Control of three colour LED
#include "src/buttons/buttons.h" //Control of buttons (joystick)
#include "src/field/field.h"  //Field control
#include "src/adc/adc.h" //ADC input control

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

/* The measured parameters */
extern diodes Vdiodes;   /*Signals of photodiodes */
extern references Vrefs; /* Reference photodiode signals */
extern feedbacks Vfb;    /* Settings of current feedback loop */
extern double Temperature_degrees; /* Temperature estimated from temperature sensor */

/* Calculated parameters */
extern double OD;  //Optical Density. Calculated in CaldOD()

/* Update frequencies */
extern float freq_meas; //Measurement frequency in Hz
extern float freq_screen; //Screen update frequency in Hz

/* Time parameters */
extern unsigned long time_of_data_point; //Store time when datapoint was taken
extern unsigned long time_of_start; //Time at which the measurement was started
extern unsigned long time_last_field_change; //Time since the last field step

/* LED parameters */
#define LEDnumber 3 //Number of LEDs available
extern int LEDs[LEDnumber];
extern int LED_type; //The color of the LED e.g (RED, GREEN, BlUE)
#if defined(_MAGOD2)
extern int LED_intensity[LEDnumber];//The brightness of the leds
#endif
extern int LED_switch_cycles; //The number of cycles after which the LED changes the frequency, when a 0 is entered, the LED keeps the beginning frequency during the complete measurement 
extern int Counter_cycles_led; //counter used to store the amount of complete cycles the LED has had the same colour, to check when the colour has to change (after LED_switch_cycles)
extern bool ref_all_wavelength; //Set this to 1 for specific programs where you work with multiple wavelengths in a single measurement (such that it stores the reference value of all 3 wavelengths)

/* Parameters to control the menu */
extern const uint16_t program_nmb;//Total number of menus.
extern uint16_t program_cnt; //Current program menu

/* Declare variables to define the field sequence */
#define B_NR_MAX 12 //Max number of elements
extern unsigned int B_nr_set; //the number of elements in the array
extern long Nr_cycles; //The number of cycles throught the array, a value of 0 means an infinite amound of cycles
extern unsigned int Looppar_1; //Looppar_1,2 track at which point of the field-array the program is
extern unsigned int Looppar_2; 

extern double B_arrayfield_x[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the x-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_y[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the y-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_z[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the z-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern bool Gradient_x[B_NR_MAX];  //determines whether both coils must be on or just one of them for the x-direction, 0 is both on, 1 is only one on
extern bool Gradient_y[B_NR_MAX];  //determines whether both coils must be on or just one of them for the y-direction, 0 is both on, 1 is only one on
extern bool Gradient_z[B_NR_MAX];  //determines whether both coils must be on or just one of them for the z-direction, 0 is both on, 1 is only one on
extern unsigned long Switching_time[B_NR_MAX]; //the time the program waits before switching to the next value of the magnetic field, in matrix to allow an alterating switching time, keep all values the same to have a constant switching time
  

#endif
