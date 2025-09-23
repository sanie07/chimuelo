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
const int pin_L_OS = 2;
OS L_OS = OS(pin_L_OS);
// Centro:
const int pin_L_OS = A5;
OS L_OS = OS(pin_L_OS);
// Derecha-Diagonal:
const int pin_L_OS = A4;
OS L_OS = OS(pin_L_OS);
// Derecha:
const int pin_L_OS = A1;
OS L_OS = OS(pin_L_OS);

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
// Motores:
int delay_90grados = 23;
int delay_180grados = delay_90grados*2;


///////////////////////////////  FUNCIONES  ////////////////////////////////
void Frente_rapido();
void Giro_derecha();
void Giro_izquierda();
void Giro_90grados_derecha(int tiempo_90grados);
void Giro_90grados_izquierda(int tiempo_90grados);
void Giro_180grados(int tiempo_180grados);



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
  while(MS.get_start()){
    // Leemos los sensores:
    bool Read_OS[] = {L_OS.lectura(), LD_OS.lectura(), C_OS.lectura(), RD_OS.lectura(), R_OS.lectura()};
    bool Read_LS[] = {L_LS.lectura(), R_LS.lectura()};
    
    // Centro
    if(!Read_OS[1] && Read_OS[2] && !Read_OS[3]){
      Frente_rapido();
    }
    // Derecha-centrado / Derecha-diagonal
    else if((!Read_OS[1] && Read_OS[2] && Read_OS[3]) || (!Read_OS[1] && !Read_OS[2] && Read_OS[3])){
      Giro_derecha();
    }
    // Izquierda-centrado / Izquierda-diagonal
    else if((Read_OS[1] && Read_OS[2] && !Read_OS[3]) || (Read_OS[1] && !Read_OS[2] && !Read_OS[3])){
      Giro_izquierda();
    }
    // No se encuentra en frente
    else if((!Read_OS[1] && !Read_OS[2] && !Read_OS[3])){
      // Derecha
      if(Read_OS[4]){
        Giro_90grados_derecha(delay_90grados);
      }
      // Izquierda
      else if(Read_OS[0]){
        Giro_90grados_izquierda(delay_90grados);
      }
      // No se encuentra ni en frente ni en los costados
      else{
        Giro_180grados(delay_180grados);
      }
    }
  }
}
