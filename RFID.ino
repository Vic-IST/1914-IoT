/*
Project:        Godd or Bad Entry!!
Name:           Just Vic
Date:           Today (maybe tomorrow)
Purpose:        This project was to build a device that can record or 
                allow entry into a door or something that allows access. . 
Libraries:      SPI 
                MFRC522
Special-Items:  RFID Access System (RC522 + ONE Add-On if chosing that route)
*/

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

// ---- Enable only ONE add-on block below ----
// 1) Servo
//#include <Servo.h>
//Servo lockServo; const int SERVO_PIN = 6;

// 2) Keypad
//#include <Keypad.h>
//const byte ROWS = 4, COLS = 4;
//char keys[ROWS][COLS] = {
//  {'1','2','3','A'},
//  {'4','5','6','B'},
//  {'7','8','9','C'},
//  {'*','0','#','D'}
//};
//byte rowPins[ROWS] = {2,3,4,5};
//byte colPins[COLS] = {6,7,8,9};
//Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
//String pinBuffer = "";
//const String requiredPIN = "1234";

// 3) LCD I2C
//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 16, 2);

// 4) Buzzer + LEDs
//const int BUZZ = 5;
//const int LED_R = 3;
//const int LED_G = 4;

byte allowed[][4] = {
  {0xDE, 0xAD, 0xBE, 0xEF},
  {0x12, 0x34, 0x56, 0x78}
};
const int ALLOWED_COUNT = sizeof(allowed) / 4;

bool isAllowed(byte *uid, byte len) {
  if (len < 4) return false;
  for (int i = 0; i < ALLOWED_COUNT; i++) {
    bool match = true;
    for (int j = 0; j < 4; j++) {
      if (uid[j] != allowed[i][j]) match = false;
    }
    if (match) return true;
  }
  return false;
}

void grantAccess() {
  // Implement your chosen add-on behavior here
}

void denyAccess() {
  // Implement your chosen deny behavior here
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  // Initialize add-on (servo attach, keypad begin, LCD begin, etc)
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  byte *uid = mfrc522.uid.uidByte;
  byte  len = mfrc522.uid.size;

  Serial.print("UID: ");
  for (byte i = 0; i < len; i++) 
  {
    Serial.print(uid[i], HEX);
    Serial.print(i < len-1 ? ":" : "");
  }
  Serial.println();

  if (isAllowed(uid, len)) {
    Serial.println("ACCESS GRANTED");
    grantAccess();
  } else {
    Serial.println("ACCESS DENIED");
    denyAccess();
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
