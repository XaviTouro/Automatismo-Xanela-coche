/************************************************************************+************************************************************************
*  Imos imitar o funcionamento simplificado dunha xanela dun coche.
*  Cando prememos unha vez, o pulsador activa un motor que move o vidro cara arriba; ao premer unha segunda vez, 
*  o pulsador activa outro motor que move o vidro cara abaixo. Tanto as entradas como as saídas son dixitais, 
*  polo que imos activar os motores mediante dous relés. Cada un dos motores para ao estar aceso 7 segundos. 
*  Durante ese tempo, o pulsador pode en calquera momento parar o motor activo e activar o que está parado 
*  (volver atrás, sen pasar por paro). É dicir, hai que asegurar que o pulsador é reactivo ante calquer accionamento do usuario.
*  Imos facer a montaxe dos dous relés e o pulsador, asegurando que non haxa posibilidade de queimar a placa Arduíno mediante unha sobrecarga .
*  En particular teremos de protexer o pulsador, de maneira que ante un erro de programación, non se chegue a pór en curto o pin de entrada. 
*  O pin debe estar protexido para funcionar a máximo 20 mA e dar un ‘1’ lóxico ao chegarlle mínimo 3.5 V. 
*  As resistencias de protección deben ter valores comerciais e soportar a potencia empregada.
*
*  Autor: Javier Figueiro Resúa
*  Data: Febreiro 2024
*
*************************************************************************************************************************************************/

// Definición de pines
const int botonPin = 13;  // Pin del pulsador
const int relePin1 = 7;  // Pin del relé del motor que mueve el vidrio hacia arriba
const int relePin2 = 8;  // Pin del relé del motor que mueve el vidrio hacia abajo

// Variables para controlar el estado de los motores
boolean motorArribaActivo = false;
boolean motorAbajoActivo = false;

// Variable para el tiempo de trabajo
unsigned long tiempoInicio;

// Duración del tiempo de trabajo (en milisegundos)
const unsigned long duracionTrabajo = 7000; // 7 segundos

void setup() {
  // Configurar los pines
  pinMode(botonPin, INPUT_PULLUP);  // Habilitar resistencia pull-up interna para el botón
  pinMode(relePin1, OUTPUT);
  pinMode(relePin2, OUTPUT);
  
  // Inicializar relés (apagados)
  digitalWrite(relePin1, LOW);
  digitalWrite(relePin2, LOW);
}

void loop() {
  // Verificar si se ha pulsado el botón
  if (digitalRead(botonPin) == LOW) {
    // Llamar a la función para manejar el botón
    manejarBoton();
  }

  // Verificar si ha pasado el tiempo de trabajo y detener el motor activo si es necesario
  if ((motorArribaActivo || motorAbajoActivo) && (millis() - tiempoInicio >= duracionTrabajo)) {
    if (motorArribaActivo) {
      detenerMotorArriba();
    } else if (motorAbajoActivo) {
      detenerMotorAbajo();
    }
  }
}

// Función para manejar la lógica del botón
void manejarBoton() {
  if (!motorArribaActivo && !motorAbajoActivo) {
    // Si ninguno de los motores está activo, activar el motor que mueve el vidrio hacia arriba
    activarMotorArriba();
  } else if (motorArribaActivo && !motorAbajoActivo) {
    // Si el motor que mueve el vidrio hacia arriba está activo, detenerlo y activar el motor que mueve el vidrio hacia abajo
    detenerMotorArriba();
    activarMotorAbajo();
  } else if (!motorArribaActivo && motorAbajoActivo) {
    // Si el motor que mueve el vidrio hacia abajo está activo, detenerlo y activar el motor que mueve el vidrio hacia arriba
    detenerMotorAbajo();
    activarMotorArriba();
  }
  // Reiniciar el temporizador
  tiempoInicio = millis();
}

// Función para activar el motor que mueve el vidrio hacia arriba
void activarMotorArriba() {
  digitalWrite(relePin1, HIGH);
  motorArribaActivo = true;
  motorAbajoActivo = false;
  digitalWrite(relePin2, LOW); // Asegurar que el otro motor esté apagado
}

// Función para detener el motor que mueve el vidrio hacia arriba
void detenerMotorArriba() {
  digitalWrite(relePin1, LOW);
  motorArribaActivo = false;
}

// Función para activar el motor que mueve el vidrio hacia abajo
void activarMotorAbajo() {
  digitalWrite(relePin2, HIGH);
  motorAbajoActivo = true;
  motorArribaActivo = false;
  digitalWrite(relePin1, LOW); // Asegurar que el otro motor esté apagado
}

// Función para detener el motor que mueve el vidrio hacia abajo
void detenerMotorAbajo() {
  digitalWrite(relePin2, LOW);
  motorAbajoActivo = false;
}
