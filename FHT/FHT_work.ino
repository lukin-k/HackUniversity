#define FHT_N 256
#define LOG_OUT 1
#define LOW_PASS 35
#define INPUT_GAIN 1.5
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#include <FHT.h>
#include <Servo.h>
byte posOffset[33]={2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 25, 32, 36, 40, 44, 48, 52, 57, 62, 67, 72, 78, 84, 90, 96, 102, 108, 120};
Servo Ser;
void setup() {
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);
  analogReference(EXTERNAL);
  Ser.attach(11);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}
int ang=0;

void loop() {
  for (int a=0; a<2; a++){
    Serial.println("");
    Serial.print(a);
    Serial.print(") ");
    for (int i = 0 ; i < FHT_N ; i++) {
      int sample;
      if(!a)
        sample = analogRead(A0);
      else
        sample = analogRead(A1);
      fht_input[i] = sample; // put real data into bins
    }
    fht_window();  // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run();     // process the data in the fht
    fht_mag_log(); // take the output of the fht
    
    for (int i = 100; i < 128; i ++) {
        if (fht_log_out[i] < LOW_PASS) fht_log_out[i] = 0;
        fht_log_out[i] = (float)fht_log_out[i] * INPUT_GAIN;
    }
      byte pos = 31;
      int posLevel = fht_log_out[posOffset[pos]];
      byte linesBetween;
      
      if(pos > 0){
        linesBetween = posOffset[pos] - posOffset[pos - 1];
        for (byte i = 0; i < linesBetween; i++) {
          posLevel += (float) ((float)i / linesBetween) * fht_log_out[posOffset[pos] - linesBetween + i];
        }
      }
      if(pos < 32){
        linesBetween = posOffset[pos + 1] - posOffset[pos];
        for (byte i = 0; i < linesBetween; i++) {
          posLevel += (float) ((float)i / linesBetween) * fht_log_out[posOffset[pos] + linesBetween - i];
        }
      }
      Serial.print(posLevel);
      if(posLevel)
        digitalWrite(13, 1);
      else
        digitalWrite(13, 0);
      Serial.print(" ");
      if(!a)
        ang=posLevel;
      else{
        ang-=posLevel/2;
        if(ang>=0){
          ang=constrain(ang, 0, 400);
          ang=map(ang, 0, 400, 90, 0);
        }
        if(ang<0){
          ang=-ang;
          ang=constrain(ang, 0, 200);
          ang=map(ang, 0, 200, 91, 180);
        }
        Serial.print("ang=");
        Ser.write(ang);
        Serial.print(ang);
        Serial.println("");
      }
  }
  Serial.println("");
  delay(200);
}








