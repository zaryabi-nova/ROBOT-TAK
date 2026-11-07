// micro-ROS on ESP32: subscribe to /cmd_vel and drive two motors with L298N
#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>

const int ENA = 25, IN1 = 26, IN2 = 27;   // left motor
const int ENB = 14, IN3 = 12, IN4 = 13;   // right motor
const int LED = 2;
const float MAX_V = 0.5;        // m/s at full PWM (measure on your robot)
const float HALF_TRACK = 0.07;  // half the distance between the wheels (m)

rcl_subscription_t sub;
geometry_msgs__msg__Twist msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
unsigned long lastCmd = 0;

#define RCCHECK(fn) { rcl_ret_t rc = fn; if (rc != RCL_RET_OK) errorLoop(); }
void errorLoop() { while (true) { digitalWrite(LED, !digitalRead(LED)); delay(100); } }

void motor(int en, int a, int b, float s) {        // s: -1..1
  s = constrain(s, -1.0f, 1.0f);
  digitalWrite(a, s > 0);
  digitalWrite(b, s < 0);
  analogWrite(en, (int)(fabs(s) * 255));
}

void cmdCallback(const void *m) {
  const geometry_msgs__msg__Twist *t = (const geometry_msgs__msg__Twist *)m;
  float v = t->linear.x, w = t->angular.z;          // m/s, rad/s
  motor(ENA, IN1, IN2, (v - w * HALF_TRACK) / MAX_V);
  motor(ENB, IN3, IN4, (v + w * HALF_TRACK) / MAX_V);
  lastCmd = millis();
}

void setup() {
  int outs[] = {ENA, IN1, IN2, ENB, IN3, IN4, LED};
  for (int p : outs) pinMode(p, OUTPUT);
  Serial.begin(115200);
  set_microros_serial_transports(Serial);
  delay(2000);

  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "esp32_base", "", &support));
  RCCHECK(rclc_subscription_init_default(&sub, &node,
          ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "cmd_vel"));
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &sub, &msg, &cmdCallback, ON_NEW_DATA));
  digitalWrite(LED, HIGH);                          // connected to the agent
}

void loop() {
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
  if (millis() - lastCmd > 500) {                   // safety: no command -> stop
    motor(ENA, IN1, IN2, 0);
    motor(ENB, IN3, IN4, 0);
  }
}
