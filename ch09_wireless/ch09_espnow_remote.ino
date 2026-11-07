// Rabat Tak - Vol.2 section 9-9-1: ESP-NOW remote (sender) - ESP32 + joystick
// Joystick: VRx -> GPIO34, VRy -> GPIO35, SW -> GPIO32, +5V -> 3V3, GND
// Arduino-ESP32 core 3.x. Put the robot board's MAC address below.
#include <WiFi.h>
#include <esp_now.h>

uint8_t robotMac[] = {0x24, 0x6F, 0x28, 0x00, 0x00, 0x00};   // change!
typedef struct { int16_t x, y; uint8_t btn; } Command;            // same on both boards
Command cmd;

void setup() {
  Serial.begin(115200);
  pinMode(32, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) { Serial.println("ESP-NOW init failed"); while (1); }
  esp_now_peer_info_t peer = {};                                   // zero all fields
  memcpy(peer.peer_addr, robotMac, 6);
  peer.channel = 0; peer.encrypt = false;
  esp_now_add_peer(&peer);
  Serial.print("Remote MAC: "); Serial.println(WiFi.macAddress());
}

void loop() {
  cmd.x = map(analogRead(34), 0, 4095, -100, 100);
  cmd.y = map(analogRead(35), 0, 4095, -100, 100);
  cmd.btn = digitalRead(32) == LOW;
  esp_err_t r = esp_now_send(robotMac, (uint8_t *)&cmd, sizeof(cmd));
  if (r != ESP_OK) Serial.println("send error");
  delay(50);                                                      // 20 packets/s
}
