/* adc.h
 MagOD libary 
 April 2019
 Photodiode readout
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
#ifndef adc_h
#define adc_h

//#include <Adafruit_ADS1015.h>  // ADC library, now included in standard libraries

#include "../../MagOD.h"
#include "../led/led.h" // Needed for set_vrefs. adc and led are interdepenent. Perhaps solve in MagOD itself. TODO. LEON

/* trying to change the sample rate. The default used by Adafruit_ADS1015 is ADS1015_REG_CONFIG_DR_1600SPS. Standard it has the value (0x0080). The ADS1115 has the following datarates (https://cdn-shop.adafruit.com/datasheets/ads1115.pdf, page 19):
SPS    Bits [7-5] Hex
8SPS   000        (0x0000)          
16SPS  001        (0x0020)  
32SPS  010        (0x0040)  
64SPS  011        (0x0060)
128SPS 100        (0x0080)  (default) 
250SPS 101        (0x00A0) 
475SPS 110        (0x00C0) 
860SPS 111        (0x00E0) 

There is also a conversion delay. It is default at 8 ms (8), speed up by reducing. Somehow we have to change these values into e.g

#define ADS1115_CONVERSIONDELAY         (2)
#define ADS1015_REG_CONFIG_DR_1600SPS (0x0020)

https://hackaday.io/project/11154-alli-gait-or-analysis/log/44744-need-for-speed-ads1116-library-mod */


/* addresses of ADS1115 ADC's */
#define ADS1115_ADDRESS_0 (0x48) // 1001 000 (ADDR -> GND)
#define ADS1115_ADDRESS_1 (0x49) // 1001 001 (ADDR -> VDD)

#if defined(_MAGOD1)
#define Temp_read //12 //Pin for NTC ADC
#endif
/* MagOD2 uses adc1 for reading temperature */

class adc
{
 public:
  adc();
#if defined(_MAGOD1)
  double adsMaxV;  //max voltage that can be measured by the ADC
#elif defined(_MAGOD2)
  double adsMaxV0,adsMaxV1;  //max voltages, for two ADCs
#endif
  void initADC(); //Start the ADCs with correct amplification
  void set_vrefs(references &Vref, bool ref_all_wavelength, led myled); //measure reference voltage
  diodes readDiodes(); // Read the signals from the photodiodes
  double readTemp(); //Read the temperature sensor
  feedbacks readFeedbacks();//Read the currents to the coils
 private:
  //none
};

#endif

