#include <Servo.h>
Servo miServo;
unsigned long ultimaLecturaMs{0}; 
constexpr uint8_t pinPotenciometro{A0};
constexpr unsigned long periodoLecturaMs{10};
constexpr uint8_t pinServo{9};

void setup(){
Serial.begin(9600);
miServo.attach(pinServo);
}
void loop(){
unsigned long ahoraMs{millis()};
if(ahoraMs-ultimaLecturaMs >= periodoLecturaMs){
  //Actualizamos el tiempo de la ultimaLecturaMS
  int valorPotenciometro{analogRead(pinPotenciometro)};
  Serial.println(valorPotenciometro);
  ultimaLecturaMs += periodoLecturaMs;

  //Establecemos el ángulo del servo y lo movemos.
  int angulo{map(valorPotenciometro, 0, 1023, 45, 135)};
  miServo.write(angulo);
}

}
