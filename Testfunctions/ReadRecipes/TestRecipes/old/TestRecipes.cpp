#include<cstdlib>
#include<stdio.h>
#include"TestRecipes.h"

using namespace std;

#define MAX_PARAMS 20
int Recipe::count = -1; 			// initializing the Recipe_Count
char Recipe::version[32] = "MAGOD2"; 
//int count = -1;

bool param_flag;				// set when "Parameters" detected 
bool seq_flag;					// set when "Sequence" detected
//char version[20];               //Holds version

// Print to commandline, to mimic Serial.print
void serialprint(char* message) {
  cout << message;
}

// Print to commandline, to mimic Serial.println (add linefeed)
void serialprintln(char* message) {
  cout << message << '\n';
}


void TestRecipes::split_text(char * in_text, Recipe * recipe) // for splitting the line
{
    istringstream in_stream(in_text);

    size_t len; // holds the line length
    char word[20]; // holds the characters in line
    char out_text[MAX_PARAMS][20]; // holds each word in line seperated by comma
    int idx = 0; 
    int i = 0;
	
    //serialprintln(in_text);
    while(in_stream.good() && idx < MAX_PARAMS)
    {
        in_stream.getline(word, 20, ',');
		//cout << word << endl;
        
        // if (strcmp(word, "Parameters") == 0)
        // {
            // serialprintln("In parameters");
            // param_flag = true;  // true when Parameters is detected
            // return;
        // }
        if (strcmp(word, "Sequence") == 0)
        {
            recipe->seq_idx = 0; // sequence index is intialized
            seq_flag = true;	// true when Sequence is detected
            return;
        }
        else if (strcmp(word, "EndSequence") == 0)
        {
            //cout<<"In endSequence"<<endl;
            seq_flag = false;	// false when EndSequence is detected
            return;
        }
        
        len = strlen(word);
        strncpy(out_text[idx], word, len);
		//cout << out_text[idx];
        out_text[idx++][len] = '\0';
        //serialprintln(word);
    }
	if(strcmp(out_text[1], "Recipe") == 0){ // 'count + 1' if Recipe is detected
		//count+=1;
		Recipe::count++;
		strcpy(recipe[Recipe::count].name, out_text[2]);
		
		serialprintln(recipe[Recipe::count].name);
	}
    else if(seq_flag)	// assigning the sequence values to recipe parameters  
    {
        recipe->sequence[recipe->seq_idx].b_x= atof(out_text[1]); 							// B_X
        recipe->sequence[recipe->seq_idx].b_y = atof(out_text[2]);							// B_Y
        recipe->sequence[recipe->seq_idx].b_z = atof(out_text[3]);							// B_Z
        recipe->sequence[recipe->seq_idx].t_switch = atof(out_text[4]);                    // Time
        recipe->sequence[recipe->seq_idx].led_intensity = atof(out_text[6]);               //LED intensity
        recipe->sequence[recipe->seq_idx].grad_x = atof(out_text[7]);						// Grad_X
        recipe->sequence[recipe->seq_idx].grad_y = atof(out_text[8]);						// Grad_Y
        recipe->sequence[recipe->seq_idx].grad_z = atof(out_text[9]);						// Grad_Z
        strcpy(recipe->sequence[recipe->seq_idx].led_color, out_text[5]);					// LED_Color
        recipe->seq_idx++; // jumps to the next sequence
    }
    else
    {
        if(strcmp(out_text[1], "Version") == 0) 
            strcpy(recipe->version, out_text[2]); // version
        else if(strcmp(out_text[1], "N_cycles") == 0)
            recipe->n_cycles = atof(out_text[2]);// N_cycles
    }
    
    return;
}

/*Process line detects the starting character ('#','@','') */
void TestRecipes::start_line(char * line, Recipe * recipe)
{
    size_t len = strlen(line);
    switch (line[0])
    {
        case '#':
			//serialprintln(line);
            break;
            
        case '@':
			//serialprintln(line);
            this->split_text(line, recipe);
            break;
            
        default:
			//serialprintln(line);
            if (param_flag || seq_flag)
                this->split_text(line, recipe);
            break;
    }
    
}

// Reads the lines in the csv file
void file_read(TestRecipes& csv_handler)
{
    ifstream in_file(csv_handler.filename);
    int i, j;
    char line[256];
    Recipe * recipe = new Recipe[10]; // maximum 10 recipes can be read
    
    if (!in_file.is_open()) 
    {
        cout<<"Error: Could not load the file!";
        return;
    }
    for(i = 0;i < csv_handler.max_lines;i++)
    {
        if(in_file.eof())//close when end line reached
        {
            in_file.close();
            //cout<<"Warning: Reached end of file."<<endl;
            break;
        }
        else
        {
            in_file.getline(line, 256, '\n');
            if (strlen(line) == 0)
                continue;
            csv_handler.start_line(line, &recipe[Recipe::count]);
        }
        
    } 
    
    if (i == csv_handler.max_lines) //close when maximum lines reached
        in_file.close();
        
    // for (i = 0; i <= Recipe::count; i++)
    // {   
        // cout<<"Recipe "<<recipe[i].name<<endl;
        // cout<<"Version: "<<recipe[i].version<<"\t"
            // <<"N_cycles: "<<recipe[i].n_cycles<<"\t"<<endl;
        // for (j = 0; j < recipe[i].seq_idx; j++)
        // {
            // cout<<"b_x: "<<recipe[i].sequence[j].b_x<<"\t"
                // <<"b_y: "<<recipe[i].sequence[j].b_y<<"\t"
                // <<"b_z: "<<recipe[i].sequence[j].b_z<<"\t"
                // <<"t_switch: "<<recipe[i].sequence[j].t_switch<<"\t" 
                // <<"led_color: "<<recipe[i].sequence[j].led_color<<"\t"
                // <<"led_intensity: "<<recipe[i].sequence[j].led_intensity<<"\t"
                // <<"grad_x: "<<recipe[i].sequence[j].grad_x<<"\t"
                // <<"grad_y: "<<recipe[i].sequence[j].grad_y<<"\t"
                // <<"grad_z: "<<recipe[i].sequence[j].grad_z<<"\t"<<endl;
        // }
     // }
    return;
}


int main()
{
    char filename[20] = "RECIPES.csv";
    
    cout<<"\t___________________RECIPES_____________________\n\n"<<endl;
   
    TestRecipes csv_handler(filename);
	file_read(csv_handler);
	return 0;
} 
