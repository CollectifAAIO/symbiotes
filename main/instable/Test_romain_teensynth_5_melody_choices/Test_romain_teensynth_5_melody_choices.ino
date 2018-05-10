// Utiliser "" au lieu de <> pour les fichier header inclus dans le dossier projet.
#include "pitches.h"            // Correspondances notes / fréquences
#include "audioConnections.h"   // Connections Teensy Audio Lib
#include "melodies.h"           // Stockage des mélodies

elapsedMillis timeElapsed;      // gestion du timer
bool changeSeq = false;
int thisNote = 0;
int melody[16];
int rythme[16];
bool change0 = false;
bool changeNoteOn = false;
bool changeNoteOff = false;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(9));

  AudioMemory(50);             // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
  // setup audio board

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);                      //REGLAGE
}

void loop() {
  int speedVariable = 1000;
  playSong(0, speedVariable, 5);
  playSong(1, speedVariable, 5);
}

void playSong(int song, int tempo, int randomness) {

  // Changement de mélodie selon le premier argument : rythmes et mélodies copiés depuis melodies.h dans les arrays "melody" et "rythme".
  if (song == 0 && changeSeq == false) {
    int sizeOfMelody1 = sizeof(melody_1) / sizeof(int);
    for (int i = 0; i < sizeOfMelody1; i++) {
      melody[i] = melody_1[i];
      rythme[i] = rythme_1[i];
      Serial.println(rythme[i]);
    }
    changeSeq = true;
  }
  else if (song == 1 && changeSeq == false) {
    for (int i = 0; i < sizeof(melody_2) / sizeof(int); i++) {
      melody[i] = melody_2[i];
      rythme[i] = rythme_2[i];
      Serial.println(rythme[i]);
    }
    changeSeq = true;
  }
  else if (song == 2 && changeSeq == false) {
    for (int i = 0; i < sizeof(melody_3) / sizeof(int); i++) {
      melody[i] = melody_3[i];
      rythme[i] = rythme_3[i];
      Serial.println(rythme[i]);
    }
    changeSeq = true;
  }

  // Number of items in arrays: length of the melody to play.
  int sizeOfMelody = sizeof(melody) / sizeof(int);

  // Play song :
  if (thisNote < sizeOfMelody) {
    //Calculates the speed of play according to the "tempo" and "randomness" arguments of "playSong" function.
    float randomToPercentage = randomness * random(-1, 2);
    float speedOfPlay = tempo * randomToPercentage;
    // to calculate the note duration, take speedOfPlay divided by the note rhythm : e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    unsigned int noteDuration = speedOfPlay / rythme[thisNote];

    if (timeElapsed < noteDuration * 0.9) {
      if (changeNoteOn == false) {
        if (melody[thisNote] == 0) { // 0 is recognized as a rest.
          string1.noteOff(0);
          changeNoteOn = true;
        } else {
          changeNoteOn = true;
          long coeff = 10; //random(0, 200) - 50;
          long randomDetune = coeff * randomness;
          //Serial.println(randomDetune);
          string1.noteOn(melody[thisNote] + randomDetune, 1); //every other note is recognized as a frequency.
        }
      }

      else if (timeElapsed > noteDuration * 0.9 && timeElapsed < noteDuration) { // Note off when arrived at 90% of the duration of each note.
        if (changeNoteOff == false) {
          string1.noteOff(0);
          changeNoteOff = true;
        }
      }
    }
    if (timeElapsed > noteDuration) { // Reset all variables for the next note to play.
      changeNoteOff = false;
      changeNoteOn = false;
      timeElapsed = 0;
      thisNote++;
      //Serial.println(melody[thisNote]);
    }
  }

  else {
    changeSeq = false; //reset changeSeq to choose another melody.
    thisNote = 0;
  }
}
