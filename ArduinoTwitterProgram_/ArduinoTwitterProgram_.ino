
#include <EtherCard.h>

// OAUTH anahtarı. Kendinizinkini yazın.
#define TOKEN   " xx "

// Mac adresini tanımla
byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

const char website[] PROGMEM = "arduino-tweet.appspot.com";

static byte session;

byte Ethernet::buffer[700];
Stash stash;

static void sendToTwitter () {
  Serial.println("Tweet Yollaniyor...");
  byte sd = stash.create();

  const char tweet[] = "@elektrobilim Bu Twitter enc28j60 ile Arduino Kullanilarak Atildi";
  stash.print("token=");
  stash.print(TOKEN);
  stash.print("&status=");
  stash.println(tweet);
  stash.save();
  int stash_size = stash.size();


  Stash::prepare(PSTR("POST http://$F/update HTTP/1.0" "\r\n"
    "Host: $F" "\r\n"
    "Content-Length: $D" "\r\n"
    "\r\n"
    "$H"),
  website, website, stash_size, sd);


  session = ether.tcpSend();
}

void setup () {
  Serial.begin(57600);
  Serial.println("\n[Twitter Istemci]");

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println(F("Ethernet Baglantisi Yapilamadi"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP Hatasi"));

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

  if (!ether.dnsLookup(website))
    Serial.println(F("DNS Hatasi"));

  ether.printIp("SRV: ", ether.hisip);

  sendToTwitter();
}

void loop () {
  ether.packetLoop(ether.packetReceive());

  const char* reply = ether.tcpReply(session);
  if (reply != 0) {
    Serial.println("Cevap Alindi!");
    Serial.println(reply);
  }
}

