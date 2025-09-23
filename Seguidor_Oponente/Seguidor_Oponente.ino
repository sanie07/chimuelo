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
void Frente_busqueda(int delay);
void Frente_rapido(int delay);
void Giro_90grados_derecha(int delay_90grados);
void Giro_90grados_izquierda(int delay_90grados);
void Giro_180grados(int delay_180grados);
void Giro_derecha()


void setup() {
  //////////////////////////  INICIALIZAMOS LOS PINES  //////////////////////////
  // Bandera:
  pinMode(flag, OUTPUT);
  digitalWrite(flag, LOW);

  // Sensor Oponente:
  for(int i=0;i<5;i++){
    pinMode(OS[i], INPUT);
  }

  // Sensor de Linea:
  pinMode(L_LS, INPUT);
  pinMode(R_LS, INPUT);

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
  // Leemos los sensores:
  lectura_L_OS = digitalRead()
  if()












}
