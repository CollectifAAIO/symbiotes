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
  //String melod = "melody_" + song;  //inttochar
  //String melody = melod.c_str();
  //String rythme = String("rythme_" + song);  //inttochar
  //Serial.println(melody);

  int speedOfPlay = tempo + randomness;
  // int coeff = random(0, 200) - 100;
  // long randomDetune = 100 * randomness;

  if (song == 0 && changeSeq == false) {
    //memcpy(melody, melody_1, sizeof(melody_1)/sizeof(int));
    //memcpy(rythme, rythme_1, sizeof(rythme_1)/sizeof(int));
    int sizeOfMelody1 = sizeof(melody_1) / sizeof(int);
    for (int i = 0; i < sizeOfMelody1; i++) {
        melody[i] = melody_1[i];
        rythme[i] = rythme_1[i];
        Serial.println(rythme[i]);
    }
    changeSeq = true;
  }
  else if (song == 1 && changeSeq == false) {
    //memcpy(melody, melody_2, sizeof(melody_2)/sizeof(int));
    //memcpy(rythme, rythme_2, sizeof(rythme_2)/sizeof(int));
    for (int i = 0; i < sizeof(melody_2)/sizeof(int); i++) {
        melody[i] = melody_2[i];
        rythme[i] = rythme_2[i];
        Serial.println(rythme[i]);
    }
    changeSeq = true;
  }

  int sizeOfMelody = sizeof(melody) / sizeof(int);
  Serial.println(sizeOfMelody);

  if (thisNote < sizeOfMelody) {
    // to calculate the note duration, take one second divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    unsigned int noteDuration = speedOfPlay / rythme[thisNote];
    //Serial.println(noteDuration);

    if (timeElapsed < noteDuration * 0.9) {
      if (changeNoteOn == false) {
        if (melody[thisNote] == 0) {
          string1.noteOff(0);
          changeNoteOn = true;
        } else {
          changeNoteOn = true;
          long coeff = 10; //random(0, 200) - 50;
          long randomDetune = coeff * randomness;
          //Serial.println(randomDetune);
          string1.noteOn(melody[thisNote] + randomDetune, 1);
        }
      }

      else if (timeElapsed > noteDuration * 0.9 && timeElapsed < noteDuration) {
        if (changeNoteOff == false) {
          string1.noteOff(0);
          changeNoteOff = true;
        }
      }
    }
    if (timeElapsed > noteDuration) {
      changeNoteOff = false;
      changeNoteOn = false;
      timeElapsed = 0;
      thisNote++;
      //Serial.println(melody[thisNote]);
    }
  }

  else {
    changeSeq = false;
    thisNote = 0;
  }
}
