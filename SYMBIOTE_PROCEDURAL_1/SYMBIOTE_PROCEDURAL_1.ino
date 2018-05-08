#include "AudioConfig.h"
#include "GlobalVariables.h"
#include "Sensors.h"
#include "FM4_synth.h"

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A9));  //passer la fonction random en réelle fonction aléatoire.
  AudioMemory(70);             // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
  // setup audio board

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);                         //REGLAGE
}

// >>>>> MAIN LOOP <<<<<

void loop() {

  MACROExpressivite = Proxi();
  MACRODensity = map(Proxi(), 0.0, 1.0, MaxTimeNoteOnBorneMin, MaxTimeNoteOnBorneMax);

  // CPU & SENSOR MONITORING

  if (MonitorTimeElapsed > RefreshScreen) {
    //      Serial.print(AudioProcessorUsageMax());
    //      Serial.print("  ");
    //      Serial.println(AudioMemoryUsageMax());
    //      AudioProcessorUsageMaxReset();
    //      AudioMemoryUsageMaxReset();
    //      Serial.println();
    //        Serial.print("Proxi Scale : ");
    //        Serial.println(Proxi());
    //        Serial.print("Proxi Scale : ");
    //        Serial.println(Proxi());
//    Serial.print("Temps Note On : ");
//    Serial.println(randomNoteOnTime);
//    Serial.print("Temps Note Off : ");
//    Serial.println(randomNoteOffTime);
    //    Serial.print("Macro Expressivite : ");
    //    Serial.println(MACROExpressivite);
    //    Serial.print("Macro Density : ");
    //    Serial.println(MACRODensity);
    MonitorTimeElapsed = 0;
  }

  // Define new random sound density according sensor and every cycle.

  if (RandomDensiteTimeElapsed > RandomDensiteTimeCycle) {
    randomNoteOnTime = random(MinTimeNoteOn, MACRODensity);
    randomNoteOffTime =  randomNoteOnTime + random(MinTimeNoteOff, MaxTimeNoteOff);
    RandomDensiteTimeElapsed = 0;
  }

  // >>>>> NOTE ON <<<<<

  if (TimeNoteElapsed > randomNoteOnTime) {
    if (ChangeNoteOn != true) {
      Note_On = true;
      FM4_synth(Note_On, Note_Off, MACROExpressivite);
      Note_On = false;
      ChangeNoteOn = true;
    }
  }

  // >>>>> NOTE OFF <<<<<

  if (TimeNoteElapsed > randomNoteOffTime) {
    Note_Off = true;
    FM4_synth(Note_On, Note_Off, MACROExpressivite);
    Note_Off = false;
    TimeNoteElapsed = 0;
    ChangeNoteOn = false;
  }
}
