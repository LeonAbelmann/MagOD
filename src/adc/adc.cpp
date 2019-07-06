/* adc.cpp
 MagOD libary 
 April 2019
 Measures reference voltages
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "../../MagOD.h"
#include "Arduino.h"
#include "adc.h"

//Constructor
adc::adc(){
};

/* Initialize the ADC(s) with the correct amplification */
void adc::initADC(){
#if defined(_MAGOD1)
  Adafruit_ADS1115 ads0(ADS1115_ADDRESS_0); //adc0-3;
  Adafruit_ADS1115 ads1(ADS1115_ADDRESS_1); //adc4-7;

  //GAIN_TWOTHIRDS  +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  //GAIN_ONE        +/- 4.096V  1 bit = 2mV      0.125mV
  //GAIN_TWO        +/- 2.048V  1 bit = 1mV      0.0625mV
  //GAIN_FOUR       +/- 1.024V  1 bit = 0.5mV    0.03125mV
  //GAIN_EIGHT      +/- 0.512V  1 bit = 0.25mV   0.015625mV
  //GAIN_SIXTEEN    +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  myadc.ads.setGain(GAIN_FOUR);
  myadc.adsMaxV=1.024;
  myadc.ads.begin();
#elif defined(_MAGOD2)
  /* ADC 0: used for photodiodes */
  myadc.ads0.setGain(GAIN_ONE);
  myadc.adsMaxV0=4.096;
  myadc.ads0.begin();

  /* ADC 1: used for current measurement and temperature */
  myadc.ads1.setGain(GAIN_ONE);
  myadc.adsMaxV1=4.096;
  myadc.ads1.begin();
#endif
}

/* Read the fotodiode signals */
diodes adc::readDiodes()
{
#if defined(_MAGOD1)
  uint16_t adc0, adc1, adc2, adc3;  
  //Readout ADC's for photodiode voltage
  adc0 = myadc.ads.readADC_SingleEnded(0);
  adc1 = myadc.ads.readADC_SingleEnded(1);
  adc2 = myadc.ads.readADC_SingleEnded(2);
  adc3 = myadc.ads.readADC_SingleEnded(3);
  
  //Convert to voltage
  Vdiodes.Vdwn = double(adc2)/32768*myadc.adsMaxV; /* upper part of
						     slit
						     photodiode */
  Vdiodes.Vup = double(adc3)/32768*myadc.adsMaxV; /* lower part */
  Vdiodes.Vdiode = ((Vdiodes.Vup + Vdiodes.Vdwn)/2.0);
  Vdiodes.Vled = double(adc0)/32768*myadc.adsMaxV;
  Vdiodes.Vscat = double(adc1)/32768*myadc.adsMaxV;
#elif defined(_MAGOD2)
  uint16_t adc0, adc1, adc2, adc3;  
  /* Read out ADC's, always read all ports! */
  adc0=myadc.ads0.readADC_SingleEnded(0);//n.c. future: scatterDiode
  adc1=myadc.ads0.readADC_SingleEnded(1);//photoDiode
  adc2=myadc.ads0.readADC_SingleEnded(2);//nc
  adc3=myadc.ads0.readADC_SingleEnded(3);//refDiode
  /*
  Serial.print("adc0 = ");Serial.println(adc0);
  Serial.print("adc1 = ");Serial.println(adc1);
  Serial.print("adc2 = ");Serial.println(adc2);
  Serial.print("adc3 = ");Serial.println(adc3);
  */
/* Convert to voltage */
  Vdiodes.Vdiode = double(adc1)/32768*myadc.adsMaxV0;
  Vdiodes.Vled = double(adc3)/32768*myadc.adsMaxV0;
  //Vdiodes.Vscat = double(adc0)/32768*myadc.adsMaxV0;
#endif
  return Vdiodes;
}


