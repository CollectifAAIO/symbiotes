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
#include "helpers.h"

//#define SEQ_DEBUG

enum SequencerParameterIndex {
  seq_randomizeSeqOrSound,
  seq_loop,
  seq_bpm,
  seq_RestartFrom0,
  seq_StepNumber,
  seq_RandomSpeed,
  seq_DutyCycle,
  seq_arpeg,
  seq_Trigers,
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
    float DutyCycle = 0.5f) :
    bpm_(bpm),
    stepsCount_(stepsCount),
    isLooping_(isLooping),
    DutyCycle_(DutyCycle),
    arpeg_(),
    Trigers_() {
      arpeg_.data_[0] = 16.2;
      arpeg_.data_[1] = 10.2;
      arpeg_.data_[2] = 6.2;
      arpeg_.data_[3] = 2.2;
      arpeg_.data_[4] = 0.2;
      Trigers_.data_[0] = 1;
      Trigers_.data_[1] = 0;
      Trigers_.data_[2] = 1;
      Trigers_.data_[3] = 0;
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
      bpm_ = 4.0f * static_cast<unsigned>(parmValues.data_[0]);
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
    case seq_DutyCycle:{
      DutyCycle_ = parmValues.data_[0];
      break;
    }
    case seq_arpeg:{
      arpeg_ = parmValues;
      break;
    }
    case seq_Trigers:{
      Trigers_ = parmValues;
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

  void dump() const {
    Serial.printf("bpm_: %d; stepsCount_: %d; isLooping_: %d; DutyCycle_: %f; arpeg_0 1 2 3: %f %f %f %f; Trigers_0 1 2 3: %f %f %f %f;\n",
                  bpm_, stepsCount_, isLooping_, DutyCycle_, arpeg_.data_[0], arpeg_.data_[1], arpeg_.data_[2], arpeg_.data_[3], Trigers_.data_[0], Trigers_.data_[1], Trigers_.data_[2], Trigers_.data_[3]);
  }

  unsigned bpm_;
  unsigned stepsCount_;
  bool isLooping_;
  float DutyCycle_;
  ParameterValues arpeg_;
  ParameterValues Trigers_;
};

struct SequencerParmsInstance {
  SequencerParmsInstance(
    unsigned bpm = 120,
    unsigned stepsCount = 5,
    bool isLooping = false,
    float DutyCycle = 0.5f,
    const ParameterValues & arpeg = {},
    const ParameterValues & Trigers = {}) :
    bpm_(bpm),
    stepsCount_(stepsCount),
    isLooping_(isLooping),
    DutyCycle_(DutyCycle),
    arpeg_(arpeg),
    Trigers_(Trigers) {
  }

  void dump() const {
    Serial.printf("bpm_: %d; stepsCount_: %d; isLooping_: %d; DutyCycle_: %f; arpegs: %f %f %f %f %f %f %f %f; Trigers: %f %f %f %f %f %f %f %f;\n",
                  bpm_, stepsCount_, isLooping_, DutyCycle_,
                  arpeg_.data_[0], arpeg_.data_[1], arpeg_.data_[2], arpeg_.data_[3], arpeg_.data_[4], arpeg_.data_[5], arpeg_.data_[6], arpeg_.data_[7],
                  Trigers_.data_[0], Trigers_.data_[1], Trigers_.data_[2], Trigers_.data_[3], Trigers_.data_[4], Trigers_.data_[5], Trigers_.data_[6], Trigers_.data_[7]);
  }

  void LerpWith(const SequencerParmsInstance & rhs, const float interpolationFactor) {
    bpm_ = Lerp(bpm_, rhs.bpm_, interpolationFactor);
    stepsCount_ = Lerp(stepsCount_, rhs.stepsCount_, interpolationFactor);
    isLooping_ = Lerp(isLooping_, rhs.isLooping_, interpolationFactor);
    DutyCycle_ = Lerp(DutyCycle_, rhs.DutyCycle_, interpolationFactor);
    arpeg_ = Lerp(arpeg_, rhs.arpeg_, interpolationFactor);
    Trigers_ = Lerp(Trigers_, rhs.Trigers_, interpolationFactor);
  }
  unsigned bpm_;
  unsigned stepsCount_;
  bool isLooping_;
  float DutyCycle_;
  ParameterValues arpeg_;
  ParameterValues Trigers_;
};

class Sequencer {
 public:
  Sequencer()
  : timer_(0),
  stepsCounter_(0),
  cyclesCounter_(0),
  isNoteOn_(false),
  periodMs_(0),
  parms_() {}

  void start() {
    timer_ = 0;
    stepsCounter_ = 0;
    cyclesCounter_ = 0;
    isNoteOn_ = false;
  }

  void update(FM4 & synth_) {
    if (timer_ >= periodMs_) {
      if(cyclesCounter_ == 0 || parms_.isLooping_) {
        if(parms_.Trigers_.data_[stepsCounter_] > 0.0f) {
            noteOn(synth_);
        } else {
          noteOff(synth_);
        }
#ifdef SEQ_DEBUG
        Serial.printf("Cycles count %d - Steps count %d\n", cyclesCounter_, stepsCounter_);
#endif // SEQ_DEBUG
        stepsCounter_ += 1;
        stepsCounter_ = stepsCounter_ % parms_.stepsCount_;
        if (stepsCounter_ == 0) {
          cyclesCounter_ += 1;
        }
        timer_ = 0;
        noteOffTime_ = static_cast<unsigned>(static_cast<float>(periodMs_) * parms_.DutyCycle_);
      }
    } else if (timer_ >= noteOffTime_) {
      if (isNoteOn_) {
        noteOff(synth_);
      }
    }
  }

  void setAllParms(const SequencerParmsInstance & parms) {
    parms_ = parms;
  }

  void applyParms() {
    periodMs_ = static_cast<unsigned>(1000.0 * 60.0 / static_cast<float>(parms_.bpm_));
  }

  void setInterpolationFactor(const float value) {
    // In the case of the Sequencer everything is applied instantly
    SequencerParmsInstance newInstances[c_templatesCount];
    for (unsigned i = 0; i < c_templatesCount; ++i) {
      newInstances[i] = SequencerParmsInstance(
        parmsTemplate_[i].bpm_,
        parmsTemplate_[i].stepsCount_,
        parmsTemplate_[i].isLooping_,
        parmsTemplate_[i].DutyCycle_,
        parmsTemplate_[i].arpeg_,
        parmsTemplate_[i].Trigers_);
    }
    SequencerParmsInstance morphed(newInstances[0]);
    morphed.LerpWith(newInstances[1], value);
    setAllParms(morphed);
  }

  void setIndexedParameter(const unsigned templateIndex, const SequencerParameterIndex parmIndex, const ParameterValues & parmValues ) {
    parmsTemplate_[templateIndex].setIndexedParameter(parmIndex, parmValues);
  }

  void noteOn(FM4 & synth_) {
    const float midiNote = computeNextNote();
    synth_.noteOn(midiNote);
    applyParms();
    isNoteOn_ = true;
#ifdef SEQ_DEBUG
    Serial.printf("noteOn: %f - expected note off time: %d\n", midiNote, noteOffTime_);
#endif // SEQ_DEBUG
  }

  void noteOff(FM4 & synth_) {
    synth_.noteOff();
    isNoteOn_ = false;
  }

  void dumpParms() const {
    parms_.dump();
    Serial.printf("periodMs: %u, noteOffTime: %u\n",
                  periodMs_, noteOffTime_);
  }

 private:
  static constexpr unsigned c_templatesCount = 2;
  float computeNextNote() const {
    const float arpegValue = static_cast<float>(static_cast<int>(parms_.arpeg_.data_[stepsCounter_]));
    const float midiValue = arpegValue + 12.0;
    return midiValue;
  }

  elapsedMillis timer_;
  unsigned noteOffTime_;
  unsigned stepsCounter_;
  unsigned cyclesCounter_;
  bool isNoteOn_;
  unsigned periodMs_;
  SequencerParmsInstance parms_;
  SequencerParms parmsTemplate_[c_templatesCount];
};

#endif // _SEQUENCER_H_

