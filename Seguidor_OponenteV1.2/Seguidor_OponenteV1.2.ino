// Incluimos la libreria del xmotionV3
#include <xmotionV3.h>
#include "OS.h"
#include "LS.h"
#include "Flag.h"
#include "MicroStart.h"

//////////////////  PINES CON SUS OBJETOS A SER UTILIZADOS  //////////////////
// Bandera:
const int pin_flag = 4;
Flag flag = Flag(pin_flag);


// Sensores Oponente:
// Izquierda:
const int pin_L_OS = 0;
OS L_OS = OS(pin_L_OS);
// Izquierda-Diagonal:
const int pin_LD_OS = 2;
OS LD_OS = OS(pin_LD_OS);
// Centro:
const int pin_C_OS = A5;
OS C_OS = OS(pin_C_OS);
// Derecha-Diagonal:
const int pin_RD_OS = A4;
OS RD_OS = OS(pin_RD_OS);
// Derecha:
const int pin_R_OS = A1;
OS R_OS = OS(pin_R_OS);


// Sensores de Linea:
// Izquierda:
const int pin_L_LS = 1; 
LS L_LS = LS(pin_L_LS);
// Derecha:
const int pin_R_LS = A2;
LS R_LS = LS(pin_R_LS);


// Sensor de MicroStart:
const int pin_start = A0;
MicroStart MS = MicroStart(pin_start);


// LEDs:
const int UserLed1 = 9;
const int UserLed2 = 8;


//////////////////////////  VARIABLES AUXILIARES  //////////////////////////
//// Motores:
// Giro:
int delay_90grados = 19;
int delay_180grados = delay_90grados*2;

// Velocidades:
int super_fast_speed = 150; // velocidad de ataque
int fast_speed = 65;   // velocidad para el frente
int mean_speed = 50;   // velocidad promedio de busqueda
int var_speed = 18;    // velocidad variable de busqueda
int speed;
int left_speed;
int rigth_speed;

// Proporcionalidad:
float num_prop = 0;
int prop = 0;

//// Sensor Oponente:
// Filtro:
const int FILTER_N = 3;
int L_OS_F;
int LD_OS_F;
int C_OS_F;
int RD_OS_F;
int R_OS_F;
int L_LS_F;
int R_LS_F;
// Valores anteriores:
int ant_LD_OS = 0;
int ant_RD_OS = 0;


///////////////////////////////  FUNCIONES  ////////////////////////////////
void Frente_rapido();
void Ataque();
void Giro_derecha(float num_prop);
void Giro_izquierda(float num_prop);
void Giro_90grados_derecha();
void Giro_90grados_izquierda();
void Giro_180grados();

// Filtramos las lecturas de los sensores para verificar que no sean falsas lecturas
template<typename T>
bool filtro(T &sensor, int n = FILTER_N){
  int suma = 0;
  for(int i=0; i<n; i++){
    suma += sensor.lectura() ? 1 : 0;
    delayMicroseconds(200); // opcional, pequeño retardo
  }
  return (suma > n/2);
}


void setup() {
  //////////////////////////  INICIALIZAMOS LOS PINES  //////////////////////////
  // Bandera:
  flag.begin();

  // Sensores Oponente:
  L_OS.begin();
  LD_OS.begin();
  C_OS.begin();
  RD_OS.begin();
  R_OS.begin();

  // Sensores de Linea:
  L_LS.begin();
  R_LS.begin();

  // Sensor MicroStart:
  MS.begin();

  // LEDs:
  pinMode(UserLed1, OUTPUT);
  digitalWrite(UserLed1, LOW);
  pinMode(UserLed2, OUTPUT);
  digitalWrite(UserLed2, LOW);
  
  // Hacemos parpadear las luces para indicar que se esta cargando el programa:
  for(int i=0;i<5;i++){
    xmotion.ToggleLeds(100);
  }

}

