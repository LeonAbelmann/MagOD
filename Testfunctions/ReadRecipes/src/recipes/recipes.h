/* Recipes.h 
Sep 2019. Ruthvik, Leon
*/
#ifndef recipes_h
#define recipes_h

/* Definition of every step in recipe*/
struct step
{
  double fields[3]; //Array of field values
  struct LED { // Array of LED colors
    int color; //LED colors
    int intensity;//LED intensity
    /* perhaps more */
  };
};


class recipes
{
 public:
  recipes();
  /* Recipe definition */
  int version;   // Hardware version (MAGOD1, MAGOD2)
  int N_cycles;  // Number of cycles to run, 0 for infinite
  /* perhaps more */
  step steps[]; // Array with all steps in the recipe

  /* Functions */
  bool LoadRecipes(); //Initialize field array

 private:
  /* */
};

#endif
