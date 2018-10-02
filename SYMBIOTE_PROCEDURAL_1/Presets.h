/// @author gm
/// @copyright gm 2018
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

#ifndef _PRESETS_H_
#define _PRESETS_H_

#include "FM4_synth.h"
#include "helpers.h"
#include "Sequencer.h"

static const String c_tokens[] = {
  "waveform",
  "glide",
  "Pitch",
  "glide_rand",
  "Pitch_rand",
  "ListenSeq",
  "Transpose",
  "Octave",
  "FM_Osc1",
  "FM_Osc2",
  "FM_Osc3",
  "FM_Osc4",
  "FM_Osc1_rand",
  "FM_Osc2_rand",
  "FM_Osc3_rand",
  "FM_Osc4_rand",
  "AM_Waveform",
  "AM_Depth",
  "AM_Freq",
  "AM_Depth_rand",
  "AM_Freq_rand",
  "PADSR_Dlay",
  "PADSR_Amp",
  "PADSR_Atk",
  "PADSR_Dcay",
  "PADSR_Sus",
  "PADSR_Rel",
  "PADSR_Dlay_rand",
  "PADSR_Amp_rand",
  "PADSR_Atk_rand",
  "PADSR_Dcay_rand",
  "PADSR_Sus_rand",
  "PADSR_Rel_rand",
  "LADSR_Dlay",
  "LADSR_Amp",
  "LADSR_Atk",
  "LADSR_Dcay",
  "LADSR_Sus",
  "LADSR_Rel",
  "LADSR_Dlay_rand",
  "LADSR_Amp_rand",
  "LADSR_Atk_rand",
  "LADSR_Dcay_rand",
  "LADSR_Sus_rand",
  "LADSR_Rel_rand",
  "Vol",
  "RandomizeSeqOrSound",
  "loop",
  "bpm",
  "RestartFrom0",
  "StepNumber",
  "RandomSpeed",
  "DutyCycle",
  "arpeg",
  "Trigers",
  "random-trig",
  "min-trigger-time",
  "max-trigger-time",
  "Threshold",
  "Hysteresis",
  "WindowLength",
  "proxyDetectionMode"
};
constexpr int c_tokensCount = sizeof(c_tokens) / sizeof(String);

//#define PRESET_DEBUG

struct BinaryPreset {
  unsigned SynthStripIndex_;
  unsigned ParameterIndex_;
  unsigned ParameterValuesCount_;
  float ParameterValues_[ParameterValues::c_itemsCount];
};

bool ParseToken(const String & data, unsigned & inOutCursor, String & outToken) {
  unsigned cursor = inOutCursor;
  const unsigned dataLength = data.length();
  String token;
  token.reserve(dataLength);
  while(cursor < dataLength && data[cursor] != ' ' && data[cursor] != '\n') {
    token += data[cursor];
    cursor += 1;
  }
  const unsigned initialCursor = inOutCursor;
  inOutCursor = cursor;
  if (cursor > initialCursor) {
    outToken = token;
#ifdef PRESET_DEBUG
    Serial.printf("ParseToken: %s\n", token.c_str());
#endif // PRESET_DEBUG
    return true;
  }
#ifdef PRESET_DEBUG
  Serial.printf("ParseToken failed on %s", data.c_str());
#endif // PRESET_DEBUG
  return false;
}

unsigned ParseNumberTokens(const String & data, unsigned & inOutCursor, ParameterValues & out) {
  unsigned cursor = inOutCursor;
  const unsigned dataLength = data.length();
  String token;
  token.reserve(16); // arbitrary value, should be enough for the numbers we use
  int valueIndex = 0;

  while(data[cursor] == ' ') {
    cursor += 1;
  }

  while (cursor < dataLength && data[cursor] != '\n' && ParseToken(data, cursor, token)) {
    const float value = token.toFloat();
    out.data_[valueIndex++] = value;
#ifdef PRESET_DEBUG
    Serial.printf("ParseNumberToken: %s %f\n", token.c_str(), value);
#endif // PRESET_DEBUG
    token = "";
    while(data[cursor] == ' ') {
      cursor += 1;
    }
  }
  return valueIndex;
}

