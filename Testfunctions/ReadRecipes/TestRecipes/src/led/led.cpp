/* led.cpp
 MagOD libary 
 Jan 2019
 Controls led color
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "Arduino.h"
#include "led.h"

//Constructor
led::led(){
  int LEDs[] = {RED, GREEN, BLUE}; /* I think I do this twice. Check! LEON */
  // configure LED PWM functionalitites
  ledcSetup(LEDChannelRed,   LEDfreq, LEDresolution);
  ledcSetup(LEDChannelGreen, LEDfreq, LEDresolution);
  ledcSetup(LEDChannelBlue,  LEDfreq, LEDresolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(LED_red,   LEDChannelRed);
  ledcAttachPin(LED_green, LEDChannelGreen);
  ledcAttachPin(LED_blue,  LEDChannelBlue);
};

//Initializes the RGB led color and changes the color during the running of the program.
// Would be nice if 1, 2 and 3 were defined as RED, GREEN, BLUE constants in the MagOD2. TODO. LEON
void led::Set_LED_color(int color, int intensity)
{
  switch (color){
  case RED: 
    ledcWrite(LEDChannelRed, intensity);
      ledcWrite(LEDChannelGreen, 0);
      ledcWrite(LEDChannelBlue, 0);
      //      Vrefs.Vref=Vrefs.Vred;//Not elegant, update. We don't need Vref.Vref. LEON
      Serial.print("Switched to Red, intensity: ");
      Serial.println(intensity);
      break;
  case GREEN: 
      ledcWrite(LEDChannelRed, 0);
      ledcWrite(LEDChannelGreen, intensity);
      ledcWrite(LEDChannelBlue, 0);
      //Vrefs.Vref=Vrefs.Vgreen;
      Serial.print("Switched to Green, intensity: ");
      Serial.println(intensity);
      break;
  case BLUE: 
      ledcWrite(LEDChannelRed, 0);
      ledcWrite(LEDChannelGreen, 0);
      ledcWrite(LEDChannelBlue, intensity);
      //Vrefs.Vref=Vrefs.Vblue;
      Serial.print("Switched to Blue, intensity: ");
      Serial.println(intensity);      
      break;
  }
}

