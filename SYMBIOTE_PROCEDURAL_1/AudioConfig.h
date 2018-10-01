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

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#define AUDIO_DUMP

// Peak detection for trigger
AudioInputI2S     audioInput;
AudioAnalyzePeak  peak1;
AudioConnection   patchCord(audioInput, peak1);

// Synth 1
AudioSynthWaveformDc     PitchEnvDepthOsc1; //xy=92.5,45
AudioEffectEnvelope      PitchEnvOsc1;   //xy=287.5,45
AudioMixer4              mixerOSCtoOSC1; //xy=274.5,97
AudioMixer4              mixerOSC1;      //xy=491.5,41
AudioSynthWaveformModulated OSC1;           //xy=632.5,48
AudioSynthWaveform       WaveAM1;        //xy=627.5,83.66667175292969
AudioSynthWaveformDc     AMdc1;          //xy=634.1666870117188,120.33334350585938
AudioEffectEnvelope      VolEnvOsc1;     //xy=797.5,49
AudioMixer4              mixerAM1;       //xy=804.5,103
AudioEffectMultiply      AM1;            //xy=999.5,55

AudioConnection          patchCord9(PitchEnvDepthOsc1, PitchEnvOsc1);
AudioConnection          patchCord15(mixerOSCtoOSC1, 0, mixerOSC1, 1);
AudioConnection          patchCord21(PitchEnvOsc1, 0, mixerOSC1, 0);
AudioConnection          patchCord29(mixerOSC1, 0, OSC1, 0);
AudioConnection          patchCord33(WaveAM1, 0, mixerAM1, 0);
AudioConnection          patchCord36(OSC1, VolEnvOsc1);
AudioConnection          patchCord37(AMdc1, 0, mixerAM1, 1);
AudioConnection          patchCord47(VolEnvOsc1, 0, AM1, 0);
AudioConnection          patchCord51(mixerAM1, 0, AM1, 1);

// Synth 2
AudioSynthWaveformDc     PitchEnvDepthOsc2; //xy=106.16656494140625,282.33331298828125
AudioEffectEnvelope      PitchEnvOsc2;   //xy=286.16656494140625,282.33331298828125
AudioMixer4              mixerOSCtoOSC2; //xy=272.16656494140625,334.33331298828125
AudioMixer4              mixerOSC2;      //xy=497.16656494140625,271.33331298828125
AudioSynthWaveformModulated OSC2;           //xy=636.1665649414062,278.33331298828125
AudioSynthWaveform       WaveAM2;        //xy=629.8331909179688,316.6667175292969
AudioSynthWaveformDc     AMdc2;          //xy=636.4999389648438,350.6666259765625
AudioEffectEnvelope      VolEnvOsc2;     //xy=795.1665649414062,279.33331298828125
AudioMixer4              mixerAM2;       //xy=801.1665649414062,335.33331298828125
AudioEffectMultiply      AM2;            //xy=1004.1665649414062,283.33331298828125

AudioConnection          patchCord10(PitchEnvDepthOsc2, PitchEnvOsc2);
AudioConnection          patchCord14(mixerOSCtoOSC2, 0, mixerOSC2, 1);
AudioConnection          patchCord20(PitchEnvOsc2, 0, mixerOSC2, 0);
AudioConnection          patchCord31(mixerOSC2, 0, OSC2, 0);
AudioConnection          patchCord35(WaveAM2, 0, mixerAM2, 0);
AudioConnection          patchCord39(OSC2, VolEnvOsc2);
AudioConnection          patchCord41(AMdc2, 0, mixerAM2, 1);
AudioConnection          patchCord46(VolEnvOsc2, 0, AM2, 0);
AudioConnection          patchCord49(mixerAM2, 0, AM2, 1);

