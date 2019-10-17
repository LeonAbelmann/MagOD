/* Recipes.h 
Sep 2019. Ruthvik, Leon
*/
#ifndef recipes_h
#define recipes_h

#include "../../ReadRecipes.h"

int MaxNameLenght = 12; /*Not too long because we need to display it on the screen */
int MaxRecipes = 25; /*Not too long because we need to display it on the screen */
int MaxSequences = 25; /*Not too long because we need to display it on the screen */


/* Definition of led parameters */
struct LEDpars
{ 
  int color;     //LED colors
  int intensity; //LED intensity
};


/* Definition of recipe steps */
struct recipe {
  char name[MaxNameLength]; //Array of field values
  int N_cycles;  // Number of cycles to run, 0 for infinite
  sequence sequences[MaxSequences]
  /* and more... */
};


/* Definition of recipe steps */
struct sequence {
  double fields[3]; //Array of field values
  int time; //time for this step (is int big enough? LEON)
  LEDpars led;      //Struct of led parameters
  //GradPars grad; //TBD LEON
};


class recipes
{
 public:
  recipes(); /* constructor, does nothing at moment */

  /* Recipe definition of general parameters */
  int version;   // Hardware version (MAGOD1, MAGOD2)
  /* perhaps more */

  /* Definition of every single step in the recipe */
  recipe recipes_array[MaxRecipes]; // Array of step

  /* Functions to manupulate class */
  bool LoadRecipes(recipe& recipes_array); //Load the recipe from file and stores them in the recipes_array. Return true if success. Did I define the pointer passing correct? LEON

 private:
  /* In case we need something local */
};

#endif
