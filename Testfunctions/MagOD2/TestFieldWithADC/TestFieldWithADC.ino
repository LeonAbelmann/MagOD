#include <SPI.h> // Communication
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_RA8875.h> // TFTM50 screen
#include <Adafruit_ADS1015.h>  // ADC library

#define RA8875_INT 16
#define RA8875_CS 5 
#define RA8875_RESET 17

/* Intialize screen */
Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS,RA8875_RESET);
/* Initialize adc */
/* Adafruit_ADS1015(uint8_t i2cAddress = ADS1015_ADDRESS);
   0x48 (1001000) ADR -> GND
   0x49 (1001001) ADR -> VDD
   0x4A (1001010) ADR -> SDA
   0x4B (1001011) ADR -> SCL */
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
bool low=true;
int time0;



void setup () {
  Serial.begin(115200);
  delay(1000);

    /* Initialize the PWM */
 
 // Initialize channels 
  /*  channels 0-15, resolution 1-16 bits, freq limits depend on resolution
       ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits); https://github.com/espressif/esp-idf/blob/master/docs/en/api-reference/peripherals/ledc.rst
  I guessed 11, Leon
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

  
  Serial.println("Trying to find RA8875 screen...");
  /* Initialise the display using 'RA8875_480x272' or 'RA8875_800x480' */
  if (!tft.begin(RA8875_480x272)) {
    Serial.println("RA8875 Not Found!");
   while (1);
  }
  Serial.println("RA8875 Found");

  /* Initialize ADC */
  ads0.setGain(GAIN_ONE);
  ads0.begin();
  ads1.setGain(GAIN_ONE);
  ads1.begin();
  
  // Initiliaze display
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK);
  tft.textMode();

  tft.textTransparent(RA8875_WHITE);
  tft.textSetCursor(200, 1);
  tft.textWrite("TestADC.ino");

  // Initialize time
  time0 = millis();

}



void loop()
{
  uint16_t adc0, adc1, adc2, adc3, adc4, adc5, adc6, adc7; //Bit values of adc
  char string[15]; //Buffer holding string for dtostrf

  if ((millis()-time0)>2000)
    {
      low=!low;
      
      if (low)
	{
	  /* Set the current direction */
	  digitalWrite(Dir_x, LOW);
	  digitalWrite(Dir_y, LOW);
	  digitalWrite(Dir_z, LOW);
	  /* Set the PWM, second value is relative to 2^resolution */
	  ledcWrite(ledChannel_x, 0);
	  ledcWrite(ledChannel_y, 50);
	  ledcWrite(ledChannel_z, 0);
	}
      else
	{
	  digitalWrite(Dir_x, HIGH);
	  digitalWrite(Dir_y, HIGH);
	  digitalWrite(Dir_z, HIGH);
	  
	  /* Set the PWM, second value is relative to 2^resolution */
	  ledcWrite(ledChannel_x, 0);
	  ledcWrite(ledChannel_y, 50);
	  ledcWrite(ledChannel_z, 0);
	}
      time0=millis();
    }
  
  /* Read ADC1115 ports */
  /* ADC 0 */
  adc0=double(ads0.readADC_SingleEnded(0));
  Serial.print("adc0: ");
  Serial.println(adc0);
  
  adc1=double(ads0.readADC_SingleEnded(1));
  Serial.print("adc1: ");
  Serial.println(adc1);
  
  adc2=double(ads0.readADC_SingleEnded(2));
  Serial.print("adc2: ");
  Serial.println(adc2);
  
  adc3=double(ads0.readADC_SingleEnded(3));
  Serial.print("adc3: ");
  Serial.println(adc3);

  /* ADC 1 */
  adc4=double(ads1.readADC_SingleEnded(0));
  Serial.print("adc0: ");
  Serial.println(adc4);
  
  adc5=double(ads1.readADC_SingleEnded(1));
  Serial.print("adc1: ");
  Serial.println(adc5);
  
  adc6=double(ads1.readADC_SingleEnded(2));
  Serial.print("adc2: ");
  Serial.println(adc6);
  
  adc7=double(ads1.readADC_SingleEnded(3));
  Serial.print("adc3: ");
  Serial.println(adc7);

  /* Write to TFT screen */

  //Write bllue square to erase
  tft.fillRect(40,0,40,135, RA8875_BLUE);   
  tft.textTransparent(RA8875_WHITE);
  
  //Write text
  tft.textSetCursor(1, 1);
  tft.textWrite("ADC0: ");
  tft.textSetCursor(40, 1);
  tft.textWrite(dtostrf(adc0,5,0,string),6);

  tft.textSetCursor(1, 15);
  tft.textWrite("ADC1: ");
  tft.textSetCursor(40, 15);
  tft.textWrite(dtostrf(adc1,5,0,string),6);

  tft.textSetCursor(1, 30);
  tft.textWrite("ADC2: ");
  tft.textSetCursor(40, 30);
  tft.textWrite(dtostrf(adc2,5,0,string),6);

  tft.textSetCursor(1, 45);
  tft.textWrite("ADC3: ");
  tft.textSetCursor(40, 45);
  tft.textWrite(dtostrf(adc3,5,0,string),6);

  tft.textSetCursor(1, 60);
  tft.textWrite("ADC4: ");
  tft.textSetCursor(40, 60);
  tft.textWrite(dtostrf(adc4,5,0,string),6);

  tft.textSetCursor(1, 75);
  tft.textWrite("ADC5: ");
  tft.textSetCursor(40, 75);
  tft.textWrite(dtostrf(adc5,5,0,string),6);

  tft.textSetCursor(1, 90);
  tft.textWrite("ADC6: ");
  tft.textSetCursor(40, 90);
  tft.textWrite(dtostrf(adc6,5,0,string),6);

  tft.textSetCursor(1, 105);
  tft.textWrite("ADC7: ");
  tft.textSetCursor(40, 105);
  tft.textWrite(dtostrf(adc7,5,0,string),6);

  tft.textSetCursor(1, 120);
  tft.textWrite("DIR: ");
  tft.textSetCursor(40, 120);
  if (low)
    {
      tft.textWrite("low");
    }
  else
    {
      tft.textWrite("high");
    }

  
  delay(200);
}
