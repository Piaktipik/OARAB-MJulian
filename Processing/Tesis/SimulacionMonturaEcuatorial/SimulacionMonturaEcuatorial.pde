// Julian Galvez Serna

import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress myBroadcastLocation;

//--------------------------------------------------variables ejes-----------------------------------------------------
float x[] = new float[4];
float z[] = new float[4]; 
float az[] = new float[4];
float ax[] = new float[4];

float xf=0; 
float yf=0; 
float zf=0;

float Rxx=0; 
float Rxy=0; 
float Rxz=0;

// rotacion visualizacion
float rotx=radians(-45);
float roty=radians(0);
float rotz=radians(-45);

//distancia de la base al centro de la cupula
float Dx = 10;
float Dy = 10;
float Dz = 10;

// dimensiones montura
int d0 = 100;
int d1 = 100;
int d2 = 30;
int d3 = 50;
int d4 = 200;

int postes = 50;

//float latitud = radians(6.206090498573886);
float longitud = -75.550465;
float latitud = radians(6.245230);
boolean Norte = true;
int horaUTC = -5;


float DEC = 0;
float RA = 0;
// azimut cupula
float CUP = 0;

//-----------------------------------------------------programa--------------------------------------------------------
// Entrada salida Ascension Recta, Declinacion, (Altitud Azimut)
String Asc = "00 00 00.00";
int Asch = 16, Ascm = 29;
float Ascs = 24.46;
String Dec = "00 00 00.00";
int Dech = -26, Decm = 25;
float Decs = 55.7;

//float AR[]  = {16, 29, 24.46};
  //float DEC[] = {-26, 25, 55.7}; 

String s_Alt = "000.00";
String s_Azi = "000.00";
float hdeg = 0;
float Adeg = 0;

char editar = 'n';
byte cursor = 0;


// Dia  y Fecha juliana
int JDN = 0;
float JD = 0;

//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz - Setup - zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void setup() {
  size(600, 600, P3D);                 // tamaño de imagen y entrono 3D  
  //----------------------------------------------- parametros telescopio ------------------------------------------------------
  // union 1
  if (Norte) { // cambiamos latitud norte o sur
    ax[0] = latitud;
    az[0] = radians(180); //azimut al norte
  } else {
    ax[0] = -latitud;
    az[0] = radians(0); //azimut al norte
  }
  x[0] = 0;
  z[0] = d0;

  // union 2
  ax[1] = radians(90);
  x[1] = 0;            
  z[1] = d1;
  az[1] = radians(0);         
  // union 3
  ax[2] = radians(-90);
  x[2] = 0;            
  z[2] = d2;
  az[2] = radians(0); 
  // union 4
  ax[3] = radians(0);
  x[3] = 0;            
  z[3] = d3;
  az[3] = radians(0); 
  /*
   */

  // comunicacion entre ventanas
  oscP5 = new OscP5(this, 12000);
  myBroadcastLocation = new NetAddress("127.0.0.1", 12000);
}// fin void setup

