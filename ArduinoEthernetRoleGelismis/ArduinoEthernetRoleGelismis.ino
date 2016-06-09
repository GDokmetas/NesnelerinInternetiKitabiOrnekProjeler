//Ethernet Switch

#include <Ethernet.h>
#include <SPI.h>


////////////////////////////////////////////////////////////////////////
//Ayar kısmı
////////////////////////////////////////////////////////////////////////
 
  //Manuel IP Ayarları
  byte ip[] = { 192, 168, 1, 177 };   
  byte gateway[] = { 192, 168, 1, 254 }; 
  byte subnet[] = { 255, 255, 255, 0 }; 

  // MAC Adresi tanımla
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

  EthernetServer server = EthernetServer(80); // Standart port 80 den yürüt
 
  //Çıkış sayısını ayarla. Bu uygulamada 8 çıkış var. 8 kanal röle kullanılabilir.
  int outputQuantity = 8;  //10'dan fazla olmamalı
 
  //Hangi ayaktan başlanacaksa onu kararlaştır 2-9 arası
  int outputLowest = 2;    
////////////////////////////////////////////////////////////////////////

  // Değişken tanımları
  int outp = 0;
  boolean printLastCommandOnce = false;
  boolean printButtonMenuOnce = false;
  boolean initialPrint = true;
  String allOn = "";
  String allOff = "";
  boolean reading = false;
  boolean readInput[10]; // En fazla giriş için bir dizi tanımla.

//Setup kısmını başlat.
void setup(){
  Serial.begin(9600);

  //10,11,12 ve 13. bacaklar ethernet için kullanılacak
  //Kontrol ayaklarını çıkış olarak döngüyle ayarla.
  for (int var = outputLowest; var < outputLowest + outputQuantity; var++)  {
            pinMode(var, OUTPUT);
        }

  //Setting up the IP address. Comment out the one you dont need.
  //Ethernet.begin(mac); DHCP için bu komutu yazmanız gereklidir. Program manüel olarak çalıştırılacak.
  // O yüzden bütün parametreleri yazıyoruz. 
  Ethernet.begin(mac, ip, gateway, subnet); 


  server.begin();
  Serial.println(Ethernet.localIP());
}


void loop(){

  // Gelen istemcileri dinle
  checkForClient();
}


