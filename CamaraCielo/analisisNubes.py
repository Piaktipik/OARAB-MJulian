# -*- coding: utf-8 -*-
"""
Created on Mon Aug 13 15:54:08 2018

@author: Piaktipik
"""
#!/bin/python

###### Librerias 
# libreria cargue de imagen 
from PIL import Image
import numpy as np
from random import randint
import os


# %% Funciones
def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d):
        os.makedirs(d)

def enviarImagen64(f):
    print(str(f))

# %% Datos ubicacion archivos
rutai = "C:/Users/PC/Desktop/A2018M10D03"
rutap = "C:/Users/PC/Desktop/A2018M10D03/pro"
ensure_dir(rutap)
#image.show()

# %% Cargue de imagen cielo
# Cargamos imagen
print("Cargando imagen zona cielo...")
zonaCielo = Image.open( rutap + '/FondoGenerado.png')
zCielo = np.array(zonaCielo)
print(zCielo.shape)
zCielo  = zCielo[...,0] # cargamos cualquiera de los canales


# %% zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
# Codigo recurrente analisis de todas la imagenes

# tomamos lista de imagenes en la carpeta
lista = os.listdir(rutai) # dir is your directory path
number_files = len(lista)
cont = 0
for i in lista[500:-1]:
    nameImg = "/" + i[:-4]
    cont = cont + 1
    print("Cargando imagen " + nameImg + ", (" + str(cont) +"/"+ str(number_files) +")")
    image = Image.open( rutai + nameImg +'.png')

    # % Cortamos imagen 
    
    print("Cortando Imagen...")
    # extraemos solo el centro de la imagen
    width, height = image.size   # Get dimensions 720X576
    #print (str(width) +" "+ str(height))
    left = 106
    top = 28
    right = width-114
    bottom = height-18
    cImage = image.crop((left, top, right, bottom))
    widthc, heightc = cImage.size 
    #cImage.save(rutap + nameImg +'-c.png')
    #print (str(widthc) +" "+ str(heightc))  # nuevo tamano 530x500
    #cImage.show()
    
    
    # % Ponemos la mascara de fondo sobre la imagen para verificar
    pImage = np.array(cImage)
    print("Removiendo Fondo...")
    cieloM = np.zeros((530,500), 'uint8')
    for i in range(0,heightc):
        for j in range(0,widthc):
            if (zCielo[i,j] == 0):
                pImage[i,j] = 0
    
    
    ciel = Image.fromarray(pImage)
    ciel.save(rutap + nameImg +'-zonaCieloF.png')
    
    # % Estraemos colores cielo
    print("Separamos Colores cielo...")
    # creamos 3 imagenes con colores separados
    #pImage = np.array(cImage)
    #print(pImage.shape)
    
    cielo = np.zeros((530,500), 'uint8')
    cieloA = np.zeros((530,500), 'uint8')
    pRImage  = pImage[...,0]
    pGImage  = pImage[...,1]
    pBImage  = pImage[...,2]
    
    # % Deteccion de nubes (realizamos comparacion de colores de la imagen)
    offset = 255*0.05 # usado para aumentar distancia entre azul y otros colores
    oscuro = 50
    
    prom = 0
    for i in range(0,heightc):
        for j in range(0,widthc):
            prom = prom + pBImage[i,j] + pRImage[i,j] +pGImage[i,j]
    prom = prom/(3*heightc*widthc)
    print("Promedio imagen: " + str(prom))
    offset = prom*0.8

    print("Extrayendo nubes...")
    for i in range(0,heightc):
        for j in range(0,widthc):
            # las nubes tienes un color al dia diferente a la noche
            
