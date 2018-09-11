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

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// Def In & Out
#define LED              13
#define PROXI_PIN        A3

// Include Lib
#include <MedianFilter.h>   // https://github.com/daPhoosa/MedianFilter
#include <RunningAverage.h> // https://github.com/RobTillaart/Arduino/tree/master/libraries/RunningAverage
#include <elapsedMillis.h>

// DECLARATION FONCTIONS

float Proxi();
void FM4_synth (bool Note_on, bool Note_off, int Macro);

// DECLARATION & INIT VARIABLES

// Settings functions

MedianFilter MedianProx(5, 0);
RunningAverage MicroRA(15);

// MACROS

float MACROExpressivite = 0.0;
int   MACRODensity;

// Calib

bool Calib = true;

float Proximeter = 0.0;
float ProxiMedian = 0.0;
float ProxiClip = 0.0;
float ProxiScale = 0.0;

int ProxiMin = 1023;
float ProxiMax = 0.0;

const int TEMPS_CALIB = 5000; //SETTING (en ms)

// Time variables

elapsedMillis TimeNoteElapsed = 0;
elapsedMillis RandomDensiteTimeElapsed = 0;
elapsedMillis MonitorTimeElapsed = 0;

bool Note_On = false;
bool Note_Off = false;
bool ChangeNoteOn = false;

int MinTimeNoteOn = 40;    // SETTING
int MaxTimeNoteOnBorneMin = 100 ; // SETTING
int MaxTimeNoteOnBorneMax = 15000; // SETTING
int MinTimeNoteOff = 100;   // SETTING
int MaxTimeNoteOff = 1000; // SETTING

int randomNoteOnTime = 0;
int randomNoteOffTime = MaxTimeNoteOnBorneMax + 1;

int RandomDensiteTimeCycle = 1000; // SETTING (en ms)


int RefreshScreen = 100;  // SETTING (en ms)

// FM4_SYNTH GLOBAL VARIABLES

int randDecayValue;
bool NoteOffStateTime = 0;
bool NoteOffStateTimePrevious = 0;

uint32_t noteTime;
uint32_t resetNoteOffTime = 0;

const char WAVEFORM[8] = {WAVEFORM_SINE, WAVEFORM_SAWTOOTH, WAVEFORM_SAWTOOTH_REVERSE, WAVEFORM_SQUARE, WAVEFORM_TRIANGLE, WAVEFORM_TRIANGLE_VARIABLE, WAVEFORM_PULSE, WAVEFORM_SAMPLE_HOLD};


