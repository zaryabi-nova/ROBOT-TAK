// Rabat Tak - Vol.2 section 8-2: on-off control with hysteresis (automatic watering)
// Soil moisture sensor (analog) -> A0, relay module IN -> D7, LED on D13 shows pump state
// Pump ON when soil is drier than 30 %, OFF when wetter than 45 %
const int SENSOR = A0, RELAY = 7;
const int DRY = 820, WET = 400;          // raw readings in air (dry) and in water (wet): measure yours!
const int ON_BELOW = 30, OFF_ABOVE = 45; // percent, the gap is the hysteresis
bool pump = false;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY, OUTPUT); pinMode(13, OUTPUT);
}

void loop() {
  int raw = analogRead(SENSOR);
  int moist = constrain(map(raw, DRY, WET, 0, 100), 0, 100);
  if (!pump && moist < ON_BELOW)  pump = true;    // too dry  -> switch ON
  if (pump  && moist > OFF_ABOVE) pump = false;   // wet enough -> switch OFF
  digitalWrite(RELAY, pump ? HIGH : LOW);         // some relay modules are active LOW
  digitalWrite(13, pump);
  Serial.print("moisture "); Serial.print(moist);
  Serial.println(pump ? " %  pump ON" : " %  pump OFF");
  delay(1000);                                    // slow process: once per second is enough
}
