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

#include <elapsedMillis.h>

// DECLARATION & INIT VARIABLES

// Settings functions

const int TEMPS_CALIB = 5000; //SETTING (en ms)

constexpr float c_modRange = 8.0;
// log(5)
constexpr float c_modRangeFactor = c_modRange * 0.69897000433601880478626110527551;

#endif // _GLOBAL_VARIABLES_H_

