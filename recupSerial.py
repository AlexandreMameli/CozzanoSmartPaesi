import serial
import time
import csv
import datetime
import serial.tools.list_ports

ports = serial.tools.list_ports.comports(include_links=False) # even if we can detect all the com ports, we'll only take the first one for simplicity's sake

ser = serial.Serial(ports[0].device)
ser.flushInput()
serialData = ""

while True:
    try:
        ser_bytes = ser.readline()
        serialData += str(ser_bytes.decode())
        serialData += ","
        print(ser_bytes.decode())

    except:
        print("Recording stopped.")
        fileName = str(datetime.datetime.now())
        fileName = fileName.replace(" ", "")
        fileName = fileName.replace(".", "-")
        fileName = fileName.replace(":", "-")

        serialData = serialData.replace("\n", "")

        file =  open(fileName + ".csv","w")
        file.write(serialData)
        file.close()
        break