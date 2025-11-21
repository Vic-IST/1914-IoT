/*
Project:        Good or Bad Entry!!
Name:           Just Vic
Date:           Tomorrow
Purpose:        Simple RFID access control:
                  • Green LED = allowed access
                  • Red LED   = denied access
Libraries:      SPI
                MFRC522
Special-Items:  RFID reader (RC522), 2 LEDs with resistors
*/

#include <SPI.h>            // add SPI library for communication with RFID reader
#include <MFRC522.h>        // add MFRC522 library for RFID reader module

#define SS_PIN  10          // RC522: Slave Select (SDA) pin connected to D10
#define RST_PIN 9           // RC522: Reset pin connected to D9
MFRC522 rfid(SS_PIN, RST_PIN); // build an RFID reader object using these pins

const int LED_GREEN = 5;       // pin for green LED (access granted indicator)
const int LED_RED   = 6;       // pin for red LED (access denied indicator)

// list of allowed card UIDs (4 bytes each) use your own tag / student ID?
byte allowedUIDs[][4] = 
{
  { 0xDE, 0xAD, 0xBE, 0xEF }     // example UID
};
const int ALLOWED_COUNT = sizeof(allowedUIDs) / sizeof(allowedUIDs[0]); // number of allowed cards

// function: checks if the scanned card's UID matches one in the allowed list
bool isAllowed(byte *uid, byte len) 
{
  if (len < 4) return false;     // if UID length is less than 4 bytes, deny access
  for (int i = 0; i < ALLOWED_COUNT; i++) 
  {
    bool match = true;
    for (int j = 0; j < 4; j++) 
    {
      if (uid[j] != allowedUIDs[i][j]) 
      {
        match = false;
        break;                     // stop checking this allowed UID if any byte mismatches
      }
    }
    if (match) return true;        // You are in dude!!!
  }
  return false;                    // Nah man, you need another card....
}

void setup() 
{
  Serial.begin(9600);              // serial monitor at 9600 baud for debugging
  SPI.begin();                     // SPI bus for RC522 communication
  rfid.PCD_Init();                 // RC522 RFID reader

  pinMode(LED_GREEN, OUTPUT);      // LED pins as outputs
  pinMode(LED_RED,   OUTPUT);

  digitalWrite(LED_GREEN, LOW);    // ensure LEDs start OFF
  digitalWrite(LED_RED,   LOW);

  Serial.println("Scan a card/tag..."); // tells user via serial monitor
}

void loop() 
{
  if (!rfid.PICC_IsNewCardPresent()) return;    // if no new card is present, skip rest
  if (!rfid.PICC_ReadCardSerial())    return;   // if we can't read the card's UID, skip

  byte *uid = rfid.uid.uidByte; // array containing card’s UID bytes
  byte  len = rfid.uid.size;    // number of bytes in the UID

  // print the UID to serial monitor for reference/debugging
  Serial.print("UID:");
  for (byte i = 0; i < len; i++) 
  {
    Serial.print(uid[i], HEX);
    if (i < len - 1) Serial.print(":");
  }
  Serial.println();

  // See if the card is allowed
  if (isAllowed(uid, len)) 
  {
    Serial.println("ACCESS GRANTED");
    digitalWrite(LED_GREEN, HIGH);   // Kick ON green LED
    digitalWrite(LED_RED,   LOW);    // Kick OFF red LED
  } 
    else 
  {
    Serial.println("ACCESS DENIED");
    digitalWrite(LED_GREEN, LOW);    // Kick OFF green LED
    digitalWrite(LED_RED,   HIGH);   // Kick ON red LED
  }

  rfid.PICC_HaltA();              // no further reading of the current card
  rfid.PCD_StopCrypto1();         // stop encryption for security

  delay(1000);                    // waits 1 second before next scan (gives time to remove tag)
}
