/* buttons.h
 MagOD2 libary 
 Oct 2018
 Control the magnetic field
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
#ifndef field_h
#define field_h

#include "../../MagOD.h"
#include "../timer/timer.h"



// These should be capitalized, TODO LEON
#if defined(_MAGOD1)
#define Coil_x 44 // output pins of the coils in the x direction
#define Coil_y 45 // output pins of the coils in the y direction
#define Coil_z 46 // output pins of the coils in the z direction
/* direction of the coils in the x direction     low: output A of motor driver is then high and B is low (positive direction),     high: output A of motor driver is then low and B is high (negative direction) */
#define Dir_x 25  
#define Dir_y 26
#define Dir_z 27

  //pins for selecting a single coil by means of the mosfet switching
#elif defined(_MAGOD2)
#define CoilPinX 33 //Physical pin to which the PWM is sent for coil x
#define CoilPinY 26 //Physical pin to which the PWM is sent for coil y
#define CoilPinZ 14 //Physical pin to which the PWM is sent for coil z
#define Dir_x 25  
#define Dir_y 27
#define Dir_z 12

/* Note that we share the channels with LED driver
   Keep 0-2 for the LEDs */
#define Coil_x  3 // output channel of the coils in the x direction
#define Coil_y  4 // output channel of the coils in the y direction
#define Coil_z  5 // output channel of the coils in the z direction
#endif

//pins for the analog signals from the current sensing system and the temperature sensing
#if defined(_MAGOD1)
#define Current_read_x 15
#define Current_read_y 14
#define Current_read_z 13
#endif
/* MagOD2 uses adc1,inputs 0-3 for that */

class field
{
 public:
  unsigned long Time_current_update = 0; //stores the time at which the last new wanted PWM value update is given, to give the current sensing some time to stabilize before adapting
  double Period_current_sense = 10; //The period of current sensing in [seconds]
  
  field();
  void Init_field(); /*Sets the pins in the correct status*/
  void Current_feedback(); /* Calls the Current_feedback_calculation function for each direction (x,y,z) and then changes the outut PWM value to the newly found adapted value */
  void Init_current_feedback(); //initialization of the current sensing, used to measure the offset values of the difference amplfiers
  void SetBfieldFast(double Val_Bmag_x, double Val_Bmag_y, double Val_Bmag_z, bool Gx, bool Gy, bool Gz);/* sets the B-field and the direction for given values of the magnetic field magnitudes, the input should be the magnetic field where the sign determines the direction, the function also stores the latest set PWM values, which are used in the current feedback calculations*/
  //resets all settings for the B-field, direction and gradient
  void Reset_Bfield();
  /* function used in the current sensing timer to allow the current sensing functions to run */
  bool CheckUpdatedCurrentTime();
  // Should not be public. TODO. LEON
  unsigned long Current_wait_time = 100; //the time which has to be waited before updating the current after altering the wanted PWM value

 private:
  const int Frequency_PWM = 20000;    //set the pwm frequency of the coil-drivers
#if defined(_MAGOD2)
  const int resolution = 8; /* number bits resolution to set PWM, 1-16 bit */
#endif
  const double Res_initial_x = 3.19; //the initial resistance of the coil set in the x direction, which is used to estimate the wanted current from the initial PWM value (This is used in current feedback to adjust the measured current)
  const double Res_initial_y = 3.19; //same as above but for y
  const double Res_initial_z = 3.19; //same as above but for z
  double Comp_fact_x = 1.0; //the compensation factor of coil x indicating the change in resistance of the coil due to heating, which is used to alter the PWM values of the coils
  double Comp_fact_y = 1.0; //same as above but for coil y 
  double Comp_fact_z = 1.0; //same as above but for coil z
  double V_current_init_x = 2.5; //the offset value for the x-direction of the difference amplifier used in current sensing
  double V_current_init_y = 2.5; //same as above but for coil y 
  double V_current_init_z = 2.5; //same as above but for coil z

