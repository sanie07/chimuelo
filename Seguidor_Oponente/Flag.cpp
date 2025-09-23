#include "Flag.h" // Incluye la cabecera de clase OS

// Constructor de la clase OS
Flag::Flag(byte Pin) {
    _pin = Pin; // Asigna el pin de la bandera
    _abierto = false; // Asigna el estado inicial como CERRADO
}

// Metodo begin (inicializa el pin de la bandera)
void Flag::begin() {
    pinMode(_pin, OUTPUT); // Configura el PIN como salida
}

// Metodo matador (abre la bandera)
void Flag::matador(){
    if(!Flag::_abierto){
        digitalWrite(_pin, HIGH);
        delay(100);
        digitalWrite(_pin, LOW);
        Flag::_abierto = true;
    }
}

// Metodo get_abierto (devuelve el estado de la bandera)
bool Flag::get_abierto(){
    return Flag::_abierto;
}