//Manual ESC Control + Seven Segment display
//Control motor using potentiometer
//Displays % output to Seven Segment displays

#include <Servo.h>
#define MIN_POW 1200
#define MAX_POW 2000

Servo esc;
int controlPin = 2;  //To ESC; Any PWM pin will work
int potPin = A1;     //From poten; any pwm pin

int pins[3][4] = {
	{24, 23, 22, 25}, //Pins for MSD
	{28, 27, 26, 29},
	{32, 31, 30, 33} //Pins for LSD
};
int decPins[] = {34, 35}; //Pins for decimal point

void outDigit(int pin, int digit){ //Outputs single digit as 4 bit digit
  for(int i=3;i>=0;--i){
    digitalWrite(pins[pin][i], digit%2);
    digit /= 2;
  }
}

void outNum(double num){ //Output 3 digit number as 3 * 4 bits
  digitalWrite(decPins[0], 0);
  digitalWrite(decPins[1], 0);
  if(num < 0) num *= -1;
  else if(num < 10){
    digitalWrite(decPins[0], 1);
    num *= 100;
  }
  else if(num < 100){
    digitalWrite(decPins[1], 1);
    num *= 10;
  }
  for(int i=2;i>=0;--i){
    outDigit(i, (int)num%10);
    num /= 10;
  }
}

void setup(){
  Serial.begin(9600);
  esc.attach(controlPin); //Attach ESC to pin
  
  for(int i=0;i<3;i++){ //Set pins to output
    for(int j=0;j<4;j++){
      pinMode(pins[i][j], OUTPUT);
    }
  }
  for(int i=0;i<2;i++) pinMode(decPins[i], OUTPUT);
}

void loop(){
  int power = analogRead(potPin);
  outNum(map(power, 0, 1023, 0, 100));
  
  Serial.print("Input: ");
  Serial.print(power);
  power = map(power, 0, 1023, MIN_POW, MAX_POW);
  Serial.print(", Mapped: ");
  Serial.println(power);
  esc.writeMicroseconds(power);
}
