#include "recipes.h"

#define MAXPARSTRINGS 10 // Size of array that holds parameter names
#define MAX_LENGTH 80


//Constructor
recipes::recipes(){ 
  //Nothing to do
};

int recipe::count = -1; // For counting number of recipes
int ln_count = 0; // For counting the lines in the csv file
char* param[MAXPARSTRINGS];
const char* data[MAXPARSTRINGS] = {"B_x","B_y","B_z","T_switch","LED_Color","LED_Intensity","Grad_x","Grad_y","Grad_z"};
bool error_flag = false;


/* Read one line from file, return store into line. Return true if end of line
was found */
bool readOneLine(char* line, File recipeFile){
  char ch; //single characters read from file
  bool endofline = false; //did we run into a \n character?
  bool linefound = false;
  int idx = 0;
  // Read file until you find a non-comment line or run into the end of file
  while (myIO->recipeFileavailable(recipeFile) && not linefound) {
    memset(line,'\0', MAX_LENGTH); // clear line contents
    // Read characters until you find end of line
    while (myIO->recipeFileavailable(recipeFile) && !endofline) {
      ch = myIO->recipeFileread(recipeFile);
      if(ch !='\n'){
	line[idx] = ch;
	idx++;
      }			
      else{
	endofline = true;
      }
    }
    // Comment line starts with #, ignore it:
    if(line[0]=='#') {
      //myIO->serialPrint((char*)"Ignoring comment line:");
      //myIO->serialPrintln(line);
     }
    else { // Ignore line with only comma:
      if(line[0]==',' && line[1]==',' && strlen(line)<15){
	//myIO->serialPrint((char*)"Ignoring line:");
	//myIO->serialPrintln(line);
      }
      else linefound = true; // We are happy we found a line
    }
  }
  //myIO->serialPrint((char*)"Line length: ");
  //myIO->serialPrint(strlen(line));
  //myIO->serialPrint((char*)"  Line found: ");
  //myIO->serialPrintln(line);
  ln_count++;
  return linefound; // Returns only false if end of file is reached
}

/* Search for delimiters in the line, and store the strings between
   the delimeters ch (e.g. ',') into col array of Strings. Return the
   number of col found */
int readColumns(char* line, char ch, char * col[], int numCol){
	int i=0; //counts columns
	int pos = 0;
	char * ln;
	// myIO->serialPrint((char*)"readColumns: ");myIO->serialPrintln(line);
	// As long as there is at least two characters left and we want more columns
	while (strlen(line)>0 && i<numCol){
	    ln =strchr(line, ch);// find location of first delimeter
		if(ln == NULL) break;
		else{
			pos = ln - line;
			//pos = strchr(line, ch) - line;
			//myIO->serialPrintln(pos);
			// get part before delimeter
			for(int j=0; j< pos; j++){
				col[i][j] = line[j];
			}
		
		col[i][pos]='\0';
		}
		// myIO->serialPrintln(col[i]);
		i++;
		line+= pos+1; // cut that part of the line, including delimeter
		// myIO->serialPrintln((char*)"After remove: ");
		// myIO->serialPrintln(line);
	}
	return i;//Length of col, [0..i-1].
}


/* Strip first and last quotes of text string. NOT TESTED! */
char* stripQuotes(char * str){
	//Should check if string really has quotes! Leon
	int i,j;
    for (i = 0; str[i] != '\0'; ++i) {
        while (str[i]=='"') {
            for (j = i; str[j] != '\0'; ++j) {
                str[j] = str[j + 1];
            }
            str[j] = '\0';
        }
    }
	return str;
}

/* Are two strings equal? Ignore case */
bool comparestring(const char* s1, const char* s2){
	//myIO->serialPrintln((char*)"comparestring: ");myIO->serialPrintln(s1);myIO->serialPrintln("\t"s2);
	//if (s1.equalsIgnoreCase(s2)) { Serial.println("equal"); };
	return strcasecmp(s1, s2) == 0;
}

