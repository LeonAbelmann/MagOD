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
const int LEDfreq = 5000;
const int LEDresolution = 8;
/* Note that you share these values with the pwm for the Motordrivers.
   0-2 are for the LED. 3-5 are for the motors */
const int LEDChannelRed = 0; 
const int LEDChannelGreen = 1;
const int LEDChannelBlue = 2;
/* Brightness of LEDs */
const int LEDMaxRed = 200;
const int LEDMaxGreen = 200;
const int LEDMaxBlue = 200;
#endif

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
