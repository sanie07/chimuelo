#include "States.h" // Incluye la cabecera de clase OS
#include <xmotionV3.h> // Incluye la libreria xmotionV3

//Esperar para apagar
unsigned long tiempoEspera = 5000;
unsigned long tiempoEsperaInicio = 0;

// Constructor de la clase States.
States::States(OS& L_OS, OS& LD_OS, OS& C_OS, OS& RD_OS, OS& R_OS, LS& L_LS, LS& R_LS)
    : L_OS(L_OS), LD_OS(LD_OS), C_OS(C_OS), RD_OS(RD_OS), R_OS(R_OS), L_LS(L_LS), R_LS(R_LS), estadoActual(INICIO), tiempoBusquedaInicio(millis()) {}

void States::begin() {
    // Inicializa los pines de los sensores
    L_OS.begin();
    LD_OS.begin();
    C_OS.begin();
    RD_OS.begin();
    R_OS.begin();
    L_LS.begin();
    R_LS.begin();
}

void States::update() {

    //Testing giro 180

    // A 50/255
    test_speed = 50;
    duracionGiro180 = 225;
    xmotion.MotorControl(test_speed, -test_speed);
    delay(duracionGiro180);
    xmotion.StopMotors(1000);

    // A 100/255
    test_speed = 100;
    duracionGiro180 = 100;
    xmotion.MotorControl(test_speed, -test_speed);
    delay(duracionGiro180);
    xmotion.StopMotors(1000);

    // A 150/255
    test_speed = 175;
    duracionGiro180 = 75;
    xmotion.MotorControl(test_speed, -test_speed);
    delay(duracionGiro180);
    xmotion.StopMotors(1000);

    // A 200/255
    test_speed = 200;
    duracionGiro180 = 65;
    xmotion.MotorControl(test_speed, -test_speed);
    delay(duracionGiro180);
    xmotion.StopMotors(1000);

    // A 255/255
    test_speed = 255;
    duracionGiro180 = 50;
    xmotion.MotorControl(test_speed, -test_speed);
    delay(duracionGiro180);
    
    xmotion.StopMotors(10000);
    
}

