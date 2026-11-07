#include <Servo.h>

const int F_LEFT = 2, F_CENTER = 3, F_RIGHT = 4;   // flame DO: LOW = flame
const int F_NEAR = A0;                  // AO of the center sensor
const int ENA = 5, IN1 = 7, IN2 = 8;    // left motor  (L298N)
const int ENB = 6, IN3 = 9, IN4 = 10;   // right motor (L298N)
const int SERVO_PIN = 11;               // nozzle servo
const int RELAY = 12;                   // relay -> water pump
const bool RELAY_ACTIVE_LOW = true;
const int LED_PIN = A1, BUZZER = A2;    // analog pins used as digital outputs

const int NEAR = 300;     // AO below this = close enough to spray (calibrate!)
const int SPEED = 120, TURN = 110;
const unsigned long SPRAY_AFTER_MS = 1500;  // keep spraying after the flame is gone
const unsigned long MAX_SPRAY_MS  = 10000;  // safety: tank is small

Servo nozzle;
unsigned long lastNear = 0, sprayStart = 0;
bool spraying = false, gaveUp = false;

void motor(int en, int a, int b, int speed) {   // speed: -255..255
  digitalWrite(a, speed > 0);
  digitalWrite(b, speed < 0);
  analogWrite(en, abs(speed));
}
void drive(int l, int r) { motor(ENA, IN1, IN2, l); motor(ENB, IN3, IN4, r); }
void pump(bool on) { digitalWrite(RELAY, (on != RELAY_ACTIVE_LOW) ? HIGH : LOW); }

void setup() {
  pump(false);                                  // relay off before enabling the pin
  int outs[] = {ENA, IN1, IN2, ENB, IN3, IN4, RELAY, LED_PIN, BUZZER};
  for (int p : outs) pinMode(p, OUTPUT);
  pinMode(F_LEFT, INPUT); pinMode(F_CENTER, INPUT); pinMode(F_RIGHT, INPUT);
  nozzle.attach(SERVO_PIN);
  nozzle.write(90);
}

void loop() {
  unsigned long now = millis();
  bool L = digitalRead(F_LEFT) == LOW;
  bool C = digitalRead(F_CENTER) == LOW;
  bool R = digitalRead(F_RIGHT) == LOW;
  bool fire = L || C || R;
  if (C && analogRead(F_NEAR) < NEAR) lastNear = now;

  bool wantSpray = lastNear != 0 && now - lastNear < SPRAY_AFTER_MS;
  if (spraying && !wantSpray) spraying = false;
  if (spraying && now - sprayStart > MAX_SPRAY_MS) { spraying = false; gaveUp = true; }
  if (!fire && !wantSpray) gaveUp = false;
  if (wantSpray && !spraying && !gaveUp) { spraying = true; sprayStart = now; }
  pump(spraying);

  if (spraying) {                               // sweep the nozzle 50..130 degrees
    int t = (now / 15) % 160;
    nozzle.write(50 + (t < 80 ? t : 160 - t));
  } else {
    nozzle.write(90);
  }

  digitalWrite(LED_PIN, fire);
  digitalWrite(BUZZER, fire && (now / 125) % 2);

  if (spraying || gaveUp || !fire) drive(0, 0); // spraying, out of water, or no fire
  else if (C)  drive(SPEED, SPEED);             // flame ahead: approach slowly
  else if (L)  drive(-TURN, TURN);              // flame on the left: turn left
  else         drive(TURN, -TURN);              // flame on the right: turn right
  delay(20);
}
