const int LDR_LEFT  = 8, LDR_RIGHT = 9;   // DO pins: LOW = light seen
const int MOTOR_L   = 10, MOTOR_R = 11;   // MX1508 IN1 and IN3 (PWM)
const int FAST = 200;   // limit for 6 V motors on a 7.4 V battery
const int SLOW = 0;     // inner wheel while turning (0 = turn on the spot)

void drive(int left, int right) {
  analogWrite(MOTOR_L, left);
  analogWrite(MOTOR_R, right);
}

void setup() {
  pinMode(LDR_LEFT, INPUT);
  pinMode(LDR_RIGHT, INPUT);
  pinMode(MOTOR_L, OUTPUT);
  pinMode(MOTOR_R, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  bool leftLight  = (digitalRead(LDR_LEFT)  == LOW);
  bool rightLight = (digitalRead(LDR_RIGHT) == LOW);

  if (leftLight && rightLight) drive(FAST, FAST);   // light ahead: go
  else if (leftLight)          drive(SLOW, FAST);   // light on the left: turn left
  else if (rightLight)         drive(FAST, SLOW);   // light on the right: turn right
  else                         drive(0, 0);         // no light: stop

  Serial.print(leftLight); Serial.print(' '); Serial.println(rightLight);
  delay(20);
}
