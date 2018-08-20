// por: Julian Galvez Serna
//---------------------------------librerias--------------------------------------

// ------------------- variables malla ------------------- 
// tamaño de la grafica de la malla
float txi = 1000;
float tyi = 600;
float tx = txi;
float ty= tyi;
float ux = 0;
float uy = 0;

float posX = 0;
float posY = 0;

int d_ini = 50;            // posicion donde inicia el pintado de los objetos
int sep = 25;              // separacion entre nodos
int tam = 20;              // tamaño de los nodos
float tamTexi = 5;
float tamTex = tamTexi;

// ------------------- variables carga de la malla ------------------- 
String lines[];
// matriz de nodos
float pX[];
float pY[];

// ------------------- variables procesado de malla ------------------- 
char[] comando;
int[] numeracion;
float[] valor;
float mallaX = 0;
float mallaY = 0;
int comienzoFilas = 0;


// ------------ variables generales -----------------
// enteros para los ciclos for
int i = 0, j = 0;

//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

//-----------------------------------------configuracion--------------------------------------------- 
void setup() {

  // tamaño de la ventana
  size(1100, 400);
  frameRate(10);             // velocidad de actualizacion
}
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

//---------------------------------------------graficar-----------------------------------------------
void draw() {
  background(255);    // pinto el fondo de blanco

  // -------------------------------- dibujo de botones ----------------------------------
  // pinto a partir de
  translate(d_ini, d_ini);

  // -------------------------------- dibujo de la malla -------------------------------- 
  pushMatrix();    // guardo el estado inicial del lienzo

  // movemos las mallas
  translate(posX, posY);

  if (comienzoFilas != 0) {
    stroke(0);      // color de linea negra
    noFill();       // no hay relleno
    for (i=0; i<pX.length; i++) {
      line(pX[i], 0, pX[i], ty);
    }
    for (i=0; i<pY.length; i++) {
      line(0, pY[i], tx, pY[i]);
    }
  }//*/
  // -------------------------------- dibujo de los nodos -------------------------------- 
  int tamp=2;
  int cont = 1;
  textSize(tamTex);

  // si hay malla procesada
  if (comienzoFilas != 0) {
    fill(color(255, 0, 0));
    noStroke();

    for (i=0; i<pX.length; i++) {

      for (j=0; j<pY.length-2; j++) {

        ellipse(pX[i], pY[j], tamp, tamp);
        text(cont, pX[i], pY[j]);
        cont++;

        // llenado nodos intermedios
        if (j < pY.length-3) {
          ellipse(pX[i], (pY[j+1]-pY[j])/2+pY[j], tamp, tamp);
          text(cont, pX[i], (pY[j+1]-pY[j])/2+pY[j]);
          cont++;
        }
      }// fin for Y

      if (i < pX.length-1) {
        for (j=0; j<pY.length-2; j++) {
          ellipse((pX[i+1]-pX[i])/2+pX[i], pY[j], tamp, tamp);
          text(cont, (pX[i+1]-pX[i])/2+pX[i], pY[j]);
          cont++;
        }
      } //*/// llenano de espacios intermedios
    }
  }//*/
  popMatrix();

  // pinto un cuadro y los botones sobre los otros dibujos
  pushMatrix();    // guardo el estado inicial del lienzo

  // rectangulo blanco
  fill(color(255, 255, 255));       // relleno con el color respectivo
  rect(-d_ini, -d_ini, width, tam);

  // botones para cambio de mapa
  stroke(0);      // color de linea negra
  fill(color(  0, 0, 0));       // relleno con el color respectivo
  rect(0, -2*sep, tam, tam);
  fill(color(  0, 225, 0));
  textSize(10);
  text("RUN", 0, -7*sep/5);

  fill(color(255, 220, 80));
  rect(sep, -2*sep, tam, tam);
  fill(0);
  text("M", sep+tam/4, -7*sep/5);

  fill(color(255, 0, 0));
  rect(2*sep, -2*sep, tam, tam);
  fill(0);
  text("R", 2*sep+tam/4, -7*sep/5);

  popMatrix();

  text("Por: Julian Galvez Serna", 3*sep+sep/2, -7*sep/5);

  //pintamos y numeramos los nodos
}//fin draw


//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void cargarMalla() {
  lines = loadStrings("ELMSIZE.dat");
  println("there are " + lines.length + " lines");
  for (int i = 0; i < lines.length; i++) {
    println(lines[i]);
  }
}

void procesarMalla() {
  boolean columnas = true;
  char separacion = 'E';
  char comando[] = new char[lines.length];
  int numeracion[] = new int[lines.length];
  float valor[] = new float[lines.length];
  mallaX = 0;
  mallaY = 0;

  for (int i = 0; i < lines.length; i++) {
    String[] linea = split(lines[i], '\t');
    comando[i] = linea[0].charAt(0);
    numeracion[i] = int(linea[1]);
    valor[i] = float(linea[2]);

    if (columnas) {
      mallaX += valor[i];
    } else {
      mallaY += valor[i];
    }
    if (separacion == comando[i]) {
      if (columnas) {
        comienzoFilas=i;
        println(i);
      }
      columnas = false;
    }
  }
  println("Totales: Columnas: " + mallaX + ", Filas: " + mallaY);
  for (int j = 0; j<lines.length; j++) {
    print(lines[j]+ "-");
    println(comando[j] + "," + numeracion[j]+ "," + valor[j]);
  }//*/

  this.comando = comando;
  this.numeracion = numeracion;
  this.valor = valor;
  // una vez cargados y procesados los datos los graficamos
  // se calcula el tamaño maximo para manetenerlo a escala
  if (mallaX > mallaY) {
    ux = tx/mallaX;
    uy = ux;
    ty = uy*mallaY;
  } else {
    uy = ty/mallaY;
    ux = uy;
    tx = ux*mallaX;
  }
  // cremos vector con las posiciones de los nodos
  pX = new float[comienzoFilas+2];
  pY = new float[(lines.length - comienzoFilas)+2];
  float sumaCol = 0;
  pX[0]=0;
  float sumaFil = ty;
  pY[0]=ty;
  int cx = 1, cy = 1;
  for (i=0; i<valor.length; i++) {
    if (i<=comienzoFilas) {
      sumaCol += ux*valor[i];
      pX[cx]=sumaCol;
      cx++;
    } else {
      sumaFil -= uy*valor[i];
      pY[cy]=sumaFil;
      cy++;
    }
  }
}

void mousePressed() {
  // obtengo el valor entero de la posicion
  if (mouseX>50 && mouseX<70 && mouseY>0 && mouseY<20) {
    cargarMalla();
  }
  if (mouseX>75 && mouseX<95 && mouseY>0 && mouseY<20) {
    procesarMalla();
  }
  if (mouseX>100 && mouseX<120 && mouseY>0 && mouseY<20) {
    tx = txi;
    ty= tyi;
    posY = 0;
    posX = 0;
    tamTex = tamTexi;
    procesarMalla();
  }
  println(mouseX + ", " + mouseY);
}

void keyPressed() {
  if (key == CODED) {
    if (keyCode == UP) {
      posY -= 10;
    } else if (keyCode == DOWN) {
      posY += 10;
    } else if (keyCode ==LEFT) {
      posX -= 10;
    } else if (keyCode == RIGHT) {
      posX += 10;
    }
  }
  if (key == '+') {
    tx += tx*0.1;
    ty += ty*0.1;
    tamTex += tamTex*0.04;
  }
  if (key == '-') {
    tx -= tx*0.1;
    ty -= ty*0.1;
    tamTex -= tamTex*0.04;
  }
  procesarMalla();
}

//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz