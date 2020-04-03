/* Define what you are compiling for here */
#define stdioVersion // Version that we can test on a PC
//#define SDVersion // Version that will run on an Arduino

#if defined(stdioVersion)
/* Library for file-io if we are compiling for stdio */
#include "fileStdio/IO.h"
#elif defined(SDVersion)
/* Library for file IO if we are compiling for Arduino and Flashcard */
//#include <Arduino.h>
#include "fileSD/IO.h"
#endif

/* myIO needs to be know to recipes.cpp */
extern IO myIO;

/* Declare variables to define the field sequence (originally from MagOD.h) */
#define B_NR_MAX 12 //Max number of elements
extern unsigned int B_nr_set; //the number of elements in the array
extern long Nr_cycles; //The number of cycles throught the array, a value of 0 means an infinite amound of cycles
extern unsigned int Looppar_1; //Looppar_1,2 track at which point of the field-array the program is
extern unsigned int Looppar_2; 

extern unsigned long Switching_time[B_NR_MAX]; //the time the program waits before switching to the next value of the magnetic field, in matrix to allow an alterating switching time, keep all values the same to have a constant switching time
extern double B_arrayfield_x[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the x-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_y[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the y-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_z[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the z-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions

extern int LEDColor_array[B_NR_MAX]; // array containing color of the LED (RED, GREEN, BLUE)
extern int LEDInt_array[B_NR_MAX]; // array containing LED intensities (0-255), MagOD2 only

extern bool Gradient_x[B_NR_MAX];  //determines whether both coils must be on or just one of them for the x-direction, 0 is both on, 1 is only one on
extern bool Gradient_y[B_NR_MAX];  //determines whether both coils must be on or just one of them for the y-direction, 0 is both on, 1 is only one on
extern bool Gradient_z[B_NR_MAX];  //determines whether both coils must be on or just one of them for the z-direction, 0 is both on, 1 is only one on
