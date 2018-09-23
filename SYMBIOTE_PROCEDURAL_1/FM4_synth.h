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

static const char WAVEFORM[8] = {WAVEFORM_SINE, WAVEFORM_SQUARE, WAVEFORM_SAWTOOTH, WAVEFORM_PULSE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH_REVERSE, WAVEFORM_TRIANGLE_VARIABLE, WAVEFORM_SAMPLE_HOLD};

enum SynthParameterIndex {
  waveform,
  glide,
  Pitch,
  glide_rand,
  Pitch_rand,
  ListenSeq,
  FM_Osc1,
  FM_Osc2,
  FM_Osc3,
  FM_Osc4,
  FM_Osc1_rand,
  FM_Osc2_rand,
  FM_Osc3_rand,
  FM_Osc4_rand,
  AM_Waveform,
  AM_Depth,
  AM_Freq,
  AM_Depth_rand,
  AM_Freq_rand,
  PADSR_Dlay,
  PADSR_Amp,
  PADSR_Atk,
  PADSR_Dcay,
  PADSR_Sus,
  PADSR_Rel,
  PADSR_Dlay_rand,
  PADSR_Amp_rand,
  PADSR_Atk_rand,
  PADSR_Dcay_rand,
  PADSR_Sus_rand,
  PADSR_Rel_rand,
  LADSR_Dlay,
  LADSR_Amp,
  LADSR_Atk,
  LADSR_Dcay,
  LADSR_Sus,
  LADSR_Rel,
  LADSR_Dlay_rand,
  LADSR_Amp_rand,
  ADSR_Atk_rand,
  LADSR_Dcay_rand,
  LADSR_Sus_rand,
  LADSR_Rel_rand,
  Vol,
  Count
};

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
    const ADSRParms & VolParms = ADSRParms( 1, 200, 0.2, 1000, 0 ),
    int FreqOsc = 440,
    bool ListenSeq = false,
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
    VolParms_( VolParms ),
    FreqOsc_( FreqOsc ),
    ListenSeq_( ListenSeq ),
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

  // OSC VOLUME

  ADSRParms VolParms_;

  // OSC PITCH

  // Note
  int FreqOsc_;         // frequency of OSC1.

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

  // Noise enveloppe
  float DepthNoiseMod_;     // Depth of noise modulation
  ADSRParms NoiseParms_;

  // AM
  float AMdepth_;      // Mix between amp

  int AMFreq_;         // Frequency of Amplitude Modulation (Hz)
  int WaveformAM_;      // Waveform of AM (Waveform selected in the array between 0 & 7.)

  void setIndexedParameter(const SynthParameterIndex parmIndex, const float parmValue ) {
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
    case ListenSeq:{
      ListenSeq_ = parmValue > 0.0f;
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
    case PADSR_Atk:
    case PADSR_Dcay:
    case PADSR_Sus:
    case PADSR_Rel:{
      const int adsrParmIndex = parmIndex - PADSR_Dlay;
      PitchParms_.setIndexedParameter(adsrParmIndex, parmValue);
      break;
    }
    case PADSR_Amp: {
      PitchDepth_ = parmValue;
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
      VolParms_.setIndexedParameter(adsrParmIndex, parmValue);
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
  OSC_(OSC),
  WaveAM_(WaveAM),
  AMdc_(AMdc),
  VolEnvOsc_(VolEnvOsc),
  mixerAM_(mixerAM),
  AM_(AM),
  EnvNoise_(EnvNoise)
  {}

  void initialise() {
    AudioNoInterrupts();

    OSC_.amplitude(1.0);
    OSC_.frequencyModulation(1.0);
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

    OSC_.begin(WAVEFORM[parms_.WaveformOSC_]);

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

    // Noise enveloppe
    mixerOSC_.gain(2, parms_.DepthNoiseMod_);       // Depth of noise modulation
    parms_.NoiseParms_.applyToADSR( EnvNoise_ );

    // AM
    mixerAM_.gain(0, parms_.AMdepth_);
    mixerAM_.gain(1, 1.0 - parms_.AMdepth_);

    WaveAM_.begin(1, parms_.AMFreq_, WAVEFORM[parms_.WaveformAM_]); // Waveform & Rate of AM

    AudioInterrupts();
  }

  void setAllParms(const SynthStripParms & parms) {
    parms_ = parms;
  }

  void setIndexedParameter(const SynthParameterIndex parmIndex, const float parmValue ) {
    parms_.setIndexedParameter(parmIndex, parmValue);
    Serial.printf("Parm  %d - %f\n", parmIndex, parmValue);
  }

  void noteOn(const float noteFreqHz) {
    if(parms_.ListenSeq_) {
      setIndexedParameter(Pitch, noteFreqHz);
    }
    applyParms();

    VolEnvOsc_.noteOn();
    PitchEnvOsc_.noteOn();
    EnvNoise_.noteOn();
  }

  void noteOff() {
    VolEnvOsc_.noteOff();
    PitchEnvOsc_.noteOff();
    EnvNoise_.noteOff();
  }

 private:
  SynthStripParms parms_;

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
      0.0, /* DepthNoiseMod */
      ADSRParms( 1, 50, 0.0, 100, 0 ), /* NoiseParms */
      1, /* AMdepth */
      4, /* AMFreq */
      0 /* WaveformAM */ );

     SynthStripParms defaultSynth2Parms(
      0, /* WaveformOSC */
      ADSRParms( 1, 200, 0.5, 500, 0 ), /* VolParms */
      440, /* FreqOsc */
      false, /* ListenSeq */
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
      ADSRParms( 1, 200, 0.5, 500, 0 ), /* VolParms */
      440, /* FreqOsc */
      false, /* ListenSeq */
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
      ADSRParms( 1, 200, 0.2, 500, 0 ), /* VolParms */
      440, /* FreqOsc */
      false, /* ListenSeq */
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

    strip1_.setAllParms(defaultSynth1Parms);
    strip2_.setAllParms(defaultSynth2Parms);
    strip3_.setAllParms(defaultSynth3Parms);
    strip4_.setAllParms(defaultSynth4Parms);
    for (int i = 0; i < 4; ++i) {
      getStrip(i).applyParms();
      getStrip(i).initialise();
    }
  }

  void noteOn(const float midiNote) {
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
    const float noteFreqHz = mtof(midiNote);
    Serial.printf("noteOn: frequency %f\n", noteFreqHz);
    for (int i = 0; i < 4; ++i) {
      getStrip(i).noteOn(noteFreqHz);
    }
  }

  void noteOff() {
    Serial.println(">>>>>>>> NOTE OFF !! <<<<<<<<");
    for (int i = 0; i < 4; ++i) {
      getStrip(i).noteOff();
    }
  }

  void setIndexedParameter(const int stripIndex, const SynthParameterIndex parmIndex, const float parmValue ) {
    if(stripIndex > 3) {
      for (int i = 0; i < 4; ++i) {
        setIndexedParameterOnStrip(i, parmIndex, parmValue);
      }
    } else {
        setIndexedParameterOnStrip(stripIndex, parmIndex, parmValue);
    }
  }

  void applyParms() {
    for (int i = 0; i < 4; ++i) {
      getStrip(i).applyParms();
    }
  }

 private:
  SynthStrip & getStrip( int index ) {
    // We expect indices from 0 to 3
    return *all_synth_strips_[index];
  }

  float mtof(const float midiNote) {
    const float freqValueHz = 8.17579891564 * exp(0.0577622650 * midiNote);
    return freqValueHz;
  }

  void setIndexedParameterOnStrip(const int stripIndex, const SynthParameterIndex parmIndex, const float parmValue ) {
    Serial.printf("Set Strip %d - ", stripIndex);
    getStrip(stripIndex).setIndexedParameter(parmIndex, parmValue);
  }

  SynthStrip strip1_;
  SynthStrip strip2_;
  SynthStrip strip3_;
  SynthStrip strip4_;
  SynthStrip * all_synth_strips_[4];
};

#endif // _FM4_SYNTH_H_

