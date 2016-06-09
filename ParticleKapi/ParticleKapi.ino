const int servoAyak = D0;  // D0 servo sinyal ayağı SARI KABLO

const int ic_reed = D2;  // D2 İçeri REED anahtarı
const int dis_reed = D3; // D3 Dışarı REED anahtarı

const int kapama_anahtari = D6;  // D5 Kapı kapatma düğmesi
const int acma_anahtari = D4;  // D4 Kapı açma düğmesi

const int reed_led = D5;    // Reed durum ledi
const int kapi_led = D7;    // Kapı durum ledi

int acma_zamani  = 6*60;   // Kapının açık olacağı zaman Sabah 6
int kapama_zamani = 13*60;  // Kapının kapanacağı zaman gece 1
// Siz bu zamanı ihtiyaca göre değiştirebilirsiniz
int timeZoneOffset = 3;  // Istanbul Saati

int ndis_reed = 0;  
int nkapi_ac = 0;  // 0=automatic zaman ayarlı;1=manual açma
bool bOverride = false;
bool bCatWasIn = true;
bool bOkToPublish = false;  //Başlangıçta herhangi birşey yollama

Servo myservo;  // Servo nesnesini tanımla

void ServoPosition(int pos)
{
   
    myservo.write(pos);                 // Açı değerini gir ve döndür
                        
}

void kapiyiac()
{
    ServoPosition(90);
    digitalWrite(kapi_led, LOW);  // Led kapalıysa kapı açıktır
    if (bOkToPublish)
        Spark.publish("KapiAcik");    // BULUT sisteme KapiAcik verisini yolla
        // Bu veriye göre işleme yapılacak. 
}

void kapiyikapat()
{
    ServoPosition(0);   // 0 açısına döndür
    digitalWrite(kapi_led, HIGH);  // Led açıksa kapı kapalıdır
    if (bOkToPublish)
        Spark.publish("KapiKapali");  // Bulut sisteme KapiKapali bilgisini yolla
}

bool doorIsCurrentlyClosed()
{
    return myservo.read() == 0; // Kapı zaten kapalıysa birşey yapma
}

// Kapı durumuna göre buluta veri yolla
int SetDoorState(String state)
{
    if (state == "OPEN") {
        nkapi_ac = 1;
        kapiyiac();
    }
    else {
        nkapi_ac = 2;
        kapiyikapat();
    }
    return 1;
}

void ShowCatIsIn()
{
    if (!bCatWasIn) {
        digitalWrite(reed_led, HIGH);  // Led açık ise hayvan içeridedir.
       
        if (bOkToPublish)
            Spark.publish("CatIsIn");   // Veriyi Yolla. Kedi içerde. 
        delay(1000);
    }
    bCatWasIn = true;
}

void ShowCatIsOut()
{
    if (bCatWasIn) {
        digitalWrite(reed_led, LOW);  // Led kapaliysa hayvan dışarıda
        if (bOkToPublish)
            Spark.publish("CatIsOut");  //Veriyi yolla. Hayvan dışarıda
        delay(1000);
    }
    bCatWasIn = false; 
}

// kesme servis rutini
void onCatIsIn()
{
    ndis_reed = 0;  // buluttan değere bakabilirsiniz.
}

// kesme servis rutini
void onCatIsOut()
{
    ndis_reed = 1;  
}

void onOpenDoor()
{
    nkapi_ac = 1;
}

void onCloseDoor()
{
    nkapi_ac = 2;
}

// Gerçek zamanlı saat buluttan veri alir ve güncellenir
int getTime_Minute()
{
    return (Time.hour() + timeZoneOffset)%24 * 60 + Time.minute();    
}

// Kışın kapıyı 1 saat geç aç (opsiyonel)
int getExtraTime_Minute(int x)
{
    return abs(((((x + 10)%365) -182) * 60) / 182);
}

bool okToGoOut()
{
    int currentTime = getTime_Minute();
        
 
    int dayOfTheYear = (Time.month() - 1)*30.5 + Time.day();
   
    int extra = getExtraTime_Minute(dayOfTheYear);  // open later in the winter
    
    if (currentTime == (acma_zamani + extra) || (currentTime == kapama_zamani))
        nkapi_ac = 0; 
        
    return (currentTime >= (acma_zamani + extra)) && (currentTime < kapama_zamani);
}

void setupPins()
{
    // Ayakları ayarla
    pinMode(ic_reed, INPUT_PULLUP);
    pinMode(dis_reed, INPUT_PULLUP); 
    pinMode(kapama_anahtari, INPUT_PULLUP); 
    pinMode(acma_anahtari, INPUT_PULLUP); 
    // Ledler çıkış olmalı
    pinMode(kapi_led, OUTPUT);
    pinMode(reed_led, OUTPUT);
}

void setupInterrupts()
{
    // Kesmeleri ayarla
    attachInterrupt(ic_reed, onCatIsIn, FALLING);
    attachInterrupt(dis_reed, onCatIsOut, FALLING);
    attachInterrupt(kapama_anahtari, onCloseDoor, FALLING);
    attachInterrupt(acma_anahtari, onOpenDoor, FALLING);  
}

// Bütün ayakları kesmeleri ve servoyu tanımla
void setup()
{
    nkapi_ac = 0;    // Otomatik zaman modu
    ndis_reed = 0; 
    bCatWasIn = true;   //önceki durumu gözle

    setupPins();
    setupInterrupts();
    myservo.attach(servoAyak);  // D0'a servoyu bağla
    
    
    // Bulut değerlerini tanımla
    Spark.variable("bCatIsOut", &ndis_reed, INT);
    // Bulut fonksiyonları
    Spark.function("SetDoorState", SetDoorState);
    
    // Eğer kapılar açılmalıysa denetle
    if (okToGoOut())
        kapiyiac();
    else
        kapiyikapat();
    
    bOkToPublish = true;
}


void loop()
{
    if (ndis_reed == 1)
        ShowCatIsOut();
    else
        ShowCatIsIn();

    bool oktogoout = okToGoOut();
    // Kapı açılmalıysa denetle
    if ( (nkapi_ac == 1 || ( oktogoout && nkapi_ac == 0 )) && doorIsCurrentlyClosed())
            kapiyiac();

    // Kapı açılmalıysa denetle
    if ( (nkapi_ac == 2 || ( !oktogoout && nkapi_ac == 0 )) && !doorIsCurrentlyClosed())
            kapiyikapat();

}
