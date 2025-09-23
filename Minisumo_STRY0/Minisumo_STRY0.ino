/* ------------------------ CHIMUELO - STRATEGY 0 -------------------------- */

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
//const int start = A0;

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
/*int numero_sensores = 7;
int *read_sensors;*/

// Motores:
int delay_90grados = 21;
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
  // Cuando se apriete 'start' iniciamos el programa:
  while(start == 1){
    // 1. Extendemos la bandera una sola vez:
      if(close == 0){
        // Abrimos la bandera:
        digitalWrite(flag, HIGH);
        delay(100);
        digitalWrite(flag, LOW);
        // Ya esta abierta y no volvemos a ejecutar este programa:
        close = 1;
      }

    // 2. Probamos sensores de linea durante 5seg.:
      for(int i=0; i<5000; i++){
        // Leemos ambos sensores:
        lectura_L = digitalRead(L_LS);
        lectura_R = digitalRead(R_LS);

        // SL_1 prende el led1 en caso de detectar una linea blanca:
        if(lectura_L == HIGH) { digitalWrite(UserLed2, HIGH); }
        else { digitalWrite(UserLed2, LOW); }

        // SL_2 prende el led2 en caso de detectar una linea blanca:
        if(lectura_R == HIGH) { digitalWrite(UserLed1, HIGH); }
        else { digitalWrite(UserLed1, LOW); }

        // Aplicamos un delay de 1ms.:
        delay(1);
      }

    // 3. Control del motor:
      // Va para adelante a una velocidad lenta:
      Frente_despacio(500);
      delay(1000);

      // Gira 180 grados:
      Giro_180grados(delay_180grados);
      delay(5000);

      // Va para adelante a una velocidad rapida:
      Frente_rapido(300);
      delay(1000);

      // Gira 90 grados derecha:
      Giro_90grados_derecha(delay_90grados);
      delay(5000);

      // Gira 90 grados derecha:
      Giro_90grados_izquierda(delay_90grados);
      delay(5000);
  }
}

void Frente_despacio(int time){
  int slow_speed = 100;      // ajuste de velocidad lenta
  xmotion.MotorControl(-slow_speed, -slow_speed);     // ( ...255, ...255 )
  delay(time);              // avanzamos durante un delay
  xmotion.StopMotors(1);    // paramos los motores
}

void Frente_rapido(int time){
  int fast_speed = 180;     // ajuste de velocidad rapida
  xmotion.MotorControl(-fast_speed, -fast_speed);     // ( ...255, ...255 )
  delay(time);              // avanzamos durante un delay
  xmotion.StopMotors(1);    // paramos los motores
}

void Giro_90grados_derecha(int delay_90grados){
  xmotion.Left0(100, delay_90grados);    // ( %speed, delay )
  xmotion.StopMotors(1);    // paramos los motores
}

void Giro_90grados_izquierda(int delay_90grados){
  xmotion.Right0(100, delay_90grados);    // ( %speed, delay )
  xmotion.StopMotors(1);    // paramos los motores
}

void Giro_180grados(int delay_180grados){
  xmotion.Left0(100, delay_180grados);  // ( %speed, delay )
  xmotion.StopMotors(1);    // paramos los motores
}
