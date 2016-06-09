#include <SPI.h>
#include <Ethernet.h>

// Local Network Settings
byte mac[] = { 0xD4, 0x28, 0xB2, 0xFF, 0xA0, 0xA1 }; // Mac Adresi

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "BURAYA API ANAHTARINI YAZIN";
const int updateThingSpeakInterval = 16 * 1000;      // Thingspeak yenileme hızı

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int failedCounter = 0;


EthernetClient client;

void setup()
{
  // Seri monitör ekranını aç, ayrıca bakabilirsiniz. 
  Serial.begin(9600);
  
  
  startEthernet();
}

void loop()
{
  // A0 ayağından veriyi al ve karakter dizisine dönüştür
  String analogValue0 = String(analogRead(A0), DEC);
  
  // İstemci hazırsa ekrana yazdır
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  // Thingspeak'dan ayrıl
  if (!client.connected() && lastConnected)
  {
    Serial.println("...disconnected");
    Serial.println();
    
    client.stop();
  }
  
  // Thingspeak güncelle
  if(!client.connected() && (millis() - lastConnectionTime > updateThingSpeakInterval))
  {
    updateThingSpeak("field1="+analogValue0);
  }
  
  // Ethernet yeniden başlatma istiyorsa denetle
  if (failedCounter > 3 ) {startEthernet();}
  
  lastConnected = client.connected();
}

void updateThingSpeak(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  {         
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    
    lastConnectionTime = millis();
    
    if (client.connected())
    {
      Serial.println("ThingSpeake Baglaniliyor...");
      Serial.println();
      
      failedCounter = 0;
    }
    else
    {
      failedCounter++;
  
      Serial.println("Thingspeak baglantisi basarisiz ("+String(failedCounter, DEC)+")");   
      Serial.println();
    }
    
  }
  else
  {
    failedCounter++;
    
    Serial.println("Thingspeak baglantisi  basarisiz ("+String(failedCounter, DEC)+")");   
    Serial.println();
    
    lastConnectionTime = millis(); 
  }
}

void startEthernet()
{
  
  client.stop();

  Serial.println("Arduino aga baglaniyor...");
  Serial.println();  

  delay(1000);
  
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("DHCP Hatasi, bir daha deneyin");
    Serial.println();
  }
  else
  {
    Serial.println("DHCP Baglantisi Basarili, Arduino Baglandi");
    Serial.println();
  }
  
  delay(1000);
}
