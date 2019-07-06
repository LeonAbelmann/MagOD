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
  screenSiz_x=SCRN_HOR,	screenSiz_h=SCRN_VERT;
  //Text area
  locText_x=0, locText_y=0; //Top left of text area
  locText_hSpace = 40, locText_vSpace=12;//Size of each line
  column_space = 140; //Space between the two columns of text data
  //Graph display
  g_x=0,g_y=88,g_w=SCRN_HOR-1,g_h=SCRN_VERT-g_y-2;
  g_xCursor=g_x+1;
  g_minVal=0;
  g_maxVal=1.024;
  g_xCursor_prev=g_x+1;
  g_value_prev=g_y+g_h-2;
  value_min = 1.024;
  value_max = 0;
}

void screen::updateGraph(double value, int led)
{	
  //Plot measurement point and correct for out-of-bounds
  double val_conv = (value-g_minVal)/(g_maxVal-g_minVal);
  if(val_conv<0){val_conv=0;}
  if(val_conv>1){val_conv=1;}
  val_conv = val_conv*(g_h-3);
  val_conv = round(g_y+g_h-val_conv-2);
  int pixelcolor = TFTCOLOR_GRAY;
  // Pixel color is determined by led color (1=red, 2=green, 3=blue)
  switch (led) {
  case 1:
    pixelcolor = TFTCOLOR_RED;
    break;
  case 2:
    pixelcolor = TFTCOLOR_GREEN;
    break;
  case 3:
    pixelcolor = TFTCOLOR_BLUE;
    break;
  }
  
  tft.drawPixel(g_xCursor, val_conv, pixelcolor);
  tft.drawLine(g_xCursor_prev, g_value_prev, g_xCursor, val_conv, pixelcolor);
  
  //store min and max value of a a cycle
  if (value < value_min)
    {
      value_min = value;
    }
  if (value > value_max)
    {
      value_max = value;
    }
  
  //Update counter
  g_xCursor_prev = g_xCursor;
  g_value_prev = val_conv;
  g_xCursor++;
  //rescaling of the screengraph
  if(g_xCursor>=g_x+g_w-1)
    {
      g_xCursor=g_x+1;
      g_xCursor_prev=g_xCursor;
      g_maxVal = value_max *1.004+0.002;
      g_minVal = value_min /1.004-0.002;
      value_max = value;
      value_min = value;
      
    }
  
  
  //Insert empty space
  tft.drawFastVLine(g_xCursor, g_y+1, g_h-2, TFTCOLOR_BLACK);
}

//sets button to indicate whether the program is running
void screen::setRecButton(bool active)
{
#if defined(_MAGOD1)
  uint8_t buttonSize = 5;
  if(active)
    {
      tft.fillCircle(screenSiz_x-buttonSize-1, buttonSize, buttonSize, TFTCOLOR_RED);
    }
  else
    {
      tft.fillCircle(screenSiz_x-buttonSize-1, buttonSize, buttonSize, TFTCOLOR_GRAY);
    }
#elif defined(_MAGOD2)
  if(active)
    {
      /* Change color and label on start/stop button */
      mybuttons.showButtonArea(1, (char *)"Stop", TFTCOLOR_RED, TFTCOLOR_BLACK);
    }
  else
    {
      /* Change color and label on start/stop button */
      mybuttons.showButtonArea(1, (char *)"Start", TFTCOLOR_GREEN, TFTCOLOR_YELLOW);

    }
#endif
}


