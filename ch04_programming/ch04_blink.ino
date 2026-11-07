void setup() {
  pinMode(LED_BUILTIN, OUTPUT);   // run once
  Serial.begin(115200);
}

void loop() {                     // runs forever
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
