
class grafica {
  //-----------------------------------------------------atributos------------------------------------------------------
  int tamBorde = 20;
  float Y[] = {0};
  float X_eje[] = {0};
  int X;
  private int tamano_x;
  private int tamano_y;
  private int magMax;
  private int x;
  private int y;
  private int color_linea;
  private int color_fondo;
  private int separacion;
  private int paso_x;
  private int paso_y;
  private boolean malla = false;
  //-----------------------------------------------------contructores---------------------------------------------------
  // selecciona la posicion x,y
  grafica(int x, int y) {
    this.x=x;
    this.y=y;
  }
  grafica() {
  }
  //-------------------------------------------------------metodos------------------------------------------------------
  // selecciona la posicion x,y
  void posicion(int x, int y) {
    this.x=x;
    this.y=y;
  }
  void tamano(int tamano_x, int tamano_y) {
    this.tamano_x = tamano_x;
    this.tamano_y = tamano_y;
    magMax = tamano_y/2;
  }

  void paso(int paso_x, int paso_y) {
    this.paso_x = paso_x;
    this.paso_y = paso_y;
  }
  void setmalla(boolean malla) {
    this.malla = malla;
  }
  //---------------------crea el cuadro de fondo con los ejes--------------------------

  //-------------------------grafica los separadores y una linea como eje X------------------------
  void graficar(float X_eje[]) {
    translate(x, y);   // me muevo a trabajar en la esquina donde colocaron la graficadora
    strokeWeight(1);  // tamaño de linea = 1 pixel
    stroke(0, 0, 0);    // color negro 

    //----------------rectangulo de fondo color blanco------------------
    fill(250);
    rect(0, 0, tamano_x, tamano_y);   // del tamano elegido

    // -------------------- eje y ---------------------------
    stroke(0, 0, 0);    // color eje
    line(tamBorde, 0, tamBorde, tamano_y);  // linea Inico
    textSize(10);
    fill(0, 0, 0);
    text("1",10, 15); // pongo valores
    text("-1",5, tamano_y-5); // pongo valores
    // lineas de guia
    if (malla) {
      stroke(200, 200, 200);    // color  malla
    }
    for (float i=paso_y; i<=tamano_y; i=i+paso_y) {
      if (malla) {  // agrego mallado a la grafica
        line(tamBorde, i, tamano_x, i);
      } else {
        line(tamBorde-1, i, tamBorde+1, i);  // agrego una pequeña marca a la linea
      }
    }
   
    // --------------------eje x---------------------------
    translate(tamBorde, tamano_y/2);
    stroke(0, 0, 0);    // color eje
    line(0, 0, tamano_x-20, 0);

    // lineas de guia
    if (malla) {
      stroke(200, 200, 200);
    }    // color  malla
    for (float i = 0; i <tamano_x-40; i+= paso_x) {
      if (malla) {  // agrego mallado a la grafica
        line(i, -tamano_y/2, i, tamano_y/2);
      } else { 
        line(i, -1, i, +1); // pongo una marca pequeña
      }
      rotate(PI/2);
       text("" + nf(X_eje[int(i)], 2, 1), 10, -10-(i-paso_x/4)); // pongo valores
       rotate(-PI/2);
    }
    translate(-x-tamBorde, -y-tamano_y/2);
    
    //---------fin Grafica fondo-----------
  }

  //-------------------------------grafica un vector---------------------------------
  void linea(float Y[], color col) {
    X = Y.length;                        // tamaño de linea
    translate(x+tamBorde, y+tamano_y/2); // nos movemos al inicio de la grafica
    strokeWeight(1);                     // grosor linea
    stroke(col);                         // se usa el color enviado
    fill(col);
    float p = float(tamano_x-tamBorde)/(X);     // delta en X para graficar
    //--------------grafico con lineas-------------------
    //println("grafico");
    for (int i=0; i<X-1; i++) {
      //c = (int)(i/p); println("c: " + c + " P: " + p);
      try {
        line(i*p, -Y[i]*magMax, (i+1)*p, -Y[i+1]*magMax);
      } 
      catch (Exception e) {
        println("Err: " + e.toString());
      }
    }
    translate(-x-20, -y-tamano_y/2);
  }
}  // fin dibujo linea