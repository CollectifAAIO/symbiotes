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

#include "Presets.h"

// Struct defining an envelope generator parameters
struct ADSRParms {
  ADSRParms( int AtkMs = 100,
    int DcayMs = 100,
    float Sus = 1.0,
    int RlsMs = 500,
    int DelayMs = 0 ) :
    AtkMs_( AtkMs ),
    DcayMs_( DcayMs ),
    Sus_( Sus ),
    RlsMs_( RlsMs ),
    DelayMs_( DelayMs ) {}

    void ApplyToADSR( AudioEffectEnvelope & instance ) const {
      instance.attack(AtkMs_);
      instance.decay(DcayMs_);
      instance.sustain(Sus_);
      instance.release(RlsMs_);
      instance.delay(DelayMs_);
    }

    void SetIndexedParameter(const int parmIndex, const float parmValue ) {
      switch(parmIndex) {
      case 0:{
          AtkMs_ = parmValue;
          break;
        }
      case 1:{
          DcayMs_ = parmValue;
          break;
        }
      case 2:{
          Sus_ = parmValue;
          break;
        }
      case 3:{
          RlsMs_ = parmValue;
          break;
        }
      case 4:{
          DelayMs_ = parmValue;
          break;
        }
      default: {
        Serial.println("Bad parameter index.");
        break;
      }
      }
    }

    int AtkMs_;
    int DcayMs_;
    float Sus_;
    int RlsMs_;
    int DelayMs_;
};

struct SynthStripParms {
  SynthStripParms(
    int WaveformOSC = 0,
    float ShapeModFreq = 5.0,
    int ShapeModWaveform = 0,
    float PWShapeMod = 0.0,
    const ADSRParms & ShapeModParms = ADSRParms( 100, 100, 1.0, 500, 0 ),
    const ADSRParms & VolParms = ADSRParms( 1, 200, 0.2, 1000, 0 ),
    int FreqOsc = 440,
    float PitchDepth = 0.2,
    const ADSRParms & PitchParms = ADSRParms( 100, 220, 0.0, 200, 100 ),
    float FMOsc1toOsc = 0.4,
    float FMOsc2toOsc = 0.0,
    float FMOsc3toOsc = 0.0,
    float FMOsc4toOsc = 0.0,
    float DepthNoiseMod = 0.0,
    const ADSRParms & NoiseParms = ADSRParms( 1, 50, 0.0, 100, 0 ),
    float AMdepth = 0,
    int AMFreq = 4,
    int WaveformAM = 0 )
    :
    WaveformOSC_( WaveformOSC ),
    ShapeModFreq_( ShapeModFreq ),
    ShapeModWaveform_( ShapeModWaveform ),
    PWShapeMod_( PWShapeMod ),
    ShapeModParms_( ShapeModParms ),
    VolParms_( VolParms ),
    FreqOsc_( FreqOsc ),
    PitchDepth_( PitchDepth ),
    PitchParms_( PitchParms ),
    FMOsc1toOsc_( FMOsc1toOsc ),
    FMOsc2toOsc_( FMOsc2toOsc ),
    FMOsc3toOsc_( FMOsc3toOsc ),
    FMOsc4toOsc_( FMOsc4toOsc ),
    DepthNoiseMod_( DepthNoiseMod ),
    NoiseParms_( NoiseParms ),
    AMdepth_( AMdepth ),
    AMFreq_( AMFreq ),
    WaveformAM_( WaveformAM ) {}

  // OSCs SHAPE
  int WaveformOSC_;       // Waveform selected in the array between 0 & 7.
  float ShapeModFreq_;  // Rate/frequency of the modulation of the shape of the waveform.
  int ShapeModWaveform_;  // Waveform modulating the shape of OSC1. It's selected in an array between 0 & 7.
  float PWShapeMod_;    // Width of Pulse signal.
  ADSRParms ShapeModParms_;

  // OSC VOLUME

  ADSRParms VolParms_;

  // OSC PITCH

  // Note
  int FreqOsc_;         // frequency of OSC1.

  float PitchDepth_;         // Depth of Pitch enveloppe (between 0.0 & 1.0)
  ADSRParms PitchParms_;