void adc::set_vrefs(references &Vrefs, bool ref_all_wavelength, led theled)
{
#if defined(_MAGOD1)
//Measure reference diode voltage. If ref_all_wavelength true, than cycle over all led colours
//  struct references Vrefs;
  double _Vref;
  double _adc2, _adc3;
  if (ref_all_wavelength == 0)
    {//Average 10 times
      _adc2 = 0;
      _adc3 = 0;
      for (int i=0; i<10; i++){
	_adc2 += double(ads.readADC_SingleEnded(2));//It would make sense to define the port so ADC2 instead of 2. Leon
	_adc3 += double(ads.readADC_SingleEnded(3));
      }
      _Vref = _adc2/20 + _adc3/20;
      _Vref = _Vref/32768*adsMaxV;
      Vrefs.Vref=_Vref;
      //For debug by Leon
      Serial.print("Updated Vref = ");Serial.println(_Vref);
    }
  else
    {// Red
      //TODO LEON 1, 2, 3 is strange. Define constants RED, BLUE, GREEN
      theled.Set_LED_color(1);
      delay(5000);
      _adc2 = 0;
      _adc3 = 0;
      for (int i=0; i<10; i++){
	_adc2 += double(ads.readADC_SingleEnded(2));
	_adc3 += double(ads.readADC_SingleEnded(3));
      }
      _Vref = _adc2/20 + _adc3/20;
      _Vref = _Vref/32768*adsMaxV;
      Vrefs.Vred=_Vref;
      // Green
      theled.Set_LED_color(2);
      delay(5000);
      _adc2 = 0;
      _adc3 = 0;
      for (int i=0; i<10; i++){
	_adc2 += double(ads.readADC_SingleEnded(2));
	_adc3 += double(ads.readADC_SingleEnded(3));
      }
      _Vref = _adc2/20 + _adc3/20;
      _Vref = _Vref/32768*adsMaxV;
      Vrefs.Vgreen=_Vref;
      // Blue
      theled.Set_LED_color(3);
      delay(5000);
      _adc2 = 0;
      _adc3 = 0;
      for (int i=0; i<10; i++){
	_adc2 += double(ads.readADC_SingleEnded(2));
	_adc3 += double(ads.readADC_SingleEnded(3));
      }
      _Vref = _adc2/20 + _adc3/20;
      _Vref = _Vref/32768*adsMaxV;
      Vrefs.Vblue = _Vref;
    }
#elif defined(_MAGOD2)
  /* Iterate over all colors in the LEDs list
     note that there might be only one color :) */
  for(auto iter = 1; iter<sizeof(LEDs); iter++)
    {
      theled.Set_LED_color(iter);
      /* Read out diodes 10 times and average*/
      double Vdiode;  
      Vdiode=0;
      for (int i=0; i<10; i++){
	  Vdiodes=myadc.readDiodes();
	  Vdiode+= Vdiodes.Vdiode;
      }
      Vdiode=Vdiode/10;
      /* Assign to the correct reference */
      switch(iter) {
      case RED   : Vrefs.Vred   =Vdiode;
	break;
      case GREEN : Vrefs.Vgreen =Vdiode;
	break;
      case BLUE  : Vrefs.Vblue  =Vdiode;
	break;
      }
      /* And remember the last one */
      Vrefs.Vref=Vdiode;
    }
#endif 
}

double adc::readTemp(){
  double Temperature_voltage, Temperature_degrees;
#if defined(_MAGOD1)
  Temperature_voltage = analogRead(mypins.Temp_read)*5.0/1024.0;
    //calculates the temperature from datasheet calibration
  Temperature_degrees = 3.4328*pow(Temperature_voltage,3)
    -25.099*pow(Temperature_voltage,2)
    +76.047*Temperature_voltage-61.785;
  
#elif defined(_MAGOD2)
  uint16_t adc4; // Is this the correct port? Leon
  adc4=double(ads1.readADC_SingleEnded(0));
  Temperature_voltage=double(adc4)/32768*myadc.adsMaxV1;
  Temperature_degrees = 3.4328*pow(Temperature_voltage,3)
    -25.099*pow(Temperature_voltage,2)
    +76.047*Temperature_voltage-61.785;
#endif 
  return Temperature_degrees;
}
