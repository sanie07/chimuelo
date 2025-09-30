// Incluimos la libreria del xmotionV3
#include <xmotionV3.h>

// LEDs:
const int UserLed1 = 9;
const int UserLed2 = 8;


void setup() {

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
    test_speed = 1;
    xmotion.MotorControl(test_speed, test_speed);

}