  // MODULATIONS

  // FM

  // Osc
  float FMOsc1toOsc_;     // Depth of FM from OSC1
  float FMOsc2toOsc_;     // Depth of FM from OSC2
  float FMOsc3toOsc_;     // Depth of FM from OSC3
  float FMOsc4toOsc_;     // Depth of FM from OSC4

  // Noise enveloppe
  float DepthNoiseMod_;     // Depth of noise modulation
  ADSRParms NoiseParms_;

  // AM
  float AMdepth_;      // Mix between amp

  int AMFreq_;         // Frequency of Amplitude Modulation (Hz)
  int WaveformAM_;      // Waveform of AM (Waveform selected in the array between 0 & 7.)

  void SetIndexedParameter(const parameterIndex parmIndex, const float parmValue ) {
    switch(parmIndex) {
    case waveform:{
      WaveformOSC_ = static_cast<int>(parmValue);
      break;
    }
    case glide:{
      break;
    }
    case Pitch:{
      FreqOsc_ = parmValue;
      break;
    }
    case glide_rand:
    case Pitch_rand:{
      break;
    }
    case FM_Osc1:{
      FMOsc1toOsc_ = parmValue;
      break;
    }
    case FM_Osc2:{
      FMOsc2toOsc_ = parmValue;
      break;
    }
    case FM_Osc3:{
      FMOsc3toOsc_ = parmValue;
      break;
    }
    case FM_Osc4:{
      FMOsc4toOsc_ = parmValue;
      break;
    }
    case FM_Osc1_rand:
    case FM_Osc2_rand:
    case FM_Osc3_rand:
    case FM_Osc4_rand:{
      break;
    }
    case AM_Waveform:{
      WaveformAM_ = parmValue;
      break;
    }
    case AM_Depth:{
      AMdepth_ = parmValue;
      break;
    }
    case AM_Freq:{
      AMFreq_ = parmValue;
      break;
    }
    case AM_Depth_rand:
    case AM_Freq_rand:{
      break;
    }
    case PADSR_Dlay:
    case PADSR_Amp:
    case PADSR_Atk:
    case PADSR_Dcay:
    case PADSR_Sus:
    case PADSR_Rel:{
      const int adsrParmIndex = parmIndex - PADSR_Dlay;
      PitchParms_.SetIndexedParameter(adsrParmIndex, parmValue);
      break;
    }
    case PADSR_Dlay_rand:
    case PADSR_Amp_rand:
    case PADSR_Atk_rand:
    case PADSR_Dcay_rand:
    case PADSR_Sus_rand:
    case PADSR_Rel_rand:{
      break;
    }
    case LADSR_Dlay:
    case LADSR_Amp:
    case LADSR_Atk:
    case LADSR_Dcay:
    case LADSR_Sus:
    case LADSR_Rel:{
      const int adsrParmIndex = parmIndex - LADSR_Dlay;
      VolParms_.SetIndexedParameter(adsrParmIndex, parmValue);
      break;
    }
    case LADSR_Dlay_rand:
    case LADSR_Amp_rand:
    case ADSR_Atk_rand:
    case LADSR_Dcay_rand:
    case LADSR_Sus_rand:
    case LADSR_Rel_rand:{
      break;
    }
    case Vol:{
      break;
    }
    default:{
      Serial.println("Bad parameter index.");
      break;
    }
    }
  }
};

// One synth strip, wrapped for easier global changes
struct SynthStrip {
  SynthStrip(AudioSynthWaveformDc &     PitchEnvDepthOsc,
  AudioEffectEnvelope &      PitchEnvOsc,
  AudioMixer4 &              mixerOSCtoOSC,
  AudioMixer4 &              mixerOSC,
  AudioSynthWaveform &       waveform,
  AudioEffectEnvelope &      EnvShapeMod,
  AudioSynthWaveformModulated & OSC,
  AudioSynthWaveform &       WaveAM,
  AudioSynthWaveformDc &     AMdc,
  AudioEffectEnvelope &      VolEnvOsc,
  AudioMixer4 &              mixerAM,
  AudioEffectMultiply &      AM,
  AudioEffectEnvelope &      EnvNoise) :
  PitchEnvDepthOsc_(PitchEnvDepthOsc),
  PitchEnvOsc_(PitchEnvOsc),
  mixerOSCtoOSC_(mixerOSCtoOSC),
  mixerOSC_(mixerOSC),
  waveform_(waveform),
  EnvShapeMod_(EnvShapeMod),
  OSC_(OSC),
  WaveAM_(WaveAM),
  AMdc_(AMdc),
  VolEnvOsc_(VolEnvOsc),
  mixerAM_(mixerAM),
  AM_(AM),
  EnvNoise_(EnvNoise)
  {}

