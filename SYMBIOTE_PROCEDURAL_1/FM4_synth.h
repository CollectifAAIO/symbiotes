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

#ifndef _FM4_SYNTH_H_
#define _FM4_SYNTH_H_

#include "GlobalVariables.h"
#include "helpers.h"

//#define SYNTH_DEBUG

constexpr char WAVEFORM[8] = {WAVEFORM_SINE, WAVEFORM_SQUARE, WAVEFORM_SAWTOOTH, WAVEFORM_PULSE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH_REVERSE, WAVEFORM_TRIANGLE_VARIABLE, WAVEFORM_SAMPLE_HOLD};
constexpr int WAVEFORM_NOISE = 5; // From the pd patch

enum SynthParameterIndex {
  synth_waveform,
  synth_glide,
  synth_Pitch,
  synth_glide_rand,
  synth_Pitch_rand,
  synth_ListenSeq,
  synth_Transpose,
  synth_Octave,
  synth_FM_Osc1,
  synth_FM_Osc2,
  synth_FM_Osc3,
  synth_FM_Osc4,
  synth_FM_Osc1_rand,
  synth_FM_Osc2_rand,
  synth_FM_Osc3_rand,
  synth_FM_Osc4_rand,
  synth_AM_Waveform,
  synth_AM_Depth,
  synth_AM_Freq,
  synth_AM_Depth_rand,
  synth_AM_Freq_rand,
  synth_PADSR_Dlay,
  synth_PADSR_Amp,
  synth_PADSR_Atk,
  synth_PADSR_Dcay,
  synth_PADSR_Sus,
  synth_PADSR_Rel,
  synth_PADSR_Dlay_rand,
  synth_PADSR_Amp_rand,
  synth_PADSR_Atk_rand,
  synth_PADSR_Dcay_rand,
  synth_PADSR_Sus_rand,
  synth_PADSR_Rel_rand,
  synth_LADSR_Dlay,
  synth_LADSR_Amp,
  synth_LADSR_Atk,
  synth_LADSR_Dcay,
  synth_LADSR_Sus,
  synth_LADSR_Rel,
  synth_LADSR_Dlay_rand,
  synth_LADSR_Amp_rand,
  synth_LADSR_Atk_rand,
  synth_LADSR_Dcay_rand,
  synth_LADSR_Sus_rand,
  synth_LADSR_Rel_rand,
  synth_Vol,
  synth_Count
};

float computeModulationLevelFromExpectedPitch(const float expected, const float base) {
  float out = 0.0f;
  if (expected > 0) {
    out = log(expected / base) / c_modRangeFactor;
  }
  return out;
}

// Struct defining an envelope generator parameters
struct ADSRParms {
  ADSRParms( unsigned AtkMs = 100,
    unsigned DcayMs = 100,
    float Sus = 1.0,
    unsigned RlsMs = 500,
    unsigned DelayMs = 0 ) :
    AtkMs_( AtkMs ),
    DcayMs_( DcayMs ),
    Sus_( Sus ),
    RlsMs_( RlsMs ),
    DelayMs_( DelayMs ) {}

    void applyToADSR( AudioEffectEnvelope & instance ) const {
      instance.attack(AtkMs_);
      instance.decay(DcayMs_);
      instance.sustain(Sus_);
      instance.release(RlsMs_);
      instance.delay(DelayMs_);
    }

    void setIndexedParameter(const int parmIndex, const float parmValue ) {
      // Note there is a mismatch between Arduino and the pd patch parameters orde
      // The pd patch saves as follows:
      // 0 - Delay
      // 1 - Amplitude (which does not exist in Teensy ADSR)
      // 2 - Attack
      // 3 - Decay
      // 4 - Sustain
      // 5 - Release
      switch(parmIndex) {
      case 0:{
          DelayMs_ = parmValue;
          break;
        }
      case 1:{
          // Nothing here!
          break;
        }
      case 2:{
          AtkMs_ = parmValue;
          break;
        }
      case 3:{
          DcayMs_ = parmValue;
          break;
        }
      case 4:{
          Sus_ = parmValue;
          break;
        }
      case 5:{
          RlsMs_ = parmValue;
          break;
        }
      default: {
        Serial.printf("ADSR: Bad parameter index %d.\n", parmIndex);
        break;
      }
      }
    }

