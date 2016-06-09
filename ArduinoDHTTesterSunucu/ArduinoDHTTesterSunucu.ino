
#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

#define DHTPIN A3     // DHT Data Ayağının Bağlı olduğu yer

#define DHTTYPE DHT22   // DHT 22 Kullanıyoruz.

DHT dht(DHTPIN, DHTTYPE);
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);


EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  pinMode(A5,OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(A5,LOW); // GND Ayağı
  digitalWrite(A2,HIGH); // Besleme ayağı 
  dht.begin(); // DHT başlat. 
    Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Server IP ");
  Serial.println(Ethernet.localIP());
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

 EthernetClient client = server.available();
  if (client) {
    Serial.println("Yeni istemci");

    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
    
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
          client.println("Refresh: 5");  
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
    
 
            client.print("Sicaklik:");
            client.print(t);
            client.print("C     ");
            client.print("Nem:");
            client.print(h);
            client.print("% ");
            client.println("<br />");
          
          client.println("</html>");
          break;
        }
        if (c == '\n') {
        
          currentLineIsBlank = true;
        } else if (c != '\r') {
         
          currentLineIsBlank = false;
        }  
      }
    }
}
    
    delay(1);

    client.stop();
    Serial.println("Istemci Cikti");
    Ethernet.maintain();
  }

