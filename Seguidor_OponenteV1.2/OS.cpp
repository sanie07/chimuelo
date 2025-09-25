#include "OS.h" // Incluye la cabecera de clase OS

// Constructor de la clase OS
OS::OS(byte Pin) {
    _pin = Pin; // Asigna el pin del sensor
    _estado = false; // Asigna el estado inicial como LOW
}

// Metodo begin (inicializa el pin del sensor)
void OS::begin() {
    pinMode(_pin, INPUT); // Configura el PIN como entrada
}

// Metodo lectura (lee el sensor y devuelve su estado)
bool OS::lectura(){
    _estado = digitalRead(_pin);
    return _estado;
}