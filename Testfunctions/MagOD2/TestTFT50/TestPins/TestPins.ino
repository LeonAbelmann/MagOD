#include <SPI.h>

void setup() 
{
  Serial.begin(9600);

  // Temporary fix: pull original CS high to "output enable" the buffers
  //pinMode(5, OUTPUT);
  //digitalWrite(5, LOW);
  //delay(2000);
  
  Serial.println("test start");

  Serial.println("CS (5) high");
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  pinMode(18, OUTPUT);
  pinMode(19, INPUT);
  pinMode(23, OUTPUT);
 
}

void loop() 
{
  int pin;
  Serial.println("23 high");
  digitalWrite(23, HIGH);
  
  pin = 19;
  Serial.print("Pin: ");
  Serial.println(pin);
  //pinMode(pin, INPUT);
  Serial.println(digitalRead(pin));
  delay(3000);

  Serial.println("23 low");
  digitalWrite(23, LOW);
  
  pin = 19;
  Serial.print("Pin: ");
  Serial.println(pin);
  //pinMode(pin, INPUT);
  Serial.println(digitalRead(pin));
  delay(3000);

  
  // pinMode(pin, OUTPUT);
  // Serial.println("LOW");
  // digitalWrite(pin, LOW);
  // delay(3000);
  // Serial.println("HIGH");
  // digitalWrite(pin, HIGH);
  // delay(3000);

  // Serial.println("CS (5) low");
  // pinMode(5, OUTPUT);
  // pinMode(5, LOW);

  // Serial.print("Pin: ");
  // Serial.println(pin);
  // pinMode(pin, OUTPUT);
  // Serial.println("LOW");
  // digitalWrite(pin, LOW);
  // delay(3000);
  // Serial.println("HIGH");
  // digitalWrite(pin, HIGH);
  // delay(3000);
 
}
