

void enviarAzCupula() {
  OscMessage myMessage = new OscMessage("AZ");
  myMessage.add(CUP);
  println("Enviando Azimut: " + CUP);
  /* send the message */
  oscP5.send(myMessage, myBroadcastLocation);
}

void convertirRaDe2AZh() {

  print("Cordendas ASC: " + Asch + ":" + Ascm + ":" + Ascs);

  float hora = (hour()+float(minute())/60+float(second())/3600)-(longitud/15);
  //float hora = (Asch+float(Ascm)/60+float(Ascs)/3600)-(longitud/15);

  // segun la longitud
  int y = year();
  int m = month();
  int day = day();
  float TSL = 0;
  float longitudg = degrees(longitud);
  print(", FechaSistema: " + y + "/" + m + "/" + day + " " + hora + ", longitudg: " + longitudg); 

  if (longitudg>=180 && longitudg<=360) {
    float J=367*y-int(7*(y+int((m+9)/12))/4)+int(275*m/9)+day-730531.5 + (hora/24.0)+1;
    float GMST=(280.46061837+360.98564736629*J)%360; 
    print(", J: " + J + ", GMST: " + GMST); 

    TSL = (GMST+longitudg)/15;
    if (TSL<0) {
      TSL=TSL+24;
    } else if (TSL>24) {
      TSL=TSL-24;
    }
    //print("El tiempo sideral local es:");
    //print("%d:%02d:%02d" % (TSL, 60*(TSL%1), 60 * (60*(TSL%1) % 1));
  } else {
    float J=367*y-int(7*(y+int((m+9)/12))/4)+int(275*m/9)+day-730531.5 + (hora/24.0);
    float GMST=(280.46061837+360.98564736629*J)%360; 
    print(", J: " + J + ", GMST: " + GMST); 

    TSL = (GMST+longitudg)/15;
    if (TSL<0) {
      TSL=TSL+24;
    } else if (TSL>24) {
      TSL=TSL-24;
    }
  }

  println(", TSL=", TSL);
}


float calculoFechaJuliana() {
  // Dia de referencia para el calculo de la fecha juliana es el 1 de enero del 4713 antes de jesucristo del mediodia de GreenWich (12h TU (Tiempo Universal))
  // rutina de calculo segun Meeus 1991, tomada de gregorio portilla
  //print("Calculo Fecha Juliana: "); 
  // segun la longitud
  int Y = year();
  int M = month();
  int D = day();
  float hora = (hour()+float(minute())/60+float(second())/3600);
  float DD = float(hour()-12)/24.0 + float(minute())/1440.0 + float(second())/86400.0;
  float longitudg = degrees(longitud);
  //print(" FechaSistema: " + Y + "/" + M + "/" + D + " " + hora + " " + DD +", longitudg: " + longitudg); 


  JDN = (1461 * (Y + 4800 + (M-14)/12))/4 +(367*(M-2-12*((M-14)/12)))/12-(3*((Y + 4900 + (M - 14)/12)/100))/4 + D-32075;
  JD = JDN + DD; 
  // agregar un dia si el tiempo esta entre las 00 (medianoche) UT y las 12 (mediodia) UT

  //println(", Dia Juliano: " + JDN + ", Fecha juliana: " + JD);

  return JD;
}


float calculoTSG0() {
https://es.wikipedia.org/wiki/Tiempo_sid%C3%A9reo
  // Dia de referencia para el calculo de la fecha juliana es el 1 de enero del 4713 antes de jesucristo del mediodia de GreenWich (12h TU (Tiempo Universal))
  // rutina de calculo segun Meeus 1991, tomada de gregorio portilla
  print("Calculo tiempo sideral en Greenwich 0h: "); 


  float T = (JD - 2451545.0)/35525; // (fecha juliana - fecha juliana del 1 enero a las 12h de TU del año 2000)/??
  float TSG0 = 0.0;

  //TSG0 = 6.646065556*T + 0.0929 * pow(T,2); //6h 38min 45.836s + 8640184,542s *T + 0.0929s * T^2;



  //float hora = (hour()+float(minute())/60+float(second())/3600);
  float TSGt = TSG0 + 1.00273790935*(hour() + horaUTC); // tiempo sidereo en greenwich 0h a tiempo t

  float TSL = TSGt +(longitud/15);    //ponemos el tiempo sideral a la latitud local

  // segun la longitud
  int Y = year();
  int M = month();
  int D = day();

  float DD = float(hour()-12)/24.0 + float(minute())/1440.0 + float(second())/86400.0;
  float longitudg = degrees(longitud);
  //print(" FechaSistema: " + Y + "/" + M + "/" + D + " " + hora + " " + DD +", longitudg: " + longitudg); 
  return 0;
}

