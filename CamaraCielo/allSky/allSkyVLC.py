
#!/bin/python

###### Librerias 

#import RPi.GPIO as GPIO
import os
from time import sleep
from time import gmtime, strftime, localtime, strptime, time#, timedelta

def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d):
        os.makedirs(d)

def enviarImagen64(f):
    print(str(f))

###### Puerto indicador estado

#gpio_pin_number = 21

#Replace YOUR_CHOSEN_GPIO_NUMBER_HERE with the GPIO pin number you wish to use
#Make sure you know which rapsberry pi revision you are using first
#The line should look something like this e.g. "gpio_pin_number=7"

#GPIO.setmode(GPIO.BCM)

#Use BCM pin numbering (i.e. the GPIO number, not pin number)
#WARNING: this will change between Pi versions
#Check yours first and adjust accordingly

#GPIO.setup(gpio_pin_number, GPIO.IN, pull_up_down = GPIO.PUD_UP)

#It's very important the pin is an input to avoid short-circuits
#The pull-up resistor means the pin is high by default

try:
    #Informo que la raspberry esta capturando fotos
    
    #GPIO.wait_for_edge(gpio_pin_number, GPIO.FALLING)
 
    #Revert all GPIO pins to their normal states (i.e. input = safe)
    #GPIO.cleanup()
    
    print ("Inicio Script captura AllSky VLC")
    # cerramos VLC
    os.system("sudo killall vlc")
    
    ruta = "/home/pi/Desktop/fotosCieloAllSky"
    
    ruta = '/home/pi/Desktop/fotosCieloAllSky/' + strftime("%Y", localtime()) + '/' + strftime("%m", localtime()) + '/' + strftime("%d", localtime())
    ensure_dir(ruta)
    # fecha actual
    tiempo = strftime("%Y-%m-%d-%H-%M-%S", localtime())
    

    # Configuracion tiempos entre fotos y numero de fotos
    # Cada 10 minutos 
    tcap = 60 # Tiempo caputura fotogramas en segundos (Se captura aproximadamente 1 imagen cada 30 segundos, la primera sale negra por lo que el EasyCap require unos segundos para iniciar)
    tencap = 30 # Tiempo entre capturas en segundos
    cont = 1 # Contador capturas (permite crear carpetas para cada momento de captura)
    

    # verificamos conexion y funcionamiento
    videoIn = 0
    maxVideo = 2
    esperar = True

    # Ciclo principal captura de imagenes del cielo
    while(1):   
        # Capturamos imagenes cada x tiempo
        print ("Esperando... " + str(tencap) + ' Segundos')
        if esperar: sleep(tencap)		#dormimos el resto de tiempo hasta timeEntreF
        else: esperar = True

        # Creamos sistema de archivos
        ruta = '/home/pi/Desktop/fotosCieloAllSky/' + strftime("%Y", localtime()) + '/' + strftime("%m", localtime()) + '/' + strftime("%d", localtime()) + '/' + str(cont) + '/' 
        ensure_dir(ruta)
        tiempo = strftime("%Y-%m-%d-", localtime())
        
        # Iniciamos VLC
        print ("Iniciando VLC... ")
        os.system("cvlc v4l2:///dev/video"+str(videoIn)+" :v4l2-standard= :live-caching=3000 --scene-path=" + str(ruta) + " --scene-prefix=" + tiempo + " &")
        cont = cont + 1
        
        # Esperamos que capture un par de escenas
        print ("Capturando... " + str(tcap) + ' Segundos')
        sleep(tcap)		#dormimos el resto de tiempo hasta timeEntreF
        
        # verificamos que este guardando si no cambiamos puerto de video y reintentamos de inmediato
        lista = os.listdir(ruta) # dir is your directory path
        number_files = len(lista)
        print number_files

        if number_files == 0:
            videoIn = (videoIn + 1) % maxVideo
            print ("Falla video, probando otro puerto: " + str(videoIn))
            cont = cont - 1     # Reiniciamos la captura anterior
            esperar = False     # Intentamos capturar de inmediato
        else:
            #imagenes generadas
            #procesarImagen(ruta)
            pass

        # Cerramos VLC
        print ("cerrando VLC... ")
        os.system("sudo killall vlc")

        
except:
    pass


def procesarImagen(f):
    # cargamos la imagen

    # ajustamos sitema coordenado

    # ortogonalizamos la imagen

    # analizamos cmabio de nubes, clusters... 

    pass
