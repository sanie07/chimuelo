#include "States.h" // Incluye la cabecera de clase OS
#include <xmotionV3.h> // Incluye la libreria xmotionV3

//////////////////////////  VARIABLES AUXILIARES  //////////////////////////

// Velocidades:
int fast_speed = 65;   // velocidad para el frente
int mean_speed = 50;   // velocidad promedio de busqueda
int var_speed = 18;    // velocidad variable de busqueda
int speed;
int left_speed;
int rigth_speed;


// Constructor de la clase States.
States::States(OS& L_OS, OS& LD_OS, OS& C_OS, OS& RD_OS, OS& R_OS, LS& L_LS, LS& R_LS)
    : L_OS(L_OS), LD_OS(LD_OS), C_OS(C_OS), RD_OS(RD_OS), R_OS(R_OS), L_LS(L_LS), R_LS(R_LS), estadoActual(INICIO), tiempoBusquedaInicio(0) {}

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
    // Lee los sensores
    bool Read_OS[] = {L_OS.lectura(), LD_OS.lectura(), C_OS.lectura(), RD_OS.lectura(), R_OS.lectura()};
    bool Read_LS[] = {L_LS.lectura(), R_LS.lectura()};

    bool no_oponente = !(Read_OS[1] && Read_OS[2] && Read_OS[3] && Read_OS[0] && Read_OS[4]);
    


    //Prioridad: línea (si no nos estan empujando)
    if((Read_LS[0] || Read_LS[1]) && no_oponente){
        estadoActual = DETEC_LINEA;
    } 
        switch (estadoActual) {
            case INICIO:
                xmotion.StopMotors(1);
                if (!no_oponente) {
                    estadoActual = ALINEAR;
                }else{
                    estadoActual = BUSCAR;
                }
                break;

            case BUSCAR:
                if (millis() - tiempoBusquedaInicio < duracionBusqueda) {
                    xmotion.MotorControl(mean_speed, -mean_speed); //Gira para escanear
                } else if (millis() - tiempoBusquedaInicio > duracionBusqueda) {
                    // Si ha pasado el tiempo de búsqueda, girar 180 grados
                    Giro_180grados();
                    tiempoBusquedaInicio = millis(); // Reiniciar el temporizador de búsqueda
                } else {
                    // Girar lentamente para buscar al oponente
                    Giro_derecha(0.5); 
                }
                break;
            case ALINEAR:
                if (Read_OS[2] && !Read_OS[1] && !Read_OS[3]) {
                    estadoActual = AVANZAR;
                } else if (Read_OS[1] && !Read_OS[3]) {
                    // Girar a la izquierda
                    Giro_izquierda(1.0);
                } else if (Read_OS[3] && !Read_OS[1]) {
                    // Girar a la derecha
                    Giro_derecha(1.0);
                } else if (!no_oponente) {
                    // Si no hay oponente, buscar
                    estadoActual = BUSCAR;
                    tiempoBusquedaInicio = millis();
                }
                break;

            case AVANZAR:
                if (!Read_OS[2]) {
                    estadoActual = ALINEAR;
                } else {
                    Frente_rapido();
                }
                break;



            case DETEC_LINEA:
                // Retrocede y gira 180°
                MOTORS.MotorControl(-fast_speed, -fast_speed);
                delay(350); // tiempo de retroceso
                MOTORS.StopMotors(1);
                if(Read_LS[0] && !Read_LS[1]){
                    Giro_90grados_derecha();
                }
                else if(!Read_LS[0] && Read_LS[1]){
                    Giro_90grados_izquierda();
                }
                else{
                    Giro_180grados();
                }
                estadoActual = INICIO; // Volver al estado inicial después de evitar la línea
                break;
        
    }


// Funcion para cuando es: Centro.
void Frente_rapido(){
  xmotion.MotorControl(fast_speed, fast_speed);
}

// Funcion para cuando es: Derecha-diagonal o Derecha-centrada
void Giro_derecha(float num_prop){
  speed = int(num_prop*var_speed);
  left_speed = mean_speed + speed;
  rigth_speed = mean_speed - speed;
  xmotion.MotorControl(rigth_speed, left_speed);
}

// Funcion para cuando es: Izquierda-diagonal o Izquierda-centrada
void Giro_izquierda(float num_prop){
  speed = int(num_prop*var_speed);
  left_speed = mean_speed - speed;
  rigth_speed = mean_speed + speed;
  xmotion.MotorControl(rigth_speed, left_speed);
}

// Funcion para cuando es: Derecha
void Giro_90grados_derecha(){
  xmotion.Right0(100, delay_90grados);
}

// Funcion para cuando es: Izquierda
void Giro_90grados_izquierda(){
  xmotion.Left0(100, delay_90grados);
}

// Funcion para cuando ningun sensor detecta
void Giro_180grados(){
  xmotion.Right0(100, delay_180grados);
}