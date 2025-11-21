/*
Project:        Good or Bad Entry!!
Name:           Just Vic
Date:           Always tomorrow
Purpose:        Simple RFID access control:
                  Green LED = allowed
                  Red LED   = denied
Libraries:      SPI, MFRC522
Special-Items:  RFID reader (RC522), 2 LEDs / Arduino-UNO
*/
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

const int LED_GREEN = 5;
const int LED_RED   = 6;

byte allowedUIDs[][4] = 
{
  {0xDE, 0xAD, 0xBE, 0xEF}
};

bool isAllowed(byte *uid) 
{
  for (int i = 0; i < 4; i++) 
  {
    if (uid[i] != allowedUIDs[0][i]) 
    {
      return false;
    }
  }
  return true;
}

void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED,   OUTPUT);
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial())    return;

  byte *uid = rfid.uid.uidByte;

  if (isAllowed(uid)) 
  {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED,   LOW);
  } 
    else 
  {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED,   HIGH);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(1000);
}
