#include <RTClib.h>
#include <Wire.h>
#include "U8glib.h"
#include <Servo.h>

const int tempPin = A3;


RTC_DS3231 rtc;
Servo myservo;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

void setup() {
  myservo.attach(8);

  Serial.begin(9600);
  Wire.begin();
  rtc.begin();

  u8g.setFont(u8g_font_unifont);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  for (int i = 5; i <= 7; i++)
  pinMode(i, OUTPUT);
}

void loop() {
  myservo.write(abithot());
  Serial.println(abithot());
  mightbehot();


}

float getTemp() {
  int Vo;
  float R1 = 10000;  // value of R1 on board
  float logR2;
  float R2;
  float T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;  // Steinhart-Hart coefficients for thermistor

  Vo = analogRead(tempPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);  // calculate resistance on thermistor
  logR2 = log(R2);
  T = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));  // temperature in Kelvin
  T = T - 273.15;                                              // convert Kelvin to Celsius
  return T;
}

String getTime() {
  DateTime now = rtc.now();
  return "Time: " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

void mightbehot(){
    
  if (getTemp() > 26) {
    oledWrite(25, 30, "TOOOOOOO HOT INNIT", 25, 40, "RAAAAAAAAH");
    alarm();

  } else {
   oledWrite(0, 30, (getTime()), 0, 50, "Temp: " + String(getTemp()));
  }
 
}

void alarm(){
  
  for (int i = 5; i <= 7; i++){
  digitalWrite(i, LOW);
  delay(100);
  digitalWrite(i, HIGH);
  }
}

void oledWrite(int x, int y, String text, int x2, int y2, String text2) {
  u8g.firstPage();
  do {
    u8g.drawStr(x, y, text.c_str());
    u8g.drawStr(x2, y2, text2.c_str());
  } while (u8g.nextPage());
}

int abithot() {
  return (map(getTemp(), 18, 30, 0, 179));
}