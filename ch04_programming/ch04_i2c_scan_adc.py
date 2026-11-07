from machine import Pin, I2C, ADC

i2c = I2C(0, scl=Pin(9), sda=Pin(8), freq=400000)   # Pico pins
print([hex(a) for a in i2c.scan()])

adc = ADC(Pin(26))
print(adc.read_u16() * 3.3 / 65535)                  # volts
