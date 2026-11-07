// Rabat Tak - Vol.2 section 9-5: read a FlySky receiver (iBUS) with ESP32, no library
// FS-iA6B: iBUS SERVO signal -> GPIO16 (RX2), + -> 5V, - -> GND
// iBUS frame: 32 bytes, 0x20 0x40, 14 channels (2 bytes each, little endian), checksum
uint8_t buf[32]; int idx = 0;
uint16_t ch[14]; uint32_t lastFrame = 0;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
}

void readIbus() {
  while (Serial2.available()) {
    uint8_t b = Serial2.read();
    if (idx == 0 && b != 0x20) continue;
    if (idx == 1 && b != 0x40) { idx = 0; continue; }
    buf[idx++] = b;
    if (idx < 32) continue;
    idx = 0;
    uint16_t sum = 0xFFFF;
    for (int i = 0; i < 30; i++) sum -= buf[i];
    if (sum != (buf[30] | buf[31] << 8)) continue;          // bad frame
    for (int i = 0; i < 14; i++) ch[i] = buf[2 + i * 2] | (buf[3 + i * 2] << 8);
    lastFrame = millis();
  }
}

void loop() {
  readIbus();
  static uint32_t t = 0;
  if (millis() - t < 100) return;
  t = millis();
  if (millis() - lastFrame > 300) { Serial.println("no signal - motors off"); return; }
  int throttle = map(ch[2], 1000, 2000, 0, 255);
  int steer    = map(ch[0], 1000, 2000, -100, 100);
  Serial.printf("CH1..6: %d %d %d %d %d %d | thr %d steer %d\n",
                ch[0], ch[1], ch[2], ch[3], ch[4], ch[5], throttle, steer);
}
