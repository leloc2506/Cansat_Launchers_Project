#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

SoftwareSerial ss(4, 3);
RF24 radio(9, 10);
TinyGPSPlus gps;
Servo myservo;

float docao;
unsigned char autoservo = 0;
unsigned long hientai = 0;
unsigned long saiso = 0;
#define goc_mo 0
#define goc_dong 90

const byte address[6] = "25061";

void setup() {
  Serial.begin(9600);
  delay(2000);
  ss.begin(9600);
  myservo.attach(8);
  myservo.write(goc_dong);

  Serial.println("\nKiem tra NRF");
  setupRadio();
  
}

void loop() {
  if (ss.available() > 0)
    gps.encode(ss.read());

  if (gps.altitude.isUpdated())
  { 
    if(millis() - hientai >= 3000) {
    hientai = millis();
    Serial.print("\n Do cao = ");
    Serial.print(gps.altitude.meters()); 
    docao = gps.altitude.meters();
    radio.write(&docao, sizeof(docao));
    }

    if (docao >= 25.00 && docao <= 26.00) 
    {
      autoservo = 1;
      saiso = millis();
      myservo.write(goc_mo);
    }

    if (millis() - saiso >= 2000 && autoservo == 1){
      autoservo = 0;
      myservo.write(goc_dong);
    }
  } 
}

int setupRadio() {
  radio.begin();

  if (!radio.begin())
  {
    Serial.println("module khd");
  }
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(80);
  radio.setDataRate(RF24_2MBPS);
  radio.stopListening();

  if (!radio.available())
  {
    Serial.println("Don't connect RX");
    Serial.println("Loading");
    delay(1000);
  }
} // ham setup radio nrf