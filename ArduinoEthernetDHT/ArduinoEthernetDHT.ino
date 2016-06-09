/*
  E-Pot v.1.0
*/

#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>


// Sunucu Ayarları
//
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
//
EthernetClient client;

#define DHTPIN 2        // DHT Veri Ayağı
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// LDR Ayarı
int lightPin = 0;      // LDR değeri
int PRvalue = 0;       // Karşılaştırma değeri
int PRlight = 0;       // Sonuç değeri

String data;           // Gönderilecek veri 

void setup() {
 //
  Serial.begin(115200);

  
  Serial.println("Seri Baglanti Tamamlandi");
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("DHCP Tanimlama Hatasi");
  }
  
  dht.begin();
  Serial.println("DHT11 Baglanti Tamamlandi");
  
  data = "";
  
}

void loop() {
  delay(1000);

  Serial.println("Nem Sensoru Modulu");
  // Ölçüm arası biraz bekleyelim.
  
  int t = 0;      // temperature
  int h = 0;      // humidity
  
  // Sıcaklık ve nemi ölç. Yaklaşık 250 mili saniye tutar. 
  // Ayrıca ölçümler 2 saniye öncesinin değerleri olabilir.
  h = dht.readHumidity();
  delay(500);
  // Santigrat derece olarak oku. 
  t = dht.readTemperature();
  delay(500);

  // Hata olursa mesajla bildir.
  if (isnan(h) || isnan(t)) {
   Serial.println("DHT Algilayici Okuma Hatasi!");
   return;
  }
  
  // Şimdi de LDR okuması yapalım 
  PRvalue = analogRead(lightPin);
  
  // 500 burada tercihe bağlıdır. İstediğiniz değerde değiştirebilirsiniz.
  // Hassaslık da böylece değişecektir. 
  if (PRvalue>500) {
    PRlight = 1;    // Işık var!
  } 
  else {
    PRlight = 0;    // Işık yok!
  }
  
  // Test için seri port komutları.
  // 
  Serial.print("SICAKLIK: ");
  Serial.println(t);
  Serial.print("NEM: ");
  Serial.println(h);  
  Serial.print("ISIK: ");
  Serial.println(PRlight); 
  
  // Gönderilecek veriyi hazırla. 
  data = "ahum1=" + String(h) + "&temp1=" + String(t) + "&light=" + String(PRlight);
  
  Serial.println(data);    // Yine test amaçlı yazdır. 
  
  // Şimdi yolluyoruz. 
  if (client.connect("www.gokhandokmetas.net", 80)) {    // BURAYA SİTE İSMİNİZ (DOMAIN) YAZILACAK!!!
    Serial.println("Client connected.");
    client.println("POST /add.php HTTP/1.1");
    client.println("Host: gokhandokmetas.net");          // Burası yine domain olacak. 
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.println(data);
  }
  
  if (client.connected()) {
    client.stop();
  }
  
  // Şimdi bir saat bekleyelim. Milisaniyeyi saate çevirince 
  // 3600000 değeri çıkıyor. Bunu yazdırıyoruz. Long olarak belirtmeyi de unutmayın. 
 
  delay(3600000L);
}
