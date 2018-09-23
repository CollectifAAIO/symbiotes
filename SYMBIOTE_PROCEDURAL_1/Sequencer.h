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
    octave_(octave),
    arpeg_() {
  }

  void setIndexedParameter(const SequencerParameterIndex parmIndex, const ParameterValues & parmValues ) {
    Serial.printf("Sequencer: set parameter %d\n", (int)parmIndex);
    switch(parmIndex) {
    case seq_randomizeSeqOrSound:{
      break;
    }
    case seq_loop:{
      isLooping_ = parmValues.data_[0] > 0.0f;
      break;
    }
    case seq_bpm:{
      bpm_ = static_cast<unsigned>(parmValues.data_[0]);
      break;
    }
    case seq_RestartFrom0:{
      break;
    }
    case seq_StepNumber:{
      stepsCount_ = static_cast<unsigned>(parmValues.data_[0]);
      break;
    }
    case seq_RandomSpeed:{
      break;
    }
    case seq_octave:{
      octave_ = static_cast<unsigned>(parmValues.data_[0]);
      break;
    }
    case seq_arpeg:{
      arpeg_ = parmValues;
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
  ParameterValues arpeg_;
};

class Sequencer {
 public:
  Sequencer()
  : timer_(0),
  stepsCounter_(0),
  isNoteOn_(false),
  periodMs_(0),
  parms_() {}

  void start() {
    timer_ = 0;
    stepsCounter_ = 0;
    isNoteOn_ = false;
  }

  void update(FM4 & synth_) {
    if (timer_ >= periodMs_) {
      if(stepsCounter_ <= parms_.stepsCount_ || parms_.isLooping_) {
        if (!isNoteOn_) {
          noteOn(synth_);
        } else {
          noteOff(synth_);
        }
        Serial.printf("Steps count: %d\n", stepsCounter_);
      } else if(isNoteOn_) {
        noteOff(synth_);
      }
    }
  }

  void applyParms() {
    periodMs_ = static_cast<unsigned>(1000.0 * 60.0 / static_cast<float>(parms_.bpm_));
  }

  void setIndexedParameter(const SequencerParameterIndex parmIndex, const ParameterValues & parmValues ) {
    parms_.setIndexedParameter(parmIndex, parmValues);
  }

  void noteOn(FM4 & synth_) {
    applyParms();
    synth_.noteOn();
    isNoteOn_ = true;
    timer_ = 0;
    stepsCounter_ += 1;
  }

  void noteOff(FM4 & synth_) {
    synth_.noteOff();
    isNoteOn_ = false;
    timer_ = 0;
    stepsCounter_ += 1;
  }

 private:
  elapsedMillis timer_;
  unsigned stepsCounter_;
  bool isNoteOn_;
  unsigned periodMs_;
  SequencerParms parms_;
};

#endif // _SEQUENCER_H_

