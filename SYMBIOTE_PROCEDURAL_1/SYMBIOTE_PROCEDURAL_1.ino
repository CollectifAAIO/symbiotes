/// @author fb
/// @copyright fb 2018
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Lesser Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU Lesser Public License for more details.
///
/// You should have received a copy of the GNU Lesser Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.


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

  FM4_init();
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
