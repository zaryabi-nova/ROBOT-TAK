import rclpy
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import Image
from geometry_msgs.msg import Twist
from cv_bridge import CvBridge
from ch11_red_object_track import find_red   # فایل ردیابی قرمز همین فصل

class Follower(Node):
    def __init__(self):
        super().__init__("color_follower")
        self.declare_parameter("kp", 0.005)
        self.br = CvBridge()
        self.create_subscription(Image, "/image_raw", self.cb, qos_profile_sensor_data)
        self.pub = self.create_publisher(Twist, "/cmd_vel", 10)

    def cb(self, msg):
        frame = self.br.imgmsg_to_cv2(msg, "bgr8")
        res, _ = find_red(frame)
        t = Twist()                                   # پیش‌فرض: ایست
        if res:
            err_x = res[0] - frame.shape[1] / 2
            kp = self.get_parameter("kp").value
            t.angular.z = max(-1.0, min(1.0, -kp * err_x))
        self.pub.publish(t)

def main():
    rclpy.init()
    node = Follower()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.pub.publish(Twist())                         # ایست هنگام خروج
    node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()
