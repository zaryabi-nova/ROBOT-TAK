// Rabat Tak - Vol.2 section 9-9-1: ESP-NOW robot (receiver) - ESP32 + L298N / TB6612
// Motor A: PWM GPIO25, IN1 26, IN2 27 | Motor B: PWM GPIO14, IN3 12, IN4 13
// Arduino-ESP32 core 3.x (ledcAttach). Prints its MAC at start: copy it into the remote.
#include <WiFi.h>
#include <esp_now.h>

typedef struct { int16_t x, y; uint8_t btn; } Command;
volatile Command cmd; volatile uint32_t lastRx = 0;

void onRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  if (len == sizeof(Command)) { memcpy((void *)&cmd, data, len); lastRx = millis(); }
}

void motor(int pwm, int in1, int in2, int s) {
  digitalWrite(in1, s > 0); digitalWrite(in2, s < 0);
  ledcWrite(pwm, constrain(abs(s), 0, 255));
}

void setup() {
  Serial.begin(115200);
  int outs[] = {26, 27, 12, 13};
  for (int p : outs) pinMode(p, OUTPUT);
  ledcAttach(25, 1000, 8); ledcAttach(14, 1000, 8);
  WiFi.mode(WIFI_STA);
  Serial.print("Robot MAC: "); Serial.println(WiFi.macAddress());
  if (esp_now_init() != ESP_OK) { Serial.println("ESP-NOW init failed"); while (1); }
  esp_now_register_recv_cb(onRecv);
}

void loop() {
  int fwd = 0, turn = 0;
  if (millis() - lastRx < 300) {                 // failsafe: stop if no packet for 300 ms
    fwd = cmd.y * 255 / 100; turn = cmd.x * 255 / 100;
    if (abs(fwd) < 25) fwd = 0;                  // dead zone
    if (abs(turn) < 25) turn = 0;
  }
  motor(25, 26, 27, fwd + turn);                 // left
  motor(14, 12, 13, fwd - turn);                 // right
  delay(10);
}
