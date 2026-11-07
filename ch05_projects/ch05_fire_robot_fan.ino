const int FLAME_DO = 2;           // digital: LOW = flame seen
const int FLAME_AO = A0;          // analog: smaller value = stronger flame
const int FAN_A = 5, FAN_B = 6;   // L9110 fan module: PWM on A, B stays LOW
const int LED_PIN = 8;            // red LED (with 220 ohm)
const int BUZZER  = 9;            // active buzzer

const int FAN_SPEED = 255;
const unsigned long FAN_AFTER_MS = 3000;   // keep blowing 3 s after the flame is gone
const unsigned long MAX_FAN_MS   = 20000;  // safety: stop the fan after 20 s

unsigned long lastFlame = 0, fanStart = 0, lastPrint = 0;
bool fanOn = false, gaveUp = false;

void setup() {
  pinMode(FLAME_DO, INPUT);
  pinMode(FAN_A, OUTPUT); pinMode(FAN_B, OUTPUT);
  pinMode(LED_PIN, OUTPUT); pinMode(BUZZER, OUTPUT);
  digitalWrite(FAN_B, LOW);
  Serial.begin(9600);
}

void loop() {
  unsigned long now = millis();
  bool flame = (digitalRead(FLAME_DO) == LOW);
  if (flame) lastFlame = now;

  // fan logic: start on flame, run on a little, never longer than MAX_FAN_MS
  if (fanOn && now - lastFlame > FAN_AFTER_MS) fanOn = false;
  if (fanOn && now - fanStart > MAX_FAN_MS) { fanOn = false; gaveUp = true; }
  if (!flame && now - lastFlame > FAN_AFTER_MS) gaveUp = false;
  if (flame && !fanOn && !gaveUp) { fanOn = true; fanStart = now; }
  analogWrite(FAN_A, fanOn ? FAN_SPEED : 0);

  // alarm: LED on, buzzer beeps 4 times a second while a flame is seen
  digitalWrite(LED_PIN, flame);
  digitalWrite(BUZZER, flame && (now / 125) % 2);

  if (now - lastPrint > 200) {         // flame strength for calibration
    lastPrint = now;
    Serial.print("AO="); Serial.print(analogRead(FLAME_AO));
    Serial.print(" flame="); Serial.print(flame);
    Serial.print(" fan="); Serial.println(fanOn);
  }
}
