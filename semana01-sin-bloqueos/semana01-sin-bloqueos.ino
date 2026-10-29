#include <Servo.h>
Servo miServo;
unsigned long ultimaLecturaMs{0}; 
constexpr uint8_t pinPotenciometro{A0};
constexpr unsigned long periodoLecturaMs{10};
constexpr unsigned long periodoMovimiento{50};
constexpr unsigned long periodoLed(500);
constexpr unsigned long periodoSerialMs{50};
constexpr uint8_t pinServo{9};
constexpr int umbralCierre{600};
constexpr int umbralApertura{400};
int anguloActual{45};
int ultimoValorADC{0};
unsigned long ultimaImpresionMs{0};
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
    int valorPotenciometro{analogRead(pinPotenciometro)};
    ultimoValorADC = valorPotenciometro;
    
    if (estadoActualDedo == EstadoDedo::ABIERTO &&
        valorPotenciometro > umbralCierre) {
        estadoActualDedo = EstadoDedo::CERRADO;
    }

    else if (estadoActualDedo == EstadoDedo::CERRADO &&
         valorPotenciometro < umbralApertura) {
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
        anguloActual += 5;
    }
    else if (anguloObjetivo < anguloActual) {
        anguloActual -= 5;
    }

    miServo.write(anguloActual);
    ultimoMovimiento += periodoMovimiento;
  }

  if(ahoraMs - ultimoEncendido >=  periodoLed){
    estadoLed = !estadoLed;
    ultimoEncendido += periodoLed;
    digitalWrite(LED_BUILTIN, estadoLed);

  }
  
  if(ahoraMs - ultimaImpresionMs >= periodoSerialMs){
    ultimaImpresionMs += periodoSerialMs;
    Serial.println(ultimoValorADC);
  }
}

