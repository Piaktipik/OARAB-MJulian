
#!/bin/python
#This script was authored by AndrewH7 and belongs to him (www.instructables.com/member/AndrewH7)
#You have permission to modify and use this script only for your own personal usage
#You do not have permission to redistribute this script as your own work
#Use this script at your own risk

#import RPi.GPIO as GPIO
#import serial
import os
from time import sleep
from time import gmtime, strftime, localtime, strptime, time#, timedelta
#import time


def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d):
        os.makedirs(d)

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
    # informo que la raspberry esta encendida
    #port = serial.Serial("/dev/ttyAMA0", baudrate=115200, timeout=3.0)
    #port.write("on")
    #port.close()
    #Use falling edge detection to see if pin is pulled
    #low to avoid repeated polling
    #GPIO.wait_for_edge(gpio_pin_number, GPIO.FALLING)
    # informo que esta siendo apagada
    #port = serial.Serial("/dev/ttyAMA0", baudrate=115200, timeout=3.0)
    #port.write("off")
    #port.close()
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
    
    tcap = 60 # Tiempo caputura fotogramas en segundos
    tencap = 240 # Tiempo entre capturas en segundos
    
    # Contador capturas
    file = open("conteo.txt", "r") 
    num  = file.read() 
    print "conteo:", str(num)
    cont = int(num)
    
    while(1):   
        # Capturamos imagenes cada x tiempo
        print ("Esperando... " + str(tencap) + ' Segundos')
        sleep(tencap)		#dormimos el resto de tiempo hasta timeEntreF
        
        # Creamos sistema de archivos
        ruta = '/home/pi/Desktop/fotosCieloAllSky/' + strftime("%Y", localtime()) + '/' + strftime("%m", localtime()) + '/' + strftime("%d", localtime()) + '/' + str(cont) + '/' 
        ensure_dir(ruta)
        tiempo = strftime("%Y-%m-%d-", localtime())
        
        # Iniciamos VLC
        print ("Iniciando VLC... ")
        os.system("vlc v4l2:///dev/video0 :v4l2-standard= :live-caching=3000 --scene-path=" + str(ruta) + " --scene-prefix=" + tiempo + " &")
        
        cont = cont + 1
        file = open("conteo.txt","w") 
        file.write(str(cont)) 
        file.close() 

        
        # Esperamos que capture un par de escenas
        print ("Capturando... " + str(tcap) + ' Segundos')
        sleep(tcap)		#dormimos el resto de tiempo hasta timeEntreF
        
        # Cerramos VLC
        print ("cerrando VLC... ")
        os.system("sudo killall vlc")

        
except:
    pass


