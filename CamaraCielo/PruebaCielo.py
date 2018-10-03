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

# %% Declaraciones iniciales

print("Cargando imagen...")
rutai = "C:/Users/PC/Desktop/A2018M10D03"
rutap = "C:/Users/PC/Desktop/A2018M10D03/pro"
ensure_dir(rutap)
nameImg = '/2018-10-03-01-27-C13088_00031'
image = Image.open( rutai + nameImg +'.png')
#image.show()

# %% 

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
cImage.save(rutap + nameImg +'-c.png')
#print (str(widthc) +" "+ str(heightc))  # nuevo tamano 530x500
#cImage.show()

# %% Generamos mascara de importancia (quitamos el fondo)
print("Separamos Colores...")
# creamos 3 imagenes con colores separados
pImage = np.array(cImage)
#print(pImage.shape)

cielo = np.zeros((530,500), 'uint8')
cieloA = np.zeros((530,500), 'uint8')
pRImage  = pImage[...,0]
pGImage  = pImage[...,1]
pBImage  = pImage[...,2]

imgR = Image.fromarray(pRImage)
#imgR.save(ruta +'/2018/07/12/3/2018-07-12-00061-cr.png')
imgR.save(rutap + nameImg +'-cr.png')

imgG = Image.fromarray(pGImage)
#imgG.save(ruta +'/2018/07/12/3/2018-07-12-00061-cg.png')
imgG.save(rutap + nameImg +'-cg.png')

imgB = Image.fromarray(pBImage)
#imgB.save(ruta +'/2018/07/12/3/2018-07-12-00061-c.png')
imgB.save(rutap + nameImg +'-cb.png')

# %% Deteccion de cielo (realizamos comparacion de colores de la imagen)
offset = 255*0.20 # usado para aumentar distancia entre azul y otros colores
print("Comparando colores...")
prom = 0
for i in range(0,heightc):
    for j in range(0,widthc):
        prom = prom + pBImage[i,j] + pRImage[i,j] +pGImage[i,j]
prom = prom/(3*heightc*widthc)
print(prom)
offset = prom*0.8
for i in range(0,heightc):
    for j in range(0,widthc):
        #print("i: " + str(i) + " j: " + str(j))
#        if ((pBImage[i,j] + offset) > ((pRImage[i,j]+pGImage[i,j])/2)):
#            cielo[i,j] = 255
#        else:
#            cielo[i,j] = 0
#
#        if ( (pBImage[i,j] + offset) > pRImage[i,j] and (pBImage[i,j] + offset) > pGImage[i,j] ):
#            cieloA[i,j] = 255
#        else:
#            cieloA[i,j] = 0
        #
        if ( (pBImage[i,j] > offset) and (pRImage[i,j] > offset) and ( pGImage[i,j] > offset) ):
            cieloA[i,j] = 255
        else:
            cieloA[i,j] = 0

    # fin recorrido j
# fin recorrido i
#Image.fromarray(cielo).save(rutap + nameImg +'-ccp.png')
Image.fromarray(cieloA).save(rutap + nameImg +'-cct.png')

# %% Buscamos lineas verticales y las removemos (promediandolas)
print("Eliminando saturacion sol...")
pixMax = 10    # Numero de pixeles maximo diferetes de blanco en cada linea
nLineas = 0 # Numero de lineas detectadas
lineas = np.zeros(widthc)   # Guardamos las posiciones
acielo = cielo
for j in range(0, widthc):
    linea = 0
    for i in range(0, heightc):
        # sumamos cada linea si esta en su mayoria en blanco la removemos
        linea = linea + cielo[i,j]
        #print ("linea: " + str(linea) + " mas cielo: " + str(cielo[i,j]))
    # muchos pixeles en 255
    #print ("lineaDiv: " + str(linea/255) + " heigtc: " + str(heightc - pixMax))
    if (linea/255) > (heightc - pixMax):
        # linea de saturaccion detectada (contamos el numero de lineas y su posicion y ancho)
        for i in range(0,heightc):
            acielo[i,j] = 120       # Ponemos un color gris sobre la linea detectada
            lineas[nLineas] = j    # Guardamos la posisicion de la linea
        nLineas = nLineas + 1
        
ciel = Image.fromarray(acielo)
ciel.save(rutap + nameImg +'-cls.png')     

# %% Promediamos estas lineas con su fondo
# vector vecindario pixel
cielo = acielo # aceptamos cielo con lineas
VV = np.array([[2,1],[-2,1],[1,1],[-1,1],[0,1], [2,-1],[-2,-1],[1,-1],[-1,-1],[0,-1]]) # evitar [1,0],[-1,0]
Vt = VV.shape[0]            #se usa para recorrer los pixeles aledano

for j in range(0, nLineas):
    for i in range(0, heightc):
        prom = 0.0
        cont = 0
        # Recorremos cada pixel de las lineas para promediarlas 
        for fron in range(0,Vt):
            # Tomamos la posicion del pixel en la frontera
            ii = int(i+VV[fron,0])
            jj = int(lineas[j]+VV[fron,1])
            # verificamos que las posiciones sean validas (dentro del tamano)
            if(ii>=0 and ii < heightc and jj >= 0 and jj < widthc):
                if cielo[ii,jj] != 120: # no es parte de la linea
                    prom = prom + cielo[ii,jj]
                    cont = cont + 1
                     
        cielo[i,lineas[j]] = int(prom/cont)
    print ( 'Columna: ' + str(lineas[j]))
    
