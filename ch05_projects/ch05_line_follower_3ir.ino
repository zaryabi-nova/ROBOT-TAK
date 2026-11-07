const int ENA = 5, IN1 = 7, IN2 = 8;    // left motor  (L298N)
const int ENB = 6, IN3 = 9, IN4 = 10;   // right motor (L298N)
const int S_LEFT = 2, S_CENTER = 3, S_RIGHT = 4;
const int LINE = HIGH;   // output over the black line: check your module!

const int BASE = 150;    // straight-line speed
const int SOFT = 90;     // inner wheel on a soft turn
const int HARD = 0;      // inner wheel on a sharp turn
const int SEARCH = 120;  // spin speed when the line is lost

int lastSide = 1;        // -1: line was last seen on the left, +1: right

void motor(int en, int a, int b, int speed) {   // speed: -255..255
  digitalWrite(a, speed > 0);
  digitalWrite(b, speed < 0);
  analogWrite(en, abs(speed));
}

void drive(int left, int right) {
  motor(ENA, IN1, IN2, left);
  motor(ENB, IN3, IN4, right);
}

void setup() {
  int outs[] = {ENA, IN1, IN2, ENB, IN3, IN4};
  for (int p : outs) pinMode(p, OUTPUT);
  pinMode(S_LEFT, INPUT); pinMode(S_CENTER, INPUT); pinMode(S_RIGHT, INPUT);
  Serial.begin(9600);
}

void loop() {
  bool L = digitalRead(S_LEFT)   == LINE;
  bool C = digitalRead(S_CENTER) == LINE;
  bool R = digitalRead(S_RIGHT)  == LINE;

  if (L && C && R)        drive(0, 0);                          // junction / end
  else if (C && !L && !R) drive(BASE, BASE);                    // centered
  else if (L && C)        { drive(SOFT, BASE); lastSide = -1; } // soft left
  else if (R && C)        { drive(BASE, SOFT); lastSide = +1; } // soft right
  else if (L)             { drive(HARD, BASE); lastSide = -1; } // sharp left
  else if (R)             { drive(BASE, HARD); lastSide = +1; } // sharp right
  else if (lastSide < 0)  drive(-SEARCH, SEARCH);              // lost: search left
  else                    drive(SEARCH, -SEARCH);              // lost: search right

  Serial.print(L); Serial.print(' '); Serial.print(C);
  Serial.print(' '); Serial.println(R);       // e.g. "0 1 0"
}
