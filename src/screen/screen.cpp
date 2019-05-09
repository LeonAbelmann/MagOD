/* screen.h
 MagOD2 libary 
 Mar 2019
 Definition of screen layout and screen update functions 
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/


#include "Arduino.h"
#include "screen.h"
#include "../../MagOD.h" //Needed so that MagOD version is known

//Avoid that Arduino IDE compiles this file when not MagOD1
#if defined(_MAGOD1)
#warning "Screen for MAGOD1"

screen::screen()
{
  //Screen theScreen = Screen(scrn_hor, scrn_vert, &tft);
  //Screen.h defines Adafruit_GFX *tft;
  //tft = tft_p;
  locText_x=0,	locText_y=0,	locText_vSpace=10, locText_hSpace = 40;
  screenSiz_x=SCRN_HOR,	screenSiz_h=SCRN_VERT;
  g_x=0, 	g_y=70, 	g_w=SCRN_HOR, 	g_h=SCRN_VERT-g_y;
  //g_x=50, 	g_y=60, 	g_w=70, 	g_h=70;
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
  uint8_t buttonSize = 5;
  if(active)
    {
      tft.fillCircle(screenSiz_x-buttonSize-1, buttonSize, buttonSize, TFTCOLOR_RED);
    }
  else
    {
      tft.fillCircle(screenSiz_x-buttonSize-1, buttonSize, buttonSize, TFTCOLOR_GRAY);
    }
}


//setup of text on the screen 
void screen::setupScreen()
{
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(TFTCOLOR_BLACK);
  tft.setTextColor(TFTCOLOR_YELLOW);
  tft.setTextSize(0);
  
  //Voltage average slit
  tft.setCursor(locText_x, locText_y);
  tft.println("Vav:");
  
  //Voltage led reference
  tft.setCursor(locText_x, locText_y+locText_vSpace);
  tft.println("Vled:");
  
  //Voltage reflection
  tft.setCursor(locText_x, locText_y+2*locText_vSpace);	
  tft.println("Vref:");
  
  //Voltage reference
  tft.setCursor(locText_x, locText_y+3*locText_vSpace);	
  tft.println("OD:");
  
  //Filename:
  tft.setCursor(locText_x, locText_y+4*locText_vSpace);
  tft.println("FILE:");
  
  //Program:
  tft.setCursor(locText_x, locText_y+5*locText_vSpace);
  tft.println("PRG:");
  
  //Measurement frequency:
  tft.setCursor(locText_x, locText_y+6*locText_vSpace);
  tft.println("RUN:");
  
  //Frame rate:
  tft.setCursor(screenSiz_x/2+10, locText_y+6*locText_vSpace);
  tft.println("STP:");
  
  //Draw rectangle for graph
  this->updateV(Vdiodes,Vrefs,OD);
  char welcomeword[] = "WELCOME!"; 
  this->updateFILE(welcomeword);
  this->updateInfo(10,1,1);
  tft.drawRect(g_x, g_y, g_w, g_h, TFTCOLOR_WHITE);
  //tft.drawRect(g_x+1, g_y+1, g_w-2, g_h-2, TFTCOLOR_RED);
  
  //Draw recording button
  this->setRecButton(false);
}


//filling in the values on the screen
void screen::updateV(diodes Vdiodes, references Vref, double OD)
{
  //Clear existing data
  tft.fillRect(locText_x+locText_hSpace, locText_y,
	       50, 4*locText_vSpace, TFTCOLOR_BLACK);
  tft.setTextColor(TFTCOLOR_RED);
  
  //Signal photodiode
  tft.setCursor(locText_x+locText_hSpace,locText_y);
  tft.println(Vdiodes.Vdiode,5);
  
  //Signal led reference
  tft.setCursor(locText_x+locText_hSpace,locText_y+locText_vSpace);
  tft.println(Vdiodes.Vled,5);
  
  //Reference signal
  tft.setCursor(locText_x+locText_hSpace,locText_y+2*locText_vSpace);
  tft.println(Vref.Vref,5);
  
  //OD
  tft.setCursor(locText_x+locText_hSpace,locText_y+3*locText_vSpace);
  tft.println(OD,5);
}

//update program settings whenever requested
void screen::updateInfo(unsigned int Looppar_1, unsigned int Looppar_2, int16_t program_cnt)
{
  //Clear existing data
  tft.fillRect(locText_x+locText_hSpace, locText_y+5*locText_vSpace, 50, locText_vSpace, TFTCOLOR_BLACK);
  tft.fillRect(locText_x+locText_hSpace, locText_y+6*locText_vSpace, 28, 10, TFTCOLOR_BLACK);
  tft.fillRect(locText_x+locText_hSpace+screenSiz_x/2, locText_y+6*locText_vSpace, 30, 10, TFTCOLOR_BLACK);
  
  tft.setTextColor(TFTCOLOR_RED);
  
  //program number
  tft.setCursor(locText_x+locText_hSpace, locText_y+5*locText_vSpace);
  tft.println(program_cnt);
  
  //Looppar_2, number of cycles
  tft.setCursor(locText_x+locText_hSpace, locText_y+6*locText_vSpace);
  tft.println(Looppar_2,1);
  
  //Looppar_1, which step in the cycle
  tft.setCursor(locText_x+locText_hSpace+screenSiz_x/2, locText_y+6*locText_vSpace);
  tft.println(Looppar_1,1);
}

//update the line of text on the screen which indicates the program number ect..
void screen::updateFILE(const char *str)
{	
  //Clear existing data
  tft.fillRect(locText_x+locText_hSpace, locText_y+4*locText_vSpace, 100, locText_vSpace, TFTCOLOR_BLACK);
  
  tft.setTextColor(TFTCOLOR_RED);
  tft.setCursor(locText_x+locText_hSpace, 	locText_y+4*locText_vSpace);
  tft.println(str);	
}


#endif // defined _MAGOD1