    void dump() const {
      Serial.printf("AtkMs_: %d; DcayMs_: %d; Sus_: %f; RlsMs_: %d; DelayMs_: %d\n",
                    AtkMs_, DcayMs_, Sus_, RlsMs_, DelayMs_);
    }

    unsigned AtkMs_;
    unsigned DcayMs_;
    float Sus_;
    unsigned RlsMs_;
    unsigned DelayMs_;
};

static ADSRParms Lerp(const ADSRParms & lhs, const ADSRParms & rhs, const float interpolationFactor) {
  return ADSRParms(
    Lerp(lhs.AtkMs_, rhs.AtkMs_, interpolationFactor),
    Lerp(lhs.DcayMs_, rhs.DcayMs_, interpolationFactor),
    Lerp(lhs.Sus_, rhs.Sus_, interpolationFactor),
    Lerp(lhs.RlsMs_, rhs.RlsMs_, interpolationFactor),
    Lerp(lhs.DelayMs_, rhs.DelayMs_, interpolationFactor));
}

ADSRParms getRandom(const ADSRParms & meanADSR, const ADSRParms & halfRangeADSR) {
  ADSRParms out;
  out.AtkMs_ = getRandom(meanADSR.AtkMs_, halfRangeADSR.AtkMs_);
  out.DcayMs_ = getRandom(meanADSR.DcayMs_, halfRangeADSR.DcayMs_);
  out.Sus_ = getRandom(meanADSR.Sus_, halfRangeADSR.Sus_);
  out.RlsMs_ = getRandom(meanADSR.RlsMs_, halfRangeADSR.RlsMs_);
  out.DelayMs_ = getRandom(meanADSR.DelayMs_, halfRangeADSR.DelayMs_);
  return out;
}

// Parameters that are directly set from the presets
struct SynthStripParms {
  SynthStripParms(
    unsigned WaveformOSC = 0,
    const ADSRParms & VolParms = ADSRParms( 1, 200, 0.2, 1000, 0 ),
    unsigned FreqOsc = 440,
    bool ListenSeq = false,
    unsigned Transpose = 0,
    unsigned Octave = 0,
    float PitchDepth = 0.2,
    const ADSRParms & PitchParms = ADSRParms( 100, 220, 0.0, 200, 100 ),
    float FMOsc1toOsc = 0.4,
    float FMOsc2toOsc = 0.0,
    float FMOsc3toOsc = 0.0,
    float FMOsc4toOsc = 0.0,
    float AMdepth = 0,
    unsigned AMFreq = 4,
    unsigned WaveformAM = 0 )
    :
    WaveformOSC_( WaveformOSC ),
    VolParms_( VolParms ),
    VolParmsRand_{},
    FreqOsc_( FreqOsc ),
    FreqOscRand_{},
    ListenSeq_( ListenSeq ),
    Transpose_( Transpose ),
    Octave_( Octave ),
    PitchDepth_( PitchDepth ),
    PitchDepthRand_{},
    PitchParms_( PitchParms ),
    PitchParmsRand_{},
    FMOsc1toOsc_( FMOsc1toOsc ),
    FMOsc1toOscRand_{},
    FMOsc2toOsc_( FMOsc2toOsc ),
    FMOsc2toOscRand_{},
    FMOsc3toOsc_( FMOsc3toOsc ),
    FMOsc3toOscRand_{},
    FMOsc4toOsc_( FMOsc4toOsc ),
    FMOsc4toOscRand_{},
    AMdepth_( AMdepth ),
    AMdepthRand_{},
    AMFreq_( AMFreq ),
    AMFreqRand_{},
    WaveformAM_( WaveformAM ) {}

