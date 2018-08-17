
void configurar_radio_receptor(){
   printf_begin();
  radio.begin();
  // RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  //radio.setPayloadSize(100);
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

void identificarModulos(){
  String datos  =  escucharMensaje();

  if(datos.substring(0, 4)=="HOLA"){
    // extraemos la direccion para responder
    String direcionString =  datos.substring(5, datos.length());
    uint64_t direccion = stringToUint64(direcionString);
    
    if(depuracion){
      Serial.print("Direccion Modulo Recibida: ");
      Serial.println(direcionString);
    }
    radio_direccion_transmisor(direccion);
    if(depuracion){
      Serial.print("Transmitiendo A: ");
      Serial.println(direcionString);
    }
    // enviamos mensaje de reconocimiento a esa direccion
    enviarMensaje("HOLA-IDENTIFICATE");
  }else if(datos[0]=='{'){
    // si envia correctamente su defincion en formato json parseable, damos la bienbenida
    enviarMensaje("BIENVENIDO");
    moduloValido[0] = true;
  }
  
}

void radio_direccion_transmisor(uint64_t direccionNueva){
  radio.openWritingPipe(direccionNueva);
}

int obtenerNumeroPaquetes(String paquete){
  String numero = "";
  for(int i = 0; i<paquete.length();i++){
    if (paquete[i] != '-'){
      numero += paquete[i];
    }
  }
  return numero.toInt();
}


String uint64ToString(uint64_t numero){
    String numerostring = "";
    uint64_t xx = numero/1000000000ULL;

    if (xx >0) numerostring = String((long)xx);
    numerostring += String((long)(numero-xx*1000000000));
    return numerostring;
}

uint64_t stringToUint64(String numerostring){
  //Serial.println(numerostring);
  uint64_t numero = 0;
  uint64_t potencia = 1;
  for(int i=numerostring.length()-1; i>=0;i--){
    numero += (((long)numerostring[i])-48)*potencia;
    potencia *= 10;
  }
  return numero;
}




void enviarMensaje(String mensaje){
        unsigned long tiempoReintentando = millis();
        while(!enviarStringNrf24l01(mensaje) && millis() < tiempoReintentando+200){}
}

bool enviarStringNrf24l01(String mensaje){
  // convertimos el string de salida a un arreglo de caracteres usado por radio.write
  unsigned int largoMensaje = mensaje.length();

  unsigned int numeroPaquetes  = largoMensaje / 32;
  if(largoMensaje % 32 > 0){numeroPaquetes++;}

  if(numeroPaquetes > 1){
      if(depuracion){
        Serial.print(" Lm: ");
        Serial.print(largoMensaje);
        Serial.print(" #P: ");
        Serial.println(numeroPaquetes);
      }
      // envimos el paquete de inicio
      enviarHasta32CharNrf24l01("-" + String(numeroPaquetes) + "-");
  }
  for(int i=0;i<numeroPaquetes;i++){
    unsigned int fin  = (i+1)*32;
    if(fin >= largoMensaje){
      fin = largoMensaje;   // para el ultimo paquete que no es de tamaño 32*i
    }
    String paquete = mensaje.substring(i*32, fin);

    //bool noEnviado = true;
    unsigned long tiempoInicioMensaje = millis();
    unsigned long maximoTiempoEspera = 100;     // lo que esperamos antes de dejar de reenviar
    while(!enviarHasta32CharNrf24l01(paquete) && millis() < (tiempoInicioMensaje + maximoTiempoEspera)){} // fin while reenvio

    if(!(millis() < (tiempoInicioMensaje + maximoTiempoEspera))){
      if(depuracion)Serial.println("-Tiempo- Mensaje Cancelado");
      return false;   // canselamos el envio del paquete
    }
  } // fin for paquetes
  // si se logra enviar el mensaje completo
  return true;
}

bool enviarHasta32CharNrf24l01(String mensaje){
  radio.stopListening();
  // convertimos el string de salida a un arreglo de caracteres usado por radio.write
  char datos[mensaje.length()];
  strncpy(datos, mensaje.c_str(), sizeof(datos));
  bool enviado = radio.write( &datos, sizeof(datos) );

  if(depuracion){
    Serial.print("Enviando: #");
    Serial.print(mensaje.length());
    Serial.print(" ");
    Serial.print(mensaje);
      // enviamos los datos
      if (enviado){
        Serial.println(F(" Exitoso"));
      }else{
        Serial.println(F(" Fallido"));
      }
  }
    return enviado;
}


String recivirStringNrf24l01(){
    // leemos un primer paquete
    String mensaje = recivirHasta32Nrf24l01();

    // si enviaron paquetes
    if (mensaje[0] == '-'){
      // son varios paquetes, obtengo el numero de paquetes
      byte numeroPaquetes = obtenerNumeroPaquetes(mensaje);

      // preguntamos aunque es obio
      if(numeroPaquetes > 0 ){
            mensaje = "";       // limpiamos mensaje
            int contadorPaquetesRecibidos = 0;
            unsigned long tiempoInicioMensaje = millis();
            unsigned long maximoTiempoEspera = 200;     // lo que esperamos antes de cancelar el paquete
            while(contadorPaquetesRecibidos < numeroPaquetes && millis() < (tiempoInicioMensaje + maximoTiempoEspera)){
                // armamos el paquete
                String paqueteLeido = recivirHasta32Nrf24l01();
                if(paqueteLeido != ""){
                    mensaje += paqueteLeido;
                    contadorPaquetesRecibidos++;
                }
            } // fin while varios paquetes

            // si el paquete no llego completo lo descartamos
            if(contadorPaquetesRecibidos < numeroPaquetes){
                if(depuracion){Serial.print("Paquete Incompleto");}
                return "";
            }else{
                return mensaje;
            }
        }// fin numero de paquetes mayor a 0

    }else{
        return mensaje;     // si no esta por paquetes, retornamos el mensaje recivido
    }
}

String recivirHasta32Nrf24l01(){

  radio.startListening();
  uint8_t len;
  char data[buffer_datos_recividos];
  String mensaje = "";

  if(radio.available()){
    while (radio.available()) {             // While there is data ready
      len = radio.getDynamicPayloadSize();
      radio.read(&data,len);              // Get the payload
      mensaje = String(data).substring(0, len);
    }
    if(depuracion){
      Serial.print("Recibido: #");
      Serial.print(len);
      Serial.print(" ");
      Serial.println(mensaje);
    }
    return mensaje;
  }else{
    return "";
  }
}

String escucharMensaje(){
  unsigned long tiempoReintentando = millis();
  String datos = "";
  while((datos == "") && millis() < tiempoReintentando+200){
      datos = recivirStringNrf24l01();
  }
  return datos;
}

