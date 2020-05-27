#include <SPI.h> // Communication
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_RA8875.h> // TFTM50 screen
#include "src/MagOD_ADS1115/MagOD_ADS1115.h" // Modified Adafruit_ADS1015 lib
#include "src/timer/timer.h"

const int N          = 500; // Averaging
uint8_t   sampleRate = 0;  /* Sample rates: 0: 8SPS, 1: 16 SPS, 2: 32
			   SPS, 3: 64 SPS, 4: 128 SPS, 5: 250 SPS 6:
			   475 SPS 7: 860 SPS */
float freq_intensity = 0.1; //Change of intensity frequency in Hz
int channel[] = {1,3}; // Which channels to sample (only ads0) {0,1,2,3} 
int numChan   = 2;         // Number of channels in above channel array (max 4)

//Pins for the RGB LED.
#define LED_red 2 //RED LED
#define LED_green 32 //Green LED
#define LED_blue 4 //BLUE LED

/* Intialize screen */
#define RA8875_INT 16
#define RA8875_CS 5 
#define RA8875_RESET 17
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
// ADS1115 for adc0-3 (0x48) has ALERT/RDY connected to pin 13 of EPS32  
#define ADS1115_RDY 13
Adafruit_ADS1115 ads0(ADS1015_ADDRESS_0); //adc0-3;
Adafruit_ADS1115 ads1(ADS1015_ADDRESS_1); //adc4-7;

/* initialize timer */
timer mytimer;
hw_timer_t * timer1 = NULL;
hw_timer_t * timer3 = NULL;
hw_timer_t * timer4 = NULL;

/* LED variables */
const int LEDChannelRed = 0;
const int LEDChannelGreen = 1;
const int LEDChannelBlue = 2;
// Maximum frequency: 80 MHz/2^LEDresolutions
// 1  bit:  40 MHz
// 8  bit: 312 kHz
//16  bit: 1220 Hz
const int LEDresolution = 8;
const int LEDfreq = 200000;


/* Coil variables (to switch them off) */
uint8_t Coil_x = 33; // output of the coils in the x direction
uint8_t Coil_y = 26; // output of the coils in the y direction
uint8_t Coil_z = 14; // output of the coils in the z direction
const int ledChannel_x = 3; /*0-15*/
const int ledChannel_y = 4; /*0-15*/
const int ledChannel_z = 5; /*0-15*/

struct adc {
  double average; 
  double stdev; 
  double extremes; 
};

bool adcReady = false; // Flag that raises when conversion is done
bool changeIntensity=false; // Flag that raises when timer has finished
int iChan     = 0;     // Counter for channel array [0..chanNum].
uint16_t adc0[N], adc1[N], adc2[N], adc3[N]; //Bit values of adc0
uint32_t t0[N], t1[N], t2[N], t3[N];         //Time at which data is taken
adc adc0r, adc1r, adc2r, adc3r; // results

uint16_t adcValue; 
long int currenttime;
long int sampletime;
char string[15]; //Buffer holding string for dtostrf
int samplenum = 0;
int i = 0;
int time0;
int intensity=0;
int d_int=0;

const int RED=1;
const int GREEN=2;
const int BLUE=3;
int colors[]={RED,GREEN,BLUE}; //or {GREEN,BLUE}, or {RED, GREEN, BLUE} :)
int size_colors = sizeof(colors)/sizeof(colors[0]);

adc analyze(uint16_t adcin[], int N) {
  double sum     = 0;
  double average = 0;
  double sumsqr  = 0;
  double diff    = 0;
  double min     = adcin[0];
  double max     = adcin[0];
  adc results;
  // First calculate average value
  for  (int i = 0; i < N; i++) {
    sum = sum + adcin[i];
  }
  average = sum/N;
  /* Than calculate sum of square of difference with average, and
     while you at it, calculate min and max values */
  for  (int i = 0; i < N; i++) {
    diff = (adcin[i]-average);
    sumsqr = sumsqr + diff*diff;
    if ( adcin[i] < min) {min = adcin[i];};
    if ( adcin[i] > max) {max = adcin[i];};
  }
  // Return results
  results.average = average;
  if (N>1) {
    results.stdev = sqrt((1.0/(N-1))*sumsqr);
  }
  else {
    results.stdev =0;
  }
  results.extremes= (max-min);
  return results;
}

void tftWrite(int i,adc adcr){
    tft.textSetCursor(1, 20+i*15);
    tft.textWrite("ADC");
    tft.textSetCursor(24, 20+i*15);
    tft.textWrite(dtostrf(i,1,0,string),2);
    tft.textSetCursor(40, 20+i*15);
    tft.textWrite(dtostrf(adcr.average,5,0,string),6);
    tft.textSetCursor(100, 20+i*15);
    tft.textWrite(dtostrf(adcr.stdev,5,0,string),6);
}

void serialWrite(int i,adc adcr){
  Serial.print("adc");Serial.print(i);Serial.print(": ");
  Serial.print(adcr.average);Serial.print(", ");
  Serial.print(adcr.stdev);Serial.print(", ");
  Serial.println(adcr.extremes);
}

//Initialization of timer for intensity change
void Init_timer()
{
  mytimer.initTimer(1,long(1000000/freq_intensity));
  mytimer.attachInterrupt(1, intensityEvent);
}

