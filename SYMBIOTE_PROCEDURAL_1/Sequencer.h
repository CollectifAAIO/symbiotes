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

#include "FM4_synth.h"

//#define SEQ_DEBUG

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
    unsigned bpm = 120,
    unsigned stepsCount = 5,
    bool isLooping = false,
    unsigned octave = 6) :
    bpm_(bpm),
    stepsCount_(stepsCount),
    isLooping_(isLooping),
    octave_(octave),
    arpeg_() {
      arpeg_.data_[0] = 16.2;
      arpeg_.data_[1] = 10.2;
      arpeg_.data_[2] = 6.2;
      arpeg_.data_[3] = 2.2;
      arpeg_.data_[4] = 0.2;
  }

  void setIndexedParameter(const SequencerParameterIndex parmIndex, const ParameterValues & parmValues ) {
#ifdef SEQ_DEBUG
    Serial.printf("Sequencer: set parameter %d\n", (int)parmIndex);
#endif // SEQ_DEBUG
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
  isStarted_(false),
  isNoteOn_(false),
  periodMs_(0),
  parms_() {}

  void start() {
    timer_ = 0;
    stepsCounter_ = 0;
    isNoteOn_ = false;
    isStarted_ = true;
  }

  void update(FM4 & synth_) {
    if (timer_ >= periodMs_) {
      if(stepsCounter_ < parms_.stepsCount_ || parms_.isLooping_) {
        if (isStarted_ && !isNoteOn_) {
          noteOn(synth_);
        } else {
          noteOff(synth_);
        }
#ifdef SEQ_DEBUG
        Serial.printf("Steps count: %d\n", stepsCounter_);
#endif // SEQ_DEBUG
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
    const float midiNote = computeNextNote();
#ifdef SEQ_DEBUG
    Serial.printf("noteOn: %f\n", midiNote);
#endif // SEQ_DEBUG
    synth_.noteOn(midiNote);
    applyParms();
    isNoteOn_ = true;
    timer_ = 0;
    stepsCounter_ += 1;
    stepsCounter_ = stepsCounter_ % parms_.stepsCount_;
    isStarted_ = stepsCounter_ != 0;
  }

  void noteOff(FM4 & synth_) {
    synth_.noteOff();
    isNoteOn_ = false;
    timer_ = 0;
  }

 private:
  float computeNextNote() const {
    const float arpegValue = parms_.arpeg_.data_[stepsCounter_];
    const float midiValue = arpegValue + 12.0 * (11.0 - parms_.octave_);
    return midiValue;
  }

  elapsedMillis timer_;
  unsigned stepsCounter_;
  bool isStarted_;
  bool isNoteOn_;
  unsigned periodMs_;
  SequencerParms parms_;
};

#endif // _SEQUENCER_H_

