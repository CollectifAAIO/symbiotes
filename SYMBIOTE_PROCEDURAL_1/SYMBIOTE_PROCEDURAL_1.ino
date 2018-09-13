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
#include "Presets.h"

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A9));  //passer la fonction random en réelle fonction aléatoire.
  AudioMemory(16);             // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
  // setup audio board

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.25);                         //REGLAGE
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);

  FM4_init();
}

// >>>>> MAIN LOOP <<<<<

void loop() {

  //MACROExpressivite = Proxi();
  //MACRODensity = map(Proxi(), 0.0, 1.0, MaxTimeNoteOnBorneMin, MaxTimeNoteOnBorneMax);

  if (isNoteOn) {
    if (TimeNoteElapsed > randomNoteOffTime) {
      Serial.print(MonitorTimeElapsed);
      FM4_synth(false, true, 1.0f);
      isNoteOn = false;
    }
  }

  if( TimeNoteElapsed > MinTimeNoteOn ) {
    if ( peak1.available() ) {
      if ( peak1.read() > 0.25 ) {
        Serial.print(MonitorTimeElapsed);
        FM4_synth(true, false, 1.0f);
        isNoteOn = true;
        TimeNoteElapsed = 0;
        randomNoteOffTime = random(MinTimeNoteOff, MaxTimeNoteOff);
      }
    }
  }

  // Diagnostic
  if (Serial.available() > 0) {
    // don't care about the actual character that was read
    Serial.read();
    Serial.println(AudioProcessorUsageMax());
    Serial.println(AudioMemoryUsageMax());

    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();
  }

  // Debug presets management
  SynthStripParms parms;
  int SynthStripIndex = 0;
  ParseParms(parms, SynthStripIndex);
}
