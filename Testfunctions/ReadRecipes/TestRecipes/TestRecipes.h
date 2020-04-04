//<<<<<<< HEAD
#include<fstream>
#include <sstream>
#include<iostream>
#include <cstring>

using namespace std;

struct Sequence
{
	    char led_color[32];                    //Color of LED
	    float b_x, b_y, b_z;    				// B_X, B_Y, B_Z
	    float t_switch;                    // Time
	    float led_intensity;               //LED intensity
	    float grad_x, grad_y, grad_z; 		//Grad_X, Grad_Y, Grad_Z
};

struct Recipe
{        
		// Recipe()
		// {
			// n_cycles =0;
		// }
 //Declare private variables here
        static char version[32];               //Holds version
	    int n_cycles;					// holds N_cycles
	    char name[32];                //Name of the recipe
	    
        static int count;                  // Count of Recipe
        int seq_idx;						//Counts Sequence
	    Sequence sequence[10];				// max number of sequences 10
	    
};

//Class for handling csv file
class TestRecipes
{
 private:         
 //Declare private variables here
 protected: 
        char filename[80];   //Holds filename
	    int max_lines;       //Max lines to read
	    int index;             //Position of file pointer
 public:
        TestRecipes(char * fname, int n = 256) //: filename(fname), max_lines(n)
		{
            strcpy(filename, fname);
            max_lines = n;
		}
		TestRecipes()
		{
            cout<<"Please enter the filename: ";
            cin>>filename;
            cout<<"Please enter number of lines: [maximum 256]";
            cin>>max_lines;
            if (max_lines > 256)
            {
                cout<<"Warning: Maximum possible lines exceeded, setting to default (256)!";
                max_lines = 256;
            }
		}
		void start_line(char * line, Recipe * );
		void split_text(char * , Recipe * );
		friend void file_read(TestRecipes & );
};

//=======
/* Define what you are compiling for here */
#define stdioVersion // Version that we can test on a PC
//#define SDVersion // Version that will run on an Arduino

#if defined(stdioVersion)
/* Library for file-io if we are compiling for stdio */
#include "fileStdio/IO.h"
#elif defined(SDVersion)
/* Library for file IO if we are compiling for Arduino and Flashcard */
//#include <Arduino.h>
#include "fileSD/IO.h"
#endif

/* myIO needs to be know to recipes.cpp */
extern IO myIO;

/* Declare variables to define the field sequence (originally from MagOD.h) */
#define B_NR_MAX 12 //Max number of elements
extern unsigned int B_nr_set; //the number of elements in the array
extern long Nr_cycles; //The number of cycles throught the array, a value of 0 means an infinite amound of cycles
extern unsigned int Looppar_1; //Looppar_1,2 track at which point of the field-array the program is
extern unsigned int Looppar_2; 

extern unsigned long Switching_time[B_NR_MAX]; //the time the program waits before switching to the next value of the magnetic field, in matrix to allow an alterating switching time, keep all values the same to have a constant switching time
extern double B_arrayfield_x[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the x-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_y[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the y-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions
extern double B_arrayfield_z[B_NR_MAX]; //an array containing B_Nr_set elements for the field in the z-direction, each element has to be an integer between -256 and 256 and negative numbers can be used for opposite directions

extern int LEDColor_array[B_NR_MAX]; // array containing color of the LED (RED, GREEN, BLUE)
extern int LEDInt_array[B_NR_MAX]; // array containing LED intensities (0-255), MagOD2 only

extern bool Gradient_x[B_NR_MAX];  //determines whether both coils must be on or just one of them for the x-direction, 0 is both on, 1 is only one on
extern bool Gradient_y[B_NR_MAX];  //determines whether both coils must be on or just one of them for the y-direction, 0 is both on, 1 is only one on
extern bool Gradient_z[B_NR_MAX];  //determines whether both coils must be on or just one of them for the z-direction, 0 is both on, 1 is only one on
//>>>>>>> d74e5c21e504d07a43fccb37c016bfdd42f1ffa8
