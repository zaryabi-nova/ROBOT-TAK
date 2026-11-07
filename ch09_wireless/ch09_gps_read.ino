// Rabat Tak - Vol.2 section 9-11: read position from a GPS module (NEO-M8N / M10) with ESP32
// GPS: TX->GPIO16 (RX2), RX->GPIO17 (TX2), VCC 3.3V or 5V (see module), GND
// Library: TinyGPSPlus. Test outdoors; the first fix can take 1-2 minutes.
#include <TinyGPSPlus.h>
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);   // M10 modules may use 38400
}

void loop() {
  while (Serial2.available()) gps.encode(Serial2.read());
  static uint32_t t = 0;
  if (millis() - t < 1000) return;
  t = millis();
  if (gps.location.isValid()) {
    Serial.printf("lat %.6f  lon %.6f  sats %d  speed %.1f km/h  hdop %.1f\n",
                  gps.location.lat(), gps.location.lng(), gps.satellites.value(),
                  gps.speed.kmph(), gps.hdop.hdop());
  } else {
    Serial.printf("waiting for fix... sats %d, chars %lu\n",
                  gps.satellites.value(), gps.charsProcessed());
  }
}
