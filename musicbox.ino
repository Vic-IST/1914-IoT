/*
Project:        Retro Music Box
Name:           Just Vic
Date:           This is always done last week!!
Purpose:        Build a music box that plays retro-style tunez
                using a buzzer, buttons, and LEDs.
Libraries:      none
Special-Items:  Piezo buzzer, pushbuttons, LEDs
*/

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

const int buzzerPin = 8;   // Buzzer on D8
const int buttonSong1 = 2; // Button for Song 1 on D2
const int ledPin = 13;     // LED on D13

int melodySong1[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5,
  NOTE_B4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_D4, NOTE_C4, 0     // 0 = rest
};

int durationsSong1[] = {
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4,
  4, 4, 4, 4
};

int lengthSong1 = sizeof(melodySong1) / sizeof(int);

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonSong1, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Music Box Ready! Press Button 1 to play Song 1.");
}

void loop() {
  if (digitalRead(buttonSong1) == LOW) {   // Button pressed
    Serial.println("Playing Song 1...");
    playSong(melodySong1, durationsSong1, lengthSong1);
  }

  delay(50); // small debounce
}

void playSong(int melody[], int durations[], int length) {
  for (int i = 0; i < length; i++) {
    int note = melody[i];
    int noteDuration = 1000 / durations[i]; // 4 = quarter note, 8 = eighth note

    if (note > 0) {
      tone(buzzerPin, note, noteDuration);
      digitalWrite(ledPin, HIGH);  // LED on while note plays
    } else {
      noTone(buzzerPin);
      digitalWrite(ledPin, LOW);   // LED off during rest
    }

    delay(noteDuration * 1.30);    // pause between notes
    digitalWrite(ledPin, LOW);
  }

  noTone(buzzerPin);
}