// Synth 3
AudioSynthWaveformDc     PitchEnvDepthOsc3; //xy=107.50006103515625,550.3333129882812
AudioEffectEnvelope      PitchEnvOsc3;   //xy=288.50006103515625,549.3333129882812
AudioMixer4              mixerOSCtoOSC3; //xy=277.50006103515625,605.3333129882812
AudioMixer4              mixerOSC3;      //xy=495.50006103515625,529.3333129882812
AudioSynthWaveformModulated OSC3;           //xy=634.5000610351562,536.3333129882812
AudioSynthWaveform       WaveAM3;        //xy=627.5000610351562,579
AudioSynthWaveformDc     AMdc3;          //xy=635.1666870117188,615.3333129882812
AudioEffectEnvelope      VolEnvOsc3;     //xy=793.5000610351562,537.3333129882812
AudioMixer4              mixerAM3;       //xy=798.5000610351562,598.3333129882812
AudioEffectMultiply      AM3;            //xy=1004.8334350585938,543.3333129882812

AudioConnection          patchCord11(PitchEnvDepthOsc3, PitchEnvOsc3);
AudioConnection          patchCord16(mixerOSCtoOSC3, 0, mixerOSC3, 1);
AudioConnection          patchCord23(PitchEnvOsc3, 0, mixerOSC3, 0);
AudioConnection          patchCord30(mixerOSC3, 0, OSC3, 0);
AudioConnection          patchCord34(WaveAM3, 0, mixerAM3, 0);
AudioConnection          patchCord38(OSC3, VolEnvOsc3);
AudioConnection          patchCord40(AMdc3, 0, mixerAM3, 1);
AudioConnection          patchCord45(VolEnvOsc3, 0, AM3, 0);
AudioConnection          patchCord48(mixerAM3, 0, AM3, 1);

// Synth 4
AudioSynthWaveformDc     PitchEnvDepthOsc4; //xy=110.5,807
AudioEffectEnvelope      PitchEnvOsc4;   //xy=294.5,807
AudioMixer4              mixerOSCtoOSC4; //xy=281.5,855
AudioMixer4              mixerOSC4;      //xy=504.5,819
AudioSynthWaveformModulated OSC4;           //xy=652.5,827
AudioSynthWaveform       WaveAM4;        //xy=644.8333129882812,865
AudioSynthWaveformDc     AMdc4;          //xy=649.5,901.9999389648438
AudioEffectEnvelope      VolEnvOsc4;     //xy=801.5,827
AudioMixer4              mixerAM4;       //xy=811.5,884
AudioEffectMultiply      AM4;            //xy=976.5,834

AudioConnection          patchCord12(PitchEnvDepthOsc4, PitchEnvOsc4);
AudioConnection          patchCord18(mixerOSCtoOSC4, 0, mixerOSC4, 1);
AudioConnection          patchCord25(PitchEnvOsc4, 0, mixerOSC4, 0);
AudioConnection          patchCord32(mixerOSC4, 0, OSC4, 0);
AudioConnection          patchCord42(WaveAM4, 0, mixerAM4, 0);
AudioConnection          patchCord43(AMdc4, 0, mixerAM4, 1);
AudioConnection          patchCord44(OSC4, VolEnvOsc4);
AudioConnection          patchCord50(VolEnvOsc4, 0, AM4, 0);
AudioConnection          patchCord52(mixerAM4, 0, AM4, 1);

AudioMixer4              mixerMASTER;    //xy=1219.166748046875,422.6666564941406
AudioOutputI2S           i2s1;           //xy=1371.83349609375,429.33331298828125
AudioControlSGTL5000     sgtl5000_1;     //xy=1363.1666259765625,490.9999694824219

AudioSynthNoiseWhite     Noise;         //xy=58.33331298828125,390.66668701171875
AudioConnection          patchCord1(Noise, 0, mixerOSC1, 2);
AudioConnection          patchCord2(Noise, 0, mixerOSC2, 2);
AudioConnection          patchCord3(Noise, 0, mixerOSC3, 2);
AudioConnection          patchCord4(Noise, 0, mixerOSC4, 2);

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

#ifdef AUDIO_DUMP
AudioOutputUSB           usb1;
AudioConnection          patchCordAudioDump(mixerMASTER, 0, usb1, 0);
#endif // AUDIO_DUMP