  void Initialise() {
    AudioNoInterrupts();

    OSC_.amplitude(0.5);
    VolEnvOsc_.releaseNoteOn(30);
    AMdc_.amplitude(1);
    WaveAM_.begin(1, 4, WAVEFORM_TRIANGLE);
    mixerOSC_.gain(1, 1);    // Importance of pitch enveloppe in the modulation
    mixerOSC_.gain(2, 1);  // Depth of FM OSCILLATORS

    AudioInterrupts();
  }

  void ApplyParms() {
    AudioNoInterrupts();

    // COMMAND SYNTH

    // OSCs SHAPE

    OSC_.begin(WAVEFORM[parms_.WaveformOSC_]);

    waveform_.begin(1, parms_.ShapeModFreq_, WAVEFORM[parms_.ShapeModWaveform_]); // Waveform parameters which modulate the shape of the OSC.
    waveform_.pulseWidth(parms_.PWShapeMod_); //If not any modulation is desired, switch the waveform to pulse mode and setup the pulse width to 0.
    parms_.ShapeModParms_.ApplyToADSR(EnvShapeMod_);

    // OSC VOLUME

    // Envelop
    parms_.VolParms_.ApplyToADSR(VolEnvOsc_);

    // OSC PITCH

    // Note
    OSC_.frequency(parms_.FreqOsc_);

    // Envelop
    PitchEnvDepthOsc_.amplitude(parms_.PitchDepth_); // Depth of Pitch enveloppe
    parms_.PitchParms_.ApplyToADSR( PitchEnvOsc_ );

    // MODULATION

    // FM

    // Osc
    mixerOSCtoOSC_.gain(0, parms_.FMOsc1toOsc_);     // Depth of FM from OSC1
    mixerOSCtoOSC_.gain(1, parms_.FMOsc2toOsc_);     // Depth of FM from OSC2
    mixerOSCtoOSC_.gain(2, parms_.FMOsc3toOsc_);     // Depth of FM from OSC3
    mixerOSCtoOSC_.gain(3, parms_.FMOsc4toOsc_);     // Depth of FM from OSC4

    // Noise enveloppe
    mixerOSC_.gain(2, parms_.DepthNoiseMod_);       // Depth of noise modulation
    parms_.NoiseParms_.ApplyToADSR( EnvNoise_ );

    // AM
    mixerAM_.gain(0, parms_.AMdepth_);
    mixerAM_.gain(1, 1.0 - parms_.AMdepth_);

    WaveAM_.begin(1, parms_.AMFreq_, WAVEFORM[parms_.WaveformAM_]); // Waveform & Rate of AM

    AudioInterrupts();
  }

  void SetAllParms(const SynthStripParms & parms) {
    parms_ = parms;
  }

  void SetIndexedParameter(const parameterIndex parmIndex, const float parmValue ) {
    parms_.SetIndexedParameter(parmIndex, parmValue);
    Serial.printf("Parm  %d - %f\n", parmIndex, parmValue);
  }

  void NoteOn() {
    VolEnvOsc_.noteOn();
    PitchEnvOsc_.noteOn();
    EnvShapeMod_.noteOn();
    EnvNoise_.noteOn();
  }

  void NoteOff() {
    VolEnvOsc_.noteOff();
    PitchEnvOsc_.noteOff();
    EnvShapeMod_.noteOff();
    EnvNoise_.noteOff();
  }

 private:
  SynthStripParms parms_;

