/* buttons.h
 MagOD2 libary 
 Oct 2018
 Controls buttons (joystick)
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
#ifndef buttons_h
#define buttons_h

#include <Adafruit_ADS1015.h>  // ADC library
#include "../../MagOD.h"

#define BUTTON_NONE 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3
#define BUTTON_UP 4
#define BUTTON_LEFT 5

class buttons
{
 public:
  buttons();//Constructor
  void initButton();//Setup buttons 
  uint8_t readButton();//Read button status, any of defined stati above
  #if defined(_MAGOD2)
  void showButtonArea(int i, const char* str, int bgcolor, int fgcolor);//Draw button i
  int buttonOffset_x, buttonOffset_y; // Upper left corner of button area
  int buttonSize_x,buttonSize_y; // Size of a button
 private:
  int whichButton(int x, int y); //Check which button is pressed on touchscreen
  #endif
};

#endif
