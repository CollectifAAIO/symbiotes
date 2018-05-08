#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <elapsedMillis.h>

// GUItool: begin automatically generated code
AudioSynthKarplusStrong  string1;        //xy=446.6666564941405,412.6666564941405
AudioMixer4              mixer1;         //xy=952.6666564941404,246.66665649414057
AudioOutputI2S           i2s1;           //xy=1301.3333740234375,262
AudioConnection          patchCord1(string1, 0, mixer1, 0);
AudioConnection          patchCord2(mixer1, 0, i2s1, 0);
AudioConnection          patchCord3(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=1311.3333740234375,339
// GUItool: end automatically generated code
