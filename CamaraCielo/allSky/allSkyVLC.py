
#!/bin/python

###################################### Parametros Codigo ##################################################

# Parametros codigo:
tcap = 35 # Tiempo caputura fotogramas en segundos
tencap = 25 # Tiempo entre capturas en segundos
    
# Contador capturas
rutaCon = "/home/pi/allSky/conteo.txt"
rutaLog = "/home/pi/allSky/logSkyCam.txt"

###################################### Librerias ##################################################

import os
from time import gmtime, strftime, localtime, strptime, time, sleep#, timedelta
import RPi.GPIO as GPIO
import gps as gp
import threading

###################################### Puertos ###################################### 

led_rojo = 19
led_amar = 13
led_verd = 6
ena_easy = 12

GPIO.setmode(GPIO.BCM)

GPIO.setup(led_rojo, GPIO.OUT)
GPIO.setup(led_amar, GPIO.OUT)
GPIO.setup(led_verd, GPIO.OUT)
GPIO.setup(ena_easy, GPIO.OUT)

######################################  variables captura tiempo 

tiempoS = [2000, 0, 0, 0, 0, 0]
tiempoG = [2000, 0, 0, 0, 0, 0]
tiempo = [2000, 0, 0, 0, 0, 0]
stopT1 = False

###################################### Funciones ##################################################

# hilo de captura tiempo GPS
def worker():
    """funcion que realiza el trabajo en el thread"""
    #Listen on port 2947 (gpsd) of localhost
    session = gp.gps("localhost", "2947")
    session.stream(gp.WATCH_ENABLE | gp.WATCH_NEWSTYLE)

    while True:
        try:
            report = session.next() # congela la ejecucion!
            # Wait for a 'TPV' report and display the current time
            # To see all report data, uncomment the line below
            # print report
            if report['class'] == 'TPV':
                if hasattr(report, 'time'):
                    tiempoG[0] = int(report.time[:4])
                    tiempoG[1] = int(report.time[5:7])
                    tiempoG[2] = int(report.time[8:10])
                    tiempoG[3] = int(report.time[11:13])
                    tiempoG[4] = int(report.time[14:16])
                    tiempoG[5] = int(report.time[17:19])
                    #print(tiempoG)

            if stopT1:
                break

        except KeyError:
            pass
        except KeyboardInterrupt:
            quit()
        except StopIteration:
            session = None
            print "GPSD has terminated"
    return

# iniciamos hilo captura tiempo GPS
threads = list()
t = threading.Thread(target=worker)
threads.append(t)
t.start()

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

