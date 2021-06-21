import serial
import time
import string

# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

while True:
    char_num = s.read(3)
    char_num = atoi(char_num)
    rsv = s.read(char_num)
    print(rsv.decode())

s.close()