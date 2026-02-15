#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h> // Incluye funciones básicas de Arduino

class Motor {
private:
    byte _pinA_IN1; // Pin IN1 del motor A
    byte _pinA_IN2; // Pin IN2 del motor A
    byte _pinB_IN1; // Pin IN1 del motor B
    byte _pinB_IN2; // Pin IN2 del motor B

public:
    // Constructor de la clase Motor.
    // Recibe el número de los 4 pines.
    Motor(byte pinA_IN1, byte _pinA_IN2, byte _pinB_IN1, byte _pinB_IN2);

    // Metodo para inicializar los pines.
    void begin();

    // Metodos para el control de los motores
    // Para controlar las velocidades de los dos motores:
    void MotorControl(int left_speed, int right_speed);

    //Para parar los motores:
    void StopMotors(int time);

    // Para girar a la derecha:
    void Right0(int speed, int time);

    // Para girar a la izquierda:
    void Left0(int speed, int time);
};

#endif