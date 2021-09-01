import pyvisa as visa
import time
import numpy as np
import csv
import datetime

# Determine instruments
# Get list of instruments on port
rm = visa.ResourceManager('@py')
#print(rm.list_resources())
num = 0
usb = []
print("Checking for connected USB devices")
# Check which ones are on the USB port
for name in rm.list_resources():
    #print(name)
    if name.count("USB"):
        print(num,": ",name)
        usb.append(num)
    num = num + 1
#print(usb)

# Find the Metrolab Hall sensor and Voltcraft Multimeter
inst_found = 0
for num in usb:
    #print(num)
    time.sleep(0.5)
    inst = rm.open_resource(rm.list_resources()[num])
    instrument = inst.query("*IDN?")
    print(num,": ",instrument)
    if instrument.count("Voltcraft"):
        instVC = inst 
        inst_found = inst_found+1
    if instrument.count("Metrolab"):
        instML = inst
        inst_found = inst_found+1
print("Number of instruments found: ", inst_found)

# Report what you found
if (inst_found > 1):
    print("Multimeter:  ",instVC.query("*IDN?"))
    print("Hall sensor: ",instML.query("*IDN?"))
else:
    print("There is an instrument missing")
    exit()

# Setup Voltcraft

# Setup THM1176
# Set range to minimum (0.1T)
instML.write(":SENS:FLUX:RANGE:UPP 0.1T")

# Take point every timtep
timestep = 1 # seconds
# CSV filename and format:
filename = "data.csv"
fmt = '%s, %s, %s, %s, %s'

while True :
    time.sleep(0.5)
    # Reads multimeter value
    valueVC = float(instVC.query("MEASure:CURRent:DC?"))
    print("Current: ", valueVC)
    # Read Hall sensor value with <expected value>,<digits>
    # Reported value has T behind it, convert only first part to float
    # with split
    valueML_X = float(instML.query("MEAS:SCAL:FLUX:X? 0.1,5").split()[0])
    valueML_Y = float(instML.query("MEAS:SCAL:FLUX:Y? 0.1,5").split()[0])
    valueML_Z = float(instML.query("MEAS:SCAL:FLUX:Z? 0.1,5").split()[0])
    print("Field: ", valueML_X, valueML_Y, valueML_Z)
    # Get time
    timestr = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S:%f")
    #print(timestr)
    # Create line with data for csv file
    xarr = [timestr] + [valueVC] + [valueML_X] + [valueML_Y] + [valueML_Z]
    print(xarr)
    # Add that line to csv file
    fhandle = open(filename,'a')
    np.savetxt(fhandle, [xarr], fmt = fmt)
    fhandle.close()

