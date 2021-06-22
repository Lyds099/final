
import serial
import time
import sys,tty,termios

# XBee setting
serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

while True:
    if(s.readable):
        rsv = s.readline()
        print(rsv.decode())