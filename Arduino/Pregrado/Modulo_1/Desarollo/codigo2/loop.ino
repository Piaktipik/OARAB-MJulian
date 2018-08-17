
void Loop(){
  // se ejecuta un programa dependiendo de lo que se quiera ver del robot
  switch(ver){
    
    // ************************************** caso ************************************** 
    case Encoders : 
      informarEstadoMedicionEncoders();

    // ************************************** caso ************************************** 
    break;case SensoresSuelo:
    
      informarEstadoMedicionSuelo();
      haySuelo();
      //delay(50);
    // ************************************** caso ************************************** 
    break;case SensoresDistancia:
    
      //revisarSerialUltrasonidos();
      informarMedicionDistancia();
      
    // ************************************** caso ************************************** 
    break;case SensoresDistanciaRadar:
    
      informarEstadoMedicionRadares();
    
    // ************************************** caso ************************************** 
    break;case ControlSerialMotores:
      
      // recivo comandos por serial
      //revisarSerial();
      //escucharRaspberry();
      //if(hayColision() || !haySuelo())movimiento(Detenido);
      
    // ************************************** caso ************************************** 
    break;case EvasorObstaculos:
      
     
      // ----------------- evado obstaculos ----------------------  
      obstaculo = false; // empiezo diciendo que no hay obstaculos
      // derecho
      if(true){
        //informarEstadoMedicionRadares();
        radarActivado = true;
        if(medidaDistanciaDerecha < dist_mini_late) {movimiento(Izquierda);obstaculo = true;}
        // izquierdo
        if(medidaDistanciaIzquierda < dist_mini_late) {movimiento(Derecha);obstaculo = true;}
          // frontal
        if(medidaDistanciaFrontal < dist_mini_fron) {/*movimiento(Atras);*/escaparHaciaAtras();obstaculo = true;}
      }else{
        //informarMedicionDistanciaBasica();
        posicionFijaRadares();
        if(medidaDistanciaDerecha < dist_mini_late) {movimiento(Izquierda);obstaculo = true;}
        // izquierdo
        if(medidaDistanciaIzquierda < dist_mini_late) {movimiento(Derecha);obstaculo = true;}
          // frontal
        if(medidaDistanciaFrontal < dist_mini_fron) {/*movimiento(Atras);*/escaparHaciaAtras();obstaculo = true;}
      }
      
      if(hayColision() || !haySuelo())movimiento(Detenido);
      // si no hay obtaculo voy hacia adelante
      else if(!obstaculo){movimiento(Adelante);}
      
    // ************************************** caso ************************************** 
    break;case SeguidorLinea:
    
    informarEstadoMedicionSuelo();
    if(cont_line_dere + 2 < cont_line_izqu){
      movimiento(Izquierda);
    }else if (cont_line_dere > cont_line_izqu + 2){
      movimiento(Derecha);
    }else{
      if(haySuelo()){
        movimiento(Adelante);
      }else{
        movimiento(Detenido);
      }
    }
      
    // ************************************** caso ************************************** 
    break;case SeguidorObjetos:
      obstaculo = false; // empiezo diciendo que no hay obstaculos

      if(true){
        //informarMedicionDistanciaBasica();
        posicionFijaRadares();
        // frontal muy cerca
        if(medidaDistanciaFrontal < dist_mini_fron) {movimiento(Atras);obstaculo = true;}
        // frontal muy lejos
        else if(medidaDistanciaFrontal > dist_maxi_fron) {movimiento(Adelante);obstaculo = true;}
        // derecho
        else if(medidaDistanciaDerecha < dist_maxi_late) {movimiento(Derecha);obstaculo = true;}
        // izquierdo
        else if(medidaDistanciaIzquierda < dist_maxi_late) {movimiento(Izquierda);obstaculo = true;}
      }else{
        //informarMedicionDistanciaBasica();
        posicionFijaRadares();
        // frontal muy cerca
        if(medidaDistanciaFrontal < dist_mini_fron) {movimiento(Atras);obstaculo = true;}
        // frontal muy lejos
        else if(medidaDistanciaFrontal > dist_maxi_fron) {movimiento(Adelante);obstaculo = true;}
        // derecho
        else if(medidaDistanciaDerecha < dist_mini_late) {movimiento(Izquierda);obstaculo = true;}
        // izquierdo
        else if(medidaDistanciaIzquierda < dist_mini_late) {movimiento(Derecha);obstaculo = true;}
         
      }
      
      if(hayColision() || !haySuelo())movimiento(Detenido);
      // si no hay obtaculo voy hacia adelante
      else if(!obstaculo){movimiento(Detenido);}
      
      
    break;
  } // fin switch

  // llamado a la funcion principal de la libreria interface
  (*interface).revisarInterface();
  
}// fin loop
