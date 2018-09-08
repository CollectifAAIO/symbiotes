

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 3:
  int proxiValue = analogRead(A3);
  int micValue = analogRead(A13);
  // print out the value you read:
  int proxi = map(proxiValue, 0, 1023, 0, 100); //mapping values
  int mic = map(micValue, 0, 1023, 101, 200);
  Serial.write(proxi);
  Serial.write(mic);
  delay(50);        // delay in between reads for stability
}
