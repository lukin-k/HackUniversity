//#define SerPin 11
//#define DelDur 30
//#include <Servo.h>
//Servo Ser;
//void setup() {
//  Ser.attach(SerPin);
//  Serial.begin(9600);
////  pinMode(A0, INPUT);
//}
//
//void loop() {
//  double k=analogRead(A0);
////  Serial.println((k*29)/10000);
////  Serial.println(analogRead(A0));
//  if (!(k>=0&&k<=100)){
//    Serial.println(k);
//    Serial.println(map(k,100,600,0,180));
//    Ser.write(map(k,100,600,0,180));
//  }
////  Ser.write(map(k,100,600,0,180));
////  for(int i=15;i<=165;i++)
////  {  
////    Ser.write(i);
////    delay(DelDur);
////  }
////  for(int i=165;i>15;i--)
////  {  
////    Ser.write(i);
////    delay(DelDur);
////  }
//delay(100);
//}

#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht
#include <FHT.h> // include the library
void setup() {
   Serial.begin(115200); // use the serial port
   TIMSK0 = 0; // turn off timer0 for lower jitter
   ADCSRA = 0xe5; // set the adc to free running mode
   ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}
void loop() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
    while(!(ADCSRA & 0x10)); // wait for adc to be ready
         ADCSRA = 0xf5; // restart adc
         byte m = ADCL; // fetch adc data
         byte j = ADCH;
         int k = (j << 8) | m; // form into an int
         k -= 0x0200; // form into a signed int
         k <<= 6; // form into a 16b signed int
         fht_input[i] = k; // put real data into bins
       }
       fht_window(); // window the data for better frequency response
       fht_reorder(); // reorder the data before doing the fht
       fht_run(); // process the data in the fht
       fht_mag_log(); // take the output of the fht
       sei();
//       Serial.write(255); // send a start byte
       Serial.write("\n");
//       Serial.write(fht_log_out, FHT_N/2); // send out the data
      for (int ko=0; ko<FHT_N/2;++ko){
        Serial.print(fht_log_out[ko]);
        Serial.print(" ");
      }
     }
   }
