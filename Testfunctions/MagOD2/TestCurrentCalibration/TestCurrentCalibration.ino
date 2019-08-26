#include <SPI.h> // Communication
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ADS1015.h>  // ADC library

/* Help program that scans the PWM of the motordrivers and measures the current sensors adc4-6. If you connect a multimeter at the same time to the coils, you can make a graph of measured current versus real current */

#define RA8875_INT 16
#define RA8875_CS 5 
#define RA8875_RESET 17

/* MagOD2 has two ADS1115. 
   U2 with address pin to gnd, used for AIN0-3.
   U5 with address pin to VDD (3.3V), used for AIN4-7 */
#define ADS1015_ADDRESS_0 (0x48)// 1001 000 (ADDR -> GND)
#define ADS1015_ADDRESS_1 (0x49)// 1001 001 (ADDR -> VDD)
Adafruit_ADS1115 ads0(ADS1015_ADDRESS_0); //adc0-3;
Adafruit_ADS1115 ads1(ADS1015_ADDRESS_1); //adc4-7;

uint8_t Coil_x = 33; // output of the coils in the x direction
uint8_t Coil_y = 26; // output of the coils in the y direction
uint8_t Coil_z = 14; // output of the coils in the z direction
const uint32_t Frequency_PWM = 20000;    //set the pwm frequency of the coil-drivers
const int ledChannel_x = 0; /*0-15*/
const int ledChannel_y = 1; /*0-15*/
const int ledChannel_z = 2; /*0-15*/
const int resolution = 8; /* no bits resolution, 1-16 bit */
int Dir_x = 25;                // direction of the coils in the x direction     low: output A of motor driver is then high and B is low (positive direction),     high: output A of motor driver is then low and B is high (negative direction)
int Dir_y = 27;                // direction of the coils in the y direction
int Dir_z = 12;                // direction of the coils in the z direction

/* help variable */

void setup () {
  Serial.begin(115200);

  /* Initialize the PWM */
 
 // Initialize channels 
  /*  channels 0-15, resolution 1-16 bits, freq limits depend on
       resolution ledcSetup(uint8_t channel, uint32_t freq, uint8_t
       resolution_bits);
     https://github.com/espressif/esp-idf/blob/master/docs/en/api-reference/peripherals/ledc.rst
  */

  ledcSetup(ledChannel_x, Frequency_PWM, resolution);
  ledcSetup(ledChannel_y, Frequency_PWM, resolution);
  ledcSetup(ledChannel_z, Frequency_PWM, resolution);

  ledcAttachPin(Coil_x, ledChannel_x);
  ledcAttachPin(Coil_y, ledChannel_y);
  ledcAttachPin(Coil_z, ledChannel_z);
    					
  /* Initialize the direction pins for the motor drives */
    /* sets the direction pin as output */
  pinMode(Dir_x, OUTPUT);
  pinMode(Dir_y, OUTPUT);
  pinMode(Dir_z, OUTPUT);
 

  /* Set the voltage */
  /* ledcWrite(channel, dutycycle)
     note that dutycycle ranges from 0-2^(resolution) set in ledcSetup
  2^11=2048*/
  ledcWrite(ledChannel_x, 0);
  ledcWrite(ledChannel_y, 0);
  ledcWrite(ledChannel_z, 0);

  /* Initialize ADC */
  ads0.setGain(GAIN_ONE);
  ads0.begin();
  ads1.setGain(GAIN_ONE);
  ads1.begin();
}

const int X=1;
const int Y=2;
const int Z=3;
int directions[]={Z}; //or {Y}, or {X,Y,Z} :)

void loop()
{
  int adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7; //Bit values of adc

  int Dir_i = Dir_x;
  int Chan_i = ledChannel_x;

  Serial.println(directions[0]);
  
  for(int dir = directions[0]; dir <= sizeof(directions); dir = dir + 1 )
    { 
      switch(dir) {
      case X:
	Serial.println("X");
	Dir_i  = Dir_x;
	Chan_i = ledChannel_x;
	break;
      case Y:
	Serial.println("Y");
	Dir_i  = Dir_y;
	Chan_i = ledChannel_y;
	break;
      case Z:
	Serial.println("Z");
	Dir_i  = Dir_z;
	Chan_i = ledChannel_z;
	break;
      default:
	Serial.print("Dir is :");
	Serial.println(Dir_i);
      }
      
      for (double step = -1; step <= 1; step = step+0.1)
	{
	  /* Set the current direction */
	  if (step < 0) {digitalWrite(Dir_i, LOW);};
	  if (step >= 0) {digitalWrite(Dir_i, HIGH);};
	  /* Set the PWM, second value is relative to 2^resolution */
	  int PWM = fabs(step)*0.3*pow(2,resolution);//fabs is for doubles
	  ledcWrite(Chan_i, PWM);

	  /* Read ADC1115 ports */
	  adc4 = 0, adc5 = 0, adc6 = 0;
	  int N = 200;
	  for (int i = 0; i < N; i++)
	    {
	      adc4 = adc4 + ads1.readADC_SingleEnded(0);
	      adc5 = adc5 + ads1.readADC_SingleEnded(1);
	      adc6 = adc6 + ads1.readADC_SingleEnded(2);
	    }

	  if (step < 0)  {Serial.print("-");};
	  if (step >= 0) {Serial.print(" ");};
	  Serial.print(PWM);
	  Serial.print(", ");
	  Serial.print(adc4/N);
	  Serial.print(", ");
	  Serial.print(adc5/N);
	  Serial.print(", ");
	  Serial.println(adc6/N);
	}
      ledcWrite(Chan_i, 0); //Stop current
    }  
}
