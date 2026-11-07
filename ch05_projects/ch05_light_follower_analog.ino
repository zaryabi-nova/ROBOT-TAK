// analog version: LDR to 5V, 10 kΩ resistor to GND, middle point to A0 / A1
const int BASE = 150;     // forward speed
const float K = 0.4;      // steering gain: tune by testing
const int DARK = 300;     // below this total, there is no useful light

void loop() {
  int l = analogRead(A0), r = analogRead(A1);   // more light = larger value
  if (l + r < DARK) { drive(0, 0); return; }
  int diff = r - l;                             // > 0: light is on the right
  drive(constrain(BASE + K * diff, 0, 200), constrain(BASE - K * diff, 0, 200));
  delay(20);
}
