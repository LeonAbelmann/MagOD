/* screen_8875.cpp
 MagOD2 libary 
 Mar 2019
 Definition of screen layout and screen update functions 
 for TFT050 screen (MAGOD2 system has that one) 
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "Arduino.h"

//Avoid that Arduino IDE compiles this file when not MagOD2
#include "../../MagOD.h" //MagOD version is defined in MagOD.h
#if defined(_MAGOD2)

#include <SPI.h>
#include "screen_RA8875.h"

screen::screen(void)
{
}

/**************************************************************/
/* clearGraph */
/* Clear the graph area */
/**************************************************************/
void screen::clearGraph(){
  tft.fillRect(g_x+1, g_y+1, g_w-2, g_h-2, TFTCOLOR_BLACK);
}

/**************************************************************/
/* updateGraph */
/* Plot the next datapoint in the graph area. Store the last updated point in lastCount.*/
/* graphArray : array of datapoints */
/* graphCount : index of last datapoint in array */
/* graphLength: total length of array */
/* startTime  : starting time for data in the array (offset) */
/* graphTime  : total length of time axis */
/* led        : color of the graph (RED/GREEN/BLUE) */
/**************************************************************/
void screen::updateGraph(dataPlot *graphArray,
			 int graphCount, int graphLength,
			 long startTime, long graphTime)
{ /*
  Serial.print(" updateGraph : lastCount: ");
  Serial.print(lastCount);
  Serial.print(" , graphCount: ");
  Serial.println(graphCount);
  */
  
  /* If graphCount < lastCount than we apparently have wrapped. In
     that case clear the graph and make sure we draw from 0 */
  if (graphCount < lastCount) {
    clearGraph();
    lastCount=0; // we draw the entire array
  }
  /* draw all points since the last time this function was called */
    for(int cnt=lastCount;cnt<=graphCount;cnt++){
      //Serial.print(" cnt: ");Serial.print(cnt);
      /* Calculate coordinate of pixel along time axis */
      /* Note, int/int=int, so convert to double first */
      int x_pixel = g_x + round(g_w*(double)cnt/graphLength);
      /* Calculate coordinate of pixel along signal axis. Note that
	 (g_x,g_y) are the coordinates of the TOP left corner */
      /*
      //Serial.print(" g_y: ");Serial.print(g_y);
      //Serial.print(" g_h: ");Serial.print(g_h);
      Serial.print(" g_maxVal: ");Serial.print(g_maxVal);
      Serial.print(" g_minVal: ");Serial.print(g_minVal);
      Serial.print(" val: ");Serial.print(graphArray[cnt].val);
      */
      int y_pixel = g_y + round(g_h *
				(double)(g_maxVal - graphArray[cnt].val)
				/(g_maxVal - g_minVal));
      /* Serial.print(" x,y : ");
      Serial.print(x_pixel);
      Serial.print(" : ");
      Serial.print(y_pixel);
      */
      /* Select the color for the line, matches LED color */
      int pixelcolor = TFTCOLOR_GRAY; /* init to grey, to indicate
					 error */
      //Serial.print("Screen: LED type: ");Serial.println(led);
      switch (graphArray[cnt].color) {
      case RED:
	pixelcolor = TFTCOLOR_RED;
	break;
      case GREEN:
	pixelcolor = TFTCOLOR_GREEN;
	break;
      case BLUE:
	pixelcolor = TFTCOLOR_BLUE;
	break;
      }
      /* 
      Serial.print(" col: ");Serial.print(graphArray[cnt].color);
      Serial.println(""); //debug LEON
      */
      tft.graphicsMode();
      tft.drawPixel(x_pixel, y_pixel, pixelcolor);
    }
    lastCount = graphCount;
    /*
    Serial.print("LastCount : ");
    Serial.println(lastCount);//debug
    */
    //Insert empty space
    //tft.graphicsMode();
    //tft.drawFastVLine(g_xCursor, g_y+1, g_h-3, TFTCOLOR_BLACK);
      
    // Write scale, does not have to be done every run! LEON
    tft.textMode();/*Go back to default textMode */
    // tft.textTransparent(TFTCOLOR_YELLOW);
    // char string[15];
    // //Write min value bottom left
    // tft.textSetCursor(0,g_y+g_h-locText_vSpace);
    // dtostrf(g_minVal, 5, 3, string);
    // tft.textWrite(string,5);
    // //Write max value top left
    // tft.textSetCursor(0,g_y);
    // dtostrf(g_maxVal, 5, 3, string);
    // tft.textWrite(string,5);
}

