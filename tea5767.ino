/*
   https://youtu.be/sNjPl--rgiA
*/

#include <TEA5767N.h>  //https://github.com/mroger/TEA5767
TEA5767N radio = TEA5767N();

#include <LiquidCrystal_I2C.h>       //I²C li LCD için gerekli...
LiquidCrystal_I2C lcd(0x3F, 20, 4);  //Kullandığımız LCD I²C adresi ve karakter satır sayısı
#include <Wire.h>                    //I²C için gerekli...
#include <EEPROM.h>;
float memo;
bool audioFlag = 0;
float stn[] = { 88.01, 88.51, 89.61, 89.81, 90.51, 91.21, 97.21, 97.71, 99.51, 101.81, 102.51 };
String s_name[] = { "TRT RADYO 3", "KORDELYA FM", "SHOW RADYO", "TRT RADYO", "NUMBER 1 FM", "TRT FM", "METRO FM", "JOY FM", "FENOMEN", "VIRGIN", "Radyo Trafik" };
int i;
int sayStn = *(&stn + 1) - stn;

void setup() {
  pinMode(9, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  lcd.begin();  //LCD başlasın
  lcd.clear();  // ve ekran temizlensin
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print("FM RADYO TEA5767");
  lcd.setCursor(0, 1);
  lcd.print("Dr.TRonikYouTube");
  delay(3000);
  //memo = 88.01;  //İlk olarak yorumsuz yüklenip hafızaya alınıp, sonra yorumlu yüklenebilir.
  //EEPROM.put(0, memo);  //İlk olarak yorumsuz yüklenip hafızaya alınıp, sonra yorumlu yüklenebilir.
  EEPROM.get(0, memo);
  delay(500);  
  lcd.clear();
  radio.selectFrequency(memo);
  // radio.setStereoNoiseCancellingOn();
  lcd.setCursor(3, 1);
  lcd.print(radio.readFrequencyInMHz(),1) + lcd.print("MHz. ") + lcd.print("%") + lcd.print(radio.getSignalLevel() * 100 / 15);
  lcd.setCursor(0, 0);
  lcd.print("MEM. FREQ:");
}

void loop() {

  if (digitalRead(9) == LOW) {
    lcd.clear();
    chngStation();
  }
  if (digitalRead(8) == LOW) {
    audioFlag = !audioFlag;
    if (audioFlag == 1) {
      radio.mute();
      lcd.setCursor(15, 0);
      lcd.print("m");
    } else {
      radio.turnTheSoundBackOn();
      lcd.setCursor(15, 0);
      lcd.print(" ");
    }
    delay(400);
  }
  if (digitalRead(7) == LOW) {
   memo = radio.readFrequencyInMHz();   
   EEPROM.put(0, memo);  
   delay(100);  
   digitalWrite(13, HIGH);}
  else digitalWrite(13, LOW);
}

void chngStation() {
  radio.mute();
  radio.selectFrequency(stn[i]); 
  lcd.clear();
  delay(100);
  lcd.setCursor(0, 0);
  lcd.print(s_name[i]);
  lcd.setCursor(0, 1);
  lcd.print("FM ") + lcd.print(radio.readFrequencyInMHz(), 1) + lcd.print("MHz");
  lcd.setCursor(13, 1);
  lcd.print("%") + lcd.print(radio.getSignalLevel() * 100 / 15);
  radio.turnTheSoundBackOn();
  i++;
  if (i == sayStn) i = 0;
}
