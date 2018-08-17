
void actulizar(){
  actulizarSensoresDistancia();
  actualizarSensoresSuelo();
  actulizarSensoresColision();
  actualizarMovimiento();
}

void escaparHaciaAtras(){
  if(medidaDistanciaDerecha <= medidaDistanciaIzquierda){movimiento(Izquierda);}
  else{movimiento(Derecha);}
}

void revisarSerial(){
  if(Serial.available() > 0){
    char dato  = Serial.read();
    if(depuracion)Serial.println(dato);
    mover(dato);// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  }
}

void revisarSerialUltrasonidos(){
  if(Serial.available() > 0){
    char dato  = Serial.read();
    if(depuracion)Serial.println(dato);
    MostrarUltrasonido(dato);// dependiendo del valor recibido, cambio el ultrasonido que esta siendo leido
  }
}

void escucharRaspberry(){
  while(Serial1.available() > 0){
    char dato  = Serial1.read();
    if(depuracion)Serial.print(dato); // muestro todo lo que esta enviando la raspberry
    // si la raspberry esta enviando datos y no el log del arranque o cierre
    if(RaspberryHabla()) accion(dato);// dependiendo del valor recibido, ejecuto una secuencia para mover el vehiculo
  }
}

void accion(char dato){
  // modificamos el movimiento
  switch(dato){
    case 'e'        : ver = EvasorObstaculos;
    break;case 's'  : ver = SeguidorObjetos;
    break;case 'r'  : ver = SensoresDistanciaRadar;
    break;case 'a'  : ver = ControlSerialMotores;movimiento(Adelante);
    break;case 'h'  : ver = ControlSerialMotores;movimiento(Atras);
    break;case 'd'  : ver = ControlSerialMotores;movimiento(Derecha);
    break;case 'i'  : ver = ControlSerialMotores;movimiento(Izquierda);
    break;case 'p'  : ver = ControlSerialMotores;movimiento(Detenido);
    break;
  }
  
}


bool RaspberryHabla(){
  if(digitalRead(pinRaspberryHabla)){
    if(depuracion)Serial.println(" :R hablo");
    return true;
  }else{
    return false;
  }
}

