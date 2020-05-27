// setting PWM properties
const int LED_blue = 32;
const int freq = 1000;
const int ledChannel = 0;
const int resolution = 8;

void setup () {
// configure LED PWM functionalitites
ledcSetup(ledChannel, freq, resolution);

// attach the channel to the GPIO to be controlled
ledcAttachPin(LED_blue, ledChannel);
}


void loop()
{
  for( int intensity = 0; intensity <=255;intensity=intensity+5){
  ledcWrite(ledChannel, intensity);
  delay(100);
  }
}