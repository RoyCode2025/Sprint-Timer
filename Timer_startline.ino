
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);  
RF24 radio(9, 10);          
const byte address[10] = "ADDRESS01";

char txt1[] = "GO", txt2[] = "STOP";
char txt0[10];
int laserin = A0;           
int laserval;
int grnled = 3;             
int pwrled = 2;             
int resetd = A7;
int distval;             
int Start1 = 0, Stop1 = 0, Ready1 = 0;
unsigned long startTime = 0;  
unsigned long elapsedTime = 0;  
unsigned long lastMillis = 0; 

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("<SprintTimer>");
  
  pinMode(laserin, INPUT);
  pinMode(grnled, OUTPUT);
  pinMode(pwrled, OUTPUT);
  pinMode(resetd, INPUT);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  digitalWrite(pwrled, HIGH);
}

void loop() {
  laserval = analogRead(laserin);
  distval = analogRead(resetd);
  Serial.println(distval);

  if (laserval >= 40) {
    digitalWrite(grnled, HIGH);  
  } else {
    digitalWrite(grnled, LOW);   
  }

  if (radio.available()) {
    radio.read(&txt0, sizeof(txt0));
    Serial.println(txt0);
    
    if (laserval >= 40 && Ready1 == 0 && strcmp(txt0, txt1) == 0) {
      Start1 = 1;
      Ready1 = 1; 
      startTime = millis(); 
      lcd.setCursor(2, 1);
      lcd.print("READY");
    }

    if (Start1 == 1 && laserval <= 30) {
      lcd.setCursor(2, 1);
      lcd.print("START");
      Stop1 = 1;
      startTime = millis();
    }

    if (Stop1 == 1 && strcmp(txt0, txt2) == 0) {
      Stop1 = 0;
      Ready1 = 1;  
      lcd.setCursor(2, 1);
      lcd.print("STOP");
      elapsedTime = millis() - startTime; 
      displayTime(elapsedTime);
    }

    if (distval>=150) {
      resetTimer(); 
    }
  }
  delay(10);
}
void displayTime(unsigned long ms) {
  unsigned long ms2 = ms/100;
  unsigned long seconds = ms / 1000;
  unsigned long minutes = seconds / 60;
  seconds = seconds % 60;
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);
  lcd.print(":");
  if (ms2 < 10) lcd.print("0");
  lcd.print(ms2);
}

void resetTimer() {
  Start1 = 0;
  Stop1 = 0;
  Ready1 = 0;
  elapsedTime = 0;
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("<SprintTimer>");
  delay(500);
}
