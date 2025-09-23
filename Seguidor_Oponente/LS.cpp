#include "LS.h" // Incluye la cabecera de clase OS

// Constructor de la clase OS
LS::LS(byte Pin) {
    _pin = Pin; // Asigna el pin del sensor
    _estado = false; // Asigna el estado inicial como NEGRO
}

// Metodo begin (inicializa el pin del sensor)
void LS::begin() {
    pinMode(_pin, INPUT); // Configura el PIN como entrada
}

// Metodo lectura (lee el sensor y devuelve su estado)
bool LS::lectura(){
    _estado = digitalRead(_pin);
    return _estado;
}