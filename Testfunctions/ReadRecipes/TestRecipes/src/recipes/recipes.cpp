#include "recipes.h"

#define MAXPARSTRINGS 10 // Size of array that holds parameter names

//Constructor
recipes::recipes(){
  //Nothing to do
};

/* Read one line from file, return store into line. Return true if end of line
was found */
bool readOneLine(String& line){
  char ch; //single characters read from file
  bool endofline = false; //did we run into a \n character?
  bool linefound=false;
  
  // Read file until you find a non-comment line or run into the end of file
  while (myIO.recipeFileavailable() && not linefound){
      line=""; // clear line contents
    // Read next line.
    // as long as the file has more chars, and we don't run into the end of line, do
    while (myIO.recipeFileavailable() && !endofline) { 
      ch = myIO.recipeFileread(); // read one single character into ch
      if (ch != '\n') { // if it is not \n, than add it to line
	line += ch;
	//Serial.print(ch);
      }
      else {
	endofline = true;
	//Serial.println(".");
      }
    }
    //Serial.println(line);
    // Comment line starts with #, ignore it:
    if (endofline && line.startsWith("#")) {
      //Serial.print("Ignoring comment line:");
      //Serial.println(line);
      endofline=false; //force read next line
    }
    else{
      linefound=true;}
  }
  return linefound;
}

/* Search for delimiters in the line, and store the strings between
   the delimeters ch (e.g. ",") into col array of Strings. Return the
   number of col found */
int readColumns(String line, const char* ch, String col[], int numCol){
  int i=0; //counts columns
  //Serial.println("readColumns: " + line);
  // As long as there is at least two characters left and we want more columns
  while (line.length() > 1 && i<numCol){
    int pos=line.indexOf(ch);// find location of first delimeter
    //Serial.print(i);Serial.print(" , delimeter at :");
    //Serial.print(pos);Serial.print(" , substring is :");Serial.println(line.substring(0,pos));
    col[i]=line.substring(0,pos); // get part before delimeter
    //Serial.print("col[");Serial.print(i);Serial.print("] =");Serial.println(col[i]);
    i=i+1;
    line.remove(0,pos+1); // cut that part of the line, including delimeter
    //Serial.println("After remove: " + line);
  }
  // Serial.print("Columns: ");
  // for (int j=0; j<i; j++) {
  //   Serial.print(col[j]);Serial.print(" , ");
  // };
  // Serial.println();
  return i;//Length of col, [0..i-1].
}


/* Strip first and last quotes of text string. NOT TESTED! */
bool stripQuotes(String& str){
  //Should check if string really has quotes! Leon
  serialPrint((char *)"stripQuotes: ");serialPrint((char *)str);
  str.substring(1,str.length()-2);
  serialPrint((char *)" -> ");serialPrint((char *)str);
  return true;
}

/* Are two strings equal? Ignore case */
bool comparestring(String s1, String s2)
{
  //Serial.print("comparestring: ");Serial.print(s1);Serial.println(s2);
  //if (s1.equalsIgnoreCase(s2)) { Serial.println("equal"); };
  return s1.equalsIgnoreCase(s2);
}

/* Which MagOD version do we have. Return 0 if not found */
int getversion(String str){
  int version = 0;
  //Serial.print("getVersion: str= ");Serial.println(str);
  if (comparestring(str,"MAGOD1")) {version = 1;}
  else
    if (comparestring(str,"MAGOD2")) {version = 2;}
    else {serialPrint((char *)"Version not recognized");} 
  return version;
}

/* Get list of parameter names from col[], which has length N.
Non-empty names are stored in names[]. Returns length of list [0:length] */
int getparameters(String names[], String col[], int N){
  // cout << sizeof(col);
  int j=0;
  for (int i=0;( i<= 10 && i <=N) ; i++){
    // Forget the empty fields
    if (col[i]!="") {
      names[j]=col[i];
      j=j+1;}
  };
  return j-1;
}

/* convert LED color in string to int, return -1 if not recognized */
int getLEDColor(String str){
  if (comparestring(str,"GREEN")) return GREEN;
  if (comparestring(str,"RED")) return RED;
  if (comparestring(str,"BLUE")) return BLUE;
  return -1;
}

/* Read one line of the sequence and store the field and led parameter at location 'num'*/
bool readSequenceStep(String line, sequence& seq, int num){
  bool stepFound=false;
  int N=7;
  String col[N]; //We need to read 7 columns (0:6)
  int numCol; //Number of columns read
  
  numCol = readColumns(line, ",", col, N);
  // We expect six columns  (col[0] is empty):
  if (numCol>5){
    seq.Bx[num]            = col[1].toDouble();//B_x[mT]
    seq.By[num]            = col[2].toDouble();//B_y[mT]
    seq.Bz[num]            = col[3].toDouble();//B_z[mT]
    seq.time[num]          = col[4].toDouble()*1000;//T_switch[ms]
    seq.led[num].color     = getLEDColor(col[5]);//LED Color (RED, GREEN, BLUE)
    seq.led[num].intensity = col[6].toInt();//LED Intensity
    //Check if all values make sense:
    stepFound=(seq.Bx[num]            >=0 &&
	       seq.By[num]            >=0 &&
	       seq.Bz[num]            >=0 &&
	       seq.time[num]          >=0 &&
	       seq.led[num].color     >=0 &&
	       seq.led[num].intensity >=0);
  }
  return stepFound;
}

