#include <SPI.h>
#include <Ethernet.h> // Kütüphaneleri Yükle
#include <Servo.h> 
Servo myservo;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,177);   // Ip ve Mac tanımla 


EthernetServer server(80); // Port iletimli numara. Neredeyse hep 80 olur. 

String readString; // 
int val;    // PIR değerini okuma için
int pir=5;

//////////////////////

void setup(){

  pinMode(2, OUTPUT);
  pinMode(pir, INPUT); // 3. ayak
  Ethernet.begin(mac, ip);
  myservo.attach(9); // Servo Başlat
  myservo.write(45);
  pinMode(3,OUTPUT);
  digitalWrite(3,LOW);
  pinMode(A3,OUTPUT);
  pinMode(A5,OUTPUT);
  digitalWrite(A3,HIGH);
  digitalWrite(A5,LOW);
}

void loop(){
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {                     // İstemci ayarları
        char c = client.read();                     
        if (readString.length() < 100) {
          readString += c;
        } 
        if (c == '\n') {
          Serial.println(readString);                      // HTML koduna başlıyoruz.
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<hmtl>");
client.println("<head>");
client.println("ARDUINO SAYFASI");
client.println("</head>");
client.println("<title>");
client.println("ARDUINO + ETHERNET SAYFASI");
client.println("</title>");
client.println("<body bgcolor=black>");
client.println("<font color=white>");
client.println("<meta http-equiv=\"refresh\" content=\"4\">"); // Hareket olup olmadığını kontrol için yenileme
client.println("<center>");                                      
client.println("<b>");
client.println("Arduino ile Akilli Ev Sistemine Hos Geldiniz!!");
client.println("</br>");
client.println("Asagidaki dugmeleri kullanarak cesitli gorevleri yapabilirsiniz");
client.println("</br>");
client.println("Servo veya led fark etmez. Sadece dugmelere basin. Iyi eglenceler.");
client.println("</b>");
client.println("<p>");
client.println("<table border=0 width=200>");
client.println("<tr>");
client.println("<td align=center>");
client.println("<font color=white>");
client.println("SICAKLIK:");
client.println("</td>");
client.println("</tr>");
client.println("<tr>");
client.println("<td align=center>");
client.println("<font color = turquoise size=10>");
int val = analogRead(A4);
float mv = ( val/1024.0)*5000;
float cel = mv/10;

client.print(cel);
client.println("* C");
client.println("</td>");
client.println("</tr>");
client.println("</table>");
client.println("<p>");
client.println("<FORM>");
client.println("<INPUT type=button value=LED1-ON onClick=window.location='/?lighton1\'>");
client.println("<INPUT type=button value=LED1-OFF onClick=window.location='/?lightoff1\'>");
client.println("</FORM>");   // 
client.println("<FORM>");    // 
client.println("<INPUT type=button value=Servo-0 onClick=window.location='/?serv0\'>");
client.println("<INPUT type=button value=Servo-45 onClick=window.location='/?serv45\'>");
client.println("<INPUT type=button value=Servo-90 onClick=window.location='/?serv90\'>");
client.println("</FORM>");
client.print("<table border=1 width=200>");
client.print("<tr>");
client.print("<td align=center>");
client.print("<font color=white size=3>");
client.print("Hareket Durumu");
client.print("</td>");
client.print("</tr>");
client.print("<tr>");
client.print("<td align=center>");
client.print("<font color=white size=3>");  // Hareketi Yazdir
val = digitalRead(pir);
          if (val == HIGH){
           client.print("HAREKET VAR");
          }
         else {
          client.print("HAREKET YOK");
         }
client.print("</td>");
client.print("</tr>");
client.print("</table>");
client.println("</center>");
client.println("</font>");
client.println("</body>");
client.println("</html>");
 
          delay(1);
      
          if(readString.indexOf("?lighton") >0)     //çeşitli komutlar.
          {
            digitalWrite(2, HIGH);
          }
          else{
          if(readString.indexOf("?lightoff") >0)
          {
            digitalWrite(2, LOW);
          }
          
          else{
            if(readString.indexOf("?serv0") >0)
            {
              myservo.write(0);
            }
          
          else{
            if(readString.indexOf("?serv45") >0)
            {
              myservo.write(45);
            }
          
          else{
            if(readString.indexOf("?serv90") >0)
            {
              myservo.write(90);
            }
          }
          }
          }
          }
          readString="";
          client.stop();            // End of session.

        }
      }
    }
  }

}
