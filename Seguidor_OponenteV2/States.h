#ifndef States_H
#define States_H

#include <Arduino.h> // Incluye funciones básicas de Arduino
#include <xmotionV3.h> // Incluye la libreria xmotionV3
#include "OS.h" // Incluye la cabecera de clase OS
#include "LS.h" // Incluye la cabecera de clase LS 

enum Estado {
    INICIO,
    BUSCAR,
    ALINEAR,
    AVANZAR,
    ATAQUE_RAPIDO,
    DETEC_LINEA,
    RETROCESO_LINEA,
    RETROCESO_LINEA_DERECHA,
    RETROCESO_LINEA_IZQUIERDA,
    GIRO180,
};


class States {
public:
    States(OS& L_OS, OS& LD_OS, OS& C_OS, OS& RD_OS, OS& R_OS, LS& L_LS, LS& R_LS);
    void begin();
    void update();

private:
    OS& L_OS; 
    OS& LD_OS; 
    OS& C_OS; 
    OS& RD_OS; 
    OS& R_OS; 
    LS& L_LS; 
    LS& R_LS; 

        // --- Logica de estados ---
    Estado estadoActual;
    unsigned long tiempoBusquedaInicio;
    unsigned long tiempoRetrocesoInicio;
    unsigned long tiempoGiro180Inicio;

    // --- Parametros de comportamiento
    const int max_speed = 70;
    const int fast_speed = 50;
    const int mean_speed = 20;
    int left_speed;
    int rigth_speed;

    const unsigned long duracionBusqueda = 1000; 
    const unsigned long duracionGiro90 = 200; 
    const unsigned long duracionGiro180 = duracionGiro90 * 2; 
    const unsigned long duracionRetroceso = 500;

};

#endif