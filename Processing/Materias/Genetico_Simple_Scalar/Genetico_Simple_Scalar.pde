// por: Julian Galvez Serna y John Byron Buitrago Paniagua
//---------------------------------librerias--------------------------------------
import java.io.*;

// ------------------- variables malla ------------------- 
// tamaño de la grafica de la malla

int d_ini = 50;            // posicion donde inicia el pintado de los objetos
int sep = 25;              // separacion entre nodos
int tam = 20;              // tamaño de los nodos


// ------------------- variables ejecusion simulacion ------------------- 
String ruta = "/home/piaktipik/Desktop/SoCs/Practica2/Lab/";  //ruta archivos

String commandToRun = "./sc_simooo.sh";    // .sh que corre la simulacion
//String commandToRun = "which awk"; 

String archivoParametros = "vbles_vals.sh";

File workingDir = new File(ruta);   // where to do it - should be full path
String returnedValues;              // value to return any results

float offset = -0.0210;

// -------------------- variables algoritmo genetico -------------------- 
// Elegimos un tamaño de poblacion de 10
byte tamPoblacion = 10;
genotipo poblacion[] = new genotipo[tamPoblacion];  // poblacion de genotipos;
byte ordenPoblacion[] = new byte[tamPoblacion];
byte primeraPoblacion = 0;

// cada genotipo es un conjunto de configuraciones,
// en base a una asignacion y evaluacion de las capacidades y los grados de los pacientes

// ------------ lista posibles parametros -----------------
String parametros[] ={"fetch_ifqsize", "decode_width", "issue_width", "issue_inorder", "issue_wrongpath", 
  "ruu_size", "commit_width", "res_ialu", "res_imult", "res_fpalu", "res_fpmult", 
  "fetch_mplat", "bpred", "bpred_bimod", "bpred_2lev_l1sz", "bpred_2lev_l2sz", 
  "bpred_2lev_hist_sz", "lsq_size", "cache_dl1", "cache_dl1lat", "cache_dl2", 
  "cache_dl2lat", "cache_il1", "cache_il1lat", "cache_il2", "cache_il2lat", 
  "cache_flush", "cache_icompress", "mem_lat_fs_chunk", "mem_lat_int_chunk", 
  "mem_width", "tlb_itlb", "tlb_dtlb", "tlb_lat", "res_memport", "seed", "bugcompat"};

int nPara = parametros.length;

String valorEjemploParametros[] = {"4", "4", "4", "false", "false", "16", "16", "4", "4", "4", "4", "3", "nottaken", "2048", 
  "1", "1024", "8", "8", "dl1:128:32:4:l", "1", "ul2:1024:64:4:l", "6", "il1:512:32:1:l", 
  "1", "dl2", "6", "false", "false", "18", "2", "8", "itlb:16:4096:4:l", "dtlb:32:4096:4:l", 
  "30", "4", "1", "false"};


// ------------ llenamos todas las posibilidades ------------ 
String posiblesParametros[][] = {
  {"1","2","4","8","16","32","64","128","256","512","1024"}, 
  {"1","2","4","8","16","32","64","128","256","512","1024"}, 
  {"1","2","4","8","16","32","64","128","256","512","1024"}, 
  {"false","true"}, 
  {"true","false"}, 
  
  {"2","4","8","16","32","64","128","256","512","1024"}, 
  {"1","2","3","4","5","6","7","8"}, 
  {"1","2","3","4","5","6","7","8"}, 
  {"1","2","3","4","5","6","7","8"}, 
  {"1","2","3","4","5","6","7","8"}, 
  
  {"1","2","3","4","5","6","7","8"}, 
  {"3"}, 
  {"taken","nottaken","perfect","bimod"}, 
  {"1"}, 
  {"1"}, 
  
  {"1"}, 
  {"1"}, 
  {"1","2","4","8","16","32","64","128","256","512","1024"},
  {"dl1:128:32:4:l"}, 
  {"1"}, 
  
  {"ul2:1024:64:4:l"}, 
  {"6"}, 
  {"il1:512:32:1:l"}, 
  {"1"}, 
  {"dl2"}, 
  
  {"6"}, 
  {"false","true"}, 
  {"false","true"}, 
  {"18"}, 
  {"2"}, 
  
  {"1","2","4","8","16","32","64","128","256","512","1024"},
  {"itlb:16:4096:4:l"}, 
  {"dtlb:32:4096:4:l"}, 
  {"30"}, 
  {"1","2","3","4","5","6","7","8"},
  
  {"1"}, 
  {"false"}
};

int tPosiblesParametros[]={
11,11,11,1,1,
7,8,8,8,8,
8,0,1,0,0,
0,0,8,0,0,
0,0,0,0,0,
0,1,1,0,0,
8,0,0,0,8,
0,0
};
// ------------ variables manejo archivos -----------------
PrintWriter output;
// ------------ variables generales -----------------
// enteros para los ciclos for
int i = 0, j = 0;
int npol = 0;
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

