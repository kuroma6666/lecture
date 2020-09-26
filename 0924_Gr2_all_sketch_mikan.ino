#include <WiFi.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#define USE_ARDUINO_INTERRUPTS true   
#include <PulseSensorPlayground.h>     

char groupname[] = {0xCA, 0xDF, 0xB2, 0xC5, 0xAF, 0xCC, 0xDF, 0xD9, 0xBD, 0xDE, '\0'};
char keisoku[] = {0xB9, 0xB2, 0xBF, 0xB8, 0xC1, 0xAD, 0xB3};

int LEDSW = 35;
int outtempsensor=34;

const int PulseWire = 32;      
const int LED13 = 13;         
int Threshold = 545;    
PulseSensorPlayground pulseSensor;    

//const char *ssid = "oneKosen";
//const char *password = "e=BJ8m~E`^HZ?Px";
const char *ssid = "aterm-5767f8_2.4GHz";
const char *password = "776290e87876f";
struct tm timeInfo;
char s1[20];

float getBodyTemp(){
  const float R = 10000.0;
  const float R0 = 10000.0;
  const float T0 = 25.0;
  const float B = 3435.0;
  const float v0 = 0.0048828125F;

  float v = (float)analogRead(34)*v0;
  float RR = (5-v)/(v/R);
  float BodyTemp = 1.0/(((1.0/B)*log(RR/R0))+(1.0/(T0+273.15)))-273.15;

  return BodyTemp;
  }

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Gr2");
  lcd.setCursor(0, 1);
  lcd.print(groupname);
  delay(2000);
  pinMode( LEDSW, INPUT );

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  if (WiFi.begin(ssid, password) != WL_DISCONNECTED) {
    ESP.restart();
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println("Connected Success!");
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
}


void loop() {
  int value;
  value = digitalRead(LEDSW);
  
  getLocalTime(&timeInfo);
  sprintf(s1, " %02d/%02d %02d:%02d:%02d",
          timeInfo.tm_mon + 1, timeInfo.tm_mday,
          timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
          
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(s1);

  delay(100);
  
  if (value == 1) {
    delay(5);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("sokutei now");
    delay(2000);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("hoge");
    lcd.setCursor(0, 1);
    lcd.print("BT");
    
    delay(5000);
    
  }
}
