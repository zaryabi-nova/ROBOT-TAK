// Smallest web control: ESP32 makes its own Wi-Fi and a page with two buttons (LED on/off)
#include <WiFi.h>
#include <WebServer.h>

const int LED = 2;
WebServer server(80);

const char PAGE[] PROGMEM = R"HTML(<!DOCTYPE html><html><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1"></head><body style="text-align:center">
<h2>ESP32</h2>
<button onclick="fetch('/on')" style="font-size:28px">ON</button>
<button onclick="fetch('/off')" style="font-size:28px">OFF</button>
</body></html>)HTML";

void setup() {
  pinMode(LED, OUTPUT);
  WiFi.softAP("RoboTak", "robot1234");             // Access Point: phone connects to the ESP32
  server.on("/", []() { server.send_P(200, "text/html", PAGE); });   // the page
  server.on("/on", []() { digitalWrite(LED, HIGH); server.send(200, "text/plain", "ON"); });
  server.on("/off", []() { digitalWrite(LED, LOW); server.send(200, "text/plain", "OFF"); });
  server.begin();
}

void loop() {
  server.handleClient();                            // answer the browser's requests
}
