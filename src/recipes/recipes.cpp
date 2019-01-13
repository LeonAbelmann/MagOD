/* 
Here your measurement recipe is defined. There are two locations where you program the recipe, in the function LED_init(), where you set the led color, and in the function program_init(), where you set the field values.

If you increase the number of menus, increase program_nmb in MagOD2.ino. E.g. const uint16_t program_nmb = 3;
*/

#include "Arduino.h"
#include "recipes.h"

//Constructor
recipes::recipes(){
  //Nothing to do
};


/* LED_init()
initialization program of mainly the LED color, this is not added to the function program_init because this should be executed immediately when the program number switches, such that the LED color then immediately switches and not only when the program starts. This is required for the measurement of the reference value, wich should be done with the right color
LED_type: 1 = RED, 2 = GREEN, 3 = BLUE
ref_all_wavelenght: 0 = measure only selected color, 1 = do all 3 colours
save_extra_parameter: for backward compatibility
*/
void recipes::LED_init()
{
  switch (program_cnt){
   case 1:
     LED_type = 2; /* Green only */
     ref_all_wavelength = 0;
     break;
   case 2:
     LED_type = 2;
     ref_all_wavelength = 1; /* Cycle through all LED colors */
     break;
  } 
}

/* Program_init()
This is the main function to change when altering the programs, This defines what each individual program can do. The most important setting is the field B_arrayfield_i[j] (i = x or y or z) (j = 1,2,3,4... B_nr_set) which stores the magnetic fields that should be applied in consecutive order. Furthermore the timing should be stored in the Switching_time[j] array, which stores how long the magnetic field is kept at the value of B_arrayfield_i[j]. The length of these arrays should always be equal to B_nr_set. Some extra parameters are 
   //LED_switch_cycles;  whether the led color should switch after a number of cycles (0 = no switch, always the same color)
   //Nr_cycles; whether the program should stop after a number of cycles (0 = never stops)
   //Period_current_sense; //frequency of the current update
   //extra_par; //the value of the extra parameter that is saved in the header of the .CSV file
//all these parameters are initialized at standard values:
    //B_arrayfield_x[j] = 0.0; 
    //B_arrayfield_y[j] = 0.0; 
    //B_arrayfield_z[j] = 0.0;
    //Switching_time[j] = 500; (ms)
    //Gradient_x[j] = 1;
    //Gradient_y[j] = 1;
    //Gradient_z[j] = 1;
    //LED_switch_cycles = 0;
    //Nr_cycles = 0;
    //Period_current_sense = 1;
    //extra_par = 0.0;
    //ONLY changes to these standard settings should be programmed (when you want B_arrayfield_x[3] to be 0 in program x the program does not need the line B_arrayfield_x[3] = 0;) 

B_nr_max is set in MagOD2.ino. Increase if you need bigger sequences:
#define B_nr_max 12 //Max number of elements
*/

void recipes::program_init()
{   
  Serial.print("Starting program_init");delay(1000);
  
  switch (program_cnt){
  case 1: /* Sequence for MSR-1 analysis, monocolor */
     B_nr_set = 5; // [0..5]
     B_arrayfield_x[0] = 0.4; //0.4 mT field along the light, same as how we ended
     Switching_time[0] = 1000; //Leave it on for 10 sec, giving time for saving data
     B_arrayfield_z[1] = 1; //Start with 1 mT field perpendicular to the light
     Switching_time[1] = 1000; // For 10 sec
     B_arrayfield_x[2] = 1; //Than 1 mT parallel to the field
     Switching_time[2] = 1000; // For 10 sec
     B_arrayfield_z[3] = 0.4; //0.4 mT field perpendicular to the light
     Switching_time[3] = 1000; // For 20 sec, because it will take longer
     B_arrayfield_x[4] = 0.4; // 0.4 mT parallel to field
     Switching_time[4] = 1000; // Leave it on for 10 sec 
      break;
    case 2:  /* Sequence for MSR-1 analysis, multi-color */
      LED_switch_cycles = 1; /* Change the color after every field cycle */
      B_nr_set = 5; // [0..5]
      B_arrayfield_x[0] = 0.4; //0.4 mT field along the light
      Switching_time[0] = 1000; //Giving time for saving data
      B_arrayfield_z[1] = 1; //Start with 1 mT field perpendicular to the light
      Switching_time[1] = 1000; // For 10 sec
      B_arrayfield_x[2] = 1; //Than 1 mT parallel to the field
      Switching_time[2] = 1000; // For 10 sec
      B_arrayfield_z[3] = 0.4; //0.4 mT field perpendicular to the light
      Switching_time[3] = 1000; // For 20 sec, because it will take longer
      B_arrayfield_x[4] = 0.4; // 0.4 mT parallel to field
      Switching_time[4] = 1000; // Leave it on for 10 sec 
      break;
  }
}