  void setIndexedParameter(const SynthParameterIndex parmIndex, const float parmValue ) {
    switch(parmIndex) {
    case synth_waveform:{
      WaveformOSC_ = static_cast<unsigned>(parmValue);
      break;
    }
    case synth_glide:{
      break;
    }
    case synth_Pitch:{
      FreqOsc_ = parmValue;
      break;
    }
    case synth_glide_rand:
    case synth_Pitch_rand:{
      FreqOscRand_ = parmValue;
      break;
    }
    case synth_ListenSeq:{
      ListenSeq_ = parmValue > 0.0f;
      break;
    }
    case synth_Transpose:{
      Transpose_ = static_cast<unsigned>(parmValue);
      break;
    }
    case synth_Octave:{
      Octave_ = static_cast<unsigned>(parmValue);
      break;
    }
    case synth_FM_Osc1:{
      FMOsc1toOsc_ = parmValue;
      break;
    }
    case synth_FM_Osc2:{
      FMOsc2toOsc_ = parmValue;
      break;
    }
    case synth_FM_Osc3:{
      FMOsc3toOsc_ = parmValue;
      break;
    }
    case synth_FM_Osc4:{
      FMOsc4toOsc_ = parmValue;
      break;
    }
    case synth_FM_Osc1_rand:{
      FMOsc1toOscRand_ = parmValue;
      break;
    }
    case synth_FM_Osc2_rand:{
      FMOsc2toOscRand_ = parmValue;
      break;
    }
    case synth_FM_Osc3_rand:{
      FMOsc3toOscRand_ = parmValue;
      break;
    }
    case synth_FM_Osc4_rand:{
      FMOsc4toOscRand_ = parmValue;
      break;
    }
    case synth_AM_Waveform:{
      WaveformAM_ = parmValue;
      break;
    }
    case synth_AM_Depth:{
      AMdepth_ = parmValue;
      break;
    }
    case synth_AM_Freq:{
      AMFreq_ = parmValue;
      break;
    }
    case synth_AM_Depth_rand:{
      AMdepthRand_ = parmValue;
      break;
    }
    case synth_AM_Freq_rand:{
      AMFreqRand_ = parmValue;
      break;
    }
    case synth_PADSR_Dlay:
    case synth_PADSR_Atk:
    case synth_PADSR_Dcay:
    case synth_PADSR_Sus:
    case synth_PADSR_Rel:{
      const int adsrParmIndex = parmIndex - synth_PADSR_Dlay;
      PitchParms_.setIndexedParameter(adsrParmIndex, parmValue);
      break;
    }
    case synth_PADSR_Amp:{
      PitchDepth_ = parmValue;
      break;
    }
    case synth_PADSR_Amp_rand:{
      PitchDepthRand_ = parmValue;
      break;
    }
    case synth_PADSR_Dlay_rand:
    case synth_PADSR_Atk_rand:
    case synth_PADSR_Dcay_rand:
    case synth_PADSR_Sus_rand:
    case synth_PADSR_Rel_rand:{
      const int adsrParmIndex = parmIndex - synth_PADSR_Dlay_rand;
      PitchParmsRand_.setIndexedParameter(adsrParmIndex, parmValue);
      break;
    }
    case synth_LADSR_Dlay:
    case synth_LADSR_Amp:
    case synth_LADSR_Atk:
    case synth_LADSR_Dcay:
    case synth_LADSR_Sus:
    case synth_LADSR_Rel:{
      const int adsrParmIndex = parmIndex - synth_LADSR_Dlay;
      VolParms_.setIndexedParameter(adsrParmIndex, parmValue);
      break;
    }
    case synth_LADSR_Dlay_rand:
    case synth_LADSR_Amp_rand:
    case synth_LADSR_Atk_rand:
    case synth_LADSR_Dcay_rand:
    case synth_LADSR_Sus_rand:
    case synth_LADSR_Rel_rand:{
      const int adsrParmIndex = parmIndex - synth_LADSR_Dlay_rand;
      VolParmsRand_.setIndexedParameter(adsrParmIndex, parmValue);
      break;
    }
    case synth_Vol:{
      break;
    }
    default:{
        Serial.printf("Synth: Bad parameter index %d.\n", parmIndex);
      break;
    }
    }
  }

