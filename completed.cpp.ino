#include <Servo.h>

//==================================
// Definición de pines
//==================================

// Puente H Izquierdo (para ruedas del lado izquierdo)
// Rueda izquierda trasera:
const bool DEBUG = true;

const int LEFT_REAR_IN1 = 2;
const int LEFT_REAR_IN2 = 3;
// Rueda izquierda frontal:
const int LEFT_FRONT_IN3 = 11;
const int LEFT_FRONT_IN4 = 12;

// Puente H del Medio (para sistema de polea/elevador)
const int PULLEY_IN1 = 4;
const int PULLEY_IN2 = 5;

// Puente H Derecho (para ruedas del lado derecho)
// Rueda derecha frontal:
const int RIGHT_FRONT_IN1 = 6;
const int RIGHT_FRONT_IN2 = 7;
// Rueda derecha trasera:
const int RIGHT_REAR_IN3 = 8;
const int RIGHT_REAR_IN4 = 13;

// Servos normales (garra)
const int CLAW_ROTATION_PIN = 10;  // Rota la garra (arriba/abajo)
const int CLAW_GRIP_PIN = 9;       // Abre o cierra la garra

//==================================
// Objetos Servo
//==================================
Servo clawRotation; // Servo para rotar la garra
Servo clawGrip;     // Servo para abrir/cerrar la garra

//==================================
// Enumeración para direcciones
//==================================
enum Direction { STOP, FORWARD, BACKWARD };

//==================================
// Funciones para controlar cada rueda individualmente
//==================================

void setLeftRear(Direction d) {
  switch(d) {
    case FORWARD:
      digitalWrite(LEFT_REAR_IN1, HIGH);
      digitalWrite(LEFT_REAR_IN2, LOW);
      break;
    case BACKWARD:
      digitalWrite(LEFT_REAR_IN1, LOW);
      digitalWrite(LEFT_REAR_IN2, HIGH);
      break;
    case STOP:
    default:
      digitalWrite(LEFT_REAR_IN1, LOW);
      digitalWrite(LEFT_REAR_IN2, LOW);
      break;
  }
}

void setLeftFront(Direction d) {
  switch(d) {
    case FORWARD:
      digitalWrite(LEFT_FRONT_IN3, HIGH);
      digitalWrite(LEFT_FRONT_IN4, LOW);
      break;
    case BACKWARD:
      digitalWrite(LEFT_FRONT_IN3, LOW);
      digitalWrite(LEFT_FRONT_IN4, HIGH);
      break;
    case STOP:
    default:
      digitalWrite(LEFT_FRONT_IN3, LOW);
      digitalWrite(LEFT_FRONT_IN4, LOW);
      break;
  }
}

void setRightFront(Direction d) {
  switch(d) {
    case FORWARD:
      digitalWrite(RIGHT_FRONT_IN1, HIGH);
      digitalWrite(RIGHT_FRONT_IN2, LOW);
      break;
    case BACKWARD:
      digitalWrite(RIGHT_FRONT_IN1, LOW);
      digitalWrite(RIGHT_FRONT_IN2, HIGH);
      break;
    case STOP:
    default:
      digitalWrite(RIGHT_FRONT_IN1, LOW);
      digitalWrite(RIGHT_FRONT_IN2, LOW);
      break;
  }
}

void setRightRear(Direction d) {
  switch(d) {
    case FORWARD:
      digitalWrite(RIGHT_REAR_IN3, HIGH);
      digitalWrite(RIGHT_REAR_IN4, LOW);
      break;
    case BACKWARD:
      digitalWrite(RIGHT_REAR_IN3, LOW);
      digitalWrite(RIGHT_REAR_IN4, HIGH);
      break;
    case STOP:
    default:
      digitalWrite(RIGHT_REAR_IN3, LOW);
      digitalWrite(RIGHT_REAR_IN4, LOW);
      break;
  }
}

//==================================
// Funciones para el sistema de polea/elevador
//==================================
void setPulley(Direction d) {
  switch(d) {
    case FORWARD:  // Subir
      digitalWrite(PULLEY_IN1, HIGH);
      digitalWrite(PULLEY_IN2, LOW);
      break;
    case BACKWARD: // Bajar
      digitalWrite(PULLEY_IN1, LOW);
      digitalWrite(PULLEY_IN2, HIGH);
      break;
    case STOP:
    default:
      digitalWrite(PULLEY_IN1, LOW);
      digitalWrite(PULLEY_IN2, LOW);
      break;
  }
}

//==================================
// Funciones de movimientos del robot (combinación de ruedas)
//==================================

// Movimiento en "frente": todas las ruedas hacia adelante
void moveForward() {
  setLeftFront(FORWARD);
  setLeftRear(FORWARD);
  setRightFront(FORWARD);
  setRightRear(FORWARD);
}

// Movimiento en "atrás": todas las ruedas hacia atrás
void moveBackward() {
  setLeftFront(BACKWARD);
  setLeftRear(BACKWARD);
  setRightFront(BACKWARD);
  setRightRear(BACKWARD);
}

