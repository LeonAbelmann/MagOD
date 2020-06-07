/* adc.cpp
 MagOD libary 
 April 2019
 Measures reference voltages
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#include "../../MagOD.h"
#include "Arduino.h"
#include "adc.h"

#if defined(_MAGOD1)
Adafruit_ADS1115 ads(ADS1115_ADDRESS_0); //adc0-3;
#elif defined(_MAGOD2)
Adafruit_ADS1115 ads0(ADS1115_ADDRESS_0); //adc0-3;
Adafruit_ADS1115 ads1(ADS1115_ADDRESS_1); //adc4-7
#endif

//Constructor
adc::adc(){
};

/* Initialize the ADC(s) with the correct amplification */
void adc::initADC(){
#if defined(_MAGOD1)
  //GAIN_TWOTHIRDS  +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  //GAIN_ONE        +/- 4.096V  1 bit = 2mV      0.125mV
  //GAIN_TWO        +/- 2.048V  1 bit = 1mV      0.0625mV
  //GAIN_FOUR       +/- 1.024V  1 bit = 0.5mV    0.03125mV
  //GAIN_EIGHT      +/- 0.512V  1 bit = 0.25mV   0.015625mV
  //GAIN_SIXTEEN    +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads.setGain(GAIN_FOUR);
  adsMaxV=1.024;
  ads.begin();
#elif defined(_MAGOD2)
  /* ADC 0: used for photodiodes */
  Serial.print("Setting ads0 and ads1 gain. V0= ");
  ads0.setGain(GAIN_ONE);
  adsMaxV0=4.096;
  ads0.begin();
  Serial.print(myadc.adsMaxV0);
  /* ADC 1: used for current measurement and temperature */
  ads1.setGain(GAIN_ONE);
  adsMaxV1=4.096;
  ads1.begin();
  Serial.print(" V1=");
  Serial.println(myadc.adsMaxV1);
#endif
}

/* Read the fotodiode signals */
diodes adc::readDiodes()
{
#if defined(_MAGOD1)
  uint16_t adc0, adc1, adc2, adc3;

  //Readout ADC's for photodiode voltage
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);

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
  adc0=ads0.readADC_SingleEnded(0);//n.c. future: scatterDiode
  adc1=ads0.readADC_SingleEnded(1);//photoDiode
  adc2=ads0.readADC_SingleEnded(2);//nc
  adc3=ads0.readADC_SingleEnded(3);//refDiode
  /* debug: /
  Serial.print("adc0 = ");Serial.println(adc0);
  Serial.print("adc1 = ");Serial.println(adc1);
  Serial.print("adc2 = ");Serial.println(adc2);
  Serial.print("adc3 = ");Serial.println(adc3);
  /* */
  /* Convert to voltage */
  Vdiodes.Vdiode = double(adc1)/32768*myadc.adsMaxV0;
  Vdiodes.Vled = double(adc3)/32768*myadc.adsMaxV0;
  //Vdiodes.Vscat = double(adc0)/32768*myadc.adsMaxV0;
#endif
  return Vdiodes;
}

/* Maybe this routine should be in MagOD.ino. It is a measurement, after all.... Leon */
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
      theled.Set_LED_color(RED);
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
      theled.Set_LED_color(GREEN);
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
      theled.Set_LED_color(BLUE);
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
  for(auto iter = 0; iter<LEDnumber; iter++)
    {
      theled.Set_LED_color(LEDs[iter],LED_intensity[LEDs[iter]]);
      delay(3000);//Wait for the LED to stabilize
      /* Read out diodes 10 times and average*/
      double Vdiode;  
      Vdiode=0;
      for (int i=0; i<10; i++){
	  Vdiodes= myadc.readDiodes();
	  Vdiode+= Vdiodes.Vdiode;
      }
      Serial.print("Vref for ");
      Vdiode=Vdiode/10;
      /* Assign to the correct reference */
      switch(LEDs[iter]) {
      case RED   : Vrefs.Vred   =Vdiode;
	Serial.print("Red :");
	break;
      case GREEN : Vrefs.Vgreen =Vdiode;
	Serial.print("Green :");
	break;
      case BLUE  : Vrefs.Vblue  =Vdiode;
	Serial.print("Blue :");
	break;
      }
      Serial.println(Vdiode);
      /* And remember the last one */
      Vrefs.Vref=Vdiode;
    }
#endif 
}

