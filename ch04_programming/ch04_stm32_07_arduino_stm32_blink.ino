// Vol.1 section 4-11 - Example 7: STM32 with the Arduino IDE (STM32duino core)
// Boards Manager URL:
//   https://github.com/stm32duino/BoardManagerFiles/raw/main/package_stmicroelectronics_index.json
// Tools > Board: "Nucleo-64", Board part number: "Nucleo F446RE",
// Upload method: "STM32CubeProgrammer (SWD)" (install STM32CubeProgrammer).

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);         // LD2 = PA5
  pinMode(USER_BTN, INPUT);             // B1 = PC13
  Serial.begin(115200);                 // ST-LINK virtual COM port
}

void loop() {
  bool pressed = digitalRead(USER_BTN) == LOW;
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  Serial.println(pressed ? "button pressed" : "running");
  delay(pressed ? 100 : 500);
}
