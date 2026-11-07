from machine import Pin, PWM
from time import sleep_ms

LINE = 1                     # value over the black line: check your module!
sL, sC, sR = Pin(6, Pin.IN), Pin(7, Pin.IN), Pin(8, Pin.IN)

class Motor:
    def __init__(self, en, a, b):
        self.pwm = PWM(Pin(en)); self.pwm.freq(1000)
        self.a, self.b = Pin(a, Pin.OUT), Pin(b, Pin.OUT)
    def run(self, speed):        # speed: -100..100 %
        self.a.value(speed > 0)
        self.b.value(speed < 0)
        self.pwm.duty_u16(int(min(abs(speed), 100) * 655))

left, right = Motor(2, 3, 4), Motor(5, 10, 11)
BASE, SOFT, SEARCH = 60, 35, 45
last = 1

def drive(l, r):
    left.run(l); right.run(r)

while True:
    L, C, R = sL.value() == LINE, sC.value() == LINE, sR.value() == LINE
    if L and C and R:          drive(0, 0)
    elif C and not (L or R):   drive(BASE, BASE)
    elif L and C:              drive(SOFT, BASE); last = -1
    elif R and C:              drive(BASE, SOFT); last = 1
    elif L:                    drive(0, BASE); last = -1
    elif R:                    drive(BASE, 0); last = 1
    elif last < 0:             drive(-SEARCH, SEARCH)
    else:                      drive(SEARCH, -SEARCH)
    sleep_ms(5)
