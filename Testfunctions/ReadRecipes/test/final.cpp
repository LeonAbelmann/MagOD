#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#define MAXPARSTRINGS 10

using namespace std;


class CSVCol
{
    public:
        string const& operator[](size_t index) const
        {
            return m_data[index];
        }
        size_t size() const
        {
            return m_data.size();
        }
        void readNextRow(istream& str)
        {
            string         line;
            getline(str, line);

            stringstream   lineStream(line);
            string         cell;

            m_data.clear();
            while(getline(lineStream, cell, ','))
            {
                m_data.push_back(cell);
            }
            //This checks for a trailing comma with no data after it.
            if (!lineStream && cell.empty())
            {
                //If there was a trailing comma then add an empty element.
                m_data.push_back("\n");
            }
        }
    private:
        vector<string>    m_data;
};

bool comparestring(string s1, string s2)
{
  return s1.find(s2)!=string::npos;
}

std::istream& operator>>(istream& str, CSVCol& data)
{
    data.readNextRow(str);
    return str;
}  

/* Which MagOD version do we have. Return 0 if not found */
int getversion(string str){
  int version;
  if (comparestring(str,"MAGOD1")) {version = 1;}
  else
    if (comparestring(str,"MAGOD2")) {version = 2;}
    else {
      cout<<"Version "<< str <<" not recognized";
			     version = 0;}
  return version;
}

/* Get list of parameter names, returns length of list [0:length] */
int getparameters(string names[], CSVCol col){
  // cout << sizeof(col);
  int j=0;
  for (int i=0;( i<= 10 && i <= sizeof(col) );i++){
    //cout << i <<" : " << col[i] << "\n";
    // Forget the empty fields
    if (col[i]!="") {
      names[j]=col[i];
      cout << i << ", " << j << " " << col[i] << " " << names[j] << "\n";
      j=j+1;}
  };
  return j;
}



int main(){
  ifstream file("RECIPES.csv");
  string val[] = {"B_x","B_y","B_z","T_switch","LED_Color","LED_Intensity","Grad_x","Grad_y","Grad_z"};
	
  
  CSVCol col;
  // Col reads the csv file column by column, so between all comma's
  //cout << col[0];
  bool error = false;
  int version = 0 ;
  string parameterstrings[MAXPARSTRINGS]={""};
  while(file >> col && not error)
    {
     // Comment line starts with #, ignore it:
      if(col[0]=="#"){
	// cout << "# ";
	// int i=1;
	// while(col[i]!="\n"){
	//   cout << col[i] << ", ";
	//   i=i+1;
	// }
	//cout << "\n";
	continue;
      }
      // Parameter
      else if(col[0]=="@"){
	cout << col[1] << " : " << col[2]<<"\n";
	if (col[1]=="Version") {
	  version=getversion(col[2]);
	  cout << "Version: "<< col[2] <<"recognized as: "<< version <<"\n";
	  if (version==0) {error=true; break;}
	};
	if (col[1]=="Parameters") {
	  cout << "_Parameters: " << "\n";
	  // Read next line
	  file >> col;
	  int parNamesLength=getparameters(parameterstrings,col);
	  // for (int i=0;i<parNamesLength;i++){
	  //   cout << parameterstrings[i] << ", ";
	  // }
	  // cout << "\n"
	    ;
	};
	// Second parameter should be Parameter list

	// Than we should get a set of recipes
	
	   // For every recipe, there is a set of sequences

	   // No more sequences. Done
	
	// No more recipes. Done


	
      }
      // Values
      // else if(col[0]==""){
      // 	cout << "\n";
      // 	for(int i = 1; i<5;i++){
      // 				int temp = col[i];
      // 				cout << val[i-1] << "=" << temp << "\n";
      // 				cout << "\n";
      // 	}
	
      // 	cout << val[4] << "=" << col[5] << "\n";
      // 	cout<<"\n";
      // 	for(int i = 6; i<10;i++){
      // 	  int temp = stof(col[i]);
      // 	  cout << val[i-1] << "=" << temp << "\n";
      // 	  cout << "\n";
      // 	}
    }
	int a;
	cin >> a;
}

