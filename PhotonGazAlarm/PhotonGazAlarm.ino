int SensorA0;

int buzzzer = D0;
int sensorValue;
int threshold = 1500;  // Eşik değeri, Siz bunu uygun bir değer olarak düzenleyin.
void setup() 
{
    pinMode(0, OUTPUT);//Buzzer ayağı

    Spark.variable("SensorA0", &SensorA0, INT);//sensörü A0'a bağla
    

}

void loop() 
{
    
sensor_degeri= analogRead(A0);  // A0'dan değerleri oku
    
  if(sensor_degeri> threshold){
   Spark.publish("PartyHard");
   digitalWrite(buzzzer, HIGH);
   delay(15000); // flood olmaması için az bekle
   
   sensor_degeri= analogRead(A0);
   
   if(sensor_degeri> threshold){
   Spark.publish("PartyHard");
   digitalWrite(buzzzer, HIGH);
   delay(60000);
   }
   
   sensor_degeri= analogRead(A0);
   
   if(sensor_degeri> threshold){
   Spark.publish("PartyHard");
   digitalWrite(buzzzer, HIGH);
   delay(120000);
   }
   
   sensor_degeri= analogRead(A0);
   
   if(sensor_degeri> threshold){
   Spark.publish("PartyHard");
   digitalWrite(buzzzer, HIGH);
   delay(600000);
   }
   
   sensor_degeri= analogRead(A0);
   
   if(sensor_degeri> threshold){
   Spark.publish("PartyHard");
   digitalWrite(buzzzer, HIGH);
   delay(600000);
   }
   
    digitalWrite (buzzzer, LOW);
  }

  
  
  
 
  delay(100);     
  

    
}
