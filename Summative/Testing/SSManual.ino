//Testing of Seven Segment displays
//Manual number input
int pins[3][4] = {
	{2, 3, 4, 5}, //Pins for MSD
	{6, 7, 8, 9},
	{10, 11, 12, 13} //Pins for LSD
};
int decPins[] = {5, 10}; //Pins for decimal point

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
  for(int i=2;i<=13;++i) pinMode(i, OUTPUT); //Set pins to output
}

void loop(){
  for(int i=0;i<=999;i++){
    outNum(i);
    delay(500);
  }
}
