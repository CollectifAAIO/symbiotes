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

// One synth strip, wrapped for easier global changes
struct Synth {
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

// All 4 instances of the strips
Synth synth1_ = {
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
  EnvNoise1
};
Synth synth2_ = {
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
  EnvNoise2
};
Synth synth3_ = {
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
  EnvNoise3
};
Synth synth4_ = {
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
  EnvNoise4
};

Synth * all_synths_[] = { &synth1_, &synth2_, &synth3_, &synth4_ };

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

    int AtkMs_;
    int DcayMs_;
    float Sus_;
    int RlsMs_;
    int DelayMs_;
};

// PARAMETERS SETTINGS (MAPPING & RANDOM VALUES)
struct SynthParms {
  SynthParms(
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

    void ApplyToSynth( Synth & synth ) const {
      AudioNoInterrupts();

      // COMMAND SYNTH

      // OSCs SHAPE

      synth.OSC_.begin(WAVEFORM[WaveformOSC_]);

      synth.waveform_.begin(1, ShapeModFreq_, WAVEFORM[ShapeModWaveform_]); // Waveform parameters which modulate the shape of the OSC.
      synth.waveform_.pulseWidth(PWShapeMod_); //If not any modulation is desired, switch the waveform to pulse mode and setup the pulse width to 0.
      ShapeModParms_.ApplyToADSR( synth.EnvShapeMod_ );

      // OSC VOLUME

      // Envelop
      VolParms_.ApplyToADSR( synth.VolEnvOsc_ );

      // OSC PITCH

      // Note
      synth.OSC_.frequency(FreqOsc_);

      // Envelop
      synth.PitchEnvDepthOsc_.amplitude(PitchDepth_); // Depth of Pitch enveloppe
      PitchParms_.ApplyToADSR( synth.PitchEnvOsc_ );

      // MODULATION

      // FM

      // Osc
      synth.mixerOSCtoOSC_.gain(0, FMOsc1toOsc_);     // Depth of FM from OSC1
      synth.mixerOSCtoOSC_.gain(1, FMOsc2toOsc_);     // Depth of FM from OSC2
      synth.mixerOSCtoOSC_.gain(2, FMOsc3toOsc_);     // Depth of FM from OSC3
      synth.mixerOSCtoOSC_.gain(3, FMOsc4toOsc_);     // Depth of FM from OSC4

      // Noise enveloppe
      synth.mixerOSC_.gain(2, DepthNoiseMod_);       // Depth of noise modulation
      NoiseParms_.ApplyToADSR( synth.EnvNoise_ );

      // AM
      synth.mixerAM_.gain(0, AMdepth_);
      synth.mixerAM_.gain(1, 1.0 - AMdepth_);

      synth.WaveAM_.begin(1, AMFreq_, WAVEFORM[WaveformAM_]); // Waveform & Rate of AM

      AudioInterrupts();
    }

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
};

Synth & GetSynth( int index ) {
  // We expect indices from 0 to 3
  return *all_synths_[index];
}

void InitialiseSynth( int index ) {
  AudioNoInterrupts();

  Synth & synth = GetSynth( index );
  synth.OSC_.amplitude(1);
  synth.VolEnvOsc_.releaseNoteOn(30);
  synth.mixerOSC_.gain(1, 1);
  synth.AMdc_.amplitude(1);
  synth.WaveAM_.begin(1, 4, WAVEFORM_TRIANGLE);
  synth.mixerOSC_.gain(1, 1);    // Importance of pitch enveloppe in the modulation
  synth.mixerOSC_.gain(2, 1);  // Depth of FM OSCILLATORS

  AudioInterrupts();
}

void FM4_init() {
   SynthParms defaultSynth1Parms(
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

   SynthParms defaultSynth2Parms(
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

   SynthParms defaultSynth3Parms(
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

   SynthParms defaultSynth4Parms(
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

  defaultSynth1Parms.ApplyToSynth( GetSynth( 0 ) );
  defaultSynth2Parms.ApplyToSynth( GetSynth( 1 ) );
  defaultSynth3Parms.ApplyToSynth( GetSynth( 2 ) );
  defaultSynth4Parms.ApplyToSynth( GetSynth( 3 ) );
  for (int i = 0; i < 4; ++i) {
    InitialiseSynth( i );
  }
}

void SynthNoteOn( int index ) {
  Synth & synth = GetSynth( index );
  synth.VolEnvOsc_.noteOn();
  synth.PitchEnvOsc_.noteOn();
  synth.EnvShapeMod_.noteOn();
  synth.EnvNoise_.noteOn();
}

void SynthNoteOff( int index ) {
  Synth & synth = GetSynth( index );
  synth.VolEnvOsc_.noteOff();
  synth.PitchEnvOsc_.noteOff();
  synth.EnvShapeMod_.noteOff();
  synth.EnvNoise_.noteOff();
}

void FM4_note( const bool _on ) {
  for (int i = 0; i < 4; ++i) {
    if ( _on ) {
      SynthNoteOn( i );
    } else {
      SynthNoteOff( i );
    }
  }
}

void FM4_synth (bool Note_on, bool Note_off, int Macro) {

  // Locked Parameters

  if (InitialiseFM4 == true) {
    FM4_init();
    InitialiseFM4 = false;
  }

  // Synth Modulation

  // >>>>> NOTE ON <<<<<

  if (Note_on == true) {
    Serial.println(">>>>>>>> NOTE ON !! <<<<<<<<");

    // MASTER MIXER

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
    // TRIG ENVELOPPES

    FM4_note( true );

  }

  // >>>>> NOTE OFF <<<<<

  if (Note_off == true) {
    Serial.println(">>>>>>>> NOTE OFF !! <<<<<<<<");

    FM4_note( false );
  }
}
