
void DH() {

  float x0 = ax[0];
  float x1 = ax[1];
  float x2 = ax[2];

  float z0 = az[0];
  float z1 = az[1];
  float z2 = az[2];

  //xf = d2*sin(x2)*(cos(z0)*sin(z1)+cos(x1)*cos(z1)*sin(z0))+d1*sin(x1)*sin(z0)+d2*cos(x2)*sin(x1)*sin(z0);
  //yf = d0*sin(x0)*-1.0-d2*cos(x2)*(cos(x1)*sin(x0)+cos(x0)*cos(z0)*sin(x1))*1.0-d1*cos(x1)*sin(x0)*1.0+d2*sin(x2)*(cos(z1)*sin(x0)*sin(x1)+cos(x0)*sin(z0)*sin(z1)-cos(x0)*cos(x1)*cos(z0)*cos(z1)*1.0)-d1*cos(x0)*cos(z0)*sin(x1)*1.0;
  //zf = d0*cos(x0)+d1*cos(x0)*cos(x1)-d2*sin(x2)*(cos(x0)*cos(z1)*sin(x1)-sin(x0)*sin(z0)*sin(z1)*1.0+cos(x1)*cos(z0)*cos(z1)*sin(x0))*1.0+d2*cos(x2)*(cos(x0)*cos(x1)-cos(z0)*sin(x0)*sin(x1)*1.0)-d1*cos(z0)*sin(x0)*sin(x1)*1.0;

  xf = d3*(sin(x2)*(cos(z0)*sin(z1)+cos(x1)*cos(z1)*sin(z0))+cos(x2)*sin(x1)*sin(z0))+d2*sin(x2)*(cos(z0)*sin(z1)+cos(x1)*cos(z1)*sin(z0))+d1*sin(x1)*sin(z0)+d2*cos(x2)*sin(x1)*sin(z0);
  yf = d3*(sin(x2)*(cos(z1)*sin(x0)*sin(x1)+cos(x0)*sin(z0)*sin(z1)-cos(x0)*cos(x1)*cos(z0)*cos(z1)*1.0)-cos(x2)*(cos(x1)*sin(x0)+cos(x0)*cos(z0)*sin(x1))*1.0)-d0*sin(x0)*1.0-d2*cos(x2)*(cos(x1)*sin(x0)+cos(x0)*cos(z0)*sin(x1))*1.0-d1*cos(x1)*sin(x0)*1.0+d2*sin(x2)*(cos(z1)*sin(x0)*sin(x1)+cos(x0)*sin(z0)*sin(z1)-cos(x0)*cos(x1)*cos(z0)*cos(z1)*1.0)-d1*cos(x0)*cos(z0)*sin(x1)*1.0;
  zf = d0*cos(x0)-d3*(sin(x2)*(cos(x0)*cos(z1)*sin(x1)-sin(x0)*sin(z0)*sin(z1)*1.0+cos(x1)*cos(z0)*cos(z1)*sin(x0))-cos(x2)*(cos(x0)*cos(x1)-cos(z0)*sin(x0)*sin(x1)*1.0)*1.0)*1.0+d1*cos(x0)*cos(x1)-d2*sin(x2)*(cos(x0)*cos(z1)*sin(x1)-sin(x0)*sin(z0)*sin(z1)*1.0+cos(x1)*cos(z0)*cos(z1)*sin(x0))*1.0+d2*cos(x2)*(cos(x0)*cos(x1)-cos(z0)*sin(x0)*sin(x1)*1.0)-d1*cos(z0)*sin(x0)*sin(x1)*1.0;

  Rxx=cos(z2)*(cos(z0)*cos(z1)-cos(x1)*sin(z0)*sin(z1)*1.0)-cos(x2)*sin(z2)*(cos(z0)*sin(z1)+cos(x1)*cos(z1)*sin(z0))*1.0+sin(x1)*sin(x2)*sin(z0)*sin(z2); 
  Rxy=sin(z2)*(cos(z0)*cos(z1)-cos(x1)*sin(z0)*sin(z1)*1.0)*-1.0-cos(x2)*cos(z2)*(cos(z0)*sin(z1)+cos(x1)*cos(z1)*sin(z0))*1.0+cos(z2)*sin(x1)*sin(x2)*sin(z0); 
  Rxz=sin(x2)*(cos(z0)*sin(z1)+cos(x1)*cos(z1)*sin(z0))+cos(x2)*sin(x1)*sin(z0);
}

