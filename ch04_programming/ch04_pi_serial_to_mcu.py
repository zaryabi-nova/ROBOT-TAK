import serial, time

ser = serial.Serial("/dev/ttyACM0", 115200, timeout=1)
time.sleep(2)                       # Arduino resets on open
ser.write(b"M 120 120\n")           # motor command
print(ser.readline().decode().strip())
