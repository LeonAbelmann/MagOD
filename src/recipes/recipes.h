/* Header file for definition of measurement recipes */
/* DO NOT EDIT. Change your recipe in recipes.cpp */
#ifndef recipes_h
#define recipes_h

//Globals defined in MagOD2.ino
extern uint16_t program_cnt;
extern int LED_type;
extern bool ref_all_wavelength;
extern int B_nr_set;
extern long Nr_cycles; 
extern unsigned int Looppar_1, Looppar_2;
extern double B_arrayfield_x[];
extern double B_arrayfield_y[];
extern double B_arrayfield_z[];
extern long Switching_time[];
extern int LED_switch_cycles;

class recipes
{
 public:
  recipes();
  void LED_init(); //Initialize color of led
  void program_init(); //Initialize field array
 private:
  /* any variable that you'd like to share between LED_init and program_init, but do not want to make global */
};

#endif
