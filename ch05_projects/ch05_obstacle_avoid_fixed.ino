const int IN1 = 7, IN2 = 6;   // left motor  (L298N OUT1/OUT2)
const int IN3 = 5, IN4 = 4;   // right motor (L298N OUT3/OUT4)
const int TRIG = 10, ECHO = 9;          // as in the wiring figure
const int SAFE_CM = 25;                 // obstacle closer than this: avoid

void motors(int left, int right) {      // each: 1 forward, 0 stop, -1 back
  digitalWrite(IN1, left > 0);  digitalWrite(IN2, left < 0);
  digitalWrite(IN3, right > 0); digitalWrite(IN4, right < 0);
}

float readCm() {
  digitalWrite(TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);                    // end the trigger pulse!
  unsigned long t = pulseIn(ECHO, HIGH, 25000); // timeout 25 ms (about 4 m)
  if (t == 0) return 400;                     // no echo: nothing in front
  return t / 58.0;                            // microseconds -> cm
}

float distanceCm() {                          // median of 3 removes odd readings
  float a = readCm(); delay(10);
  float b = readCm(); delay(10);
  float c = readCm();
  return max(min(a, b), min(max(a, b), c));
}

void setup() {
  int outs[] = {IN1, IN2, IN3, IN4, TRIG};
  for (int p : outs) pinMode(p, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(A0));                 // different turns each run
}

void loop() {
  float d = distanceCm();
  Serial.println(d);
  if (d > SAFE_CM) {
    motors(1, 1);                             // path is clear: go
  } else {
    motors(0, 0);  delay(200);                // stop
    motors(-1, -1); delay(400);               // back up
    motors(0, 0);  delay(100);
    if (random(2)) motors(1, -1);             // spin right ...
    else           motors(-1, 1);             // ... or left
    delay(random(300, 700));
    motors(0, 0);
  }
  delay(30);
}