void loop() {
  // Si activamos el MicroStart
  while(MS.get_start()){
    // Activamos la bandera
    if(!flag.get_abierto()){
      flag.matador();
    }

    // Leemos sensores con filtro
    L_OS_F = filtro(L_OS);
    LD_OS_F = filtro(LD_OS);
    C_OS_F = filtro(C_OS);
    RD_OS_F = filtro(RD_OS);
    R_OS_F = filtro(R_OS);
    L_LS_F = filtro(L_LS);
    R_LS_F = filtro(R_LS);

    // Ubicamos esas lecturas dentro de vectores
    bool Read_OS[] = {
      L_OS_F, LD_OS_F, C_OS_F, RD_OS_F, R_OS_F
    };
    bool Read_LS[] = {
      L_LS_F, R_LS_F
    };

    // Sensor de Linea
    if((Read_LS[0] && !Read_LS[1])){  // Si es solo el izquierdo
      // Retrocede girando a la izquierda (rueda izquierda gira mas rapido que el derecho)
      xmotion.MotorControl(-int(fast_speed/2), -fast_speed);
      delay(85); // tiempo de retroceso
      xmotion.StopMotors(1);  // paramos
      continue; // vuelve al inicio del loop
    }
    else if(!Read_LS[0] && Read_LS[1]){
      // Retrocede girando a la derecha (rueda derecha gira mas rapido que el izquierdo)
      xmotion.MotorControl(-fast_speed, -int(fast_speed/2));
      delay(85); // tiempo de retroceso
      xmotion.StopMotors(1);  // paramos
      continue; // vuelve al inicio del loop
    }
    else if(Read_LS[0] && Read_LS[1]){
      // Retrocede girando a la izquierda (rueda izquierda gira mas rapido que el derecho)
      xmotion.MotorControl(-fast_speed, -fast_speed);
      delay(85); // tiempo de retroceso
      Giro_180grados();
      continue;
    }
      
    // Izquierda - Centro - Derecha: Ataque al Oponente
    /*if(Read_OS[1] && Read_OS[2] && Read_OS[3]){
      Ataque();
    }*/
    // Centro
    else if(!Read_OS[1] && Read_OS[2] && !Read_OS[3] || Read_OS[1] && Read_OS[2] && Read_OS[3]){
      Frente_rapido();
    }
    // Derecha-centrado / Derecha-diagonal
    else if((!Read_OS[1] && Read_OS[2] && Read_OS[3]) || (!Read_OS[1] && !Read_OS[2] && Read_OS[3])){
      prop = 3-(int(Read_OS[2]) + int(Read_OS[3]));
      if(prop == 2){num_prop = 1.5;}
      else{num_prop = 1;}
      Giro_derecha(num_prop);
    }
    // Izquierda-centrado / Izquierda-diagonal
    else if((Read_OS[1] && Read_OS[2] && !Read_OS[3]) || (Read_OS[1] && !Read_OS[2] && !Read_OS[3])){
      prop = 3-(int(Read_OS[2]) + int(Read_OS[1]));
      if(prop == 2){num_prop = 1.5;}
      else{num_prop = 1;}
      Giro_izquierda(num_prop);
    }
    // No se encuentra en frente
    else if((!Read_OS[1] && !Read_OS[2] && !Read_OS[3])){
      // Derecha
      if(Read_OS[4]){
        xmotion.Right0(40, 10);
      }
      // Izquierda
      else if(Read_OS[0]){
        num_prop = 2.25;
        Giro_izquierda(num_prop);
      }
      // No se encuentra ni en frente ni en los costados
      else{
        if(ant_LD_OS){xmotion.Right0(20, 5);}       // Ultimo sensor detectado fue el Izquierda-diagonal
        else if(ant_RD_OS){xmotion.Left0(20, 5);}   // Ultimo sensor detectados fue el derecha-diagonal
        else{xmotion.Right0(10, 1);}
      }
    }
    ant_LD_OS = Read_OS[1];
    ant_RD_OS = Read_OS[3];
  }
  xmotion.StopMotors(1);
}

// Funcion de Atauque
void Ataque(){
  xmotion.MotorControl(super_fast_speed, super_fast_speed);
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

