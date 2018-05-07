#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthNoiseWhite     noise1;         //xy=63,192
AudioSynthNoiseWhite     noise2;         //xy=70,393
AudioSynthWaveform       waveform1;      //xy=71,154
AudioSynthNoiseWhite     noise3;         //xy=71,669
AudioSynthWaveform       waveform2;      //xy=80,355
AudioSynthWaveform       waveform3;      //xy=81,633
AudioSynthWaveformDc     PitchEnvDepthOsc1; //xy=98,48
AudioSynthWaveformDc     PitchEnvDepthOsc3; //xy=108,525
AudioSynthWaveformDc     PitchEnvDepthOsc2; //xy=110,252
AudioSynthWaveformDc     PitchEnvDepthOsc4; //xy=116,810
AudioSynthNoiseWhite     noise4;         //xy=160,946
AudioSynthWaveform       waveform4;      //xy=167,909
AudioMixer4              mixerOSCtoOSC2; //xy=276,304
AudioMixer4              mixerOSCtoOSC3; //xy=278,580
AudioMixer4              mixerOSCtoOSC1; //xy=280,100
AudioMixer4              mixerOSCtoOSC4; //xy=287,858
AudioEffectEnvelope      PitchEnvOsc2;   //xy=290,252
AudioEffectEnvelope      PitchEnvOsc3;   //xy=289,524
AudioEffectEnvelope      PitchEnvOsc1;   //xy=293,48
AudioEffectEnvelope      EnvNoise1;      //xy=294,191
AudioEffectEnvelope      EnvWave1;       //xy=297,154
AudioEffectEnvelope      EnvWave3;       //xy=299,636
AudioEffectEnvelope      EnvNoise3;      //xy=299,670
AudioEffectEnvelope      EnvWave2;       //xy=301,355
AudioEffectEnvelope      PitchEnvOsc4;   //xy=300,810
AudioEffectEnvelope      EnvNoise2;      //xy=302,393
AudioEffectEnvelope      EnvWave4;       //xy=309,909
AudioEffectEnvelope      EnvNoise4;      //xy=309,945
AudioMixer4              mixerOSC1;      //xy=497,44.14283752441406
AudioMixer4              mixerOSC3;      //xy=496.85711669921875,504.5713806152344
AudioMixer4              mixerOSC2;      //xy=501.857177734375,241.42855834960938
AudioMixer4              mixerOSC4;      //xy=510.42852783203125,822.5714111328125
AudioSynthWaveform       WaveAM2;        //xy=617,353
AudioSynthWaveform       WaveAM4;        //xy=617.28564453125,903
AudioSynthWaveformDc     AMdc4;          //xy=620,940
AudioSynthWaveform       WaveAM3;        //xy=623,622
AudioSynthWaveformDc     AMdc2;          //xy=627,387
AudioSynthWaveform       WaveAM1;        //xy=628,150
AudioSynthWaveformDc     AMdc3;          //xy=629,660
AudioSynthWaveformModulated OSC3;   //xy=635.7142944335938,511.4286804199219
AudioSynthWaveformDc     AMdc1;          //xy=638,190
AudioSynthWaveformModulated OSC1;   //xy=638.5713500976562,51.42857360839844
AudioSynthWaveformModulated OSC2;   //xy=640.0000610351562,248.5714111328125
AudioSynthWaveformModulated OSC4;   //xy=648.5714721679688,830
AudioEffectEnvelope      VolEnvOsc3;     //xy=794,512
AudioEffectEnvelope      VolEnvOsc2;     //xy=799,249
AudioMixer4              mixerAM3;       //xy=799,573
AudioEffectEnvelope      VolEnvOsc1;     //xy=803,52
AudioMixer4              mixerAM2;       //xy=805,305
AudioEffectEnvelope      VolEnvOsc4;     //xy=807,830
AudioMixer4              mixerAM1;       //xy=810,106
AudioMixer4              mixerAM4;       //xy=817,887
AudioEffectMultiply      AM3;            //xy=972,518
AudioEffectMultiply      AM4;            //xy=982,837
AudioEffectMultiply      AM1;            //xy=1005,58
AudioEffectMultiply      AM2;            //xy=1008,253
AudioMixer4              mixerMASTER;    //xy=1233,299
AudioOutputI2S           i2s1;           //xy=1544,304
AudioConnection          patchCord1(noise1, EnvNoise1);
AudioConnection          patchCord2(noise2, EnvNoise2);
AudioConnection          patchCord3(waveform1, EnvWave1);
AudioConnection          patchCord4(noise3, EnvNoise3);
AudioConnection          patchCord5(waveform2, EnvWave2);
AudioConnection          patchCord6(waveform3, EnvWave3);
AudioConnection          patchCord7(PitchEnvDepthOsc1, PitchEnvOsc1);
AudioConnection          patchCord8(PitchEnvDepthOsc3, PitchEnvOsc3);
AudioConnection          patchCord9(PitchEnvDepthOsc2, PitchEnvOsc2);
AudioConnection          patchCord10(PitchEnvDepthOsc4, PitchEnvOsc4);
AudioConnection          patchCord11(noise4, EnvNoise4);
AudioConnection          patchCord12(waveform4, EnvWave4);
AudioConnection          patchCord13(mixerOSCtoOSC2, 0, mixerOSC2, 1);
AudioConnection          patchCord14(mixerOSCtoOSC3, 0, mixerOSC3, 1);
AudioConnection          patchCord15(mixerOSCtoOSC1, 0, mixerOSC1, 1);
AudioConnection          patchCord16(mixerOSCtoOSC4, 0, mixerOSC4, 1);
AudioConnection          patchCord17(PitchEnvOsc2, 0, mixerOSC2, 0);
AudioConnection          patchCord18(PitchEnvOsc3, 0, mixerOSC3, 0);
AudioConnection          patchCord19(PitchEnvOsc1, 0, mixerOSC1, 0);
AudioConnection          patchCord20(EnvNoise1, 0, mixerOSC1, 3);
AudioConnection          patchCord21(EnvWave1, 0, mixerOSC1, 2);
AudioConnection          patchCord22(EnvWave1, 0, OSC1, 1);
AudioConnection          patchCord23(EnvWave3, 0, mixerOSC3, 2);
AudioConnection          patchCord24(EnvWave3, 0, OSC3, 1);
AudioConnection          patchCord25(EnvNoise3, 0, mixerOSC3, 3);
AudioConnection          patchCord26(EnvWave2, 0, mixerOSC2, 2);
AudioConnection          patchCord27(EnvWave2, 0, OSC2, 1);
AudioConnection          patchCord28(PitchEnvOsc4, 0, mixerOSC4, 0);
AudioConnection          patchCord29(EnvNoise2, 0, mixerOSC2, 3);
AudioConnection          patchCord30(EnvWave4, 0, mixerOSC4, 2);
AudioConnection          patchCord31(EnvWave4, 0, OSC4, 1);
AudioConnection          patchCord32(EnvNoise4, 0, mixerOSC4, 3);
AudioConnection          patchCord33(mixerOSC1, 0, OSC1, 0);
AudioConnection          patchCord34(mixerOSC3, 0, OSC3, 0);
AudioConnection          patchCord35(mixerOSC2, 0, OSC2, 0);
AudioConnection          patchCord36(mixerOSC4, 0, OSC4, 0);
AudioConnection          patchCord37(WaveAM2, 0, mixerAM2, 0);
AudioConnection          patchCord38(WaveAM4, 0, mixerAM4, 0);
AudioConnection          patchCord39(AMdc4, 0, mixerAM4, 1);
AudioConnection          patchCord40(WaveAM3, 0, mixerAM3, 0);
AudioConnection          patchCord41(AMdc2, 0, mixerAM2, 1);
AudioConnection          patchCord42(WaveAM1, 0, mixerAM1, 0);
AudioConnection          patchCord43(AMdc3, 0, mixerAM3, 1);
AudioConnection          patchCord44(OSC3, VolEnvOsc3);
AudioConnection          patchCord45(AMdc1, 0, mixerAM1, 1);
AudioConnection          patchCord46(OSC1, VolEnvOsc1);
AudioConnection          patchCord47(OSC2, VolEnvOsc2);
AudioConnection          patchCord48(OSC4, VolEnvOsc4);
AudioConnection          patchCord49(VolEnvOsc3, 0, AM3, 0);
AudioConnection          patchCord50(VolEnvOsc2, 0, AM2, 0);
AudioConnection          patchCord51(mixerAM3, 0, AM3, 1);
AudioConnection          patchCord52(VolEnvOsc1, 0, AM1, 0);
AudioConnection          patchCord53(mixerAM2, 0, AM2, 1);
AudioConnection          patchCord54(VolEnvOsc4, 0, AM4, 0);
AudioConnection          patchCord55(mixerAM1, 0, AM1, 1);
AudioConnection          patchCord56(mixerAM4, 0, AM4, 1);
AudioConnection          patchCord57(AM3, 0, mixerOSCtoOSC1, 2);
AudioConnection          patchCord58(AM3, 0, mixerOSCtoOSC2, 2);
AudioConnection          patchCord59(AM3, 0, mixerOSCtoOSC3, 2);
AudioConnection          patchCord60(AM3, 0, mixerOSCtoOSC4, 2);
AudioConnection          patchCord61(AM3, 0, mixerMASTER, 2);
AudioConnection          patchCord62(AM4, 0, mixerMASTER, 3);
AudioConnection          patchCord63(AM4, 0, mixerOSCtoOSC1, 3);
AudioConnection          patchCord64(AM4, 0, mixerOSCtoOSC2, 3);
AudioConnection          patchCord65(AM4, 0, mixerOSCtoOSC3, 3);
AudioConnection          patchCord66(AM4, 0, mixerOSCtoOSC4, 3);
AudioConnection          patchCord67(AM1, 0, mixerMASTER, 0);
AudioConnection          patchCord68(AM1, 0, mixerOSCtoOSC1, 0);
AudioConnection          patchCord69(AM1, 0, mixerOSCtoOSC2, 0);
AudioConnection          patchCord70(AM1, 0, mixerOSCtoOSC3, 0);
AudioConnection          patchCord71(AM1, 0, mixerOSCtoOSC4, 0);
AudioConnection          patchCord72(AM2, 0, mixerOSCtoOSC1, 1);
AudioConnection          patchCord73(AM2, 0, mixerOSCtoOSC3, 1);
AudioConnection          patchCord74(AM2, 0, mixerOSCtoOSC2, 1);
AudioConnection          patchCord75(AM2, 0, mixerOSCtoOSC4, 1);
AudioConnection          patchCord76(AM2, 0, mixerMASTER, 1);
AudioConnection          patchCord77(mixerMASTER, 0, i2s1, 0);
AudioConnection          patchCord78(mixerMASTER, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1512,359
// GUItool: end automatically generated code

