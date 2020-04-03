#include "TestRecipes.h"
#include "src/recipes/recipes.h"

IO      myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
recipes myrecipes; //Defintion of recipe file

void setup(){
  myIO.initSerial();
}

int main(){
  
  myIO.serialPrintln("TestRecipes, expects RECIPES.CSV in current directory");

  /* check if Recipes file exists */
  char* filename = (char*)"RECIPES.CSV";
  bool recipeFile = myIO.checkFile(filename);
  if(!recipeFile)
    {
      myIO.serialPrintln("Failed to open file for writing recipes file");
      return 0;
    }

  /* Read file contents character by character and display on command
     line or serial monitor */
  while(myIO.recipeFileavailable())
    {
      myIO.serialPrint((char*)myIO.recipeFileread());
    }
  myIO.recipeFileclose();
    
  /* Read Recipes from file */
  int numRecipes= myrecipes.LoadRecipes(myrecipes.recipes_array);
  myIO.serialPrint("Number of recipes found : ");
  myIO.serialPrintln((char *)numRecipes);
}

