import math
import time

import rclpy
from geometry_msgs.msg import Twist
from rclpy.action import ActionServer, CancelResponse, GoalResponse
from rclpy.callback_groups import ReentrantCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node
from turtlesim.msg import Pose

from turtle_goto.control import compute_cmd
from turtle_interfaces.action import GoToPoint
from turtle_interfaces.srv import SetSpeed

WORLD_MIN = 0.0
WORLD_MAX = 11.0
LOOP_PERIOD = 0.05  # seconds -> 20 Hz control loop


class GoToServer(Node):
    def __init__(self):
        super().__init__('go_to_server')

        # Parameters
        self.declare_parameter('tolerance', 0.1)
        self.declare_parameter('timeout_sec', 60.0)

        # State
        self.max_linear = 2.0
        self.max_angular = 2.0
        self.pose = None   # latest Pose from turtlesim
        self.busy = False  # True while a goal is being executed

        # A reentrant group + MultiThreadedExecutor lets the pose subscriber
        # and the service keep running while execute() is looping.
        group = ReentrantCallbackGroup()

        self.cmd_pub = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)
        self.create_subscription(
            Pose, '/turtle1/pose', self.on_pose, 10, callback_group=group)

        # Service: change speed limits
        self.create_service(
            SetSpeed, 'set_speed', self.on_set_speed, callback_group=group)

        # Action server: go to a point
        self._action_server = ActionServer(
            self,
            GoToPoint,
            'go_to_point',
            execute_callback=self.execute,
            goal_callback=self.on_goal,
            cancel_callback=self.on_cancel,
            callback_group=group,
        )

        self.get_logger().info('go_to_server ready (action: go_to_point, service: set_speed)')

    # ---------------------------------------------------------------- topics
    def on_pose(self, msg: Pose):
        self.pose = msg

    def publish_cmd(self, linear: float, angular: float):
        msg = Twist()
        msg.linear.x = float(linear)
        msg.angular.z = float(angular)
        self.cmd_pub.publish(msg)

    def stop(self):
        self.publish_cmd(0.0, 0.0)

    # --------------------------------------------------------------- service
    def on_set_speed(self, request, response):
        if request.max_linear <= 0.0 or request.max_angular <= 0.0:
            response.success = False
            response.message = 'Both speeds must be greater than 0.'
            return response

        self.max_linear = request.max_linear
        self.max_angular = request.max_angular
        response.success = True
        response.message = (
            f'max_linear={self.max_linear:.2f}, max_angular={self.max_angular:.2f}')
        self.get_logger().info(f'Speed limits changed: {response.message}')
        return response

    # ---------------------------------------------------------------- action
    def on_goal(self, goal_request):
        """Decide whether to accept or reject a new goal."""
        if self.pose is None:
            self.get_logger().warn('Rejected: no pose received yet. Is turtlesim running?')
            return GoalResponse.REJECT
        if self.busy:
            self.get_logger().warn('Rejected: another goal is still running.')
            return GoalResponse.REJECT
        if not (WORLD_MIN <= goal_request.x <= WORLD_MAX
                and WORLD_MIN <= goal_request.y <= WORLD_MAX):
            self.get_logger().warn(
                f'Rejected: ({goal_request.x:.1f}, {goal_request.y:.1f}) is outside the world.')
            return GoalResponse.REJECT

        self.busy = True
        self.get_logger().info(
            f'Goal accepted: ({goal_request.x:.2f}, {goal_request.y:.2f})')
        return GoalResponse.ACCEPT

    def on_cancel(self, goal_handle):
        self.get_logger().info('Cancel request received.')
        return CancelResponse.ACCEPT

    def execute(self, goal_handle):
        """Runs the control loop until the goal is reached, cancelled or times out."""
        goal = goal_handle.request
        tolerance = self.get_parameter('tolerance').value
        timeout = self.get_parameter('timeout_sec').value

        feedback = GoToPoint.Feedback()
        result = GoToPoint.Result()
        start = time.monotonic()

        try:
            while rclpy.ok():
                p = self.pose
                distance = math.hypot(goal.x - p.x, goal.y - p.y)

                # 1) cancelled by the client?
                if goal_handle.is_cancel_requested:
                    self.stop()
                    goal_handle.canceled()
                    result.success = False
                    result.final_distance = float(distance)
                    self.get_logger().info('Goal cancelled.')
                    return result

                # 2) arrived?
                if distance <= tolerance:
                    self.stop()
                    goal_handle.succeed()
                    result.success = True
                    result.final_distance = float(distance)
                    self.get_logger().info(f'Goal reached (error {distance:.3f}).')
                    return result

                # 3) taking too long?
                if time.monotonic() - start > timeout:
                    self.stop()
                    goal_handle.abort()
                    result.success = False
                    result.final_distance = float(distance)
                    self.get_logger().warn('Goal aborted: timeout.')
                    return result

                # 4) otherwise: drive toward the goal
                linear, angular = compute_cmd(
                    p.x, p.y, p.theta, goal.x, goal.y,
                    self.max_linear, self.max_angular)
                self.publish_cmd(linear, angular)

                feedback.distance_remaining = float(distance)
                goal_handle.publish_feedback(feedback)

                time.sleep(LOOP_PERIOD)

            # rclpy is shutting down
            self.stop()
            goal_handle.abort()
            return result
        finally:
            self.busy = False


def main(args=None):
    rclpy.init(args=args)
    node = GoToServer()
    executor = MultiThreadedExecutor()
    executor.add_node(node)
    try:
        executor.spin()
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()
