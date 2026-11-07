const int PIN_A = 2, PIN_B = 3;
volatile long count = 0;

void isrA() {                      // on CHANGE of A
  if (digitalRead(PIN_A) == digitalRead(PIN_B)) count++;
  else count--;
}

void setup() {
  pinMode(PIN_A, INPUT_PULLUP); pinMode(PIN_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_A), isrA, CHANGE);
}

long readCount() {
  noInterrupts(); long c = count; interrupts();   // atomic copy
  return c;
}
