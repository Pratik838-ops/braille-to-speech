#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(10, 11); // RX, TX
DFRobotDFPlayerMini player;

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buttons
int b1 = 2, b2 = 3, b3 = 4, b4 = 5, b5 = 6, b6 = 7;

void setup() {
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(b4, INPUT_PULLUP);
  pinMode(b5, INPUT_PULLUP);
  pinMode(b6, INPUT_PULLUP);

  Serial.begin(9600);
  mySerial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Braille System");
  delay(2000);
  lcd.clear();

  if (!player.begin(mySerial)) {
    Serial.println("DFPlayer NOT detected");
    lcd.print("DF Error");
    while(true);
  } else {
    Serial.println("DFPlayer Ready");
    lcd.print("System Ready");
    delay(1500);
    lcd.clear();
  }

  player.volume(30);
}

void loop() {

  while (digitalRead(b1)==1 && digitalRead(b2)==1 &&
         digitalRead(b3)==1 && digitalRead(b4)==1 &&
         digitalRead(b5)==1 && digitalRead(b6)==1);

  int d1=1,d2=1,d3=1,d4=1,d5=1,d6=1;

  lcd.clear();
  lcd.print("Reading...");

  unsigned long t = millis();

  while(millis()-t < 5000){
    if(digitalRead(b1)==0) d1=0;
    if(digitalRead(b2)==0) d2=0;
    if(digitalRead(b3)==0) d3=0;
    if(digitalRead(b4)==0) d4=0;
    if(digitalRead(b5)==0) d5=0;
    if(digitalRead(b6)==0) d6=0;
  }

  int letter = 0;

  // Braille mapping
  if(d1==0 && d2==1 && d3==1 && d4==1 && d5==1 && d6==1) letter=1;
  else if(d1==0 && d2==0 && d3==1 && d4==1 && d5==1 && d6==1) letter=2;
  else if(d1==0 && d2==1 && d3==1 && d4==0 && d5==1 && d6==1) letter=3;
  else if(d1==0 && d2==1 && d3==1 && d4==0 && d5==0 && d6==1) letter=4;
  else if(d1==0 && d2==1 && d3==1 && d4==1 && d5==0 && d6==1) letter=5;
  else if(d1==0 && d2==0 && d3==1 && d4==0 && d5==1 && d6==1) letter=6;
  else if(d1==0 && d2==0 && d3==1 && d4==0 && d5==0 && d6==1) letter=7;
  else if(d1==0 && d2==0 && d3==1 && d4==1 && d5==0 && d6==1) letter=8;
  else if(d1==1 && d2==0 && d3==1 && d4==0 && d5==1 && d6==1) letter=9;
  else if(d1==1 && d2==0 && d3==1 && d4==0 && d5==0 && d6==1) letter=10;
  else if(d1==0 && d2==1 && d3==0 && d4==1 && d5==1 && d6==1) letter=11;
  else if(d1==0 && d2==0 && d3==0 && d4==1 && d5==1 && d6==1) letter=12;
  else if(d1==0 && d2==1 && d3==0 && d4==0 && d5==1 && d6==1) letter=13;
  else if(d1==0 && d2==1 && d3==0 && d4==0 && d5==0 && d6==1) letter=14;
  else if(d1==0 && d2==1 && d3==0 && d4==1 && d5==0 && d6==1) letter=15;
  else if(d1==0 && d2==0 && d3==0 && d4==0 && d5==1 && d6==1) letter=16;
  else if(d1==0 && d2==0 && d3==0 && d4==0 && d5==0 && d6==1) letter=17;
  else if(d1==0 && d2==0 && d3==0 && d4==1 && d5==0 && d6==1) letter=18;
  else if(d1==1 && d2==0 && d3==0 && d4==0 && d5==1 && d6==1) letter=19;
  else if(d1==1 && d2==0 && d3==0 && d4==0 && d5==0 && d6==1) letter=20;
  else if(d1==0 && d2==1 && d3==0 && d4==1 && d5==1 && d6==0) letter=21;
  else if(d1==0 && d2==0 && d3==0 && d4==1 && d5==1 && d6==0) letter=22;
  else if(d1==1 && d2==0 && d3==1 && d4==0 && d5==0 && d6==0) letter=23;
  else if(d1==0 && d2==1 && d3==0 && d4==0 && d5==1 && d6==0) letter=24;
  else if(d1==0 && d2==1 && d3==0 && d4==0 && d5==0 && d6==0) letter=25;
  else if(d1==0 && d2==1 && d3==0 && d4==1 && d5==0 && d6==0) letter=26;

  int displayLetter = letter; // ✅ keep original

  // shift ONLY for audio
  if(letter == 1) letter = 26;
  else if(letter != 0) letter = letter - 1;

  lcd.clear();

  if(displayLetter != 0){

    char ch = (char)(displayLetter + 64);

    lcd.setCursor(0,0);
    lcd.print("Letter:");
    lcd.setCursor(0,1);
    lcd.print(ch);

    Serial.print("Letter: ");
    Serial.print(ch);
    Serial.print(" | File: 00");
    Serial.print(letter);
    Serial.println(".mp3");

    player.play(letter);

  } else {
    lcd.print("Invalid Input");
    Serial.println("Invalid Input");
  }

  delay(3000);
}