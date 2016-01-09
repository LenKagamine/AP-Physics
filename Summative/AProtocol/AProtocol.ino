//Ardulink Protcol for 1315-MH Wind Tunnel
//This protocol is uploaded to the wind tunnel and
//is used for communicating between the wind tunnel's components and the GUI.

#include <Servo.h>
#include <HX711.h>

Servo esc;    //ESC
int servoPin; //Pin that ESC is attached to

double loadCalib = -7050.0;  //Calibation factor for load cell amps
int clockPin = 49;
HX711 scale(11, clockPin);
HX711 scale2(10, clockPin);
HX711 scale3(9, clockPin);
HX711 scale4(8, clockPin);
HX711 scale5(53, 51);
int scaleListenedValue[4];
bool scaleListening = false; //Listen for value

String inputString = "";        //Incoming data from Ardulink
boolean stringComplete = false; //If incoming string is completed

void setup() {
  Serial.begin(115200);
  
  servoPin = -1; //ESC not attached yet
  
  for(int i=0; i<4; i++){ //Reset values for scale
    scaleListenedValue[i] = -1;
  }
  scale.set_scale(loadCalib);
  scale2.set_scale(loadCalib);
  scale3.set_scale(loadCalib);
  scale4.set_scale(loadCalib);
  scale5.set_scale(loadCalib);
}

void loop() {
  if(stringComplete){
    if(inputString.startsWith("alp://")){ //Message from ALProtocol
      boolean msgRecognized = true;

      if(inputString.substring(6,10) == "ppsw") { //Power Pin Switch: digitalWrite (alp://ppsw/[pin]/[power]), for seven segments
        int separatorPosition = inputString.indexOf('/', 11);
        String pin = inputString.substring(11, separatorPosition);
        String power = inputString.substring(separatorPosition + 1);
        
        pinMode(pin.toInt(), OUTPUT);
        if(power.toInt() == 1) digitalWrite(pin.toInt(), HIGH);
        else if(power.toInt() == 0) digitalWrite(pin.toInt(), LOW);
      }
      
      else if(inputString.substring(6,15) == "cust/pesc"){ //Attach/Detach & Power ESC (alp://cust/pesc/[pin]/[value])
        int separatorPosition = inputString.indexOf('/', 16);
        int pin = inputString.substring(16, separatorPosition).toInt();
        int value = inputString.substring(separatorPosition + 1).toInt();
        
        if(pin == -1) esc.detach(); //Detach ESC
        else{
          if(pin != servoPin){ //Already attached, move to different pin
            if(servoPin != -1) esc.detach(); //Previously attached to a different pin
            servoPin = pin;
            esc.attach(servoPin);
          }
          esc.writeMicroseconds(value);
        }
      }
      
      else if(inputString.substring(6,15) == "cust/load"){ //Request value for load cells (alp://cust/load)
        scaleListening = true;
      }
      else if(inputString.substring(6,15) == "cust/tare"){ //Reset load cells (load cells #1 - #4) (alp://cust/tare)
        scale.tare(1);
        scale2.tare(1);
        scale3.tare(1);
        scale4.tare(1);
      }
      else if(inputString.substring(6,15) == "cust/wind"){ //Reset wind speed sensor (load cell #5) (alp://cust/wind)
        scale5.tare(1);
      }
      else{ //Not a valid command, command is ko (not ok)
        msgRecognized = false; 
      }
    }
    
    //Clear the string
    inputString = "";
    stringComplete = false;
  }
  
  if(scaleListening){ //Load cell values (alp://load/[num]/[value])
    scaleListening = false;
    double value;
    for(int i = 0; i < 5; i++){
      if(i == 0) value = scale.get_units();
      else if(i == 1) value = scale2.get_units();
      else if(i == 2) value = scale3.get_units();
      else if(i == 3) value = scale4.get_units();
      else if(i == 4) value = scale5.get_units();
      if(scaleListenedValue[i] != value){
        scaleListenedValue[i] = value;
        Serial.print("alp://load/");
        Serial.print(i);
        Serial.print("/");
        Serial.print(value, 1);
        Serial.write(255);
        Serial.flush();
      }
    }
  }
}

void serialEvent(){
  while(Serial.available() && !stringComplete){
    char inChar = (char)Serial.read();
    inputString += inChar;
    if(inChar == '\n') stringComplete = true;
  }
}


