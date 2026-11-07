from machine import Pin, PWM
import time

led = Pin("LED", Pin.OUT)          # Pico W; use Pin(25) on Pico
pwm = PWM(Pin(15))
pwm.freq(1000)

while True:
    led.toggle()
    pwm.duty_u16(32768)            # 50 percent
    time.sleep(0.5)
