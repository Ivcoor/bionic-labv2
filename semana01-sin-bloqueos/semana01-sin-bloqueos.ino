#include <Servo.h>
Servo miServo;
unsigned long ultimaLecturaMs{0}; 
constexpr uint8_t pinPotenciometro{A0};
constexpr unsigned long periodoLecturaMs{10};
constexpr unsigned long periodoMovimiento{20};
constexpr unsigned long periodoLed(500);
constexpr uint8_t pinServo{9};
constexpr int umbralLectura{5};
long ultimoValorPotenciometro{0};  
int anguloActual{45};
unsigned long ultimoMovimiento{0};
unsigned long ultimoEncendido{0};
bool estadoLed = false;
enum class EstadoDedo {
  ABIERTO,
  CERRADO
};
EstadoDedo estadoActualDedo{EstadoDedo::ABIERTO};

void setup(){
Serial.begin(9600);
miServo.attach(pinServo);
miServo.write(anguloActual);
pinMode(LED_BUILTIN, OUTPUT);
}


void loop(){
  const unsigned long ahoraMs{millis()};  

  if(ahoraMs-ultimaLecturaMs >= periodoLecturaMs){
    
    //Actualizamos el tiempo de la ultimaLecturaMS
    ultimaLecturaMs += periodoLecturaMs;
    long valorPotenciometro{analogRead(pinPotenciometro)};
  

    
    if (estadoActualDedo == EstadoDedo::ABIERTO &&
        valorPotenciometro > 600) {
        estadoActualDedo = EstadoDedo::CERRADO;
    }

    else if (estadoActualDedo == EstadoDedo::CERRADO &&
         valorPotenciometro < 400) {
          estadoActualDedo = EstadoDedo::ABIERTO;
      
    }   

  }

  
  if(ahoraMs-ultimoMovimiento >= periodoMovimiento){
    
    int anguloObjetivo{0};

    if(estadoActualDedo == EstadoDedo::ABIERTO){
      anguloObjetivo = 45;
    }
    else{
      anguloObjetivo = 120;
    }

     if (anguloObjetivo > anguloActual) {
        anguloActual += 1;
    }
    else if (anguloObjetivo < anguloActual) {
        anguloActual -= 1;
    }

    miServo.write(anguloActual);
    ultimoMovimiento += periodoMovimiento;
  }

  if(ahoraMs - ultimoEncendido >=  periodoLed){
    estadoLed = !estadoLed;
    ultimoEncendido += periodoLed;
    digitalWrite(LED_BUILTIN, estadoLed);

  }
  
}

