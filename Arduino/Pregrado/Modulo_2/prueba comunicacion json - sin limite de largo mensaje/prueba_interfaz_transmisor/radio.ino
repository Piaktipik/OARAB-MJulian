
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

void configurar_radio_transmisor(){
  radio.begin();
  // RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  radio.enableDynamicPayloads();
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  // First, stop listening so we can talk.
  radio.stopListening();
}

bool enviar_datos(String string){
  // convertimos el string de salida a un arreglo de caracteres usado por radio.write
  char datos[string.length()];
  strncpy(datos, string.c_str(), sizeof(datos));
  if(depuracion){
    Serial.print("Enviando: ");
    Serial.print(string);
    Serial.print(" #");
    Serial.println(string.length());
  }
  // enviamos los datos
  if (!radio.write(&datos, sizeof(datos) ) ){
    return false;
  }else{
    return true;
  } 
}

void enviar_datos_cortos_mostrar(String string){
  // convertimos el string de salida a un arreglo de caracteres usado por radio.write
  char datos[string.length()];
  strncpy(datos, string.c_str(), sizeof(datos));
  if(depuracion){
    Serial.print("Enviando: ");
    Serial.print(string);
    Serial.print(" #");
    Serial.println(string.length());
  }
  // enviamos los datos
  if (!radio.write(&datos, sizeof(datos) ) ){
    Serial.println(F("failed"));
  }else{
    Serial.println(F("Enviado"));
  } 
}

bool enviar_datos_mostrar(String mensaje){
  // convertimos el string de salida a un arreglo de caracteres usado por radio.write
  unsigned int largoMensaje = mensaje.length();
  unsigned int numeroPaquetes  = largoMensaje / 32;
  if(largoMensaje % 32 > 0){
    numeroPaquetes++;
  }
  if(depuracion){
    Serial.print(" Lm: ");
    Serial.print(largoMensaje);
    Serial.print(" #P: ");
    Serial.println(numeroPaquetes);
  }
  // envimos el paquete de inicio
  enviar_datos("-" + String(numeroPaquetes) + "-");

  for(int i=0;i<numeroPaquetes;i++){
    unsigned int fin  = (i+1)*32;
    if(fin >= largoMensaje){
      fin = largoMensaje;
    }
    String paquete = mensaje.substring(i*32, fin);

    
    bool noEnviado = true;
    unsigned long tiempoInicioMensaje = millis();
    unsigned long maximoTiempoEspera = 5000;     // lo que esperamos antes de dejar de reenviar    
    while(noEnviado && millis() < (tiempoInicioMensaje + maximoTiempoEspera)){
      if(enviar_datos(paquete)){
        noEnviado = false; 
      } // fin if enviado con exito
    } // fin while reenvio
    if(noEnviado){
      if(depuracion)Serial.println("-Tiempo- Mensaje Cancelado");
      return false;   // canselamos el envio del paquete
    }
  } // fin for paquetes
  
  // si no se logra enviar el mesaje completo
  return false;
}

String leer_datos(){
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

