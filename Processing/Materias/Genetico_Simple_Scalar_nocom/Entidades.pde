
class genotipo {
  String valDParametros[];
  // desempeño (instructions per cycle)
  float ipc;

  // constructor
  genotipo() {
    ipc = 0;
    valDParametros = new String[nPara];
  }

  void evaluar() {
    escribirParametros(valDParametros);
    ipc = correrSimulacion();
  }
}


void mostrarConfiguraciones() {
  for (int i = 0; i<poblacion.length; i++) {
    //System.out.println("Configuracion # " + i + ", " + poblacion[i]);
  }
}

void escribirParametros(String valDParametros[]) {
  // escribimos los parametros en un archivo

  /*//mostramos parametros
  for (i = 0; i<parametros.length; i++) {
    println(parametros[i] + "=" + valDParametros[i]);
  }//*/

  // imprimimos informacion a archivo
  // Create a new file in the sketch directory
  output = createWriter(ruta+archivoParametros); 

  for (i = 0; i<parametros.length; i++) {
    output.println(parametros[i] + "=" + valDParametros[i]); // imprimimos configuraciones
  }
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  output.close(); // Finishes the file
}


float correrSimulacion() {
  //println("--------------------Corriendo Simulacion");
  try {
    Process p = Runtime.getRuntime().exec(commandToRun, null, workingDir);
    // variable to check if we've received confirmation of the command
    int i = p.waitFor();
    // if we have an output, print to screen
    if (i == 0) {
      // BufferedReader used to get values back from the command
      BufferedReader stdInput = new BufferedReader(new InputStreamReader(p.getInputStream()));
      // read the output from the command
      while ( (returnedValues = stdInput.readLine ()) != null) {
        //println(returnedValues);
        String valores[] = returnedValues.split(":");
        if (valores.length>1) {
          //println(returnedValues);
          //print("v0= " + valores[0]);
          //println(" v1= " + valores[1]);
          if (valores[0].equals("IPC")) {
            //println("IPC retornado = " + valores[1]);
            //println("IPC ajustado = " + (float(valores[1])+offset));
            //println("---------------------------------------------");
            return float(valores[1])+offset;
          }
        }
      }
    }

    // if there are any error messages but we can still get an output, they print here
    else {
      BufferedReader stdErr = new BufferedReader(new InputStreamReader(p.getErrorStream()));
      // if something is returned (ie: not null) print the result
      while ( (returnedValues = stdErr.readLine ()) != null) {
        //println(returnedValues);
      }
    }
  }
  // if there is an error, let us know
  catch (Exception e) {
    //println("Error running command!");  
    //println(e);
  }
  // when done running command, quit
  //println("DONE!");
  //println("\n---------------------------------------------");
  return 0;
}