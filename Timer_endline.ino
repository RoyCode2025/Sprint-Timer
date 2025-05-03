
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(7, 8);          // CE, CSN
const byte address[10] = "ADDRESS01";

char txt1[] = "GO", txt2[] = "STOP";
int laserIn=A0;
int laserVal;
int greenLed=10;
int pwrLed=7;

void setup() {
  Serial.begin(9600);
  pinMode(laserIn, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(pwrLed, OUTPUT);
  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  digitalWrite(pwrLed, HIGH);
}; 

void loop() {
  
  laserVal=analogRead(laserIn);
 
  if (laserVal>=50) {
     digitalWrite(greenLed, HIGH);
     radio.write(&txt1, sizeof(txt1)); Serial.println(txt1);
  } else {
     digitalWrite(greenLed, LOW);
     radio.write(&txt2, sizeof(txt2)); Serial.println(txt2);
  };
  //Serial.println(laserVal);
  delay(10);
};