/* Which MagOD version do we have. Return 0 if not found */
int getversion(char* str){
	int version = 0;
	//myIO->serialPrintln((char*)"getVersion: str= ");myIO->serialPrint(str);
	if (comparestring(str,"MAGOD1")) version = 1;
	else if (comparestring(str,"MAGOD2")) version = 2;
    else myIO->serialPrintln((char*)"Version not recognized");
	return version;
}

/* Get list of parameter names from col[], which has length N.
Non-empty names are stored in names[]. Returns length of list [0:length] */
int getparameters(char* names[], char * col[], int N){
	int j=1;
	//int k=1;
	for (int i = 0; i < N; i++){
		names[i] = new char[MAX_LENGTH];
	}
	while(j!=N){
		if (strlen(col[j]) != 0)
			strcpy(names[j-1], col[j]);
		j++;
	}
	return j-1;
}

/* convert LED color in string to int, return -1 if not recognized */
int getLEDColor(char* str){
	if (comparestring(str,"GREEN")) return GREEN;
	if (comparestring(str,"RED")) return RED;
	if (comparestring(str,"BLUE")) return BLUE;
	return -1;
}

/* Read one line of the sequence and store the field and led parameter at location 'num'*/
bool readSequenceStep(char* line, sequence& seq, int num, char* recipe_name){
  bool stepFound=false;
  int N=10;
  char * col[N]; //We need to read 7 columns (0:6)
  int numCol; //Number of columns read
  //myIO->serialPrintln(strlen(line));
  //myIO->serialPrintln(line);
  for (int i = 0; i < N; i++){
    col[i] = new char[MAX_LENGTH];
  }
  numCol = readColumns(line, ',', col, N);
  // Throws error message when negative value(s) detected and corrects to zero
  /*for(int j = 1; j < N; j++){
    if(atof(col[j]) < 0 && j!= 5){
      col[j] = (char*)"0";
      myIO->serialPrintln((char*)"");
      myIO->serialPrint((char*)"Negative value(s) detected on line number: ");
      myIO->serialPrint(ln_count);
      myIO->serialPrintln((char*)" and corrected to ZERO.");
    }
  }*/
	
  if(line[0]!='#' && getLEDColor(col[5]) < 0){
    myIO->serialPrint((char*)"Line: ");
    myIO->serialPrint(ln_count);
    myIO->serialPrint((char*)"\tError, ");
    myIO->serialPrint((char*)"' ");
    myIO->serialPrint(col[5]);
    myIO->serialPrint((char*)" '");
    myIO->serialPrintln((char*)" LED color unrecognized");
    error_flag = true;
  }
	
  //	 We expect nine columns  (col[0] is empty):
  if (numCol>5){
    seq.Bx[num]            = atof(col[1]);//B_x[mT]
    seq.By[num]            = atof(col[2]);//B_y[mT]
    seq.Bz[num]            = atof(col[3]);//B_z[mT]
    seq.time[num]          = atof(col[4])*1000;//T_switch[ms]
    seq.led[num].color     = getLEDColor(col[5]);//LED Color (RED, GREEN, BLUE)
    seq.led[num].intensity = atoi(col[6]);//LED Intensity
    seq.grad[num].grad_x = atof(col[7]);
    seq.grad[num].grad_y = atof(col[8]);
    seq.grad[num].grad_z = atof(col[9]);

    //Check if all values make sense:
    stepFound=(seq.Bx[num]>=0 &&
	       seq.By[num]>=0 && seq.Bz[num]>=0 &&
	       seq.time[num]>=0 &&
	       seq.led[num].color>=0 &&
	       seq.led[num].intensity >=0 &&
	       seq.grad[num].grad_x>=0 &&
	       seq.grad[num].grad_y>=0 &&
	       seq.grad[num].grad_z>=0);
  }
  return stepFound;
}

