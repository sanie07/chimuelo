#include "States.h" // Incluye la cabecera de clase OS
#include <xmotionV3.h> // Incluye la libreria xmotionV3

        //// Auxiliar Sensor Oponente:
    // Filtro:
    const int FILTER_N = 3;
    int L_OS_F;
    int LD_OS_F;
    int C_OS_F;
    int RD_OS_F;
    int R_OS_F;
//    int L_LS_F;
//    int R_LS_F;

// --- Funcion filtro por mayoria ---
template<typename T>
bool filtro(T &sensor, int n = FILTER_N){
  int suma = 0;
  for(int i=0; i<n; i++){
    suma += sensor.lectura() ? 1 : 0;
    delayMicroseconds(200); // opcional, pequeño retardo
  }
  return (suma > n/2);
}

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
    // Leemos sensores con filtro
    L_OS_F = filtro(L_OS);
    LD_OS_F = filtro(LD_OS);
    C_OS_F = filtro(C_OS);
    RD_OS_F = filtro(RD_OS);
    R_OS_F = filtro(R_OS);
    //L_LS_F = filtro(L_LS);
    //R_LS_F = filtro(R_LS);

    // Ubicamos esas lecturas dentro de vectores
    bool Read_OS[] = {
      L_OS_F, LD_OS_F, C_OS_F, RD_OS_F, R_OS_F
    };
    bool Read_LS[] = {
      L_LS.lectura(), R_LS.lectura()
    };

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
                xmotion.MotorControl(base_speed, base_speed); //Avanza para escanear
            } //else if (millis() - tiempoBusquedaInicio < 1.25*duracionBusqueda) {
                //xmotion.MotorControl(base_speed, -base_speed); //Gira para escanear
            //} 
            else {
                tiempoBusquedaInicio = millis(); // Reiniciar el temporizador de búsqueda
            }
            
            // Veriificar si se detecta el oponente
            if (oponente) {
                estadoActual = ALINEAR;
            }
            break;

    case ALINEAR: {
                    
        }            
        
        case ATAQUE_RAPIDO:
            if(abs(error) < 0.5){
            xmotion.MotorControl(max_speed, max_speed);}
            else{
                estadoActual = ALINEAR;
            }
            break;
            

        case RETROCESO_LINEA:
            if (millis() - tiempoRetrocesoInicio < duracionRetroceso) {
                xmotion.MotorControl(-base_speed, -base_speed);  // retrocede y gira
            }
            else {
                tiempoGiro180Inicio = millis();   
                estadoActual = GIRO180;  
            }
            break;

        case RETROCESO_LINEA_IZQUIERDA:
            if (millis() - tiempoRetrocesoInicio < duracionRetroceso) {
                xmotion.MotorControl(-0.2*base_speed, -base_speed);  // retrocede y gira a la derecha
            }
            else{   
                estadoActual = BUSCAR;  
            }
            break;

        case RETROCESO_LINEA_DERECHA:
            if (millis() - tiempoRetrocesoInicio < duracionRetroceso) {
                xmotion.MotorControl(-base_speed, -0.2*base_speed);  // retrocede y gira a la izquierda
            }
            else {   
                estadoActual = BUSCAR;  
            }
            break;

        case GIRO180:
            if (millis() - tiempoGiro180Inicio < duracionGiro180) {
                xmotion.MotorControl(-base_speed, base_speed); 
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

