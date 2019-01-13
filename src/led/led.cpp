/* led.cpp
 MagOD2 libary 
 Oct 2018
 Controls led color
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "Arduino.h"
#include "led.h"

//Constructor
led::led(){
  //nothing to initialize
};

//Initializes the RGB led color and changes the color during the running of the program.
// Would be nice if 1, 2 and 3 were defined as RED, GREEN, BLUE constants in the MagOD2. TODO. LEON

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
    Serial.print("Blue ");
    break;
  }
}
