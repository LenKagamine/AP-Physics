//Anemometer Test
//Calculates windspeed from anemometer
//Displays to serial monitor

const float offset = 1.0;      //Offset to match 0 from sensor to 0 wind speed
const float sensitivity = 1.0; //Property of differential pressure sensor

int windPinSensor = A0;        //Pin of sensor

void setup(){
  Serial.begin(9600);
}

String ssDisplay(float wind){
  if(wind < 0) wind *= -1;
  return String(wind).substring(0,4);
}

void loop(){
  //int rawADC = analogRead(windPinSensor);
  int rawADC = random(0, 1023);
  float voltage = (float) rawADC / 1023.0 * 5.0;   // voltage at Arduino pin. Range is 5V, 10 bits.
  float pressure = (voltage - offset) / sensitivity;   // differential pressure in kPa
  float windspeed = sqrt ( 2.0 * pressure / 1.2 );
  Serial.print("Raw: ");
  Serial.print(windspeed);
  Serial.println(", Display: " + ssDisplay(windspeed));
  delay(1);
}
