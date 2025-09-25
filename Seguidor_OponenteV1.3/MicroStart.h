#ifndef MicroStart_H
#define MicroStart_H

#include <Arduino.h> // Incluye funciones básicas de Arduino

class MicroStart {
private:
    byte _pin; // Pin del sensor IR
    bool _start; // Estado = 1: START, Estado = 0: STOP

public:
    // Constructor de la clase MicroStart.
    // Recibe el número del pin.
    MicroStart(byte Pin);

    // Metodo para inicializar el pin.
    void begin();

    // Metodo para leer el sensor y devolver el estado.
    bool get_start();
};

#endif