  //the following parameters are used to make an accurate estimate of the current when using the measured voltage, as these values depend on the exact resistor values, they differ for each op amp, this also compensates for the op-amp offset

  //current_sense to field, these values have been found by characterization measurements of the coils (measure the field and output of current sense at same time)


#if defined(_MAGOD1)
  const double Ax_neg_VT = 0.016491; //parameter A of (Field = A * (measured voltage of current sensing) + B) when current is negative for the x-direction
  const double Bx_neg_VT = 0.142405 ; //parameter B of (Field = A * (measured voltage of current sensing) + B) when current is negative for the x-direction
  const double Ax_pos_VT = 0.015052; //parameter A of (Field = A * (measured voltage of current sensing) + B) when current is positive for the x-direction
  const double Bx_pos_VT = 0.0082232; //parameter B of (Field = A * (measured voltage of current sensing) + B) when current is positive for the x-direction

  const double Ay_neg_VT = 0.015689; //same but for y
  const double By_neg_VT = 0.15482; //same but for y
  const double Ay_pos_VT = 0.0144144; //same but for y
  const double By_pos_VT = -0.013692; //same but for y

  const double Az_neg_VT = 0.0147; //same but for z
  const double Bz_neg_VT = 0.14772; //same but for z
  const double Az_pos_VT = 0.013822; //same but for z
  const double Bz_pos_VT = 0.0037237; //same but for z

  
//field to PWM, to find the required PWM values to get the wanted field, these values have been found by characterization measurements of the coils
  const double Ax_neg_TPWM = 43.798; //parameter A of (PWM = A * field + B) when current is negative for the x-direction
  const double Bx_neg_TPWM = -1.9599 ; //parameter B of (PWM = A * field + B) when current is negative for the x-direction
  const double Ax_pos_TPWM = 43.921; //parameter A of (PWM = A * field + B) when current is positive for the x-direction
  const double Bx_pos_TPWM = 0.7935; //parameter B of (PWM = A * field + B) when current is positive for the x-direction
  //device 4
  const double Ay_neg_TPWM = 44.45; //same but for y
  const double By_neg_TPWM = 0.4447; //same but for y
  const double Ay_pos_TPWM = 43.843; //same but for y
  const double By_pos_TPWM = 0.7436; //same but for y
  //device 2
  const double Az_neg_TPWM = 47.51; //same but for z
  const double Bz_neg_TPWM = -0.9038; //same but for z
  const double Az_pos_TPWM = 46.999; //same but for z
  const double Bz_pos_TPWM = -0.4293; //same but for z

#elif defined(_MAGOD2)
  /* Definition of Ax_neg_TPWM etc moved to calibration.h */

  // Only used for current feedback. This has been moved to adc.cpp.
  const double Ax_neg_VT = 0;
  const double Bx_neg_VT = 0;
  const double Ax_pos_VT = 0;
  const double Bx_pos_VT = 0;

  const double Ay_neg_VT = 0;
  const double By_neg_VT = 0;
  const double Ay_pos_VT = 0;
  const double By_pos_VT = 0;
  
  const double Az_neg_VT = 0;
  const double Bz_neg_VT = 0;
  const double Az_pos_VT = 0;
  const double Bz_pos_VT = 0;
  
#endif


  int Current_PWM_value_x; //tracks the x part of the PWM value that is currently used at the output, which may be different from the PWM value set at the B_arrayfield to compensate for the heating of the coils (changing the resistance and thus the required voltage for the same current)
  int Current_PWM_value_y; //same but for y
  int Current_PWM_value_z; //same but for z
 
  uint8_t timer3_seconds_counted = 0;
  
  double Current_feedback_calculation(double T_wanted, int Read_pin, double V_current_init, bool gradient, double A_neg, double B_neg, double A_pos, double B_pos, double Comp_fact_prev);
  void coilPwmWrite(int Coil, int PWM_value);

};
  
  
#endif
