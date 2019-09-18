/* 
Leons measurement settings
*/

#include "Arduino.h"
#include "./recipes.h"

//Constructor
recipes::recipes(){
  //Nothing to do
};

bool recipes::LoadRecipes()
{   
  Serial.println("Reading Recipes from file");
  
  /* To be done. Read the file header, general variables etc. */
  /* version is defined in the class, let's hope it is global */
  version = 1; /* Example: we will read the value from file */
  int i = 0;

  /* Read the lines with measurement settings.
     For every line: */
  while (false) // while not EOF
    {
      steps[i].fields[0]=1.0;/*x value */
      steps[i].led.color=RED;
      steps[i].led.intensity=0;
    }
  return false; /* reading was unsuccesfull */
  return true;  /* reading was succesfull */
}
