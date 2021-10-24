/* adc.cpp
 MagOD libary 
 December 2020
 Measures reference voltages
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann */
/* Updated in version 2.4 with hardware interupt of adc converter and
   data buffer.
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

/* void IRAM_ATTR adc0ISR(): ADC 0 interrupt routine */
/* IRAM_ATTR is used to make sure that the interrupt routine (ISR) can
   run while SPI or i2c communication is happening */
/* https://docs.espressif.com/projects/esp-idf/en/latest/esp32/
   api-reference/system/freertos.html
   #_CPPv418xTaskNotifyFromISR12TaskHandle_t
   8uint32_t13eNotifyActionP10BaseType_t */
void IRAM_ATTR adc0ISR() {
  // Variable to store the outcome of a notification in
  BaseType_t xHigherPriorityTaskWokenADC0;
  /* Notify task readADC0Handle to start running and don't pass along
     any data with the notification (0, eNoAction) */
  xTaskNotifyFromISR(readADC0Handle, 0, eNoAction,
		     &xHigherPriorityTaskWokenADC0);
  /* Handle possible task exception due to the nature of an ISR, see
     link above for more information */
    if ( xHigherPriorityTaskWokenADC0 ) {
        portYIELD_FROM_ISR ();
    }
}

/* Void readADC0(): Task is called by interupt service routine
   adc0ISR(). Runs on second core (0, default core is 1)*/
/* Designed according to example: */
/* https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference
   /system/freertos.html#
   _CPPv411xTaskCreate14TaskFunction_tPCKcK8uint32_tPCv11UBaseType_tPC12
   TaskHandle_t */
