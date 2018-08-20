// por: Julian Galvez Serna
//---------------------------------librerias--------------------------------------

// ------------------- variables mapa ------------------- 
// tamaño del mapa
byte x = 10;
byte y = 10;
int  t = x*y;

// matriz del mapa
byte pista[][] = new byte[x][y];  // pista actual
byte pistat[][] = new byte[x][y];  // pista actual

byte posx = 0;
byte posy = 0;

// -------------------- variables algoritmo genetico -------------------- 
// Elegimos un tamaño de poblacion de 10
byte tamPoblacion = 10;
genotipo poblacion[] = new genotipo[tamPoblacion];  // poblacion de genotipos;
byte ordenPoblacion[] = new byte[tamPoblacion];
byte verPoblacion = 0;

// cada genotipo es un conjunto de pacientes-hospitales,
// en base a una asignacion y evaluacion de las capacidades y los grados de los pacientes

// pacientes y hospitales de la interface grafica
ArrayList<hospital> hospitales = new ArrayList<hospital>();
ArrayList<acidentado> acidentados = new ArrayList<acidentado>();
// ----------------------- inicializo el mapa ----------------------- 

// valores objetos mapa
byte mapa = 0;
byte hospi = 1;
byte aciden = 2;
byte drone = 3;

// valores seleccion
byte seleccion = 0;
// ------------ variables generales -----------------
// enteros para los ciclos for
int i = 0, j = 0;
// lo uso para pintar la matriz

int d_ini = 50;            // posicion donde inicia el pintado de los objetos
int sep = 25;              // separacion entre nodos
int tam = 20;              // tamaño de los nodos

// clases propias
D_pista dibujar_pista = new D_pista();
//A_asterisco A_aster = new A_asterisco();
//arbol_ancho arbol_an = new arbol_ancho();

/*// activacion diferentes entes
 boolean pint_hospital = true;  //
 boolean pint_acidentado = false;  //
 boolean pint_drone = false;  //
 */

//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

//-----------------------------------------configuracion--------------------------------------------- 
void setup() {

  // tamaño de la ventana
  size(350, 350);
  frameRate(20);             // velocidad de actualizacion

  // ---------------------- incializo valores ---------------------- 
  // inicializo pista
  for (i=0; i<x; i++) {
    for (j=0; j<y; j++) {
      pista[i][j] = mapa;
      pistat[i][j] = 1;
    }
  }

  // -------- lleno el mapa -------- 
  pista[1][2] = hospi;
  agregarItem(pista[1][2], byte(1), byte(2));
  pista[7][3] = hospi;
  agregarItem(pista[7][3], byte(7), byte(3));
  pista[4][5] = hospi;
  agregarItem(pista[4][5], byte(4), byte(5));
  pista[6][7] = hospi;
  agregarItem(pista[6][7], byte(6), byte(7));
  pista[1][8] = hospi;
  agregarItem(pista[1][8], byte(1), byte(8));

  pista[4][3] = aciden;
  agregarItem(pista[4][3], byte(4), byte(3));
  pista[7][6] = aciden;
  agregarItem(pista[7][6], byte(7), byte(6));
  pista[2][7] = aciden;
  agregarItem(pista[2][7], byte(2), byte(7));
  pista[8][9] = aciden;
  agregarItem(pista[8][9], byte(8), byte(9));
}// fin setup

//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

//---------------------------------------------graficar-----------------------------------------------
void draw() {
  background(255);    // pinto el fondo de blanco
  // -------------------------------- dibujo marco de la pista ----------------------------------
  // pinto a partir de
  translate(d_ini, d_ini);
  pushMatrix();    // guardo el estado inicial del lienzo
  dibujar_pista.draw();

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
  text("H", sep+sep/2, -7*sep/5);

  fill(color(255, 0, 0));
  rect(2*sep, -2*sep, tam, tam);
  fill(0);
  text("A", 2*sep+sep/2, -7*sep/5);

  text("P: "+ verPoblacion, 3*sep+sep/2, -7*sep/5);

  popMatrix();
}//fin draw


//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void mousePressed() {
  int moux = mouseX; 
  int mouy = mouseY; 
  // retiro los bordes superiores
  moux -= d_ini;    
  mouy -= d_ini;
  // obtengo el valor entero de la posicion
  byte mou_x = byte(moux/sep);
  byte mou_y = byte(mouy/sep);

  if (!(mou_x < 0 || mou_x >= x || mou_y < 0 || mou_y >= y)) {

    // cambio el estado de esa posicion
    if (pista[mou_x][mou_y] == mapa) {
      // cambio el mapa al valor seleccionado "creo un objeto"
      pista[mou_x][mou_y] = seleccion;
      posx = mou_x;
      posy = mou_y;
      // agregamos item
      agregarItem(seleccion, mou_x, mou_y);
    } else if (pista[mou_x][mou_y] == seleccion) {
      // elimino un objeto
      pista[mou_x][mou_y] = mapa;
      posx = 0;
      posy = 0;
      // removemos item
      removerItem(seleccion, mou_x, mou_y);
    }
  }

  if (mouseX>50 && mouseX<70 && mouseY>0 && mouseY<20) {
    genetico();  // boton calcular
  }
  if (mouseX>75 && mouseX<95 && mouseY>0 && mouseY<20) {
    seleccion = hospi;
  }
  if (mouseX>100 && mouseX<120 && mouseY>0 && mouseY<20) {
    seleccion = aciden;
  }
  println(mouseX + ", " + mouseY);
}

void keyPressed() {
  if (key == CODED) {
    acidentado viejoAccidentado = buscarAcidentado(posx, posy);
    hospital viejoHospital = buscarHospital(posx, posy);

    if (viejoAccidentado.nivel != 0) {
      println("aX:"+ viejoAccidentado.x +" Y:"+ viejoAccidentado.y +" N:"+ viejoAccidentado.nivel);
      if (keyCode == UP) {
        // sumamos al nivel si es mayor a 5
        if (viejoAccidentado.nivel<5)viejoAccidentado.nivel = byte((viejoAccidentado.nivel+1));
        pistat[posx][posy] = viejoAccidentado.nivel;
      } else if (keyCode == DOWN) {
        // restamos al nivel si es menor a 0
        if (viejoAccidentado.nivel>1)viejoAccidentado.nivel = byte((viejoAccidentado.nivel-1));
        pistat[posx][posy] = viejoAccidentado.nivel;
      }
    } else if (viejoHospital.nivel != 0) {
      println("hX:"+ viejoHospital.x +" Y:"+ viejoHospital.y +" N:"+ viejoHospital.nivel);
      if (keyCode == UP) {
        // sumamos al nivel si es mayor a 5
        if (viejoHospital.nivel<5)viejoHospital.nivel = byte((viejoHospital.nivel+1));
        pistat[posx][posy] = viejoHospital.nivel;
      } else if (keyCode == DOWN) {
        // restamos al nivel si es menor a 0
        if (viejoHospital.nivel>1)viejoHospital.nivel = byte((viejoHospital.nivel-1));
        pistat[posx][posy] = viejoHospital.nivel;
      }
    } else {
      if (verPoblacion>0) {
        //verPoblacion
        if (keyCode == UP) {
          // sumamos al nivel si es menor al tamaño maximo
          if (verPoblacion<poblacion.length-1)verPoblacion++;
        } else if (keyCode == DOWN) {
          // restamos al nivel si es mayor a 1
          if (verPoblacion>1)verPoblacion--;
        }
      }
      
    }
  }
}


//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

// ---------------------------------------- comentarios ---------------------------------------------------