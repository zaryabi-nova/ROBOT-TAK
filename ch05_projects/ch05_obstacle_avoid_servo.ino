#include <Servo.h>

const int IN1 = 7, IN2 = 6, IN3 = 5, IN4 = 4;   // L298N
const int TRIG = 10, ECHO = 9;
const int SERVO_PIN = 3;
const int SAFE_CM = 25;
const int LEFT_DEG = 160, AHEAD_DEG = 90, RIGHT_DEG = 20;   // check on your servo

Servo eye;                                   // servo that turns the sensor

void motors(int left, int right) {
  digitalWrite(IN1, left > 0);  digitalWrite(IN2, left < 0);
  digitalWrite(IN3, right > 0); digitalWrite(IN4, right < 0);
}

float readCm() {
  digitalWrite(TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long t = pulseIn(ECHO, HIGH, 25000);
  return (t == 0) ? 400 : t / 58.0;
}

float look(int angle) {                      // turn the sensor, wait, measure
  eye.write(angle);
  delay(350);
  return readCm();
}

void setup() {
  int outs[] = {IN1, IN2, IN3, IN4, TRIG};
  for (int p : outs) pinMode(p, OUTPUT);
  pinMode(ECHO, INPUT);
  eye.attach(SERVO_PIN);
  eye.write(AHEAD_DEG);
  delay(500);
}

void loop() {
  if (readCm() > SAFE_CM) { motors(1, 1); delay(30); return; }

  motors(0, 0);   delay(150);                // obstacle: stop and back up
  motors(-1, -1); delay(300);
  motors(0, 0);

  float left  = look(LEFT_DEG);              // look both ways
  float right = look(RIGHT_DEG);
  eye.write(AHEAD_DEG);

  if (left < SAFE_CM && right < SAFE_CM) {   // dead end: turn around
    motors(1, -1); delay(800);
  } else if (left > right) {                 // more room on the left
    motors(-1, 1); delay(400);
  } else {                                   // more room on the right
    motors(1, -1); delay(400);
  }
  motors(0, 0);
}
