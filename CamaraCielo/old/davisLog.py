#!/bin/python


###################################### Librerias ##################################################
import os
import serial
import io
from time import strftime, localtime, strptime, time, sleep
import csv

###################################### Parametros Codigo ##################################################

# Parametros codigo:
tcap = 35 # Tiempo caputura fotogramas en segundos
tencap = 25 # Tiempo entre capturas en segundos
    
# Contador capturas
rutaCon = "/home/pi/allSky/conteo.txt"
rutaLog = "/home/pi/allSky/davisLog/DavisLog.txt"

Nombres = ['L','O','O','Bar Trend','Packet Type','Next Record','Barometer','Inside Temperature','Inside Humidity','Outside Temperature','Wind Speed','10 Min Avg Wind Speed','Wind Direction','Extra Temperatures','Soil Temperatures','Leaf Temperatures','Outside Humidity','Extra Humidties','Rain Rate','UV','Solar Radiation','Storm Rain','Start Date of current Storm','Day Rain','Month Rain','Year Rain','Day ET','Month ET','Year ET','Soil Moistures','Leaf Wetnesses','Inside Alarms','Rain Alarms','Outside Alarms','Extra Temp/Hum Alarms','Soil & Leaf Alarms','Transmitter Battery Status','Console Battery Voltage','Forecast Icons','Forecast Rule number','Time of Sunrise','Time of Sunset','<LF> = 0x0A','<CR> = 0x0D','CRC']
Doff    = [1, 2, 3, 4, 5, 6, 8, 10, 12, 13, 15, 16, 17, 19, 26, 30, 34, 35, 42, 44, 45, 47, 49, 51, 53, 55, 57, 59, 61, 63, 67, 71, 72, 73, 75, 83, 87, 88, 90, 91, 92, 94, 96, 97, 98]
Dsize   = [1, 1, 1, 1, 1, 2, 2, 2, 1, 2, 1, 1, 2, 7, 4, 4, 1, 7, 2, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2, 8, 4, 1, 2, 1, 1, 2, 2, 1, 1, 2]
DFact   = [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]  
DSave   = [0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
TamD = len(Nombres)

###################################### Variables ##################################################
tiempo = [2000, 0, 0, 0, 0, 0]

###################################### Puertos ###################################### 
#print(serial.tools.list_ports)
ser = serial.Serial('/dev/ttyUSB0', 19200, timeout=5)
print(ser.name)


###################################### Funciones ###################################### 
def mix2bytes(datosE,pos):
  return ord(datosE[pos + 1]) * 256 + ord(datosE[pos])

# Registro suscesos en Txt y consola
def regLog(texto):
    print (texto)
    fileL = open(rutaLog, "a")   # se crea el archivo 
    fileL.write(texto + '\n') 
    fileL.close() 

# Verificacion rutas guardado archivos
def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d):
        os.makedirs(d)


###################################### Codigo principal ###################################### 
# Consualtamos datos consola recursivamente
while(1):
    regLog('Solicitando LOOP: ')
    ser.write(b'\n')
    ser.write(b'\n')
    sleep(0.1)
    ser.write(b'LOOP 1') 
    x = ser.read(100)          # read 99 bytes
    regLog('Lectura: ')
    regLog(x)

    #Tomamos tiempo sistema
    tiempo[0] = int(strftime("%Y", localtime()))
    tiempo[1] = int(strftime("%m", localtime()))
    tiempo[2] = int(strftime("%d", localtime()))
    tiempo[3] = int(strftime("%H", localtime()))
    tiempo[4] = int(strftime("%M", localtime()))
    tiempo[5] = int(strftime("%S", localtime()))

    # Creamos sistema de archivos
    ruta = '/home/pi/allSky/davisLog/A' + str(tiempo[0]) + 'M' + "%02d"%tiempo[1] + '/'
    ensure_dir(ruta)

    # Se carga el tiempo
    nombreArchivo = 'DatosEstacion' + str(tiempo[0]) + '-' + "%02d"%tiempo[1] + '-' + "%02d"%tiempo[2] 
    tiempoStr = str(tiempo[0]) + ':' + "%02d"%tiempo[1] + ':' + "%02d"%tiempo[2]  + '-' + "%02d"%tiempo[3] + ':' + "%02d"%tiempo[4] + ':' + "%02d"%tiempo[5] 
    regLog("nCaptura... " + ruta + " T: " + tiempoStr)

    #Procesamos los datos Capturados
    regLog('Procesando Datos: ')
    regLog('# Datos Capturados' + str(len(x)))
    #xe = x.encode('ASCII') 
    Datos = [tiempoStr]     # Cargamos el tiempo como primera columna
    if len(x) > 99:
        for i in range(TamD):
            aux = ''
            regLog(Nombres[i] + ' Save:' +  str(DSave[i]) + ' * [' + str(DFact[i])+ ']')
            if i<4:
               aux = x[i] 
            if Dsize[i] < 2:
               aux = str(ord(x[Doff[i]]))
            else:
               aux = str(mix2bytes(x,Doff[i]))
            regLog(aux)
            # Agregamos Datos a salida
            if DSave[i] == 1:
                Datos.append(aux)
    else:
       regLog('Problema Captura')
    

    #Guardamos los datos Capturados
    fileName = ruta + nombreArchivo + '.csv'
    try:
        regLog('Abriendo archivo...' + fileName)
        file = open(fileName, 'rb')
    except:
        # Archivo no existe, lo creamos
        print('Error apertura... Creando archivo')
        writer = csv.writer(open(fileName, 'w'))
        # Cargamos nombres headers
        headers = ['Tiempo Sistema'] # Cargamos el tiempo como primera columna
        for i in range(TamD):
            if DSave[i] == 1:
                headers.append(Nombres[i])
        regLog('Cabecera a escribir: ' + str(headers))
        writer.writerow(headers)

    writer = csv.writer(open(fileName, 'a'))
    regLog('Datos a escribir: ' + str(Datos))
    writer.writerow(Datos)

    # esperamos para realizar la proxima solicitud
    sleep(5)


