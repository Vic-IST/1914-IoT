/*
Project:        Da-Bomb!! / Bomb Voyage!!
Name:           Just Vic
Date:           Today (maybe yesterday)
Purpose:        This project was to build a bomb that can 
                  count down from 30 seconds and make a 
                  beeping noise to simulate an explosion. 
Libraries:      Adafruit_GFX
                Adafruit_LEDBackpack
Special-Items:  HT16K33 (7-seg I2C)
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

#define BTN_START 5
#define BTN_DEFUSE 6
#define BTN_RESET 7
#define LED_RED 12
#define LED_GREEN 13
#define BUZZ 8

Adafruit_7segment disp = Adafruit_7segment();

int startSeconds = 30;     // default countdown length (keep it default and change as you wish)
int timerValue = startSeconds;
bool armed = false;
bool defused = false;

unsigned long lastTick = 0;
unsigned long lastColonBlink = 0;
bool colonOn = true;

void showTime(int seconds) {
  // Display as MMSS (e.g., 0030)
  int mm = seconds / 60;
  int ss = seconds % 60;
  int value = mm * 100 + ss;
  disp.print(value);            //  number
  disp.drawColon(colonOn);      // blinks for drama from LED
  disp.writeDisplay();
}

void boom() {
  armed = false;
  for (int i=0; i<6; i++) {
    digitalWrite(LED_RED, HIGH);
    tone(BUZZ, 2000, 150);
    delay(200);
    digitalWrite(LED_RED, LOW);
    delay(200);
  }
  // Show 0000 at end
  colonOn = false;
  showTime(0);
}

void setup() {
  pinMode(BTN_START, INPUT_PULLUP);
  pinMode(BTN_DEFUSE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZ, OUTPUT);

  disp.begin(0x70);         // default I2C addr for HT16K33
  disp.setBrightness(15);   // 0..15
  showTime(timerValue);

  Serial.begin(9600);
  Serial.println("Ready. Press START to arm.");
}

void loop() {
  // --- Button handling (active LOW due to INPUT_PULLUP) ---
  if (digitalRead(BTN_START) == LOW && !armed) {
    armed = true; defused = false; timerValue = startSeconds;
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    Serial.println("Armed!");
    delay(200);
  }

  if (digitalRead(BTN_DEFUSE) == LOW && armed) {
    defused = true; armed = false;
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZ);
    Serial.println("Defused!");
    delay(200);
  }

  if (digitalRead(BTN_RESET) == LOW) {
    timerValue = startSeconds; armed = false; defused = false;
    digitalWrite(LED_RED, LOW); digitalWrite(LED_GREEN, LOW);
    colonOn = true; showTime(timerValue);
    Serial.println("Reset.");
    delay(200);
  }

  // --- Colon blinky (every 500 ms) ---
  if (millis() - lastColonBlink >= 500) {
    lastColonBlink = millis();
    colonOn = !colonOn;
    showTime(timerValue);
  }

  // --- timer/countdown (every 1000 ms) ---
  if (armed && !defused && millis() - lastTick >= 1000) {
    lastTick = millis();
    tone(BUZZ, 1000, 80);      // ticky/beepy
    showTime(timerValue);
    Serial.print("Timer: "); Serial.println(timerValue);
    timerValue--;

    if (timerValue < 0) {
      boom();
    }
  }
}
