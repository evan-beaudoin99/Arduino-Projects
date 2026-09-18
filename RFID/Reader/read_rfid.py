import serial
import time




allowed = b'\n06008E5D95\r'

ser = serial.Serial(port='COM10', baudrate=2400, timeout=1)

time.sleep(2)

print("Connected")

while True:
    response = ser.read(12)
    if response != "":
        print(response)
    time.sleep(0.001)

    if (response == allowed):
        break


print("Authentication Success")
