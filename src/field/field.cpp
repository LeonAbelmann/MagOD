/* field.cpp
 MagOD2 libary 
 Oct 2018
 Controls buttons (joystick)
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "Arduino.h"
#include "field.h"
//#include "../timer/timer.h"

/* PWM.h only inplemented for Arduino's */
/* TimerThree only implemented for Arduino's */
#if defined(_MAGOD1)
#include <PWM.h>  //Library for changing the PWM frequency of the pwm-pins
#include <TimerThree.h> //Timer 3 controls current feedback event
#elif defined(_MAGOD2)
/* Use ledcSetup, ledcAttachPin, ledcWrite, which are native? */
#else
#error "field.cpp: No version is defined"
#endif

//Constructor
field::field(){
};

void field::Init_field() /*Sets the pins in the correct status*/
{
#if defined(_MAGOD1)
  //sets the frequency for the specified pins to 20kHz
  SetPinFrequencySafe(Coil_x, Frequency_PWM);
  SetPinFrequencySafe(Coil_y, Frequency_PWM);
  SetPinFrequencySafe(Coil_z, Frequency_PWM);
#elif defined(_MAGOD2)
  // attach the channel to the GPIO2 to be controlled
  //ledcAttachPin(LED, ledChannel);
  
  //sets the frequency for the specified pins to 20kHz
  // Initialize channels 
  /*  channels 0-15, resolution 1-16 bits, freq limits depend on resolution
       ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits); https://github.com/espressif/esp-idf/blob/master/docs/en/api-reference/peripherals/ledc.rst
  I guessed 11, Leon
  */
  ledcSetup(Coil_x, Frequency_PWM, 11);
  ledcSetup(Coil_y, Frequency_PWM, 11);
  ledcSetup(Coil_z, Frequency_PWM, 11);
#endif
  
  //sets the direction pin as output
  pinMode(Dir_x, OUTPUT);
  pinMode(Dir_y, OUTPUT);
  pinMode(Dir_z, OUTPUT);

  //sets the mosfet pins as output
  pinMode(Relay_x, OUTPUT);
  pinMode(Relay_y, OUTPUT);
  pinMode(Relay_z, OUTPUT);
}
  
void field::SetBfieldFast(double Val_Bmag_x, double Val_Bmag_y, double Val_Bmag_z, bool Gx, bool Gy, bool Gz)
/* sets the B-field and the direction for given values of the magnetic field magnitudes, the input should be the magnetic field where the sign determines the direction, the function also stores the latest set PWM values, which are used in the current feedback calculations*/
{
  double fact_x;
  double fact_y;
  double fact_z;

  //these functions sets the gradient for each direction
  //fact_i is the extra factor for the gradient, PWM must be twice as high to get same field with one coil, compared to getting the field with 2 coils
  if (Gx ==1)
   {
      digitalWrite(Relay_x, HIGH); 
      fact_x = 1.0;
   }
   else
   {
      digitalWrite(Relay_x, LOW); 
      fact_x = 2.0;
   }


  if (Gy ==1)
   {
      digitalWrite(Relay_y, HIGH); 
      fact_y = 1.0;
   }
   else
   {
      digitalWrite(Relay_y, LOW);
      fact_y = 2.0; 
   }


   if (Gz ==1)
   {
      digitalWrite(Relay_z, HIGH);
      fact_z = 1.0; 
   }
   else
   {
      digitalWrite(Relay_z, LOW); 
      fact_z = 2.0;
   }

   
  //first checks whether the field is positive, negative or 0 (is required because otherwise the offset (B-term) in linearization of relation between Field and PWM is non-zero)
  //whether the B-field is negative or positive determines the direction of the motor driver (Dir_i)
  //the PWM value is calculated from the wanted B-field (using the characterization measurements giving the parameters A and B) teh compensationfactors for a gradient (use 1 coil) and for heating (current sense) are then used to calculate the PWM value required
  //compensation factors do not influence the offset of the sensor and thus not affect B
  if (Val_Bmag_x < -0.0001)
    { 
      Current_PWM_value_x =
	round(
	   abs(fact_x * Comp_fact_x*Ax_neg_TPWM * Val_Bmag_x + Bx_neg_TPWM)
	     );
#if defined(_MAGOD1)
      pwmWrite(Coil_x, Current_PWM_value_x);
#elif defined(_MAGOD2)
      /* ledcWrite(channel, dutycycle)
	 note that dutycycle ranges from 0-2^(resolution) set in ledcSetup */
      ledcWrite(Coil_x, Current_PWM_value_x);
#endif
      digitalWrite(Dir_x, HIGH);
    }
   else if (Val_Bmag_x > 0.0001)
   {
      Current_PWM_value_x =
	round(
	   abs(fact_x * Comp_fact_x*Ax_pos_TPWM * Val_Bmag_x + Bx_pos_TPWM)
	     );
      coilPwmWrite(Coil_x, Current_PWM_value_x);
      digitalWrite(Dir_x, LOW);
    }
    else
    {
      Current_PWM_value_x = 0;
      coilPwmWrite(Coil_x, Current_PWM_value_x);
      digitalWrite(Dir_x, LOW);
    }
    
   if (Val_Bmag_y < -0.0001)
    {
       Current_PWM_value_y =
	 round(
	       abs(fact_y *Comp_fact_y*Ay_neg_TPWM * Val_Bmag_y + By_neg_TPWM)
	       );
     coilPwmWrite(Coil_y, Current_PWM_value_y);
       digitalWrite(Dir_y, HIGH);
    }
   else if (Val_Bmag_y > 0.0001)
   {
      Current_PWM_value_y = round(abs(fact_y * Comp_fact_y*Ay_pos_TPWM * Val_Bmag_y + By_pos_TPWM));
    coilPwmWrite(Coil_y, Current_PWM_value_y);
      digitalWrite(Dir_y, LOW);
    }
   else
   {
      Current_PWM_value_y = 0;
    coilPwmWrite(Coil_y, Current_PWM_value_y);
      digitalWrite(Dir_y, LOW);
   }
   if (Val_Bmag_z < -0.0001)
    {
       
       
       Current_PWM_value_z = round(abs(fact_z * Comp_fact_z*Az_neg_TPWM * Val_Bmag_z + Bz_neg_TPWM));
     coilPwmWrite(Coil_z, Current_PWM_value_z);
       digitalWrite(Dir_z, HIGH);
    }
   else if (Val_Bmag_z > 0.0001)
   {
      
      Current_PWM_value_z = round(abs(fact_z * Comp_fact_z*Az_pos_TPWM * Val_Bmag_z + Bz_pos_TPWM));
    coilPwmWrite(Coil_z, Current_PWM_value_z);
      digitalWrite(Dir_z, LOW);
    }
    else
    {
      Current_PWM_value_z = 0;
    coilPwmWrite(Coil_z, Current_PWM_value_z);
      digitalWrite(Dir_z, LOW);
    }

    //print the PWM values for debugging
    //Serial.print("PWM_x: ");
    //Serial.println(Current_PWM_value_x);
    //Serial.print("PWM_y: ");
    //Serial.println(Current_PWM_value_y);
    //Serial.print("PWM_z: ");
    //Serial.println(Current_PWM_value_z);
   
   
 }