void readADC0( void * parameter ) {
  // Counter to keep track of the channel that should be measured next
  int chanCount = 0;
  // List of channels to be read
  int iChan[] = {DIODE, LED};
  int iChanLen = 2; // Number of elements in list above (1-4)
  // Temp variable to store the data from the ADC in temporarily
  bufferData_t writeData;
  // Temp variable storing the status of the buffer
  BaseType_t queueStatus;
    
  for (;;) {// Infinite loop to keep task running
    /* https://docs.espressif.com/projects/esp-idf/en/latest/esp32/
       api-reference/system/freertos.html#
       _CPPv415xTaskNotifyWait8uint32_t8uint32_tP8uint32_t10TickType_t*/
    
    /* Wait at maximum 5000ms for a notification otherwise it will
       return a fail. Also don't do anything with the notification
       bytes (0,0,0 ...) */
    if (xTaskNotifyWait(0, 0, 0, pdMS_TO_TICKS(5000)) == pdPASS){  
      //Save the data to the temporary variable
      // Save ADC value:
      writeData.val = ads0.getADC();
      // Save time at which the data was retrieved
      writeData.time = millis();
      // Save the channel on which the measurement was done
      writeData.ch = iChan[chanCount];
      
      // Push data to buffer
      /*  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/
	  api-reference/system/freertos.html
	  #c.xQueueSendToBack */
      queueStatus = xQueueSendToBack(dataQueueHandle, &writeData, 0);
      /* Push the data to buffer dataQueueHandle and do not wait to
	 retry if it was unsuccesfull */
      
      //Check if succesfull
      if ( queueStatus != pdTRUE ) {
	Serial.println("Buffer is full!");
      }
      
      //Increase channel counter and handle overflow
      /* We don't need to read all channels. In CEA/Bayreuth/ASTON
	 systems we only need DIODE and LED channel */ 
      chanCount++;
      /* There are 4 channels to cover 0-3, overflow if it hits 4 or
	 higher. Maybe you don't need to read all channels. TBD. LEON */
      if (chanCount >= iChanLen) {
	chanCount = 0;
      }
      
      /* ADC1 is not on interupt, but we read it on timer */
      if (doMeasurementFlag) {
	doMeasurementFlag = false;
	// Serial.print("Measuring ADC1: ");
	// Loop over 4 channels
	for (int DAC1channel = 0; DAC1channel<=3; DAC1channel++) { 
	  writeData.val = ads1.readADC_SingleEnded(DAC1channel);
	  // Serial.print(writeData.val);Serial.print(" , ");
	  // Save time at which the data was retrieved
	  writeData.time = millis();
	  // Save the channel on which the measurement was done
	  writeData.ch = 4 + DAC1channel; // Channels 4,5,6,7
	  // Push data to buffer
	  queueStatus = xQueueSendToBack(dataQueueHandle, &writeData, 0);
	  // Check if succesfull
	  if ( queueStatus != pdTRUE ) {
	    Serial.println("Buffer is full!");
	  }
	}
	//Serial.println();
      }
      
      /* Start the next measurement on the ADC by setting the
	 channel and the samplerate */
      ads0.startReadADC(iChan[chanCount], sampleRateADC0);   
    }
  }
}

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
  // /* ADC 0: used for photodiodes */
  Serial.print("Setting ads0 and ads1 gain. V0= ");
  ads0.setGain(GAIN_ONE);
  adsMaxV0=4.096;
  ads0.begin();
  /* ADC0 is interupt based. Pin 13 is connect to ALERT/RDY pin of ads0 */
  pinMode(ADS1115_RDY, INPUT_PULLUP);
  /* Interupt when RDY goes down */
  attachInterrupt(ADS1115_RDY, adc0ISR, FALLING);     
  Serial.print(adsMaxV0);

  /* Create the buffer */
  /*  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/
      api-reference/system/freertos.html#c.xQueueCreate */
  /* Create a buffer with a size of bufferSize and a element size
     equal to the size of bufferData_t, the custom struct */
  dataQueueHandle = xQueueCreate(bufferSize, sizeof(bufferData_t));   
  
  
  /* Setup ADC0 Task to run on Core 0 (second core, 1 is default) */
  /*  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/
      api-reference/system/freertos.html
      #_CPPv423xTaskCreatePinnedToCore14TaskFunction_tPCKcK8uint32_t
      PCv11UBaseType_tPC12TaskHandle_tK10BaseType_t */
  xTaskCreatePinnedToCore(readADC0 /* Name of the function that will
				      serve as the task */
			  ,  "readADC0"  /* A name just for humans */
			  ,  4096/* Stack size can be seen as RAM
				    allocated to the task this stack
				    size can be checked & adjusted by
				    reading the
				    uxTaskGetStackHighWaterMark(); it
				    has not been optimized yet. 4096B
				    is equal to that of the void loop
				    task */
			  ,  NULL/* Extra pointer parameter which is
				    not neccessary */
			  ,  2 /* Priority, with 3
				  (configMAX_PRIORITIES - 1) being the
				  highest, and 0 being the lowest.*/
			  ,  &readADC0Handle /* Handle to refer to the
						 task */
			  ,  0);              /* Core to run the task
						 on */
        
  //Start measurement
  ads0.startReadADC(0, sampleRateADC0);

  //Add delay to force the buffer to fill up initially
  delay(2000);

  
  /* ADC 1: used for current measurement and temperature */
  ads1.setGain(GAIN_ONE);
  adsMaxV1=4.096;
  ads1.begin();
  Serial.print(" V1=");
  Serial.println(adsMaxV1);
#endif
}


/* Check if buffer for ADC0 has data */
bool adc::bufferEmpty(){
  uint16_t bufferRemaining;

  /* Get the amount of free buffer spaces */
  bufferRemaining = uxQueueSpacesAvailable(dataQueueHandle);

  /* If what is remaining is smaller than the total size there must be
     something in the buffer */
  if (bufferRemaining < bufferSize) {
    // For debug:
    // Serial.print("Buf: ");
    // Serial.println(bufferRemaining);
    return false;
  }
  else {
    /* In case the buffer is empty add a small delay to prevent the
       task from locking up other tasks */
    vTaskDelay(1);
    return true;
  }
}

/* Convert bit value of ADC to voltage */
double convertADC0(uint16_t value)
{ double result;
  //Convert to voltage
  result = double(value)/32768*adsMaxV0;
  return result;
}

/* Convert bit value of ADC to voltage */
double convertADC1(uint16_t value)
{ double result;
  //Convert to voltage
  result = double(value)/32768*adsMaxV1;
  return result;
}

