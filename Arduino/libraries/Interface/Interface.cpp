
//#include "Arduino.h"
#include "Interface.h"

// definicion metodos miembro clase Interface
Interface::Interface() {
  buffer_datos_recividos = 100;
  depuracion =  false;
  /* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
  conectado = false;
  publicado =  false;
  objetoActual = "";
  comunicacionNRF24L01 = false;
  comunicacionSerial = false;
  contadorObjetosCargados = 0;
  contadorFuncionesCargadas = 0;
  contadorVariablesEstadoCargadas = 0;
}

void Interface::revisarInterface() {
  if (!publicado) {
    publicacion();
  } else {
    revisarVariablesEstado();
  }
  procesarMensaje();
}

void Interface::publicacion() {
  if (!conectado) {
    enviarSaludo();
  } else if (!publicado) {
    enviarIdentificacion();
  }
}

String Interface::procesarMensaje() {
  unsigned long tiempoReintentando = millis();
  String datos = "";
  while ((datos == "") && millis() < tiempoReintentando + 200) {
    datos = recivirMensaje();

    // si hay mensaje valido
    if (datos != "") {

      if (publicado && datos == "PING") {
        enviarMensaje("PONG");

      } else if (datos[0] == '{') {
        ProcesarFuncion(obtenerFuncionJson(datos));

      } else if (datos == "HOLA-IDENTIFICATE") {
        conectado = true;

      } else if (datos == "BIENVENIDO") {
        publicado = true;

      } else if (datos.substring(0, 2) == "OK") {
        variablesEstadoReconocida(datos);
      }

    } // fin if mensaje valido
  } // fin while escucha

  return datos;
}

void Interface::revisarVariablesEstado() {
  for (int i = 0; i < contadorVariablesEstadoCargadas; i++ ) {
    if (*apuntadoresVariablesEstado[i] != VariablesEstadoAnterior[i]) {
      enviarMensaje("{\"" + nombreObjetosVariablesEstado[i] + "\":{\"" + nombreVariablesEstado[i] + "\":\"" + *apuntadoresVariablesEstado[i] + "\"}}");
    }
  }
}

void Interface::variablesEstadoReconocida(String variableReconocida) {
  variableReconocida = variableReconocida.substring(3, variableReconocida.length());
  //(*serialDepuracion).println(variableReconocida);
  for (int i = 0; i < contadorVariablesEstadoCargadas; i++ ) {
    if (*apuntadoresVariablesEstado[i] != VariablesEstadoAnterior[i] && variableReconocida == nombreVariablesEstado[i]) {
      VariablesEstadoAnterior[i] = *apuntadoresVariablesEstado[i] ;
      if (depuracion) {
        (*serialDepuracion).print("Cambio Estado ");
        (*serialDepuracion).print(variableReconocida);
        (*serialDepuracion).println(" Reconocido");
      }
    }
  }
}

String Interface::obtenerFuncionJson(String datos) {
  int largoDatos = datos.length();
  int puntos = 0;
  for (int i = 0; i < largoDatos; i++) {
    if (datos[i] == ':') {
      puntos = i;
    }
  }

  if (depuracion)(*serialDepuracion).print("Lectura Funcion: ");
  if (datos[puntos] == ':' && datos[0] == '{' && datos[largoDatos - 1] == '}'
      && datos[largoDatos - 2] == '\"' && datos[1] == '\"' && datos[puntos - 1] == '\"' && datos[puntos + 1] == '\"') {

    String objeto = datos.substring(2, puntos - 1);
    String funcion = datos.substring(puntos + 2, largoDatos - 2);
    if (depuracion)(*serialDepuracion).println("Exitosa");

    if (depuracion) {
      (*serialDepuracion).print(objeto);
      (*serialDepuracion).print(" ");
      (*serialDepuracion).println(funcion);
      (*serialDepuracion).println();
    }
    return funcion;
  } else {
    if (depuracion)(*serialDepuracion).println("Fallida");
  }
  if (depuracion) {(*serialDepuracion).println();}
  return "";
}

void Interface::ProcesarFuncion(String funcionRequerida) {
  for (int i = 0; i < contadorFuncionesCargadas; i++ ) {
    if (nombreFunciones[i] == funcionRequerida)
      (*apuntadoresFunciones[i])();
  }
}

void Interface::agregarObjeto(const String nombreObjetoCargar, String objetoPadre) {
  if (depuracion) {
    (*serialDepuracion).print("Cargando Objeto: ");
    (*serialDepuracion).print(nombreObjetoCargar);
  }

  if (contadorObjetosCargados > 0 && objetoPadre != "") {
    // verificamos que el padre exista
    bool objetoPadreExiste = false;
    for (byte i = 0; i <= contadorObjetosCargados; i++) {
      if (nombreObjetos[i] == objetoPadre) {
        objetoPadreExiste = true; break;
      }
    }
    if (!objetoPadreExiste) {
      if (depuracion) {
        (*serialDepuracion).println(" ...No cargado, El Padre No Existe");
      }
      return;
    }
  } else if (contadorObjetosCargados == 0 && objetoPadre != "") {
    if (depuracion) {
      (*serialDepuracion).println(" ..El Primer Objeto No Tiene Padre");
    }
    objetoPadre = "";
  }

  nombreObjetos[contadorObjetosCargados] = nombreObjetoCargar;

  if (objetoPadre == "") {
    nombreObjetosObjetos[contadorObjetosCargados] = objetoActual;
  } else {
    nombreObjetosObjetos[contadorObjetosCargados] = objetoPadre;
  }

  objetoActual = nombreObjetoCargar;
  contadorObjetosCargados++;

  if (depuracion) {
    (*serialDepuracion).println(" ...Cargado");
  }
}

void Interface::agregarFuncion(String nombreFuncion, void (*apuntadorFuncion)()) {
  if (depuracion) {
    (*serialDepuracion).print("Cargando Funcion: ");
    (*serialDepuracion).print(nombreFuncion);
  }
  if (objetoActual != "") {
    // cargamos la funcion al vector de apuntadores a funciones
    apuntadoresFunciones[contadorFuncionesCargadas] = apuntadorFuncion;     // guardamos el apuntador a la funcion
    nombreFunciones[contadorFuncionesCargadas] = nombreFuncion;             // guardamos el nombre de la funcion
    nombreObjetosFuncion[contadorFuncionesCargadas] = objetoActual;         // asignamos esta funcion a un objeto
    contadorFuncionesCargadas++;

    if (depuracion) {
      (*serialDepuracion).println(" ...Cargada");
      delay(5);   // espero para evitar sobrecargar el buffer de salida
    }

  } else {
    if (depuracion)(*serialDepuracion).println(" ...No Cargada, Agrege Objeto Primero");
  }
}

void Interface::agregarVariableEstado(String nombreVariableEstado, bool *apuntadorVariableEstado ) {
  if (depuracion) {
    (*serialDepuracion).print("Cargando Var Est: ");
    (*serialDepuracion).print(nombreVariableEstado);
  }
  // cargamos la funcion al vector de apuntadores a funciones
  if (objetoActual != "") {
    apuntadoresVariablesEstado[contadorVariablesEstadoCargadas] = (apuntadorVariableEstado);    // guardamos el apuntador a la variable de estado
    nombreVariablesEstado[contadorVariablesEstadoCargadas] = nombreVariableEstado;            // guardamos el nombre de la variable de estado
    nombreObjetosVariablesEstado[contadorVariablesEstadoCargadas] = objetoActual;             // asignamos esta variable a un objeto
    VariablesEstadoAnterior[contadorVariablesEstadoCargadas] = *apuntadorVariableEstado;      // inicializamos para saber si cambia
    contadorVariablesEstadoCargadas++;

    if (depuracion) {
      (*serialDepuracion).println(" ...Cargada");
      delay(5);
    }

  } else {
    if (depuracion)(*serialDepuracion).println(" ...No Cargada, Agrege Objeto Primero");
  }
}


void Interface::enviarSaludo() {
  if (comunicacionNRF24L01) {
    enviarMensaje("HOLA-" + uint64tostring(addresses[1]));  // enviamos hola + la direccion por la que este modulo escucha (deve de cambiar)
  } else if (comunicacionSerial) {
    enviarMensaje("HOLA");
  }
}

void Interface::enviarIdentificacion() {
  static bool modeloListo = false;
  static String modeloJson = "";

  if (modeloListo) {
    enviarMensaje(modeloJson);
  } else {
    modeloJson = componerObjetoJson();
    if (modeloJson != "") {
      modeloListo = true;
    }
  }
}


String Interface::componerObjetoJson(byte numObjeto) {
  //se cargan las funciones y luegos los objetos, para cada objeto empezando con el primer objeto fijo como raiz
  String modeloJson = "";
  String funcionesObjeto = "";
  String VariablesEstadoObjeto = "";


  for (byte funcionRevisar = 0; funcionRevisar < contadorFuncionesCargadas; funcionRevisar++) {

    if (nombreObjetos[numObjeto] == nombreObjetosFuncion[funcionRevisar]) {

      if (funcionesObjeto != "") {
        funcionesObjeto = addArregloJSON(funcionesObjeto, nombreFunciones[funcionRevisar]);
      } else {
        funcionesObjeto = crearArregloJSON(nombreFunciones[funcionRevisar]);
      }

    }

  }
  if (funcionesObjeto != "") {
    funcionesObjeto = crearObjetoJSON("Funciones", funcionesObjeto);
  }


  for (byte variableEstadoRevisar = 0; variableEstadoRevisar < contadorVariablesEstadoCargadas; variableEstadoRevisar++) {

    if (nombreObjetos[numObjeto] == nombreObjetosVariablesEstado[variableEstadoRevisar]) {

      if (VariablesEstadoObjeto != "") {
        VariablesEstadoObjeto = addArregloJSON(VariablesEstadoObjeto, nombreVariablesEstado[variableEstadoRevisar]);
      } else {
        VariablesEstadoObjeto = crearArregloJSON(nombreVariablesEstado[variableEstadoRevisar]);
      }

    }

  }
  if (VariablesEstadoObjeto != "") {
    VariablesEstadoObjeto = crearObjetoJSON("Estados", VariablesEstadoObjeto);
  }

  // consolidamos las funciones y las variables de estado en una arreglo
  if (funcionesObjeto != "") {
    modeloJson = crearArregloJSON(funcionesObjeto);
  } else if (VariablesEstadoObjeto != "") {
    modeloJson = crearArregloJSON(VariablesEstadoObjeto);
  }
  if (funcionesObjeto != "" && VariablesEstadoObjeto != "") {
    modeloJson = addArregloJSON(modeloJson, VariablesEstadoObjeto);
  }

  // para cada uno de los objetos en nombreObjetos[contadorObjetosCargados] verificamos quien es el padre
  // si este objeto es padre de algun objeto se agrega al arreglo de objetos.
  for (byte objetoRevisar = 0; objetoRevisar < contadorObjetosCargados; objetoRevisar++) {

    if (nombreObjetos[numObjeto] == nombreObjetosObjetos[objetoRevisar]) {

      if (modeloJson != "") {
        modeloJson = addArregloJSON(modeloJson, crearObjetoJSON(nombreObjetos[objetoRevisar], componerObjetoJson(objetoRevisar)));
      } else {
        modeloJson = crearArregloJSON(crearObjetoJSON(nombreObjetos[objetoRevisar], componerObjetoJson(objetoRevisar)));
      }

    }

  }
  // creo el objeto principal que contiene a estos objetos hijos
  if (numObjeto == 0) {
    modeloJson = crearObjetoJSON(nombreObjetos[numObjeto], modeloJson);
  }

  return modeloJson;
}

String Interface::crearObjetoJSON(String string, String valor) {
  String objetoJson = "";
  if (valor[0] == '[' || valor[0] == '{' ) {
    objetoJson = "{\"" + string + "\":" + valor + "}";
  } else {
    objetoJson = "{\"" + string + "\":\"" + valor + "\"}";
  }
  return objetoJson;
}

String Interface::parObjetoJSON(String objeto, String string, String valor) {
  int tamObjeto = objeto.length();
  String objetoJson = "";
  if (valor[0] == '[' || valor[0] == '{' ) {
    objetoJson = objeto.substring(0, tamObjeto - 1) + ",\"" + string + "\":" + valor + "}";
  } else {
    objetoJson = objeto.substring(0, tamObjeto - 1) + ",\"" + string + "\":\"" + valor + "\"}";
  }
  return objetoJson;
}

String Interface::crearArregloJSON(String valor) {
  String arregloJson = "";
  if (valor[0] == '[' || valor[0] == '{' ) {
    arregloJson = "[" + valor + "]";
  } else {
    arregloJson = "[\"" + valor + "\"]";
  }
  return arregloJson;
}

String Interface::addArregloJSON(String arreglo, String valor) {
  int tamArreglo = arreglo.length();
  String arregloJson = "";
  if (valor[0] == '[' || valor[0] == '{' ) {
    arregloJson = arreglo.substring(0, tamArreglo - 1) + "," + valor + "]";
  } else {
    arregloJson = arreglo.substring(0, tamArreglo - 1) + ",\"" + valor + "\"]";
  }
  return arregloJson;
}


String Interface::recivirMensaje() {
  if (comunicacionNRF24L01) {
    // nos comunicamos a travez del modulo NRF24L01
    unsigned long tiempoReintentando = millis();
    String mensaje = "";
    while (mensaje == "" && millis() < tiempoReintentando + 300) {
      mensaje =  recivirStringNrf24l01();
    }    
    return mensaje;

  } else if (comunicacionSerial) {
    // nos comunicamos por serial
    return leerLineaSerial();

  } else {
    // nos comunicamos por serial
  }
}

String Interface::leerLineaSerial() {
  // revisar
  String mensaje = "";
  static String bufferDatosSerial = "";
  unsigned long tiempoMaximoLectura = millis();
  while ((*serialComunicacion).available() > 0 && millis() < tiempoMaximoLectura + 200) {
    mensaje = char((*serialComunicacion).read());
  }
  if (mensaje == "\r" || mensaje == "\n") {
    mensaje = bufferDatosSerial;
    //(*serialComunicacion).println("OK");
    bufferDatosSerial = "";
    if (depuracion) {
      (*serialDepuracion).print("Recibido: ");
      (*serialDepuracion).println(mensaje);
    }
    return mensaje;
  } else if (mensaje != "\n" && mensaje != "\r" ) {
    bufferDatosSerial += mensaje;
  }
  return "";
}


String Interface::recivirStringNrf24l01() {
  // leemos un primer paquete
  String mensaje = recivirHasta32Nrf24l01();

  // si enviaron paquetes
  if (mensaje[0] == '-') {
    // son varios paquetes, obtengo el numero de paquetes
    byte numeroPaquetes = obtenerNumeroPaquetes(mensaje);

    // preguntamos aunque es obio
    if (numeroPaquetes > 0 ) {
      mensaje = "";       // limpiamos mensaje
      int contadorPaquetesRecibidos = 0;
      unsigned long tiempoInicioMensaje = millis();
      unsigned long maximoTiempoEspera = 200;     // lo que esperamos antes de cancelar el paquete
      while (contadorPaquetesRecibidos < numeroPaquetes && millis() < (tiempoInicioMensaje + maximoTiempoEspera)) {
        // armamos el paquete
        String paqueteLeido = recivirHasta32Nrf24l01();
        if (paqueteLeido != "") {
          mensaje += paqueteLeido;
          contadorPaquetesRecibidos++;
        }
      } // fin while varios paquetes

      // si el paquete no llego completo lo descartamos
      if (contadorPaquetesRecibidos < numeroPaquetes) {
        if (depuracion) {(*serialDepuracion).println("Paquete Incompleto");}
        return "";
      } else {
        return mensaje;
      }
    }// fin numero de paquetes mayor a 0

  } else {
    return mensaje;     // si no esta por paquetes, retornamos el mensaje recivido
  }
}

String Interface::recivirHasta32Nrf24l01() {

  radio.startListening();
  uint8_t len;
  char data[buffer_datos_recividos];
  String mensaje32 = "";

  if (radio.available()) {
    while (radio.available()) {             // While there is data ready
      len = radio.getDynamicPayloadSize();
      radio.read(&data, len);             // Get the payload
      mensaje32 = String(data).substring(0, len);
    }
    if (depuracion) {
      (*serialDepuracion).print("Recibido: #");
      (*serialDepuracion).print(len);
      (*serialDepuracion).print(" ");
      (*serialDepuracion).println(mensaje32);
    }
    return mensaje32;
  } else {
    return "";
  }
}


void Interface::enviarMensaje(String mensaje) {
  if (comunicacionNRF24L01) {
    // nos comunicamos a travez del modulo NRF24L01
    unsigned long tiempoReintentando = millis();
    while (!enviarStringNrf24l01(mensaje) && millis() < tiempoReintentando + 200) {}

  } else if (comunicacionSerial) {
    // nos comunicamos por serial
    (*serialComunicacion).println(mensaje);
  } else {

  }
}

bool Interface::enviarStringNrf24l01(String mensaje) {
  // convertimos el string de salida a un arreglo de caracteres usado por radio.write
  unsigned int largoMensaje = mensaje.length();

  unsigned int numeroPaquetes  = largoMensaje / 32;
  if (largoMensaje % 32 > 0) {numeroPaquetes++;}
  if (numeroPaquetes > 1) {
    if (depuracion) {
      (*serialDepuracion).print(" Lm: ");
      (*serialDepuracion).print(largoMensaje);
      (*serialDepuracion).print(" #P: ");
      (*serialDepuracion).println(numeroPaquetes);
    }
    // envimos el paquete de inicio
    enviarHasta32CharNrf24l01("-" + String(numeroPaquetes) + "-");
  }
  for (int i = 0; i < numeroPaquetes; i++) {
    unsigned int fin  = (i + 1) * 32;
    if (fin >= largoMensaje) {
      fin = largoMensaje;   // para el ultimo paquete que no es de tamaño 32*i
    }
    String paquete = mensaje.substring(i * 32, fin);

    //bool noEnviado = true;
    unsigned long tiempoInicioMensaje = millis();
    unsigned long maximoTiempoEspera = 100;     // lo que esperamos antes de dejar de reenviar
    while (!enviarHasta32CharNrf24l01(paquete) && millis() < (tiempoInicioMensaje + maximoTiempoEspera)) {} // fin while reenvio

    if (!(millis() < (tiempoInicioMensaje + maximoTiempoEspera))) {
      if (depuracion)(*serialDepuracion).println("-Tiempo- Mensaje Cancelado");
      return false;   // canselamos el envio del paquete
    }
  } // fin for paquetes
  // si se logra enviar el mensaje completo
  return true;
}

bool Interface::enviarHasta32CharNrf24l01(String mensaje) {
  radio.stopListening();
  // convertimos el string de salida a un arreglo de caracteres usado por radio.write
  char datos[mensaje.length()];
  strncpy(datos, mensaje.c_str(), sizeof(datos));
  bool enviado = radio.write( &datos, sizeof(datos) );

  if (depuracion) {
    (*serialDepuracion).print("Enviando: #");
    (*serialDepuracion).print(mensaje.length());
    (*serialDepuracion).print(" ");
    (*serialDepuracion).print(mensaje);
    // enviamos los datos
    if (enviado) {
      (*serialDepuracion).println(F(" Exitoso"));
    } else {
      (*serialDepuracion).println(F(" Fallido"));
    }
  }
  radio.startListening();
  return enviado;
}


void Interface::usarNRF24L01() {
  comunicacionNRF24L01 = true;
  radio.init(7, 8);
  configurarRadioModulo();
}

void Interface::usarSerial(HardwareSerial& serial) {
  comunicacionSerial = true;
  serialComunicacion = &serial;
}

void Interface::activarDepuracion(HardwareSerial& serial) {
  depuracion =  true;
  serialDepuracion = &serial;
  (*serialDepuracion).println("Depuracion Interfaz: Activada");
}


void Interface::configurarRadioModulo() {
  radio.begin();
  // RF24_PA_MAX is default-> requiere capacitor
  radio.setPALevel(RF24_PA_LOW);
  radio.enableDynamicPayloads();
  // configuracion direcciones por defecto
  radio.openWritingPipe(addresses[0]);      // escribimos a la dirrecion de la interfaz por defecto
  radio.openReadingPipe(1, addresses[1]);   // escuchamos en nuestra direccion por defecto
}


int Interface::obtenerNumeroPaquetes(String paquete) {
  String numero = "";
  for (int i = 0; i < paquete.length(); i++) {
    if (paquete[i] != '-') {
      numero += paquete[i];
    }
  }
  return numero.toInt();
}

String Interface::uint64tostring(uint64_t numero) {
  String numerostring = "";
  uint64_t xx = numero / 1000000000ULL;

  if (xx > 0) numerostring = String((long)xx);
  numerostring += String((long)(numero - xx * 1000000000));
  return numerostring;
}

