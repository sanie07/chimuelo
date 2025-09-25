#ifndef LS_H
#define LS_H

#include <Arduino.h> // Incluye funciones básicas de Arduino

class LS {
private:
    byte _pin; // Pin del sensor de linea
    bool _estado; // Estado = 1: BLANCO, Estado = 0: NEGRO

public:
    // Constructor de la clase LS.
    // Recibe el número del pin.
    LS(byte Pin);

    // Metodo para inicializar el pin.
    void begin();

    // Metodo para leer el sensor y devolver el estado.
    bool lectura();
};

#endif