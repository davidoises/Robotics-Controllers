import serial
import csv
import matplotlib.pyplot as plt
import time

#data = [['Iteracion', 'Entrada', 'Salida', 'Tiempo desde sample previo']]
data = []
encoder = []
iteration_entrada = []
iteration_encoder = []
entrada = []


COMport = 'COM10'
ser = serial.Serial(port=COMport, baudrate=115200, parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)
    
for i in range(7000):
    d = str(ser.readline())[2:-5].split()
    d.insert(0, i)
    data.append(d)
    print(d)
    try:
        encoder.append(float(d[2]))
        iteration_encoder.append(d[0])
    except Exception:
        pass                            
    try:
        entrada.append(float(d[1]))
        iteration_entrada.append(d[0])
    except Exception:
        pass


time.sleep(.5)

plt.plot(iteration_encoder, encoder)
plt.show()

plt.plot(iteration_entrada, entrada)
plt.show()
  

with open('data1.csv', 'w',  newline='')as csvFile:
    writer = csv.writer(csvFile)
    for d in data:
        writer.writerow(d)