//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz - Draw - zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//-------------------------------------------------------graficas----------------------------------------------------
void draw() {
  // condiciones fondo
  lights();                             //luz ambiente por defecto
  background(250);                      // pinta de blanco el fondo

  // -------------- Escribimos Cordenadas -------------- 
  fill(255);
  textAlign(CENTER, CENTER);
  textSize(20);
  fill(0, 0, 0);
  
  char RA = char(224);
  char DE = char(100);
  
  text(RA + ": " + Asc, 80, 20); 
  text(DE + ": " + Dec, 80, 45); 

  text("A: " + s_Azi, width-65, height-20); 
  text("h: " + s_Alt, width-65, height-45); 


  //---------------posicion y orentacion del plano 0----------------------------------------
  translate(width/2, height/2, 0);        //pone el plano en el centro
  rotateX(-rotx);              // roto el plano para ver mejor el brazo
  rotateY(-roty);
  rotateZ(-rotz);
  
  // dibujamos circulos importantes
  noFill();
  // dibujamos horizonte matematico
  stroke(color(153, 102, 51));
  ellipse(0,0,550,550);
  // dibujamos ecuador celeste
  rotateX(-PI/2 - ax[0]);
  stroke(color(0, 153, 0));
  ellipse(0,0,550,550);
  rotateX(PI/2 + ax[0]);
  
  //----------------dibuja el plano 0----------------------------------------
  fill(50, 128, 50);                     // color de las esferas (r g b)
  strokeWeight(1);                     // grueso de la linea 

  noStroke();                          // no dibuja las lineas de contorno
  sphere(10);                          // efera en el plano 0

  int dist = 200;
  plano(dist, 10, 2);                     // dibujo el plano 0 con los separadores de tamaño 2, distancia de 300 y pasos
  // dibujamos Norte y otros punto cardinales
  float radioT=dist+30;
  fill(255);
  textAlign(CENTER, CENTER);
  textSize(50);
  fill(255, 0, 0);
  text("N", 0, -radioT); 
  fill(0);
  text("S", 0, radioT); 
  text("E", radioT, 0); 
  text("W", -radioT, 0); 
  // de a 10

  //----------------------------------------- dibujo el telescopio -----------------------------------------------
  // lo uso para mover los ejes independientemente(solo proposito de prueba)

  az[2] = radians(450*mouseX/width-180);  // DEC     
  az[1] = radians(450*mouseY/height-180);  // RA


  DH();                                              // se calculan los vectores finales
  stroke(200);
  strokeWeight(5);

  line(0, 0, 0, xf, -yf, zf);  // se pintan los vectores finales

  translate(xf, -yf, zf);
  // en el punto final del telescopio
  // probamos la orientacion dada por la matriz de rotacion
  //line(0, 0, 0, d4*Rxx, d4*Rxy, d4*Rxz);  // se pintan los vectores finales

  translate(-xf, yf, -zf);
  //*
  //print( xf ); print( "\t" );                        // solo para visualizacion de las componetes del vector final
  //print( yf ); print( "\t" ); 
  //println( zf );//*/

  // se dibuja la montura

  for (int i=0; i < x.length; i++) {

    rotateX(-ax[i]);

    stroke(0);
    strokeWeight(5); 
    // x, y ,z
    line(0, 0, 0, x[i], 0, z[i]);
    // nos trasladamos al final de la linea dibujada
    translate(x[i], 0, z[i]); 
    // dibujamos nueva cordenada
    noStroke();
    sphere(5);

    rotateZ(-az[i]);

    // se pinta el nuevo plano
    stroke(0);
    strokeWeight(2);
    plano(20, 20, 2);                                  // muestra los planos pequeñós en cada eje
  }

  // Dibujamos OTA
  // lineas vista

  //line(0, 0, 0, d4, 0,0);

  //cilindro
  rotateY(radians(90));
  stroke(5);
  strokeWeight(2);
  drawCylinder( 40, d2, 100 );
  rotateY(radians(-90));


  /*// enviamos azimut a la cupula
   float ncup = (360.0*mouseX/width);
   if (abs(CUP-ncup)>1) {
   CUP = (360.0*mouseX/width);
   enviarAzCupula();
   }
   */

  //CUP = 180;  // Cargamso azimut para enviar a simulador cupula 
  //enviarAzCupula();

  //------------------------------con este codigo me devuelvo al plano 0 despues de dibujar el brazo-------------------

  /*
   for (int i=0;i < x.length;i++){
   rotateZ(ax[i]);
   stroke(0);strokeWeight(5); 
   line(0,0,0,x[i],0,z[i]);
   translate(x[i],0,z[i]);
   noStroke();sphere(5);
   rotateX(az[i]);
   stroke(0);strokeWeight(2);
   plano(40,20,2);                                  // muestra los planos pequeñós en cada eje   
   }
   
   
   for (int i=x.length-1;i >=0 ;i--){    
   rotateX(-az[i]);
   translate(-x[i],0,-z[i]);
   rotateZ(-ax[i]);    
   noStroke();sphere(10);    
   }//*/

  /*stroke(10);
  lights();
  translate(58, 48, 0);
  sphere(28);*/
  
  //calculoFechaJuliana();
  //calculoTSG0();
  float sexa[] = grad2sexa(6.646065556);
  sexa2grad(sexa[0],sexa[1],sexa[2]);
  
  //float AR[]  = {16, 29, 24.46}; // Ascension recta del objeto {horas, minutos, segundos} 0h<=AR<24h
  //float DEC[] = {-26, 25, 55.7}; // Declinacion del objeto     {grados, minutos, segundos} -90grados<=DEC<=90grados
  
  
  float AR[]  = {Asch, Ascm, Ascs};
  
  
  float DEC[] = {Dech, Decm, Decs};
  
  CUP = codigoMauro(AR,DEC);
  
  Asc = Asch + " " + Ascm + " " + Ascs;
  Dec = Dech + " " + Decm + " " + Decs;
  // mostramos resultado conversion
  s_Alt = String.format("%.2f", hdeg);
  //Float.toString(hdeg);
  s_Azi = String.format("%.2f", Adeg);
  //Float.toString(Adeg);
  
  enviarAzCupula();
  //convertirRaDe2AZh();
}//fin void draw

//--------------------------------------------subprogramas-------------------------------------------------------------