//sets button to indicate whether the program is running
void screen::setRecButton(bool active)
{
#if defined(_MAGOD1)
  uint8_t buttonSize = 5;
  if(active){
    tft.fillCircle(screenSiz_x-buttonSize-1, buttonSize, buttonSize,
		   TFTCOLOR_RED);
  }
  else {
    tft.fillCircle(screenSiz_x-buttonSize-1, buttonSize, buttonSize,
		   TFTCOLOR_GRAY);
  }
#elif defined(_MAGOD2)
  if(active) {
    /* Change color and label on start/stop button */
    mybuttons.showButtonArea(1, (char *)"Stop",
			     TFTCOLOR_RED, TFTCOLOR_BLACK);
  }
  else {
    /* Change color and label on start/stop button */
    mybuttons.showButtonArea(1, (char *)"Start",
			     TFTCOLOR_GREEN, TFTCOLOR_BLACK);
  }
#endif
}


//setup of text on the screen 
void screen::setupScreen()
{
  screenSiz_x=SCRN_HOR,	screenSiz_h=SCRN_VERT;
  //Text area
  locText_x=0, locText_y=0; //Top left of text area
  locText_hSpace = 40, locText_vSpace=12;//Size of each line
  column_space = 85; //Space between the two columns of text data
  //Graph display
  g_x=40,g_y=88; // Top left of graph
  g_w=SCRN_HOR-g_x-1;
  g_h=SCRN_VERT-g_y-2;
  g_xCursor=g_x+1;
  g_minVal=0;//Minimum value on the graph (V)
  g_maxVal=adsMaxV0;//Maximum value on the graph (V)
  g_xCursor_prev=g_x+1;
  g_value_prev=g_y+g_h-2;
  value_min = g_maxVal;
  value_max = 0;
  lastCount = 0; //Last datapoint that was drawn

  //tft.initR(INITR_BLACKTAB); No initR in RA8875, ignore for the moment, Leon
  Serial.println("Trying to find RA8875 screen...");
  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_480x272)) {
    Serial.println("RA8875 Not Found!");
   while (1);
  }
  Serial.println("RA8875 Found");

  // Initiliaze display
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_WHITE);
  if (mirror_tft) {
    /* Mirror, defined in calibration.h */
    /* https://cdn-shop.adafruit.com/datasheets/RA8875_DS_V12_Eng.pdf
       table 5.3 reg[20 h]. Bit 3 (0x08): mirror HDIR, Bit 2 (0x04): mirror
       VDIR. Both is sum: 0x0C */
    tft.writeReg(0x20, 0x0C);
  }

  delay(100);
  /* Switch to text mode */  
  Serial.println("Screen in Textmode");
  int text0_x=10;
  int text0_y=10;
  tft.textMode();
  tft.textSetCursor(text0_x,text0_y);
  tft.textTransparent(RA8875_BLACK);
  tft.textWrite("MagOD 2.2");
  tft.textSetCursor(text0_x+100, text0_y);
#if defined(_KIST)
  tft.textWrite("KIST MagOD2");
#endif  
#if defined(_CEA)
  tft.textWrite("CEA MagOD2");
#endif  
#if defined(_BAYREUTH)
  tft.textWrite("Bayreuth MagOD2");
#endif  
#if defined(_ASTON)
  tft.textWrite("ASTON MagOD2");
