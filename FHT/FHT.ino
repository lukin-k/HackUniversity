#define FHT_N 256
#define LOG_OUT 1
#define LOW_PASS 35
#define INPUT_GAIN 1.5
#define DEF_GAIN 100
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#include <FHT.h>
byte posOffset[33]={2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 20, 25, 32, 36, 40, 44, 48, 52, 57, 62, 67, 72, 78, 84, 90, 96, 102, 108, 120};

void setup() {
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);
  analogReference(EXTERNAL);
  Serial.begin(9600);
}

void loop() {
  for (int i = 0 ; i < FHT_N ; i++) {
    int sample = analogRead(AUDIO_IN);
    fht_input[i] = sample; // put real data into bins
  }
  fht_window();  // window the data for better frequency response
  fht_reorder(); // reorder the data before doing the fht
  fht_run();     // process the data in the fht
  fht_mag_log(); // take the output of the fht
  
  for (int i = 0; i < 128; i ++) {
      if (fht_log_out[i] < LOW_PASS) fht_log_out[i] = 0;
      fht_log_out[i] = (float)fht_log_out[i] * INPUT_GAIN;
  }
  for (byte pos = 0; pos < 32; pos++) {
    int posLevel = fht_log_out[posOffset[pos]];
    byte linesBetween;
//    if (pos > 0 && pos < 32) {
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
    Serial.print(" ");
  }
  Serial.println("");
  delay(100);
}








