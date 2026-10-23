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
void setup(){
Serial.begin(9600);
miServo.attach(pinServo);
miServo.write(anguloActual);
pinMode(LED_BUILTIN, OUTPUT);
}


void loop(){
  const unsigned long ahoraMs{millis()};


  if(ahoraMs-ultimoMovimiento >= periodoMovimiento){
    int anguloObjetivo{map(ultimoValorPotenciometro, 0, 1023, 45, 120)};

    if (anguloObjetivo > anguloActual){
      anguloActual += 1;
    }
    else if (anguloObjetivo < anguloActual){
    anguloActual -= 1;}

    miServo.write(anguloActual);
    ultimoMovimiento += periodoMovimiento;
  }

  
  if(ahoraMs-ultimaLecturaMs >= periodoLecturaMs){
    //Actualizamos el tiempo de la ultimaLecturaMS
    ultimaLecturaMs += periodoLecturaMs;
    long valorPotenciometro{analogRead(pinPotenciometro)};
    //Lee si hay cambio de menos de 5 ADC. Si es así se ignora. 
    if(abs(ultimoValorPotenciometro-valorPotenciometro) >= umbralLectura){
      ultimoValorPotenciometro = valorPotenciometro;}
      
      

  }
  
  if(ahoraMs - ultimoEncendido >=  periodoLed){
    estadoLed = !estadoLed;
    ultimoEncendido += periodoLed;
    digitalWrite(LED_BUILTIN, estadoLed);

  }

}

