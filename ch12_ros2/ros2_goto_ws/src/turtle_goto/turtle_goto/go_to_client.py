import sys

import rclpy
from action_msgs.msg import GoalStatus
from rclpy.action import ActionClient
from rclpy.node import Node
from rclpy.utilities import remove_ros_args

from turtle_interfaces.action import GoToPoint

USAGE = """Usage:
  ros2 run turtle_goto go_to_client X Y [X2 Y2 ...]

Examples:
  ros2 run turtle_goto go_to_client 8 3
  ros2 run turtle_goto go_to_client 2 2  9 2  9 9  2 9   # visit 4 points in order
"""


class GoToClient(Node):
    def __init__(self):
        super().__init__('go_to_client')
        self._client = ActionClient(self, GoToPoint, 'go_to_point')

    def _on_feedback(self, feedback_msg):
        remaining = feedback_msg.feedback.distance_remaining
        self.get_logger().info(
            f'distance remaining: {remaining:.2f}', throttle_duration_sec=0.5)

    def go(self, x: float, y: float) -> bool:
        """Send one goal and block until it finishes. Returns True on success."""
        if not self._client.wait_for_server(timeout_sec=5.0):
            self.get_logger().error('Action server not found. Is go_to_server running?')
            return False

        goal = GoToPoint.Goal()
        goal.x = float(x)
        goal.y = float(y)
        self.get_logger().info(f'Sending goal ({x:.1f}, {y:.1f})')

        send_future = self._client.send_goal_async(
            goal, feedback_callback=self._on_feedback)
        rclpy.spin_until_future_complete(self, send_future)
        goal_handle = send_future.result()

        if not goal_handle.accepted:
            self.get_logger().error('Goal was rejected by the server.')
            return False

        result_future = goal_handle.get_result_async()
        try:
            rclpy.spin_until_future_complete(self, result_future)
        except KeyboardInterrupt:
            # Ctrl+C -> ask the server to cancel the goal (turtle stops)
            self.get_logger().warn('Ctrl+C pressed: cancelling goal...')
            cancel_future = goal_handle.cancel_goal_async()
            rclpy.spin_until_future_complete(self, cancel_future)
            return False

        response = result_future.result()
        if response.status == GoalStatus.STATUS_SUCCEEDED:
            self.get_logger().info(
                f'Reached goal. Final error: {response.result.final_distance:.3f}')
            return True

        names = {
            GoalStatus.STATUS_CANCELED: 'canceled',
            GoalStatus.STATUS_ABORTED: 'aborted',
        }
        self.get_logger().warn(
            f'Goal {names.get(response.status, "failed")} '
            f'(distance left: {response.result.final_distance:.2f})')
        return False


def main(args=None):
    rclpy.init(args=args)
    argv = remove_ros_args(args=sys.argv)[1:]

    if len(argv) < 2 or len(argv) % 2 != 0:
        print(USAGE)
        rclpy.shutdown()
        sys.exit(1)

    try:
        coords = [float(v) for v in argv]
    except ValueError:
        print('X and Y must be numbers.\n')
        print(USAGE)
        rclpy.shutdown()
        sys.exit(1)

    node = GoToClient()
    try:
        for x, y in zip(coords[0::2], coords[1::2]):
            if not node.go(x, y):
                break
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()
