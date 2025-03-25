#include <Servo.h>

// Objetos Servo para cada mecanismo
Servo wheelFL;    // Rueda frontal izquierda
Servo wheelFR;    // Rueda frontal derecha
Servo wheelBL;    // Rueda trasera izquierda
Servo wheelBR;    // Rueda trasera derecha
Servo elevator;   // Sistema de polea para el elevador
Servo clawAngle;  // Servo para la orientación de la garra (-90 a +90, remapeado a 0-180)
Servo clawGrip;   // Servo para el agarre

// Pines de conexión (valores de prueba)
const int pinWheelFL = 8;
const int pinWheelFR = 9;
const int pinWheelBL = 10;
const int pinWheelBR = 11;
const int pinElevator = 12;
const int pinClawAngle = 7;  //4 tierra, 6 positivo, 7 señal
const int pinClawGrip = 13;  // Conexión: señal (positivo a 5V)

// Intervalo de actualización en milisegundos (10 ms)
const unsigned long updateInterval = 10;
unsigned long previousMillis = 0;

//---------------------------------
// Funciones de control de movimientos
//---------------------------------

// Función para detener el movimiento (estado neutro en las ruedas)
void stopMovement() {
  wheelFL.write(90);
  wheelFR.write(90);
  wheelBL.write(90);
  wheelBR.write(90);
}

// Función para mover el robot a la izquierda
void moveLeft() {
  wheelFL.write(60);
  wheelFR.write(120);
  wheelBL.write(60);
  wheelBR.write(120);
}

// Función para mover el robot a la derecha
void moveRight() {
  wheelFL.write(120);
  wheelFR.write(60);
  wheelBL.write(120);
  wheelBR.write(60);
}

// Función para mover el robot hacia adelante
void moveForward() {
  wheelFL.write(100);
  wheelFR.write(100);
  wheelBL.write(100);
  wheelBR.write(100);
}

// Función para mover el robot hacia atrás
void moveBackward() {
  wheelFL.write(80);
  wheelFR.write(80);
  wheelBL.write(80);
  wheelBR.write(80);
}

//---------------------------------
// Funciones para el elevador y la garra
//---------------------------------

// Función para subir el elevador
void elevatorUp() {
  elevator.write(110);  // Valor de ejemplo para subir
}

// Función para bajar el elevador
void elevatorDown() {
  elevator.write(70);   // Valor de ejemplo para bajar
}

// Función para abrir la garra
void clawOpen() {
  clawGrip.write(180);  // 180° para la garra abierta
}

// Función para cerrar la garra
void clawClose() {
  clawGrip.write(0);    // 0° para la garra cerrada
}

// Función para ajustar la orientación de la garra.
// Recibe un valor en el rango -90 a 90 y lo remapea a 0-180.
void setClawAngle(int8_t angleSigned) {
  int mappedAngle = constrain(angleSigned + 90, 0, 180);
  clawAngle.write(mappedAngle);
}

//---------------------------------
// Función para procesar el comando recibido
//---------------------------------
void processCommand(char command) {
  switch (command) {
    case 'A':  // Mover a la izquierda
      moveLeft();
      break;
    case 'D':  // Mover a la derecha
      moveRight();
      break;
    case 'W':  // Avanzar
      moveForward();
      break;
    case 'S':  // Retroceder
      moveBackward();
      break;
    case 'U':  // Elevar (subir)
      elevatorUp();
      break;
    case 'J':  // Bajar el elevador
      elevatorDown();
      break;
    case 'O':  // Abrir la garra
      clawOpen();
      break;
    case 'C':  // Cerrar la garra
      clawClose();
      break;
    // Puedes agregar más comandos según lo necesites.
    default:
      // Si el comando no es reconocido o es 'X', se detiene el movimiento.
      stopMovement();
      break;
  }
}

//---------------------------------
// Configuración inicial
//---------------------------------
void setup() {
  Serial.begin(9600); // Inicializa el puerto serial (Bluetooth) a 9600 baudios
  
  // Adjunta cada servo a su pin correspondiente
  wheelFL.attach(pinWheelFL);
  wheelFR.attach(pinWheelFR);
  wheelBL.attach(pinWheelBL);
  wheelBR.attach(pinWheelBR);
  elevator.attach(pinElevator);
  clawAngle.attach(pinClawAngle);
  clawGrip.attach(pinClawGrip);
  
  // Posiciones iniciales: 90 es el estado neutro para los servos
  stopMovement();
  elevator.write(90);
  setClawAngle(0); // 0 en función setClawAngle equivale a 90° del servo (estado central)
  clawGrip.write(90);
}

//---------------------------------
// Bucle principal
//---------------------------------
void loop() {
  // Se asume que la app envía un char cada 10ms, sin excepción.
  if (Serial.available() > 0) {
    char command = Serial.read();
    // Si el comando recibido es distinto de 'X', se procesa la acción
    if (command != 'X') {
      processCommand(command);
    } else {
      // Si se recibe 'X', se detienen los movimientos
      stopMovement();
    }
  }
  
  // Control para respetar el intervalo de actualización de 10ms
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < updateInterval) {
    return;
  }
  previousMillis = currentMillis;
}