double field::Current_feedback_calculation(double T_wanted, int Read_pin, double V_current_init, bool gradient, double A_neg, double B_neg, double A_pos, double B_pos, double Comp_fact_prev)
/*Calculate the difference between the measured magnetic field and wanted field (caused by for instance heating of the coil which changes the resistance), and as result gives a compensation factor which is used to alter the driving voltage of the coils to get the wanted fields*/
{
  double Val_meas = 0;             //the measured value of the analog input, which is averaged over a number of cycles
  int Nmb_cycles = 10;          //the number of cycles over which is averaged to filter the input pins
  double T_meas = 0;      //the measured current by the current sensing circuit
  
  double Compensation_factor;

  //measure and average the current
  for (int i=0; i < Nmb_cycles; i++){ 
      Val_meas += analogRead(Read_pin); 
   }      
  Val_meas /= Nmb_cycles;    //find the average measured value by dividing over the number of measurements 


  double Val_comp = (Val_meas-V_current_init); //subtracts the offset of the difference amplifying circuit
  //if the gradient is not 1, only a single coil is used instead of two and the relation between the field and current changes (current through 2 coils is twice as high as through 1 coil)
  if (gradient == 1)
  {
      if (T_wanted < 0){
          T_meas = -abs(A_neg*Val_comp + B_neg);  //uses the charaterization to convert the current into a magnetic field
        }
      else{
          T_meas = abs(A_pos*Val_comp + B_pos);
        }
  }
  else
  {
      if (T_wanted < 0)
      {
          T_meas = -abs(A_neg/2*Val_comp + B_neg);
      }
      else
      {
          T_meas = abs(A_pos/2*Val_comp + B_pos);
      }
  }

 //print current sense values for characterization or debugging
  //Serial.print("current sense value: ");
  //Serial.println(Val_comp);
  //Serial.print("wanted field: ");
  //Serial.println(T_wanted);
  //Serial.print("obtained field ");
  //Serial.println(T_meas);

  //calculation of compensation factor of magnetic field to adjust the field to the wanted value
  Compensation_factor = T_wanted/(T_meas/Comp_fact_prev);

 
  //sets some limits for the compensation factor as safety to keep the PWM value within reasinable limits of the initially set one
  if (Compensation_factor > (1.2)){
    Compensation_factor = 1.2;
  }
  if (Compensation_factor < (0.85)){
    Compensation_factor = 0.85;
  }
  //the function returns the compensation factor which is the used in the coil driving function
  return Compensation_factor;
  }
  