  void dump() const {
    Serial.println("Vol/Pitch parms");
    VolParms_.dump();
    PitchParms_.dump();
    Serial.println("Vol/Pitch random parms");
    VolParmsRand_.dump();
    PitchParmsRand_.dump();
    Serial.printf("WaveformOSC_: %d; FreqOsc_: %d; ListenSeq_: %d; Transpose_: %d; Octave_: %d; PitchDepth_: %f; FMOsc1toOsc_: %f; FMOsc2toOsc_: %f; FMOsc3toOsc_: %f; FMOsc4toOsc_: %f; AMdepth_: %f; AMFreq_: %d; WaveformAM_: %d;\n",
                  WaveformOSC_, FreqOsc_, ListenSeq_, Transpose_, Octave_, PitchDepth_, FMOsc1toOsc_, FMOsc2toOsc_, FMOsc3toOsc_, FMOsc4toOsc_, AMdepth_, AMFreq_, WaveformAM_);
    Serial.printf("FreqOscRand_: %d; FMOsc1toOscRand_: %f; FMOsc2toOscRand_: %f; FMOsc3toOscRand_: %f; FMOsc4toOscRand_: %f; AMdepthRand_: %f; AMFreqRand_: %d;\n",
                  FreqOscRand_, FMOsc1toOscRand_, FMOsc2toOscRand_, FMOsc3toOscRand_, FMOsc4toOscRand_, AMdepthRand_, AMFreqRand_);
  }

  // Only accessible through getters so we can implement randomness etc.
  unsigned WaveformOSC() const {
    return WaveformOSC_;
  }
  ADSRParms VolParms() const {
    return getRandom(VolParms_, VolParmsRand_);
  }
  float FreqOsc(const float freqHz) const {
    if(ListenSeq_) {
      const float pitchedNoteHz = (freqHz + Transpose_) * pow(2.0f, Octave_);
      return pitchedNoteHz;
    } else {
      return FreqOsc_;
    }
  }
  bool ListenSeq() const {
    return ListenSeq_;
  }
  float PitchDepth() const {
    return getRandom(PitchDepth_, PitchDepthRand_);
  }
  ADSRParms PitchParms() const {
    return getRandom(PitchParms_, PitchParmsRand_);
  }
  float FMOsc1toOsc() const {
    return getRandom(FMOsc1toOsc_, FMOsc1toOscRand_);
  }
  float FMOsc2toOsc() const {
    return getRandom(FMOsc2toOsc_, FMOsc2toOscRand_);
  }
  float FMOsc3toOsc() const {
    return getRandom(FMOsc3toOsc_, FMOsc3toOscRand_);
  }
  float FMOsc4toOsc() const {
    return getRandom(FMOsc4toOsc_, FMOsc4toOscRand_);
  }
  float AMdepth() const {
    return getRandom(AMdepth_, AMdepthRand_);
  }
  float AMFreq() const {
    return getRandom(AMFreq_, AMFreqRand_);
  }
  unsigned WaveformAM() const {
    return WaveformAM_;
  }

 private:
  unsigned WaveformOSC_;
  ADSRParms VolParms_;
  ADSRParms VolParmsRand_;
  unsigned FreqOsc_;
  float FreqOscRand_;
  bool ListenSeq_;
  unsigned Transpose_;
  unsigned Octave_;
  float PitchDepth_;
  float PitchDepthRand_;
  ADSRParms PitchParms_;
  ADSRParms PitchParmsRand_;
  float FMOsc1toOsc_;
  float FMOsc1toOscRand_;
  float FMOsc2toOsc_;
  float FMOsc2toOscRand_;
  float FMOsc3toOsc_;
  float FMOsc3toOscRand_;
  float FMOsc4toOsc_;
  float FMOsc4toOscRand_;
  float AMdepth_;
  float AMdepthRand_;
  unsigned AMFreq_;
  unsigned AMFreqRand_;
  unsigned WaveformAM_;
};

// The actual parameters to be applied on the audio objects as is
// Qualified as "instance" because it's one specific instantiation of parmeters with "meta" associated
// Such metas include their randomness or dependencies (e.g. modifying a pitch changes other parameters normalised with the pitch)
struct SynthStripParmsInstance {
  SynthStripParmsInstance(
    unsigned WaveformOSC = 0,
    const ADSRParms & VolParms = ADSRParms( 1, 200, 0.2, 1000, 0 ),
    unsigned FreqOsc = 440,
    bool ListenSeq = false,
    float PitchDepth = 0.2,
    const ADSRParms & PitchParms = ADSRParms( 100, 220, 0.0, 200, 100 ),
    float FMOsc1toOsc = 0.4,
    float FMOsc2toOsc = 0.0,
    float FMOsc3toOsc = 0.0,
    float FMOsc4toOsc = 0.0,
    float AMdepth = 0,
    unsigned AMFreq = 4,
    unsigned WaveformAM = 0 )
    :
    WaveformOSC_( WaveformOSC ),
    VolParms_( VolParms ),
    FreqOsc_( FreqOsc ),
    ListenSeq_( ListenSeq ),
    PitchDepth_( PitchDepth ),
    PitchParms_( PitchParms ),
    FMOsc1toOsc_( FMOsc1toOsc ),
    FMOsc2toOsc_( FMOsc2toOsc ),
    FMOsc3toOsc_( FMOsc3toOsc ),
    FMOsc4toOsc_( FMOsc4toOsc ),
    AMdepth_( AMdepth ),
    AMFreq_( AMFreq ),
    WaveformAM_( WaveformAM ) {}

