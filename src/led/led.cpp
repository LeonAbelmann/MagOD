/* led.cpp
 MagOD libary 
 Jan 2019
 Controls led color
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "Arduino.h"
#include "../../MagOD.h"
#include "led.h"

//Constructor
led::led(){
  int LEDs[] = {RED, GREEN, BLUE}; /* I think I do this twice. Check! LEON */
#if defined(_MAGOD1)
  /* set pins to output */
  pinMode(LED_red,   OUTPUT);
  pinMode(LED_green, OUTPUT);
  pinMode(LED_blue,  OUTPUT);
#elif defined(_MAGOD2)
  // configure LED PWM functionalitites
  ledcSetup(LEDChannelRed,   LEDfreq, LEDresolution);
  ledcSetup(LEDChannelGreen, LEDfreq, LEDresolution);
  ledcSetup(LEDChannelBlue,  LEDfreq, LEDresolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(LED_red,   LEDChannelRed);
  ledcAttachPin(LED_green, LEDChannelGreen);
  ledcAttachPin(LED_blue,  LEDChannelBlue);
 #endif
};

//Initializes the RGB led color and changes the color during the running of the program.
// Would be nice if 1, 2 and 3 were defined as RED, GREEN, BLUE constants in the MagOD2. TODO. LEON
#if defined(_MAGOD1)
void led::Set_LED_color(int color)
{
  switch (color){
  case 1:
    digitalWrite(LED_red, HIGH);
    digitalWrite(LED_green, LOW);
    digitalWrite(LED_blue, LOW);
    Serial.println("Red");
    break;
  case 2:
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_green, HIGH);
    digitalWrite(LED_blue, LOW);
    Serial.println("Green ");
    break;
  default:
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_green, LOW);
    digitalWrite(LED_blue, HIGH);
    Serial.println("Blue ");
    break;
  }
}
#elif defined(_MAGOD2)
void led::Set_LED_color(int color)
{
  switch (color){
  case RED: 
      ledcWrite(LEDChannelRed, LEDMaxRed);
      ledcWrite(LEDChannelGreen, 0);
      ledcWrite(LEDChannelBlue, 0);
      Serial.println("Switched to Red ");
      break;
  case GREEN: 
      ledcWrite(LEDChannelRed, 0);
      ledcWrite(LEDChannelGreen, LEDMaxGreen);
      ledcWrite(LEDChannelBlue, 0);
      Serial.println("Switched to Green ");
      break;
  case BLUE: 
      ledcWrite(LEDChannelRed, 0);
      ledcWrite(LEDChannelGreen, 0);
      ledcWrite(LEDChannelBlue, LEDMaxBlue);
      Serial.println("Switched to Blue ");
      break;
  }
}
#endif
