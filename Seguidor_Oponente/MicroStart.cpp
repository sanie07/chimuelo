#include "MicroStart.h" // Incluye la cabecera de clase MicroStart

// Constructor de la clase MicroStart
MicroStart::MicroStart(byte Pin) {
    _pin = Pin; // Asigna el pin del sensor
    _start = false; // Asigna el estado inicial como STOP
}

// Metodo begin (inicializa el pin del sensor)
void MicroStart::begin() {
    pinMode(_pin, INPUT); // Configura el PIN como entrada
}

// Metodo lectura (lee el sensor y devuelve su estado)
bool MicroStart::get_start(){
    return _start;
}