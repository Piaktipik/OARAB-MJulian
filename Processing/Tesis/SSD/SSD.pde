
// ----------------- Archivo de salida -----------------
PrintWriter output;
String tiempoInicio = "";
String datosLog = "";        // para guardar los datos a enviar al txt

// ----------------- Puerto serial -----------------
import processing.serial.*;
Serial myPort;
boolean conexion = true;

//------------------- Variables Grafica------------------- 
// se crea la grafica
grafica grafica1 = new grafica();

static int PgraficaX = 10;
static int PgraficaY = 10;

static int tamX = 250;
static int tamY = 150;

// Procesamiento datos
static char sepa[] = {'T', 'A', 'B', 'C', 'M', 'N', 'O', 'G', 'H', 'I', 'D'}; // Trama datos
float datosD[][] = new float[11][tamX]; 
float datosS[][] = new float[11][tamX];                                // Guardado datos
float scalaD[] = {1, 
  0.0022888184, 0.0022888184, 0.0022888184, 
  0.018310547, 0.018310547, 0.018310547, 
  0.0022888184, 0.0022888184, 0.0022888184, 1};         // Escalas 
int c = 0;                                                             // Variable orden captura

// Nombres datos
static String nombres[] = {"Tiem", 
  "AccX", "AccY", "AccZ", 
  "MagX", "MagY", "MagZ", 
  "VAnX", "VAnY", "VAnZ", 
  "Corr"};
static int posTextGrafI_X = tamX + PgraficaX+10;
static int pasoTextGraf = int(tamY/nombres.length);

//color de lineas
color colores[] = {color(0, 0, 0), 
color(255, 0, 0), color(0, 255, 0), color(0, 0, 255), 
color(255, 255, 0), color(0, 255, 255), color(255, 0, 255), 
color(50, 50, 50), color(100, 100, 100), color(150, 150, 150), 
color(200, 20, 50)};


// Analisis Datos


//ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Configuracion ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
void setup() {
  // Marco ventana
  size(800, 600);
  //frameRate(100);

  // print a list of the serial ports:
  //printArray(Serial.list());

  tiempoInicio = year() + "-" + day() + "-" + month() + "-" + hour() + "-" + minute();
  println("Today's date is " + tiempoInicio);

  // Guardamos registro de datos
  output = createWriter("datosSSD-" + tiempoInicio + ".txt");

  // Nos conectamos a un puerto serial
  String portOne = Serial.list()[0];

  // open the ports:
  myPort = new Serial(this, portOne, 115200);

  // ------- Congfiguracion grafica IMU --------
  grafica1.tamano(tamX, tamY);
  grafica1.paso(tamX/10, tamY/3);                        // Numero de marcas
  grafica1.posicion(PgraficaX, PgraficaY);
  grafica1.setmalla(false);
}

//ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ Dibujar ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
void draw() {
  // Preparamos Fondo:
  background(240);

  // Grafica señanes IMU
  // Graficamos Textos Señales
  textSize(pasoTextGraf);
  grafica1.graficar(datosS[0]);
  for (int k=1; k<(sepa.length); k++) {
    fill(colores[k]);
    stroke(colores[k]);
    text(nombres[k] + "" + scalaD[k], posTextGrafI_X, PgraficaY + k*pasoTextGraf+pasoTextGraf);
    grafica1.linea(datosS[k], colores[k]);
  }


  /*  for (int i=2; i<(nombres.length); i=i+1) {
   fill(colores[i]);
   }
   */

  // Capturamos datos enviados por el puerto serial
  if (conexion) {    
    while (myPort.available() > 1) {
      serial_data(myPort);
    }
  }

  for (int i=0; i<(nombres.length); i++) {
    fill(colores[i]);
    text(nombres[i], posTextGrafI_X, PgraficaY + i*pasoTextGraf+pasoTextGraf);
  }

  // Fin dibujar
}