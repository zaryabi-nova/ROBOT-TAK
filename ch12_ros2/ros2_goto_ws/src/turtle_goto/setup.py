import os
from glob import glob

from setuptools import find_packages, setup

package_name = 'turtle_goto'

setup(
    name=package_name,
    version='0.1.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages', ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.py')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='student',
    maintainer_email='student@example.com',
    description='Go-to-point demo for turtlesim using a ROS 2 Action and Service',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'go_to_server = turtle_goto.go_to_server:main',
            'go_to_client = turtle_goto.go_to_client:main',
        ],
    },
)
