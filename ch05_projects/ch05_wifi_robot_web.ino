#include <WiFi.h>
#include <WebServer.h>

const char* AP_SSID = "ESP32_Car";
const char* AP_PASS = "change-me-123";     // at least 8 characters: change it!

const int IN1 = 14, IN2 = 27;   // left motor  (L298N OUT1/OUT2)
const int IN3 = 26, IN4 = 25;   // right motor (L298N OUT3/OUT4)
const int ENA = 33, ENB = 32;   // speed (PWM); remove the ENA/ENB jumpers
const int TRIG = 4, ECHO = 5;   // HC-SR04P (3.3 V version)
const int SAFE_CM = 20;

WebServer server(80);
int speedPWM = 200;             // 0..255, set by the slider
unsigned long lastCmd = 0;      // failsafe: stop if commands stop coming
bool autoMode = false;

enum State { FWD, BACK, TURN };
State st = FWD;
unsigned long stT = 0, lastPing = 0;
bool turnRight = true;

void motors(int left, int right) {          // each: 1 forward, 0 stop, -1 back
  digitalWrite(IN1, left > 0);  digitalWrite(IN2, left < 0);
  digitalWrite(IN3, right > 0); digitalWrite(IN4, right < 0);
  ledcWrite(ENA, left  ? speedPWM : 0);
  ledcWrite(ENB, right ? speedPWM : 0);
}

float readCm() {
  digitalWrite(TRIG, LOW);  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long t = pulseIn(ECHO, HIGH, 30000);
  return (t == 0) ? 400 : t / 58.0;
}

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html><head><meta name="viewport" content="width=device-width,initial-scale=1">
<style>body{font-family:sans-serif;text-align:center;background:#222;color:#eee}
button{width:28%;height:70px;font-size:22px;margin:4px;border-radius:12px}</style>
</head><body><h2>ESP32 Car</h2>
<button data-c="F">&#9650;</button><br>
<button data-c="L">&#9664;</button><button onclick="go('S')">STOP</button><button data-c="R">&#9654;</button><br>
<button data-c="B">&#9660;</button>
<p>Speed: <span id="v">200</span></p>
<input type="range" min="100" max="255" value="200" style="width:80%"
 oninput="v.innerText=this.value" onchange="fetch('/speed?v='+this.value)">
<p><button onclick="fetch('/auto?on=1')">Auto ON</button>
<button onclick="fetch('/auto?on=0')">Auto OFF</button></p>
<script>
let t; function go(c){ fetch('/cmd?c='+c); }
document.querySelectorAll('button[data-c]').forEach(b => {
  b.onpointerdown = () => { go(b.dataset.c); clearInterval(t);
                            t = setInterval(() => go(b.dataset.c), 200); };
  b.onpointerup = b.onpointerleave = () => { clearInterval(t); go('S'); };
});
</script></body></html>)rawliteral";

void handleCmd() {
  if (!autoMode) {
    String c = server.arg("c");
    if      (c == "F") motors(1, 1);
    else if (c == "B") motors(-1, -1);
    else if (c == "L") motors(-1, 1);
    else if (c == "R") motors(1, -1);
    else               motors(0, 0);
    lastCmd = millis();
  }
  server.send(200, "text/plain", "ok");
}

void autoStep(unsigned long now) {          // obstacle avoidance without delay
  switch (st) {
    case FWD:
      if (now - lastPing > 60) {
        lastPing = now;
        if (readCm() < SAFE_CM) { motors(-1, -1); st = BACK; stT = now; }
        else motors(1, 1);
      }
      break;
    case BACK:
      if (now - stT > 600) {
        if (turnRight) motors(1, -1); else motors(-1, 1);
        st = TURN; stT = now;
      }
      break;
    case TURN:
      if (now - stT > 700) { turnRight = !turnRight; motors(0, 0); st = FWD; }
      break;
  }
}

void setup() {
  int outs[] = {IN1, IN2, IN3, IN4, TRIG};
  for (int p : outs) pinMode(p, OUTPUT);
  pinMode(ECHO, INPUT);
  ledcAttach(ENA, 20000, 8);                 // 20 kHz, 8 bit (ESP32 core 3)
  ledcAttach(ENB, 20000, 8);
  motors(0, 0);
  Serial.begin(115200);

  WiFi.softAP(AP_SSID, AP_PASS);
  Serial.println(WiFi.softAPIP());           // 192.168.4.1
  server.on("/", []() { server.send(200, "text/html", PAGE); });
  server.on("/cmd", handleCmd);
  server.on("/speed", []() {
    speedPWM = constrain(server.arg("v").toInt(), 0, 255);
    server.send(200, "text/plain", "ok");
  });
  server.on("/auto", []() {
    autoMode = server.arg("on") == "1";
    motors(0, 0); st = FWD; lastCmd = millis();
    server.send(200, "text/plain", "ok");
  });
  server.begin();
}

void loop() {
  server.handleClient();
  unsigned long now = millis();
  if (autoMode) autoStep(now);
  else if (now - lastCmd > 500) motors(0, 0);   // failsafe
  delay(2);
}