//setup of text on the screen 
void screen::setupScreen()
{
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
  tft.fillScreen(RA8875_BLACK);

  /* Switch to text mode */  
  Serial.println("Screen in Textmode");
  tft.textMode();
  tft.textSetCursor(100, 100);
  tft.textTransparent(RA8875_WHITE);
  tft.textWrite("MagOD 2");
  tft.textSetCursor(100, 124);
  tft.textWrite("Tijmen Hageman, Jordi Hendrix, Hans Keizer");
  tft.textSetCursor(100, 148);
  tft.textWrite("Marcel Welleweerd, Dave van As, Leon Abelmann");
  delay(1000);

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
  tft.textWrite("OD:");

  /* Right Column */
  
  //Voltage reference photodiode
  tft.textSetCursor(column_space, locText_y+0*locText_vSpace);
    tft.textWrite("Vref:");
  
  //Filename:
  tft.textSetCursor(column_space, locText_y+1*locText_vSpace);
  tft.textWrite("FILE:");
  
  //Program:
  tft.textSetCursor(column_space, locText_y+2*locText_vSpace);
  tft.textWrite("PRG:");
  
  //Which run in the program:
  tft.textSetCursor(column_space, locText_y+3*locText_vSpace);
  tft.textWrite("RUN:");
  
  //Which step in the program:
  tft.textSetCursor(column_space, locText_y+4*locText_vSpace);
  tft.textWrite("STP:");

  this->updateV(Vdiodes,Vrefs,0);
  this->updateInfo(0,0,0,"MAGOD2");
  //Draw rectangle for graph
  tft.drawRect(g_x, g_y, g_w, g_h, TFTCOLOR_WHITE);
  //tft.drawRect(g_x+1, g_y+1, g_w-2, g_h-2, TFTCOLOR_RED);

  //Buttons are loaded in initButton
}

//filling in the values on the screen
void screen::updateV(diodes Vdiodes, references Vref, double OD)
{
  /* Debug */
  Serial.print("V     : ");Serial.println(Vdiodes.Vdiode);
  Serial.print("Vled  : ");Serial.println(Vdiodes.Vled);
  Serial.print("Vref  : ");Serial.println(Vref.Vref);
  Serial.print("OD    : ");Serial.println(OD);
  
  //Clear existing data (x0,y0,x1,y1)
  //Left column
  tft.fillRect(locText_x+locText_hSpace,
	       locText_y,
	       column_space-locText_hSpace,
	       4*locText_vSpace+1, TFTCOLOR_BLACK);
  //Right column
  tft.fillRect(column_space+locText_hSpace,
	       locText_y,
	       2*column_space-locText_hSpace,
	       5*locText_vSpace+1, TFTCOLOR_BLACK);
  // Write measured voltages
  tft.textTransparent(TFTCOLOR_RED);
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
  dtostrf(Temperature_degrees, 5, 3, string); 
  tft.textWrite(string,5);
  
  //OD
  tft.textSetCursor(locText_x+locText_hSpace, locText_y+3*locText_vSpace);
  dtostrf(OD, 5, 3, string);
  tft.textWrite(string,5);
  
}

//update program settings whenever requested
void screen::updateInfo(unsigned int Looppar_1, unsigned int Looppar_2, int16_t program_cnt, const char *filename)
{
  //Clear existing data
  tft.fillRect(locText_x+locText_hSpace,
	       locText_y+5*locText_vSpace+2,
	       column_space - locText_hSpace,
	       2*locText_vSpace, TFTCOLOR_BLACK);
  tft.fillRect(locText_x+locText_hSpace+column_space,
	       locText_y,
	       column_space,
	       7*locText_vSpace+2, TFTCOLOR_BLACK);
  
  tft.textTransparent(TFTCOLOR_RED);

  char string[5];
  char filestring[15];

  //Reference signal
  tft.textSetCursor(column_space+locText_hSpace,
		    locText_y+0*locText_vSpace);
  dtostrf(Vrefs.Vref, 5, 3, string); 
  tft.textWrite(string,5);

  //FILE
  tft.textSetCursor(column_space+locText_hSpace,
		    locText_y+1*locText_vSpace);
  strcpy(filestring, filename); /* truncate to length of filestring */
  tft.textWrite(filestring);
  
  //program number
  tft.textSetCursor(column_space+locText_hSpace,
		    locText_y+2*locText_vSpace);
  dtostrf(program_cnt, 2, 0, string); 
  tft.textWrite(string);
  
  //Run: Looppar_2, number of cycles
  tft.textSetCursor(column_space+locText_hSpace,
		    locText_y+3*locText_vSpace);
  dtostrf(Looppar_2, 2, 0, string); 
  tft.textWrite(string);
  
  //Step: Looppar_1, which step in the cycle
  tft.textSetCursor(column_space+locText_hSpace,
		    locText_y+4*locText_vSpace);
  dtostrf(Looppar_1, 2, 0, string); 
  tft.textWrite(string);
}

// Update the filename field in the info column
void screen::updateFILE(const char *filename)
{
  /* Loopar_ and program_cnt are globals defined in MagOD.h */
  updateInfo(Looppar_1, Looppar_2, program_cnt, filename);
}

#endif // defined _MAGOD2
