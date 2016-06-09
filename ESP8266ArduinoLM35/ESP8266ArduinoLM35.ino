#include <SoftwareSerial.h>
#include <stdlib.h>

// LED 
int ledPin = 13;
// LM35 analog giriş
int lm35Pin = A0;

// ThingSpeak API Anahtarı ile değiştirin
String apiKey = "XXXXXXXXXXXXXXX";

// 10'u TX'e Bağlayın 11'i RX'e
SoftwareSerial ser(10, 11); // RX, TX


void setup() {                

  pinMode(ledPin, OUTPUT);    


  Serial.begin(9600); 
  // Yazılımsal Seri monitör aç
  ser.begin(9600);
  
  // Resetle
  ser.println("AT+RST");
}



void loop() {
  
  // Ledi yak ve söndür
  digitalWrite(ledPin, HIGH);   
  delay(200);               
  digitalWrite(ledPin, LOW);

  // LM35'den 10 değer al ortalamasını bul
  int val = 0;
  for(int i = 0; i < 10; i++) {
      val += analogRead(lm35Pin);   
      delay(500);
  }

  // Sıcaklığı çevir
  float temp = val*50.0f/1023.0f;

  // Stringe dönüştür
  char buf[16];
  String strTemp = dtostrf(temp, 4, 1, buf);
  
  Serial.println(strTemp);
  
  // TCP Bağlantısı
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com
  cmd += "\",80";
  ser.println(cmd);
   
  if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
  
  // GET Stringini hazırla
  String getStr = "GET /update?api_key=";
  getStr += apiKey;
  getStr +="&field1=";
  getStr += String(strTemp);
  getStr += "\r\n\r\n";

  // Veri Stringini yolla
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ser.println(cmd);

  if(ser.find(">")){
    ser.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");
    // Uyar
    Serial.println("AT+CIPCLOSE");
  }
    
  // 15 saniye bekle. 
  delay(16000);  
}
