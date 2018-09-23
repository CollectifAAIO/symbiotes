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

#ifndef _GLOBAL_VARIABLES_H_
#define _GLOBAL_VARIABLES_H_

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

struct ParameterValues{
  ParameterValues() : data_{} {
  }

  const ParameterValues & operator=(const ParameterValues & rhs) {
    memcpy(&data_[0], &rhs.data_[0], sizeof(data_));
    return *this;
  }

  float data_[16];
};

#endif // _GLOBAL_VARIABLES_H_

