// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306.h"


#define OLED_I2C_ADDRESS 0x3C
#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);


#if (SSD1306_LCDHEIGHT != 64)
#error("Yukseklik hatali duzenleyin, Adafruit_SSD1306.h!");
#endif

void setup()   {
    
 
    
    Serial.begin(9600);
    
  
    display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS);  // 
    // init done
    display.clearDisplay();   //ekranı temizle
    display.display();
    
    // cloud functions: up to 4 per device.
    // function name: up to 12 characters
    Spark.function("SetReminder", SetReminder);
    Spark.function("SetWeather", SetWeather);
    Spark.function("SetSurf", SetSurf);
    Spark.function("Clear", Clear);
}


void loop() {
  
}

void showMsg(int n, String message) {
    
    int fontSize = (message.length() > 10 ? 1 : 2);
    
    display.setTextSize(fontSize);   // 1 = 8 piksel uzunluk, 2 = 16 pixel uzunluk
    display.setTextColor(WHITE);
    display.setCursor(0, n*16);
    display.println(message);
    display.display();
}

void ClearMessage(int n) {
    display.fillRect(0, 16*n, 128, 16, 0);
    display.display();
}

int SetReminder(String message) {
    ClearMessage(0);
    if (message.length() > 0)
        showMsg(0, message);
}

int SetWeather(String message) {
    ClearMessage(2);
    if (message.length() > 0)
        showMsg(2, message);
}

int SetSurf(String message) {
    ClearMessage(3);
    if (message.length() > 0)
        showMsg(3, message);
}

int Clear(String message) {
    display.clearDisplay();
    display.display();
}


