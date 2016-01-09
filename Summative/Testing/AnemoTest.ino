//Anemometer Test
//Calculates windspeed from anemometer
//Displays to serial monitor

const float sensitivity = 1;  //Property of differential pressure sensor
int windPinSensor = 2;        //Pin of sensor

float ave=0,sum=0, cali, windspeed, voltage, pressure, offset; 
int input, reading;

void setup(){
  Serial.begin(9600);
  delay(200);
  //Calibration
  float csum = 0.0;
  for (int i=0;i<10;i++){
    reading = analogRead(windPinSensor);
    csum += reading;
    delay(1);
  }
  //Take 10 average readings to calibrate
  cali = csum / 10.0;
  //Determine offset using same function as below
  offset = cali - 240;
  Serial.print("Cali Raw: " + String (cali));
  Serial.println(", Offset: " + String (offset));
}

void loop(){
   //Take 10 readings and average them for most accurate display
  for (int i=0;i<10;i++){
    reading = analogRead(windPinSensor);
    input = reading - offset;
    voltage = 5.0 * input / 1023.0;
    pressure = voltage / sensitivity;   //Differential pressure in kPa
    sum += sqrt ( 2.0 * pressure / 1.2 );
    delay(1);
  }
  ave = sum / 10.0;
  //Function to convert reading into windspeed
  windspeed = 5515.570538*sqrt(ave) - 2165.047384*ave - 3494.745014;
  if(windspeed < 0.2) windspeed = 0;
  
  Serial.print("Raw: " + String(reading));
  Serial.println(", Speed: " + String(windspeed));
  windspeed = 0;
  ave = 0;
  sum = 0;
  delay(500);
}