void conversion() {
  //<!--Datos entrada-->

  //Ascension Recta
  int AH = 1;
  int AM = 1;
  int AS = 1;
  float RA = AH + AM/60 + AS/3600;

  //<!--Declinación-->
  int DG = 1;
  int DM = 1;
  int DS = 1;
  float DC=DG+DM/60+DS/3600;

  //<!--latitud-->
  int BG = 1;
  int BM = 1;
  int BS = 1;
  float LT=BG+BM/60+BS/3600;

  //<!--longitud-->
  int LG1 = 1;
  int LM = 1;
  int LS = 1;
  float LG=LG1+LM/60+LS/3600;

  //<!--fecha juliana-->
  int TH = 1;
  int TM = 1;
  int TS = 1;
  int DD1 = 0;
  float HR = TH + TM / 60+TS/3600;
  float DD=DD1+HR/24;

  int DY = floor(DD);

  int YR = 2018;
  int MN = 7;

  if (MN<3) {
    YR = YR - 1;
    MN = MN + 12;
  }

  int GR = 1;
  if (YR + MN / 100 + DY / 10000 >= 1582.1015) {
    GR =2-floor(YR/100)+floor(floor(YR/100)/4);
  } else {
    GR = 0;
  }

  JD=floor(365.25* YR)+floor(30.6001*(MN+1))+DY+1720994.5+GR;  
  float T=(JD- 2415020)/36525;
  float SS= 6.6460656 + 2400.051*T +0.00002581*T*T;
  //<!--tiempo sidereo a 0h Greenwich-->
  float ST =(SS/24-floor(SS/24))*24;

  //<!--convertir tiempo sidereo a 0h Greenwich a hms-->
  int GSTH=floor(ST);
  int GSTM=floor((ST - floor(ST)) * 60);
  float GSTS=((ST -floor(ST)) * 60 - GSTM) * 60;
  //<!--Tiempo sidereo local-->
  float SA=ST+(DD-floor(DD))*24*1.002737908;
  SA=SA+LG/15;
  if (SA<0) {
    SA=SA+24;
  }
  if (SA>24) {
    SA=SA-24;
  }
  //<!--conversion a hms del Tiempo sidereo local-->
  int TSH=floor(SA);
  int TSM=floor((SA - floor(SA)) * 60);
  float TSS=((SA -floor(SA)) * 60 - TSM) * 60;

  //<!--Angulo horario-->
  float H=SA-RA;
  if (H < 0) {
    H = H + 24;
  }
  //<!--conversion a hms del a. horario -->
  int HH=floor(H);
  int HM=floor((H - floor(H)) * 60);
  float HS=((H -floor(H)) * 60 - HM) * 60;
}

float[] grad2sexa(float val) {
  print("In: " + val);
  float sexa[] = {0, 0, 0}; // hora-minutos-segundos
  if (val<24) {
    sexa[0] = floor(val);
    val = val-sexa[0];         // Quitamos las horas
    sexa[1] = floor(val*60);   // El restante entero son minutos
    val = val*60%1;            // Quitamos los minutos
    sexa[2] = val*60;          // El restante son segundos
  }
  println(" Out: " + sexa[0] + ":" + sexa[1] + ":"+ sexa[2]);
  return sexa;
}

float sexa2grad(float H, float M, float S) {
  print("In: " + H + ":" + M + ":"+ S);
  float val = 0; // hora
  if (H<24 && M<60 && S<60) {
    val = H+M/60+S/3600;
  }
  println(" Out: " + val);
  return val;
}