/* Read sequence of steps from file, until @ endSequence is found */
bool readSequence(char* line, sequence& seq, char* recipe_name,
		  File recipeFile){
  bool endSequence=false;
  int N=2;
  char * col[N]; //We need to read only 2 columns to find endSequence
  int numCol; // number of columns read
  int sequenceNumber=0;
  for (int i = 0; i < N; i++){
    col[i] = new char[MAX_LENGTH];
  }
  // myIO->serialPrintln(line);
  /* Read until you find EndSequence. The order of checking is
     important, first check endSequence, so that you don't read one
     line too many */
  while(not endSequence && readOneLine(line,recipeFile)){
    //myIO->serialPrintch(line[0]);
    if (line[0] == '@'){
      numCol = readColumns(line, ',', col, N);
      endSequence=comparestring(col[1],"EndSequence");
      if (!endSequence){
	myIO->serialPrint((char*)"Line: ");
	myIO->serialPrint(ln_count);
	myIO->serialPrintln((char*)"\tEndsequence not found");
	error_flag = true;
	break;
      }
    }
    else{ // We assume every line has a sequence step. Can be broken! LEON
      //if(line[0]=='#') continue;
      //else if(line=="\0") line+=strlen(line);
      if(line[0]!='\0'){
	//myIO->serialPrint((char*)"sequenceNumber: ");
	//myIO->serialPrintln(sequenceNumber);
	readSequenceStep(line, seq, sequenceNumber, recipe_name);
	sequenceNumber=sequenceNumber+1;
      }
    }
  }
  seq.length=sequenceNumber-1; //Number of steps in this sequence
  //myIO->serialPrintln((char*)"Readsequence: Found number of steps: ");
  //myIO->serialPrint(seq.length);
  return endSequence; //If all went well, endSequence is true
}

/* For debugging, print the loaded recipes */
void recipes::serialRecipesPrint(recipe* recipe, int numRecipes){
  for (int i=0;i<=numRecipes;i++){
    myIO->serialPrintln((char*)"");
    myIO->serialPrint(recipe[i].name); myIO->serialPrint((char*)"  ");
    myIO->serialPrint((char*)" Number of steps [0.. N] :" );
    myIO->serialPrint(recipe[i].recipe_sequence.length);
    myIO->serialPrintln((char*)"");
    for (int j=0;j<=recipe[i].recipe_sequence.length;j++){
      myIO->serialPrint((char*)"[ ");
      myIO->serialPrint(recipe[i].recipe_sequence.Bx[j]);
      myIO->serialPrint((char*)", ");
      myIO->serialPrint(recipe[i].recipe_sequence.By[j]);
      myIO->serialPrint((char*)", ");
      myIO->serialPrint(recipe[i].recipe_sequence.Bz[j]);
      myIO->serialPrint((char*)", ");
      myIO->serialPrint(recipe[i].recipe_sequence.time[j]);
      myIO->serialPrint((char*)", ");
      myIO->serialPrint(recipe[i].recipe_sequence.led[j].color);
      myIO->serialPrint((char*)", ");
      myIO->serialPrint(recipe[i].recipe_sequence.led[j].intensity);
      myIO->serialPrint((char*)", ");
      myIO->serialPrint(recipe[i].recipe_sequence.grad[j].grad_x);
      myIO->serialPrint((char*)", ");
      myIO->serialPrint(recipe[i].recipe_sequence.grad[j].grad_y);
      myIO->serialPrint((char*)", ");
      myIO->serialPrint(recipe[i].recipe_sequence.grad[j].grad_z);
      myIO->serialPrint((char*)"] ");
      myIO->serialPrintln((char*)"");
    }
  }
}

/* Load recipes from recipes file and store into array. Returns the
   number of recipes found */
