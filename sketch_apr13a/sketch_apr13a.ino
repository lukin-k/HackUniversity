void setup() {
  Serial.begin(9600);
//  pinMode(A0, INPUT);
}

void loop() {
  double k=analogRead(A0);
  Serial.println((k*29)/10000);
  delay(100);
}
