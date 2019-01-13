/* MagOD.h
 MagOD libary 
 Jan 2019
 Struct definitions
 Leon Abelmann 
*/
#ifndef MagOD_h
#define MagOD_h

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

/* Declare variables to define the field sequence */
#define B_NR_MAX 12 //Max number of elements
extern int B_nr_set; //the number of elements in the array
extern long Nr_cycles; //The number of cycles throught the array, a value of 0 means an infinite amound of cycles
extern unsigned int Looppar_1; //Looppar_1,2 track at which point of the field-array the program is
extern unsigned int Looppar_2; 

extern double B_arrayfield_x[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the x-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_y[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the y-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_z[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the z-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern bool Gradient_x[B_NR_MAX];  //determines whether both coils must be on or just one of them for the x-direction, 0 is both on, 1 is only one on
extern bool Gradient_y[B_NR_MAX];  //determines whether both coils must be on or just one of them for the y-direction, 0 is both on, 1 is only one on
extern bool Gradient_z[B_NR_MAX];  //determines whether both coils must be on or just one of them for the z-direction, 0 is both on, 1 is only one on
extern long Switching_time[B_NR_MAX]; //the time the program waits before switching to the next value of the magnetic field, in matrix to allow an alterating switching time, keep all values the same to have a constant switching time
  

#endif
