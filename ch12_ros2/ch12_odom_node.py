import math

import rclpy
from geometry_msgs.msg import TransformStamped
from nav_msgs.msg import Odometry
from rclpy.node import Node
from std_msgs.msg import Int32
from tf2_ros import TransformBroadcaster


def update_pose(x, y, th, d_left, d_right, track):
    """Differential-drive odometry: wheel distances (m) -> new pose."""
    d = (d_left + d_right) / 2.0           # distance of the robot centre
    dth = (d_right - d_left) / track       # change of heading (rad)
    x += d * math.cos(th + dth / 2.0)
    y += d * math.sin(th + dth / 2.0)
    th = math.atan2(math.sin(th + dth), math.cos(th + dth))
    return x, y, th, d, dth


class OdomNode(Node):
    def __init__(self):
        super().__init__('odom_node')
        self.declare_parameter('ticks_per_rev', 360)
        self.declare_parameter('wheel_radius', 0.0325)   # m (65 mm wheel)
        self.declare_parameter('track', 0.14)            # m between the wheels
        self.left = self.right = None
        self.prev_left = self.prev_right = None
        self.x = self.y = self.th = 0.0
        self.last_time = self.get_clock().now()
        self.create_subscription(Int32, 'left_ticks', self.on_left, 10)
        self.create_subscription(Int32, 'right_ticks', self.on_right, 10)
        self.odom_pub = self.create_publisher(Odometry, 'odom', 10)
        self.tf = TransformBroadcaster(self)
        self.create_timer(0.05, self.tick)                # 20 Hz

    def on_left(self, msg):
        self.left = msg.data

    def on_right(self, msg):
        self.right = msg.data

    def tick(self):
        if self.left is None or self.right is None:
            return
        if self.prev_left is None:
            self.prev_left, self.prev_right = self.left, self.right
            return
        p = self.get_parameter
        m_per_tick = 2 * math.pi * p('wheel_radius').value / p('ticks_per_rev').value
        d_left = (self.left - self.prev_left) * m_per_tick
        d_right = (self.right - self.prev_right) * m_per_tick
        self.prev_left, self.prev_right = self.left, self.right

        self.x, self.y, self.th, d, dth = update_pose(
            self.x, self.y, self.th, d_left, d_right, p('track').value)

        now = self.get_clock().now()
        dt = (now - self.last_time).nanoseconds * 1e-9 or 0.05
        self.last_time = now
        qz, qw = math.sin(self.th / 2.0), math.cos(self.th / 2.0)

        t = TransformStamped()
        t.header.stamp = now.to_msg()
        t.header.frame_id = 'odom'
        t.child_frame_id = 'base_link'
        t.transform.translation.x = self.x
        t.transform.translation.y = self.y
        t.transform.rotation.z = qz
        t.transform.rotation.w = qw
        self.tf.sendTransform(t)

        o = Odometry()
        o.header = t.header
        o.child_frame_id = 'base_link'
        o.pose.pose.position.x = self.x
        o.pose.pose.position.y = self.y
        o.pose.pose.orientation.z = qz
        o.pose.pose.orientation.w = qw
        o.twist.twist.linear.x = d / dt
        o.twist.twist.angular.z = dth / dt
        self.odom_pub.publish(o)


def main():
    rclpy.init()
    node = OdomNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.try_shutdown()


if __name__ == '__main__':
    main()
