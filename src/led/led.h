/* led.h
 MagOD2 libary 
 May 2019
 Controls led color
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
#ifndef led_h
#define led_h

//Pins for the RGB LED.
#if defined(_MAGOD1)
#define LED_red   30 
#define LED_green 31 
#define LED_blue  32 
#elif defined(_MAGOD2)
#define LED_red    2 
#define LED_green 32 
#define LED_blue   4
/* ESP32 can drive the LED in PWM. We need it to reduce the light intenstity */
const int LEDfreq = 10000;
const int LEDresolution = 8;
/* Note that you share these values with the pwm for the Motordrivers.
   0-2 are for the LED. 3-5 are for the motors */
const int LEDChannelRed = 0; 
const int LEDChannelGreen = 1;
const int LEDChannelBlue = 2;
#endif

#define RED 0
#define GREEN 1
#define BLUE 2

class led
{
 public:
  //Colors
  led();
  int LEDs[];
#if defined(_MAGOD1)
  void Set_LED_color(int color); //Initializes the RGB led color and changes the color during the running of the program.
#elif defined(_MAGOD2)
  void Set_LED_color(int color, int intensity[]); //MagOD2 has PWM on leds
#endif
  private:
};

#endif
