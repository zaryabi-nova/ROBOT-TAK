// micro-ROS on ESP32: /cmd_vel -> motors (L298N), encoders -> /left_ticks and /right_ticks
#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>
#include <std_msgs/msg/int32.h>

const int ENA = 25, IN1 = 26, IN2 = 27;   // left motor
const int ENB = 14, IN3 = 12, IN4 = 13;   // right motor
const int ENC_LA = 34, ENC_LB = 35;       // left encoder A/B
const int ENC_RA = 32, ENC_RB = 33;       // right encoder A/B
const int LED = 2;
const float MAX_V = 0.5;        // m/s at full PWM (measure on your robot)
const float HALF_TRACK = 0.07;  // half the distance between the wheels (m)

volatile int32_t leftTicks = 0, rightTicks = 0;
void IRAM_ATTR onLeft()  { leftTicks  += digitalRead(ENC_LB) ? -1 : 1; }
void IRAM_ATTR onRight() { rightTicks += digitalRead(ENC_RB) ? 1 : -1; }  // mirrored side

rcl_subscription_t sub;
rcl_publisher_t pubLeft, pubRight;
rcl_timer_t timer;
geometry_msgs__msg__Twist cmd;
std_msgs__msg__Int32 tickMsg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
unsigned long lastCmd = 0;

#define RCCHECK(fn) { rcl_ret_t rc = fn; if (rc != RCL_RET_OK) errorLoop(); }
#define RCSOFT(fn)  { (void)(fn); }
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

void timerCallback(rcl_timer_t *t, int64_t) {      // 20 Hz: send the encoder counts
  if (t == NULL) return;
  noInterrupts();
  int32_t l = leftTicks, r = rightTicks;
  interrupts();
  tickMsg.data = l; RCSOFT(rcl_publish(&pubLeft, &tickMsg, NULL));
  tickMsg.data = r; RCSOFT(rcl_publish(&pubRight, &tickMsg, NULL));
}

void setup() {
  int outs[] = {ENA, IN1, IN2, ENB, IN3, IN4, LED};
  for (int p : outs) pinMode(p, OUTPUT);
  int ins[] = {ENC_LA, ENC_LB, ENC_RA, ENC_RB};
  for (int p : ins) pinMode(p, INPUT);              // GPIO34/35 have no pull-up: use the module's
  attachInterrupt(digitalPinToInterrupt(ENC_LA), onLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(ENC_RA), onRight, RISING);

  Serial.begin(115200);
  set_microros_serial_transports(Serial);
  delay(2000);

  allocator = rcl_get_default_allocator();
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "esp32_base", "", &support));
  RCCHECK(rclc_subscription_init_default(&sub, &node,
          ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "cmd_vel"));
  RCCHECK(rclc_publisher_init_default(&pubLeft, &node,
          ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "left_ticks"));
  RCCHECK(rclc_publisher_init_default(&pubRight, &node,
          ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "right_ticks"));
  RCCHECK(rclc_timer_init_default2(&timer, &support, RCL_MS_TO_NS(50), timerCallback, true)); // Humble: rclc_timer_init_default(...) without "true"
  RCCHECK(rclc_executor_init(&executor, &support.context, 2, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &sub, &cmd, &cmdCallback, ON_NEW_DATA));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));
  digitalWrite(LED, HIGH);                          // connected to the agent
}

void loop() {
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
  if (millis() - lastCmd > 500) {                   // safety: no command -> stop
    motor(ENA, IN1, IN2, 0);
    motor(ENB, IN3, IN4, 0);
  }
}