int recipes::LoadRecipes(File recipeFile, recipe recipes_array[]){
  char*    line = new char[MAX_LENGTH]; //Line read from file
  int      N = 10; // Maximum number of columns we expect to read
  char*    col[N]; // Array with column values
  int      numCol; // Number of columns found on line
  bool     error = false;
  int      version = 0 ;
  int 	 read_parms; // Read the parameters
  int 	 num_parms; // Number of parameters found
  //int recipeNumber; // We can load several recipes, numbered [0..MaxRecipes]
  //int linecount = 0; //For debug only
  //recipeNumber=-1;

  //myIO->serialPrintln((char*)"LoadRecipes:");
  // Initialize 
  for (int i = 0; i < N; i++){
    col[i] = new char[MAX_LENGTH];
  }
  while(myIO->recipeFileavailable(recipeFile) && not error){
    // Read one line from the recipe file, ignore comment lines
    if (readOneLine(line,recipeFile)) {
      //linecount++;
      //myIO->serialPrintln(ln_count);
      if(line[0] == '@'){
	// Split line into column fields at comma's
	numCol = readColumns(line, ',', col, N);
	// Version number
	if (comparestring(col[1],"Version")) {
	  version=getversion(col[2]);
	  if (version==0) error=true;
	};
	// Parameter list
	if (comparestring(col[1],"Parameters")) {
	  /* To be done. It is good to check if the order is what we
	     expect. LEON */
	  if(readOneLine(line,recipeFile)){
	    // myIO->serialPrintln(line);
	    num_parms = readColumns(line, ',', col, N);
	    read_parms = getparameters(param, col, num_parms);
	    int a=0;
	    while(a!=read_parms){
	      //myIO->serialPrintln(param[a]);
	      //myIO->serialPrintln(strlen(param[a]));
	      if(strlen(param[a])==1){
		myIO->serialPrint((char*)"In RECIPE:");
		myIO->serialPrintln((char*)"\tEmpty column detected");
		error = true;
		error_flag = true;
		break;
	      }
	      else if(!comparestring(param[a],data[a])){
		myIO->serialPrint((char*)"In RECIPE:\t");
		myIO->serialPrint((char*)"'");
		myIO->serialPrint((char*)data[a]);
		myIO->serialPrint((char*)"'");
		myIO->serialPrintln((char*)" values not found");
		error = true;
		error_flag = true;
		break;
	      }
	      a++;
	    }
	  }
	}

	// Recipe
	if (comparestring(col[1],"Recipe")
	    && recipe::count < (MaxRecipes-1) && not error){
	  // The recipe description is on the same line in col[2]:
	  recipe::count++;//recipeNumber initialized at -1.
	  strncpy(recipes_array[recipe::count].name,
		  stripQuotes(col[2]),strlen(col[2])+1);
	  strcat(recipes_array[recipe::count].name, stripQuotes(col[3]));
	  //myIO->serialPrintln(recipes_array[recipe::count].name);
	  bool recipeEnd=false;
	  //Keep reading until you find EndRecipe
	  while (readOneLine(line,recipeFile) && not recipeEnd && not error){
	    //myIO->serialPrintln(line);
	    if (line[0] == '@') {
	      // Split lines into columns
	      numCol = readColumns(line, ',', col, N);
	      // Number of cycles
	      if (comparestring(col[1],"N_cycles")){
		recipes_array[recipe::count].N_cycles = atoi(col[2]);
	      }	
	      // Sequence
	      else if (comparestring(col[1],"Sequence")){
		//myIO->serialPrintln(linecount);
		if (not readSequence(line,
	       		     recipes_array[recipe::count].recipe_sequence,
			     recipes_array[recipe::count].name,
			     recipeFile)){
		  // myIO->serialPrint((char*)"Line: ");
		  // myIO->serialPrint(ln_count);
		  // myIO->serialPrintln((char*)"\tError, unable to read sequence");
		  error=true;
		}
	      }
	      else{
								
		if(not(comparestring(col[1],"EndSequence")) &&
		   not(comparestring(col[1],"EndRecipe"))){
		  myIO->serialPrint((char*)"Line: ");
		  myIO->serialPrint(ln_count);
		  myIO->serialPrint((char*)"\tError, ");
		  myIO->serialPrint((char*)"'");
		  myIO->serialPrint(col[1]);
		  myIO->serialPrint((char*)"'");
		  myIO->serialPrintln((char*)" unrecognized");
		  error = true;
		  error_flag = true;
		}
	      }
	      // End Recipe
	      recipeEnd=comparestring(col[1],"EndRecipe");
	    };//End if line starts with @
	  };//End while readOneLine
	};//end if col[1]=Recipe
      };// end if "@"
    };//End if readOneLine for entire recipe
  }
  if(!error_flag){
    myIO->serialPrint(myIO->my_file);
    myIO->serialPrintln((char*)" OK");
  }
  /* For debug, send recipe to serial port for monitoring */
  serialRecipesPrint(recipes_array,recipe::count);
  return recipe::count; /* If succesfull recipeNumber >=0 */
}//End of LoadRecipes

