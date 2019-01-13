# ReadSerial: read data from Arduino serial usb port
# Run with python ReadSerial.py
# Interupt with ^C
# Define the port below!
# Leon, 20 May 2018

import serial

ser = serial.Serial('/dev/cu.usbmodem14101',115200)
while True:
    read_serial=ser.readline()
    # Look for "$FILE:" indicator
    if (read_serial.split(' ', 1)[0]=='$FILE:'):
        # Get Filename
        filename =read_serial.split(' ', 1)[1].replace('\r\n','') # Somehow there is a CR/LF in the name...
        print 'filename:', filename
        # Open that file
        f= open(filename,"w+")
        # Keep on reading until we find "$EOF"
        read_serial=ser.readline()
        while read_serial.split(' ', 1)[0] !='$EOF':
            f.write(read_serial)
            read_serial=ser.readline()
        # close file
        f.close()
    else: # just print to stdout
        print read_serial
