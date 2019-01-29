import csv

fileName = '/home/pi/allSky/davisLog/file.csv'

try:
    print('Abriendo archivo')
    file = open(fileName, 'rb')
except:
    # Archivo no existe, lo creamos
    print('Error apertura... Creando archivo')
    writer = csv.writer(open(fileName, 'w'))
    writer.writeheader()

file = open(fileName, 'rb')
data = list(csv.reader(file))
print(data)

for row in data:
    print(row)

writer = csv.writer(open(fileName, 'a'))
for row in data:
    writer.writerow(row)