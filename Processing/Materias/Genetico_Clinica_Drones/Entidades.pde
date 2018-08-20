

class hospital {
  // posicion
  byte x;
  byte y;
  // nivel 
  byte nivel;
  // capacidad
  byte capacidad_max;

  hospital() {
    this.x = 0;
    this.y = 0;
    this.nivel = 0;
    //this.capacidad_max = 1;
  }
}

class acidentado {
  // posicion
  public byte x;
  public byte y;
  // nivel 
  public byte nivel;

  acidentado() {
    this.x = byte(255);
    this.y = byte(255);
    this.nivel = 0;
  }
}

class genotipo {
  ArrayList<acidentado> acidentados_gen = new ArrayList<acidentado>();
  ArrayList<hospital> hospitales_gen = new ArrayList<hospital>();

  // desempeño
  int distancia;
  // pertienencia
  int pertinencia;
  // suma de distancia y pertinencia
  int aptitud = 0;

  // constructor
  genotipo() {
    distancia = 0; 
    pertinencia = 0;
    aptitud = 0;
  }

  void evaluar() {
    distancia = 0;
    pertinencia = 0; 
    for (int a = 0; a<acidentados_gen.size(); a++) {
      // suma de la diferencia de dinstancias entre cada hospital - acidentado
      // distancia manhattan
      distancia += abs(acidentados_gen.get(a).x-hospitales_gen.get(a).x)+abs(acidentados_gen.get(a).y-hospitales_gen.get(a).y); 
      // suma de la diferencia de niveles entre cada hospital - acidentado
      pertinencia += abs(acidentados_gen.get(a).nivel-hospitales_gen.get(a).nivel);
    }
    aptitud = distancia + pertinencia;
    println("Dist: " + distancia + ", Pert: " + pertinencia + ", Apti: " + aptitud);
  }
}
// ------------------------------------------------------------- funciones ------------------------------------------------------------------------
void agregarItem(byte selec, byte px, byte py) {
  if (selec == aciden) {
    acidentado nuevoAccidentado = new acidentado();
    nuevoAccidentado.x = px;
    nuevoAccidentado.y = py;
    nuevoAccidentado.nivel = 1;
    //println("NA: " + nuevoAccidentado.toString());
    acidentados.add(nuevoAccidentado);
    //mostrarAcidentado();
  } else if (selec == hospi) {
    hospital nuevoHospital = new hospital();
    nuevoHospital.x = px;
    nuevoHospital.y = py;
    nuevoHospital.nivel = 1;
    hospitales.add(nuevoHospital);
    //mostrarHospital();
  }
}
void removerItem(byte selec, byte px, byte py) {
  if (selec == aciden) {
    acidentado viejoAccidentado = buscarAcidentado(px, py);
    acidentados.remove(viejoAccidentado);
    //mostrarAcidentado();
    //println("NA: " + nuevoAccidentado.toString());
  } else if (selec == hospi) {
    hospital viejoHospital = buscarHospital(px, py);
    hospitales.remove(viejoHospital);
    //mostrarHospital();
  }
}

acidentado buscarAcidentado(byte px, byte py) {
  for (int i = 0; i<acidentados.size(); i++) {
    if (acidentados.get(i).x==px && acidentados.get(i).y==py) {
      return acidentados.get(i);
    }
  }
  return new acidentado();
}
void mostrarAcidentado() {
  for (int i = 0; i<acidentados.size(); i++) {
    System.out.println("Acidentado # " + i + ", " + acidentados.get(i));
  }
}

hospital buscarHospital(byte px, byte py) {
  for (int i = 0; i<hospitales.size(); i++) {
    if (hospitales.get(i).x==px && hospitales.get(i).y==py) {
      return hospitales.get(i);
    }
  }
  return new hospital();
}
void mostrarHospital() {
  for (int i = 0; i<hospitales.size(); i++) {
    System.out.println("Hospital # " + i + ", " + hospitales.get(i));
  }
}