//Manual ESC Control
//Control motor using potentiometer

#include <Servo.h>
#define MIN_POW 1200
#define MAX_POW 2000

Servo esc;
int controlPin = 2; //To ESC; Any PWM pin will work
int potPin = A1;     //From poten; any pwm pin

void setup(){
  Serial.begin(9600);
  esc.attach(controlPin);
}

void loop(){
  int power = analogRead(potPin);
  Serial.print("Input: ");
  Serial.print(power);
  power = map(power, 0, 1023, MIN_POW, MAX_POW);
  Serial.print(", Mapped: ");
  Serial.println(power);
  esc.writeMicroseconds(power);`
}
