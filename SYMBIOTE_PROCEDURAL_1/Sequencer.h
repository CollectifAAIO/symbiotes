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

#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

enum SequencerParameterIndex {
  seq_randomizeSeqOrSound,
  seq_loop,
  seq_bpm,
  seq_RestartFrom0,
  seq_StepNumber,
  seq_RandomSpeed,
  seq_octave,
  seq_arpeg,
  seq_random_trig,
  seq_min_trigger_time,
  seq_max_trigger_time,
  seq_Count
};

struct SequencerParms {
  SequencerParms(
    unsigned bpm = 60,
    unsigned stepsCount = 2,
    bool isLooping = false,
    unsigned octave = 0) :
    bpm_(bpm),
    stepsCount_(stepsCount),
    isLooping_(isLooping),
    octave_(octave) {
  }

  void setIndexedParameter(const SequencerParameterIndex parmIndex, const float parmValue ) {
    Serial.printf("Sequencer: set parameter %d\n", (int)parmIndex);
    switch(parmIndex) {
    case seq_randomizeSeqOrSound:{
      break;
    }
    case seq_loop:{
      isLooping_ = parmValue > 0.0f;
      break;
    }
    case seq_bpm:{
      bpm_ = static_cast<unsigned>(parmValue);
      break;
    }
    case seq_RestartFrom0:{
      break;
    }
    case seq_StepNumber:{
      stepsCount_ = static_cast<unsigned>(parmValue);
      break;
    }
    case seq_RandomSpeed:{
      break;
    }
    case seq_octave:{
      octave_ = static_cast<unsigned>(parmValue);
      break;
    }
    case seq_arpeg:{
      break;
    }
    case seq_random_trig:{
      break;
    }
    case seq_min_trigger_time:{
      break;
    }
    case seq_max_trigger_time:{
      break;
    }
    default:{
      Serial.println("Bad parameter index.");
      break;
    }
    }
  }

  unsigned bpm_;
  unsigned stepsCount_;
  bool isLooping_;
  unsigned octave_;
};

class Sequencer {
 public:
  Sequencer()
  : counter_(0),
  isNoteOn_(false),
  periodMs_(0),
  parms_() {}

  void setBpm(unsigned value) {
    parms_.setIndexedParameter(static_cast<SequencerParameterIndex>(seq_bpm), static_cast<float>(value));
  }

  void start() {
    counter_ = 0;
  }

  void update(FM4 & synth_) {
    if (counter_ >= periodMs_) {
      if (!isNoteOn_) {
        applyParms();
        synth_.noteOn();
        isNoteOn_ = true;
        counter_ = 0;
      } else {
        synth_.noteOff();
        isNoteOn_ = false;
        counter_ = 0;
      }
    }
  }

  void applyParms() {
    periodMs_ = static_cast<unsigned>(1000.0 * 60.0 / static_cast<float>(parms_.bpm_));
  }

  void setIndexedParameter(const SequencerParameterIndex parmIndex, const float parmValue ) {
    parms_.setIndexedParameter(parmIndex, parmValue);
  }

 private:
  elapsedMillis counter_;
  bool isNoteOn_;
  unsigned periodMs_;
  SequencerParms parms_;
};

#endif // _SEQUENCER_H_