  // OSCs SHAPE
  unsigned WaveformOSC_;       // Waveform selected in the array between 0 & 7.

  // OSC VOLUME

  ADSRParms VolParms_;

  // OSC PITCH

  // Note
  unsigned FreqOsc_;         // frequency of OSC1.

  bool ListenSeq_;

  float PitchDepth_;         // Depth of Pitch enveloppe (between 0.0 & 1.0)
  ADSRParms PitchParms_;

  // MODULATIONS

  // FM

  // Osc
  float FMOsc1toOsc_;     // Depth of FM from OSC1
  float FMOsc2toOsc_;     // Depth of FM from OSC2
  float FMOsc3toOsc_;     // Depth of FM from OSC3
  float FMOsc4toOsc_;     // Depth of FM from OSC4

  // AM
  float AMdepth_;      // Mix between amp

  unsigned AMFreq_;         // Frequency of Amplitude Modulation (Hz)
  unsigned WaveformAM_;      // Waveform of AM (Waveform selected in the array between 0 & 7.)

  void dump() const {
    VolParms_.dump();
    PitchParms_.dump();
    Serial.printf("WaveformOSC_: %d; FreqOsc_: %d; ListenSeq_: %d; PitchDepth_: %f; FMOsc1toOsc_: %f; FMOsc2toOsc_: %f; FMOsc3toOsc_: %f; FMOsc4toOsc_: %f; AMdepth_: %f; AMFreq_: %d; WaveformAM_: %d;\n",
                  WaveformOSC_, FreqOsc_, ListenSeq_, PitchDepth_, FMOsc1toOsc_, FMOsc2toOsc_, FMOsc3toOsc_, FMOsc4toOsc_, AMdepth_, AMFreq_, WaveformAM_);
  }

  void LerpWith(const SynthStripParmsInstance & rhs, const float interpolationFactor) {
    WaveformOSC_ = Lerp(WaveformOSC_, rhs.WaveformOSC_, interpolationFactor);
    VolParms_ = Lerp(VolParms_, rhs.VolParms_, interpolationFactor);
    FreqOsc_ = Lerp(FreqOsc_, rhs.FreqOsc_, interpolationFactor);
    ListenSeq_ = Lerp(ListenSeq_, rhs.ListenSeq_, interpolationFactor);
    PitchDepth_ = Lerp(PitchDepth_, rhs.PitchDepth_, interpolationFactor);
    PitchParms_ = Lerp(PitchParms_, rhs.PitchParms_, interpolationFactor);
    FMOsc1toOsc_ = Lerp(FMOsc1toOsc_, rhs.FMOsc1toOsc_, interpolationFactor);
    FMOsc2toOsc_ = Lerp(FMOsc2toOsc_, rhs.FMOsc2toOsc_, interpolationFactor);
    FMOsc3toOsc_ = Lerp(FMOsc3toOsc_, rhs.FMOsc3toOsc_, interpolationFactor);
    FMOsc4toOsc_ = Lerp(FMOsc4toOsc_, rhs.FMOsc4toOsc_, interpolationFactor);
    AMdepth_ = Lerp(AMdepth_, rhs.AMdepth_, interpolationFactor);
    AMFreq_ = Lerp(AMFreq_, rhs.AMFreq_, interpolationFactor);
    WaveformAM_ = Lerp(WaveformAM_, rhs.WaveformAM_, interpolationFactor);
  }
};

