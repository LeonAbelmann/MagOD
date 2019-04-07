/* timer.h
 MagOD2 libary 
 March 2019
 Controls the timers for Arduino and ESP32
 Leon Abelmann
*/
#ifndef timer_h
#define timer_h

class timer
{
 public:
  timer();//Constructor

  void initTimer(int Timer, long frequency); /* Initializes timer*/
  // Timer: which timer 1,3,4,5 for Arduino, 1,3,4 for ESP32
  // Frequency: which frequency

  void attachInterrupt(int Timer, void (*isr)()); /* Attached timer to
						     interupt service
						     routine */
    // Timer: which timer (1,3,4,5 for Arduino, 1-16 for ESP32
    // isr: which interupt servicing function to attach to
 private:
#if defined(_MAGOD2)
  hw_timer_t * timer1 = NULL;
  hw_timer_t * timer3 = NULL;
  hw_timer_t * timer4 = NULL;
#endif
};

#endif