try:
    ###################################### Inicio y Configuraciones ##################################################
    regLog("Inicio Script captura AllSky VLC")    
    GPIO.output(ena_easy,GPIO.HIGH) # Activamos capturador

    # Indicamos inicio programa (probamos leds)
    GPIO.output(led_rojo,GPIO.HIGH)
    sleep(1)
    GPIO.output(led_amar,GPIO.HIGH)
    sleep(1)
    GPIO.output(led_verd,GPIO.HIGH)
    sleep(1)
    GPIO.output(led_rojo,GPIO.LOW)
    sleep(1)
    GPIO.output(led_amar,GPIO.LOW)
    sleep(1)
    GPIO.output(led_verd,GPIO.LOW)
    regLog("Leds Iniciados") 
    
    # Conteo de capturas
    num = "1"
    try: # exepcion no existencia archivo
        file = open(rutaCon, "r")   
        num  = file.read() 
    except:
        file = open(rutaCon, "w+")   # se crea el archivo 
        file.write(num) 
        file.close() 
    cont = int(num)
    regLog("Conteo Cargado: " + str(num))


    # verificamos conexion y funcionamiento
    videoIn = 0
    maxVideo = 2
    esperar = True
    reiniciarE = 0      # permite reiniciar el capturador EasyCap
    reiniciarR = 0      # permite reiniciar la raspberry 

    # cerramos VLC
    os.system("sudo killall vlc")
    regLog("VLC Cerrado e inicio prog principal")

    ###################################### Programa principal ##################################################
    while(1):   

        # Extraemos tiempo sistema
        tiempoS[0] = int(strftime("%Y", localtime()))
        tiempoS[1] = int(strftime("%m", localtime()))
        tiempoS[2] = int(strftime("%d", localtime()))
        tiempoS[3] = int(strftime("%H", localtime()))
        tiempoS[4] = int(strftime("%M", localtime()))
        tiempoS[5] = int(strftime("%S", localtime()))

        # Tomamos el mayor de los tiempos entre Sistema y GPS
        for i in range(0,6):
            if tiempoS[i] < tiempoG[i]: # tiempo sistema menor que gps, actualizamos
                tiempo = tiempoG
                break
            if tiempoS[i] > tiempoG[i]: # tiempo sistema mayor que gps, actualizamos
                tiempo = tiempoS
                break
        regLog("Tsistema: " + str(tiempo))

        # Capturamos imagenes cada x tiempo
        if esperar: 
            regLog("Esperando... " + str(tencap) + ' Segundos')
            sleep(tencap)       # dormimos el resto de tiempo hasta timeEntreF
        else: 
            esperar = True      # activamos nuevamente la espera
        
        # Empiezo nueva captura 
        GPIO.output(led_verd,GPIO.LOW)

        # Creamos sistema de archivos
        ruta = '/home/pi/Desktop/fotosCieloAllSky/A' + str(tiempo[0]) + 'M' + "%02d"%tiempo[1] + 'D' + "%02d"%tiempo[2] + '/' 
        ensure_dir(ruta)

        # Se carga el tiempo
        tiempoStr = str(tiempo[0]) + '-' + "%02d"%tiempo[1] + '-' + "%02d"%tiempo[2] + '-' + "%02d"%tiempo[3]+ '-' + "%02d"%tiempo[4]
        regLog("nCaptura... " + ruta + " T: " + tiempoStr)

        # Capturamos la lista de archivos antes de capturar
        listaOld = os.listdir(ruta) # Dir is your directory path
        number_filesOld = len(listaOld) # Le sumamos 1 para garantizar que se guardaron almenos 2 archivos
        
        # Iniciamos VLC
        GPIO.output(led_amar,GPIO.HIGH)
        regLog("Iniciando VLC... ")
        os.system("vlc v4l2:///dev/video" + str(videoIn) + " :v4l2-standard= :live-caching=3000 --scene-path=" + str(ruta) + " --scene-prefix=" + tiempoStr + "-C" + str(cont) + "_ &")

        # Esperamos que capture un par de escenas
        regLog("Capturando... " + str(tcap) + ' Segundos')
        sleep(tcap)		#dormimos el resto de tiempo hasta timeEntreF

        # Cerramos VLC
        os.system("sudo killall vlc")
        GPIO.output(led_amar,GPIO.LOW)
        sleep(2) # Esperamos unos segundos que cierre
        
        # reiniciamos indicadores
        GPIO.output(led_rojo,GPIO.LOW)

        # Actualizamos el contador de captura
        cont = cont + 1
        file = open(rutaCon,"w") 
        file.write(str(cont)) 
        file.close() 

        ## verificamos que este guardando si no cambiamos puerto de video y reintentamos de inmediato
        # tomamos la primera lista de archivos creados para remover los vacios!
        lista = os.listdir(ruta) # dir is your directory path
        number_files = len(lista)

        # verificamos tamano archivos generados, removemos los vacios
        for i in lista:
            statinfo = os.stat(ruta + "/" + i)
            tamFile = statinfo.st_size

            # Removemos los Vacios
            if tamFile<10000:
                os.system("sudo rm " + ruta + "/" + i)

        # Cargamos de nuevo la lista de archivos creados aparentemente (tamano) validos
        lista = os.listdir(ruta) # dir is your directory path
        number_files = len(lista)

        regLog("Archivos nuevos detectados... " + str(number_files - number_filesOld))

        if number_files < (number_filesOld + 1):
            videoIn = (videoIn + 1) % maxVideo
            regLog("Falla video, probando otro puerto: " + str(videoIn))
            GPIO.output(led_rojo,GPIO.HIGH)
            cont = cont - 1     # Reiniciamos la captura anterior
            esperar = False     # Intentamos capturar de inmediato
            reiniciarE = reiniciarE + 1


            if reiniciarE > 3:
                GPIO.output(led_rojo,GPIO.HIGH)
                regLog("Reiniciando Sistema...")
                GPIO.output(ena_easy,GPIO.LOW) # Des-activamos capturador
                sleep(10)
                GPIO.output(ena_easy,GPIO.HIGH) # Activamos capturador
                #os.system("sudo reboot")
                reiniciarE = 0

                

        else: # Imagenes generadas correctamente?
            GPIO.output(led_verd,GPIO.HIGH)
            reiniciarE = 0

        # Cerramos VLC
        print ("cerrando VLC... ")
        os.system("sudo killall vlc")

        
except KeyError:
    GPIO.cleanup()
    quit()  

except KeyboardInterrupt:
    print("Saliendo")
    GPIO.cleanup()  # Revert all GPIO pins to their normal states (i.e. input = safe)
    quit()    



########################################### Cometarios ###############################################################################
#ruta = "/home/pi/Desktop/fotosCieloAllSky"
#ruta = '/home/pi/Desktop/fotosCieloAllSky/A' + strftime("%Y", localtime()) + 'M' + strftime("%m", localtime()) + 'D' + strftime("%d", localtime()) + '/' 
#tiempo = strftime("%Y-%m-%d-%H-%M", localtime()) 
# Reiniciamos Capturador
#os.system("./usbR1.sh")
#x = subprocess.check_output(['whoami'])
#print ("Nombre: " + i + " Size: " + str(tamFile) + " Valido: "+ str(tamFile>10000))
#print ("Removiendo: " + i )
#imagenes generadas
#procesarImagen(ruta)
#pass
#ruta = '/home/pi/Desktop/fotosCieloAllSky/' + strftime("%Y", localtime()) + '/' + strftime("%m", localtime()) + '/' + strftime("%d", localtime())
#ensure_dir(ruta)
# fecha actual
#tiempo = strftime("%Y-%m-%d-%H-%M-%S", localtime())

# comando busqueda de procesos con python 
# ps -ef | grep python