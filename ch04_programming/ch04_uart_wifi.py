from machine import UART, Pin
import network, time

uart = UART(1, baudrate=115200, tx=Pin(4), rx=Pin(5))
uart.write(b"hello\n")

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect("SSID", "PASSWORD")
while not wlan.isconnected():
    time.sleep(0.2)
print(wlan.ifconfig())