// One synth strip, wrapped for easier global changes
struct SynthStrip {
  SynthStrip(AudioSynthWaveformDc &     PitchEnvDepthOsc,
  AudioEffectEnvelope &      PitchEnvOsc,
  AudioMixer4 &              mixerOSCtoOSC,
  AudioMixer4 &              mixerOSC,
  AudioSynthWaveformModulated & OSC,
  AudioSynthWaveform &       WaveAM,
  AudioSynthWaveformDc &     AMdc,
  AudioEffectEnvelope &      VolEnvOsc,
  AudioMixer4 &              mixerAM,
  AudioEffectMultiply &      AM) :
  parms_{},
  parmsTemplate_{},
  PitchEnvDepthOsc_(PitchEnvDepthOsc),
  PitchEnvOsc_(PitchEnvOsc),
  mixerOSCtoOSC_(mixerOSCtoOSC),
  mixerOSC_(mixerOSC),
  OSC_(OSC),
  WaveAM_(WaveAM),
  AMdc_(AMdc),
  VolEnvOsc_(VolEnvOsc),
  mixerAM_(mixerAM),
  AM_(AM)
  {}

  void initialise() {
    AudioNoInterrupts();

    OSC_.amplitude(1.0);
    OSC_.frequencyModulation(c_modRange);
    // Set null values for parameters we won't touch
    VolEnvOsc_.hold( 0 );
    PitchEnvOsc_.hold( 0 );
    AMdc_.amplitude(1);
    WaveAM_.begin(1, 4, WAVEFORM_TRIANGLE);
    mixerOSC_.gain(1, 1);    // Importance of pitch enveloppe in the modulation
    mixerOSC_.gain(2, 0);  // Depth of FM OSCILLATORS

    AudioInterrupts();
  }

  void applyParms() {
    AudioNoInterrupts();

    // COMMAND SYNTH

    // OSCs SHAPE
    if (parms_.WaveformOSC_ == WAVEFORM_NOISE) {
      //OSC_.amplitude(0.0);
      mixerOSC_.gain(0, 0.0);
      mixerOSC_.gain(2, 1.0);
    } else {
      OSC_.amplitude(1.0);
      mixerOSC_.gain(0, 1.0);
      mixerOSC_.gain(2, 0.0);
      OSC_.begin(WAVEFORM[parms_.WaveformOSC_]);
    }

    // OSC VOLUME

    // Envelop
    parms_.VolParms_.applyToADSR(VolEnvOsc_);

    // OSC PITCH

    // Note
    OSC_.frequency(parms_.FreqOsc_);

    // Envelop
    PitchEnvDepthOsc_.amplitude(parms_.PitchDepth_); // Depth of Pitch enveloppe
    parms_.PitchParms_.applyToADSR( PitchEnvOsc_ );

    // MODULATION

    // FM

    // Osc
    mixerOSCtoOSC_.gain(0, parms_.FMOsc1toOsc_);     // Depth of FM from OSC1
    mixerOSCtoOSC_.gain(1, parms_.FMOsc2toOsc_);     // Depth of FM from OSC2
    mixerOSCtoOSC_.gain(2, parms_.FMOsc3toOsc_);     // Depth of FM from OSC3
    mixerOSCtoOSC_.gain(3, parms_.FMOsc4toOsc_);     // Depth of FM from OSC4

    // AM
    mixerAM_.gain(0, parms_.AMdepth_);
    mixerAM_.gain(1, 1.0 - parms_.AMdepth_);

    WaveAM_.begin(1, parms_.AMFreq_, WAVEFORM[parms_.WaveformAM_]); // Waveform & Rate of AM

    AudioInterrupts();
  }

  void setAllParms(const SynthStripParmsInstance & parms) {
    parms_ = parms;
  }

  void setIndexedParameter(const unsigned templateIndex, const SynthParameterIndex parmIndex, const float parmValue ) {
    parmsTemplate_[templateIndex].setIndexedParameter(parmIndex, parmValue);
#ifdef SYNTH_DEBUG
    Serial.printf("Parm %d - %f\n", parmIndex, parmValue);
#endif // SYNTH_DEBUG
  }

  void noteOn(const float noteFreqHz, const float interpolationFactor) {
    instantiateParms(noteFreqHz, interpolationFactor);
    applyParms();

    VolEnvOsc_.noteOn();
    PitchEnvOsc_.noteOn();
  }

