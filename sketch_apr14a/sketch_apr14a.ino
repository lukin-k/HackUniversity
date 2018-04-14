#define LOG_OUT 1
#define LOW_PASS 30
#define INPUT_GAIN 1.5
#define FHT_N 256
#include <FHT.h>
#define MIC_PIN A0
byte posOffset[17] = {2, 3, 4, 6, 8, 10, 12, 14, 16, 20, 25, 30, 35, 60, 80, 100, 127};
//byte posOffset[17] = {2, 3, 4, 6, 8, 10, 12, 14, 30, 40, 50, 60, 80, 120, 160, 200, 240};
double prevVolts = 100.0;

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void setup() {
  Serial.begin(9600);

  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  analogReference(EXTERNAL);
}

void loop() {
  for (int i = 0 ; i < FHT_N ; i++) {
    int sample = analogRead(MIC_PIN);
    fht_input[i] = sample;
  }
  fht_window();
  fht_reorder();
  fht_run();
  fht_mag_log();

  for (int i = 0; i < 128; i ++) {
      if (fht_log_out[i] < LOW_PASS) fht_log_out[i] = 0;
      fht_log_out[i] = (float)fht_log_out[i] * INPUT_GAIN;
    }

    for (byte pos = 0; pos < 16; pos++) {
      int posLevel = fht_log_out[posOffset[pos]];
      byte linesBetween;
      if (pos > 0 && pos < 16) {
        linesBetween = posOffset[pos] - posOffset[pos - 1];
        for (byte i = 0; i < linesBetween; i++) {
          posLevel += (float) ((float)i / linesBetween) * fht_log_out[posOffset[pos] - linesBetween + i];
        }
        linesBetween = posOffset[pos + 1] - posOffset[pos];
        for (byte i = 0; i < linesBetween; i++) {
          posLevel += (float) ((float)i / linesBetween) * fht_log_out[posOffset[pos] + linesBetween - i];
        }
      }
      posLevel = map(posLevel, LOW_PASS, 80, 0, 16);
      posLevel = constrain(posLevel, 0, 16);
      Serial.print(posLevel);
      Serial.print(" ");
    }
  Serial.println("");
  delay(100);
}