void plano(int tam, int res, int ta) {
  pushMatrix();
  stroke(100, 0, 0);                      // color linea eje x
  translate(-tam, 0, 0);
  for (int i=0; i < 2*tam; i = i+res) {
    box(ta);
    line(0, 0, 0, res, 0, 0);          // dibuja el eje x
    translate(res, 0, 0);
  }
  translate(-tam, 0, 0);

  stroke(0, 100, 0);                      //color de linea eje y
  translate(0, -tam, 0);
  for (int i=0; i < 2*tam; i = i+res) {
    box(ta);
    line(0, 0, 0, 0, res, 0);          // dibuja el eje y
    translate(0, res, 0);
  }
  translate(0, -tam, 0);

  stroke(0, 0, 100);                      //color de linea eje z
  translate(0, 0, -tam);
  for (int i=0; i < 2*tam; i = i+res) {
    box(ta);
    line(0, 0, 0, 0, 0, res);          // dibuja el eje z
    translate(0, 0, res);
  }
  translate(0, 0, -tam);
  popMatrix();
}

void keyPressed() {
  if (key == 'x') {
    rotx += radians(-5);
  } 
  if (key == 'y') {
    roty += radians(-5);
  }
  if (key == 'z') {
    rotz += radians(-5);
  }

  if (key == CODED) { 
    // pts
    if (keyCode == UP) {
    } else if (keyCode == DOWN) {
    } 
    // extrusion length
    if (keyCode == LEFT) {
    } else if (keyCode == RIGHT) {
    }
  }
  // captura de ascencion recta y declinacion
  if (editar == 'n' && key == 'a') {
    editar='a';
    Asc = "";
    cursor = 0;
  }
  if (editar == 'n' && key == 'd') {
    editar='d';
    Dec = "";
    cursor = 0;
  }

  if (key>='0' && key<='9') {
    if (editar == 'a') {
      Asc = Asc + key;
      //cargamos hora, minuto y segundos y decenas de segundos 
      int valor = int(key)-48;
      if (cursor==0) {
        Asch = valor*10;
      } else if (cursor==1) {
        Asch += valor;
      }
      if (cursor==2) {
        Ascm = valor*10;
      } else if (cursor==3) {
        Ascm += valor;
      }
      if (cursor==4) {
        Ascs = valor*10;
      } else if (cursor==5) {
        Ascs += valor;
      }
      if (cursor==6) {
        Ascs += float(valor)/10;
      } else if (cursor==7) {
        //Ascs += float(valor)/100;
      }
    } else if (editar == 'd') {
      Dec = Dec + key;
      int valor = int(key)-48;
      if (cursor==0) {
        Dech = valor*10;
      } else if (cursor==1) {
        Dech += valor;
      }
      if (cursor==2) {
        Decm = valor*10;
      } else if (cursor==3) {
        Decm += valor;
      }
      if (cursor==4) {
        Decs = valor*10;
      } else if (cursor==5) {
        Decs += valor;
      }
      if (cursor==6) {
        Decs += float(valor)/10;
      } else if (cursor==7) {
        //Decs += float(valor)/100;
      }
    }
    cursor++;
    // al llegar a 6 caractres se finaliza la edicion
    if (cursor>=8) {
      editar='n';
      return;
    }
    // se añade espacio cada 2 valores
    if (cursor%2==0) {
      if (editar == 'a') {
        if (cursor<6) {
          Asc = Asc + ' ';
        } else {
          Asc = Asc + '.';
        }
      } else if (editar == 'd') {
        if (cursor<6) {
          Dec = Dec + ' ';
        } else {
          Dec = Dec + '.';
        }
      }
    }
  }
}

void drawCylinder( int sides, float r, float h) {
  float angle = 360 / sides;
  float halfHeight = h / 2;
  fill(200);
  // draw top of the tube
  beginShape();
  for (int i = 0; i < sides; i++) {
    float x = cos( radians( i * angle ) ) * r;
    float y = sin( radians( i * angle ) ) * r;
    vertex( x, y, -halfHeight);
  }
  endShape(CLOSE);
  fill(0);
  // draw bottom of the tube
  beginShape();
  for (int i = 0; i < sides; i++) {
    float x = cos( radians( i * angle ) ) * r;
    float y = sin( radians( i * angle ) ) * r;
    vertex( x, y, halfHeight);
  }
  endShape(CLOSE);
  fill(200);
  // draw sides
  beginShape(TRIANGLE_STRIP);
  for (int i = 0; i < sides + 1; i++) {
    float x = cos( radians( i * angle ) ) * r;
    float y = sin( radians( i * angle ) ) * r;
    vertex( x, y, halfHeight);
    vertex( x, y, -halfHeight);
  }
  endShape(CLOSE);
}