// Rabat Tak - Vol.2 section 9-3: 433 MHz ASK modules (FS1000A / XY-MK-5V) with RCSwitch
// Transmitter board: FS1000A DATA -> D10, VCC 5V (up to 12V = more range), GND; button D4 -> GND
// Receiver board:    XY-MK-5V DATA -> D2 (interrupt 0), VCC 5V, GND; LED on D13
// Antenna: 17.3 cm straight wire on ANT of both modules. Library: rc-switch
// Upload with SENDER 1 on one Arduino and SENDER 0 on the other.
#include <RCSwitch.h>
#define SENDER 1
RCSwitch rf;
const unsigned long CODE_ON = 5393, CODE_OFF = 5396;

void setup() {
  Serial.begin(9600);
#if SENDER
  rf.enableTransmit(10);
  rf.setRepeatTransmit(5);
  pinMode(4, INPUT_PULLUP);
#else
  rf.enableReceive(0);               // interrupt 0 = pin D2 on Uno/Nano
  pinMode(13, OUTPUT);
#endif
}

void loop() {
#if SENDER
  static bool on = false;
  if (digitalRead(4) == LOW) {
    on = !on;
    rf.send(on ? CODE_ON : CODE_OFF, 24);
    Serial.println(on ? "sent ON" : "sent OFF");
    delay(400);
  }
#else
  if (rf.available()) {
    unsigned long v = rf.getReceivedValue();
    Serial.print("code "); Serial.print(v);
    Serial.print("  bits "); Serial.println(rf.getReceivedBitlength());
    if (v == CODE_ON)  digitalWrite(13, HIGH);
    if (v == CODE_OFF) digitalWrite(13, LOW);
    rf.resetAvailable();
  }
#endif
}
