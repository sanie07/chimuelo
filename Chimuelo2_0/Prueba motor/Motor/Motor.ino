////// Motor A o B
//// ADELANTE
// IN1: pwm
// IN2: 0

//// ATRAS
// IN1: 0
// IN2: pwm

//////////////////////////////////////////////////
//// Motores:
// Motor A:
const int motorL_IN1 = 3;
const int motorL_IN2 = 5;

// Motor B:
const int motorR_IN1 = 6;
const int motorR_IN2 = 9;


void setup() {
  pinMode(motorL_IN1, OUTPUT);
  pinMode(motorL_IN2, OUTPUT);
  pinMode(motorR_IN1, OUTPUT);
  pinMode(motorR_IN2, OUTPUT);
}

void loop() {
  // Al frente:
  for(int i=0; i<5000; i++){
    analogWrite(motorL_IN1, 128);
    digitalWrite(motorL_IN2, LOW);
    analogWrite(motorR_IN1, 128);
    digitalWrite(motorR_IN2, LOW);
    delay(1);
  }

  // Parar motores:
  digitalWrite(motorL_IN1, LOW);
  digitalWrite(motorL_IN2, LOW);
  digitalWrite(motorR_IN1, LOW);
  digitalWrite(motorR_IN2, LOW);
  delay(1000);

  // Atras:
  for(int i=0; i<5000; i++){
    analogWrite(motorL_IN2, 128);
    digitalWrite(motorL_IN1, LOW);
    analogWrite(motorR_IN2, 128);
    digitalWrite(motorR_IN1, LOW);
    delay(1);
  }

  // Parar motores
  digitalWrite(motorL_IN1, LOW);
  digitalWrite(motorL_IN2, LOW);
  digitalWrite(motorR_IN1, LOW);
  digitalWrite(motorR_IN2, LOW);
  delay(1000);

  // Giro a la izquierda:
  for(int i=0; i<5000; i++){
    analogWrite(motorL_IN2, 128);
    digitalWrite(motorL_IN1, LOW);
    analogWrite(motorR_IN1, 128);
    digitalWrite(motorR_IN2, LOW);
    delay(1);
  }

  // Giro a la izquierda:
  for(int i=0; i<5000; i++){
    analogWrite(motorR_IN2, 128);
    digitalWrite(motorR_IN1, LOW);
    analogWrite(motorL_IN1, 128);
    digitalWrite(motorL_IN2, LOW);
    delay(1);
  }

}
