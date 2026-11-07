// Bluetooth car with ESP32 (classic Bluetooth, original ESP32 only) + L298N
#include <BluetoothSerial.h>

BluetoothSerial bt;
const int ENA = 25, IN1 = 26, IN2 = 27;    // left motor
const int ENB = 14, IN3 = 12, IN4 = 13;    // right motor
int speedPwm = 180;
unsigned long lastCmd = 0;

void motor(int en, int a, int b, int s) {
  digitalWrite(a, s > 0);
  digitalWrite(b, s < 0);
  analogWrite(en, abs(s));
}
void drive(int l, int r) { motor(ENA, IN1, IN2, l); motor(ENB, IN3, IN4, r); }

void setup() {
  int outs[] = {ENA, IN1, IN2, ENB, IN3, IN4};
  for (int p : outs) pinMode(p, OUTPUT);
  bt.begin("RoboTak-Car");                 // name shown on the phone
}

void loop() {
  if (bt.available()) {
    char c = bt.read();
    lastCmd = millis();
    if (c == 'F') drive(speedPwm, speedPwm);
    else if (c == 'B') drive(-speedPwm, -speedPwm);
    else if (c == 'L') drive(-speedPwm, speedPwm);
    else if (c == 'R') drive(speedPwm, -speedPwm);
    else if (c == 'S') drive(0, 0);
    else if (c >= '0' && c <= '9') speedPwm = map(c - '0', 0, 9, 80, 255);
  }
  if (millis() - lastCmd > 700) drive(0, 0);
}
