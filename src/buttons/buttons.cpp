/* buttons.cpp
 MagOD2 libary 
 Feb 2020
 Controls buttons (joystick for MagOD1, touchscreen for MagOD2)
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "Arduino.h"
#include "../../MagOD.h"
#include "buttons.h"
#include "../screen/screen_RA8875.h" //Definition of pins
#include "touch_calibrate.h" // Calibration file for touchscreen

//Constructor
buttons::buttons(){
  // Adafruit TFT shield button definitions
#if defined(_MAGOD1)
  Adafruit_ADS1115 ads;
#elif defined(_MAGOD2)
#else
  #error "In buttons.cpp: No platform defined"
#endif
};

/* Help functions for touchscreen */
#if defined(_MAGOD2)
tsMatrix_t      _tsMatrix; // Calibrationmatrix
tsPoint_t       _tsLCDPoints[3]; // Set of calibration points
tsPoint_t       _tsTSPoints[3]; 


/* Calculate touched x,y values to real screen points
   int x, y: input x,y and recalculated output
   tsMatrix *matrixPtr: calibration matrix
*/
int calibrateTSPoint( tsPoint_t * displayPtr, tsPoint_t * screenPtr, tsMatrix_t * matrixPtr )
{
  int  retValue = 0 ;
  
  if( matrixPtr->Divider != 0 )
  {
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;

    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
  }
  else
  {
    return -1;
  }
  return( retValue );
}

/* Get coordinates from touchpanel, and recalculate to screen
   coordinates (x,y). Returns true is the touch event is clearly human
*/
boolean getTouchCoordinates(uint16_t *x, uint16_t *y){
  uint16_t a, b;//Coordinates read from touchscreen
  tsPoint_t raw; //a,b converted to point type
  tsPoint_t calibrated; //point calibrated to screen units
  /* Get first value of touched coordinates */
  myscreen.tft.touchRead(&a,&b);
  raw.x=a;
  raw.y=b;
  /* Integrate for 100 ms to get better estimate of touch position */
  int timestart= millis();
  int count=1;
  while (millis()-timestart < 100)
    {
      if (myscreen.tft.touched())
	{
	  myscreen.tft.touchRead(&a,&b);
	  raw.x=(raw.x+a)/2;/*Kind of low-pass */
	  raw.y=(raw.y+b)/2;
	  count=count+1;
	  }
    }
  //Serial.print("Count : ");Serial.println(count);
  // Ignore short pulses on touchscreen
  if (count < 10)
    {
      return false; /* Not enough points collected */
    }
  else
    { 
      /* Calculate screen position */
      calibrateTSPoint(&calibrated, &raw, &_tsMatrix );
      *x=(uint16_t)calibrated.x; /*Typecast into int. number of points
				   on screen is limited, we should be
				   fine. */
      *y=(uint16_t)calibrated.y;
      return true; /* We had enough points */
    }
}      

/* Draw button */
void drawButton(int off_x, int off_y, int size_x, int size_y,
		const char* str,
		int bgcolor, int fgcolor){
  /* (off_x, off_y)  : Upper left corner
     (size_x,size_y) : Size of button
     str             : Text in center of button (assume character is 8x10)
     bgcolor         : Color of button
     fgcolor         : Textcolor
  */
  /* Text halfway button with center. Substract half the length of
       the string */
  int text_x = off_x + round(size_x/2) - 4*strlen(str);
  int text_y = off_y + round(size_y/2) -10;
  //Draw rectangle
  myscreen.tft.fillRect(off_x,off_y,size_x,size_y, bgcolor);
  //Black edge around area
  myscreen.tft.drawRect(off_x, off_y, size_x, size_y, TFTCOLOR_BLACK);
  // Write texts
  myscreen.tft.textSetCursor(text_x, text_y);
  myscreen.tft.textTransparent(fgcolor);
  myscreen.tft.textWrite(str);
}

/* Define button area. There are four buttons, 1 and 2 are on the right, 
   BUTTON_NEXTRECIPE and BUTTON_PREVRECIPE are on the left */
void buttons::showButtonArea(int button, const char* str,
			     int bgcolor, int fgcolor)
