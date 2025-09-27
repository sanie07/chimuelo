#include "States.h" // Incluye la cabecera de clase OS
#include <xmotionV3.h> // Incluye la libreria xmotionV3

//////////////////////////  VARIABLES AUXILIARES  //////////////////////////

// Velocidades:
int max_speed = 70;   //velocidad maxima
int fast_speed = 50;   // velocidad para el frente
int mean_speed = 20;   // velocidad promedio de busqueda
int speed;
int left_speed;
int rigth_speed;

// Tiempo
unsigned long duracionBusqueda = 1000; 
unsigned long duracionGiro90 = 200; 
unsigned long duracionGiro180 = duracionGiro90 *2; 
unsigned long tiempoRetrocesoInicio;
unsigned long tiempoGiro180Inicio;
unsigned long duracionRetroceso = 500; // 500 ms, lo mismo que el delay original

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
    bool centro = (Read_OS[2] &&  !Read_OS[1] && !Read_OS[3]);
    bool centro_y_diagonales = (Read_OS[1] && Read_OS[2] && Read_OS[3]);
    
    // Giro medio
    bool centro_y_diagonal_izq = (Read_OS[1] && Read_OS[2] && !Read_OS[3]);
    bool centro_y_diagonal_der = (Read_OS[2] && Read_OS[3] && !Read_OS[1]);
    bool solo_diagonal_izq = (Read_OS[1] && !Read_OS[2] && !Read_OS[3]);
    bool solo_diagonal_der = (Read_OS[3] && !Read_OS[1] && !Read_OS[2]);
    
    
    bool izquierda_y_diagonal_izq = (Read_OS[0] && Read_OS[1] &&!Read_OS[2]);
    bool derecha_y_diagonal_der = (Read_OS[4] && Read_OS[3] && !Read_OS[2]);  

    // Giro mayor
    bool no_centro = (!Read_OS[1] && !Read_OS[2] && !Read_OS[3]);
    bool diagonal_izq = (Read_OS[0] && !Read_OS[1] && !Read_OS[2] && !Read_OS[3]);
    bool diagonal_der = (Read_OS[4] && !Read_OS[1] && !Read_OS[2] && !Read_OS[3]); 

    // Sensores de linea
    bool linea_izq = Read_LS[0];
    bool linea_der = Read_LS[1];


    //Prioridad: línea
    if(linea_izq || linea_der){
        tiempoRetrocesoInicio = millis();               // guardamos tiempo de inicio
            if(linea_izq && linea_der){
                estadoActual = RETROCESO_LINEA;} 
            else if(linea_izq){
                estadoActual = RETROCESO_LINEA_IZQUIERDA;}      
            else if (linea_der){
                estadoActual = RETROCESO_LINEA_DERECHA;}   
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
                xmotion.MotorControl(mean_speed, mean_speed); //Avanza para escanear
            } else if (millis() - tiempoBusquedaInicio < 1.25*duracionBusqueda) {
                xmotion.MotorControl(mean_speed, -mean_speed); //Gira para escanear
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
            if(centro || centro_y_diagonales){
                estadoActual = AVANZAR;
            //diagonales y diagonales centradas
            }else if(centro_y_diagonal_izq || solo_diagonal_izq){
                xmotion.MotorControl(mean_speed, 0);
            }else if(centro_y_diagonal_der  || solo_diagonal_der){
                xmotion.MotorControl(0, mean_speed);

            //Costados
            }else if(no_centro){
                //Derecha
                if(diagonal_der){
                xmotion.MotorControl(0, fast_speed);}
                else{
                xmotion.MotorControl(fast_speed, 0);}
            }else {
                estadoActual = BUSCAR;
            }              
            
            break;

        case AVANZAR:
            if(centro || centro_y_diagonales){
                xmotion.MotorControl(fast_speed, fast_speed);
                if(centro_y_diagonales){
                    estadoActual = ATAQUE_RAPIDO;
                }
            }else{
                estadoActual = BUSCAR;
            }
            break;
        
        case ATAQUE_RAPIDO:
            if(centro_y_diagonales){
            xmotion.MotorControl(max_speed, max_speed);}
            else{
                estadoActual = BUSCAR;
            }
            break;
            

        case RETROCESO_LINEA:
            if (millis() - tiempoRetrocesoInicio < duracionRetroceso) {
                xmotion.MotorControl(-fast_speed, -fast_speed);  // retrocede y gira
            }
            else {
                tiempoGiro180Inicio = millis();   
                estadoActual = GIRO180;  
            }
            break;

        case RETROCESO_LINEA_IZQUIERDA:
            if (millis() - tiempoRetrocesoInicio < duracionRetroceso) {
                xmotion.MotorControl(-0.5*fast_speed, -fast_speed);  // retrocede y gira a la derecha
            }
            else{   
                estadoActual = BUSCAR;  
            }
            break;

        case RETROCESO_LINEA_DERECHA:
            if (millis() - tiempoRetrocesoInicio < duracionRetroceso) {
                xmotion.MotorControl(-fast_speed, -0.5*fast_speed);  // retrocede y gira a la izquierda
            }
            else {   
                estadoActual = BUSCAR;  
            }
            break;

        case GIRO180:
            if (millis() - tiempoGiro180Inicio < duracionGiro180) {
                xmotion.MotorControl(-mean_speed, mean_speed); 
            }
            else{
                xmotion.StopMotors(1);    
                estadoActual = BUSCAR;  
            }
            break;                
        

        default:
        estadoActual = BUSCAR;
        break;
    }
}


