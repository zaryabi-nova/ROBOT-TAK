// Rabat Tak - Vol.2 section 9-4-3: drive a robot with a PS5 / PS4 / Xbox controller (Bluepad32)
// Board: classic ESP32 (DualSense and DualShock need Bluetooth Classic; S3/C3 support only BLE pads)
// Arduino IDE: install board package "esp32_bluepad32" and select an ESP32 board from it.
// Pairing PS5: hold Create + PS button until the light bar flashes quickly.
// Motors: PWM GPIO25, IN1 26, IN2 27 | PWM GPIO14, IN3 12, IN4 13 (L298N or TB6612)
#include <Bluepad32.h>
ControllerPtr pad = nullptr;

void onConnected(ControllerPtr c)    { if (!pad) { pad = c; Serial.println("controller connected"); } }
void onDisconnected(ControllerPtr c) { if (c == pad) { pad = nullptr; Serial.println("controller lost"); } }

void motor(int pwm, int in1, int in2, int s) {
  s = constrain(s, -255, 255);
  digitalWrite(in1, s > 0); digitalWrite(in2, s < 0);
  analogWrite(pwm, abs(s));
}

void setup() {
  Serial.begin(115200);
  int outs[] = {25, 26, 27, 14, 12, 13};
  for (int p : outs) pinMode(p, OUTPUT);
  BP32.setup(&onConnected, &onDisconnected);
  BP32.forgetBluetoothKeys();          // pair again on each start (remove after testing)
}

void loop() {
  BP32.update();
  int fwd = 0, turn = 0;
  if (pad && pad->isConnected()) {
    fwd  = -pad->axisY() / 2;           // left stick: -511..512 -> about -255..255
    turn =  pad->axisRX() / 2;          // right stick X
    if (abs(fwd) < 20) fwd = 0;
    if (abs(turn) < 20) turn = 0;
    if (pad->a()) { fwd = 0; turn = 0; } // Cross (X) = stop
    pad->setColorLED(fwd ? 0 : 255, fwd ? 255 : 0, 0);
  }                                      // no controller = motors off (failsafe)
  motor(25, 26, 27, fwd + turn);
  motor(14, 12, 13, fwd - turn);
  delay(10);
}
