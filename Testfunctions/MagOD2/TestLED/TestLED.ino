//Pins for the RGB LED.
#define LED_red 2 //RED LED
#define LED_green 32 //Green LED
#define LED_blue 4 //BLUE LED

/* Coil variables (to switch them off) */
uint8_t Coil_x = 33; // output of the coils in the x direction
uint8_t Coil_y = 26; // output of the coils in the y direction
uint8_t Coil_z = 14; // output of the coils in the z direction
const int ledChannel_x = 3; /*0-15*/
const int ledChannel_y = 4; /*0-15*/
const int ledChannel_z = 5; /*0-15*/


void setup () {
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

  pinMode(LED_red, OUTPUT);
  pinMode(LED_green, OUTPUT);
  pinMode(LED_blue, OUTPUT);
}


void loop()
{
  /* Cycle through RED, GREEN, BLUE, PAUZE for 1 sec each */
  digitalWrite(LED_red, HIGH);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_blue, LOW);
  delay(1000);

  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, HIGH);
  digitalWrite(LED_blue, LOW);
  delay(1000);

  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_blue, HIGH);
  delay(1000);

  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_blue, LOW);
  delay(1000);

  
}
