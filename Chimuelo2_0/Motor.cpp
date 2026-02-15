#include "Motor.h"

// Constructor Motor
Motor::Motor(byte pinL_IN1, byte _pinL_IN2, byte _pinR_IN1, byte _pinR_IN2){
    _pinL_IN1 = pinL_IN1;
    _pinL_IN2 = pinL_IN2;
    _pinR_IN1 = pinR_IN1;
    _pinR_IN2 = pinR_IN2;
}

// Metodo: begin
void Motor::begin(){
    pinMode(_pinL_IN1, OUTPUT);
    pinMode(_pinL_IN2, OUTPUT);
    pinMode(_pinR_IN1, OUTPUT);
    pinMode(_pinR_IN2, OUTPUT);
}

// Metodo: MotorControl
void Motor::MotorControl(int left_speed, int right_speed){
    // Para el motor izquierdo:
    if(left_speed >= 0 && left_speed <= 255){
        analogWrite(_pinL_IN1, left_speed);
        digitalWrite(_pinL_IN2, LOW);
    }
    else if(left_speed < 0 && left_speed >= -255){
        analogWrite(_pinL_IN2, left_speed);
        digitalWrite(_pinL_IN1, LOW);
    }
    else if(left_speed > 255){
        analogWrite(_pinL_IN1, 255);
        digitalWrite(_pinL_IN2, LOW);
    }
    else{
        analogWrite(_pinL_IN2, 255);
        digitalWrite(_pinL_IN1, LOW);
    }

    // Pra el motor derecho:
    if(left_speed >= 0 && left_speed <= 255){
        analogWrite(_pinL_IN1, left_speed);
        digitalWrite(_pinL_IN2, LOW);
    }
    else if(left_speed < 0 && left_speed >= -255){
        analogWrite(_pinL_IN2, left_speed);
        digitalWrite(_pinL_IN1, LOW);
    }
    else if(left_speed > 255){
        analogWrite(_pinL_IN1, 255);
        digitalWrite(_pinL_IN2, LOW);
    }
    else{
        analogWrite(_pinL_IN2, 255);
        digitalWrite(_pinL_IN1, LOW);
    }  
}

// Metodo: StopMotors
void Motor::StopMotors(int time){
    digitalWrite(_pinL_IN1, LOW);
    digitalWrite(_pinL_IN2, LOW);
    digitalWrite(_pinR_IN1, LOW);
    digitalWrite(_pinR_IN2, LOW);

    delay(time);
}

// Metodo: Right0
void Motor::Right0(int speed, int time){
    analogWrite(_pinL_IN1, speed);
    digitalWrite(_pinL_IN2, LOW);
    analogWrite(_pinR_IN2, speed);
    digitalWrite(_pinR_IN1, LOW);

    delay(time);
}

// Metodo: Left0
void Motor::Left0(int speed, int time){
    analogWrite(_pinL_IN2, speed);
    digitalWrite(_pinL_IN1, LOW);
    analogWrite(_pinR_IN1, speed);
    digitalWrite(_pinR_IN2, LOW);

    delay(time);
}