  void noteOff() {
    VolEnvOsc_.noteOff();
    PitchEnvOsc_.noteOff();
  }

  void dump() const {
    parms_.dump();
    parmsTemplate_[0].dump();
  }
 private:
  static constexpr unsigned c_templatesCount = 2;

  void instantiateParms(const float noteFreqHz, const float interpolationFactor) {
    SynthStripParmsInstance newInstances[c_templatesCount];
    for (unsigned i = 0; i < c_templatesCount; ++i) {
      // Handling parameters dependencies here
      const float FreqOsc = parmsTemplate_[i].FreqOsc(noteFreqHz);
      const float PitchDepth = computeModulationLevelFromExpectedPitch(parmsTemplate_[i].PitchDepth(), FreqOsc);
      ADSRParms PitchParms = parmsTemplate_[i].PitchParms();
      PitchParms.Sus_ = computeModulationLevelFromExpectedPitch(PitchParms.Sus_, FreqOsc);

#ifdef SYNTH_DEBUG
      Serial.printf("New freq %f\n", FreqOsc);
#endif // SYNTH_DEBUG

      newInstances[i] = SynthStripParmsInstance(
        parmsTemplate_[i].WaveformOSC(),
        parmsTemplate_[i].VolParms(),
        FreqOsc,
        parmsTemplate_[i].ListenSeq(),
        PitchDepth,
        PitchParms,
        parmsTemplate_[i].FMOsc1toOsc(),
        parmsTemplate_[i].FMOsc2toOsc(),
        parmsTemplate_[i].FMOsc3toOsc(),
        parmsTemplate_[i].FMOsc4toOsc(),
        parmsTemplate_[i].AMdepth(),
        parmsTemplate_[i].AMFreq(),
        parmsTemplate_[i].WaveformAM());
    }
    SynthStripParmsInstance morphed(newInstances[0]);
    morphed.LerpWith(newInstances[1], interpolationFactor);
    setAllParms(morphed);
  }

  SynthStripParmsInstance parms_;
  SynthStripParms parmsTemplate_[c_templatesCount];

  AudioSynthWaveformDc &     PitchEnvDepthOsc_;
  AudioEffectEnvelope &      PitchEnvOsc_;
  AudioMixer4 &              mixerOSCtoOSC_;
  AudioMixer4 &              mixerOSC_;
  AudioSynthWaveformModulated & OSC_;
  AudioSynthWaveform &       WaveAM_;
  AudioSynthWaveformDc &     AMdc_;
  AudioEffectEnvelope &      VolEnvOsc_;
  AudioMixer4 &              mixerAM_;
  AudioEffectMultiply &      AM_;
};

class FM4 {
 public:
  FM4() :
  strip1_(
    PitchEnvDepthOsc1,
    PitchEnvOsc1,
    mixerOSCtoOSC1,
    mixerOSC1,
    OSC1,
    WaveAM1,
    AMdc1,
    VolEnvOsc1,
    mixerAM1,
    AM1),
  strip2_(
    PitchEnvDepthOsc2,
    PitchEnvOsc2,
    mixerOSCtoOSC2,
    mixerOSC2,
    OSC2,
    WaveAM2,
    AMdc2,
    VolEnvOsc2,
    mixerAM2,
    AM2),
  strip3_(
    PitchEnvDepthOsc3,
    PitchEnvOsc3,
    mixerOSCtoOSC3,
    mixerOSC3,
    OSC3,
    WaveAM3,
    AMdc3,
    VolEnvOsc3,
    mixerAM3,
    AM3),
  strip4_(
    PitchEnvDepthOsc4,
    PitchEnvOsc4,
    mixerOSCtoOSC4,
    mixerOSC4,
    OSC4,
    WaveAM4,
    AMdc4,
    VolEnvOsc4,
    mixerAM4,
    AM4),
  all_synth_strips_{&strip1_, &strip2_, &strip3_, &strip4_ },
  interpolationFactor_(0.0f) {
  }

