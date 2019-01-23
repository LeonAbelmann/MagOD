/* Header file for definition of measurement recipes */
/* DO NOT EDIT. Change your recipe in recipes.cpp */
#ifndef recipes_h
#define recipes_h

#include "../../MagOD.h"

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
