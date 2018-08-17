
#ifndef Interface_h
#define Interface_h

#include <SPI.h>
#include "nRF24L01.h"   // lista de constantes simbolicas modulo rf
#include "RF24.h"
#include "Arduino.h"

// definicion de la clase Interface
class Interface {

public:
    Interface();
    void revisarInterface();

    void agregarObjeto(const String nombreObjeto, String objetoPadre = "");
    void agregarFuncion(String nombreFuncion, void (*apuntadorFuncion)());
    void agregarVariableEstado(String nombreVariableEstado, bool (*apuntadorVariableEstado));

    void usarNRF24L01();
    void usarSerial(HardwareSerial& serial);
    void activarDepuracion(HardwareSerial& serial);

private:
    // variables otras clases
    HardwareSerial *serialDepuracion;
    HardwareSerial *serialComunicacion;
    RF24 radio;

    // variables propias
    bool depuracion;
    bool conectado;
    bool publicado;
    String objetoActual;

    // variables comunicaciones
    bool comunicacionNRF24L01;
    bool comunicacionSerial;
    const uint64_t addresses[2] = {0xF0F0F0F0E1, 0xF0F0F0F0D2}; // direcciones por defecto de envio y de escucha
    uint64_t direcciones[2] = {0xF0F0F0F0E1, 0xF0F0F0F0D2}; // direcciones de envio y de escucha
    uint8_t buffer_datos_recividos;

    // variables manejo de funciones y objetos
    static const byte cantidadMaximaObjetos = 6;
    static const byte cantidadMaximaFunciones = 7;
    static const byte cantidadMaximaVariablesEstado = 5;
    byte contadorObjetosCargados;
    byte contadorFuncionesCargadas;
    byte contadorVariablesEstadoCargadas;

    String nombreObjetos[cantidadMaximaObjetos];
    bool ObjetosHermanos[cantidadMaximaObjetos];

    String nombreObjetosObjetos[cantidadMaximaObjetos];
    String nombreObjetosFuncion[cantidadMaximaFunciones];
    String nombreObjetosVariablesEstado[cantidadMaximaVariablesEstado];

    String nombreFunciones[cantidadMaximaFunciones];
    String nombreVariablesEstado[cantidadMaximaVariablesEstado];
    bool (VariablesEstadoAnterior[cantidadMaximaVariablesEstado]);

    void (*apuntadoresFunciones[cantidadMaximaFunciones])();
    bool (*apuntadoresVariablesEstado[cantidadMaximaVariablesEstado]);

    // funciones privadas
    void publicacion();
    String procesarMensaje();
    void revisarVariablesEstado();
    void variablesEstadoReconocida(String variableReconocida);
    String obtenerFuncionJson(String datos);
    void ProcesarFuncion(String funcionRequerida);

    void enviarSaludo();
    void enviarIdentificacion();

    String componerObjetoJson(byte numObjeto = 0);
    String crearObjetoJSON(String string, String valor);
    String parObjetoJSON(String objeto, String string, String valor);
    String crearArregloJSON(String valor);
    String addArregloJSON(String objeto, String valor);

    String recivirMensaje();
    String leerLineaSerial() ;
    String recivirStringNrf24l01();
    String recivirHasta32Nrf24l01();

    void enviarMensaje(String mensaje);
    bool enviarStringNrf24l01(String mensaje);
    bool enviarHasta32CharNrf24l01(String mensaje);

    void configurarRadioModulo();

    int obtenerNumeroPaquetes(String paquete);
    String uint64tostring(uint64_t numero);

};

#endif

