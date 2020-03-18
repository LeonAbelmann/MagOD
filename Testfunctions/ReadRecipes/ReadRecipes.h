/* ReadRecipes.h 
 Sep 2019
 Struct definitions, global (external) variables
 Ruthvik, Leon
*/
#ifndef _ReadRecipes_h
#define _ReadRecipes_h

#include <SD.h>
#include "src/recipes/recipes.h" //User measurement recipe

/* Constants */
#define SD_CS      15 //Pin for SD card.
/* Globals */
#define RED 0
#define GREEN 1
#define BLUE 2

/* LED parameters */
#define LEDnumber 3 //Number of LEDs available
extern int LEDs[LEDnumber];
extern int LED_type; //The color of the LED e.g (RED, GREEN, BlUE)
extern int LED_intensity[LEDnumber];//The brightness of the leds
extern bool ref_all_wavelength; //Only for backwards compatibility

/* Declare variables to define the field sequence */
#define B_NR_MAX 12 //Max number of elements
extern unsigned int B_nr_set; //the number of elements in the array
extern long Nr_cycles; //The number of cycles throught the array, a value of 0 means an infinite amound of cycles

extern unsigned long Switching_time[B_NR_MAX]; //the time the program waits before switching to the next value of the magnetic field, in matrix to allow an alterating switching time, keep all values the same to have a constant switching time
extern double B_arrayfield_x[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the x-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_y[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the y-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_z[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the z-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions

extern int LEDColor_array[B_NR_MAX]; // array containing color of the LED (RED, GREEN, BLUE)
extern int LEDInt_array[B_NR_MAX]; // array containing LED intensities (0-255), MagOD2 only

extern bool Gradient_x[B_NR_MAX];  //determines whether both coils must be on or just one of them for the x-direction, 0 is both on, 1 is only one on
extern bool Gradient_y[B_NR_MAX];  //determines whether both coils must be on or just one of them for the y-direction, 0 is both on, 1 is only one on
extern bool Gradient_z[B_NR_MAX];  //determines whether both coils must be on or just one of them for the z-direction, 0 is both on, 1 is only one on

#endif