#            if ( (pBImage[i,j] + offset) > pRImage[i,j] and (pBImage[i,j] + offset) > pGImage[i,j] ):
#                cielo[i,j] = 0
#            else:
#                cielo[i,j] = 255
#            # usado en proF
#            if ((oscuro + offset) < ((pBImage[i,j]+pRImage[i,j]+pGImage[i,j])/3)):
#                cielo[i,j] = 255
#            else:
#                cielo[i,j] = 0
            # usado en pro
            if ( (pBImage[i,j] > offset) and (pRImage[i,j] > offset) and ( pGImage[i,j] > offset) ):
                cielo[i,j] = 255
            else:
                cielo[i,j] = 0
            
        # fin recorrido j
    # fin recorrido i
    #Image.fromarray(cielo).save(rutap + nameImg +'-cct.png')
    
    # % Deteccion de nubes como grupos dentro del cielo
    print("Identificadon Nubes...")
    x,y = cielo.shape  # tamano imagen
    # vector vecindario pixel
    #VV = np.array([[0,-1],[-1,-1],[-1,0],[-1,1],[0,1],[1,1],[1,0],[1,-1]])
    VV = np.array([[0,-1],[-1,0],[0,1],[1,0]])
    # Variables Recorrido
    g = 1 # variable para numerar los grupos # grupos 0 = fondo 
    vecg = np.zeros([x*y])  # tamaño de los grupos maximo 500 grupos
    # creamos imagenes vacias para asignar los grupos
    #res = np.zeros([x,y], 'uint8')       # resultados grupos
    res = np.zeros([x,y])       # resultados grupos
    Vt = VV.shape[0]            #se usa para recorrer los pixeles aledano
    # creamos vector para pixeles por recorrer
    #ppr = np.zeros([1,2])
    n = 1
    
    #se recorre cada pixel 
    for i in range(0,x):
        for j in range(0,y):
            # se verifica que el pixel actual no sea fondo
            # y no tenga grupo aun (no ha sido explorado)
            if(zCielo[i,j] > 100 and res[i,j] == 0 and cielo[i,j] > 100):
                # si esto ocurre quiere decir que es un nuevo grupo de pixels
                g = g+1        # pasamos al nuevo grupo
                ppr = np.zeros([1,2])
                ppr[:,:] = [i,j] # empezamos recorriendo este pixel
                n = 0          # en la posicion inicial
                
                # recorremos la lista de pixeles por recorrer
                while(n<ppr.shape[0]):
                    # se analizan las fronteras del pixel actual
                    for fron in range(0,Vt):
                        # tomamos la posicion del pixel en la frontera
                        ii = int(ppr[n,0]+VV[fron,0])
                        jj = int(ppr[n,1]+VV[fron,1])
                        # verificamos que las posiciones sean validas (dentro del tamano)
                        if(ii>=0 and ii < x and jj >= 0 and jj < y):
                            # si el pixel del lado es del mismo grupo (cielo(ii,jj) == cielo(i,j))
                            # y no ha sido agrupado
                            if(cielo[ii,jj] == cielo[int(ppr[n,0]),int(ppr[n,1])] and int(res[ii,jj]) == 0 and zCielo[ii,jj] > 100):
                                #se agrupa 
                                res[ii,jj] = g
                                vecg[g] = vecg[g]+1
                                #print('px: ' + str(i) + ',' + str(j) + ' subpx: ' + str(ii) + ',' + str(jj) + ' : ' + str(g))
                                # se pone en cola para ser explorado (n++)
                                ppr = np.insert(ppr,len(ppr),[ii,jj],axis=0)
                                # fin pixel del mismo grupo
                            # fin pixel dentro de la imagen
                        # fin recorrido de pixeles de los lados
                    # se pasa al siguiente pixel en la lista
                    n = n+1
                    # fin lista pixeles por recorrer
                # fin pixel valido y sin recorrer
            # fin recorrido pixeles
    
    print ("g: " + str(g))      # se imprime el numero de grupos detectado
    #res =  res * (255/g)
    
    ciel = Image.fromarray(res)
    if ciel.mode != 'RGB':
        ciel = ciel.convert('RGB')
    #ciel.save(rutap + nameImg +'-nCielo.png')
    
    # % Generamos colores aleatorios para las nubes
    print("Pintando Nubes...")
    colores = np.zeros([g+1,1,3]).astype(int) # numero de nubes
    #colores[:,:] = [100,200,100]
    
    for i in range(0,g+1):
        red = randint(0,255)
        gre = randint(0,255)
        blu = randint(0,255)
        colores[i,:] = [int(red),int(gre),int(blu)]
        #np.insert(colores,len(colores),[r,g,b],axis=0)
    
    # % Pintamos las islas con colores aleatorios
    ciel = Image.fromarray(res)
    if ciel.mode != 'RGB':
        ciel = ciel.convert('RGB')
        
    for i in range(0,x):
        for j in range(0,y):
            if ciel.getpixel((j,i))[0] > 0:
                ciel.putpixel((j,i), tuple(colores[ciel.getpixel((j,i))[0],:][0]))
                
    ciel.save(rutap + nameImg +'-nCieloc.png')
    
    # %% Guardamos datos nubes en Txt
    
    
    
    # %% Comparacion tendencia nubes

