unsigned long tLed = 0, tCtrl = 0;

void loop() {
  unsigned long now = millis();
  if (now - tLed >= 500) {          // every 500 ms
    tLed = now;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  if (now - tCtrl >= 10) {          // control loop at 100 Hz
    tCtrl = now;
    // read sensors, compute PID, set motor PWM
  }
}