  AudioSynthWaveformDc &     PitchEnvDepthOsc_;
  AudioEffectEnvelope &      PitchEnvOsc_;
  AudioMixer4 &              mixerOSCtoOSC_;
  AudioMixer4 &              mixerOSC_;
  AudioSynthWaveform &       waveform_;
  AudioEffectEnvelope &      EnvShapeMod_;
  AudioSynthWaveformModulated & OSC_;
  AudioSynthWaveform &       WaveAM_;
  AudioSynthWaveformDc &     AMdc_;
  AudioEffectEnvelope &      VolEnvOsc_;
  AudioMixer4 &              mixerAM_;
  AudioEffectMultiply &      AM_;
  AudioEffectEnvelope &      EnvNoise_;
};

class FM4 {
 public:
  FM4() :
  strip1_(
    PitchEnvDepthOsc1,
    PitchEnvOsc1,
    mixerOSCtoOSC1,
    mixerOSC1,
    waveform1,
    EnvShapeMod1,
    OSC1,
    WaveAM1,
    AMdc1,
    VolEnvOsc1,
    mixerAM1,
    AM1,
    EnvNoise1),
  strip2_(
    PitchEnvDepthOsc2,
    PitchEnvOsc2,
    mixerOSCtoOSC2,
    mixerOSC2,
    waveform2,
    EnvShapeMod2,
    OSC2,
    WaveAM2,
    AMdc2,
    VolEnvOsc2,
    mixerAM2,
    AM2,
    EnvNoise2),
  strip3_(
    PitchEnvDepthOsc3,
    PitchEnvOsc3,
    mixerOSCtoOSC3,
    mixerOSC3,
    waveform3,
    EnvShapeMod3,
    OSC3,
    WaveAM3,
    AMdc3,
    VolEnvOsc3,
    mixerAM3,
    AM3,
    EnvNoise3),
  strip4_(
    PitchEnvDepthOsc4,
    PitchEnvOsc4,
    mixerOSCtoOSC4,
    mixerOSC4,
    waveform4,
    EnvShapeMod4,
    OSC4,
    WaveAM4,
    AMdc4,
    VolEnvOsc4,
    mixerAM4,
    AM4,
    EnvNoise4),
  all_synth_strips_{&strip1_, &strip2_, &strip3_, &strip4_ } {
  }

