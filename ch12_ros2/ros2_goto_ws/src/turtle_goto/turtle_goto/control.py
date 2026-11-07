"""Pure-Python control law (no ROS imports), so it can be tested on its own."""
import math


def normalize_angle(a: float) -> float:
    """Wrap an angle to the range [-pi, pi]."""
    return math.atan2(math.sin(a), math.cos(a))


def clamp(value: float, low: float, high: float) -> float:
    return max(low, min(high, value))


def compute_cmd(x, y, theta, goal_x, goal_y,
                max_linear, max_angular,
                k_linear=1.5, k_angular=4.0, turn_first_angle=0.5):
    """Return (linear, angular) velocity that drives the turtle toward the goal.

    - angular speed is proportional to the heading error
    - linear speed is proportional to the distance, but only while the turtle
      is roughly facing the goal (otherwise it first turns in place)
    """
    dx = goal_x - x
    dy = goal_y - y
    distance = math.hypot(dx, dy)
    heading_error = normalize_angle(math.atan2(dy, dx) - theta)

    angular = clamp(k_angular * heading_error, -max_angular, max_angular)

    if abs(heading_error) > turn_first_angle:
        linear = 0.0
    else:
        linear = clamp(k_linear * distance, 0.0, max_linear)

    return linear, angular
