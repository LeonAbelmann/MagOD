#include "TestRecipes.h"
//#include "src/recipes/recipes.h"


IO      myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
//recipes myrecipes; //Defintion of recipe file

void setup(){
  myIO.initSerial();
}

int main(){
  
  myIO.serialPrintln((char *)"TestRecipes, expects RECIPES.CSV in current directory");

  /* check if Recipes file exists */
  char filename[] = "RECIPES.CSV";
  bool recipeFile = myIO.checkFile(filename);
  if(!recipeFile)
    {
      myIO.serialPrintln((char *)"Failed to open file for writing recipes file");
      return 0;
    }

  /* Read file contents character by character and display on command
     line or serial monitor */
  while(myIO.recipeFileavailable())
    {
      myIO.serialPrint(myIO.recipeFileread());
    }
  myIO.recipeFileclose();
    
  /* Read Recipes from file */
  //  int numRecipes= myrecipes.LoadRecipes(myrecipes.recipes_array);
  //myIO.serialPrint((char *)"Number of recipes found : ");

  // I don't know how to convert int to char* without using the String class or sprintf.
  //myIO.serialPrintln(numRecipes);
}