/* Program_init()
This is the main function to change when altering the programs. This defines what each individual program can do. In an older version, the programs were hard coded. Later we defined a RECIPES.CSV that is read from flash, which is easier for the user. The code was tweaked, so it has a lot of remnants of the hard coded period. Needs major rehaul. LEON

The most important setting is the field B_arrayfield_i[j] (i = x or y or z) (j = 1,2,3,4... B_nr_set) which stores the magnetic fields that should be applied in consecutive order. Furthermore the timing should be stored in the Switching_time[j] array, which stores how long the magnetic field is kept at the value of B_arrayfield_i[j]. The length of these arrays should always be equal to B_nr_set. Some extra parameters are 
   //LED_switch_cycles;  whether the led color should switch after a number of cycles (0 = no switch, always the same color)
   //Nr_cycles; whether the program should stop after a number of cycles (0 = never stops)
   //Period_current_sense; //frequency of the current update
   //extra_par; //the value of the extra parameter that is saved in the header of the .CSV file
//all these parameters are initialized at standard values:
    //B_arrayfield_x[j] = 0.0; 
    //B_arrayfield_y[j] = 0.0; 
    //B_arrayfield_z[j] = 0.0;
    //Switching_time[j] = 500; (ms)
    //Gradient_x[j] = 1;
    //Gradient_y[j] = 1;
    //Gradient_z[j] = 1;
    //LED_switch_cycles = 0;
    //Nr_cycles = 0;
    //Period_current_sense = 1;
    //extra_par = 0.0;
    //ONLY changes to these standard settings should be programmed (when you want B_arrayfield_x[3] to be 0 in program x the program does not need the line B_arrayfield_x[3] = 0;) 

B_nr_max is set in MagOD2.ino. Increase if you need bigger sequences:
#define B_nr_max 12 //Max number of elements

Input: 
recipe_array: array of recipes describing measurement procedure
recipe_num: which recipe the user selected

*/

void recipes::program_init(recipe recipes_array[],int recipe_num)
{   
  myIO->serialPrintln((char*)"Starting program_init");
  myIO->initSerial();
  
  /* Reset all arrays,default to do nothing for 1 second */
  B_nr_set = 1; // [0..1]
  memset(B_arrayfield_x, 0, B_NR_MAX); // Fields off
  memset(B_arrayfield_y, 0, B_NR_MAX);
  memset(B_arrayfield_z, 0, B_NR_MAX);
  memset(Switching_time,1000,B_NR_MAX); // Switching times to 100 ms
  memset(Gradient_x, 1, B_NR_MAX); // Single coil operation (1)
  memset(Gradient_y, 1, B_NR_MAX);
  memset(Gradient_z, 1, B_NR_MAX);

  /* Copy measurement settings from recipes_array. Inefficient? See above. LEON */
  B_nr_set =  recipes_array[recipe_num].recipe_sequence.length; 

  for (int i=0; i <= B_nr_set; i++){
    B_arrayfield_x[i] = recipes_array[recipe_num].recipe_sequence.Bx[i];
    B_arrayfield_y[i] = recipes_array[recipe_num].recipe_sequence.By[i];
    B_arrayfield_z[i] = recipes_array[recipe_num].recipe_sequence.Bz[i];
    Switching_time[i] = recipes_array[recipe_num].recipe_sequence.time[i];
    LEDColor_array[i] = recipes_array[recipe_num].recipe_sequence.led[i].color;
    LEDInt_array[i]   = recipes_array[recipe_num].recipe_sequence.led[i].intensity;
  }
}
