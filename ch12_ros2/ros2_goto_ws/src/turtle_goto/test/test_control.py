"""Tests for the control law. They need no ROS and no turtlesim."""
import math

from turtle_goto.control import compute_cmd, normalize_angle


def simulate(x, y, theta, goal_x, goal_y, tolerance=0.1, dt=0.05, max_steps=4000):
    """Tiny unicycle simulation. Returns (reached, steps)."""
    for step in range(max_steps):
        if math.hypot(goal_x - x, goal_y - y) <= tolerance:
            return True, step
        v, w = compute_cmd(x, y, theta, goal_x, goal_y, 2.0, 2.0)
        x += v * math.cos(theta) * dt
        y += v * math.sin(theta) * dt
        theta = normalize_angle(theta + w * dt)
    return False, max_steps


def test_normalize_angle():
    assert abs(abs(normalize_angle(3 * math.pi)) - math.pi) < 1e-9
    assert abs(normalize_angle(0.5) - 0.5) < 1e-9
    assert abs(normalize_angle(-4.0) - (2 * math.pi - 4.0)) < 1e-9


def test_reaches_goal_from_any_heading():
    for theta in (0.0, math.pi / 2, math.pi, -math.pi / 2, 2.5):
        reached, _ = simulate(5.5, 5.5, theta, 9.0, 2.0)
        assert reached, f'did not reach goal starting with theta={theta}'


def test_turns_in_place_when_facing_away():
    linear, angular = compute_cmd(5, 5, math.pi, 9, 5, 2.0, 2.0)
    assert linear == 0.0
    assert angular != 0.0


def test_respects_speed_limits():
    linear, angular = compute_cmd(0, 0, 0, 10, 0.5, 1.0, 0.5)
    assert 0.0 <= linear <= 1.0
    assert abs(angular) <= 0.5
