// Incluimos la libreria del xmotionV3
#include <xmotionV3.h>
#include "OS.h"
#include "LS.h"
#include "MicroStart.h"

//////////////////  PINES CON SUS OBJETOS A SER UTILIZADOS  //////////////////
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

// Dipswitch:
const int DIP_SW0 = 7;
const int DIP_SW1 = 6;
const int DIP_SW2 = 5;


//////////////////////////  VARIABLES AUXILIARES  //////////////////////////
//// Motores:
// Giro:
int delay_90grados = 45;
int delay_180grados = delay_90grados*2;

// Velocidades:
int super_fast_speed = 255; // velocidad enfrentado
int fast_speed = 200;   // velocidad para el frente
int medium_speed = 150; // velocidad para el frente
int mean_speed = 100;   // velocidad promedio de busqueda
int var_speed = 30;     // velocidad variable de busqueda
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

//// Dipswitch:
int close = 0;


///////////////////////////////  FUNCIONES  ////////////////////////////////
void Frente_rapido();   // va a una velocidad alta cuando esta centrado
void Giro_derecha(float num_prop);    // va a la derecha dependiendo de los sensores
void Giro_izquierda(float num_prop);  // va a la izquierda dependiendo de los sensores
void Giro_90grados_derecha();     // derecha dependiendo si es el sensor R_OS
void Giro_90grados_izquierda();   // izquierda dependiendo si es el sensor L_OS
void Giro_180grados();   // caso de sensor de linea
void Inicio_STRGY0();    // DIPSWITCH en 000
//---- no hacemos nada   // DIPSWITCH en 001
void Inicio_STRGY2();    // DIPSWITCH en 011

// --- NUEVO: función filtro por mayoría ---
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

  // Dipswitch:
  pinMode(DIP_SW0, INPUT_PULLUP);
  pinMode(DIP_SW1, INPUT_PULLUP);
  pinMode(DIP_SW2, INPUT_PULLUP);
  
  // Hacemos parpadear las luces para indicar que se esta cargando el programa:
  for(int i=0;i<5;i++){
    xmotion.ToggleLeds(100);
  }

}

void loop() {
  while(MS.get_start()){
    // Estrategias:
    if(close == 0){
      if(!digitalRead(DIP_SW0)){
        if(!digitalRead(DIP_SW1)){  // si es 011
          Inicio_STRGY2();  // inicio de esquivo, esquivo, leo tus derechos
        }
        // si es 001 no hacemos nada
      }
      else{
        // si es 000
        Inicio_STRGY0();  // inicio de prueba
      }
      close = 1; 
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
      xmotion.MotorControl(-int(0.1*fast_speed), -fast_speed);
      delay(100);
      xmotion.StopMotors(1);
      continue;
    }
    else if(!Read_LS[0] && Read_LS[1]){
      // Retrocede girando a la derecha (rueda derecha gira mas rapido que el izquierdo)
      xmotion.MotorControl(-fast_speed, -int(0.1*fast_speed));
      delay(100);
      xmotion.StopMotors(1);
      continue;
    }
    else if(Read_LS[0] && Read_LS[1]){
      // Retrocede girando a la izquierda (rueda izquierda gira mas rapido que el derecho)
      xmotion.MotorControl(-fast_speed, -fast_speed);
      delay(250); // tiempo de retroceso
      xmotion.StopMotors(1);
      Giro_180grados();
      continue;
    }

    // Centro
    if(!Read_OS[1] && Read_OS[2] && !Read_OS[3] || Read_OS[1] && Read_OS[2] && Read_OS[3]){
      Frente_rapido();
    }
    // Derecha-centrado / Derecha-diagonal
    else if((!Read_OS[1] && Read_OS[2] && Read_OS[3]) || (!Read_OS[1] && !Read_OS[2] && Read_OS[3])){
      prop = 3-(int(Read_OS[2]) + int(Read_OS[3]));
      if(prop == 2){
        num_prop = 2.5;
      }
      else{
        num_prop = 1.5;
      }
      Giro_derecha(num_prop);
    }
    // Izquierda-centrado / Izquierda-diagonal
    else if((Read_OS[1] && Read_OS[2] && !Read_OS[3]) || (Read_OS[1] && !Read_OS[2] && !Read_OS[3])){
      prop = 3-(int(Read_OS[2]) + int(Read_OS[1]));
      if(prop == 2){
        num_prop = 2;
      }
      else{
        num_prop = 1;
      }
      Giro_izquierda(num_prop);
    }
    // No se encuentra en frente
    else if((!Read_OS[1] && !Read_OS[2] && !Read_OS[3])){
      // Derecha
      if(Read_OS[4]){
        xmotion.Right0(60, 1);
      }
      // Izquierda
      else if(Read_OS[0]){
        xmotion.Left0(60, 1);
      }
      // No se encuentra ni en frente ni en los costados
      else{
        xmotion.MotorControl(120,150);
      }
    }
  }
  xmotion.StopMotors(1);
}

// Funcion para cuando es: Centro.
void Frente_rapido(){
  int cont = 0;
  do{
    if((10*cont)<255){
      xmotion.MotorControl(medium_speed + 10*cont, medium_speed + 10*cont);
      cont++;
    }
    else if((10*cont) >= 255 && cont < 5000){
      xmotion.MotorControl(super_fast_speed, super_fast_speed);
      cont++;
    }
    else{
      xmotion.MotorControl(-int(0.1*fast_speed), -fast_speed);
      delay(100);
      cont = 26;
    }
  } while(filtro(LD_OS) && filtro(C_OS) && filtro(RD_OS) && MS.get_start());
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

// Funcion para el inicio de prueba
void Inicio_STRGY0(){
  // 1. Probamos sensores de linea durante 5seg.:
    for(int i=0; i<5000; i++){
      int lect_L = L_LS.lectura();
      int lect_R = R_LS.lectura();
      // SL_1 prende el led1 en caso de detectar una linea blanca:
      if(lect_L == HIGH) { digitalWrite(UserLed2, HIGH); }
      else { digitalWrite(UserLed2, LOW); }

      // SL_2 prende el led2 en caso de detectar una linea blanca:
      if(lect_R == HIGH) { digitalWrite(UserLed1, HIGH); }
      else { digitalWrite(UserLed1, LOW); }

      // Aplicamos un delay de 1ms.:
      delay(1);
    }
}

// Funcion para el inicio de la estrategia del retroceso
void Inicio_STRGY2(){
  if(random(0,2) == 0){
    xmotion.MotorControl(-fast_speed, -10); // izquierda
  } else {
    xmotion.MotorControl(-10, -fast_speed); // derecha
  }
  delay(80);
  xmotion.MotorControl(-fast_speed, -fast_speed);
  delay(10);
}

