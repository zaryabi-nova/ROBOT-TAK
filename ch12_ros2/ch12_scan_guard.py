import math

import rclpy
from geometry_msgs.msg import Twist
from rclpy.node import Node
from sensor_msgs.msg import LaserScan


class ScanGuard(Node):
    """Pass cmd_vel through, but block forward motion if something is too close in front."""

    def __init__(self):
        super().__init__('scan_guard')
        self.declare_parameter('stop_distance', 0.30)   # m
        self.declare_parameter('half_angle_deg', 20.0)  # front sector: +-20 degrees
        self.front = float('inf')
        self.create_subscription(LaserScan, 'scan', self.on_scan, 10)
        self.create_subscription(Twist, 'cmd_vel_in', self.on_cmd, 10)
        self.pub = self.create_publisher(Twist, 'cmd_vel', 10)

    def on_scan(self, scan):
        half = math.radians(self.get_parameter('half_angle_deg').value)
        nearest = float('inf')
        for i, r in enumerate(scan.ranges):
            a = math.atan2(math.sin(scan.angle_min + i * scan.angle_increment),
                           math.cos(scan.angle_min + i * scan.angle_increment))
            if abs(a) <= half and scan.range_min < r < scan.range_max:
                nearest = min(nearest, r)
        self.front = nearest

    def on_cmd(self, cmd):
        if cmd.linear.x > 0 and self.front < self.get_parameter('stop_distance').value:
            cmd.linear.x = 0.0                 # still allowed: turning and reversing
            self.get_logger().warn(f'Obstacle at {self.front:.2f} m', throttle_duration_sec=1.0)
        self.pub.publish(cmd)


def main():
    rclpy.init()
    node = ScanGuard()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    node.destroy_node()
    rclpy.try_shutdown()


if __name__ == '__main__':
    main()
