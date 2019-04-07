/* MagOD.h
 MagOD libary 
 Jan 2019
 Struct definitions
 Leon Abelmann 
*/
#ifndef _MagOD_h
#define _MagOD_h

/* Define for which version of MagOD meter you are compiling 
_MAGOD1: Version with Arduino MEGA and ST7735 1.8" 128x160 screen with joystick button
/_MAGOD2: Version with ESP32 Devkit and TFTM050 5" 480x272 screen and
capacitive touch */
#define _MAGOD2

/* MagOD libraries, should be subdirectory MagOD.ino folder */
/* This is still puzzling. I want all includes in this header file,
   not only timer.h, but get errors about references not
   declared. Needs attention. Leon */
#include "src/timer/timer.h"
#include "src/led/led.h" // Control of three colour LED
#include "src/pins/pins.h" // Definition of Arduino pins 
#include "src/buttons/buttons.h" //Control of buttons (joystick)
#include "src/field/field.h"  //Field control

#if defined(_MAGOD1)
#include "src/screen/screen.h" // TFT Screen with button
#elif defined(_MAGOD2)
#include "src/screen/screen_RA8875.h" // RA8875 TFTM050 with touch 
#endif

#if defined(_MAGOD2)
//#include <FS.h> // Routines for filesystem SD Card
#endif

extern timer mytimer;
extern screen myscreen;
extern buttons mybuttons;

/* Reference diode voltages */
struct references {
  double Vref;   // Current value
  double Vred;   // Voltage for red LED
  double Vgreen; // Voltage for green LED
  double Vblue;  // Voltage for blue LED
};

/* The measured parameters */
extern double Vup; // Signal of top part of split photodiode
extern double Vdwn; // Signal of bottom part of split photodiode
extern double Vled; // Signal of reference photodiode monitoring the LED
extern double Vscat; // Signal of side scatter photodiode
extern double Temperature_degrees; //Temperature estimated from temperature sensor
extern references Vrefs; 

/* Update frequencies */
extern float freq_meas; //Measurement frequency in Hz
extern float freq_screen; //Screen update frequency in Hz

/* Calculated parameters */
extern double Vav; //(Vup+Vdwn)/2
extern double OD;  //Optical Density. Calculated in CaldOD()

/* Time parameters */
extern unsigned long time_of_data_point; //Store time when datapoint was taken
extern unsigned long time_of_start; //Time at which the measurement was started
extern unsigned long time_last_field_change; //Time since the last field step

/* LED parameters */
extern int LED_type; //The color of the LED, 1 = RED, 2 = GREEN, 3 = BlUE
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
