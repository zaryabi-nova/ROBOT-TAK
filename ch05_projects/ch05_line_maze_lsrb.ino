// Line maze robot (left-hand rule) with three IR sensors
// Same wiring as the 3-sensor line follower: L298N on D5..D10, sensors on D2, D3, D4
const int ENA = 5, IN1 = 7, IN2 = 8, ENB = 6, IN3 = 9, IN4 = 10;
const int S_LEFT = 2, S_CENTER = 3, S_RIGHT = 4;
const int LINE = HIGH;       // output over the black line: check your module!

const int BASE = 130;        // forward speed
const int SPIN = 140;        // speed when turning in place
const int NUDGE_MS = 120;    // drive forward so the wheels reach the junction

void motor(int en, int a, int b, int speed) {
  digitalWrite(a, speed > 0);
  digitalWrite(b, speed < 0);
  analogWrite(en, abs(speed));
}
void drive(int left, int right) { motor(ENA, IN1, IN2, left); motor(ENB, IN3, IN4, right); }

bool onLine(int pin) { return digitalRead(pin) == LINE; }

void spinToLine(int dir) {            // dir: -1 left, +1 right
  drive(dir * SPIN, -dir * SPIN);
  delay(200);                         // first leave the current line
  unsigned long t0 = millis();
  while (!onLine(S_CENTER) && millis() - t0 < 3000) {}   // then find the new one
  drive(0, 0); delay(50);
}

void setup() {
  int outs[] = {ENA, IN1, IN2, ENB, IN3, IN4};
  for (int p : outs) pinMode(p, OUTPUT);
  pinMode(S_LEFT, INPUT); pinMode(S_CENTER, INPUT); pinMode(S_RIGHT, INPUT);
}

void loop() {
  bool L = onLine(S_LEFT), C = onLine(S_CENTER), R = onLine(S_RIGHT);

  if (L || (R && !C) || (!L && !C && !R)) {   // a decision point
    drive(BASE, BASE); delay(NUDGE_MS);       // wheels onto the junction
    bool L2 = onLine(S_LEFT), C2 = onLine(S_CENTER), R2 = onLine(S_RIGHT);
    if (L2 && C2 && R2) {                     // big black square: finish
      drive(0, 0);
      while (true) {}
    }
    if (L)              spinToLine(-1);       // 1) left first
    else if (C2)        drive(BASE, BASE);    // 2) straight if the line goes on
    else if (R)         spinToLine(+1);       // 3) right
    else                spinToLine(-1);       // 4) dead end: U-turn
  } else {
    drive(BASE, BASE);                        // only center (or center + right): straight
  }
}
