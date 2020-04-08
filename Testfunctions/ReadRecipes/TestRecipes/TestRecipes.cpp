#include "./fileStdio/IO.h"
#include "TestRecipes.h"
#include<cstdio>
#include "./src/recipes/recipes.h"

IO * myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
recipes * myrecipes; //Defintion of recipe file

void setup(){
  myIO->initSerial();
}

int main(){
  
  //myIO->serialPrintln((char *)"TestRecipes, expects RECIPES.CSV in current directory");

  /* check if Recipes file exists */
  char filename[] = "RECIPES.CSV";
  char msg[1];
  int ln_count = 0;
  
  myIO = new IO(filename);
  
  if(!myIO->recipeFileavailable())
    {
      myIO->serialPrintln((char *)"Failed to open file for writing recipes file");
      return 0;
    }
	
  /* Read file contents character by character and display on command
     line or serial monitor */
 
  while(myIO->recipeFileavailable() )
    {
      myIO->recipeFileread(msg);
      myIO->serialPrint(msg);
    }
	myIO->serialPrint((char*)ln_count);
  /* Read Recipes from file */
  // int numRecipes= myrecipes->LoadRecipes(myrecipes->recipes_array);
  // myIO->serialPrint((char *)"Number of recipes found : ");

  // //I don't know how to convert int to char* without using the String class or sprintf.
  // myIO->serialPrintln((char *)numRecipes);
  myIO->recipeFileclose();
}
