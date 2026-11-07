const int PWMA = 5, AIN1 = 7, AIN2 = 8, STBY = 6;

void setup() {
  pinMode(PWMA, OUTPUT); pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT); pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
}

void drive(int speed) {            // -255 .. 255
  speed = constrain(speed, -255, 255);
  digitalWrite(AIN1, speed > 0);
  digitalWrite(AIN2, speed < 0);
  analogWrite(PWMA, abs(speed));
}
