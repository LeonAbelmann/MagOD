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
		Recipe()
		{
			n_cycles =0;
		}
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
		// TestRecipes()
		// {
            // cout<<"Please enter the filename: ";
            // cin>>filename;
            // cout<<"Please enter number of lines: [maximum 256]";
            // cin>>max_lines;
            // if (max_lines > 256)
            // {
                // cout<<"Warning: Maximum possible lines exceeded, setting to default (256)!";
                // max_lines = 256;
            // }
		// }
		void start_line(char * line, Recipe * );
		void split_text(char * , Recipe * );
		friend void file_read(TestRecipes & );
};

