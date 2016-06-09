
#include "DHT.h"

#define DHTPIN A3     // DHT Data Ayağının Bağlı olduğu yer

#define DHTTYPE DHT22   // DHT 22 Kullanıyoruz.

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  pinMode(A5,OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(A5,LOW); // GND Ayağı
  digitalWrite(A2,HIGH); // Besleme ayağı 
  dht.begin(); // DHT başlat. 
}

void loop() {
 
  delay(2000);

  
  float h = dht.readHumidity();
 
  float t = dht.readTemperature();
 
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("DHT Sensor Okuma Hatasi!");
    return;
  }

 

  Serial.print("Nem: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Sicaklik: ");
  Serial.print(t);
  Serial.println(" *C ");

}
