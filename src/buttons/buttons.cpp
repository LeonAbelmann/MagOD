/* buttons.cpp
 MagOD2 libary 
 Oct 2018
 Controls buttons (joystick)
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "Arduino.h"
#include "buttons.h"

//Constructor
buttons::buttons(){
  // Adafruit TFT shield button definitions
  Adafruit_ADS1115 ads;
};

//buttonreadprocesses, converts voltage of button into a direction
uint8_t buttons::readButton() {
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
}



