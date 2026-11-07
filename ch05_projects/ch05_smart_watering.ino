const int SENSOR_PIN = A0;    // sensor analog output
const int SENSOR_PWR = 7;     // powers the sensor only while reading
const int RELAY_PIN  = 8;     // relay IN
const bool RELAY_ACTIVE_LOW = true;   // most 1-channel modules

const int RAW_DRY = 850;      // reading in dry air       (calibrate!)
const int RAW_WET = 400;      // reading in a cup of water (calibrate!)
const int START_BELOW = 35;   // water when moisture < 35 %

const unsigned long CHECK_MS = 600000UL;  // normal check: 10 min
const unsigned long SOAK_MS  = 60000UL;   // let water spread: 1 min
const unsigned long PUMP_MS  = 3000UL;    // one watering pulse: 3 s
const int MAX_PULSES = 5;     // still dry after this -> tank empty?

unsigned long tLast = 0;
int pulses = 0;
bool soaking = false, tankAlarm = false;

void pump(bool on) {          // active-low relay: LOW = on
  digitalWrite(RELAY_PIN, (on != RELAY_ACTIVE_LOW) ? HIGH : LOW);
}

int readMoisture() {          // returns 0..100 %
  digitalWrite(SENSOR_PWR, HIGH);
  delay(100);                 // let the sensor settle
  long sum = 0;
  for (int i = 0; i < 10; i++) { sum += analogRead(SENSOR_PIN); delay(5); }
  digitalWrite(SENSOR_PWR, LOW);
  int raw = sum / 10;
  Serial.print("raw="); Serial.print(raw);
  return constrain(map(raw, RAW_DRY, RAW_WET, 0, 100), 0, 100);
}

void setup() {
  pump(false);                // set the "off" level before enabling output
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(SENSOR_PWR, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  tLast = millis() - CHECK_MS;          // first check right after start
}

void loop() {
  unsigned long now = millis();
  if (tankAlarm) {                      // stop watering, blink the LED
    digitalWrite(LED_BUILTIN, (now / 500) % 2);
    return;
  }
  if (now - tLast < (soaking ? SOAK_MS : CHECK_MS)) return;
  tLast = now;

  int m = readMoisture();
  Serial.print("  moisture="); Serial.print(m); Serial.println(" %");

  if (m >= START_BELOW) {               // wet enough
    soaking = false; pulses = 0;
  } else if (pulses < MAX_PULSES) {     // dry: short pulse, then wait
    pump(true); delay(PUMP_MS); pump(false);
    pulses++; soaking = true;
  } else {                              // still dry after MAX_PULSES
    tankAlarm = true;
    Serial.println("Still dry: check the water tank and tube!");
  }
}