double adc::readTemp(){
  double Temperature_voltage, Temperature_degrees;
#if defined(_MAGOD1)
  Temperature_voltage = analogRead(Temp_read)*5.0/1024.0;
    //calculates the temperature from datasheet calibration
  Temperature_degrees = 3.4328*pow(Temperature_voltage,3)
    -25.099*pow(Temperature_voltage,2)
    +76.047*Temperature_voltage-61.785;
  
#elif defined(_MAGOD2)
  uint16_t adc4, adc5, adc6, adc7;
  /* Read out ADC's, always read all ports! */
  adc4=ads1.readADC_SingleEnded(0);//X-current
  adc5=ads1.readADC_SingleEnded(1);//Y-current
  adc6=ads1.readADC_SingleEnded(2);//Z-current
  adc7=ads1.readADC_SingleEnded(3);//NTC
  /* debug: /
  Serial.print("adc4 = ");Serial.println(adc4);
  Serial.print("adc5 = ");Serial.println(adc5);
  Serial.print("adc6 = ");Serial.println(adc6);
  Serial.print("adc7 = ");Serial.println(adc7);
  /* */
  
  Temperature_voltage=double(adc7)/32768*myadc.adsMaxV1;
  //double R25 = 10000;    //NTC Temperature at 25 Deg
  double Bcoeff = -4.39e-2; //NTC temperature coeff /K
  //double Bseries = 10000;//Series resistance Ohm
  double Vdrive = 5.0;   //Drive voltage
  // V= Vdrive*R25*(1+dT*Bcoeff)/(Rseries+R25*(1+dT*Bcoeff))
  // R25/(R25+Rseries*(1+dT*Bcoeff))=0.5 approximately
  // dT=2*(V/Vdrive-1)/alpha 
  Temperature_degrees = 25 - (2*(Temperature_voltage/Vdrive)-1)/Bcoeff;
#endif 
  return Temperature_degrees;
}

feedbacks adc::readFeedbacks()//Read the currents to the coils
{
  feedbacks currents;
#if defined(_MAGOD1)
//to be done. LEON  
#elif defined(_MAGOD2)
  uint16_t adc4, adc5, adc6, adc7;
  /* Read out ADC's, always read all ports! */
  adc4=ads1.readADC_SingleEnded(0);//X-voltage
  adc5=ads1.readADC_SingleEnded(1);//Z-voltage !!!!!
  adc6=ads1.readADC_SingleEnded(2);//Y-voltage !!!!!
  adc7=ads1.readADC_SingleEnded(3);//NTC
  /* debug: 
  Serial.print("FB: adc4 = ");Serial.println(adc4);
  Serial.print("FB: adc5 = ");Serial.println(adc5);
  Serial.print("FB: adc6 = ");Serial.println(adc6);
  Serial.print("FB: adc7 = ");Serial.println(adc7);
  */
  double Vx=double(adc4)/32768*adsMaxV1;
  double Vy=double(adc6)/32768*adsMaxV1; //A6 on print!
  double Vz=double(adc5)/32768*adsMaxV1; //A5 on print!

  /* We assume calibration is done at GAIN_ONE. Use the testprogram
     TestCurrentCalibration.ino and a multimeter to get the relation
     between ADC bits and currents. See CurrentCalibration.ods*/
  /* These should go into a instrument settings file! LEON */
  /* ADC 4 measures current in X-direction */
  int    adc4Offset   = 13006;
  double adc4SlopePos = 0.00018374;
  double adc4SlopeNeg = 0.00024586;
  /* ADC 5 measures current in Z-direction !!!*/
  int    adc5Offset   = 13043;
  double adc5SlopePos = 0.00019299;
  double adc5SlopeNeg = 0.00024766;
  /* ADC 6 measures current in Y-direction !!!*/
  int    adc6Offset   = 12960;
  double adc6SlopePos = 0.00027076;
  double adc6SlopeNeg = 0.00024817;

  /* ADC 4 measures current in X-direction */
  if (adc4-adc4Offset > 0)
    { currents.x=(adc4-adc4Offset)*adc4SlopePos;}
  else
    { currents.x=(adc4-adc4Offset)*adc4SlopeNeg;}

  /* ADC 5 measures current in Z-direction !!!*/
  if (adc5-adc5Offset > 0)
    { currents.z=(adc5-adc5Offset)*adc5SlopePos;}
  else
    { currents.z=(adc5-adc5Offset)*adc5SlopeNeg;}

  /* ADC 6 measures current in Y-direction !!!*/
  if (adc6-adc6Offset > 0)
    { currents.y=(adc6-adc6Offset)*adc6SlopePos;}
  else
    { currents.y=(adc6-adc6Offset)*adc6SlopeNeg;}
  
  return currents;
#endif
}