void field::Current_feedback()
/* Calls the Current_feedback_calculation function for each direction (x,y,z) and then changes the outut PWM value to the newly found adapted value */
{
   double Comp_fact_x_prev = Comp_fact_x;
   double Comp_fact_y_prev = Comp_fact_y;
   double Comp_fact_z_prev = Comp_fact_z;
   
   if (B_arrayfield_x[Looppar_1] != 0)
   {
      Comp_fact_x = Current_feedback_calculation(B_arrayfield_x[Looppar_1], Current_read_x, V_current_init_x, Gradient_x[Looppar_1], Ax_neg_VT, Bx_neg_VT, Ax_pos_VT, Bx_pos_VT, Comp_fact_x_prev);
   }
   if (B_arrayfield_y[Looppar_1] != 0)
   {
      Comp_fact_y = Current_feedback_calculation(B_arrayfield_y[Looppar_1], Current_read_y, V_current_init_y, Gradient_y[Looppar_1], Ay_neg_VT, By_neg_VT, Ay_pos_VT, By_pos_VT, Comp_fact_y_prev);
   }
   if (B_arrayfield_z[Looppar_1] != 0)
   {
      Comp_fact_z = Current_feedback_calculation(B_arrayfield_z[Looppar_1],  Current_read_z, V_current_init_z, Gradient_z[Looppar_1], Az_neg_VT, Bz_neg_VT, Az_pos_VT, Bz_pos_VT, Comp_fact_z_prev);
   }
   //uncomment to switch off current sensing
   //Comp_fact_x =1;
   //Comp_fact_y =1;
   //Comp_fact_z =1;

   //set the field with the new compensated values
   SetBfieldFast(B_arrayfield_x[Looppar_1], B_arrayfield_y[Looppar_1], B_arrayfield_z[Looppar_1],Gradient_x[Looppar_1],Gradient_y[Looppar_1],Gradient_z[Looppar_1]);
}


bool field::CheckUpdatedCurrentTime()
{
  timer3_seconds_counted++;
  if (timer3_seconds_counted >= Period_current_sense)
    {
      timer3_seconds_counted = 0;
      return true;
    }
  else
    {
      return false;
    }
}


//initialization of the current sensing, used to measure the offset values of the difference amplfiers
void field::Init_current_feedback()
{
  Serial.println("Initialising current feedback timer to 1000 sec");
  // ESP32 handles timers in a different way. Perhaps make a timer wrapper?
  //https://techtutorialsx.com/2017/10/07/esp32-arduino-timer-interrupts/
  mytimer.initTimer(3,1000000);
    //was: Timer3.initialize(1000000); Leon
  time_last_field_change = millis();
  
  int Nmb_av_current = 100;
  for (int i=0; i < Nmb_av_current; i++){
      V_current_init_x += analogRead(Current_read_x);
      V_current_init_y += analogRead(Current_read_y); 
      V_current_init_z += analogRead(Current_read_z); 
   }
  V_current_init_x /= Nmb_av_current;
  V_current_init_y /= Nmb_av_current;
  V_current_init_z /= Nmb_av_current;
  Serial.print("The initial currents are (x,y,z): ");
  Serial.print(V_current_init_x);Serial.print(", ");
  Serial.print(V_current_init_y);Serial.print(", ");
  Serial.println(V_current_init_z);
  //Assign these values to the global variabel, for export to file
  Vfb.x=V_current_init_x;
  Vfb.y=V_current_init_y;
  Vfb.z=V_current_init_z;
}

void field::Reset_Bfield()
//resets all settings for the B-field, direction, gradient and feedback correction
{
  //make all coils inactive
coilPwmWrite(Coil_x, 0);
coilPwmWrite(Coil_y, 0);
coilPwmWrite(Coil_z, 0);
  
  //set coil direction
  digitalWrite(Dir_x, LOW); 
  digitalWrite(Dir_y, LOW);
  digitalWrite(Dir_z, LOW);

  //reset short circuit relay
  digitalWrite(Relay_x, HIGH); 
  digitalWrite(Relay_y, HIGH);
  digitalWrite(Relay_z, HIGH);

  //reset current feedback control
  Comp_fact_z = 1;
  Comp_fact_y = 1;
  Comp_fact_z = 1;
}

void field::coilPwmWrite(int Coil, int PWM_value)
  // Write PWM _value to pin Coil
{
  /* Implementation depends on platform */
#if defined(_MAGOD1)
  // If Arduino, use pwm.h. PWM_Value 0:255
      pwmWrite(Coil, PWM_value);
#elif defined(_MAGOD2)
   // If ESP32, use ledcWrite. PMW_value 0:2^(resolution)
   // Resolution is set in ledcSetup
      ledcWrite(Coil, PWM_value);
#endif
}
      
