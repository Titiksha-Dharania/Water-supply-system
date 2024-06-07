#include <TinyGPS.h>
TinyGPS gps;
float temp, pressure, ph;
float slope = 0.59;
float offset = 2.97;          //the offset to account for variability of pH meter
float offset2 = 0;            //offset after calibration
unsigned long int avg;
int a[5], b;

void setup() {
  pinMode (11, OUTPUT);
  pinMode (12, OUTPUT);
  pinMode (13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  for(int i=0; i<5; i++){                   //get 5 sample values from the sensor{ 
    a[i] = analogRead(A0);
    delay(10);                            //short delay between readings
  }

  temp = analogRead (A1);
  pressure = analogRead (A2);

    for(int i=0; i<4; i++){                    //sort the analog values from small to large{
    for(int j=i+1; j<5; j++){
      if(a[i] > a[j]){
        b = a[i];              
        a[i] = a[j];          
        a[j] = b;              
      }
    }
  }
  avg = 0;
  for(int i=1; i<4; i++)                    //take the value total of 6 center array values
    {
      avg += a[i];                 //get total
    }
  ph = (float)avg*5.0/1024/5;
  ph = (slope*3.5*ph + offset + offset2);

  temp= (temp*500)/1024;
  pressure = (pressure/(float)1023+0.095)/0.009;
  

  Serial.print ("Temperature: ");
  Serial.print (temp);
  Serial.print (" Celcius");
  Serial.println ();  
  
  Serial.print ("Pressure: ");
  Serial.print (pressure);
  Serial.print (" kPa");
  Serial.println (); 

  Serial.print ("pH: ");
  Serial.print (ph);
  Serial.println ();

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for(unsigned long start=millis(); millis() - start < 1000;){
    while(Serial.available()){
      char c = Serial.read();
      if (gps.encode(c))
        newData = true;
    }
  }

  if (newData) {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    Serial.print("Latitude = ");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" Longitude = ");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.println();

  }
 
  Serial.println(failed);

  if (ph<7)
  digitalWrite (11, HIGH);
  else
  digitalWrite (11, LOW);

  if (pressure>100)
  digitalWrite (12, HIGH);
  else
  digitalWrite (12, LOW);

  if (temp>30)
  digitalWrite (13, HIGH);
  else
  digitalWrite (13, LOW);


  delay(2000);

}
