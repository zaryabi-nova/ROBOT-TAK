#include <WiFi.h>
#include <WebServer.h>

const char* WIFI_SSID = "YourWiFi";        // home network
const char* WIFI_PASS = "YourPassword";

const int SENSOR_PIN = 34;    // ADC1 pin (ADC2 does not work with Wi-Fi)
const int SENSOR_PWR = 25;
const int RELAY_PIN  = 26;
const bool RELAY_ACTIVE_LOW = true;
const int RAW_DRY = 3000, RAW_WET = 1400;  // 12-bit ADC: calibrate!
const int START_BELOW = 35;
const unsigned long CHECK_MS = 600000UL, SOAK_MS = 60000UL, PUMP_MS = 3000UL;
const int MAX_PULSES = 5;

WebServer server(80);
unsigned long tLast = 0;
int pulses = 0, moisture = -1;
bool soaking = false, tankAlarm = false;

void pump(bool on) { digitalWrite(RELAY_PIN, (on != RELAY_ACTIVE_LOW) ? HIGH : LOW); }

int readMoisture() {
  digitalWrite(SENSOR_PWR, HIGH); delay(100);
  long sum = 0;
  for (int i = 0; i < 10; i++) { sum += analogRead(SENSOR_PIN); delay(5); }
  digitalWrite(SENSOR_PWR, LOW);
  return constrain(map(sum / 10, RAW_DRY, RAW_WET, 0, 100), 0, 100);
}

void waterPulse() {
  pump(true); delay(PUMP_MS); pump(false);
  pulses++; soaking = true; tLast = millis();
}

void handleRoot() {
  String s = "<html><head><meta name='viewport' content='width=device-width'>";
  s += "<meta http-equiv='refresh' content='10'></head><body>";
  s += "<h2>My plant</h2><p>Moisture: " + String(moisture) + " %</p>";
  if (tankAlarm) s += "<p><b>Check the water tank!</b> <a href='/reset'>Reset</a></p>";
  else           s += "<p><a href='/water'>Water now (3 s)</a></p>";
  s += "</body></html>";
  server.send(200, "text/html", s);
}

void backHome() { server.sendHeader("Location", "/"); server.send(303); }

void setup() {
  pinMode(RELAY_PIN, OUTPUT); pump(false);   // relay off at once
  pinMode(SENSOR_PWR, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(300);
  Serial.println(WiFi.localIP());            // open this address on the phone
  server.on("/", handleRoot);
  server.on("/water", []() { if (!tankAlarm) waterPulse(); backHome(); });
  server.on("/reset", []() { tankAlarm = false; pulses = 0; soaking = false; backHome(); });
  server.begin();
  tLast = millis() - CHECK_MS;
}

void loop() {
  server.handleClient();
  unsigned long now = millis();
  if (tankAlarm || now - tLast < (soaking ? SOAK_MS : CHECK_MS)) return;
  tLast = now;
  moisture = readMoisture();
  if (moisture >= START_BELOW) { soaking = false; pulses = 0; }
  else if (pulses < MAX_PULSES) waterPulse();
  else tankAlarm = true;
}