void CleanIdToken(String & _inOutToken, const bool isSynthPreset) {
  constexpr const char * c_synthPrefix = "-pst-";
  constexpr const char * c_seqPrefix = "pst-";

  _inOutToken = _inOutToken.substring( isSynthPreset ? sizeof(c_synthPrefix) + 1 : sizeof(c_seqPrefix));
#ifdef PRESET_DEBUG
  Serial.printf("CleanIdToken: %s\n", _inOutToken.c_str());
#endif // PRESET_DEBUG
}

bool ParseParameterLine(const String & data, int & _outStripIndex, unsigned & _outParmIndex, ParameterValues & _outParmValues) {
  char firstChar = data[0];
  const bool isSynthPreset = isDigit(firstChar);
  const bool isSeqPreset = firstChar == 'p';
  if (isSynthPreset || isSeqPreset) {
    // 48 => "0" in ASCII
    const int stripIndex = firstChar - 48 - 1;

    unsigned cursor = isSynthPreset? 1 : 0;
    String token;
    if (ParseToken(data, cursor, token)) {
      CleanIdToken(token, isSynthPreset);
      unsigned tokenIdx = 0;
      for (; tokenIdx < c_tokensCount; ++tokenIdx) {
        if(token == c_tokens[tokenIdx]) {
          break;
        }
      }
      if(tokenIdx >= c_tokensCount) {
#ifdef PRESET_DEBUG
        Serial.println("Token not found");
#endif // PRESET_DEBUG
        return false;
      }
      if(cursor == data.length()) {
#ifdef PRESET_DEBUG
        Serial.println("Missing value!");
#endif // PRESET_DEBUG
        return false;
      }
      ParameterValues values;
      if (0 < ParseNumberTokens(data, cursor, values)) {
        _outStripIndex = stripIndex;
        _outParmIndex = tokenIdx;
        _outParmValues = values;
#ifdef PRESET_DEBUG
        Serial.printf("%d - %s - %u - %f\n", stripIndex, token.c_str(), tokenIdx, values.data_[0]);
#endif // PRESET_DEBUG
        return true;
      }
    }
  }
  return false;
}

void ParseBinaryPresets(const BinaryPreset * data_1, const BinaryPreset * data_2, const unsigned presetLength, FM4 & synth, Sequencer & seq) {
#ifdef PRESET_DEBUG
    Serial.printf("ParseBinaryPresets: %d presets\n", presetLength);
#endif // PRESET_DEBUG

  const BinaryPreset * allPresets[] = {data_1, data_2};
  for (int presetIndex = 0; presetIndex < 2; ++presetIndex) {
    const BinaryPreset * currentPreset = allPresets[presetIndex];
    unsigned itemIndex = 0;

    while (itemIndex < presetLength) {
      const BinaryPreset * presetItem = currentPreset + itemIndex;

      int synthStripIndex = presetItem->SynthStripIndex_ - 1;
      unsigned parmIndex = presetItem->ParameterIndex_;
      ParameterValues parmValues(presetItem->ParameterValuesCount_, presetItem->ParameterValues_);
#ifdef PRESET_DEBUG
      //parmValues.dump();
#endif // PRESET_DEBUG
      if (parmIndex >= SynthParameterIndex::synth_Count) {
        const SequencerParameterIndex seqParmIndex = static_cast<SequencerParameterIndex>(parmIndex - SynthParameterIndex::synth_Count);
        seq.setIndexedParameter(presetIndex, seqParmIndex, parmValues);
      } else {
        const SynthParameterIndex synthParmIndex = static_cast<SynthParameterIndex>(parmIndex);
        // For now the synth has no multi-values parameters
        synth.setIndexedParameter(synthStripIndex, presetIndex, synthParmIndex, parmValues.data_[0]);
        synth.applyParms();
      }
      itemIndex += 1;
    }
  }
}

#endif // _PRESETS_H_

