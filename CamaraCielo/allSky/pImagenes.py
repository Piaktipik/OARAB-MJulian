

# C:/Users/Piaktipik/Google Drive/Maestria UDEA/Proyecto Tesis/Proyecto-Automatizacion-Telescopio/Maestria-Julian/Desarollo/CamaraCielo
#!/bin/python

###### Librerias 
# libreria cargue de imagen 
from PIL import Image
import numpy as np

ruta = "/home/pi/Desktop/fotosCieloAllSky"

image = Image.open( ruta +'/2018/07/12/3/2018-07-12-00061.png')
image.show()

# extraemos solo el centro de la imagen
width, height = image.size   # Get dimensions 720X576
#print (str(width) +" "+ str(height))
left = 106
top = 28
right = width-114
bottom = height-18

cImage = image.crop((left, top, right, bottom))
widthc, heightc = cImage.size 

#print (str(widthc) +" "+ str(heightc))  # nuevo tamano 530x500
#cImage.show()

# generamos mascara de importancia (quitamos el fondo)

# creamos 3 imagenes con colores separados
pImage = np.array(cImage)
#print(pImage.shape)
cielo = np.zeros((530,500), 'uint8')
cieloA = np.zeros((530,500), 'uint8')
pRImage  = pImage[...,0]
pGImage  = pImage[...,1]
pBImage  = pImage[...,2]

# rgbArray = np.zeros((500,530,3), 'uint8')
# rgbArray[..., 0] = r*256
# rgbArray[..., 1] = g*256
# rgbArray[..., 2] = b*256
imgR = Image.fromarray(pRImage)
imgR.save(ruta +'/2018/07/12/3/2018-07-12-00061-cr.png')
imgG = Image.fromarray(pGImage)
imgG.save(ruta +'/2018/07/12/3/2018-07-12-00061-cg.png')
imgB = Image.fromarray(pBImage)
imgB.save(ruta +'/2018/07/12/3/2018-07-12-00061-c.png')

# deteccion de cielo (realizamos comparacion de colores de la imagen)
print("Comparando colores...")
for i in range(0,heightc):
    for j in range(0,widthc):
        #print("i: " + str(i) + " j: " + str(j))
        if (pBImage[i,j] > ((pRImage[i,j]+pGImage[i,j])/2)):
            cielo[i,j] = 255
        else:
            cielo[i,j] = 0

        if ( pBImage[i,j] > pRImage[i,j] and pBImage[i,j] > pGImage[i,j] ):
            cieloA[i,j] = 255
        else:
            cieloA[i,j] = 0

    # fin recorrido j
# fin recorrido i

#cImage.save( ruta +'/2018/07/12/3/2018-07-12-00061-c.png')

print("Eliminando saturacion sol...")
# buscamos lineas verticales y las removemos
for j in range(0,widthc):
    linea = 0;
    for i in range(0,heightc):
        # sumamos cada linea si esta en su mayoria en blanco la removemos
        linea = linea + cielo[i,j]
    if (linea/255) > (heightc -50):
        # linea de daturaccion detectada (contamos el numero de lineas y su posicion y ancho)
        for i in range(0,heightc):
            cielo[i,j] = 0


ciel = Image.fromarray(cielo)
ciel.save(ruta +'/2018/07/12/3/2018-07-12-00061-cieloL.png')

# analisis islas
cierro = 4/0
#Asignamos etiquetas a las islas
# tamano imagen
x,y = cielo.shape

# vector vecindario pixel
VV = np.array([[0,-1],[-1,-1],[-1,0],[-1,1],[0,1],[1,1],[1,0],[1,-1]])

# Variables Recorrido
# variable para numerar los grupos
g = 1 # grupos 0 = fondo 
vecg = np.zeros([255])

# creamos imagenes vacias para asignar los grupos
res = np.zeros([x,y], 'uint8')       # resultados grupos

# probando guardado
#res =  res * (255/g)
#ciel = Image.fromarray(res)
#ciel.save(ruta +'/2018/07/12/3/2018-07-12-00061-cielo.png')


Vt = VV.shape[0]            #se usa para recorrer los pixeles aledano

# creamos vector para pixeles por recorrer
ppr = np.zeros([1,2])
n = 1

# %%
print("Generando areas...")
#se recorre cada pixel 
for i in range(0,x):
    for j in range(0,y):
        # se verifica que el pixel actual no sea fondo
        # y no tenga grupo aun (no ha sido explorado)
        if(cielo[i,j] > 100 and res[i,j] == 0):
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
                        if(cielo[ii,jj] == cielo[int(ppr[n,0]),int(ppr[n,1])] and int(res[ii,jj]) == 0):
                            #se agrupa 
                            res[ii,jj] = g
                            vecg[g] = vecg[g]+1
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

print ("g: " + str(g))
#res =  res * (255/g)

ciel = Image.fromarray(res)
ciel.save(ruta +'/2018/07/12/3/2018-07-12-00061-cielo.png')

# buscamos la region mas grande y la pintamos de blanco
print("buscando mayor area...")
vecg[g]
bigG = 0
valG = 0
for i in range(0,255):
    if vecg[i] > bigG:
        valG = i
        bigG = vecg[i]

# con el valor de listo buscamos este valor en la imagen y la cambiamos por blanco
print("Extrayendo area...")
cieloM = np.zeros((530,500), 'uint8')
for i in range(0,heightc):
    for j in range(0,widthc):
        if (res[i,j] == valG):
            cieloM[i,j] = 255
        else:
            cieloM[i,j] = 0

ciel = Image.fromarray(cieloM)
ciel.save(ruta +'/2018/07/12/3/2018-07-12-00061-cieloM.png')

#print (image.getbands())
#print (image.info)
# Image.getpixel(xy)
# Image.histogram(mask=None, extrema=None)
# Image.putalpha(alpha)
# Image.putpixel(xy, value)


def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d):
        os.makedirs(d)

def enviarImagen64(f):
    print(str(f))

