int fsrAnalogPin = 0; // FSR is connected to analog 0
//int LEDpin = 9;      // connect Red LED to pin 11 (PWM pin)
int fsrReading;      // the analog reading from the FSR resistor divider
//int LEDbrightness;

void setup() {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
  //pinMode(LEDpin, OUTPUT);
  pinMode(8, OUTPUT);

}

void loop() {

  fsrReading = analogRead(fsrAnalogPin);
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);
  if (fsrReading >= 100){

  digitalWrite(8, HIGH);
  //delay(50);
  
 //delay(50);
    
  }

  else {

 digitalWrite(8, LOW);
  } 
  // we'll need to change the range from the analog reading (0-1023) down to the range
  // used by analogWrite (0-255) with map!
  //LEDbrightness = map(fsrReading, 0, 1023, 0, 255);
  // LED gets brighter the harder you press
  //digitalWrite(LEDpin, LEDbrightness);
 
  delay(100);

  


}