#print (str(nLineas))
ciel = Image.fromarray(cielo)
ensure_dir(rutap)
ciel.save(rutap + nameImg +'-css.png')


# %% Analisis Areas Islas
print("Generando areas...")
x,y = cielo.shape  # tamano imagen
# vector vecindario pixel
VV = np.array([[0,-1],[-1,-1],[-1,0],[-1,1],[0,1],[1,1],[1,0],[1,-1]])
# Variables Recorrido
g = 1 # variable para numerar los grupos # grupos 0 = fondo 
vecg = np.zeros([2])  # tamaño de los grupos maximo 500 grupos

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
        if(cielo[i,j] > 100 and res[i,j] == 0):
            # si esto ocurre quiere decir que es un nuevo grupo de pixels
            g = g+1        # pasamos al nuevo grupo
            vecg = np.insert(vecg,len(vecg),[0],axis=0)
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
                        if(cielo[ii,jj] == cielo[int(ppr[n,0]),int(ppr[n,1])] and int(res[ii,jj]) == 0):
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

print ("Areas generadas: " + str(g))      # se imprime el numero de grupos detectado
#res =  res * (255/g)

#ciel = Image.fromarray(res)
#ciel.save(ruta + nameImg +'-zonaCielo.png')

# %% Analisis Areas Islas
# buscamos la region mas grande y la pintamos de blanco
print("buscando mayor area...")
vecg[g]
bigG = 0
valG = 0
for i in range(0,255):
    if vecg[i] > bigG:
        valG = i
        bigG = vecg[i]

# Cambiamos los valores del grupo mayor por blanco
print("Extrayendo area mayor (Cielo)...")
cieloM = np.zeros((530,500), 'uint8')
for i in range(0,heightc):
    for j in range(0,widthc):
        if (res[i,j] == valG):
            cieloM[i,j] = 255
        else:
            cieloM[i,j] = 0

ciel = Image.fromarray(cieloM)
ciel.save(rutap + nameImg +'-zonaCielo.png')


# %% zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
# Codigo recurrente analisis de todas la imagenes


# %% Cargue de imagen cielo
# Cargamos imagen
print("Cargando imagen zona cielo...")
zonaCielo = Image.open( rutap + '/FondoGenerado.png')
zCielo = np.array(zonaCielo)
print(zCielo.shape)
zCielo  = zCielo[...,0] # cargamos cualquiera de los canales

# %% Ponemos la mascara de fondo sobre la imagen para verificar
print("Removiendo Fondo...")
cieloM = np.zeros((530,500), 'uint8')
for i in range(0,heightc):
    for j in range(0,widthc):
        if (zCielo[i,j] == 0):
            pImage[i,j] = 0


ciel = Image.fromarray(pImage)
ciel.save(rutap + nameImg +'-zonaCieloF.png')

# %% Estraemos colores cielo
print("Separamos Colores cielo...")
# creamos 3 imagenes con colores separados
pImage = np.array(pImage)
#print(pImage.shape)

cielo = np.zeros((530,500), 'uint8')
cieloA = np.zeros((530,500), 'uint8')
pRImage  = pImage[...,0]
pGImage  = pImage[...,1]
pBImage  = pImage[...,2]

# %% Deteccion de nubes (realizamos comparacion de colores de la imagen)
offset = -255*0.0 # usado para aumentar distancia entre azul y otros colores
print("Extrayendo nubes...")
for i in range(0,heightc):
    for j in range(0,widthc):
        if ( (pBImage[i,j] + offset) > pRImage[i,j] and (pBImage[i,j] + offset) > pGImage[i,j] ):
            cielo[i,j] = 0
        else:
            cielo[i,j] = 255

    # fin recorrido j
# fin recorrido i
Image.fromarray(cielo).save(rutap + nameImg +'-cct.png')

# %% Deteccion de nubes como grupos dentro del cielo
print("Identificadon Nubes...")
x,y = cielo.shape  # tamano imagen
# vector vecindario pixel
VV = np.array([[0,-1],[-1,-1],[-1,0],[-1,1],[0,1],[1,1],[1,0],[1,-1]])
# Variables Recorrido
g = 1 # variable para numerar los grupos # grupos 0 = fondo 
vecg = np.zeros([255])  # tamaño de los grupos maximo 500 grupos
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
                            print('px: ' + str(i) + ',' + str(j) + ' subpx: ' + str(ii) + ',' + str(jj) + ' : ' + str(g))
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
ciel.save(rutap + nameImg +'-nCielo.png')

# %% Generamos colores aleatorios para las nubes

colores = np.zeros([g+1,1,3]).astype(int) # numero de nubes
#colores[:,:] = [100,200,100]

for i in range(0,g+1):
    red = randint(0,255)
    gre = randint(0,255)
    blu = randint(0,255)
    colores[i,:] = [int(red),int(gre),int(blu)]
    #np.insert(colores,len(colores),[r,g,b],axis=0)

# %% Pintamos las islas con colores aleatorios
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


# %% comentario y funciones de posible utilidad
#print (image.getbands())
#print (image.info)
# Image.getpixel(xy)
# Image.histogram(mask=None, extrema=None)
# Image.putalpha(alpha)
# Image.putpixel(xy, value)