//-----------------------------------------configuracion--------------------------------------------- 
void setup() {
  // tamaño de la ventana
  size(400, 100);
  frameRate(10);             // velocidad de actualizacion

  // give us some info:
  println("Running command: " + commandToRun);
  println("Location:        " + workingDir);
  println("---------------------------------------------\n");
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
  text("Por: Julian Galvez Serna y Byron Buitrago Paniagua", 3*sep+sep/2, -7*sep/5);
  //pintamos y numeramos los nodos
}//fin draw


//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void mousePressed() {
  // obtengo el valor entero de la posicion
  if (mouseX>50 && mouseX<70 && mouseY>0 && mouseY<20) {
      correrGenetico();
    }
  if (mouseX>75 && mouseX<95 && mouseY>0 && mouseY<20) {
    //escribirParametros();
    //escribirParametros(poblacion[8].valDParametros);
    float ipc = correrSimulacion();
    println(ipc);
  }
  if (mouseX>100 && mouseX<120 && mouseY>0 && mouseY<20) {
    for (int i=1;i<20;i++){
      correrGenetico();
    }
  }
  println(mouseX + ", " + mouseY);
}

void keyPressed() {
  if (key == CODED) {
    if (keyCode == UP) {
      //posY -= 10;
    }
  }
}

//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz
//zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz

/*
1024 16 64 false true 32 7 7 4 7 1 3 bimod 2048 1 1024 8 8 dl1:128:32:4:l 1 ul2:1024:64:4:l 6 il1:512:32:1:l 1 dl2 6 false true 18 2 8 itlb:16:4096:4:l dtlb:32:4096:4:l 30 4 1 false

16 512 4 false false 128 6 3 7 4 2 3 bimod 2048 1 1024 8 8 dl1:128:32:4:l 1 ul2:1024:64:4:l 6 il1:512:32:1:l 1 dl2 6 false true 18 2 8 itlb:16:4096:4:l dtlb:32:4096:4:l 30 7 1 false

//*/

/*

 //#######DATAPATH##########
 String fetch_ifqsize="4";
 String decode_width="4";
 String issue_width="4";
 String issue_inorder="false";
 String issue_wrongpath="false";
 String ruu_size="16";
 String commit_width="16";
 // #########FUNCTIONAL UNITS######
 String res_ialu="4";
 String res_imult="4";
 String res_fpalu="4";
 String res_fpmult="4";
 // ########BRANCH PREDICTOR########
 String fetch_mplat="3";
 String bpred="nottaken";
 String bpred_bimod="2048";
 String bpred_2lev_l1sz="1";
 String bpred_2lev_l2sz="1024";
 String bpred_2lev_hist_sz="8";
 // #############MEMORY#######
 String lsq_size="8";
 String cache_dl1="dl1:128:32:4:l";
 String cache_dl1lat="1";
 String cache_dl2="ul2:1024:64:4:l";
 String cache_dl2lat="6";
 String cache_il1="il1:512:32:1:l";
 String cache_il1lat="1";
 String cache_il2="dl2";
 String cache_il2lat="6";
 String cache_flush="false";
 String cache_icompress="false";
 String mem_lat_fs_chunk="18";
 String mem_lat_int_chunk="2";
 String mem_width="8";
 String tlb_itlb="itlb:16:4096:4:l";
 String tlb_dtlb="dtlb:32:4096:4:l";
 String tlb_lat="30";
 String res_memport="4";
 // ########OTHERS########
 String seed="1";
 String bugcompat="false";
 
 
fetch_ifqsize=512
decode_width=1024
issue_width=16
issue_inorder=false
issue_wrongpath=true
ruu_size=128
commit_width=7
res_ialu=8
res_imult=3
res_fpalu=2
res_fpmult=1
fetch_mplat=3
bpred=taken
bpred_bimod=1
bpred_2lev_l1sz=1
bpred_2lev_l2sz=1
bpred_2lev_hist_sz=1
lsq_size=64
cache_dl1=dl1:128:32:4:l
cache_dl1lat=1
cache_dl2=ul2:1024:64:4:l
cache_dl2lat=6
cache_il1=il1:512:32:1:l
cache_il1lat=1
cache_il2=dl2
cache_il2lat=6
cache_flush=false
cache_icompress=false
mem_lat_fs_chunk=18
mem_lat_int_chunk=2
mem_width=128
tlb_itlb=itlb:16:4096:4:l
tlb_dtlb=dtlb:32:4096:4:l
tlb_lat=30
res_memport=4
seed=1
bugcompat=false
 
 
 */
 
 