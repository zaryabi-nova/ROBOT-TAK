const int S2 = 3, S3 = 4, OUT = 5;    // TCS3200 (S0 -> VCC, S1 and OE -> GND)
const int LED_UV = 6;                     // 3 mm UV (violet) LED, PWM
const int LED_R = 9, LED_G = 10, LED_B = 11;  // PWM: 3 LEDs or 1 RGB LED

const bool RGB_MODE = false;      // false: part 1 (LEDs)  true: part 2 (RGB LED)
const bool COMMON_ANODE = false;  // true if the RGB LED's long leg goes to 5V

long white[3], black[3];          // calibration: pulse length per channel (µs)

long readPulse(int ch) {          // ch: 0 = red, 1 = green, 2 = blue
  const int s2[3] = {LOW, HIGH, LOW}, s3[3] = {LOW, HIGH, HIGH};
  digitalWrite(S2, s2[ch]);
  digitalWrite(S3, s3[ch]);
  delay(5);                       // let the filter switch settle
  long sum = 0;
  for (int i = 0; i < 4; i++) {
    long p = pulseIn(OUT, LOW, 50000);
    sum += (p == 0) ? 50000 : p;  // no pulse = very dark
  }
  return sum / 4;                 // short pulse = more light
}

int level(int ch) {               // 0..255 after calibration
  long p = readPulse(ch);
  return constrain(map(p, black[ch], white[ch], 0, 255), 0, 255);
}

void calibrate(long *ref, const char *msg) {
  Serial.println(msg);
  for (int i = 0; i < 8; i++) { digitalWrite(LED_BUILTIN, i % 2); delay(400); }
  for (int ch = 0; ch < 3; ch++) ref[ch] = readPulse(ch);
}

void setLeds(int r, int g, int b, int uv) {
  if (COMMON_ANODE) { r = 255 - r; g = 255 - g; b = 255 - b; }
  analogWrite(LED_R, r); analogWrite(LED_G, g); analogWrite(LED_B, b);
  analogWrite(LED_UV, uv);        // UV LED is a separate single LED
}

void setup() {
  int outs[] = {S2, S3, LED_UV, LED_R, LED_G, LED_B, LED_BUILTIN};
  for (int p : outs) pinMode(p, OUTPUT);
  pinMode(OUT, INPUT);
  setLeds(0, 0, 0, 0);
  Serial.begin(9600);
  calibrate(white, "Put the sensor on WHITE paper...");
  calibrate(black, "Now on BLACK paper...");
  for (int ch = 0; ch < 3; ch++)                   // avoid map() divide by zero
    if (black[ch] <= white[ch]) black[ch] = white[ch] + 1;
  Serial.println("Ready!");
}

void loop() {
  int r = level(0), g = level(1), b = level(2);
  int hi = max(r, max(g, b)), lo = min(r, min(g, b));
  bool violet = (r - g > 50) && (b - g > 50);      // red + blue, little green

  if (!RGB_MODE) {                       // ---- part 1: one LED per colour
    if (hi < 60)            setLeds(0, 0, 0, 0);          // black or nothing
    else if (hi - lo < 40)  setLeds(255, 255, 255, 0);    // white or grey
    else if (violet)        setLeds(0, 0, 0, 255);        // violet / purple
    else if (hi == r)       setLeds(255, 0, 0, 0);        // red
    else if (hi == g)       setLeds(0, 255, 0, 0);        // green
    else                    setLeds(0, 0, 255, 0);        // blue
  } else {                               // ---- part 2: copy the colour
    if (hi < 40) { setLeds(0, 0, 0, 0); }
    else {
      int uv = violet ? constrain(min(r, b) - g, 0, 255) : 0;
      setLeds(r * 255L / hi, g * 255L / hi, b * 255L / hi, uv * 255L / hi);
    }
  }

  Serial.print("R="); Serial.print(r); Serial.print(" G="); Serial.print(g);
  Serial.print(" B="); Serial.println(b);
  delay(100);
}
