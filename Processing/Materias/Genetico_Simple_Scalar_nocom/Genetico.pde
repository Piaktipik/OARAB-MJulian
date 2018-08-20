
// algoritmo genetico
void correrGenetico() {
  //println(" ----------------------------- Corriendo Genetico ----------------------------- ");

  //configuracion posibleConfiguracion;

  if (primeraPoblacion == 0) {
    // funcion de generacion de nueva poblacion
    // asignamos aleatoriamente un conjunto de configuraciones
    println(" ----------------------------- primera poblacion ----------------------------- ");
    //println("Generamos primera poblacion aleatoria");

    for (int p = 0; p<poblacion.length; p++) {
      genotipo individuo = new genotipo();  // usado para crear lo pares a guardar en cada individuo

      for (int i = 0; i<nPara; i++) {
        individuo.valDParametros[i] = posiblesParametros[i][int(random(tPosiblesParametros[i]))];;
      }
      
      //guardamos el individuo
      poblacion[p] = individuo;
    }
    primeraPoblacion=1;
  }
  //println(" ----------------------------- Poblacion ----------------------------- ");
  //mostrarPoblacion();

  // ---------------------------------------------- iniciamos el proceso de evolucion ----------------------------------------------
  // seleccion mejores genes
  // se usa como funcion de salud, el IPC
  //println(" ----------------------- Evaluar poblacion");
  for (int p = 0; p<poblacion.length; p++) {
    //println("--------------------Evaluando individuo: " + p + " --------------------");
    poblacion[p].evaluar();
  }

  // se ordenan de mas optimos a menos optimos (usamos vector de orden)
  //println(" ----------------------- Ordenar Poblacion");
  ordenarPoblacion(); // me entrega en el vector ordenPoblacion[p] el orden de la poblacion

  //println(" ----------------------------- Cruce Poblacion ----------------------------- ");
  // reproduccion (cruce) -> como decidir cuantos mezclar(intercambiar propiedades) y cuantos remplazar por nueva poblacion
  // se usa un porcentaje para cruzar de maximo 60% de la poblacion donde los hijos remplazan el 30% de la peor poblacion
  float crucePoblacion = 0.6;
  int nCruces = round((tamPoblacion*crucePoblacion)/2)*2; //aseguro pares de cruce de a 2
  genotipo nuevosCruces[] = new genotipo[nCruces/2];  // nueva poblacion de cruces;
  //println("Num Cruces: "+ nCruces/2);
  int c = 0;
  // para cada pareja a cruzar
  for (int p = 0; p<nCruces; p+=2) {
    // cruzamos los mejores individuos generando nCruces/2 individuos nuevos para remplazar las peores.
    // usamos los mismos accidentados y cruzamos los hospitales
    genotipo individuo = new genotipo();  // usado para crear los pares a guardar en cada individuo

    // cruzamos dos individuos en uno nuevo
    for (int q=0; q<nPara-1; q++) {
      //println("Cruce: "+ c + ", P:" + p + ", Q:" + q +", pob:" + poblacion[ordenPoblacion[p]].acidentados_gen.get(q));
      individuo.valDParametros[q] = poblacion[ordenPoblacion[p]].valDParametros[q]; // valor madre
      individuo.valDParametros[q+1] = poblacion[ordenPoblacion[p+(q%2)]].valDParametros[q+1]; //valor padre
    }

    //println("Cruce: "+ c + ", Padre:" + poblacion[ordenPoblacion[p]] + ", Madre:" + poblacion[ordenPoblacion[p+1]] +", Hijo:" + individuo);

    nuevosCruces[c] = individuo; // guardamos el individuo cruzado y revisado
    //ordenPoblacion[p];
    c++;
  } // fin cruce


  //println(" ----------------------------- Mutar Poblacion ----------------------------- ");
  // mutacion -> cuanto mutar, como (cambiar parametro si aleatorio sobrepase el humbral)
  //(cambio el parametro por uno valido aleatorio)
  float mutacion = 0.2;

  for (int p = 0; p<(nCruces/2); p++) {
    for (int i = 0; i<nPara; i++) {
      // si un valor aleatorio supera el valor de mutacion, mutamos el parametro
      if (random(1)<mutacion) {

        //print("Muto: "+nuevosCruces[p] + "pv: " + nuevosCruces[p].valDParametros[i]);
        nuevosCruces[p].valDParametros[i] = posiblesParametros[i][int(random(tPosiblesParametros[i]))];  //tomamos un parametro aleatoriamente
        //println(", pn: " + nuevosCruces[p].valDParametros[i]);
      }
    }
  }

  // cambiamos los peores individuos por la nueva poblacion
  //println(" ----------------------------- Actualizamos individuos ----------------------------- ");
  for (int p = 0; p<(nCruces/2); p++) {
    int posp = ordenPoblacion[(ordenPoblacion.length-(p+1))]; // posicion individuo menos apto
    //println("Orden: " + posp + " Pobv: " + poblacion[posp] + ", IPC: " + poblacion[posp].ipc + " Pobvn: " + nuevosCruces[p]);
    poblacion[posp] = nuevosCruces[p];
  }
  npol++;
}



void mostrarPoblacion() {
  for (int p = 0; p<poblacion.length; p++) {
    //println("Pob: " + p + ", : "+ poblacion[p]);
    //print("Parametros:");
    for (int i = 0; i<nPara; i++) {
      //print(" " + poblacion[p].valDParametros[i]);
      // asignamos a cada paciente un hospital
    }
    //println("");
    //println("------------------------------");
  }
}

void ordenarPoblacion() {
  // se inicia el ordenamiento
  for (int p = 0; p<ordenPoblacion.length; p++) {
    ordenPoblacion[p]=byte(p);
  } 

  for (int p = 0; p<ordenPoblacion.length; p++) {
    for (int q = p; q<ordenPoblacion.length; q++) {
      if (poblacion[ordenPoblacion[p]].ipc<poblacion[ordenPoblacion[q]].ipc) {
        byte aux = ordenPoblacion[p];
        ordenPoblacion[p] = ordenPoblacion[q];
        ordenPoblacion[q] =  aux;
      }
    }
  }
  float saludpoblacion = 0;
  for (int p = 0; p<ordenPoblacion.length; p++) {
    //println("Orden: " + ordenPoblacion[p] + ", IPC: " + poblacion[ordenPoblacion[p]].ipc);
    saludpoblacion += poblacion[ordenPoblacion[p]].ipc;
  }//*/
  println("" +npol+ ","+ saludpoblacion/ordenPoblacion.length);
  
}