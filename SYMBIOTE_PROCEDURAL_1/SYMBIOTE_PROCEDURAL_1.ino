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
#include "helpers.h"
#include "Presets.h"
#include "Sensors.h"
#include "Sequencer.h"

static FM4 FM4synth;
static Sequencer seq;
static MicDetection micDetection;

#define PARMS_DUMP

const BinaryPreset c_presets_1[] = {
  #include "preset_data_1.h"
};
const BinaryPreset c_presets_2[] = {
  #include "preset_data_2.h"
};
constexpr unsigned c_presetSize = sizeof(c_presets_1)/sizeof(*c_presets_1);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A9));  //passer la fonction random en réelle fonction aléatoire.
  AudioMemory(16);             // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
  // setup audio board

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.25);                         //REGLAGE
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);

  ProxiSetup();

  FM4synth.init();
  seq.start();
  ParseBinaryPresets(c_presets_1, c_presets_2, c_presetSize, FM4synth, seq);
}

// >>>>> MAIN LOOP <<<<<

bool isNoteOn = false;

void loop() {
  const float MACROExpressivite = Proxi();
  Serial.println(MACROExpressivite);
  seq.setInterpolationFactor(MACROExpressivite);
  FM4synth.setInterpolationFactor(MACROExpressivite);

  // Mic detection
  micDetection.update();
  if (micDetection.hasDetected()) {
    seq.start();
    seq.noteOn(FM4synth);
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

  // Debug presets management
  if(Serial.available()) {
    int synthStripIndex = 0;
    unsigned parmIndex = 0;
    ParameterValues parmValues;
    const String data = Serial.readString();
    if (ParseParameterLine(data, synthStripIndex, parmIndex, parmValues)) {
      if (parmIndex > SynthParameterIndex::synth_Count) {
        if (parmIndex >= SequencerParameterIndex::seq_Count) {
          if (parmIndex >= MicDetectionParameterIndex::mic_Count) {
              proxyDetectionMode = static_cast<unsigned>(parmValues.data_[0]);
              gravityBang.reset();
          } else {
            const MicDetectionParameterIndex micParmIndex = static_cast<MicDetectionParameterIndex>(parmIndex - SynthParameterIndex::synth_Count - SequencerParameterIndex::seq_Count);
            micDetection.setIndexedParameter(micParmIndex, parmValues);
          }
        } else {
          const SequencerParameterIndex seqParmIndex = static_cast<SequencerParameterIndex>(parmIndex - SynthParameterIndex::synth_Count);
          seq.setIndexedParameter(0, seqParmIndex, parmValues);
          seq.setIndexedParameter(1, seqParmIndex, parmValues);
        }
      } else {
        const SynthParameterIndex synthParmIndex = static_cast<SynthParameterIndex>(parmIndex);
        // For now the synth has no multi-values parameters
        FM4synth.setIndexedParameter(synthStripIndex, 0, synthParmIndex, parmValues.data_[0]);
        FM4synth.setIndexedParameter(synthStripIndex, 1, synthParmIndex, parmValues.data_[0]);
        FM4synth.applyParms();
      }
    } else {
#ifdef PARMS_DUMP
      FM4synth.dumpParms();
      seq.dumpParms();
#endif // PARMS_DUMP
    }
  }
}

