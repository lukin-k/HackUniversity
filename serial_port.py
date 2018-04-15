import serial
import time
ser = serial.Serial(port='COM4', baudrate=9600)
time.sleep(2)
i=0
ser.write(b'0')
ser.close()
