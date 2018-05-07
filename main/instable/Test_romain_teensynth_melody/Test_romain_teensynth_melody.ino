// Utiliser "" au lieu de <> pour les fichier header inclus dans le dossier projet.
#include "pitches.h"            // Correspondances notes / fréquences
#include "audioConnections.h"   // Connections Teensy Audio Lib
#include "melodies.h"           // Stockage des mélodies

elapsedMillis timeElapsed;      // gestion du timer
int thisNote = 0;
bool change0 = false;
bool change1 = false;
bool change2 = false;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(9));

  AudioMemory(50);             // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
  // setup audio board


  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);                      //REGLAGE
  //  sine1.amplitude(1);
}

void loop() {
  int speedVariable = 1000;
  playSong(0, speedVariable, 10);
}

void playSong(int s, int speedOfPlay, int randomness) {
  int song = s;
  int durationVariability = speedOfPlay;
  int randomDetune = (random(200) - 100) * randomness;
  
  if (song == 0); {
    int sizeOfMelody = sizeof(melody) / sizeof(int);

    if (thisNote < sizeOfMelody) {
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      //unsigned int noteDuration;

      //Serial.println(thisNote);
      //if (change0 == false) {
      change0 = true;
      unsigned int noteDuration = durationVariability / tempo[thisNote];
      //Serial.println(noteDuration);


      if (timeElapsed < noteDuration * 0.8) {
        if (change1 == false) {
          if (melody[thisNote] == 0) {
            string1.noteOff(0);
          } else {
            string1.noteOn(melody[thisNote] + randomDetune, 1);
            change1 = true;
          }

        }

        else if (timeElapsed > noteDuration * 0.9 && timeElapsed < noteDuration) {
          if (change2 == false) {
            string1.noteOff(0);
            change2 = true;
          }
        }
      }
      if (timeElapsed > noteDuration) {
        change2 = false;
        change1 = false;
        change0 = false;
        timeElapsed = 0;
        thisNote++;
        Serial.println(thisNote);
      }
      //}
      //Serial.println(timeElapsed);
      //delay(noteDuration);

    }
    else {
      thisNote = 0;
    }
  }
}

/*void playNote(int notePitch, int noteVel, unsigned int noteLength) {
   if (timeElapsed < noteLength * 0.8) {
     if (change1 == false) {
       Serial.println(notePitch);
       string1.noteOn(notePitch, noteVel);
       change1 = true;
     }
    }
    else if (timeElapsed > noteLength * 0.8) {
     string1.noteOff(0);
    }
    if (timeElapsed > noteLength) {
     timeElapsed = 0;
     change1 = false;
    }
  }*/
