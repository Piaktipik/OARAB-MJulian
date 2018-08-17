
void configurar_radio_receptor(){
   printf_begin();
  radio.begin();
  // RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(100);
  radio.enableDynamicPayloads();
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  // Start the radio listening for data
  radio.startListening();
  radio.printDetails();
}

String leer_datos_cortos(){
  uint8_t len;
  char data[buffer_datos_recividos];
  if(radio.available()){
    while (radio.available()) {             // While there is data ready
      len = radio.getDynamicPayloadSize();
      radio.read(&data,len);              // Get the payload
    }
    if(depuracion){
      Serial.print("leido: ");
      Serial.print(len);
      Serial.print(" ");
      Serial.print(String(data).substring(0, len));
    }
    return String(data).substring(0, len);
  }else{
    return "";
  }
}

String leer_datos(){
  uint8_t len;
  char data[buffer_datos_recividos];
  if(radio.available()){
    while (radio.available()) {             // While there is data ready
      len = radio.getDynamicPayloadSize();
      radio.read(&data,len);              // Get the payload
    }
    // si llega un -###- quiere decir que el dato esta conformado por muchos paquetes, procedemos a leerlos
    String mensaje = String(data).substring(0, len);
    int numeroPaquetes = 0 ;
    if (mensaje.substring(0, 1) == "-"){
      if(depuracion){
        Serial.print("datos: ");
        Serial.print(mensaje);
      }
      // son varios paquetes, obtengo el numero de paquetes
      numeroPaquetes = obtenerNumeroPaquetes(mensaje);
      if(depuracion){
        Serial.print(" paquetes a leer: ");
        Serial.println(numeroPaquetes);
      }
    } // fin primer caracter -,
    
    if(numeroPaquetes > 0 ){
      mensaje = "";
      int contadorPaquetesRecibidos = 0;
      unsigned long tiempoInicioMensaje = millis();
      unsigned long maximoTiempoEspera = 5000;     // lo que esperamos antes de cancelar el paquete
      while(contadorPaquetesRecibidos < numeroPaquetes && millis() < (tiempoInicioMensaje + maximoTiempoEspera)){
        if(radio.available()){
          while(radio.available()) {             // While there is data ready
            len = radio.getDynamicPayloadSize();
            radio.read(&data,len);              // Get the payload
          }
          // armamos el paquete
          mensaje += String(data).substring(0, len);
          contadorPaquetesRecibidos++;
        }
      } // fin while varios paquetes

      if(depuracion){
        Serial.print("leido: ");
        Serial.print(mensaje);
      }
      // si el paquete no llego completo lo descartamos
      if(contadorPaquetesRecibidos < numeroPaquetes){
        if(depuracion){
          Serial.print("-tiempo-");
        }
        return "";
      }else{
        return mensaje;
      }
    }// fin numero de paquetes mayor a 0
    if(depuracion){
      Serial.print("fallo, datos: ");
      Serial.print(mensaje);
    }
  }
  // si hasta este punto no se retorno un dato valido, no se pudo leer nada
  return "";
}

int obtenerNumeroPaquetes(String string){
  String numero = "";
  for(int i = 0; i<string.length();i++){
    if (string[i] != '-'){
      numero += string[i];
    }
  }
  return numero.toInt();
}