#endif  
  tft.textSetCursor(text0_x, text0_y+24);
  tft.textWrite("Tijmen Hageman, Jordi Hendrix, Hans Keizer");
  tft.textSetCursor(text0_x, text0_y+48);
  tft.textWrite("Marcel Welleweerd, Dave van As, Ruthvik Bandaru");
  tft.textSetCursor(text0_x, text0_y+72);
  tft.textWrite("Rob Krawinkel");
  tft.textSetCursor(text0_x, text0_y+96);
  tft.textWrite("Leon Abelmann");
  if (true) {
    tft.textSetCursor(text0_x, text0_y+120);
    tft.textWrite("IP:");
    tft.textSetCursor(text0_x+24,text0_y+120);
    tft.textWrite(WiFi.localIP().toString().c_str());
    delay(2000);
  }
  delay(2000);
  tft.fillScreen(RA8875_BLACK);


  /* Color of all text labels */
  tft.textTransparent(TFTCOLOR_YELLOW);

  /* Left column */

  //Voltage photodiode
  tft.textSetCursor(locText_x, locText_y+0*locText_vSpace);
  tft.textWrite("Vdio:");

  //Voltage reference diode
  tft.textSetCursor(locText_x, locText_y+1*locText_vSpace);
  tft.textWrite("Vled:");
  
  //Temperature
  tft.textSetCursor(locText_x, locText_y+2*locText_vSpace);	
  tft.textWrite("Temp:");
  
  //Calculated OD
  tft.textSetCursor(locText_x, locText_y+3*locText_vSpace);	
  tft.textWrite("OD  :");

  //Current X
  tft.textSetCursor(locText_x, locText_y+4*locText_vSpace);	
  tft.textWrite("I_x :");

  //Current Y
  tft.textSetCursor(locText_x, locText_y+5*locText_vSpace);	
  tft.textWrite("I_y :");

  //Current Z
  tft.textSetCursor(locText_x, locText_y+6*locText_vSpace);	
  tft.textWrite("I_z :");

  
  /* Right Column */

  //Position 0 is used for filename
  //Position 3 is used for Vref (next to OD in left column)
  //Position 5 and 6 are for the loop counters
  //So position 1, 2, 4 are still free
  
  //Voltage reference photodiode
  tft.textSetCursor(column_space, locText_y+3*locText_vSpace);
  tft.textWrite("Vref:");
  
  //Which run in the program:
  tft.textSetCursor(column_space, locText_y+5*locText_vSpace);
  tft.textWrite("Run :");
  
  //Which step in the program:
  tft.textSetCursor(column_space, locText_y+6*locText_vSpace);
  tft.textWrite("Stp :");

  this->updateV(Vdiodes,Vrefs,0,Currents);
  this->updateInfo(0,0,0,0,"MAGOD2");
  //Draw rectangle for graph
  tft.drawRect(g_x, g_y, g_w, g_h, TFTCOLOR_WHITE);
  //tft.drawRect(g_x+1, g_y+1, g_w-2, g_h-2, TFTCOLOR_RED);

  //Buttons are loaded in initButton
}

//filling in the values on the screen
void screen::updateV(diodes Vdiodes, references Vref, double OD, feedbacks currents)
{
  /* Debug */
  /*
  Serial.print("V     : ");Serial.println(Vdiodes.Vdiode);
  Serial.print("Vled  : ");Serial.println(Vdiodes.Vled);
  Serial.print("Vref  : ");Serial.println(Vref.Vref);
  Serial.print("OD    : ");Serial.println(OD);
  Serial.print("Temp  : ");Serial.println(Temperature);
  Serial.print("FB_x  : ");Serial.println(currents.x);
  Serial.print("FB_y  : ");Serial.println(currents.y);
  Serial.print("FB_z  : ");Serial.println(currents.z);
  */
  //Clear existing data (x0,y0,x1,y1)
  //Left column
  tft.fillRect(locText_x+locText_hSpace,
	       locText_y,
	       column_space-locText_hSpace,
	       7*locText_vSpace+2, TFTCOLOR_BLACK);
  //Right column
  // tft.fillRect(column_space+locText_hSpace,
  // 	       locText_y,
  // 	       column_space-locText_hSpace,
  // 	       5*locText_vSpace+1, TFTCOLOR_BLACK);
  // Write measured voltages
  tft.textTransparent(TFTCOLOR_WHITE);
  char string[15];
  
  //Signal photodiode
  tft.textSetCursor(locText_x+locText_hSpace, locText_y+0*locText_vSpace);
  dtostrf(Vdiodes.Vdiode, 5, 3, string);
  tft.textWrite(string,5);

  //Signal reference diode
  tft.textSetCursor(locText_x+locText_hSpace, locText_y+1*locText_vSpace);
  dtostrf(Vdiodes.Vled, 5, 3, string); 
  tft.textWrite(string,5);

  //Temperature
  tft.textSetCursor(locText_x+locText_hSpace, locText_y+2*locText_vSpace);
  dtostrf(Temperature, 5, 3, string); 
  tft.textWrite(string,5);
  
  //OD
  tft.textSetCursor(locText_x+locText_hSpace, locText_y+3*locText_vSpace);
  dtostrf(OD, 5, 3, string);
  tft.textWrite(string,5);

  //Feedback current X
  tft.textSetCursor(locText_x+locText_hSpace, locText_y+4*locText_vSpace);
  dtostrf(currents.x, 5, 3, string);
  tft.textWrite(string,5);

  //Feedback current Y
  tft.textSetCursor(locText_x+locText_hSpace, locText_y+5*locText_vSpace);
  dtostrf(currents.y, 5, 3, string);
  tft.textWrite(string,5);

  //Feedback current Z
  tft.textSetCursor(locText_x+locText_hSpace, locText_y+6*locText_vSpace);
  dtostrf(currents.z, 5, 3, string);
  tft.textWrite(string,5);
}

