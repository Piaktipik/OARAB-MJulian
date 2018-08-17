void datoserial(char dato) {
  if (dato == 'a') {
    adelante();
    Serial.println('a');
  }
  if (dato == 'i') {
    izquierda();  // motor 2 Adelante
    Serial.println('i');
  }
  if (dato == 'd') {
    derecha();
    Serial.println('d');
  }
  if (dato == 'p') {
    parar();
    Serial.println('p');
  }
  if (dato == 'h') {
    atras();
    Serial.println('h');
  }

  if (dato == 'v') {
    velo_moto_1 = leerByte(); // vel motor 1
    velo_moto_2 = leerByte(); // vel motor 2
    if (estadoMotores != 0) {
      cargarVelocidades();
    }
  }

  if (dato == 's') {
    delay(100);
    set_point = int(Serial.read() - 48) * 1000;
    set_point += int(Serial.read() - 48) * 100;
    set_point += int(Serial.read() - 48) * 10;
    set_point += int(Serial.read() - 48);
  }

}

byte leerByte() {
  delay(10);
  byte cont = 0;  // contador posicion llegada
  byte dato = (Serial.read() - 48);
  int lectura = 0;
  while (dato > -1 && dato < 10 && cont < 3) {
    lectura = lectura * 10 + dato; // aumentamos decenas
    cont++;
    dato = (Serial.read() - 48);
  }
  return byte(lectura);
}

