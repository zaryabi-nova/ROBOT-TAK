// Rabat Tak - Vol.2 section 8-3-2: PID distance keeping ("follow me" robot)
// HC-SR04: TRIG -> D9, ECHO -> D8 | L298N: ENA D5, IN1 D7, IN2 D6, ENB D3, IN3 D4, IN4 D2
// The robot keeps TARGET cm from the object in front of it. Open Serial Plotter (115200).
const int TRIG = 9, ECHO = 8;
const float TARGET = 20.0;            // cm
float Kp = 12.0, Ki = 0.0, Kd = 0.0;  // start with P only, then add I and D (section 8-3-3)
const float DT = 0.05;                // 50 ms control period
float integ = 0, lastDist = 0, dist = 0;
unsigned long lastT = 0;

float readCm() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10); digitalWrite(TRIG, LOW);
  long us = pulseIn(ECHO, HIGH, 25000);          // timeout 25 ms (about 4 m)
  return us ? us / 58.0 : 400;
}

void drive(int s) {                               // s: -255..255, + = forward
  s = constrain(s, -255, 255);
  if (abs(s) < 60) s = 0;                         // dead zone: motor cannot turn below this
  digitalWrite(7, s > 0); digitalWrite(6, s < 0);
  digitalWrite(4, s > 0); digitalWrite(2, s < 0);
  analogWrite(5, abs(s)); analogWrite(3, abs(s));
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);
  int outs[] = {7, 6, 4, 2, 5, 3};
  for (int p : outs) pinMode(p, OUTPUT);
  dist = lastDist = readCm();
}

void loop() {
  if (millis() - lastT < DT * 1000) return;       // fixed period, no delay()
  lastT = millis();
  dist = 0.7 * dist + 0.3 * readCm();             // simple low-pass filter
  float error = dist - TARGET;                    // + : too far -> go forward
  integ = constrain(integ + error * DT, -100, 100);         // anti-windup
  float deriv = (dist - lastDist) / DT;           // rate of change (derivative of measurement)
  lastDist = dist;
  float u = Kp * error + Ki * integ + Kd * deriv;
  if (dist > 150) u = 0, integ = 0;               // nothing in front: stop
  drive((int)u);
  Serial.print("target:"); Serial.print(TARGET);  // Serial Plotter format
  Serial.print(" dist:");  Serial.print(dist);
  Serial.print(" cmd:");   Serial.println(u / 10);
}
