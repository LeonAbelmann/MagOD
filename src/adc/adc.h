/* adc.h
 MagOD2 libary 
 Oct 2018
 Photodiode readout
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
#ifndef adc_h
#define adc_h

#include <Adafruit_ADS1015.h>  // ADC library

#include "../globals/globals.h"
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

https://hackaday.io/project/11154-alli-gait-or-analysis/log/44744-need-for-speed-ads1116-library-mod

 */


class adc
{
 public:
  adc();
  Adafruit_ADS1115 ads;
  double adsMaxV;  //max voltage that can be measured by the ADC
  void set_vrefs(references &Vref, bool ref_all_wavelength, led myled); //measure reference voltage
 private:
  //none
};

#endif

