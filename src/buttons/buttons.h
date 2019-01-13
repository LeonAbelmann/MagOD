/* buttons.h
 MagOD2 libary 
 Oct 2018
 Controls buttons (joystick)
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
#ifndef buttons_h
#define buttons_h

#include <Adafruit_ADS1015.h>  // ADC library

#define BUTTON_NONE 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3
#define BUTTON_UP 4
#define BUTTON_LEFT 5

class buttons
{
 public:
  buttons();
  //buttonreadprocesses, converts voltage of button into a direction
  uint8_t readButton();

  /* Removed by Leon
  /* //settings for changing the screen update frequency */
  /* //total number of frequencies to toggle: */
  /* static const int freq_screen_N=7;  */
  /* //vector of possible frequencies: */
  /* const float freq_screen_a[freq_screen_N] = {0.1, 0.2, 0.5, 1, 2, 5, 10}; */
  
  /* //settings for changing the measurement update frequency */
  /* //total number of measurement frequencies to toggle: */
  /* static const int freq_meas_N=9; */
  /* //vector of possible frequencies: */
  /* const float freq_meas_a[freq_meas_N] = {0.1, 0.2, 0.5, 1, 2, 5, 10, 20, 50}; */

 private:
  //none
};

#endif
