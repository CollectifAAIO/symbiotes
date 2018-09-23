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


// Def In & Out
#define PROXI_PIN        A3

// Include Lib
#include <MedianFilter.h>   // https://github.com/daPhoosa/MedianFilter
#include <RunningAverage.h> // https://github.com/RobTillaart/Arduino/tree/master/libraries/RunningAverage
#include <elapsedMillis.h>

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
elapsedMillis MonitorTimeElapsed = 0;

int MinTimeNoteOn = 40;    // SETTING
int MaxTimeNoteOnBorneMin = 100 ; // SETTING
int MaxTimeNoteOnBorneMax = 15000; // SETTING
int MinTimeNoteOff = 100;   // SETTING
int MaxTimeNoteOff = 1000; // SETTING

int randomNoteOffTime = 0;

const char WAVEFORM[8] = {WAVEFORM_SINE, WAVEFORM_SQUARE, WAVEFORM_SAWTOOTH, WAVEFORM_PULSE, WAVEFORM_TRIANGLE, WAVEFORM_SAWTOOTH_REVERSE, WAVEFORM_TRIANGLE_VARIABLE, WAVEFORM_SAMPLE_HOLD};


