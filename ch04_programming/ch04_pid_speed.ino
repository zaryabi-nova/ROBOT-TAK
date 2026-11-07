float Kp = 1.2, Ki = 0.5, Kd = 0.0;
float integ = 0, prevErr = 0;

int pidStep(float target, float measured, float dt) {
  float err = target - measured;
  integ += err * dt;
  integ = constrain(integ, -200, 200);        // anti-windup
  float d = (err - prevErr) / dt;
  prevErr = err;
  float u = Kp * err + Ki * integ + Kd * d;
  return (int)constrain(u, -255, 255);
}
