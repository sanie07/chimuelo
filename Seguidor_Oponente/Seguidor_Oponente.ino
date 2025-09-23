// Incluimos la libreria del xmotionV3
#include <xmotionV3.h>

//////////////////////////  PINES A SER UTILIZADOS  //////////////////////////
// Bandera:
const int flag = 4;

// Sensores Oponente:
const int L_OS = 0;
const int LD_OS = 2;
const int C_OS = A5;
const int RD_OS = A4;
const int R_OS = A1;

// Sensores de Linea:
const int L_LS = 1; 
const int R_LS = A2;

// Sensor de MicroStart:
const int start = A0;

// LEDs:
const int UserLed1 = 9;
const int UserLed2 = 8;


//////////////////////////  VARIABLES AUXILIARES  //////////////////////////
// Bandera:
int close = 0;

// Sensores Oponente:
const int OS[] = {L_OS, LD_OS, C_OS, RD_OS, R_OS};

// Sensores de Linea:
int lectura_L;
int lectura_R;

// Sensores:
int lectura_L_OS;
int lectura_LD_OS;
int lectura_C_OS;
int lectura_RD_OS;
int lectura_R_OS;
/*int numero_sensores = 7;
int *read_sensors;*/

// Motores:
int delay_90grados = 23;
int delay_180grados = delay_90grados*2;


////////////////////////////////  FUNCIONES  ////////////////////////////////
void lectura(int *read_sensors[], int numero_sensores);
void Frente_despacio(int delay);
void Frente_rapido(int delay);
void Giro_90grados_derecha(int delay_90grados);
void Giro_90grados_izquierda(int delay_90grados);
void Giro_180grados(int delay_180grados);


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
