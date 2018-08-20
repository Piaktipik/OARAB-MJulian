
// algoritmo genetico
void genetico() {
  println(" ----------------------------- Corriendo Genetico ----------------------------- ");

  //poblacion
  boolean noAsignado = true;
  long tiempo;
  hospital posibleHospital;

  if (verPoblacion == 0) {
    println(" ----------------------------- Entidades ----------------------------- ");
    mostrarAcidentado();
    mostrarHospital();
    // funcion de generacion de nueva poblacion
    // asignamos aleatoriamente un conjunto de pares accidentado-hospital
    println(" ----------------------------- primera poblacion ----------------------------- ");
    println("Generamos primera poblacion aleatoria");

    for (int p = 0; p<poblacion.length; p++) {
      //println("Pob: " + p);
      genotipo individuo = new genotipo();  // usado para crear lo pares a guardar en cada individuo

      for (int i = 0; i<acidentados.size(); i++) {
        //println("Ind: "+i);
        individuo.acidentados_gen.add(acidentados.get(i));
        // asignamos a cada paciente un hospital
        noAsignado = true; 
        tiempo = millis();

        while (noAsignado && (millis()<tiempo+1000)) {
          posibleHospital = hospitales.get(int(random(hospitales.size())));  //tomamos un hospital aleatoriamente
          //println("Hos: "+ posibleHospital);

          if (posibleHospital.nivel >= acidentados.get(i).nivel) {
            individuo.hospitales_gen.add(posibleHospital);
            noAsignado = false;
          }
        }
      }
      //guardamos el individuo
      poblacion[p] = individuo;
    }
    verPoblacion=1;
  }
  println(" ----------------------------- Poblacion ----------------------------- ");
  mostrarPoblacion();

  // ---------------------------------------------- iniciamos el proceso de evolucion ----------------------------------------------
  // seleccion mejores genes
  // se usa como funcion de salud, la suma de la distancia y pertinencia (de todos los accidentados a hospitales) 
  println(" ----------------------- Evaluar poblacion");
  for (int p = 0; p<poblacion.length; p++) {
    poblacion[p].evaluar();
  }
  // se ordenan de mas optimos a menos optimos (usamos vector de orden)
  println(" ----------------------- Ordenar Poblacion");
  ordenarPoblacion(); // me entrega en el vector ordenPoblacion[p] el orden de la poblacion
  println(" ----------------------------- Cruce Poblacion ----------------------------- ");
  // reproduccion (cruce) -> como decidir cuantos mezclar(intercambiar sus hospitales en pares) y cuantos remplazar por nueva poblacion
  // se usa un porcentaje para cruzar de maximo 60% de la poblacion donde los hijos remplazan el 30% de la peor poblacion
  float crucePoblacion = 0.6;
  int nCruces = round((tamPoblacion*crucePoblacion)/2)*2; //aseguro pares de cruce de a 2
  genotipo nuevosCruces[] = new genotipo[nCruces/2];  // nueva poblacion de cruces;

  println("Num Cruces: "+ nCruces/2);

  int c = 0;
  // para cada pareja a cruzar
  for (int p = 0; p<nCruces; p+=2) {
    // cruzamos los mejores individuos generando nCruces/2 individuos nuevos para remplazar las peores.
    // usamos los mismos accidentados y cruzamos los hospitales
    genotipo individuo = new genotipo();  // usado para crear los pares a guardar en cada individuo
    for (int q=0; q<poblacion[ordenPoblacion[p]].hospitales_gen.size(); q++) {

      //println("Cruce: "+ c + ", P:" + p + ", Q:" + q +", pob:" + poblacion[ordenPoblacion[p]].acidentados_gen.get(q));

      individuo.acidentados_gen.add(poblacion[ordenPoblacion[p]].acidentados_gen.get(q));
      individuo.hospitales_gen.add(poblacion[ordenPoblacion[p+(q%2)]].hospitales_gen.get(q));
    }
    println("Cruce: "+ c + ", Padre:" + poblacion[ordenPoblacion[p]] + ", Madre:" + poblacion[ordenPoblacion[p+1]] +", Hijo:" + individuo);
    // verificamos la validez de la asignacion, si un paciente quedo en un nivel de hospital inferior lo cambiamos
    for (int i = 0; i<individuo.hospitales_gen.size(); i++) {
      // revisamos cada hospital
      if (individuo.acidentados_gen.get(i).nivel > individuo.hospitales_gen.get(i).nivel) {
        noAsignado = true; 
        tiempo = millis();
        while (noAsignado && (millis()<tiempo+1000)) {
          posibleHospital = hospitales.get(int(random(hospitales.size())));  //tomamos un hospital aleatoriamente
          if (posibleHospital.nivel >= individuo.acidentados_gen.get(i).nivel) {
            individuo.hospitales_gen.set(i, posibleHospital);
            noAsignado = false;
          }
        }
      }
    } // fin revision
    nuevosCruces[c] = individuo; // guardamos el individuo cruzado y revisado
    //ordenPoblacion[p];
    c++;
  } // fin cruce
  println(" ----------------------------- Mutar Poblacion ----------------------------- ");
  // mutacion -> cuanto mutar, como(cambiar hospital por aleatorio para par que sobrepase el humbral)
  // si un numero aleatorio supera un umbral definido se muta(cambio de hospital por uno valido aleatorio)
  float mutacion = 0.2;
  for (int p = 0; p<(nCruces/2); p++) {
    for (int i = 0; i<acidentados.size(); i++) {
      // si un valor aleatorio supera el valor de mutacion, mutamos el hospital
      if (random(1)<mutacion) {
        noAsignado = true; 
        tiempo = millis();
        print("Muto: "+nuevosCruces[p] + ", ac: " + nuevosCruces[p].acidentados_gen.get(i) + ", vh: " + nuevosCruces[p].hospitales_gen.get(i));
        while (noAsignado && (millis()<tiempo+1000)) {
          posibleHospital = hospitales.get(int(random(hospitales.size())));  //tomamos un hospital aleatoriamente

          if (posibleHospital.nivel >= nuevosCruces[p].acidentados_gen.get(i).nivel) {
            nuevosCruces[p].hospitales_gen.set(i, posibleHospital);
            println(", nh: " + nuevosCruces[p].hospitales_gen.get(i));
            noAsignado = false;
          }
        }
      }
    }
  }

  // cambiamos los peores individuos por la nueva poblacion
  println(" ----------------------------- Actualizamos individuos ----------------------------- ");
  for (int p = 0; p<(nCruces/2); p++) {
    int posp = ordenPoblacion[(ordenPoblacion.length-(p+1))]; // posicion individuo menos apto
    println("Orden: " + posp + " Pobv: " + poblacion[posp] + ", Apti: " + poblacion[posp].aptitud + " Pobvn: " + nuevosCruces[p]);
    poblacion[posp] = nuevosCruces[p];
  }
}



