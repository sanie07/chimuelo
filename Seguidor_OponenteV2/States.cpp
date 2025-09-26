#include "States.h" // Incluye la cabecera de clase OS
#include <xmotionV3.h> // Incluye la libreria xmotionV3

//////////////////////////  VARIABLES AUXILIARES  //////////////////////////

// Velocidades:
int max_speed = 150;   //velocidad maxima
int fast_speed = 65;   // velocidad para el frente
int mean_speed = 50;   // velocidad promedio de busqueda
int var_speed = 18;    // velocidad variable de busqueda
int speed;
int left_speed;
int rigth_speed;

// Giro
int delay_90grados = 19;
int delay_180grados = delay_90grados*2;

// Tiempo
unsigned long duracionBusqueda = 1000; 


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
    // Lee los sensores
    bool Read_OS[] = {L_OS.lectura(), LD_OS.lectura(), C_OS.lectura(), RD_OS.lectura(), R_OS.lectura()};
    bool Read_LS[] = {L_LS.lectura(), R_LS.lectura()};

    //////////////////////////////////  CONDICIONALES  ////////////////////////////////
    bool oponente = (Read_OS[1] || Read_OS[2] || Read_OS[3] || Read_OS[0] || Read_OS[4]);
    bool centro = (Read_OS[2] && !Read_OS[0] && !Read_OS[1] && !Read_OS[3] && !Read_OS[4]);
    bool centro_y_diagonales = (Read_OS[1] && Read_OS[2] && Read_OS[3]);
    
    // Giro medio
    bool centro_y_diagonal_izq = (Read_OS[1] && Read_OS[2] && !Read_OS[3]);
    bool centro_y_diagonal_der = (Read_OS[2] && Read_OS[3] && !Read_OS[1]);
    bool izquierda_y_diagonal_izq = (Read_OS[0] && Read_OS[1] &&!Read_OS[2]);
    bool derecha_y_diagonal_der = (Read_OS[4] && Read_OS[3] && !Read_OS[2]);  

    // Giro mayor
    bool diagonal_izq = (Read_OS[0] && !Read_OS[1] && !Read_OS[2] && !Read_OS[3]);
    bool diagonal_der = (Read_OS[4] && !Read_OS[1] && !Read_OS[2] && !Read_OS[3]); 

    // Sensores de linea
    bool linea_izq = Read_LS[0];
    bool linea_der = Read_LS[1];


    //Prioridad: línea
    if(Read_LS[0] || Read_LS[1]){
        estadoActual = DETEC_LINEA;
    } 
        switch (estadoActual) {
            case INICIO:
                if (oponente) {
                    estadoActual = ALINEAR;
                }else{
                    estadoActual = BUSCAR;
                }
                break;

            case BUSCAR:
                if (millis() - tiempoBusquedaInicio < duracionBusqueda) {
                    xmotion.MotorControl(mean_speed, -mean_speed); //Gira para escanear
                } else if (millis() - tiempoBusquedaInicio < 1.25*duracionBusqueda) {
                    xmotion.MotorControl(mean_speed, mean_speed); //Avanza para escanear
                } else {
                    tiempoBusquedaInicio = millis(); // Reiniciar el temporizador de búsqueda
                }
                
                // Veriificar si se detecta el oponente
                if (oponente) {
                    estadoActual = ALINEAR;
                }
                break;

            case ALINEAR:
                // Centro
                if(centro){
                    estadoActual = AVANZAR;
                }else if(centro_y_diagonal_izq || izquierda_y_diagonal_izq){
                    xmotion.MotorControl(mean_speed, -mean_speed);
                }else if(centro_y_diagonal_der  || izquierda_y_diagonal_der){
                    xmotion.MotorControl(-mean_speed, mean_speed);
                }else if(diagonal_izq){
                    xmotion.MotorControl(fast_speed, -fast_speed);
                }else if(diagonal_der){
                    xmotion.MotorControl(-fast_speed, fast_speed);
                }else {
                    estadoActual = BUSCAR;
                }              
                
                break;

            case AVANZAR:
                if(centro){
                    xmotion.MotorControl(fast_speed, fast_speed);
                    if(centro_y_diagonales){
                        estadoActual = ATAQUE_RAPIDO;
                    }
                }else{
                    estadoActual = ALINEAR;
                }
                break;
            
            case ATAQUE_RAPIDO:
                xmotion.MotorControl(max_speed, max_speed);
                if(!centro_y_diagonales){
                    estadoActual = ALINEAR;
                }
                break;


            case DETEC_LINEA:
                if(linea_izq && linea_der){
                xmotion.MotorControl(-fast_speed, -0.6 * fast_speed);
                delay(500); // tiempo de retroceso
                xmotion.StopMotors(1);
                }      
                if(Read_LS[0] && !Read_LS[1]){
                    xmotion.Right0(100, delay_90grados);
                    estadoActual = INICIO;
                }
                else if(!Read_LS[0] && Read_LS[1]){
                    xmotion.Left0(100, delay_90grados);
                    estadoActual = INICIO;
                }
                else{
                    xmotion.Right0(100, delay_180grados);
                    estadoActual = INICIO;
                }
                break;
    }
}


