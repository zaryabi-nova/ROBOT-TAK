import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node


def generate_launch_description():
    lidar = os.path.join(get_package_share_directory('sllidar_ros2'),
                         'launch', 'sllidar_c1_launch.py')
    return LaunchDescription([
        # wheel odometry: odom -> base_footprint
        Node(package='robot_basics', executable='odom_node',
             parameters=[{'ticks_per_rev': 360, 'wheel_radius': 0.0325, 'track': 0.14}]),
        # robot body: base_footprint -> base_link (base_link 3 cm above the floor)
        Node(package='tf2_ros', executable='static_transform_publisher',
             arguments=['--z', '0.03', '--frame-id', 'base_footprint',
                        '--child-frame-id', 'base_link']),
        # LiDAR position: base_link -> laser
        Node(package='tf2_ros', executable='static_transform_publisher',
             arguments=['--x', '0.05', '--z', '0.09', '--frame-id', 'base_link',
                        '--child-frame-id', 'laser']),
        # LiDAR driver (/scan)
        IncludeLaunchDescription(PythonLaunchDescriptionSource(lidar)),
    ])