/* Read sequence of steps from file, until @ endSequence is found */
bool readSequence(sequence& seq){
  bool endSequence=false;
  String line;
  int N=2;
  String col[N]; //We need to read only 2 columns to find endSequence
  int numCol; // number of columns read
  int sequenceNumber=0;
  
  /* Read until you find EndSequence. The order of checking is
     important, first check endSequence, so that you don't read one
     line too many */
  while(not endSequence && readOneLine(line)){ 
    //Serial.print("In readSequence line : ");Serial.println(line);
    if (line.startsWith("@")){
      numCol = readColumns(line, ",", col,N);
      endSequence=(col[1]=="EndSequence");
      //if (endSequence) Serial.println("End sequence found");
    }
    else { // We assume every line has a sequence step. Can be broken! LEON
      readSequenceStep(line, seq, sequenceNumber);
      sequenceNumber=sequenceNumber+1;
      //Serial.print("sequenceNumber: ");Serial.println(sequenceNumber);
    }
  }
  seq.length=sequenceNumber-1; //Number of steps in this sequence
  //Serial.print("Readsequence: Found number of steps: ");Serial.println(sequenceNumber);
  return endSequence; //If all went well, endSequence is true
}

/* Load recipes from recipes file and store into array. Returns the
   number of recipes found */
int recipes::LoadRecipes(recipe recipes_array[]){
  int linecount=0; //For debug only
  bool error = false;
  int version = 0 ;
  String line;
  int N=10; // Maximum number of columns we expect to read
  String col[N];
  int numCol; // Number of columns found on line
  int recipeNumber; // We can load several recipes, numbered [0..MaxRecipes]
  
  recipeNumber=-1;
  while(myIO.recipeFileavailable() && not error){
    // Read one line from the recipe file, ignore comment lines
    if (readOneLine(line)) {
      linecount=linecount+1;
      //Serial.print("Line ");Serial.print(linecount);
      //Serial.print(" : ");Serial.println(line);
      if(line.startsWith("@")){
	// Split line into column fields at comma's
	numCol = readColumns(line, ",", col, N);	
	// Version number
	if (comparestring(col[1],"Version")) {
	  version=getversion(col[2]);
	  //Serial.print("Version: ");Serial.println(version);
	  if (version==0) {error=true; break;}
	};
	// Parameter list
	if (comparestring(col[1],"Parameters")) {
	  // To be done. It is good to check if the order is what we expect. LEON
	}

	// Recipe
	if (comparestring(col[1],"Recipe")
	    && recipeNumber < MaxRecipes
	    && not error){
	  // The recipe description is on the same line in col[2]:
	  //Serial.print("Recipe: ");Serial.println(col[2]);
	  recipeNumber=recipeNumber+1;//recipeNumber initialized at -1.
	  col[2].toCharArray(recipes_array[recipeNumber].name,MaxNameLength);
	  
	  //Keep reading until you find EndRecipe
	  bool recipeEnd=false;
	  while (readOneLine(line) && not recipeEnd && not error){
	    //Serial.print("in while loop line : ");Serial.println(line);
	    if (line.startsWith("@")) {
	      // Split lines into columns
	      numCol = readColumns(line, ",", col, N);
	      // Number of cycles
	      if (comparestring(col[1],"N_cycles")){
		recipes_array[recipeNumber].N_cycles = col[2].toInt();
	      }
	      // Sequence
	      if (comparestring(col[1],"Sequence")){
		if (not readSequence(recipes_array[recipeNumber].recipe_sequence))
		  { serialPrintln((char *)"Error, sequence not found");
		    error=true;
		  }
	      }
	      // End Recipe
	      recipeEnd=comparestring(col[1],"EndRecipe");
	      //if (recipeEnd) Serial.println("End recipe found");
	    };//End if line starts with @
	  };//End while readOneLine
	};//end if col[1]=Recipe
      };// end if "@"
    };//End if readOneLine for entire recipe
  }
  /* For debug, send recipe to serial port for monitoring */
  serialPrint(recipeNumber);serialPrintln(" recipes found: ");
  for (int i=0;i<recipeNumber;i++){
    serialPrint(i);Serial.print(" : \"");serialPrint(recipes_array[i].name);
    serialPrint("\" Number of steps : ");
    serialPrint(recipes_array[i].recipe_sequence.length+1);
    for (int j=0;j<=recipes_array[i].recipe_sequence.length;j++){
      serialPrint("[ ");
      serialPrint(recipes_array[i].recipe_sequence.Bx[j]);serialPrint(", ");
      serialPrint(recipes_array[i].recipe_sequence.By[j]);serialPrint(", ");
      serialPrint(recipes_array[i].recipe_sequence.Bz[j]);serialPrint(", ");
      serialPrint(recipes_array[i].recipe_sequence.time[j]);serialPrint(", ");
      serialPrint(recipes_array[i].recipe_sequence.led[j].color);serialPrint(", ");
      serialPrint(recipes_array[i].recipe_sequence.led[j].intensity);
      serialPrintln("] ");
      };
  }
  return recipeNumber; /* If succesfull recipeNumber >=0 */
}//End load recipes


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
  Serial.println("Starting program_init");delay(1000);

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

