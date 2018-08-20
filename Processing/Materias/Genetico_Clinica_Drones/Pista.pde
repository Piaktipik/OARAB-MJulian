

class D_pista {
  // pista a ser dibujada

  void draw() {

    // -------------------------------- dibujo marco de la pista ----------------------------------
    stroke(0);      // color de linea negra
    noFill();       // no hay relleno

    // lineas horizontales
    for (i=tam/2; i<y*sep; i+=sep) {
      line(tam/2, i, x*sep-sep, i);
    }

    // lineas verticales (pista)
    for (i=tam/2; i<x*sep; i+=sep) {
      line(i, tam/2, i, y*sep-sep);
    }

    // dibujo los bordes de la pista
    stroke(color(0, 0, 255));                         // color de linea azul
    line(-sep, -sep, -sep, y*sep+tam);                // lateral izquierda
    line(x*sep+tam, -sep, x*sep+tam, y*sep+tam);      // lateral derecha
    line(-sep, -sep, x*sep+tam, -sep);                // superior
    line(-sep, y*sep+tam, x*sep+tam, y*sep+tam);      // inferior

    // ------------------------- dibujo los datos en la matriz de la pista ---------------------------
    stroke(0);      // color de linea negra
    color col;
    for (i=0; i<x; i++) {
      for (j=0; j<y; j++) {
        // ajusto el color de la pista
        switch(pista[i][j]) { // red green blue
          // camino transitable
        case 0:   
          col = color(  0, 0, 0);
          break;   // pinto pista negro
        case 1:   
          col = color(255, 220, 80);
          break;   // pinto mejor camino de amarillo 
        case 2:   
          col = color(255, 0, 0);
          break;   // pinto posible pelota de rojo
        case 3:   
          col = color(  0, 225, 0);
          break;   // pinto pelota verde feo
        case 4:   
          col = color(  0, 150, 150);
          break;   // pinto - azul marino
          // objentos no transitables
        case 5:   
          col = color(  0, 0, 255);
          break;   // pinto la rampa azul
        case 6:   
          col = color(255, 255, 255);
          break;   // pinto paredes blancas
        case 7:   
          col = color(250, 100, 50);
          break;   // pinto fin naranjado
        case 8:   
          col = color(255, 255, 255);
          break;   // pinto objetivo actual blanco
        case 9:   
          col = color(100, 150, 100);
          break;   // pinto robot verde oscuro
        default:  
          col = color(0);
          break;   // pinto posible pelota
        }
        fill(col);       // relleno con el color respectivo
        rect(i*sep, j*sep, tam, tam);
        fill(0);
        text(""+pistat[i][j], i*sep+tam/3, j*sep+tam);
      }
    }
    // ------------------------- dibujo las soluciones ---------------------------
    if (verPoblacion>0) {
      stroke(color(0, 255, 0));                         // color de linea azul
      for (int i = 0; i<poblacion[verPoblacion].acidentados_gen.size(); i++) {
        //println("Ind: "+poblacion[0].acidentados_gen.get(i) + ", Hos: " + poblacion[0].hospitales_gen.get(i));
        line(poblacion[verPoblacion].acidentados_gen.get(i).x*sep, poblacion[verPoblacion].acidentados_gen.get(i).y*sep, poblacion[verPoblacion].hospitales_gen.get(i).x*sep, poblacion[verPoblacion].hospitales_gen.get(i).y*sep);  
        // asignamos a cada paciente un hospital
      }
    }
  }// draw
} //d_pista