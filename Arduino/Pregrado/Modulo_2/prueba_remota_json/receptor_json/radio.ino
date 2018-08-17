
void configurar_radio_receptor(){
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

