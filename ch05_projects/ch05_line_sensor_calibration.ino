const int S[3] = {A0, A1, A2};          // AO pins: left, center, right
int lo[3] = {1023, 1023, 1023}, hi[3] = {0, 0, 0}, th[3];

void calibrate() {   // in setup(): move the robot across the line for 5 s
  digitalWrite(LED_BUILTIN, HIGH);
  unsigned long t0 = millis();
  while (millis() - t0 < 5000) {
    for (int i = 0; i < 3; i++) {
      int v = analogRead(S[i]);
      lo[i] = min(lo[i], v);
      hi[i] = max(hi[i], v);
    }
  }
  for (int i = 0; i < 3; i++) th[i] = (lo[i] + hi[i]) / 2;   // midpoint
  digitalWrite(LED_BUILTIN, LOW);
}

bool onLine(int i) {          // larger value = darker on most modules
  return analogRead(S[i]) > th[i];
}
// in loop(): bool L = onLine(0), C = onLine(1), R = onLine(2);
