#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // Başlık Dosyası
#include <SoftwareSerial.h>

int numRuns = 1;  
int maxRuns = 5;  
int trigPin = 2;
int echoPin = 4;
long duration, cm, inches;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  delay(5000);
  Bridge.begin();
  Serial.println("Saptama Moduna Giris Yapildi...");
}

void loop() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  inches = (duration / 2) / 74;
  Serial.println(".....");
  if (inches < 20 || inches > 500) {
  Serial.println("Izinsiz Giris Saptandi!");
  Serial.println("SMS Yollaniyor...");

  if (numRuns <= maxRuns) {
    Serial.println("Running SendSMS - Run #" + String(numRuns++));
    
    TembooChoreo SendSMSChoreo;

  
    SendSMSChoreo.begin();

    // Temboo hesap 
    SendSMSChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendSMSChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendSMSChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // Set Choreo inputs
    SendSMSChoreo.addInput("AuthToken", "); //  AUTH TOKEN 
    SendSMSChoreo.addInput("To", "+"); // Mesajın gitmesini istediğiniz telefon numarası
    SendSMSChoreo.addInput("From", "+"); // Twilio Telefon Numarası
    SendSMSChoreo.addInput("Body", "Izinsiz Giris Goruldu!!!!!!"); // Gönderilecek Mesaj
    SendSMSChoreo.addInput("AccountSID", ""); // Account SID
    
    // tanımla
    SendSMSChoreo.setChoreo("/Library/Twilio/SMSMessages/SendSMS");
    
    // choreoyu başlat ve seri port ekranında göster
    SendSMSChoreo.run();
    
    while(SendSMSChoreo.available()) {
      char c = SendSMSChoreo.read();
      Serial.print(c);
    }
    SendSMSChoreo.close();
  }

  Serial.println("Bekleniyor...");
  delay(10000); //SMSler arası 10sn bekle
  
  Serial.println("Saptama Moduna Donus Yapildi...");
  
  }
  
}
