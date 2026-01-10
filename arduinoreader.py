import serial

ser = serial.Serial("COM5", 115200)

with open("coords.txt", "w") as f:
    while True:
        line = ser.readline().decode().strip()
        print(line)
        f.write(line + "\n")
        f.flush()
