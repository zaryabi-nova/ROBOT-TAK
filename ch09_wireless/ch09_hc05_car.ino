// Bluetooth car: Arduino Uno + HC-05 + L298N. Phone app sends one letter per command.
#include <SoftwareSerial.h>

SoftwareSerial bt(10, 11);                 // RX=10 (from HC-05 TX), TX=11 (to HC-05 RX via divider)
const int ENA = 5, IN1 = 7, IN2 = 8;       // left motor
const int ENB = 6, IN3 = 9, IN4 = 4;       // right motor
int speedPwm = 180;
unsigned long lastCmd = 0;

void motor(int en, int a, int b, int s) {  // s: -255..255
  digitalWrite(a, s > 0);
  digitalWrite(b, s < 0);
  analogWrite(en, abs(s));
}

void drive(int l, int r) { motor(ENA, IN1, IN2, l); motor(ENB, IN3, IN4, r); }

void setup() {
  int outs[] = {ENA, IN1, IN2, ENB, IN3, IN4};
  for (int p : outs) pinMode(p, OUTPUT);
  bt.begin(9600);                          // HC-05 default baud rate
  Serial.begin(9600);
}

void loop() {
  if (bt.available()) {
    char c = bt.read();
    Serial.println(c);
    lastCmd = millis();
    switch (c) {
      case 'F': drive(speedPwm, speedPwm); break;      // forward
      case 'B': drive(-speedPwm, -speedPwm); break;    // back
      case 'L': drive(-speedPwm, speedPwm); break;     // turn left
      case 'R': drive(speedPwm, -speedPwm); break;     // turn right
      case 'S': drive(0, 0); break;                    // stop
      default:
        if (c >= '0' && c <= '9') speedPwm = map(c - '0', 0, 9, 80, 255);  // speed 0..9
    }
  }
  if (millis() - lastCmd > 700) drive(0, 0);   // safety: no command -> stop
}
