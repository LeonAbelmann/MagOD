/* led.h
 MagOD2 libary 
 May 2019
 Controls led color
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
#ifndef led_h
#define led_h

//Pins for the RGB LED.
#define LED_red 30 //RED LED
#define LED_green 31 //Green LED
#define LED_blue 32 //BLUE LED

#define RED 1
#define GREEN 2
#define BLUE 3

class led
{
 public:
  //Colors
  led();
  int LEDs[];
  void Set_LED_color(int color); //Initializes the RGB led color and changes the color during the running of the program.
  private:
};

#endif
