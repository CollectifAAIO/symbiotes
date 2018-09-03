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
  // We expect indices from 1 to 4
  return *all_synths_[index - 1];
}

void InitialiseSynth( int index ) {
  Synth & synth = GetSynth( index );
  synth.OSC_.amplitude(1);
  synth.VolEnvOsc_.releaseNoteOn(30);
  synth.mixerOSC_.gain(1, 1);
  synth.AMdc_.amplitude(1);
  synth.WaveAM_.begin(1, 4, WAVEFORM_TRIANGLE);
  synth.mixerOSC_.gain(1, 1);    // Importance of pitch enveloppe in the modulation
  synth.mixerOSC_.gain(2, 1);  // Depth of FM OSCILLATORS
}

void FM4_init() {
  for (int i = 0; i < 4; ++i) {
    InitialiseSynth( i );
  }
}

void FM4_setMasterGain() {
  for (int i = 0; i < 4; ++i) {
    InitialiseSynth( i );
  }
}
                      // SYNTH FM4.
void FM4_synth (bool Note_on, bool Note_off, int Macro) {

  // Locked Parameters

  if (InitialiseFM4 == true) {
    FM4_init();
    InitialiseFM4 = false;
  }

  // Synth Modulation

  // >>>>> NOTE ON <<<<<

  AudioNoInterrupts();

  if (Note_on == true) {
    Serial.println(">>>>>>>> NOTE ON !! <<<<<<<<");

    // MASTER MIXER

    float VolOsc1 = 1.0;  // Oscillator 1 Level
    float VolOsc2 = 0.0;  // Oscillator 2 Level
    float VolOsc3 = 0.0;  // Oscillator 3 Level
    float VolOsc4 = 0.0;  // Oscillator 4 Level

    mixerMASTER.gain(0, VolOsc1);
    mixerMASTER.gain(1, VolOsc2);
    mixerMASTER.gain(2, VolOsc3);
    mixerMASTER.gain(3, VolOsc4);

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SETTINGS OSC2 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // PARAMETERS SETTINGS (MAPPING & RANDOM VALUES)

    // OSCs SHAPE
    int WaveformOSC2 = 0; // Waveform selected in the array between 0 & 7.
    float ShapeModFreq2 = 5.0;
    int ShapeModWaveform2 = 0; // Waveform modulating the shape of OSC2. It's selected in an array between 0 & 7.
    float PWShapeMod2 = 1.0;    // Width of Pulse signal.
    int ShapeModAtk2 = 100;    // Attack of Shape modulation between (between 0 & 11880ms)
    int ShapeModDcay2 = 100;   // Decay of Shape modulation          (between 0 & 11880ms)
    float ShapeModSus2 = 1.0;  // Sustain of Shape modulation        (between 0.0 & 1.0)
    int ShapeModRls2 = 500;    // Decay of Shape modulation          (between 0 & 11880ms)
    int ShapeModDelay2 = 0;    // Delay of Shape modulation          (between 0 & 11880ms)

    // OSC VOLUME

    int VolAtk2 = 1;          // Attack of OSC2 level (between 0 & 11880ms)
    int VolDcay2 = 200;       // Decay of OSC2 level (between 0 & 11880ms)
    float VolSus2 = 0.5;      // Sustain of OSC2 level (between 0.0 & 1.0)
    int VolRls2 = 500;        // Release of OSC2 level (between 0 & 11880ms)
    int VolDelay2 = 0;        // Delay of OSC2 level (between 0 & 11880ms)

    // OSC PITCH

    // Note
    int FreqOsc2 = 440;         // frequency of OSC2.

    float PitchDepth2 = 0.0   ;         // Depth of Pitch enveloppe (between 0.0 & 1.0)
    int PitchAtk2 = 100 ;               // Attack of OSC2 pitch (between 0 & 11880ms)
    int PitchDcay2 = 220;               // Decay of OSC2 pitch (between 0 & 11880ms)
    float PitchSus2 = 0.3;              // sustain of OSC2 pitch (between 0.0 & 1.0)
    int PitchRls2 = 200;                // Release of OSC2 pitch (between 0 & 11880ms)
    int PitchDelay2 = 0 ;               // Delay of OSC2 pitch (between 0 & 11880ms)

    // MODULATIONS

    // FM

    // Osc
    float FMOsc1toOsc2 = 0.0;     // Depth of FM from OSC1
    float FMOsc2toOsc2 = 0.0;     // Depth of FM from OSC2
    float FMOsc3toOsc2 = 0.0;     // Depth of FM from OSC3
    float FMOsc4toOsc2 = 0.0;     // Depth of FM from OSC4

    // Noise enveloppe
    float DepthNoiseMod2 = 0.0;     // Depth of noise modulation
    int NoiseAtk2 = 250;            // Attack of Noise Modulation (between 0 & 11880ms)
    int NoiseDcay2 = 100;           // Decay of Noise Modulation (between 0 & 11880ms)
    float NoiseSus2 = 0.5;          // sustain of Noise Modulation (between 0.0 & 1.0)
    int NoiseRls2 = 100;            // Release of Noise Modulation (between 0 & 11880ms)
    int NoiseDelay2 = 0;            // Delay of Noise Modulation (between 0 & 11880ms)

    // AM
    float AMdepth2 = 0;       // Mix between amp

    int AMFreq2 = 4;          // Frequency of Amplitude Modulation (Hz)
    int WaveformAM2 = 0;      // Waveform of AM (Waveform selected in the array between 0 & 7.)


    // ----------------------

    // COMMAND SYNTH

    // OSCs SHAPE

    OSC2.begin(WAVEFORM[WaveformOSC2]);

    waveform2.begin(1, ShapeModFreq2, WAVEFORM[ShapeModWaveform2]); // Waveform parameters which modulate the shape of the OSC.
    waveform2.pulseWidth(PWShapeMod2); //If not any modulation is desired, switch the waveform to pulse mode and setup the pulse width to 0.
    EnvShapeMod2.attack(ShapeModAtk2);
    EnvShapeMod2.decay(ShapeModDcay2);
    EnvShapeMod2.sustain(ShapeModSus2);
    EnvShapeMod2.release(ShapeModRls2);
    EnvShapeMod2.delay(ShapeModDelay2);

    // OSC VOLUME

    // Envelop
    VolEnvOsc2.attack(VolAtk2);
    VolEnvOsc2.decay(VolDcay2);
    VolEnvOsc2.sustain(VolSus2);
    VolEnvOsc2.release(VolRls2);
    VolEnvOsc2.delay(VolDelay2);

    // OSC PITCH

    // Note
    OSC2.frequency(FreqOsc2);

    // Envelop
    PitchEnvDepthOsc2.amplitude(PitchDepth2); // Depth of Pitch enveloppe
    PitchEnvOsc2.attack(PitchAtk2);
    PitchEnvOsc2.decay(PitchDcay2);
    PitchEnvOsc2.sustain(PitchSus2);
    PitchEnvOsc2.release(PitchRls2);
    PitchEnvOsc2.delay(PitchDelay2);

    // MODULATION

    // FM

    // Osc
    mixerOSCtoOSC2.gain(0, FMOsc1toOsc2);     // Depth of FM from OSC1
    mixerOSCtoOSC2.gain(1, FMOsc2toOsc2);     // Depth of FM from OSC2
    mixerOSCtoOSC2.gain(2, FMOsc3toOsc2);     // Depth of FM from OSC3
    mixerOSCtoOSC2.gain(3, FMOsc4toOsc2);     // Depth of FM from OSC4

    // Noise enveloppe
    mixerOSC2.gain(2, DepthNoiseMod2);       // Depth of noise modulation
    EnvNoise2.attack(NoiseAtk2);
    EnvNoise2.decay(NoiseDcay2);
    EnvNoise2.sustain(NoiseSus2);
    EnvNoise2.release(NoiseRls2);
    EnvNoise2.delay(NoiseDelay2);

    // AM
    mixerAM2.gain(0, AMdepth2);
    mixerAM2.gain(1, 1.0 - AMdepth2);

    WaveAM2.begin(1, AMFreq2, WAVEFORM[WaveformAM2]); // Waveform & Rate of AM



    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SETTINGS OSC3 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



    // PARAMETERS SETTINGS (MAPPING & RANDOM VALUES)

    // OSCs SHAPE
    int WaveformOSC3 = 0; // Waveform selected in the array between 0 & 7.
    float ShapeModFreq3 = 5.0;
    int ShapeModWaveform3 = 0; // Waveform modulating the shape of OSC3. It's selected in an array between 0 & 7.
    float PWShapeMod3 = 1.0;    // Width of Pulse signal.
    int ShapeModAtk3 = 100;    // Attack of Shape modulation between (between 0 & 11880ms)
    int ShapeModDcay3 = 100;   // Decay of Shape modulation          (between 0 & 11880ms)
    float ShapeModSus3 = 1.0;  // Sustain of Shape modulation        (between 0.0 & 1.0)
    int ShapeModRls3 = 500;    // Decay of Shape modulation          (between 0 & 11880ms)
    int ShapeModDelay3 = 0;    // Delay of Shape modulation          (between 0 & 11880ms)

    // OSC VOLUME

    int VolAtk3 = 1;          // Attack of OSC3 level (between 0 & 11880ms)
    int VolDcay3 = 200;       // Decay of OSC3 level (between 0 & 11880ms)
    float VolSus3 = 0.5;      // Sustain of OSC3 level (between 0.0 & 1.0)
    int VolRls3 = 500;        // Release of OSC3 level (between 0 & 11880ms)
    int VolDelay3 = 0;        // Delay of OSC3 level (between 0 & 11880ms)

    // OSC PITCH

    // Note
    int FreqOsc3 = 440;         // frequency of OSC3.

    float PitchDepth3 = 0.0   ;         // Depth of Pitch enveloppe (between 0.0 & 1.0)
    int PitchAtk3 = 100 ;               // Attack of OSC3 pitch (between 0 & 11880ms)
    int PitchDcay3 = 220;               // Decay of OSC3 pitch (between 0 & 11880ms)
    float PitchSus3 = 0.3;              // sustain of OSC3 pitch (between 0.0 & 1.0)
    int PitchRls3 = 200;                // Release of OSC3 pitch (between 0 & 11880ms)
    int PitchDelay3 = 0 ;               // Delay of OSC3 pitch (between 0 & 11880ms)

    // MODULATIONS

    // FM

    // Osc
    float FMOsc1toOsc3 = 0.0;     // Depth of FM from OSC1
    float FMOsc2toOsc3 = 0.0;     // Depth of FM from OSC2
    float FMOsc3toOsc3 = 0.0;     // Depth of FM from OSC3
    float FMOsc4toOsc3 = 0.0;     // Depth of FM from OSC4

    // Noise enveloppe
    float DepthNoiseMod3 = 0.0;     // Depth of noise modulation
    int NoiseAtk3 = 250;            // Attack of Noise Modulation (between 0 & 11880ms)
    int NoiseDcay3 = 100;           // Decay of Noise Modulation (between 0 & 11880ms)
    float NoiseSus3 = 0.5;          // sustain of Noise Modulation (between 0.0 & 1.0)
    int NoiseRls3 = 100;            // Release of Noise Modulation (between 0 & 11880ms)
    int NoiseDelay3 = 0;            // Delay of Noise Modulation (between 0 & 11880ms)

    // AM
    float AMdepth3 = 0;       // Mix between amp

    int AMFreq3 = 4;          // Frequency of Amplitude Modulation (Hz)
    int WaveformAM3 = 0;      // Waveform of AM (Waveform selected in the array between 0 & 7.)


    // ----------------------

    // COMMAND SYNTH

    // OSCs SHAPE

    OSC3.begin(WAVEFORM[WaveformOSC3]);

    waveform3.begin(1, ShapeModFreq3, WAVEFORM[ShapeModWaveform3]); // Waveform parameters which modulate the shape of the OSC.
    waveform3.pulseWidth(PWShapeMod3); //If not any modulation is desired, switch the waveform to pulse mode and setup the pulse width to 0.
    EnvShapeMod3.attack(ShapeModAtk3);
    EnvShapeMod3.decay(ShapeModDcay3);
    EnvShapeMod3.sustain(ShapeModSus3);
    EnvShapeMod3.release(ShapeModRls3);
    EnvShapeMod3.delay(ShapeModDelay3);

    // OSC VOLUME

    // Envelop
    VolEnvOsc3.attack(VolAtk3);
    VolEnvOsc3.decay(VolDcay3);
    VolEnvOsc3.sustain(VolSus3);
    VolEnvOsc3.release(VolRls3);
    VolEnvOsc3.delay(VolDelay3);

    // OSC PITCH

    // Note
    OSC3.frequency(FreqOsc3);

    // Envelop
    PitchEnvDepthOsc3.amplitude(PitchDepth3); // Depth of Pitch enveloppe
    PitchEnvOsc3.attack(PitchAtk3);
    PitchEnvOsc3.decay(PitchDcay3);
    PitchEnvOsc3.sustain(PitchSus3);
    PitchEnvOsc3.release(PitchRls3);
    PitchEnvOsc3.delay(PitchDelay3);

    // MODULATION

    // FM

    // Osc
    mixerOSCtoOSC3.gain(0, FMOsc1toOsc3);     // Depth of FM from OSC1
    mixerOSCtoOSC3.gain(1, FMOsc2toOsc3);     // Depth of FM from OSC2
    mixerOSCtoOSC3.gain(2, FMOsc3toOsc3);     // Depth of FM from OSC3
    mixerOSCtoOSC3.gain(3, FMOsc4toOsc3);     // Depth of FM from OSC4

    // Noise enveloppe
    mixerOSC3.gain(2, DepthNoiseMod3);       // Depth of noise modulation
    EnvNoise3.attack(NoiseAtk3);
    EnvNoise3.decay(NoiseDcay3);
    EnvNoise3.sustain(NoiseSus3);
    EnvNoise3.release(NoiseRls3);
    EnvNoise3.delay(NoiseDelay3);

    // AM
    mixerAM3.gain(0, AMdepth3);
    mixerAM3.gain(1, 1.0 - AMdepth3);

    WaveAM3.begin(1, AMFreq3, WAVEFORM[WaveformAM3]); // Waveform & Rate of AM



    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SETTINGS OSC4 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



    // PARAMETERS SETTINGS (MAPPING & RANDOM VALUES)

    // OSCs SHAPE
    int WaveformOSC4 = 0; // Waveform selected in the array between 0 & 7.
    float ShapeModFreq4 = 5.0;
    int ShapeModWaveform4 = 0; // Waveform modulating the shape of OSC3. It's selected in an array between 0 & 7.
    float PWShapeMod4 = 1.0;    // Width of Pulse signal.
    int ShapeModAtk4 = 100;    // Attack of Shape modulation between (between 0 & 11880ms)
    int ShapeModDcay4 = 100;   // Decay of Shape modulation          (between 0 & 11880ms)
    float ShapeModSus4 = 1.0;  // Sustain of Shape modulation        (between 0.0 & 1.0)
    int ShapeModRls4 = 500;    // Decay of Shape modulation          (between 0 & 11880ms)
    int ShapeModDelay4 = 0;    // Delay of Shape modulation          (between 0 & 11880ms)

    // OSC VOLUME

    int VolAtk4 = 1;          // Attack of OSC4 level (between 0 & 11880ms)
    int VolDcay4 = 200;       // Decay of OSC4 level (between 0 & 11880ms)
    float VolSus4 = 0.5;      // Sustain of OSC4 level (between 0.0 & 1.0)
    int VolRls4 = 500;        // Release of OSC4 level (between 0 & 11880ms)
    int VolDelay4 = 0;        // Delay of OSC4 level (between 0 & 11880ms)

    // OSC PITCH

    // Note
    int FreqOsc4 = 440;         // frequency of OSC4.

    float PitchDepth4 = 0.0   ;         // Depth of Pitch enveloppe (between 0.0 & 1.0)
    int PitchAtk4 = 100 ;               // Attack of OSC4 pitch (between 0 & 11880ms)
    int PitchDcay4 = 220;               // Decay of OSC4 pitch (between 0 & 11880ms)
    float PitchSus4 = 0.3;              // sustain of OSC4 pitch (between 0.0 & 1.0)
    int PitchRls4 = 200;                // Release of OSC4 pitch (between 0 & 11880ms)
    int PitchDelay4 = 0 ;               // Delay of OSC4 pitch (between 0 & 11880ms)

    // MODULATIONS

    // FM

    // Osc
    float FMOsc1toOsc4 = 0.0;     // Depth of FM from OSC1
    float FMOsc2toOsc4 = 0.0;     // Depth of FM from OSC2
    float FMOsc3toOsc4 = 0.0;     // Depth of FM from OSC3
    float FMOsc4toOsc4 = 0.0;     // Depth of FM from OSC4

    // Noise enveloppe
    float DepthNoiseMod4 = 0.0;     // Depth of noise modulation
    int NoiseAtk4 = 250;            // Attack of Noise Modulation (between 0 & 11880ms)
    int NoiseDcay4 = 100;           // Decay of Noise Modulation (between 0 & 11880ms)
    float NoiseSus4 = 0.5;          // sustain of Noise Modulation (between 0.0 & 1.0)
    int NoiseRls4 = 100;            // Release of Noise Modulation (between 0 & 11880ms)
    int NoiseDelay4 = 0;            // Delay of Noise Modulation (between 0 & 11880ms)

    // AM
    float AMdepth4 = 0;       // Mix between amp

    int AMFreq4 = 4;          // Frequency of Amplitude Modulation (Hz)
    int WaveformAM4 = 0;      // Waveform of AM (Waveform selected in the array between 0 & 7.)


    // ----------------------

    // COMMAND SYNTH

    // OSCs SHAPE

    OSC4.begin(WAVEFORM[WaveformOSC4]);

    waveform4.begin(1, ShapeModFreq4, WAVEFORM[ShapeModWaveform4]); // Waveform parameters which modulate the shape of the OSC.
    waveform4.pulseWidth(PWShapeMod4); //If not any modulation is desired, switch the waveform to pulse mode and setup the pulse width to 0.
    EnvShapeMod4.attack(ShapeModAtk4);
    EnvShapeMod4.decay(ShapeModDcay4);
    EnvShapeMod4.sustain(ShapeModSus4);
    EnvShapeMod4.release(ShapeModRls4);
    EnvShapeMod4.delay(ShapeModDelay4);

    // OSC VOLUME

    // Envelop
    VolEnvOsc4.attack(VolAtk4);
    VolEnvOsc4.decay(VolDcay4);
    VolEnvOsc4.sustain(VolSus4);
    VolEnvOsc4.release(VolRls4);
    VolEnvOsc4.delay(VolDelay4);

    // OSC PITCH

    // Note
    OSC4.frequency(FreqOsc4);

    // Envelop
    PitchEnvDepthOsc4.amplitude(PitchDepth4); // Depth of Pitch enveloppe
    PitchEnvOsc4.attack(PitchAtk4);
    PitchEnvOsc4.decay(PitchDcay4);
    PitchEnvOsc4.sustain(PitchSus4);
    PitchEnvOsc4.release(PitchRls4);
    PitchEnvOsc4.delay(PitchDelay4);

    // MODULATION

    // FM

    // Osc
    mixerOSCtoOSC4.gain(0, FMOsc1toOsc4);     // Depth of FM from OSC1
    mixerOSCtoOSC4.gain(1, FMOsc2toOsc4);     // Depth of FM from OSC2
    mixerOSCtoOSC4.gain(2, FMOsc3toOsc4);     // Depth of FM from OSC3
    mixerOSCtoOSC4.gain(3, FMOsc4toOsc4);     // Depth of FM from OSC4

    // Noise enveloppe
    mixerOSC4.gain(2, DepthNoiseMod4);       // Depth of noise modulation
    EnvNoise4.attack(NoiseAtk4);
    EnvNoise4.decay(NoiseDcay4);
    EnvNoise4.sustain(NoiseSus4);
    EnvNoise4.release(NoiseRls4);
    EnvNoise4.delay(NoiseDelay4);

    // AM
    mixerAM4.gain(0, AMdepth4);
    mixerAM4.gain(1, 1.0 - AMdepth4);

    WaveAM3.begin(1, AMFreq4, WAVEFORM[WaveformAM4]); // Waveform & Rate of AM


    // TRIG ENVELOPPES

    VolEnvOsc1.noteOn();
    VolEnvOsc2.noteOn();
    VolEnvOsc3.noteOn();
    VolEnvOsc4.noteOn();

    PitchEnvOsc1.noteOn();
    PitchEnvOsc2.noteOn();
    PitchEnvOsc3.noteOn();
    PitchEnvOsc4.noteOn();

    EnvShapeMod1.noteOn();
    EnvShapeMod2.noteOn();
    EnvShapeMod3.noteOn();
    EnvShapeMod4.noteOn();

    EnvNoise1.noteOn();
    EnvNoise2.noteOn();
    EnvNoise3.noteOn();
    EnvNoise4.noteOn();

    AudioInterrupts();
  }

  // >>>>> NOTE OFF <<<<<

  if (Note_off == true) {
    Serial.println(">>>>>>>> NOTE OFF !! <<<<<<<<");

    VolEnvOsc1.noteOff();
    VolEnvOsc2.noteOff();
    VolEnvOsc3.noteOff();
    VolEnvOsc4.noteOff();

    PitchEnvOsc1.noteOff();
    PitchEnvOsc2.noteOff();
    PitchEnvOsc3.noteOff();
    PitchEnvOsc4.noteOff();

    EnvShapeMod1.noteOff();
    EnvShapeMod2.noteOff();
    EnvShapeMod3.noteOff();
    EnvShapeMod4.noteOff();

    EnvNoise1.noteOff();
    EnvNoise2.noteOff();
    EnvNoise3.noteOff();
    EnvNoise4.noteOff();
  }
}
