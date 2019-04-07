/* adc.cpp
 MagOD libary 
 Jan 2019
 Measures reference voltages
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "Arduino.h"
#include "adc.h"

//Constructor
adc::adc(){
};

void adc::set_vrefs(references &Vrefs, bool ref_all_wavelength, led theled)
{
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
}
