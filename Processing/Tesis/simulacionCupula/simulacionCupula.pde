// Julian Galvez Serna

import oscP5.*;
import netP5.*;

ArrayList objects;
int count = 0 ;

OscP5 oscP5;
NetAddress myRemoteLocation;


float azimut = 0;
float comp = 0;
float paso = 50;

void setup() {
  size(500, 500, P3D);

  oscP5 = new OscP5(this, 12000);
  myRemoteLocation = new NetAddress("127.0.0.1", 12000);
}

void draw() {
  background(200, 200, 200);

  // dibujamos cuadricula
  //cuadricula();

  lights();
  fill(50, 50, 50);

  float diametro = 400;
  //azimut = (360.0*mouseX/width);
  comp = (1.0*mouseY/height);
  cupula(height/2, width/2, diametro, azimut, comp);
}

void cupula() {
  // inicia en azimut 0, apuntando al norte y compuertas cerradas
  cupula(0, 0, 0, 0, 0);
}

void cupula(float posx, float posy, float diametro, float azimut, float comp) {
  // trasformamos azimut
  azimut = radians(azimut)+radians(-90);
  // transformamos comp
  if (comp<0)comp=0;
  if (comp>1)comp=1;
  comp = comp*paso; // escalamos la apertura de la compuerta

  // dibujamos cupula
  float radio = diametro/2;
  float radioT = diametro/2+25;
  ellipse(posx, posy, diametro, diametro);
  // dibujamos puntos cardinales
  fill(255);
  textAlign(CENTER, CENTER);
  textSize(50);
  fill(255, 0, 0);
  text("N", posx, (posy-radioT)); 
  fill(255);
  text("S", posx, (posy+radioT)); 
  text("E", (posx+radioT), posy); 
  text("W", (posx-radioT), posy); 

  // dibujamos compuertas
  translate(posx, posy);
  rotate(azimut);
  fill(0);
  rect(-paso, -paso, radio+paso, 2*paso);
  fill(255);
  rect(-paso, -paso-comp, radio+paso, paso);
  rect(-paso, comp, radio+paso, paso);
}

void Compuerta(float estado) {
}


void cuadricula() {
  fill(255);
  for (int i=1; i<height; i+=paso) {
    for (int j=1; j<width; j+=paso) {
      line(0, j, height, j);  // altura
      line(i, 0, i, width);  // ancho
    }
  }
}

void oscEvent(OscMessage theOscMessage) {  
  
  // get the first value as an integer
  if (theOscMessage.checkAddrPattern("AZ") == true) {
    if (theOscMessage.checkTypetag("f")) {
      float azi = theOscMessage.get(0).floatValue();
      println(azi);
      azimut = azi;
    }
  }
}