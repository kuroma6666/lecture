#include <LiquidCrystal_I2C.h>
//#include <WiFi.h>
#include <time.h>
#define USE_ARDUINO_INTERRUPTS true
//#include <PulseSensorPlayground.h>


 

#define JST 3600*9

 

//const char*ssid = "SSID";
//const char*password = "pass";

 

//struct tm timeInfo;
//char s1[20];

 


LiquidCrystal_I2C lcd(0x27,16,2);

 

char groupname[] = {0xCA, 0xDF, 0xB2, 0xC5, 0xAF, 0xCC, 0xDF, 0xD9, 0xBD, 0xDE, '\0'};
char keisoku[] = {0xB9, 0xB2, 0xBF, 0xB8, 0xC1, 0xAD, 0xB3, 0xA1, 0xA1, 0xA1, '\0'};
char jikoku[] = {0xCB, 0xC6, 0xC1, 0x20, 0x2F, 0x20, 0xBC, 0xDE, 0xBA, 0xB8, '\0'};
char ondotani[] = {0xDF, 0x43, '\0'};
char sokutehuno[] = {0xBF, 0xB8, 0xC3, 0xB2, 0xCC, 0xC9, 0xB3, '\0'};

 


#define LED_PIN 5
#define Temp_PIN 34
#define BodyTemp_PIN 33
//#define Puls_PIN 32
#define Switch_PIN 35

 

/*気温測定*/
float getTemp(){
   float Vo, Temp;
   int ans = analogRead(34);
   Vo = ans * 3300 / 4095.0 + 182; //"182"はESP32のADコンバータの電圧変換誤差補正
   Temp = (Vo - 600)/ 10;
   return Temp;
}

 

/*体温測定*/
float getBodyTemp(){
 const float R = 10000.0;
  const float R0 = 10000.0;
  const float T0 = 25.0;
  const float B = 3435.0;
  const float v0 = 3.3 / 4095.0;

 

 
  float v, RR, BodyTemp;
  v = analogRead(33)*v0 + 0.182;//"182"はESP32のADコンバータの電圧変換誤差補正
  RR = (3.3-v)/(v/R);
  BodyTemp = 1.0/(((1.0/B)*log(RR/R0))+(1.0/(T0+273.15)))-273.15 + 4.65;
  /*↑の4.65によって外部接触で体温を補正*/
  /*市販の体温計とサーミスタ抵抗によるオフセットなしver.で数回比較し、平均値をとって
  差の4.65を導出*/
 
  return BodyTemp;
}

 

//BPM測定
/*
float getBodyBPM(){
  int BPM = pulseSensor.getBeatsPerMinute();
  if(pulseSensor.sawStartOfBeat()){
    
    return BPM;
    
  }
}
*/
 


 

 

void setup() {
  
  //WiFi.begin(ssid, password);
  
  
  pinMode(LED_PIN, OUTPUT);  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(6, 0);
  lcd.print("Gr.2");
  lcd.setCursor(3, 1);
  lcd.print(groupname);
  delay(2000);
  lcd.clear();

 

/*
   WiFi.mode(WIFI_STA);
   WiFi.disconnect();
   if(WiFi.begin(ssid, password) != WL_DISCONNECTED) {
    ESP.restart();
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println("Connected Success!");
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
  */ 

 

  /*脈拍setup
  float Threshold = 2047.5;
  pulseSensor.analogInput(Puls_PIN);
  pulseSensor.blinkOnPulse(LED_PIN);
  pulseSensor.setThreshold(Threshold);
   */
  
  
}

 


int loopcount = 0;  //lcd.clear();を一度だけ実行するため。詳細は↓

 

void loop() {
  
  int sw = analogRead(Switch_PIN);//switchのON,OFF判定

 

  if(sw>=4000){     //switchが押される
/*計測中*/
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print(keisoku);
      delay(2000);
      
    
      /*気温表示*/
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp:");
      lcd.setCursor(5, 0);
      lcd.print(getTemp());
      lcd.setCursor(10, 0);
      lcd.print(ondotani);

 

      /*体温表示*/
      lcd.setCursor(0, 1);
      lcd.print("Body:");
      lcd.setCursor(5, 1);

 

 /*体温の値は、体温に合わせるために4.65℃増やしているため、
サーミスタを接触させていない時は表示させない。*/
      float Judge = getBodyTemp();  //体温の取得
      if(Judge >= 35.50){           //サーミスタ抵抗が接触しているか判定
        lcd.print(getBodyTemp());
        lcd.setCursor(10, 1);
        lcd.print(ondotani);
      }else{
        lcd.print(sokutehuno);
      }
           
      delay(5000);
      lcd.clear();

 

      lcd.setCursor(0, 0);
      lcd.print("BPM:");
      lcd.setCursor(4, 0);
      lcd.print("72(sample)");
      delay(4500);

 


      lcd.clear();
  }else{
    
     //LCDによる時刻表示スケッチ
  /*時刻loop
    getLocalTime(&timeInfo);
    sprintf(s1, " %02d/%02d %02d:%02d:%02d",
          timeInfo.tm_mon + 1, timeInfo.tm_mday,
          timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
   */
    lcd.setCursor(3, 0);
    lcd.print(jikoku);
    lcd.setCursor(1, 1);
    //lcd.print(s1); 
    lcd.print("mm/dd hh:mm:ss" ) ;      
  }
   
}