/* Convert ADC1 voltages to currents for coil system 'coil' */
double convertCurrent(uint16_t value, int coil) 
{
#if defined(_MAGOD1)
//to be done. LEON
  return 0;
#elif defined(_MAGOD2)
  double result;
  /* Every coil system has a slightly different calibration. */
  /* adc4Offset, adc4SlopePos and adc4SlopeNeg are defined in calibration.h */
  //Serial.print("convertCurrent : adc = ");Serialln.print(value);
  switch (coil) {
  case IX: // X-coils on adc4
    if (value-adc4Offset < 0)
      { result=(value-adc4Offset)*adc4SlopeNeg;}
    else
      { result=(value-adc4Offset)*adc4SlopePos;}
    //Serial.print(", current X: ");Serial.println(result);
    return result;
  case IY: // Y-coils on adc6 !!!!!
    if (value-adc6Offset < 0)
      { result=(value-adc6Offset)*adc6SlopeNeg;}
    else
      { result=(value-adc6Offset)*adc6SlopePos;}
    //Serial.print(", current Y: ");Serial.println(result);
    return result;
  case IZ: // Z-coils on adc5 !!!!!
    if (value-adc5Offset < 0)
      { result=(value-adc5Offset)*adc5SlopeNeg;}
    else
      { result=(value-adc5Offset)*adc5SlopePos;}
    //Serial.print(", current Z: ");Serial.println(result);
    return result;
  default:
    return 0;
  }
#endif
}

/* Convert ADC1-Ain7 to temperature */
/* NTC parameters defined in calibration.h */
double convertTemperature(uint16_t value)  {
  double Temperature_voltage=(double(value)/32768)*adsMaxV1;
  /*  At NTCT=25 degC the NTC has a resistance of 10 kOhm. The NTC is
      in series with R26=10 or 3.3 kOhm (version dependent). NTRC =
      R25/RNTC(25 oC). The NTC temperature coefficient is
      NTCoeff=-4.39 %/K. */  
  double Temperature_degrees = NTCT +
    (-1/NTCC)*(1-NTCR*((NTCV/Temperature_voltage)-1));
  /* Debug: */
  // Serial.print("Voltage ADC7: ");Serial.println(Temperature_voltage);
  // Serial.print("NTCT        : ");Serial.println(NTCT);
  // Serial.print("NTCR        : ");Serial.println(NTCR);
  // Serial.print("NTCC        : ");Serial.println(NTCC);
  // Serial.print("Temperature: ");Serial.println(Temperature_degrees);
  return Temperature_degrees;
}

bufferData_t readBuffer(){
  /* Retrieve the oldest point from the buffer (FIFO) */
  /* https://docs.espressif.com/projects/esp-idf/en/latest/esp32/
     api-reference/system/freertos.html#c.xQueueReceive */
  // Temp variable to store the data in that was just retrieved from the buffer
  bufferData_t readData;
  xQueueReceive(dataQueueHandle, &readData, 0); 
  
  /* For debug : */
  /*
  Serial.print("\tCh: ");
  Serial.print(readData.ch);
  Serial.print("\tVal: ");
  Serial.print(readData.val);
  Serial.print("\tTime: ");
  Serial.println(readData.time);
  */
  return readData;
}

dataPoint adc::getDataPoint() { /* Get datapoint and convert to units*/
  bufferData_t dataIn;
  dataPoint dataOut;
  dataIn = readBuffer();
  dataOut.time = dataIn.time;
  dataOut.channel = dataIn.ch;
  /* Depending on the channel, convert the data to units */
  switch (dataIn.ch) {
  case LED:
    dataOut.val = convertADC0(dataIn.val);
    break;
  case SCAT:
    dataOut.val = convertADC0(dataIn.val);
    break;
  case DIODE:
    dataOut.val = convertADC0(dataIn.val);
    break;
  case SPLIT:
    dataOut.val = convertADC0(dataIn.val);
    break;
  case IX:
    dataOut.val = convertCurrent(dataIn.val,IX);
    break;
  case IY:
    dataOut.val = convertCurrent(dataIn.val,IY);
    break;
  case IZ:
    dataOut.val = convertCurrent(dataIn.val,IZ);
    break;
  case NTC:
    dataOut.val = convertTemperature(dataIn.val);
    break;
  default:
    dataOut.channel = 8;
    dataOut.val = 0;
    break;
  }
  return dataOut;
}



#if defined(_MAGOD1)
double readTemp(){
  double Temperature_voltage, Temperature_degrees;
  Temperature_voltage = analogRead(Temp_read)*5.0/1024.0;
    //calculates the temperature from datasheet calibration
  Temperature_degrees = 3.4328*pow(Temperature_voltage,3)
    -25.099*pow(Temperature_voltage,2)
    +76.047*Temperature_voltage-61.785;
  return Temperature_degrees;
}
#endif 


