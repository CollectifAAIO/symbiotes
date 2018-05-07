// SYNTH FM4.
void FM4_synth (int Trig, int Macro) {

  // Locked Parameters

  if (InitialiseFM4 = true) {
    // Osc 1
    OSC1.amplitude(1);
    noise1.amplitude(1);
    VolEnvOsc1.releaseNoteOn(30);
    mixerOSC1.gain(1, 1);
    AMdc1.amplitude(1);
    WaveAM1.begin(1, 4, WAVEFORM_TRIANGLE);

    // Osc 2
    OSC2.amplitude(1);
    noise2.amplitude(1);
    VolEnvOsc2.releaseNoteOn(30);
    mixerOSC2.gain(1, 1);
    AMdc2.amplitude(1);
    WaveAM2.begin(1, 4, WAVEFORM_PULSE);

    // Osc 3
    OSC3.amplitude(1);
    noise3.amplitude(1);
    VolEnvOsc3.releaseNoteOn(30);
    mixerOSC3.gain(1, 1);
    AMdc3.amplitude(1);
    WaveAM3.begin(1, 4, WAVEFORM_TRIANGLE);

    // Osc 4
    OSC4.amplitude(1);
    noise4.amplitude(1);
    VolEnvOsc4.releaseNoteOn(30);
    mixerOSC4.gain(1, 1);
    AMdc4.amplitude(1);
    WaveAM4.begin(1, 4, WAVEFORM_TRIANGLE);

    InitialiseFM4 = false;
  }

  // Synth Modulation

  // >>>>> NOTE ON <<<<<

  AudioNoInterrupts();

  if (Trig = true) {
    // MASTER
    Serial.println(">>>>>>>> NOTE ON !! <<<<<<<<");

    mixerMASTER.gain(0, 1);
    mixerMASTER.gain(1, 0);
    mixerMASTER.gain(2, 0);
    mixerMASTER.gain(3, 0);

    // >> SETTINGS OSC1 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Macro & Random parameters
    int randFreqValueOSC1 = random(440, 440);
    //    int randPhaseValueOSC1 = random(0, 360);

    int randVolAtk1 = random(5, 5);
    int randVolDcay1 = random(1, 200);
    int randVolRls1 = random(0, 10);

    float PitchDepth1 = 0.0;
    int randPitchAtk1 = random(1, 1000);
    int randPitchDcay1 = random(10, 200);
    float randPitchSus1 = random(0, 100) / 100.0;

    int randPitchWaveform1 = random(2, 15);
    int randWaveform1 = random(2, 15);

    //    float randFMOsc3 = random(20, 100) / 100.0;

    // OSC Settings
    OSC1.frequency(randFreqValueOSC1);


    waveform1.begin(1, randPitchWaveform1, WAVEFORM[randWaveform1]);

    // Env Settings
    VolEnvOsc1.attack(randVolAtk1);
    VolEnvOsc1.decay(randVolDcay1);
    VolEnvOsc1.sustain(0);
    VolEnvOsc1.release(randVolRls1);
    //    VolEnvOsc1.delay(0);

    //    PitchEnvOsc1.attack(randPitchAtk1);
    //    PitchEnvOsc1.decay(randPitchDcay1);
    //    PitchEnvOsc1.sustain(randPitchSus1);
    //    PitchEnvOsc1.release(500);

    //     PitchEnvOsc1.release(300);

    EnvWave1.attack(100);
    EnvWave1.decay(20);
    EnvWave1.sustain(0.3);
    EnvWave1.release(100);
    EnvWave1.delay(400);

    EnvNoise1.attack(1);
    EnvNoise1.decay(50);
    EnvNoise1.sustain(0);
    //Envnoise1.release(200);
    //Envnoise1.delay(200);

    // Modulation Settings

    //FM
    mixerOSCtoOSC1.gain(0, 0);
    mixerOSCtoOSC1.gain(1, 0.3);
    mixerOSCtoOSC1.gain(2, 0.5);
    mixerOSCtoOSC1.gain(3, 0.2);

    //AM
    float AMdepth1 = 0;
    WaveAM1.frequency(4);
    mixerAM1.gain(0, AMdepth1);
    mixerAM1.gain(1, 1.0 - AMdepth1);
    //autres
    PitchEnvDepthOsc1.amplitude(PitchDepth1);
    mixerOSC1.gain(1, 1);
    mixerOSC1.gain(2, 0.3);
    mixerOSC1.gain(3, 0);


    // >> SETTINGS OSC2 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Random parameters
    int randFreqValueOSC2 = random(2000, 3000);
    int randPhaseValueOSC2 = random(0, 0);

    float randPitchDepth2 = random(0, 100) / 100.0;
    int randPitchAtk2 = random(50, 400);
    int randPitchDcay2 = random(5, 500);
    int randOscDlay2 = random(100, 500);

    float randPitchSus2 = random(0, 100) / 100.0;

    int randPitchWaveform2 = random(4000, 10000);
    //    int randWaveform2 = random(0, 7);

    //float randFMOsc2 = random(20, 100) / 100.0;

    // OSC Settings

    OSC2.frequency(randFreqValueOSC2);
    //    OSC2.phase(randPhaseValueOSC2);

    waveform2.begin(1, randPitchWaveform2, WAVEFORM_SAWTOOTH);

    // Env Settings
    VolEnvOsc2.attack(100);
    VolEnvOsc2.decay(200);
    VolEnvOsc2.sustain(0.4);
    VolEnvOsc2.release(1000);
    VolEnvOsc2.delay(randOscDlay2);

    PitchEnvOsc2.attack(randPitchAtk2);
    PitchEnvOsc2.decay(randPitchDcay2);
    PitchEnvOsc2.sustain(randPitchSus2);
    PitchEnvOsc2.release(500);
    PitchEnvOsc2.delay(200);

    EnvWave2.attack(50);
    EnvWave2.decay(100);
    EnvWave2.sustain(0.2);
    //EnvWave2.release(200);
    //    EnvWave2.delay(200);

    EnvNoise2.attack(1);
    EnvNoise2.decay(50);
    EnvNoise2.sustain(0);
    //    Envnoise2.release(200);
    //    Envnoise2.delay(200);

    // Modulation Settings
    // FM
    mixerOSCtoOSC2.gain(0, 1);
    mixerOSCtoOSC2.gain(1, 0);
    mixerOSCtoOSC2.gain(2, 0.5);
    mixerOSCtoOSC2.gain(3, 0);
    //AM
    float AMdepth2 = 1;
    WaveAM2.frequency(2);
    mixerAM2.gain(0, AMdepth2);
    mixerAM2.gain(1, 1.0 - AMdepth2);
    //autres
    PitchEnvDepthOsc2.amplitude(0);
    mixerOSC2.gain(1, 1);
    mixerOSC2.gain(2, 1);
    mixerOSC2.gain(3, 0);


    resetNoteOffTime = millis();

    // >> SETTINGS OSC3 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Random parameters
    int randFreqValueOSC3 = random(400, 3000);
    //    int randPhaseValueOSC3 = random(0, 360);

    float randPitchDepth3 = random(0, 100) / 100.0;
    int randPitchAtk3 = random(1, 200);
    int randPitchDcay3 = random(5, 200);
    float randPitchSus3 = random(0, 100) / 100.0;

    int randPitchWaveform3 = random(1, 20);
    //    int randWaveform3 = random(0, 7);

    float randFMOsc2 = random(20, 100) / 100.0;

    // OSC Settings
    OSC3.frequency(randFreqValueOSC3);

    waveform3.begin(1, randPitchWaveform2, WAVEFORM_SAWTOOTH);

    // Env Settings
    VolEnvOsc3.attack(1);
    VolEnvOsc3.decay(200);
    VolEnvOsc3.sustain(0.8);
    VolEnvOsc3.release(2000);
    VolEnvOsc3.delay(100);

    PitchEnvOsc3.attack(randPitchAtk3);
    PitchEnvOsc3.decay(randPitchDcay3);
    PitchEnvOsc3.sustain(randPitchSus3);
    //     PitchEnvOsc3.release(500);
    //     PitchEnvOsc3.delay(500);

    EnvWave3.attack(50);
    EnvWave3.decay(100);
    EnvWave3.sustain(0);
    //EnvWave3.release(200);
    //EnvWave3.delay(200);

    EnvNoise3.attack(1);
    EnvNoise3.decay(50);
    EnvNoise3.sustain(0);
    //    Envnoise3.release(200);
    //    Envnoise3.delay(200);

    // Modulation Settings
    //FM
    mixerOSCtoOSC3.gain(0, 0);
    mixerOSCtoOSC3.gain(1, randFMOsc2);
    mixerOSCtoOSC3.gain(2, 0);
    mixerOSCtoOSC3.gain(3, 0.4);
    //AM
    float AMdepth3 = 1;
    WaveAM3.frequency(5);
    mixerAM3.gain(0, AMdepth3);
    mixerAM3.gain(1, 1.0 - AMdepth3);
    //autres
    PitchEnvDepthOsc3.amplitude(1);
    mixerOSC3.gain(1, 1);
    mixerOSC3.gain(2, 0);
    mixerOSC3.gain(3, 0);

    // >> SETTINGS OSC4 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    // Random parameters
    int randFreqValueOSC4 = random(8000, 15000);
    //    int randPhaseValueOSC4 = random(0, 360);

    float randPitchDepth4 = random(0, 100) / 100.0;
    int randPitchAtk4 = random(200, 1000);
    int randPitchDcay4 = random(5, 200);
    float randPitchSus4 = random(0, 100) / 100.0;

    int randPitchWaveform4 = random(1, 20);
    //    int randWaveform4 = random(0, 7);

    //    float randFMOsc2 = random(20, 100) / 100.0;

    // OSC Settings
    OSC4.frequency(randFreqValueOSC4);


    waveform4.begin(1, randPitchWaveform2, WAVEFORM_SAWTOOTH);

    // Env Settings
    VolEnvOsc4.attack(20);
    VolEnvOsc4.decay(200);
    VolEnvOsc4.sustain(0.5);
    VolEnvOsc4.release(2000);
    //    VolEnvOsc4.delay(2000);

    PitchEnvOsc4.attack(randPitchAtk4);
    PitchEnvOsc4.decay(randPitchDcay4);
    PitchEnvOsc4.sustain(randPitchSus4);
    //        PitchEnvOsc4.release(500);
    //        PitchEnvOsc4.delay(500);

    EnvWave4.attack(50);
    EnvWave4.decay(100);
    EnvWave4.sustain(0);
    //EnvWave4.release(200);
    //EnvWave4.delay(200);

    EnvNoise4.attack(1);
    EnvNoise4.decay(50);
    EnvNoise4.sustain(0);
    //    Envnoise4.release(200);
    //    Envnoise4.delay(200);

    // Modulation Settings
    //FM
    mixerOSCtoOSC4.gain(0, 0);
    mixerOSCtoOSC4.gain(1, 1);
    mixerOSCtoOSC4.gain(2, 0.7);
    mixerOSCtoOSC4.gain(3, 0);
    //AM
    float AMdepth4 = 0.0;
    WaveAM4.frequency(7);
    mixerAM4.gain(0, AMdepth4);
    mixerAM4.gain(1, 1.0 - AMdepth4);
    //autres
    PitchEnvDepthOsc4.amplitude(0.2);
    mixerOSC4.gain(1, 1);
    mixerOSC4.gain(2, 0);
    mixerOSC4.gain(3, 0);

    // TRIG

    VolEnvOsc1.noteOn();
    VolEnvOsc2.noteOn();
    VolEnvOsc3.noteOn();
    VolEnvOsc4.noteOn();

    //    PitchEnvOsc1.noteOn();
    PitchEnvOsc2.noteOn();
    PitchEnvOsc3.noteOn();
    PitchEnvOsc4.noteOn();

    EnvWave1.noteOn();
    EnvWave2.noteOn();
    EnvWave3.noteOn();
    EnvWave4.noteOn();

    EnvNoise1.noteOn();
    EnvNoise2.noteOn();
    EnvNoise3.noteOn();
    EnvNoise4.noteOn();

    AudioInterrupts();
  }


  // >>>>> NOTE OFF <<<<<

if(Note_Off == true) {
    Serial.println(">>>>>>>> NOTE OFF !! <<<<<<<<");
    VolEnvOsc1.noteOff();
    VolEnvOsc2.noteOff();
    VolEnvOsc3.noteOff();
    VolEnvOsc4.noteOff();
  }
}