// Movimiento lateral "izquierda" (strafe left):
// Según especificaciones para moverse a la izquierda:
// - Rueda derecha frontal (wheel 1): FORWARD (arriba)
// - Rueda derecha trasera (wheel 2): BACKWARD (atrás)
// - Rueda izquierda frontal (wheel 3): BACKWARD (atrás)
// - Rueda izquierda trasera (wheel 4): FORWARD (arriba)
void moveLeft() {
  setRightFront(FORWARD);   // Wheel 1
  setRightRear(BACKWARD);   // Wheel 2
  setLeftFront(BACKWARD);   // Wheel 3
  setLeftRear(FORWARD);     // Wheel 4
}

// Movimiento lateral "derecha" (strafe right):
// Para moverse a la derecha:
// - Rueda derecha frontal (wheel 1): BACKWARD (atrás)
// - Rueda derecha trasera (wheel 2): FORWARD (arriba)
// - Rueda izquierda frontal (wheel 3): FORWARD (arriba)
// - Rueda izquierda trasera (wheel 4): BACKWARD (atrás)
void moveRight() {
  setRightFront(BACKWARD);  // Wheel 1
  setRightRear(FORWARD);    // Wheel 2
  setLeftFront(FORWARD);    // Wheel 3
  setLeftRear(BACKWARD);    // Wheel 4
}

// Rotación: Vuelta izquierda (rotate left)
// Comúnmente se hace: ruedas izquierdas hacia atrás y ruedas derechas hacia adelante.
void rotateLeft() {
  setLeftFront(BACKWARD);
  setLeftRear(BACKWARD);
  setRightFront(FORWARD);
  setRightRear(FORWARD);
}

// Rotación: Vuelta derecha (rotate right)
// Ruedas izquierdas hacia adelante y ruedas derechas hacia atrás.
void rotateRight() {
  setLeftFront(FORWARD);
  setLeftRear(FORWARD);
  setRightFront(BACKWARD);
  setRightRear(BACKWARD);
}

// Detener todas las ruedas
void stopWheels() {
  setLeftFront(STOP);
  setLeftRear(STOP);
  setRightFront(STOP);
  setRightRear(STOP);
}

//==================================
// Funciones para servos de la garra
//==================================
void clawOpen() {
  // 0 = garra abierta
  clawGrip.write(0);
}

void clawClose() {
  // 180 = garra cerrada
  clawGrip.write(180);
}

void setClawRotation(int angle) {
  // Rota la garra; se espera ángulo entre 0 y 180
  clawRotation.write(angle);
}

//==================================
// Configuración inicial
//==================================
void setup() {
  // Configurar pines del Puente H Izquierdo:
  pinMode(LEFT_REAR_IN1, OUTPUT);
  pinMode(LEFT_REAR_IN2, OUTPUT);
  pinMode(LEFT_FRONT_IN3, OUTPUT);
  pinMode(LEFT_FRONT_IN4, OUTPUT);
  
  // Configurar pines del Puente H del Medio (polea):
  pinMode(PULLEY_IN1, OUTPUT);
  pinMode(PULLEY_IN2, OUTPUT);
  
  // Configurar pines del Puente H Derecho:
  pinMode(RIGHT_FRONT_IN1, OUTPUT);
  pinMode(RIGHT_FRONT_IN2, OUTPUT);
  pinMode(RIGHT_REAR_IN3, OUTPUT);
  pinMode(RIGHT_REAR_IN4, OUTPUT);
  
  // Inicializar comunicación serial para el módulo Bluetooth
  Serial.begin(9600);
  
  // Inicializar servos de la garra:
  clawRotation.attach(CLAW_ROTATION_PIN);
  clawGrip.attach(CLAW_GRIP_PIN);
  
  // Posiciones iniciales: detener motores y poner servos en posición neutra
  stopWheels();
  setPulley(STOP);
  clawRotation.write(90); // Centro para la rotación de la garra
  clawGrip.write(90);     // Posición intermedia (ajustar según calibración)
  
  // Todos los pines se usan en este diseño.
  Serial.println("Todos los pines están configurados y en uso.");
}

//==================================
// Bucle principal: procesamiento de comandos vía Bluetooth
//==================================
void loop() {

  if (DEBUG)
  {
    moveForward();
  }
  else
  {
    if (Serial.available() > 0) {
      char command = Serial.read();
      switch(command) {
        // Movimientos de ruedas:
        case 'W': moveForward(); break;    // Avanzar
        case 'S': moveBackward(); break;   // Retroceder
        case 'A': moveLeft(); break;       // Mover lateral izquierda (strafe left)\n      
        case 'D': moveRight(); break;      // Mover lateral derecha (strafe right)\n      
        case 'Q': rotateLeft(); break;     // Rotar a la izquierda\n      
        case 'E': rotateRight(); break;    // Rotar a la derecha\n      
        case 'X': stopWheels(); break;     // Detener ruedas\n      \n      
        // Sistema de polea/elevador:\n      
        case 'U': setPulley(FORWARD); break;  // Subir elevador\n      
        case 'J': setPulley(BACKWARD); break; // Bajar elevador\n      
        case 'P': setPulley(STOP); break;      // Detener elevador\n      \n      
        // Servos de la garra:\n      
        case 'O': clawOpen(); break;       // Abrir garra\n      
        case 'C': clawClose(); break;      // Cerrar garra\n      
        // Comandos adicionales pueden agregarse si es necesario\n      
        default: break;
      }
    }
  }
}
