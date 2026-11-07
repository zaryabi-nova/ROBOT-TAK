// Rabat Tak - Vol.2 section 9-7: FM radio with RDA5807M and Arduino Uno
// RDA5807M: SDA->A4, SCL->A5, VCC 3.3V, GND; headphones on the module jack
// Buttons: D4 = seek up, D5 = volume up (to GND, INPUT_PULLUP). Library: RDA5807 (PU2CLR)
#include <RDA5807.h>
RDA5807 rx;
int vol = 6;

void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP); pinMode(5, INPUT_PULLUP);
  rx.setup();
  rx.setVolume(vol);
  rx.setFrequency(10390);          // 103.9 MHz (unit = 10 kHz)
  show();
}

void show() {
  Serial.print("Freq "); Serial.print(rx.getRealFrequency() / 100.0);
  Serial.print(" MHz  RSSI "); Serial.print(rx.getRssi());
  Serial.print("  Vol "); Serial.println(vol);
}

void loop() {
  if (digitalRead(4) == LOW) { rx.seek(RDA_SEEK_WRAP, RDA_SEEK_UP); show(); delay(300); }
  if (digitalRead(5) == LOW) { vol = (vol + 1) % 16; rx.setVolume(vol); show(); delay(300); }
}
