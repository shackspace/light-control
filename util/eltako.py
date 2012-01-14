import serial
import sys

ser= serial.Serial(sys.argv[1],sys.argv[2],timeout=1)
toSend = [0xa5,0x5a,0x0b,0x05,int(sys.argv[4])*32+80,0x0,0x0,0x0,0x0,0x0,0x0,int(sys.argv[3]),0x10]
checksum = 0
for aInt in toSend[2:]:
	checksum=checksum + aInt
checksum=checksum%256
for aByte in toSend:
	ser.write(chr(aByte))
ser.write(chr(checksum))
ser.close()
