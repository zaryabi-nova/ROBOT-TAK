// Rabat Tak - Vol.2 section 9-8: long-range serial link with HC-12 (433 MHz)
// HC-12: TXD->D10, RXD->D11, SET->D4, VCC 5V (100 uF capacitor across VCC/GND), GND
// Same sketch on both boards. Type in Serial Monitor (9600, Newline) to send.
// Lines starting with "AT" go to the module in setup mode, e.g. AT+C001 (channel), AT+RX
#include <SoftwareSerial.h>
SoftwareSerial hc12(10, 11);
const int SET = 4;

void setup() {
  pinMode(SET, OUTPUT); digitalWrite(SET, HIGH);     // HIGH = normal (transparent) mode
  Serial.begin(9600); hc12.begin(9600);
  Serial.println("HC-12 ready");
}

void loop() {
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    bool at = s.startsWith("AT");
    if (at) { digitalWrite(SET, LOW); delay(50); }   // enter command mode
    hc12.print(s);
    if (at) { delay(100); while (hc12.available()) Serial.write(hc12.read());
              digitalWrite(SET, HIGH); delay(100); }
  }
  while (hc12.available()) Serial.write(hc12.read());
}
