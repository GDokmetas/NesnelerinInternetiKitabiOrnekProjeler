

/*
 * Twitter2LCD
 * gets xml data from http://twitter.com/statuses/user_timeline/16297873.rss
 * reads the most recent tweet from field:  <title> 
 * writes the output to the LCD.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 See more here:
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */


#include <Ethernet.h>
#include <TextFinder.h>
#include <LiquidCrystal_I2C.h>

// LCD'yi Doğrudan Bağlayacaksanız şu komutu kullanın
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
// Eğer I2C modül kullanacaksanız bununla değiştirin
LiquidCrystal_I2C lcd (0x27,20,4); 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte server[] = {128,242,240,20}; // Twitter

char tweet[140];

EthernetClient client(server, 80);

TextFinder  finder( client );  

void setup()
{
  lcd.begin();
  //Eğer I2C kullanmayacaksanız şununla değiştirin 
  //lcd.begin(20,4)
  lcd.clear();
  lcd.print("Twitter LCD Okuyucu");
  Serial.begin(9600);
  Ethernet.begin(mac);
}


void loop()
{
  lcd.clear();
  if (client.connect()) {
    client.println("GET https://twitrss.me/twitter_user_to_rss/?user=elektrobilim&replies=on HTTP/1.0");  // twitter rss for fgranelli
    client.println();
  } 
  else {
    lcd.println("Connection failed");
    Serial.println("Connection failed");
  } 
  if (client.connected()) {
     // get the last tweet by simply parsing the item and title tags
     if((finder.find("<item>")&&(finder.getString("<title>","</title>",tweet,140)!=0)))
     {  
         Serial.println(tweet);
         
         for (int j=0; j<2; j++) {
           // first part of the tweet
           lcd.clear();
           lcd.setCursor(0,0);
           for (int i=0; i<20; i++)
             lcd.print(tweet[i]);
           lcd.setCursor(0,1);
           for (int i=20; i<40; i++)
             lcd.print(tweet[i]);
           lcd.setCursor(0,2);
           for (int i=40; i<60; i++)
             lcd.print(tweet[i]);
           lcd.setCursor(0,3);
           for (int i=60; i<80; i++)
             lcd.print(tweet[i]);
           delay(10000);
           // second part of the tweet
           lcd.clear();
           lcd.setCursor(0,0);
           for (int i=60; i<80; i++)
             lcd.print(tweet[i]);
           lcd.setCursor(0,1);
           for (int i=80; i<100; i++)
             lcd.print(tweet[i]);
           lcd.setCursor(0,2);
           for (int i=100; i<120; i++)
             lcd.print(tweet[i]);
           lcd.setCursor(0,3);
           for (int i=120; i<140; i++)
             lcd.print(tweet[i]);
           delay(10000);
           }
     } 
    else
      lcd.println("Could not find item field"); 
  }
  else {
    lcd.println("Disconnected"); 
  }
  client.stop();
  client.flush(); 
  delay(60000); // wait a minute before next update
}
