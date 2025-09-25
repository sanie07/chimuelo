#ifndef OS_H
#define OS_H

#include <Arduino.h> // Incluye funciones básicas de Arduino

class OS {
private:
    byte _pin; // Pin del sensor oponente
    bool _estado; // Estado = 1: HIGH, Estado = 0: LOW

public:
    // Constructor de la clase OS.
    // Recibe el número del pin.
    OS(byte Pin);

    // Metodo para inicializar el pin.
    void begin();

    // Metodo para leer el sensor y devolver el estado.
    bool lectura();
};

#endif