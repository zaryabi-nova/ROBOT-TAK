// Two-state Kalman filter (angle + gyro bias) for MPU6050 pitch angle
// Arduino Uno/Nano: SDA=A4, SCL=A5
#include <Wire.h>
#include <math.h>

const uint8_t MPU = 0x68;

struct Kalman {
  float Qa = 0.001, Qb = 0.003, R = 0.03;   // tune: R = accel noise variance (deg^2)
  float angle = 0, bias = 0;
  float P[2][2] = {{1, 0}, {0, 1}};
  float update(float accAngle, float gyroRate, float dt) {
    // 1) predict with the gyro
    angle += dt * (gyroRate - bias);
    P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + Qa);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Qb * dt;
    // 2) correct with the accelerometer
    float S = P[0][0] + R;
    float K0 = P[0][0] / S, K1 = P[1][0] / S;
    float y = accAngle - angle;
    angle += K0 * y;
    bias  += K1 * y;
    float P00 = P[0][0], P01 = P[0][1];
    P[0][0] -= K0 * P00; P[0][1] -= K0 * P01;
    P[1][0] -= K1 * P00; P[1][1] -= K1 * P01;
    return angle;
  }
} kf;

int16_t read16() { int16_t v = Wire.read() << 8; return v | Wire.read(); }

unsigned long last;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU); Wire.write(0x6B); Wire.write(0); Wire.endTransmission(); // wake up
  delay(100);
  last = micros();
}

void loop() {
  Wire.beginTransmission(MPU); Wire.write(0x3B); Wire.endTransmission(false);
  Wire.requestFrom(MPU, (uint8_t)14);
  read16();                                  // accel x (not used for pitch)
  int16_t ay = read16(), az = read16();
  read16();                                  // temperature (not used)
  int16_t gx = read16(); read16(); read16(); // gyro x; y and z not used

  unsigned long now = micros();
  float dt = (now - last) * 1e-6; last = now;

  float accAngle = atan2((float)ay, (float)az) * 180.0 / M_PI;  // deg
  float gyroRate = gx / 131.0;                                  // deg/s (+-250 range)
  float angle = kf.update(accAngle, gyroRate, dt);

  Serial.print(accAngle); Serial.print(' ');
  Serial.print(angle);    Serial.print(' ');
  Serial.println(kf.bias);
  delay(5);                                  // about 200 Hz
}
