#define AG_ "BurayaAgAdiniYazin"
#define SIFRE_ "BurayaSifreYazin"
#define IP "184.106.153.149" // ThingSpeak.com'un IP ADRESI
String API_KEY("XXXXXXXXXXXX");
void setup()
{
// Seri port işlemleri:
Serial.begin(115200); 
//ESP ile iletişimde 115200 baud rate ayarlandı. 
Serial.println("AT");
delay(5000);
// Wi-Fi Bağlantısını ayarla. 
if(Serial.find("OK")){ 
Serial.println("AT+CWMODE=1");
delay(2000);
String gerekliBilgiler=String("AT+CWJAP=\"")+AG_+"\",\""+SIFRE_+"\"";
Serial.println(gerekliBilgiler);
delay(10000); // 10 saniye bağlanmak için bekle. 
}
}
void loop(){
float val = analogRead(A0);
float mv = ( val/1024.0)*5000;
float cel = mv/10;
verileriYolla(cel);
// 30 saniye bekleyelim. 
delay(30000);
}
void verileriYolla(float cel){
// Sunucu Bağlantısını yap
Serial.println(String("AT+CIPSTART=\"TCP\",\"") + IP + "\",80");
// 3 saniye bağlantı kurulması için bekleyelim. 
delay(3000);
// ve değeri sunucuya gönderelim.
String veriYolla = "GET /update?key=";
veriYolla += (API_KEY + "&field1=" + String(cel) + "\r\n");
Serial.println(veriYolla.length());
// Sorun oluşmadıysa veriyi göndersin.
if(Serial.find(">")){
Serial.print(veriYolla);
}
else{
Serial.println("AT+CIPCLOSE");
}
}
