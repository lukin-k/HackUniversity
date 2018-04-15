char val = '1';

void setup() {
        Serial.begin(9600); 
        pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    val = Serial.read();
//    Serial.println(val);
    if (val=='1'){
//      Serial.println("hoh1");
      digitalWrite(13,HIGH);
    }
    if (val=='0'){
      digitalWrite(13,LOW);
//      Serial.println("hoh2");
    }
  }
}
