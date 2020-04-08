#include "TestRecipes.h"
//#include "src/recipes/recipes.h"

IO      myIO;      //The exact IO routines depend on defined(SD) or defined(stdio)
//recipes myrecipes; //Defintion of recipe file

void setup(){
  myIO.initSerial();
}

int main(){
  
  //myIO.serialPrintln((char *)"TestRecipes, expects RECIPES.CSV in current directory");

  /* check if Recipes file exists */
  char * filename = "RECIPES.CSV";
  bool recp = myIO.checkFile(filename);
  istrem * fp 
  myIO.recipeFile.open(filename, ios::in));
  
  if(recp.eof() != 0)
    {
      myIO.serialPrintln((char *)"Failed to open file for writing recipes file");
      return 0;
    }
	//myIO.serialPrint(recipeFile);
	std::cout << recipeFile;
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