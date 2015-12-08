//Testing PWM control to motor via ESC

int pin = 2; //ESC Control pin

void setup(){
  pinMode(pin, OUTPUT); //Set pin to output
}

void loop(){
  for(int i=0;i<256;++i){ //Gradually increase to max RPM
    analogWrite(pin, i);
    delay(200);
  }
  for(int i=255;i>=0;--i){ //Gradually decrease to stop
    analogWrite(pin, i);
    delay(200);
  }
}
