

void actualizarpuertos() { 
  //list.clear();
  for (int i=0; i<(Serial.list().length); i++) { 
    //ListBoxItem lbi = list.addItem(Serial.list()[i], i);
    //lbi.setColorBackground(color(0,0,0));
  }
}

void serial_data(Serial p) {
  String datos = myPort.readStringUntil('\r');    // Leemos linea entrante

  if (datos != null) {                            // Si es valida la linea
    datosLog = "";
    try {                                         // intentamos procesarla
      //print(datos);

      String[] datoss = split(datos, sepa[0]);                 // Extraemos Separador inicial = datoss[0]

      for (int j=0; j<(sepa.length)-1; j=j+1) {                // Para cada dato entre separador
        //println("Datos: " + datoss[1] + " I: " + j);
        datoss = split(datoss[1], sepa[j+1]);                  // Extraemos datos usando los separadores
        datosS[j][c] = float(datoss[0])*scalaD[j];             // Escalamos datos
        datosD[j][c] = float(datoss[0]); 
        datosLog =  datosLog + datoss[0] + "\t";               // Guardamos datos
        //println("DCap: " + datosS[j][c] + "");

        // Revisamos la escala
        // && nEscala < scalaD[j] && nEscala > 0 

        if (j>0 && (datosS[j][c] > 1 || datosS[j][c] < -1)) {
          float nEscala =  (1)/abs(float(datoss[0]));  // Calculamos nueva
          scalaD[j] =  nEscala;                        // Guardamos nueva escala
          datosS[j][c] = datosS[j][c]*scalaD[j];       // Corregimos dato
          //println("NEscala: " + nEscala);
        }  // Fin ajuste Escala

        if (j==(sepa.length)-2) {
          // cargamos ultimo dato (Corriente)
          datosS[j+1][c] = float(datoss[1])*scalaD[j+1];
          if (datosS[j+1][c] > 1 || datosS[j+1][c] < -1) {
            float nEscala =  (1)/abs(float(datoss[1]));      // Calculamos nueva
            scalaD[j+1] =  nEscala;                          // Guardamos nueva escala
            datosS[j+1][c] = datosS[j+1][c]*scalaD[j+1];     // Corregimos dato
            //println("NEscala: " + nEscala);
          }  // Fin ajuste Escala
        } // fin ultimo dato
        
      }    // fin datos entre separadores

      // = float(datoss[1]);
      datosLog =  datosLog + datoss[1] + "\n";              // guardamo datos en disco
      output.println(datosLog);
      //println("DCap: " + datosS[sepa.length-1][c] + "");

      //mostramos linea de datos capturada
      for (int j=0; j<(sepa.length); j=j+1) {
        print(sepa[j] +""+ datosS[j][c]);
      }
      println();
      c = (c+1) % tamX;                                     // Saltamos a guardar el sigueinte dato
    }
    catch(Exception e) {
      println("Err: " + e.toString());
    }
  }
}