/* button: which button [1,2,3,4]
   string : text in button
   bgcolor : which fill color TFTCOLOR_RED etc. defined in screen_RA8875.h
   fgcolor : which text color
*/
{ int off_x, off_y, size_x, size_y, text_x, text_y;
  size_x = buttonSize_x; // For now, all buttons have same size...
  size_y = buttonSize_y;
  switch (button) {
  case 1 : // top right
    off_x = buttonOffset_x+size_x;
    off_y = buttonOffset_y;
    drawButton(off_x, off_y, size_x, size_y, str, bgcolor, fgcolor);
    break;
  case 2 : // bottom right
    off_x = buttonOffset_x + size_x;
    off_y = buttonOffset_y + size_y;
    drawButton(off_x, off_y, size_x, size_y, str, bgcolor, fgcolor);
    break;
  case BUTTON_NEXTRECIPE : // top left
    off_x = buttonOffset_x;
    off_y = buttonOffset_y;
    drawButton(off_x, off_y, size_x, size_y, str, bgcolor, fgcolor);
    break;
  case BUTTON_PREVRECIPE : // bottom left
    off_x = buttonOffset_x;
    off_y = buttonOffset_y + size_y;
    drawButton(off_x, off_y, size_x, size_y, str, bgcolor, fgcolor);
    break;
  default :
    break;
  }
}
#endif

#if defined(_MAGOD2)
/* Did the user press button, if so, which one?
return 0 if button was not hit
int x,y: coordinates that were hit
*/
int buttons::whichButton(int x, int y){
  int buttonpressed = 0;
  // Serial.print(buttonOffset_x);Serial.print(" ");
  // Serial.print(buttonOffset_x + buttonSize_x);Serial.print(" ");
  // Serial.print(buttonOffset_y);Serial.print(" ");
  // Serial.println(buttonOffset_y+ buttonSize_y);
  int off_x, off_y, size_x, size_y, text_x, text_y;
  size_x = buttonSize_x; // For now, all buttons have same size...
  size_y = buttonSize_y;
  // Button 1, top right
  if ((x > buttonOffset_x+size_x)   &&
      (x < buttonOffset_x+2*size_x) &&
      (y > buttonOffset_y)          &&
      (y < buttonOffset_y + size_y)) {
    buttonpressed = 1;}
  // Button 2, bottom right
  if ((x > buttonOffset_x+size_x)   &&
      (x < buttonOffset_x+2*size_x) &&
      (y > buttonOffset_y + size_y) &&
      (y < buttonOffset_y + 2*size_y)) {
    buttonpressed = 2;}
  // Button NEXTRECIPE , top left
  if ((x > buttonOffset_x)         &&
      (x < buttonOffset_x+size_x)  &&
      (y > buttonOffset_y)         &&
      (y < buttonOffset_y + size_y)) {
    buttonpressed = BUTTON_NEXTRECIPE;}
  // Button PREV_RECIPE , bottom left
  if ((x > buttonOffset_x)         &&
      (x < buttonOffset_x+size_x)  &&
      (y > buttonOffset_y+size_y)         &&
      (y < buttonOffset_y+2*size_y)) {
    buttonpressed = BUTTON_PREVRECIPE;}
  return buttonpressed;
}
#endif //defined(_MAGOD2)

