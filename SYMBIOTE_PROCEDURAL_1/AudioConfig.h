#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthNoiseWhite     Noise;         //xy=58.33331298828125,390.66668701171875
AudioSynthWaveform       waveform1;      //xy=65.6666259765625,197.6666717529297
AudioSynthWaveform       waveform2;      //xy=69.49993896484375,442
AudioSynthWaveform       waveform3;      //xy=78.83346557617188,716.666748046875
AudioSynthWaveform       waveform4;      //xy=84.83328247070312,967.6666870117188
AudioSynthWaveformDc     PitchEnvDepthOsc1; //xy=92.5,45
AudioSynthWaveformDc     PitchEnvDepthOsc2; //xy=106.16656494140625,282.33331298828125
AudioSynthWaveformDc     PitchEnvDepthOsc3; //xy=107.50006103515625,550.3333129882812
AudioSynthWaveformDc     PitchEnvDepthOsc4; //xy=110.5,807
AudioEffectEnvelope      EnvShapeMod1;       //xy=269.83343505859375,197.66673278808594
AudioMixer4              mixerOSCtoOSC2; //xy=272.16656494140625,334.33331298828125
AudioMixer4              mixerOSCtoOSC1; //xy=274.5,97
AudioMixer4              mixerOSCtoOSC3; //xy=277.50006103515625,605.3333129882812
AudioEffectEnvelope      EnvShapeMod3;       //xy=281.8334655761719,716.3333740234375
AudioMixer4              mixerOSCtoOSC4; //xy=281.5,855
AudioEffectEnvelope      EnvShapeMod2;       //xy=283.8334045410156,440.3333740234375
AudioEffectEnvelope      PitchEnvOsc2;   //xy=286.16656494140625,282.33331298828125
AudioEffectEnvelope      PitchEnvOsc1;   //xy=287.5,45
AudioEffectEnvelope      EnvNoise1;      //xy=288.5,149.66664123535156
AudioEffectEnvelope      PitchEnvOsc3;   //xy=288.50006103515625,549.3333129882812
AudioEffectEnvelope      EnvNoise2;      //xy=293.1665954589844,388.33331298828125
AudioEffectEnvelope      PitchEnvOsc4;   //xy=294.5,807
AudioEffectEnvelope      EnvNoise3;      //xy=298.50006103515625,658.6666870117188
AudioEffectEnvelope      EnvNoise4;      //xy=303.5,908.6665649414062
AudioEffectEnvelope      EnvShapeMod4;       //xy=303.5000305175781,967.6666870117188
AudioMixer4              mixerOSC1;      //xy=491.5,41
AudioMixer4              mixerOSC3;      //xy=495.50006103515625,529.3333129882812
AudioMixer4              mixerOSC2;      //xy=497.16656494140625,271.33331298828125
AudioMixer4              mixerOSC4;      //xy=504.5,819
AudioSynthWaveform       WaveAM1;        //xy=627.5,83.66667175292969
AudioSynthWaveform       WaveAM3;        //xy=627.5000610351562,579
AudioSynthWaveform       WaveAM2;        //xy=629.8331909179688,316.6667175292969
AudioSynthWaveformModulated OSC1;           //xy=632.5,48
AudioSynthWaveformDc     AMdc1;          //xy=634.1666870117188,120.33334350585938
AudioSynthWaveformModulated OSC3;           //xy=634.5000610351562,536.3333129882812
AudioSynthWaveformModulated OSC2;           //xy=636.1665649414062,278.33331298828125
AudioSynthWaveformDc     AMdc3;          //xy=635.1666870117188,615.3333129882812
AudioSynthWaveformDc     AMdc2;          //xy=636.4999389648438,350.6666259765625
AudioSynthWaveform       WaveAM4;        //xy=644.8333129882812,865
AudioSynthWaveformDc     AMdc4;          //xy=649.5,901.9999389648438
AudioSynthWaveformModulated OSC4;           //xy=652.5,827
AudioEffectEnvelope      VolEnvOsc3;     //xy=793.5000610351562,537.3333129882812
AudioEffectEnvelope      VolEnvOsc2;     //xy=795.1665649414062,279.33331298828125
AudioEffectEnvelope      VolEnvOsc1;     //xy=797.5,49
AudioMixer4              mixerAM3;       //xy=798.5000610351562,598.3333129882812
AudioMixer4              mixerAM2;       //xy=801.1665649414062,335.33331298828125
AudioEffectEnvelope      VolEnvOsc4;     //xy=801.5,827
AudioMixer4              mixerAM1;       //xy=804.5,103
AudioMixer4              mixerAM4;       //xy=811.5,884
AudioEffectMultiply      AM4;            //xy=976.5,834
AudioEffectMultiply      AM1;            //xy=999.5,55
AudioEffectMultiply      AM2;            //xy=1004.1665649414062,283.33331298828125
AudioEffectMultiply      AM3;            //xy=1004.8334350585938,543.3333129882812
AudioMixer4              mixerMASTER;    //xy=1219.166748046875,422.6666564941406
AudioOutputI2S           i2s1;           //xy=1371.83349609375,429.33331298828125
AudioConnection          patchCord1(Noise, EnvNoise1);
AudioConnection          patchCord2(Noise, EnvNoise2);
AudioConnection          patchCord3(Noise, EnvNoise3);
AudioConnection          patchCord4(Noise, EnvNoise4);
AudioConnection          patchCord5(waveform1, EnvShapeMod1);
AudioConnection          patchCord6(waveform2, EnvShapeMod2);
AudioConnection          patchCord7(waveform3, EnvShapeMod3);
AudioConnection          patchCord8(waveform4, EnvShapeMod4);
AudioConnection          patchCord9(PitchEnvDepthOsc1, PitchEnvOsc1);
AudioConnection          patchCord10(PitchEnvDepthOsc2, PitchEnvOsc2);
AudioConnection          patchCord11(PitchEnvDepthOsc3, PitchEnvOsc3);
AudioConnection          patchCord12(PitchEnvDepthOsc4, PitchEnvOsc4);
AudioConnection          patchCord13(EnvShapeMod1, 0, OSC1, 1);
AudioConnection          patchCord14(mixerOSCtoOSC2, 0, mixerOSC2, 1);
AudioConnection          patchCord15(mixerOSCtoOSC1, 0, mixerOSC1, 1);
AudioConnection          patchCord16(mixerOSCtoOSC3, 0, mixerOSC3, 1);
AudioConnection          patchCord17(EnvShapeMod3, 0, OSC3, 1);
AudioConnection          patchCord18(mixerOSCtoOSC4, 0, mixerOSC4, 1);
AudioConnection          patchCord19(EnvShapeMod2, 0, OSC2, 1);
AudioConnection          patchCord20(PitchEnvOsc2, 0, mixerOSC2, 0);
AudioConnection          patchCord21(PitchEnvOsc1, 0, mixerOSC1, 0);
AudioConnection          patchCord22(EnvNoise1, 0, mixerOSC1, 2);
AudioConnection          patchCord23(PitchEnvOsc3, 0, mixerOSC3, 0);
AudioConnection          patchCord24(EnvNoise2, 0, mixerOSC2, 2);
AudioConnection          patchCord25(PitchEnvOsc4, 0, mixerOSC4, 0);
AudioConnection          patchCord26(EnvNoise3, 0, mixerOSC3, 2);
AudioConnection          patchCord27(EnvNoise4, 0, mixerOSC4, 2);
AudioConnection          patchCord28(EnvShapeMod4, 0, OSC4, 1);
AudioConnection          patchCord29(mixerOSC1, 0, OSC1, 0);
AudioConnection          patchCord30(mixerOSC3, 0, OSC3, 0);
AudioConnection          patchCord31(mixerOSC2, 0, OSC2, 0);
AudioConnection          patchCord32(mixerOSC4, 0, OSC4, 0);
AudioConnection          patchCord33(WaveAM1, 0, mixerAM1, 0);
AudioConnection          patchCord34(WaveAM3, 0, mixerAM3, 0);
AudioConnection          patchCord35(WaveAM2, 0, mixerAM2, 0);
AudioConnection          patchCord36(OSC1, VolEnvOsc1);
AudioConnection          patchCord37(AMdc1, 0, mixerAM1, 1);
AudioConnection          patchCord38(OSC3, VolEnvOsc3);
AudioConnection          patchCord39(OSC2, VolEnvOsc2);
AudioConnection          patchCord40(AMdc3, 0, mixerAM3, 1);
AudioConnection          patchCord41(AMdc2, 0, mixerAM2, 1);
AudioConnection          patchCord42(WaveAM4, 0, mixerAM4, 0);
AudioConnection          patchCord43(AMdc4, 0, mixerAM4, 1);
AudioConnection          patchCord44(OSC4, VolEnvOsc4);
AudioConnection          patchCord45(VolEnvOsc3, 0, AM3, 0);
AudioConnection          patchCord46(VolEnvOsc2, 0, AM2, 0);
AudioConnection          patchCord47(VolEnvOsc1, 0, AM1, 0);
AudioConnection          patchCord48(mixerAM3, 0, AM3, 1);
AudioConnection          patchCord49(mixerAM2, 0, AM2, 1);
AudioConnection          patchCord50(VolEnvOsc4, 0, AM4, 0);
AudioConnection          patchCord51(mixerAM1, 0, AM1, 1);
AudioConnection          patchCord52(mixerAM4, 0, AM4, 1);
AudioConnection          patchCord53(AM4, 0, mixerMASTER, 3);
AudioConnection          patchCord54(AM4, 0, mixerOSCtoOSC1, 3);
AudioConnection          patchCord55(AM4, 0, mixerOSCtoOSC2, 3);
AudioConnection          patchCord56(AM4, 0, mixerOSCtoOSC3, 3);
AudioConnection          patchCord57(AM4, 0, mixerOSCtoOSC4, 3);
AudioConnection          patchCord58(AM1, 0, mixerMASTER, 0);
AudioConnection          patchCord59(AM1, 0, mixerOSCtoOSC1, 0);
AudioConnection          patchCord60(AM1, 0, mixerOSCtoOSC2, 0);
AudioConnection          patchCord61(AM1, 0, mixerOSCtoOSC3, 0);
AudioConnection          patchCord62(AM1, 0, mixerOSCtoOSC4, 0);
AudioConnection          patchCord63(AM2, 0, mixerOSCtoOSC1, 1);
AudioConnection          patchCord64(AM2, 0, mixerOSCtoOSC3, 1);
AudioConnection          patchCord65(AM2, 0, mixerOSCtoOSC2, 1);
AudioConnection          patchCord66(AM2, 0, mixerOSCtoOSC4, 1);
AudioConnection          patchCord67(AM2, 0, mixerMASTER, 1);
AudioConnection          patchCord68(AM3, 0, mixerOSCtoOSC1, 2);
AudioConnection          patchCord69(AM3, 0, mixerOSCtoOSC2, 2);
AudioConnection          patchCord70(AM3, 0, mixerOSCtoOSC3, 2);
AudioConnection          patchCord71(AM3, 0, mixerOSCtoOSC4, 2);
AudioConnection          patchCord72(AM3, 0, mixerMASTER, 2);
AudioConnection          patchCord73(mixerMASTER, 0, i2s1, 0);
AudioConnection          patchCord74(mixerMASTER, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1363.1666259765625,490.9999694824219
// GUItool: end automatically generated code

