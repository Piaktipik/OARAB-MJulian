// zzzzzzzzzzzzzzzzzzzzzzzzzzzzz motores zzzzzzzzzzzzzzzzzzzzzzzzzzzzz

void parar() {
  digitalWrite(pin_dire_moto_1, LOW);    // motor 1
  analogWrite(pin_velo_moto_1, 0);
  digitalWrite(pin_dire_moto_2, LOW);    // motor 2
  analogWrite(pin_velo_moto_2, 0);
  estadoMotores = 0;
}
void adelante() {
  digitalWrite(pin_dire_moto_1, LOW);    // motor 1
  analogWrite(pin_velo_moto_1, velo_moto_1);
  digitalWrite(pin_dire_moto_2, LOW);    // motor 2
  analogWrite(pin_velo_moto_2, velo_moto_2);
  estadoMotores = 1;
}
void atras() {
  digitalWrite(pin_dire_moto_1, HIGH);    // motor 1
  analogWrite(pin_velo_moto_1, velo_moto_1);
  digitalWrite(pin_dire_moto_2, HIGH);    // motor 2
  analogWrite(pin_velo_moto_2, velo_moto_2);
  estadoMotores = 2;
}
void izquierda() {
  digitalWrite(pin_dire_moto_1, LOW);    // motor 1
  analogWrite(pin_velo_moto_1, velo_moto_1);
  digitalWrite(pin_dire_moto_2, HIGH);    // motor 2
  analogWrite(pin_velo_moto_2, velo_moto_2);
  estadoMotores = 3;
}
void derecha() {
  digitalWrite(pin_dire_moto_1, HIGH);    // motor 1
  analogWrite(pin_velo_moto_1, velo_moto_1);
  digitalWrite(pin_dire_moto_2, LOW);    // motor 2
  analogWrite(pin_velo_moto_2, velo_moto_2);
  estadoMotores = 4;
}

void cargarVelocidades() {
  analogWrite(pin_velo_moto_1, velo_moto_1);
  analogWrite(pin_velo_moto_2, velo_moto_2);
}

void velocidad(byte vel) {
  vel = abs(vel);
  if (vel > 255) {
    vel = 255;
  }
  velo_moto_1 = vel;
  velo_moto_2 = vel;
  if (estadoMotores != 0) {
    cargarVelocidades();
  }
}
