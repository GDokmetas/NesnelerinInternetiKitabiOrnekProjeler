#include <AvrI2c_Greiman.h>
#include <LiquidCrystal_I2C_AvrI2C.h>
LiquidCrystal_I2C_AvrI2C lcd (0x27,16,2);

#include "Bridge.h"
#include "YunServer.h"
#include "YunClient.h"

YunServer server;

void setup() {
  // Bridge startup
  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  server.listenOnLocalhost();
  server.begin();
  lcd.begin();
}

void loop() {
  
  YunClient client = server.accept();

 
  if (client) {
    
    process(client);

    
    client.stop();
  }

  delay(50); 
}

void process(YunClient client) {
  // komutu oku
  String command = client.readStringUntil('/');

  // mesaj komutuysa gerçekleştir
  if (command == "message") {
    messageCommand(client);
  }
}

void messageCommand(YunClient client) {

  String message = client.readStringUntil('/');

  // İstemciye dönüş yap
  client.print(F("Mesaj"));
  client.println(message);

  // Güncellemeleri yap
  String key = "Message";
  Bridge.put(key, message);
  
  // Ekrana mesajı yolla

  lcd.print(message);
}