/* Setup the hardware buttons or touchscreen */
void buttons::initButton(){
#if defined(_MAGOD1)
  /* nothing to do */
#elif defined(_MAGOD2)
  Serial.println("Enable the touch screen");
  pinMode(TFT_INT, INPUT);
  digitalWrite(TFT_INT, HIGH);
  myscreen.tft.touchEnable(true);
  /* Clear any previous interrupts to avoid false buffered reads */
  uint16_t x, y;
  myscreen.tft.touchRead(&x, &y);
  delay(1);
  
  /* Load calibration factors. Defined in touch_calibrate.h */
  _tsMatrix.Divider = TOUCH_DIVIDER;
  _tsMatrix.An = TOUCH_An;
  _tsMatrix.Bn = TOUCH_Bn;
  _tsMatrix.Cn = TOUCH_Cn;
  _tsMatrix.Dn = TOUCH_Dn;
  _tsMatrix.En = TOUCH_En;
  _tsMatrix.Fn = TOUCH_Fn;

  /* create button fields */
  /* Upper left corner of left button area */
  buttonOffset_x = 3*myscreen.column_space;// + myscreen.locText_hSpace; 
  buttonOffset_y = 0;
  /* Size of a button */
  buttonSize_x = 0.5*(myscreen.screenSiz_x - 3*myscreen.column_space);
  buttonSize_y = 0.5*(7*myscreen.locText_vSpace); // 7 lines high, we want two buttons

  /* We should rename the buttons. 1,2 etc should be BUTTON_START,
  BUTTON_VREF etc. This implies that MagOD1 also needs to be
  changed. Work... LEON  */

  /* Button 1: Start/Stop */
  showButtonArea(1, (char *)"Start", TFTCOLOR_GREEN, TFTCOLOR_BLACK);

  /* Button 2: Set Vref */
  showButtonArea(2, (char *)"Set Vref", TFTCOLOR_DARKGRAY, TFTCOLOR_YELLOW);

  /* Button BUTTON_NEXTRECIPE: Toggle to next recipe in list */
  showButtonArea(BUTTON_NEXTRECIPE, (char *)"Next recipe", TFTCOLOR_RED, TFTCOLOR_BLACK);

  /* Button BUTTON_PREVRECIPE: Toggle to previous recipe in list */
  showButtonArea(BUTTON_PREVRECIPE, (char *)"Prev recipe", TFTCOLOR_RED, TFTCOLOR_BLACK);
#endif
}

//Is there a button pressed. If so, which one?
uint8_t buttons::readButton() {
#if defined(_MAGOD1)
  /* converts voltage of button into a direction */
  float a = analogRead(3);
  
  a *= 5.0;
  a /= 1024.0;
  
  //Serial.print("Button read analog = ");
  //Serial.println(a);
  if (a < 0.2) return BUTTON_DOWN;
  if (a < 1.0) return BUTTON_RIGHT;
  if (a < 1.5) return BUTTON_SELECT;
  if (a < 2.0) return BUTTON_UP;
  if (a < 3.0) return BUTTON_LEFT;
  else return BUTTON_NONE;
#elif defined(_MAGOD2)
  //Serial.println("Testing if screen is touched");
  if (myscreen.tft.touched())
    { // Read touchscreen coordinates, and convert to screen coordinates
      uint16_t x, y;
      boolean acceptable_touch;
      acceptable_touch=getTouchCoordinates(&x, &y);

      /*Force reset of touched status */
      myscreen.tft.writeReg(RA8875_INTC2, RA8875_INTC2_TP);

      if (acceptable_touch)
	{
	  Serial.print("Touchscreen coordinates: ");
	  Serial.print(x);Serial.print(", ");Serial.println(y);
	  delay(100);
	  // Determine which button was pressed:
	  Serial.print(" Button detected: ");
	  switch (whichButton(x,y))
	    {
	    case 1 :
	      Serial. println("1: BUTTON_SELECT");
	      showButtonArea(1, (char *)"Starting",
			     TFTCOLOR_WHITE, TFTCOLOR_BLACK);
	      return BUTTON_SELECT; //Start/stop button
	    case 2 :
	      Serial. println("2: BUTTON_LEFT");
	      showButtonArea(2, (char *)"Setting...",
			     TFTCOLOR_WHITE, TFTCOLOR_BLACK);
	      return BUTTON_LEFT; //Set Vref

	      // BUTTON_NEXTRECIPE is for next menu item
	    case BUTTON_NEXTRECIPE :
	      Serial. println("Button: BUTTON_NEXTRECIPE");
	      showButtonArea(BUTTON_NEXTRECIPE, (char *)"Next recipe",
			     TFTCOLOR_WHITE, TFTCOLOR_BLACK);
	      return BUTTON_NEXTRECIPE;

	      // BUTTON_UP is for previous item
	    case BUTTON_PREVRECIPE :
	      Serial. println("Button: BUTTON_PREVRECIPE");
	      showButtonArea(BUTTON_PREVRECIPE, (char *)"Prev recipe",
			     TFTCOLOR_WHITE, TFTCOLOR_BLACK);
	      return BUTTON_PREVRECIPE;

	    default:
	      Serial. println("0: BUTTON_NONE");
	      return BUTTON_NONE;
	    }
	}
      else
	{
	  delay(100);
	  return BUTTON_NONE;
	} //If acceptable touch
    }
  else
    {
      return BUTTON_NONE;
    } //If touched
#endif
} // End of readbutton

