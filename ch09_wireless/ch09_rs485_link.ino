// Rabat Tak - Vol.2 section 9-2: RS485 link between two Arduino Uno boards (MAX485)
// Wiring (each board): RO->D10, DI->D11, DE+RE->D2, VCC 5V, GND; A-A, B-B between modules
// Upload once with MASTER 1 and once with MASTER 0
#include <SoftwareSerial.h>
#define MASTER 1
const int DIR = 2;               // HIGH = send, LOW = receive
SoftwareSerial bus(10, 11);      // RX, TX

void setup() {
  pinMode(DIR, OUTPUT); digitalWrite(DIR, LOW);
  pinMode(LED_BUILTIN, OUTPUT);
  bus.begin(9600); Serial.begin(9600);
}

void send(char c) {
  digitalWrite(DIR, HIGH);
  bus.write(c); bus.flush();     // wait until the byte has left
  digitalWrite(DIR, LOW);
}

void loop() {
#if MASTER
  static bool on = false;
  on = !on; send(on ? '1' : '0');
  Serial.println(on ? "sent 1" : "sent 0");
  delay(1000);
#else
  if (bus.available()) {
    char c = bus.read();
    if (c == '1') digitalWrite(LED_BUILTIN, HIGH);
    if (c == '0') digitalWrite(LED_BUILTIN, LOW);
    Serial.print("got "); Serial.println(c);
  }
#endif
}