  void init() {
     SynthStripParms defaultSynth1Parms(
      0, /* WaveformOSC */
      5.0, /* ShapeModFreq */
      0, /* ShapeModWaveform */
      0.0, /* PWShapeMod */
      ADSRParms( 100, 100, 1.0, 500, 0 ), /* ShapeModParms */
      ADSRParms( 1, 200, 0.2, 1000, 0 ), /* VolParms */
      440, /* FreqOsc */
      0.2, /* PitchDepth */
      ADSRParms( 100, 220, 0.0, 200, 100 ), /* PitchParms */
      0.4, /* FMOsc1toOsc */
      0.0, /* FMOsc2toOsc */
      0.0, /* FMOsc3toOsc */
      0.0, /* FMOsc4toOsc */
      0.0, /* DepthNoiseMod */
      ADSRParms( 1, 50, 0.0, 100, 0 ), /* NoiseParms */
      0, /* AMdepth */
      4, /* AMFreq */
      0 /* WaveformAM */ );

     SynthStripParms defaultSynth2Parms(
      0, /* WaveformOSC */
      5.0, /* ShapeModFreq */
      0, /* ShapeModWaveform */
      1.0, /* PWShapeMod */
      ADSRParms( 100, 100, 1.0, 500, 0 ), /* ShapeModParms */
      ADSRParms( 1, 200, 0.5, 500, 0 ), /* VolParms */
      440, /* FreqOsc */
      0.0, /* PitchDepth */
      ADSRParms( 100, 220, 0.3, 200, 0 ), /* PitchParms */
      0.0, /* FMOsc1toOsc */
      0.0, /* FMOsc2toOsc */
      0.0, /* FMOsc3toOsc */
      0.0, /* FMOsc4toOsc */
      0.0, /* DepthNoiseMod */
      ADSRParms( 250, 100, 0.5, 100, 0 ), /* NoiseParms */
      0, /* AMdepth */
      4, /* AMFreq */
      0 /* WaveformAM */ );

     SynthStripParms defaultSynth3Parms(
      0, /* WaveformOSC */
      5.0, /* ShapeModFreq */
      0, /* ShapeModWaveform */
      1.0, /* PWShapeMod */
      ADSRParms( 100, 100, 1.0, 500, 0 ), /* ShapeModParms */
      ADSRParms( 1, 200, 0.5, 500, 0 ), /* VolParms */
      440, /* FreqOsc */
      0.0, /* PitchDepth */
      ADSRParms( 100, 220, 0.3, 200, 0 ), /* PitchParms */
      0.0, /* FMOsc1toOsc */
      0.0, /* FMOsc2toOsc */
      0.0, /* FMOsc3toOsc */
      0.0, /* FMOsc4toOsc */
      0.0, /* DepthNoiseMod */
      ADSRParms( 250, 100, 0.5, 100, 0 ), /* NoiseParms */
      0, /* AMdepth */
      4, /* AMFreq */
      0 /* WaveformAM */ );

     SynthStripParms defaultSynth4Parms(
      0, /* WaveformOSC */
      5.0, /* ShapeModFreq */
      0, /* ShapeModWaveform */
      1.0, /* PWShapeMod */
      ADSRParms( 100, 100, 1.0, 500, 0 ), /* ShapeModParms */
      ADSRParms( 1, 200, 0.2, 500, 0 ), /* VolParms */
      440, /* FreqOsc */
      0.0, /* PitchDepth */
      ADSRParms( 100, 220, 0.3, 200, 0 ), /* PitchParms */
      0.0, /* FMOsc1toOsc */
      0.0, /* FMOsc2toOsc */
      0.0, /* FMOsc3toOsc */
      0.0, /* FMOsc4toOsc */
      0.0, /* DepthNoiseMod */
      ADSRParms( 250, 100, 0.5, 100, 0 ), /* NoiseParms */
      0, /* AMdepth */
      4, /* AMFreq */
      0 /* WaveformAM */ );

    strip1_.SetAllParms(defaultSynth1Parms);
    strip2_.SetAllParms(defaultSynth2Parms);
    strip3_.SetAllParms(defaultSynth3Parms);
    strip4_.SetAllParms(defaultSynth4Parms);
    for (int i = 0; i < 4; ++i) {
      GetStrip(i).ApplyParms();
      GetStrip(i).Initialise();
    }
  }

  void NoteOn() {
    Serial.println(">>>>>>>> NOTE ON !! <<<<<<<<");
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
    for (int i = 0; i < 4; ++i) {
      GetStrip(i).NoteOn();
    }
  }

  void NoteOff() {
    Serial.println(">>>>>>>> NOTE OFF !! <<<<<<<<");
    for (int i = 0; i < 4; ++i) {
      GetStrip(i).NoteOff();
    }
  }

  void SetIndexedParameter(const int stripIndex, const parameterIndex parmIndex, const float parmValue ) {
    if(stripIndex > 3) {
      for (int i = 0; i < 4; ++i) {
        SetIndexedParameterOnStrip(i, parmIndex, parmValue);
      }
    } else {
        SetIndexedParameterOnStrip(stripIndex, parmIndex, parmValue);
    }
  }

  void ApplyParms() {
    Serial.println(">>>>>>>> NOTE OFF !! <<<<<<<<");
    for (int i = 0; i < 4; ++i) {
      GetStrip(i).NoteOff();
    }
  }

 private:
  SynthStrip & GetStrip( int index ) {
    // We expect indices from 0 to 3
    return *all_synth_strips_[index];
  }

  void SetIndexedParameterOnStrip(const int stripIndex, const parameterIndex parmIndex, const float parmValue ) {
    Serial.printf("Set Strip %d - ", stripIndex);
    GetStrip(stripIndex).SetIndexedParameter(parmIndex, parmValue);
  }

  SynthStrip strip1_;
  SynthStrip strip2_;
  SynthStrip strip3_;
  SynthStrip strip4_;
  SynthStrip * all_synth_strips_[4];
};

#endif // _FM4_SYNTH_H_
