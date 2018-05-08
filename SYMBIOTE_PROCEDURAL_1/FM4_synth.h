


// SYNTH FM4.
void FM4_synth (bool Note_on, bool Note_off, int Macro) {

  // Locked Parameters

  if (InitialiseFM4 = true) {
    Noise.amplitude(1);
    // Osc 1
    OSC1.amplitude(1);
    VolEnvOsc1.releaseNoteOn(30);
    mixerOSC1.gain(1, 1);
    AMdc1.amplitude(1);
    WaveAM1.begin(1, 4, WAVEFORM_TRIANGLE);
    mixerOSC1.gain(1, 1);    // Importance of pitch enveloppe in the modulation
    mixerOSC1.gain(2, 1);  // Depth of FM OSCILLATORS

    // Osc 2
    OSC2.amplitude(1);
    VolEnvOsc2.releaseNoteOn(30);
    mixerOSC2.gain(1, 1);
    AMdc2.amplitude(1);
    WaveAM2.begin(1, 4, WAVEFORM_PULSE);
    mixerOSC2.gain(1, 1);    // Importance of pitch enveloppe in the modulation

    // Osc 3
    OSC3.amplitude(1);
    VolEnvOsc3.releaseNoteOn(30);
    mixerOSC3.gain(1, 1);
    AMdc3.amplitude(1);
    WaveAM3.begin(1, 4, WAVEFORM_TRIANGLE);
    mixerOSC3.gain(1, 1);    // Importance of pitch enveloppe in the modulation

    // Osc 4
    OSC4.amplitude(1);
    VolEnvOsc4.releaseNoteOn(30);
    mixerOSC4.gain(1, 1);
    AMdc4.amplitude(1);
    WaveAM4.begin(1, 4, WAVEFORM_TRIANGLE);
    mixerOSC4.gain(1, 1);    // Importance of pitch enveloppe in the modulation

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

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SETTINGS OSC1 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // PARAMETERS SETTINGS (MAPPING & RANDOM VALUES)

    // OSCs SHAPE
    int WaveformOSC1 = 0;       // Waveform selected in the array between 0 & 7.
    float ShapeModFreq1 = 5.0;  // Rate/frequency of the modulation of the shape of the waveform.
    int ShapeModWaveform1 = 0;  // Waveform modulating the shape of OSC1. It's selected in an array between 0 & 7.
    float PWShapeMod1 = 0.0;    // Width of Pulse signal.
    int ShapeModAtk1 = 100;     // Attack of Shape modulation between (between 0 & 11880ms)
    int ShapeModDcay1 = 100;    // Decay of Shape modulation          (between 0 & 11880ms)
    float ShapeModSus1 = 1.0;   // Sustain of Shape modulation        (between 0.0 & 1.0)
    int ShapeModRls1 = 500;     // Decay of Shape modulation          (between 0 & 11880ms)
    int ShapeModDelay1 = 0;     // Delay of Shape modulation          (between 0 & 11880ms)

    // OSC VOLUME

    int VolAtk1 = 1;          // Attack of OSC1 level (between 0 & 11880ms)
    int VolDcay1 = 200;       // Decay of OSC1 level (between 0 & 11880ms)
    float VolSus1 = 0.2;      // Sustain of OSC1 level (between 0.0 & 1.0)
    int VolRls1 = 1000;        // Release of OSC1 level (between 0 & 11880ms)
    int VolDelay1 = 0;        // Delay of OSC1 level (between 0 & 11880ms)

    // OSC PITCH

    // Note
    int FreqOsc1 = 440;         // frequency of OSC1.

    float PitchDepth1 = 0.2   ;         // Depth of Pitch enveloppe (between 0.0 & 1.0)
    int PitchAtk1 = 100 ;               // Attack of OSC1 pitch (between 0 & 11880ms)
    int PitchDcay1 = 220;               // Decay of OSC1 pitch (between 0 & 11880ms)
    float PitchSus1 = 0.0;              // sustain of OSC1 pitch (between 0.0 & 1.0)
    int PitchRls1 = 200;                // Release of OSC1 pitch (between 0 & 11880ms)
    int PitchDelay1 = 100 ;               // Delay of OSC1 pitch (between 0 & 11880ms)

    // MODULATIONS

    // FM

    // Osc
    float FMOsc1toOsc1 = 0.4;     // Depth of FM from OSC1
    float FMOsc2toOsc1 = 0.0;     // Depth of FM from OSC2
    float FMOsc3toOsc1 = 0.0;     // Depth of FM from OSC3
    float FMOsc4toOsc1 = 0.0;     // Depth of FM from OSC4

    // Noise enveloppe
    float DepthNoiseMod1 = 0.0;     // Depth of noise modulation
    int NoiseAtk1 = 1;            // Attack of Noise Modulation (between 0 & 11880ms)
    int NoiseDcay1 = 50;           // Decay of Noise Modulation (between 0 & 11880ms)
    float NoiseSus1 = 0.0;          // sustain of Noise Modulation (between 0.0 & 1.0)
    int NoiseRls1 = 100;            // Release of Noise Modulation (between 0 & 11880ms)
    int NoiseDelay1 = 0;            // Delay of Noise Modulation (between 0 & 11880ms)

    // AM
    float AMdepth1 = 0;      // Mix between amp

    int AMFreq1 = 4;         // Frequency of Amplitude Modulation (Hz)
    int WaveformAM1 = 0;      // Waveform of AM (Waveform selected in the array between 0 & 7.)


    // ----------------------

    // COMMAND SYNTH

    // OSCs SHAPE

    OSC1.begin(WAVEFORM[WaveformOSC1]);

    waveform1.begin(1, ShapeModFreq1, WAVEFORM[ShapeModWaveform1]); // Waveform parameters which modulate the shape of the OSC.
    waveform1.pulseWidth(PWShapeMod1); //If not any modulation is desired, switch the waveform to pulse mode and setup the pulse width to 0.
    EnvShapeMod1.attack(ShapeModAtk1);
    EnvShapeMod1.decay(ShapeModDcay1);
    EnvShapeMod1.sustain(ShapeModSus1);
    EnvShapeMod1.release(ShapeModRls1);
    EnvShapeMod1.delay(ShapeModDelay1);

    // OSC VOLUME

    // Envelop
    VolEnvOsc1.attack(VolAtk1);
    VolEnvOsc1.decay(VolDcay1);
    VolEnvOsc1.sustain(VolSus1);
    VolEnvOsc1.release(VolRls1);
    VolEnvOsc1.delay(VolDelay1);

    // OSC PITCH

    // Note
    OSC1.frequency(FreqOsc1);

    // Envelop
    PitchEnvDepthOsc1.amplitude(PitchDepth1); // Depth of Pitch enveloppe
    PitchEnvOsc1.attack(PitchAtk1);
    PitchEnvOsc1.decay(PitchDcay1);
    PitchEnvOsc1.sustain(PitchSus1);
    PitchEnvOsc1.release(PitchRls1);
    PitchEnvOsc1.delay(PitchDelay1);

    // MODULATION

    // FM

    // Osc
    mixerOSCtoOSC1.gain(0, FMOsc1toOsc1);     // Depth of FM from OSC1
    mixerOSCtoOSC1.gain(1, FMOsc2toOsc1);     // Depth of FM from OSC2
    mixerOSCtoOSC1.gain(2, FMOsc3toOsc1);     // Depth of FM from OSC3
    mixerOSCtoOSC1.gain(3, FMOsc4toOsc1);     // Depth of FM from OSC4

    // Noise enveloppe
    mixerOSC1.gain(2, DepthNoiseMod1);       // Depth of noise modulation
    EnvNoise1.attack(NoiseAtk1);
    EnvNoise1.decay(NoiseDcay1);
    EnvNoise1.sustain(NoiseSus1);
    EnvNoise1.release(NoiseRls1);
    EnvNoise1.delay(NoiseDelay1);

    // AM
    mixerAM1.gain(0, AMdepth1);
    mixerAM1.gain(1, 1.0 - AMdepth1);

    WaveAM1.begin(1, AMFreq1, WAVEFORM[WaveformAM1]); // Waveform & Rate of AM


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