float codigoMauro(float AR[], float DEC[]) {

  // Datos de entrada manuales
  //int TL[]  = {19, 0, 0}; // Tiempo local de la observacion {horas, minutos, segundos} 0h<=TL<24h
  //int fecha[] = {5, 8, 2018}; // Fecha de la observacion {dia, mes, ano}, mes en numero.
  float lugar[] = {6, 14, 42.90, -75, 33, 4.87}; // Posicion geografica del lugar {grados_latitud, minutos_latitud, segundos_latitud, grados_longitud, minutos_longitud, segundos_longitud}
  
  int TL[]  = {hour(), minute(), second()}; // Tiempo local de la observacion {horas, minutos, segundos} 0h<=TL<24h
  int fecha[] = {day(), month(), year()}; // Fecha de la observacion {dia, mes, ano}, mes en numero.
  
  // si la latitud es sur, se indica con el signo menos en los grados_latitud.
  // si la longitud es oeste, se indica con el signo menos en los grados_longitud.
  // -90 grados<= Latitud <= 90 grados
  // -180 grados <= Longitud <180 grados
  //int US = 5;  // Uso horario del lugar

  // Conversion de datos de entrada 
  float ARhoras =  AR[0] + AR[1]/60. + AR[2]/3600.; // de [horas, minutos, segundos] a [horas.decimales]
  float DECrad;
  float longgrad;
  float latitudrad;
  if (DEC[0] >= 0.0 )
    DECrad = radians(DEC[0] + DEC[1]/60. + DEC[2]/3600.); // de [grados, minutos, segundos] a [radianes]
  else
    DECrad = radians(DEC[0] - DEC[1]/60. - DEC[2]/3600.); // de [grados, minutos, segundos] a [radianes]
  if (lugar[3] >= 0.0)
    longgrad = lugar[3] + lugar[4]/60. + lugar[5]/3600.;  // de [horas, minutos, segundos] a [horas.decimales]
  else
    longgrad = lugar[3] - lugar[4]/60. - lugar[5]/3600.; // de [horas, minutos, segundos] a [horas.decimales]
  if (lugar[0] >= 0.0)
    latitudrad = radians(lugar[0] + lugar[1]/60. + lugar[2]/3600.);
  else
    latitudrad = radians(lugar[0] - lugar[1]/60. - lugar[2]/3600.);

  ////// CALCULO DE LA FECHA JULIANA DEL CALCULO DE LA EFEMERIDE //////

  int TU[] = {0, 0, 0}; //tiempo universal
  int fechaTU[] = {0, 0, 0}; //fecha del tiempo universal

  TU[0] = TL[0] - horaUTC; // paso al tiempo universal
  TU[1] = TL[1];
  TU[2] = TL[2];
  if (TU[0] >= 24){
    TU[0] = 0;
    fecha[0] = fecha[0]+1;
  }else if (TU[0] < 0){
    TU[0] = TU[0] + 24;
    fecha[0] = fecha[0]-1;
  }
  fechaTU[0] = fecha[0];
  fechaTU[1] = fecha[1];
  fechaTU[2] = fecha[2];



  if (TU[0] >= 24) {
    // si las horas superan las 24 horas, el dia sera el siguiente 
    fechaTU[0] = fecha[0] + 1;
    TU[0] = TU[0] - 24;
  }
  float fracdia =  ( TU[0] * 3600.0 + TU[1] * 60.0 + TU[2] ) / ( 24.0 * 3600.0 );
  if (fechaTU[1] == 1 || fechaTU[1] == 2) {
    fechaTU[2] = fechaTU[2] - 1;
    fechaTU[1] = fechaTU[1] + 12;
  }


  float FJ = int(365.25 * (fechaTU[2] + 4716 ) ) + int( 30.6001 * ( fechaTU[1] + 1 ) ) - int( fechaTU[2] / 100 ) + int( int ( fechaTU[2] / 100 ) / 4 ) + fechaTU[0] + fracdia -1522.5; //ecuacion 7.15 Portilla
  float T = ( FJ - 2451545.0 ) / 36525; // ecuacion 7.17 Portilla
  float TSG0 = 6.697374558 + 2400.051337 * T + 258.61111E-06 *T*T; // ecuacion 7.16 portilla
  TSG0 = TSG0 - int(TSG0/ 24)*24;
  //TSG0 = 10.80423889
  float TSGt = TSG0 + (TU[0] + TU[1]/60. + TU[2]/3600.) * 1.0027379; // ecuacion 7.12 portilla
  float TSL =  TSGt + longgrad/15.0; // ecuacion 7.13 portilla

  // Transformacion de coordenadas ecuatoriales absolutas (AR, DEC) a coordenadas ecuatoriales horarias (H, DEC)

  float Hhoras = TSL - ARhoras; //hallando el angulo horario
  if (Hhoras<0.0) {
    Hhoras = Hhoras + 24.0;
  }
  // en este punto ya se han convertido las coordenadas ecuatoriales absolutas (AR, DEC) a coordenadas ecuatoriales horarias (H, DEC). Continua con la 
  //transformacion de coordenadas ecuatoriales horarias a coordenadas horizontales.

  //conversion de resultados
  float H[] = {int(Hhoras), abs(int((Hhoras - int(Hhoras))* 60.)), abs( ( ( Hhoras- int(Hhoras) ) * 60. -  int((Hhoras - int(Hhoras)) * 60.)) * 60.)};
  float Hrad = radians(Hhoras*15);

  float hrad = asin ( sin(DECrad) * sin(latitudrad) + cos(DECrad) * cos(latitudrad) * cos(Hrad) ); // calculo de la Altura h
  float Arad = acos ( ( sin(DECrad) - sin(latitudrad) * sin(hrad) ) / ( cos(latitudrad) * cos(hrad) ) ); // calculo del Azimuth

  //reglas para cuadrantes

  if (H[0] < 12) {
    Arad = 2.0 * PI - Arad;
  }  
  // Conversion de resultado

  //float hdeg = degrees(hrad);
  hdeg = degrees(hrad);
  float h[] = {int(hdeg), abs(int((hdeg - int(hdeg)) * 60.)), abs( ( ( hdeg - int(hdeg ) ) * 60. -  int((hdeg - int(hdeg)) * 60.)) * 60.   )  }; // de [radianes] a [grados, minutos, segundos] 

  //float Adeg = degrees(Arad);
  Adeg = degrees(Arad);
  float A[] = {int(Adeg), abs(int((Adeg - int(Adeg))* 60.)), abs( ( ( Adeg - int(Adeg ) ) * 60. -  int((Adeg - int(Adeg)) * 60.)) * 60.   )}; // de [radianes] a [grados, minutos, segundos]


  // Impresion de resultados
  //print FechaHoraLocal TiempoHoraLocal FechaTU HoraTU FechaJuliana CoorEcuaAbs CoorEcuaHor CoorHoriz
  println("Fecha y tiempo local");
  println(fecha[0] +","+ fecha[1] +","+  fecha[2] +" TL:"+  TL[0] +","+  TL[1] +","+  TL[2]);
  println("Fecha y hora UT");
  println(fechaTU[0] +","+  fechaTU[1] +","+  fechaTU[2]);
  println(TU[0] +","+  TU[1] +","+  TU[2]);
  //println("Fecha juliana");
  //println(FJ);
  println("Coordenadas ecuatoriales absolutas AR/DEC");
  println(int(AR[0]) +":"+  int(AR[1]) +":"+  AR[2] +"  "+  int(DEC[0]) +":"+  int(DEC[1]) +":"+  DEC[2]);
  //println("Coordenadas ecuatoriales horarias H/DEC");
  //println(H[0] +","+  H[1] +","+  H[2] +"  "+  DEC[0] +","+  DEC[1] +","+  DEC[2]);
  println("Coordenadas horizontales altura / azimut");
  println( int(h[0]) +":"+  int(h[1]) +":"+  h[2] +"  "+  int(A[0]) +":"+  int(A[1]) +":"+  A[2]);
  //*/
  return Adeg;
}