void mostrarPoblacion() {
  for (int p = 0; p<poblacion.length; p++) {
    println("Pob: " + p + ", : "+ poblacion[p]);
    for (int i = 0; i<acidentados.size(); i++) {
      println("Ind: "+poblacion[p].acidentados_gen.get(i) + ", Hos: " + poblacion[p].hospitales_gen.get(i));
      // asignamos a cada paciente un hospital
    }
    println("------------------------------");
  }
}

void ordenarPoblacion() {
  // se inicia el ordenamiento
  for (int p = 0; p<ordenPoblacion.length; p++) {
    ordenPoblacion[p]=byte(p);
  } 

  for (int p = 0; p<ordenPoblacion.length; p++) {
    for (int q = p; q<ordenPoblacion.length; q++) {
      if (poblacion[ordenPoblacion[p]].aptitud>poblacion[ordenPoblacion[q]].aptitud) {
        byte aux = ordenPoblacion[p];
        ordenPoblacion[p] = ordenPoblacion[q];
        ordenPoblacion[q] =  aux;
      }
    }
  }
  for (int p = 0; p<ordenPoblacion.length; p++) {
    println("Orden: " + ordenPoblacion[p] + ", Dista: " + poblacion[ordenPoblacion[p]].distancia + ", Perti: " + poblacion[ordenPoblacion[p]].pertinencia + ", Apti: " + poblacion[ordenPoblacion[p]].aptitud);
  }
}