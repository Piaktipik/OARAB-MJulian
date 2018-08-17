
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

void enviar_datos_mostrar(String string){
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