  void init() {
     SynthStripParmsInstance defaultSynthParms(
      0, /* WaveformOSC */
      ADSRParms( 1, 200, 0.2, 1000, 0 ), /* VolParms */
      550, /* FreqOsc */
      true, /* ListenSeq */
      1.0, /* PitchDepth */
      ADSRParms( 100, 200, 0.25, 200, 0 ), /* PitchParms */
      //ADSRParms( 100, 220, 0.0, 200, 100 ), /* PitchParms */
      0.0, /* FMOsc1toOsc */
      0.0, /* FMOsc2toOsc */
      0.0, /* FMOsc3toOsc */
      0.0, /* FMOsc4toOsc */
      1, /* AMdepth */
      4, /* AMFreq */
      0 /* WaveformAM */ );

    strip1_.setAllParms(defaultSynthParms);
    strip2_.setAllParms(defaultSynthParms);
    strip3_.setAllParms(defaultSynthParms);
    strip4_.setAllParms(defaultSynthParms);
    for (int i = 0; i < 4; ++i) {
      getStrip(i)->applyParms();
      getStrip(i)->initialise();
    }
    // Global initialisations
    Noise.amplitude(1.0);
  }

  void noteOn(const float midiNote) {
#ifdef SYNTH_DEBUG
    Serial.println(">>>>>>>> NOTE ON !! <<<<<<<<");
#endif // SYNTH_DEBUG
    float VolOsc1 = 1.0;  // Oscillator 1 Level
    float VolOsc2 = 0.0;  // Oscillator 2 Level
    float VolOsc3 = 0.0;  // Oscillator 3 Level
    float VolOsc4 = 0.0;  // Oscillator 4 Level

    AudioNoInterrupts();
    mixerMASTER.gain(0, VolOsc1);
    mixerMASTER.gain(1, VolOsc2);
    mixerMASTER.gain(2, VolOsc3);
    mixerMASTER.gain(3, VolOsc4);

    AudioInterrupts();
    const float noteFreqHz = mtof(midiNote);
#ifdef SYNTH_DEBUG
    Serial.printf("noteOn: frequency %f\n", noteFreqHz);
#endif // SYNTH_DEBUG
    for (int i = 0; i < 4; ++i) {
      getStrip(i)->noteOn(noteFreqHz, interpolationFactor_);
    }
  }

  void noteOff() {
#ifdef SYNTH_DEBUG
    Serial.println(">>>>>>>> NOTE OFF !! <<<<<<<<");
#endif // SYNTH_DEBUG
    for (int i = 0; i < 4; ++i) {
      getStrip(i)->noteOff();
    }
  }

  void setIndexedParameter(const int stripIndex, const unsigned templateIndex, const SynthParameterIndex parmIndex, const float parmValue ) {
    if(stripIndex > 3) {
      for (int i = 0; i < 4; ++i) {
        setIndexedParameterOnStrip(i, templateIndex, parmIndex, parmValue);
      }
    } else {
        setIndexedParameterOnStrip(stripIndex, templateIndex, parmIndex, parmValue);
    }
  }

  void applyParms() {
    for (int i = 0; i < 4; ++i) {
      getStrip(i)->applyParms();
    }
  }

  void setInterpolationFactor(const float value) {
    interpolationFactor_ = value;
  }

  void dumpParms() const {
    getStrip(0)->dump();
  }
 private:
  const SynthStrip * getStrip( int index ) const {
    if (index >= 0 && index < 4) {
      return all_synth_strips_[index];
    } else {
      return nullptr;
    }
  }

  SynthStrip * getStrip( int index ) {
    return const_cast<SynthStrip*>(const_cast<const FM4*>(this)->getStrip(index));
  }

  float mtof(const float midiNote) {
    const float freqValueHz = 8.17579891564 * exp(0.0577622650 * midiNote);
    return freqValueHz;
  }

  void setIndexedParameterOnStrip(const int stripIndex, const unsigned templateIndex, const SynthParameterIndex parmIndex, const float parmValue ) {
#ifdef SYNTH_DEBUG
    Serial.printf("Set Strip %d - \n", stripIndex);
#endif // SYNTH_DEBUG
    SynthStrip * strip = getStrip(stripIndex);
    if (strip != nullptr) {
      strip->setIndexedParameter(templateIndex, parmIndex, parmValue);
    }
  }

  SynthStrip strip1_;
  SynthStrip strip2_;
  SynthStrip strip3_;
  SynthStrip strip4_;
  SynthStrip * all_synth_strips_[4];
  float interpolationFactor_;
};

#endif // _FM4_SYNTH_H_

