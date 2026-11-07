// Read the UID of an RFID card with RC522 (ESP32: SCK18 MISO19 MOSI23 SS5 RST4)
#include <SPI.h>
#include <MFRC522.h>

MFRC522 rfid(5, 4);                                 // SS, RST

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("Show a card...");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;
  Serial.print("UID:");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();
  rfid.PICC_HaltA();                                // wait for the next card
}
