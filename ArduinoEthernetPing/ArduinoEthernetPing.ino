
#include <SPI.h>
#include <Ethernet.h>

//Mac Adresini Tanımlıyoruz. Bu sizin elinizle vereceğiniz bir adres
//olabilir veya kartın etiketinde (varsa) olan adres olabilir
//Mac adresi 6 baytlık bir dizidir ve genellikle onaltılık olarak 
//gösterilir.
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
//IP adresini tanımlıyoruz. Bu yerel ağa ait IP adresi olup 4lü
//Bayt dizisi halindedir. IP adresi ağ geçiti ile uyumlu olmalıdır
IPAddress ip(192, 168, 1, 177);
//DNS tanımlama 
IPAddress myDns(192,168,1, 1);
//Ağ geçiti tanımlama. Modem IP'si olacak. Genelde 192.168.1.1 oluyor.
IPAddress gateway(192, 168, 1, 1);
//Ağ alt maskesi
IPAddress subnet(255, 255, 0, 0);


// Sunucuyu başlat
EthernetServer server(23);

void setup() {
 // Çeşitli değerlerle başlat.
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
 // Sunucuyu aç
  server.begin();
// Seri monitör ekranını aç 
  Serial.begin(9600);

  

// Adresi ekranda göster. 
  Serial.print("Adres:");
  Serial.println(Ethernet.localIP());
}

void loop() {
 

  }

