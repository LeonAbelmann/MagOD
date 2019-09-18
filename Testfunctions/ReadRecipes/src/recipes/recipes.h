/* Recipes.h 
Sep 2019. Ruthvik, Leon
*/
#ifndef recipes_h
#define recipes_h

#include "../../ReadRecipes.h"

/* Definition of led parameters */
struct LEDpars
{ 
  int color;     //LED colors
  int intensity; //LED intensity
};

/* Definition of recipe steps */
struct step {
  double fields[3]; //Array of field values
  LEDpars led;      //Struct of led parameters 
  /* and more... */
};


class recipes
{
 public:
  recipes(); /* constructor, does nothing at moment */

  /* Recipe definition of general parameters */
  int version;   // Hardware version (MAGOD1, MAGOD2)
  int N_cycles;  // Number of cycles to run, 0 for infinite
  /* perhaps more */

  /* Definition of every single step in the recipe */
  step steps[]; // Array of step

  /* Functions to manupulate class */
  bool LoadRecipes(); //Load the recipe from file. Return true if success

 private:
  /* In case we need something local */
};

#endif
