
#include <Servo.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


TinyGPSPlus gps;
SoftwareSerial ss(4, 3);
Servo myservo;
RF24 radio(9, 10);

int servo = 8;
int gocmax=90;
int gocmin=0;
const byte address[6] = "25061";

// ham setup radio nrf
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
}

int getGPSData() {
  float docao = 0;
  if (ss.available() > 0)
  {
    gps.encode(ss.read());
    if (gps.altitude.isUpdated()) {
    if((unsigned long) (millis() - hientai >= 5000)){
      hientai = millis();
      Serial.print("\n Do cao: "); 
      Serial.print(gps.altitude.meters());
      docao = gps.altitude.meters();
    }
    }
  }
  return docao;
} 

void setup() 
{
  Serial.begin(9600);
  delay(2000);
  ss.begin(9600);
  //myservo.attach(servo);
  //myservo.write(90);
  setupRadio();

}

void loop() 
{
  if (ss.available() > 0)
  {
    gps.encode(ss.read());
    if (gps.altitude.isUpdated())
    {
      Serial.print("\n Do cao = ");
      Serial.print(gps.altitude.meters());  docao = gps.altitude.meters();

      radio.write(&docao, sizeof(docao));

      if (docao == 28.40) 
      {
        myservo.write(gocmin);
        Serial.println(" Trang thai mo ");
      }
      else
      {
        myservo.write(gocmax);
        Serial.println(" Trang thai dong ");
      } 
      delay(3000);
    }
  }
}

