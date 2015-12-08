//Automatic Testing of Seven Segment Display
//Loops through all numbers and decimal points
int pins[3][4] = {
	{22, 23, 24, 25}, //Pins for MSD
	{26, 27, 28, 29},
	{30, 31, 32, 33} //Pins for LSD
};
int decPins[] = {34, 35}; //Pins for decimal point

void outDigit(int pin, int digit){ //Outputs single digit as 4 bit digit
  for(int i=3;i>=0;--i){
    digitalWrite(pins[pin][i], digit%2);
    digit /= 2;
  }
}

void setup(){
  for(int i=22;i<=35;++i) pinMode(i, OUTPUT); //Set pins to output
}

void loop(){
  for(int i=0;i<=9;++i){
    outDigit(0,i);
    outDigit(1,i);
    outDigit(2,i);
    digitalWrite(decPins[0], i%2);
    digitalWrite(decPins[0], i%2);
    delay(1000);
  }
}
