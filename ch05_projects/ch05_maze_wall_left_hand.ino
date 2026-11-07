// Maze robot: wall following (left-hand rule) + collision avoidance + finish line
// Arduino Uno + L298N + three HC-SR04 (left, front, right) + one IR sensor facing the floor
const int ENA = 5, IN1 = 7, IN2 = 8;     // left motor
const int ENB = 6, IN3 = 9, IN4 = 10;    // right motor
const int TRIG_L = A0, ECHO_L = A1;      // left sensor
const int TRIG_F = A2, ECHO_F = A3;      // front sensor
const int TRIG_R = A4, ECHO_R = A5;      // right sensor
const int FINISH_IR = 2;                 // IR sensor under the robot (finish line)
const int LINE = HIGH;                   // output over black tape: check your module!

const int BASE = 140;        // forward speed (PWM)
const int TURN = 150;        // speed when turning in place
const float KP = 6.0;        // correction per cm of error
const int MAX_FIX = 70;      // limit of the correction
const int WALL_TARGET = 8;   // cm from the left wall
const int DANGER_F = 6;      // cm: about to hit something in front
const int DANGER_S = 3;      // cm: touching a side wall
const int FRONT_STOP = 12;   // cm: wall ahead
const int SIDE_OPEN = 25;    // cm: opening on a side
const int TURN90_MS = 330;   // time for a 90-degree turn (tune it!)
const int STEP_MS = 250;     // drive into an opening before/after turning
const int BACK_MS = 200;     // back off after a near collision

long readCm(int trig, int echo) {
  digitalWrite(trig, LOW);  delayMicroseconds(2);
  digitalWrite(trig, HIGH); delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long us = pulseIn(echo, HIGH, 25000);  // timeout 25 ms (about 4 m)
  if (us == 0) return 400;               // no echo: very far
  return us / 58;                        // microseconds -> cm
}

void motor(int en, int a, int b, int speed) {   // speed: -255..255
  speed = constrain(speed, -255, 255);
  digitalWrite(a, speed > 0);
  digitalWrite(b, speed < 0);
  analogWrite(en, abs(speed));
}

void drive(int left, int right) {
  motor(ENA, IN1, IN2, left);
  motor(ENB, IN3, IN4, right);
}

void pauseMotors() { drive(0, 0); delay(120); }

void turn90(int dir) {               // dir: -1 left, +1 right
  pauseMotors();
  drive(dir * TURN, -dir * TURN);
  delay(TURN90_MS);
  pauseMotors();
}

bool atFinish() {                    // two readings: ignore a single dark spot
  if (digitalRead(FINISH_IR) != LINE) return false;
  delay(30);
  return digitalRead(FINISH_IR) == LINE;
}

void setup() {
  int outs[] = {ENA, IN1, IN2, ENB, IN3, IN4, TRIG_L, TRIG_F, TRIG_R};
  for (int p : outs) pinMode(p, OUTPUT);
  pinMode(ECHO_L, INPUT); pinMode(ECHO_F, INPUT); pinMode(ECHO_R, INPUT);
  pinMode(FINISH_IR, INPUT);
  Serial.begin(9600);
  delay(2000);                       // time to put the robot in the maze
}

void loop() {
  // 0) finish line under the robot: stop
  if (atFinish()) {
    drive(0, 0);
    Serial.println("Finished!");
    while (true) {}                  // stop until reset
  }

  long L = readCm(TRIG_L, ECHO_L); delay(15);   // one sensor at a time:
  long F = readCm(TRIG_F, ECHO_F); delay(15);   // no echo from the others
  long R = readCm(TRIG_R, ECHO_R); delay(15);

  if (F < DANGER_F || L < DANGER_S || R < DANGER_S) {
    // 1) collision avoidance: back off, then turn away from the nearer side
    pauseMotors();
    drive(-BASE, -BASE); delay(BACK_MS);
    if (F < DANGER_F) turn90(L > R ? -1 : +1);
    else {                           // touching a side wall: steer away
      if (L < R) drive(BASE, -BASE / 2); else drive(-BASE / 2, BASE);
      delay(120);
    }
    pauseMotors();
  } else if (L > SIDE_OPEN) {        // 2) opening on the left: take it
    drive(BASE, BASE); delay(STEP_MS);
    turn90(-1);
    drive(BASE, BASE); delay(STEP_MS);
  } else if (F > FRONT_STOP) {       // 3) go straight along the wall
    int err = WALL_TARGET - L;       // > 0: too close to the left wall
    if (R < SIDE_OPEN) err = (R - L) / 2;   // walls on both sides: stay centered
    int fix = constrain((int)(KP * err), -MAX_FIX, MAX_FIX);
    drive(BASE + fix, BASE - fix);   // fix > 0 steers to the right
  } else if (R > SIDE_OPEN) {        // 4) only the right is open
    turn90(+1);
  } else {                           // 5) dead end
    turn90(+1); turn90(+1);
  }

  Serial.print("L="); Serial.print(L); Serial.print(" F="); Serial.print(F);
  Serial.print(" R="); Serial.println(R);
}
