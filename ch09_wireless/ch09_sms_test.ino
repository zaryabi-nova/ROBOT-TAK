// Send one SMS with an A7670E module on ESP32 UART2
// Wiring (as in the figure): module T -> GPIO5 (ESP32 RX), module R -> GPIO15 (ESP32 TX),
// common GND, module powered from its own adapter; LED on GPIO18 shows the result.
HardwareSerial modem(2);
const int MODEM_RX = 5, MODEM_TX = 15, LED = 18;

bool at(const char *cmd, const char *expect, int waitMs) {  // send, print the answer, look for expect
  if (cmd[0]) modem.println(cmd);                 // empty command: only wait and print
  String ans;
  unsigned long t0 = millis();
  while (millis() - t0 < (unsigned long)waitMs)
    while (modem.available()) { char c = modem.read(); Serial.write(c); ans += c; }
  return ans.indexOf(expect) >= 0;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  modem.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);                                      // let the module register on the network
  at("AT", "OK", 1000);
  at("AT+CSQ", "OK", 1000);                         // signal quality (10..31 is good)
  at("AT+CMGF=1", "OK", 1000);                      // text mode
  modem.println("AT+CMGS=\"+98912xxxxxxx\"");       // receiver number
  delay(500);
  modem.print("Hello from RoboTak!");
  modem.write(26);                                  // Ctrl+Z = send
  bool sent = at("", "+CMGS", 10000);               // only wait; expect: +CMGS: ... OK
  digitalWrite(LED, sent ? HIGH : LOW);             // LED on = SMS sent
}

void loop() {}
