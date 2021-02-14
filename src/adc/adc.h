/* adc.h
 MagOD libary 
 December 2020
 Photodiode readout
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
#ifndef adc_h
#define adc_h

//#include <Adafruit_ADS1015.h>  // ADC library, now included in standard libraries
#include "../MagOD_ADS1115/MagOD_ADS1115.h" // Modified Adafruit_ADS1015 lib


#include "../../MagOD.h"
#include "../led/led.h" // Needed for set_vrefs. adc and led are interdepenent. Perhaps solve in MagOD itself. TODO. LEON

/* addresses of ADS1115 ADC's */
#define ADS1115_ADDRESS_0 (0x48) // 1001 000 (ADDR -> GND)
#define ADS1115_ADDRESS_1 (0x49) // 1001 001 (ADDR -> VDD)
/* ESP32 pin that the ready pin of the ADC is attached to */
#define ADS1115_RDY 13                     
/* samplerate for adc0 */
#define sampleRateADC0 0 /* Sample rates: 0: 8 SPS, 1: 16 SPS, 2: 32
			    SPS, 3: 64 SPS, 4: 128 SPS, 5: 250 SPS, 6:
			    475 SPS 7: 860 SPS */

#if defined(_MAGOD1)
#define Temp_read //12 //Pin for NTC ADC
#endif
/* MagOD2 uses adc1 for reading temperature */


/* Definition of the buffer size, how many elements does it have */
#define bufferSize 1000
/* Buffer data structure type*/
typedef struct {
  uint8_t ch;                 // Channel that was measured on
  uint16_t val;               // Value that was measured
  unsigned long time;         // Time of the measurement
  /* the millis function returns a 32bit value meaning it will
     overflow after 49 days */
} bufferData_t;

/* readable names for the ADC channels */
#define SCAT  0 /* ads0 ADC0 Ain0 scattering photodiode */
#define DIODE 1 /* ads0 ADC1 Ain1 photodetector */
#define SPLIT 2 /* ads0 ADC2 Ain2 not connected */
#define LED   3 /* ads0 ADC3 Ain3 photodiode that measures LED directly */
#define IX    4 /* ads1 ADC0 A4 x-coils current monitor */
#define IZ    5 /* ads1 ADC1 A5 y-coils current monitor */
#define IY    6 /* ads1 ADC1 A6 z-coils current monitor */
#define NTC   7 /* ads1 ADC3 Ain7 ADCNTC temperature sensor */
// MagOD1 has DIODE_UP ad 2 DIODE_DOWN at 3.


class adc
{
 public:
  adc();
  void       initADC(); /* Start the ADCs with correct amplification,etc */
  bool       bufferEmpty();//Check if ADC0 buffer has data
  dataPoint  getDataPoint(); /* Get datapoint and convert*/
  //measure reference voltage:
  void       set_vrefs(references &Vref, bool ref_all_wavelength, led myled); 
  /* diodes     readDiodes(); // Read the signals from the photodiodes */
  /* double     readTemp(); //Read the temperature sensor */
  /* feedbacks  readFeedbacks();//Read the currents to the coils */
 private:
};

#endif

