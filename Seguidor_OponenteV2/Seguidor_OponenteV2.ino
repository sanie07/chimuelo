// Incluimos la libreria del xmotionV3
#include <xmotionV3.h>
#include "OS.h"
#include "LS.h"
#include "Flag.h"
#include "MicroStart.h"
#include "States.h"

//////////////////  PINES CON SUS OBJETOS A SER UTILIZADOS  //////////////////
// Bandera:
const int pin_flag = 4;

// Sensores Oponente:
const int pin_L_OS = 0; //Izquierda
const int pin_LD_OS = 2; //Izquierda-Diagonal 
const int pin_C_OS = A5; //Centro
const int pin_RD_OS = A4; // Derecha-Diagonal
const int pin_R_OS = A1; // Derecha


// Sensores de Linea:
const int pin_L_LS = 1; // Izquierda 
const int pin_R_LS = A2; // Derecha


// Sensor de MicroStart:
const int pin_start = A0;


// LEDs:
const int UserLed1 = 9;
const int UserLed2 = 8;

//////////////////////////  DECLARACION DE OBJETOS  //////////////////////////
Flag flag = Flag(pin_flag);

OS L_OS = OS(pin_L_OS);
OS LD_OS = OS(pin_LD_OS);
OS C_OS = OS(pin_C_OS);
OS RD_OS = OS(pin_RD_OS);
OS R_OS = OS(pin_R_OS);

LS L_LS = LS(pin_L_LS); 
LS R_LS = LS(pin_R_LS);

MicroStart MS = MicroStart(pin_start);

States estados = States(L_OS, LD_OS, C_OS, RD_OS, R_OS, L_LS, R_LS);

///////////////////////////////  FUNCIONES  ////////////////////////////////
void Frente_rapido();
void Giro_derecha(float num_prop);
void Giro_izquierda(float num_prop);
void Giro_90grados_derecha();
void Giro_90grados_izquierda();
void Giro_180grados();



void setup() {
  //////////////////////////  INICIALIZAMOS LOS PINES  //////////////////////////
  // Bandera:
  flag.begin();

  // Sensor MicroStart:
  MS.begin();

  // Estados:
  estados.begin();

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
    estados.update();
  }

  xmotion.StopMotors(1);
}





