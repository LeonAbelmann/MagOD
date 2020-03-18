String someItems[5]={"Item1","Item2","Item3"};

void setup() {
   myFunction(someItems);
}

void loop(){}

void myFunction(String items[5]) {

  for(int i=0;i<5;i++) {
    Serial.println(items[i]);
  } 

}
