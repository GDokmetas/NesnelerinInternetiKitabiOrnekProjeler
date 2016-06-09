int bars [] = {D0, D1, D2, D3, D4, D5, D6, D7, A0, A1};
//RX ayağını hoparlör ayağı olarak belirt. 
int speakerPin = RX;

void setup() {

    //RGB ledin kontrolünü ele al. 
  // RGB led öylesine yanmayacak
  // Hata mesajında yanacak. 
    RGB.control(true);

    pinMode(D7, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D0, OUTPUT);
    
    pinMode(A1, OUTPUT);
    pinMode(A0, OUTPUT);
}

bool error = false;
bool errorLightOn = false;
int errorCode;

int lastStrength;

void loop() {
   
    if(error){
        if(!errorLightOn){
            //Wifi çip hatasındaki hata kodu 
            if(errorCode == 1)
                RGB.color(255, 0, 0);
            //Zaman aşımındaki hata kodu
            if(errorCode == 2)
                RGB.color(0, 0, 255);
            errorLightOn = true;
        }else{
            RGB.color(0, 0, 0);        
            errorLightOn = false;
        }
        delay(100);
    }else{
        RGB.color(0, 0, 0);
    }
        
    //Gücü al 
  // WiFi.RSSI fonksiyonu bu işi yapar. 
    int strength = WiFi.RSSI();
    
    //-127 -1 arasındaki wi-fi güç değerini 
  // bar için uygun değere dönüştürür. 
    int color = (((strength - -127) * 10) / (-1 - -127));
        
    //Hata denetimi
    if(strength > 0){
        // 1 = WiFi çip hatası
        // 2 = Zaman aşımı
        if(strength == 1 || strength ==2){
            error = true;
            errorCode = strength;
            return;
        }else{

            error = false;
            
            for (int i = 0; i < 10; i++ )
                digitalWrite(bars[i], HIGH);
        }
    }else{
        error = false;
        
        //Güç durum barını yak ve söndür
        for (int i = 0; i < 10; i++ ){
            if(i < color)
                digitalWrite(bars[i], HIGH);
            else
                digitalWrite(bars[i], LOW);
                
            //Tone fonksiyonu ile hoparlörden bir ses çıkar. 
      // Buradaki parametreleri değiştirmekle değişik 
      // sesler elde edilebilir. Şimdiki durumda 
      // 1908hz , 125ms bir sinyal elde edilir 
      //Bu durum sinyal seviyesinin değişmesiyle 
      // tekrarlanır. 
            if(color != lastStrength)
                 tone(speakerPin, 1908, 125);    
            lastStrength = color;
        }
    }
}

