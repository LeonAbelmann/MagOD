/* timer.cpp
 MagOD2 libary 
 March 2019
 Controls timers
 Leon Abelmann
*/

#include "Arduino.h"
#include "timer.h"


//Constructor
timer::timer()
{
  hw_timer_t * timer1 = NULL;
  hw_timer_t * timer3 = NULL;
  hw_timer_t * timer4 = NULL;
}

void timer::initTimer(int Timer, long time) /* Initializes timer*/
// Timer: which timer 1,3,4,5 for Arduino, 1,3,4 for ESP32
// Time: Time between alarms (microseconds)
{
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
}

void timer::attachInterrupt(int Timer, void (*isr)()) /* Attached timer Timer to interupt function measEvent*/
{
  switch (Timer){
  case 1 : timerAttachInterrupt(timer1, isr, true);break;/* Attach timer1 to
						      function isr,
						      automaticallly
						      reload*/
  case 3 : timerAttachInterrupt(timer3, isr, true);break;
  case 4 : timerAttachInterrupt(timer4, isr, true);break;
  default : Serial.print("Error: Timer ");Serial.print(Timer);Serial.println(" not recognized!");
  }
}





