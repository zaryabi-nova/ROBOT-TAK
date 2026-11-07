// Rabat Tak - Vol.2 section 9-7: low-power FM transmitter with KT0803L and Arduino
// KT0803L: SDA->A4, SCL->A5, VCC 3.3V, GND; audio into the LINE/jack input
// Set FREQ to an empty frequency; listen on any FM radio
#include <Wire.h>
const uint8_t KT = 0x3E;            // I2C address
const float FREQ = 107.5;           // MHz

uint8_t rd(uint8_t r) { Wire.beginTransmission(KT); Wire.write(r); Wire.endTransmission(false);
                        Wire.requestFrom(KT, (uint8_t)1); return Wire.read(); }
void wr(uint8_t r, uint8_t v) { Wire.beginTransmission(KT); Wire.write(r); Wire.write(v); Wire.endTransmission(); }

void setFreq(float mhz) {
  uint16_t ch = (uint16_t)(mhz * 20 + 0.5);          // 50 kHz steps
  wr(0x00, (ch >> 1) & 0xFF);                        // CHSEL[8:1]
  wr(0x01, (rd(0x01) & 0xF8) | ((ch >> 9) & 0x07));  // CHSEL[11:9]
  wr(0x02, (rd(0x02) & 0x7F) | ((ch & 1) << 7));     // CHSEL[0]
}

void setup() {
  Serial.begin(9600); Wire.begin();
  Wire.beginTransmission(KT);
  if (Wire.endTransmission() != 0) { Serial.println("KT0803L not found"); while (1); }
  setFreq(FREQ);
  Serial.print("Transmitting on "); Serial.print(FREQ); Serial.println(" MHz");
}

void loop() {}
