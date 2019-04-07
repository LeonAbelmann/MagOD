/* timer.cpp
 MagOD2 libary 
 March 2019
 Controls timers
 Leon Abelmann
*/

#include "Arduino.h"
#include "../../MagOD.h"
#include "timer.h"

#if defined(_MAGOD1)
#include <TimerOne.h> //Timer1 used for measurement frequency
#include <TimerThree.h> //Timer3 is used to set the current sensing
#include <TimerFour.h> //Timer4 used for the screen update frequency
#include <TimerFive.h> //This timer is not used, set to zero not to disturb the other timers.
#elif defined(_MAGOD2)
#else
#error "No version defined"
#endif

//Constructor
timer::timer()
{
}

void timer::initTimer(int Timer, long time) /* Initializes timer*/
// Timer: which timer 1,3,4,5 for Arduino, 1,3,4 for ESP32
// Time: Time between alarms (microseconds)
{
#if defined(_MAGOD1)
  switch (Timer){
  case 1 : Timer1.initialize(time);break;
  case 3 : Timer3.initialize(time);break;
  case 4 : Timer4.initialize(time);break;
  case 5 : Timer5.initialize(time);break;
  default : Serial.println("Error in timer.cpp: Timer not recognized!");
  }
#elif defined(_MAGOD2)
  //http://www.iotsharing.com/2017/06/how-to-use-interrupt-timer-in-arduino-esp32.html
  switch(Timer){
  case 1:
    Serial.print("Set timer 1 at ");Serial.println(time);
    timer1 = timerBegin(0, 80, true);//Timer 0, prescaler 80 (at 80 Mhz, that is 1 us), count up
    timerAlarmWrite(timer1, time,  true);//Timer, alarm value, autoreload
    timerAlarmEnable(timer1);
    break;
  case 3:
    Serial.print("Set timer 3 at ");Serial.println(time);
    timer3 = timerBegin(1, 80, true);
    timerAlarmWrite(timer3, time,  true);
    timerAlarmEnable(timer3);
    break;
  case 4:
    Serial.print("Set timer 4 at ");Serial.println(time);
    timer4 = timerBegin(2, 80, true);
    timerAlarmWrite(timer4, time,  true);
    timerAlarmEnable(timer4);
    break;
  default :  Serial.print("Error: Timer ");Serial.print(Timer);Serial.println(" not recognized!");
  }
#endif
}

void timer::attachInterrupt(int Timer, void (*isr)()) /* Attached timer Timer to interupt function measEvent*/
{
#if defined(_MAGOD1)
  switch (Timer){
  case 1 : Timer1.attachInterrupt(isr);break;
  case 3 : Timer3.attachInterrupt(isr);break;
  case 4 : Timer4.attachInterrupt(isr);break;
  case 5 : Timer5.attachInterrupt(isr);break;
  default : Serial.println("Error: Timer not recognized!");
  }
#elif defined(_MAGOD2)
  switch (Timer){
  case 1 : timerAttachInterrupt(timer1, isr, true);break;/* Attach timer1 to
						      function isr,
						      automaticallly
						      reload*/
  case 3 : timerAttachInterrupt(timer3, isr, true);break;
  case 4 : timerAttachInterrupt(timer4, isr, true);break;
  default : Serial.print("Error: Timer ");Serial.print(Timer);Serial.println(" not recognized!");
  }
#endif
}





