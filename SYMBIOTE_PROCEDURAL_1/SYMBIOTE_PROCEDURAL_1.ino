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
#include "FM4_synth.h"
#include "GlobalVariables.h"
#include "Presets.h"
#include "Sensors.h"
#include "Sequencer.h"

static FM4 FM4synth;
static Sequencer seq;

//#define PARMS_DUMP

const BinaryPreset c_presets[] = {
  #include "preset_data.h"
};
constexpr unsigned c_presetsCount = sizeof(c_presets)/sizeof(*c_presets);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A9));  //passer la fonction random en réelle fonction aléatoire.
  AudioMemory(16);             // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
  // setup audio board

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.25);                         //REGLAGE
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);

  FM4synth.init();
  seq.start();
  ParseBinaryPresets(c_presets, c_presetsCount, FM4synth, seq);
}

// >>>>> MAIN LOOP <<<<<

bool isNoteOn = false;

void loop() {

  //MACROExpressivite = Proxi();
  //MACRODensity = map(Proxi(), 0.0, 1.0, MaxTimeNoteOnBorneMin, MaxTimeNoteOnBorneMax);

  if ( peak1.available() ) {
    if ( peak1.read() > 0.5 ) {
      seq.start();
      seq.noteOn(FM4synth);
    }
  }

  seq.update(FM4synth);

  // Diagnostic
  if (Serial.available() > 0) {
    // don't care about the actual character that was read
    Serial.println(AudioProcessorUsageMax());
    Serial.println(AudioMemoryUsageMax());

    AudioProcessorUsageMaxReset();
    AudioMemoryUsageMaxReset();
  }

#ifdef PARMS_DUMP
  if(Serial.available()) {
    FM4synth.dumpParms();
    seq.dumpParms();
  }
#endif // PARMS_DUMP

  // Debug presets management
  if(Serial.available()) {
    int synthStripIndex = 0;
    unsigned parmIndex = 0;
    ParameterValues parmValues;
    const String data = Serial.readString();
    if (ParseParameterLine(data, synthStripIndex, parmIndex, parmValues)) {
      if (parmIndex > SynthParameterIndex::Count) {
        const SequencerParameterIndex seqParmIndex = static_cast<SequencerParameterIndex>(parmIndex - SynthParameterIndex::Count);
        seq.setIndexedParameter(seqParmIndex, parmValues);
      } else {
        const SynthParameterIndex synthParmIndex = static_cast<SynthParameterIndex>(parmIndex);
        // For now the synth has no multi-values parameters
        FM4synth.setIndexedParameter(synthStripIndex, synthParmIndex, parmValues.data_[0]);
        FM4synth.applyParms();
      }
    }
  }
}

