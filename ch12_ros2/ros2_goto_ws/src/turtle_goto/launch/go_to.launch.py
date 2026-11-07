from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        # The simulator
        Node(package='turtlesim', executable='turtlesim_node', name='sim'),

        # Our action server + service
        Node(
            package='turtle_goto',
            executable='go_to_server',
            name='go_to_server',
            output='screen',
            parameters=[{'tolerance': 0.1, 'timeout_sec': 60.0}],
        ),
    ])
