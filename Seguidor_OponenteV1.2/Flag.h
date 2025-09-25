#ifndef Flag_H
#define Flag_H

#include <Arduino.h> // Incluye funciones básicas de Arduino

class Flag {
private:
    byte _pin; // Pin de la bandera.
    bool _abierto; // Estado = 1: ABIERTO, Estado = 0: CERRADO

public:
    // Constructor de la clase Flag.
    // Recibe el número del pin.
    Flag(byte Pin);

    // Metodo para inicializar el pin.
    void begin();

    // Metodo para abrir la bandera.
    void matador();

    // Metodo para saber su estado.
    bool get_abierto();

};

#endif