/* Recipes.h 
Sep 2019. Leon
*/
#ifndef recipes_h
#define recipes_h
#include <string> // for string class 
#include "../led/led.h"
//#include <Arduino.h>
#include "../../TestRecipes.h"

/*Not too long because we need to display it on the screen and it will consume memory*/
#define MaxNameLength       15 // Length of recipe name
#define MaxRecipes          5  // Maximum number of recipes MagOD can load
#define MaxSequenceLength   20 // Maximum number of steps per recipes (not too big, consumes memory


/* Definition of led parameters */
struct LEDpars
{ 
  int color;     //LED colors  [RED, GREEN, BLUE]
  int intensity; //LED intensity [0..255]
};

/* Definition of a measurement sequence in a recipe */
struct sequence {
  int length; // Number of steps in this sequence [0..length]
  double  Bx[MaxSequenceLength]; //Array of Bx field values [mT]
  double  By[MaxSequenceLength];
  double  Bz[MaxSequenceLength]; 
  double  time[MaxSequenceLength]; //time in ms for this step
  LEDpars led[MaxSequenceLength];  //Struct of led parameters
  //GradPars grad; //TBD LEON
};

/* Definition of recipe */
struct recipe {
  char name[MaxNameLength]; // Name of the recipe
  int N_cycles;  // Number of cycles to run, default 0 for infinite
  sequence recipe_sequence; /* Measurement sequence. Could be an array
			       in the future so that every recipe has
			       multiple sequences, one for
			       initialization, one for measurement
			       e.g. Other option is that we reserve
			       step 0 for initialization, and than
			       loop back to step 1. LEON*/
};

class recipes
{
 public:
  recipes(); /* constructor, does nothing at moment */
  
  /* Recipe definitions: general parameters */
  int version;   // Hardware version (MAGOD1, MAGOD2)
  // perhaps Parameters should go here, LEON

  /* Definition of recipes themselves*/
  recipe recipes_array[MaxRecipes]; // [0..] The number of recipes is limited, should fit on the screen.

  /* Functions to manupulate class */
  /* Load the recipe from file and stores them in the recipes_array. Return true if success. */
  int LoadRecipes(recipe recipes_array[]);

  /* Initialize the measurement arrays to those defined in recipe 'recipe_num' */
  void program_init(recipe recipes_array[],int recipe_num);
  
  /* Intialize LED parameters. Should not need to be necessary. LEON*/
  void LED_init(recipe recipes_array[],int recipe_num);
  
 private:
  /* In case we need something local */
};

#endif