/* update list of recipe, highight the currently selected recipe */
/* names : array of recipe titles
   N     : total number of recipes
   cnt   : the recipe that should be highlighted
*/
void screen::showRecipes(recipe recipe_arr[], int N, int cnt){
  Serial.print("Changing highlighted recipe to: ");
  Serial.print(cnt);Serial.print(" : ");
  Serial.println(recipe_arr[cnt].name);
  //Clear existing data
  tft.fillRect(locText_x+2*column_space,
	       locText_y,
	       column_space,
	       7*locText_vSpace+2, TFTCOLOR_BLACK);
  // Write the recipe names
  int text_x=locText_x+2*column_space; // left position of texts
  for (int i=0;i<=N;i++){
    //Serial.println(recipe_arr[i].name);
    tft.textSetCursor(text_x,i*locText_vSpace);
    if (i==cnt) {
      tft.textTransparent(TFTCOLOR_RED); // Highlight color
    }
    else {
      tft.textTransparent(TFTCOLOR_YELLOW); // Default color
    }
    tft.textWrite(recipe_arr[i].name,10); // Write title
  }
};

void stripfilename(char *fname){
  /* Strip everything after last /
     https://stackoverflow.com/questions/43163677/
     how-do-i-strip-a-file-extension-from-a-string-in-c/43163761*/
  
  char *end = fname + strlen(fname);
  while (end > fname && *end != '/') {
    --end;
  }
  if (end > fname) {//so we found a '/'
    *end = '\0';
  }
  /* trunctate to 10 characters*/
  if (strlen(fname) > 10) {
    end = fname +10;
    *end = '\0';
  }
}

//update program settings whenever requested
void screen::updateInfo(unsigned int Looppar_1, unsigned int Looppar_2,
			int16_t program_cnt, int led, const char *filename)
{
  // First element is filename
  // Clear existing file field
  tft.fillRect(locText_x+column_space,
	       locText_y,
	       column_space,
	       locText_vSpace+3, TFTCOLOR_BLACK);
  tft.textSetCursor(column_space,
		    locText_y);
  char filestring[40];
  filestring[0] = '\0';//strip length
  strcpy(filestring, filename);
  stripfilename(filestring);
  /*
  Serial.print("filename : ");Serial.println(filename);
  Serial.print("filestring : ");Serial.println(filestring);
  Serial.print("length : ");Serial.println(strlen(filestring));
  */
  
  tft.textTransparent(TFTCOLOR_RED);
  tft.textWrite(filestring,strlen(filestring));

  //Clear existing data
  tft.fillRect(locText_x+column_space+locText_hSpace,
	       locText_y+locText_vSpace+3,
	       column_space-locText_hSpace,
	       6*locText_vSpace+1, TFTCOLOR_BLACK);
  
  char string[5];

  //Reference signal
  //Text color matches led color :)
  switch (led) {
  case RED:
    tft.textTransparent(TFTCOLOR_RED);  
    break;
  case GREEN:
    tft.textTransparent(TFTCOLOR_GREEN);
    break;
  case BLUE:
    tft.textTransparent(TFTCOLOR_BLUE);
    break;
    }

  tft.textSetCursor(column_space+locText_hSpace,
		    locText_y+3*locText_vSpace);
  dtostrf(Vrefs.Vref, 5, 3, string); 
  tft.textWrite(string,5);

  //Run: Looppar_2, number of cycles
  tft.textTransparent(TFTCOLOR_RED);
  tft.textSetCursor(column_space+locText_hSpace,
		    locText_y+5*locText_vSpace);
  dtostrf(Looppar_2, 5, 0, string); 
  tft.textWrite(string,5);
  
  //Step: Looppar_1, which step in the cycle
  tft.textSetCursor(column_space+locText_hSpace,
		    locText_y+6*locText_vSpace);
  dtostrf(Looppar_1, 5, 0, string); 
  tft.textWrite(string,5);
}

// Update the filename field in the info column
void screen::updateFILE(char *filename)
{
  /* Loopar_ and program_cnt are globals defined in MagOD.h */
  Serial.print("UpdateFile, filename is ");
  Serial.println(filename);
  updateInfo(Looppar_1, Looppar_2, program_cnt,
	     LEDColor_array[Looppar_1],filename);
}

#endif // defined _MAGOD2
