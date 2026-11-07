from gpiozero import Motor, DistanceSensor
from time import sleep

motor = Motor(forward=17, backward=18)
sensor = DistanceSensor(echo=24, trigger=23)   # echo needs a 3.3V divider

while True:
    if sensor.distance > 0.3:      # meters
        motor.forward(0.6)
    else:
        motor.stop()
    sleep(0.05)
