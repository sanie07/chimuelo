#ifndef States_H
#define States_H

#include <Arduino.h> // Incluye funciones básicas de Arduino
#include <xmotionV3.h> // Incluye la libreria xmotionV3
#include "OS.h" // Incluye la cabecera de clase OS
#include "LS.h" // Incluye la cabecera de clase LS 

enum Estado {
    INICIO
    BUSCAR,
    ALINEAR,
    AVANZAR,
    ATAQUE_RAPIDO,
    DETEC_LINEA,
};


class States {
public:
    States(OS& L_OS, OS& LD_OS, OS& C_OS, OS& RD_OS, OS& R_OS, LS& L_OS, LS& R_LS, xmotion& MOTORS);
    void begin();
    void update();

private:
    OS& L_OS; 
    OS& LD_OS; 
    OS& C_OS; 
    OS& RD_OS; 
    OS& R_OS; 
    LS& L_OS; 
    LS& R_LS; 
    xmotion& MOTORS;
    Estado estadoActual;
    unsigned long tiempoBusquedaInicio;
    const unsigned long duracionBusqueda = 1000;   
};

#endif