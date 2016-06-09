
#include <SPI.h>
#include <Ethernet.h>

boolean incoming = 0;

// Buraya kullanmak istediğimiz MAC adresini giriyoruz.
// Mac adresi bizim istediğimiz bir adres olabilir. 
byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDA, 0x02 };
IPAddress ip(192,168,1,177); //<Buraya kullanmak istediğiniz Ip adresini girin

// HTTP için port 80'i aç ve kütüphaneyi tanımla
EthernetServer server(80);

void setup()
{
  pinMode(2, OUTPUT); //2. bacak çıkış olarak ayarlandı

  // Bağlantıyı ve sunucuyu başlat. 
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.begin(9600);
}

void loop()
{
  // Gelen istemcileri dinle.
  EthernetClient client = server.available();
  if (client) {
    // Http isteği boşlukla biterse
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // Eğer boş http isteği yollanırsa birşey olmaz.
    
        
        //$ karakterinden itibaren karakter cilerini okur
        if(incoming && c == ' '){ 
          incoming = 0;
        }
        if(c == '$'){ 
          incoming = 1; 
        }
        
        //URL'nin $1 veya $2 ile bitip bitmediğini denetler.
        if(incoming == 1){
          Serial.println(c);
          
          if(c == '1'){
            Serial.println("ACIK");
            digitalWrite(2, HIGH);
          }
          if(c == '2'){
            Serial.println("KAPALI"); // Seri monitore de durum bildirir
            digitalWrite(2, LOW);
          }
        
        }

        if (c == '\n') {
          // Yeni satırsa 
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          //Satirda karakter alirse
          currentLineIsBlank = false;
        }
      }
    }
    // Tarayıcıya biraz süre ver. 
    delay(1);
    // İşi bitir
    client.stop();
  }
}