void checkForClient(){

  EthernetClient client = server.available();

  if (client) {

    // HTTP isteği boş satır ile bitecektir.
    boolean currentLineIsBlank = true;
    boolean sentHeader = false;

    while (client.connected()) {
      if (client.available()) {

        if(!sentHeader){
         // Standart HTTP cevap başlığını yolla
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<head>");
         
          // Sayfa başlığını Ekle
          client.println("<title>Ethernet Role Uygulaması</title>");
          client.println("<meta name=\"description\" content=\"Ethernet Switching\"/>");

          // Sayfa yenileme aralığını kararlaştır. 5 saniyede bir yenilenir
          client.println("<meta http-equiv=\"refresh\" content=\"10; url=/\">");
         
          // Diğer tarayıcı ayarları. 
          client.println("<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">");
          client.println("<meta name=\"apple-mobile-web-app-status-bar-style\" content=\"default\">");
          client.println("<meta name=\"viewport\" content=\"width=device-width, user-scalable=no\"/>");         
         
          //CSS stil dosyaları
          client.println("<style type=\"text/css\">");
          client.println("");
         
          //Sayfanın grafik olarak nasıl görüleceğini kararlaştır.
          client.println("html { height:100%; }"); 
 
          client.println("  body {");
          client.println("    height: 100%;");
          client.println("    margin: 0;");
          client.println("    font-family: helvetica, sans-serif;");
          client.println("    -webkit-text-size-adjust: none;");
          client.println("   }");
          client.println("");
          client.println("body {");
          client.println("    -webkit-background-size: 100% 21px;");
          client.println("    background-color: #c5ccd3;");
          client.println("    background-image:");
          client.println("    -webkit-gradient(linear, left top, right top,");
          client.println("    color-stop(.75, transparent),");
          client.println("    color-stop(.75, rgba(255,255,255,.1)) );");
          client.println("    -webkit-background-size: 7px;");
          client.println("   }");
          client.println("");
          client.println(".view {");
          client.println("    min-height: 100%;");
          client.println("    overflow: auto;");
          client.println("   }");
          client.println("");
          client.println(".header-wrapper {");
          client.println("    height: 44px;");
          client.println("    font-weight: bold;");
          client.println("    text-shadow: rgba(0,0,0,0.7) 0 -1px 0;");
          client.println("    border-top: solid 1px rgba(255,255,255,0.6);");
          client.println("    border-bottom: solid 1px rgba(0,0,0,0.6);");
          client.println("    color: #fff;");
          client.println("    background-color: #8195af;");
          client.println("    background-image:");
          client.println("    -webkit-gradient(linear, left top, left bottom,");
          client.println("    from(rgba(255,255,255,.4)),");
          client.println("    to(rgba(255,255,255,.05)) ),");
          client.println("    -webkit-gradient(linear, left top, left bottom,");
          client.println("    from(transparent),");
          client.println("    to(rgba(0,0,64,.1)) );");
          client.println("    background-repeat: no-repeat;");
          client.println("    background-position: top left, bottom left;");
          client.println("    -webkit-background-size: 100% 21px, 100% 22px;");
          client.println("    -webkit-box-sizing: border-box;");
          client.println("   }");
          client.println("");
          client.println(".header-wrapper h1 {");
          client.println("    text-align: center;");
          client.println("    font-size: 20px;");
          client.println("    line-height: 44px;");
          client.println("    margin: 0;");
          client.println("   }");
          client.println("");
          client.println(".group-wrapper {");
          client.println("    margin: 9px;");
          client.println("    }");
          client.println("");
          client.println(".group-wrapper h2 {");
          client.println("    color: #4c566c;");
          client.println("    font-size: 17px;");
          client.println("    line-height: 0.8;");
          client.println("    font-weight: bold;");
          client.println("    text-shadow: #fff 0 1px 0;");
          client.println("    margin: 20px 10px 12px;");
          client.println("   }");
          client.println("");
          client.println(".group-wrapper h3 {");
          client.println("    color: #4c566c;");
          client.println("    font-size: 12px;");
          client.println("    line-height: 1;");
          client.println("    font-weight: bold;");
          client.println("    text-shadow: #fff 0 1px 0;");
          client.println("    margin: 20px 10px 12px;");
          client.println("   }");
          client.println("");
          client.println(".group-wrapper table {");
          client.println("    background-color: #fff;");
          client.println("    -webkit-border-radius: 10px;");
         
          client.println("    -moz-border-radius: 10px;");
          client.println("    -khtml-border-radius: 10px;");
          client.println("    border-radius: 10px;");
         
          client.println("    font-size: 17px;");
          client.println("    line-height: 20px;");
          client.println("    margin: 9px 0 20px;");
          client.println("    border: solid 1px #a9abae;");
          client.println("    padding: 11px 3px 12px 3px;");
          client.println("    margin-left:auto;");
          client.println("    margin-right:auto;");
         
          client.println("    -moz-transform :scale(1);"); //Firefox için kod
          client.println("    -moz-transform-origin: 0 0;");


         
          client.println("   }");
          client.println("");


          //Led yanınca yeşil göster. Bunun için komutlar. 
          client.println(".green-circle {");
          client.println("    display: block;");
          client.println("    height: 23px;");
          client.println("    width: 23px;");
          client.println("    background-color: #0f0;");
        //client.println("    background-color: rgba(60, 132, 198, 0.8);");
          client.println("    -moz-border-radius: 11px;");
          client.println("    -webkit-border-radius: 11px;");
          client.println("    -khtml-border-radius: 11px;");
          client.println("    border-radius: 11px;");
          client.println("    margin-left: 1px;");

          client.println("    background-image: -webkit-gradient(linear, 0% 0%, 0% 90%, from(rgba(46, 184, 0, 0.8)), to(rgba(148, 255, 112, .9)));@");
          client.println("    border: 2px solid #ccc;");
          client.println("    -webkit-box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px;");
          client.println("    -moz-box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px; /* FF 3.5+ */");
          client.println("    box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px; /* FF 3.5+ */");
         
          client.println("    }");
          client.println("");
         
          //Led kapalıyken gösterilecek ikon
          client.println(".black-circle {");
          client.println("    display: block;");
          client.println("    height: 23px;");
          client.println("    width: 23px;");
          client.println("    background-color: #040;");
          client.println("    -moz-border-radius: 11px;");
          client.println("    -webkit-border-radius: 11px;");
          client.println("    -khtml-border-radius: 11px;");
          client.println("    border-radius: 11px;");
          client.println("    margin-left: 1px;");
          client.println("    -webkit-box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px;");
          client.println("    -moz-box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px; /* FF 3.5+ */");
          client.println("    box-shadow: rgba(11, 140, 27, 0.5) 0px 10px 16px; /* FF 3.5+ */");
          client.println("    }");
          client.println("");
         
          //Led için parlaklık efekti ekle
          client.println("   .glare {");
          client.println("      position: relative;");
          client.println("      top: 1;");
          client.println("      left: 5px;");
          client.println("      -webkit-border-radius: 10px;");
          client.println("      -moz-border-radius: 10px;");
          client.println("      -khtml-border-radius: 10px;");
          client.println("      border-radius: 10px;");
          client.println("      height: 1px;");
          client.println("      width: 13px;");
          client.println("      padding: 5px 0;");
          client.println("      background-color: rgba(200, 200, 200, 0.25);");
          client.println("      background-image: -webkit-gradient(linear, 0% 0%, 0% 95%, from(rgba(255, 255, 255, 0.7)), to(rgba(255, 255, 255, 0)));");
          client.println("    }");
          client.println("");
         
         
          //Şimdi stil başlığının sonu
          client.println("</style>");
          client.println("</head>");
         
          //Sayfanın kendisini oluştur.
          client.println("<body>");
          client.println("<div class=\"view\">");
          client.println("    <div class=\"header-wrapper\">");
          client.println("      <h1>Ethernet Role Uygulamasi</h1>");
          client.println("    </div>");
          client.println("<div  class=\"group-wrapper\">");
          client.println("    <h2>Tuslara Basarak Denetleyin.</h2>");
          client.println();
         
         
          sentHeader = true;
        }
       
        char c = client.read();

        if(reading && c == ' '){
          reading = false;
          }
      
//       Serial.print(c);
       
       
        if(c == '?') {
          reading = true; //? işaretini bul. 
        }
    

        if(reading){
            if(c == 'H') {outp = 1;}
            if(c == 'L') {outp = 0;}
          Serial.print(c);   //C değerini seri ekrandan yazdır. (Opsiyonel)
          //Serial.print(outp);
          //Serial.print('\n');
        
           switch (c) {
            case '2':
              //2 kodu için
              triggerPin(2, client, outp);
              break;
            case '3':
            //3 kodu için
              triggerPin(3, client, outp);
              break;
            case '4':
            //4 kodu için
              triggerPin(4, client, outp);
              break;
            case '5':
            //5 kodu için
              triggerPin(5, client, outp);
              //printHtml(client);
              break;
            case '6':
            //6 kodu için
              triggerPin(6, client, outp);
              break;
            case '7':
            //7 kodu için
              triggerPin(7, client, outp);
              break;
            case '8':
            //8 kodu için
              triggerPin(8, client, outp);
              break;
            case '9':
            //9 kodu için
              triggerPin(9, client, outp);
              break;
          }

        }

        if (c == '\n' && currentLineIsBlank){
          printLastCommandOnce = true;
          printButtonMenuOnce = true;
          triggerPin(777, client, outp); 
          break;
        }
      }
    }
   
    //Çıkmadan önce değerleri ayarla
    printLastCommandOnce = false;
    printButtonMenuOnce = false;
   
    allOn = "";
    allOff = "";
    client.println("\n<h3 align=\"center\"> <br> </h3>");
    client.println("</div>\n</div>\n</body>\n</html>");
   
    delay(1); // Tarayıcının veriyi alması için süre tanı
    client.stop(); // bağlantıyı kes

  }

}



void triggerPin(int pin, EthernetClient client, int outp){
//Çıkışları sıfırla.

  //Çıkışları ayarla.
    if (pin != 777){
        if(outp == 1) {
          digitalWrite(pin, HIGH);
         }
        if(outp == 0){
          digitalWrite(pin, LOW);
         }
    }
  //Okunan değerleri yenile
  readOutputStatuses();
 
 
  //Düğmeleri yazdır
          if (printButtonMenuOnce == true){
              printHtmlButtons(client);
               printButtonMenuOnce = false;
          }
        
}


//HTML düğmeleri yazdır.
void printHtmlButtons(EthernetClient client){
    
     //Tablo oluştur. 
     client.println("");
     //client.println("<p>");
     client.println("<FORM>");
     client.println("<table border=\"0\" align=\"center\">");
    
     //Düğme düğme yazdır.
     for (int var = outputLowest; var < outputLowest + outputQuantity; var++)  {     
             
              //kodları ayarla
              allOn += "H";
              allOn += var;
              allOff += "L";
              allOff += var;
             
             
              //Yazdırmaya satırdan itibaren başla
              client.print("<tr>\n");       
             
              //Prints the ON Buttons
              client.print(" <td><INPUT TYPE=\"button\" VALUE=\"AC - Pin  ");
              client.print(var);
              client.print("\" onClick=\"parent.location='/?H");
              client.print(var);
              client.print("'\"></td>\n");
             
              //Prints the OFF Buttons
              client.print(" <td><INPUT TYPE=\"button\" VALUE=\"KAPA - Pin  ");
              client.print(var);
              client.print("\" onClick=\"parent.location='/?L");
              client.print(var);
              client.print("'\"></td>\n");
             
             
              //ledlerin ilk parçasını yazdır
              if (readInput[var] == true){
                client.print(" <td><div class='green-circle'><div class='glare'></div></div></td>\n");
              }else
              {
                client.print(" <td><div class='black-circle'><div class='glare'></div></div></td>\n");
              } 
             
             
              //satır sonu
              client.print("</tr>\n"); 
         }
        

              //On kısmını yazdır
              client.print("<tr>\n<td><INPUT TYPE=\"button\" VALUE=\"HEPSINI AC");
              client.print("\" onClick=\"parent.location='/?");
              client.print(allOn);
              client.print("'\"></td>\n");
            
              //OFF kısmını yazdır          
              client.print("<td><INPUT TYPE=\"button\" VALUE=\"HEPSINI KAPA");
              client.print("\" onClick=\"parent.location='/?");
              client.print(allOff);
              client.print("'\"></td>\n<td></td>\n</tr>\n");
             
              //tablo ve formu kapat
              client.println("</table>");
              client.println("</FORM>");
              //client.println("</p>");
   
    }

//Çıkış durumlarını oku
void readOutputStatuses(){
  for (int var = outputLowest; var < outputLowest + outputQuantity; var++)  {
            readInput[var] = digitalRead(var);
            //Serial.print(readInput[var]);
       }
 
} 
