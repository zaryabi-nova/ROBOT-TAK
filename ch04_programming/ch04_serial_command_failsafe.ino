unsigned long lastCmd = 0;

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    int l, r;
    if (sscanf(line.c_str(), "M %d %d", &l, &r) == 2) {
      setMotors(l, r);
      lastCmd = millis();
    }
  }
  if (millis() - lastCmd > 500) setMotors(0, 0);   // failsafe
}