/* Timer 1 triggers a change in intensity */
void intensityEvent()
{
  changeIntensity=true;
}


void setup () {
  Serial.begin(115200);
  delay(1000);

  // configure LED PWM functionalitites
  ledcSetup(LEDChannelRed, LEDfreq, LEDresolution);
  ledcSetup(LEDChannelGreen, LEDfreq, LEDresolution);
  ledcSetup(LEDChannelBlue, LEDfreq, LEDresolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(LED_red, LEDChannelRed);
  ledcAttachPin(LED_green, LEDChannelGreen);
  ledcAttachPin(LED_blue, LEDChannelBlue);

  //Switch off the magnets:
  ledcSetup(ledChannel_x, 1000, 8);
  ledcSetup(ledChannel_y, 1000, 8);
  ledcSetup(ledChannel_z, 1000, 8);

  ledcAttachPin(Coil_x, ledChannel_x);
  ledcAttachPin(Coil_y, ledChannel_y);
  ledcAttachPin(Coil_z, ledChannel_z);

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
  // ads0 is interupt based. Pin 13 is connect to ALERT/RDY pin of ads0 
  pinMode(ADS1115_RDY,INPUT_PULLUP);
  // Interupt when RDY goes down
  attachInterrupt(ADS1115_RDY, adcISR, FALLING);
  /* The first sample is tricky. We need to start with adcReady true,
     and read the register once. At the lowest sample rate (8SPS), we
     need to wait 130 ms for the conversion to finish. All other rates
     are faster.  */
  adcReady = true;
  ads0.startReadADC(channel[0],sampleRate);
  delay(130);//

  // Init Timer
  Init_timer();
  
  // Initiliaze display
  tft.displayOn(true);
  tft.GPIOX(true);      // Enable TFT - display enable tied to GPIOX
  tft.PWM1config(true, RA8875_PWM_CLK_DIV1024); // PWM output for backlight
  tft.PWM1out(255);
  tft.fillScreen(RA8875_BLACK);
  tft.textMode();

  tft.textTransparent(RA8875_WHITE);
  tft.textSetCursor(200, 1);
  tft.textWrite("TestNoise.ino");
  tft.textSetCursor(1, 1);
  tft.textWrite("     Average Stddev");
}

/* buttonISR: runs when button is pressed */
//void IRAM_ATTR adcISR()
void  adcISR()
 {
   adcReady = true;
 }

void loop()
{
  if (adcReady) {
    adcReady = false;
    adcValue=ads0.getADC();
    // samplenum = samplenum +1;
    // if (samplenum % 1 == 0) { // slow down serial output
    //   Serial.print(samplenum);
    //   Serial.print(" channel: ");
    //   Serial.print(channel[iChan]);
    //   Serial.print( " Time: ");
    //   Serial.print(sampletime);
    //   Serial.print(" value: ");Serial.println(adcValue);
    // }
    switch (channel[iChan]) {
    case 0 :
      adc0[i]=adcValue;
      t0[i]=millis();
      break;
    case 1 :
      adc1[i]=adcValue;
      t1[i]=millis();
      break;
    case 2 :
      adc2[i]=adcValue;
      t2[i]=millis();
      break;
    case 3 :
      adc3[i]=adcValue;
      t3[i]=millis();
      break;
    }
    iChan=iChan+1;//next channel
    if (iChan > numChan-1) {
      iChan=0;//fold over
      i=i+1;  //fill arrays with next values
    }
    ads0.startReadADC(channel[iChan],sampleRate);
  }
  
  if (changeIntensity) {
    changeIntensity=false;
    intensity = 0.18*pow(2,LEDresolution);
    if (d_int == 0) {d_int = 1;}
    else {d_int = 0;}
    intensity = intensity + d_int;
    Serial.print("change intensity to: ");
    Serial.println(intensity);
    ledcWrite(LEDChannelRed,intensity);
    //delay(1000);//wait for the led to stabilize	  
  }
  
  if (i>=N) {
    i=0;
    sampletime=millis()-currenttime;

    /* Calculate */
    adc0r = analyze(adc0,N);
    adc1r = analyze(adc1,N);
    adc2r = analyze(adc2,N);
    adc3r = analyze(adc3,N);
    
    /* Write to serial port */
    Serial.print("time: ");
    Serial.println(sampletime);
    //serialWrite(0,adc0r);
    serialWrite(1,adc1r);
    // serialWrite(2,adc2r);
    serialWrite(3,adc3r);
    Serial.println("#t1, adc1, t3, adc3 ");
    for (int j=0;j<N;j++){
      Serial.print(t1[j]);Serial.print(", ");
      Serial.print(adc1[j]);Serial.print(", ");
      Serial.print(t3[j]);Serial.print(", ");
      Serial.print(adc3[j]);
      Serial.println();
    }
    
    /* Write to TFT screen */
    //Write blue square to erase
    tft.fillRect(40,20,100,120, RA8875_BLUE);   
    tft.textTransparent(RA8875_WHITE);
    //Write text
    tftWrite(0,adc0r);
    tftWrite(1,adc1r);
    tftWrite(2,adc2r);
    tftWrite(3,adc3r);
    currenttime = millis();
  }
  //delay(200);
}
