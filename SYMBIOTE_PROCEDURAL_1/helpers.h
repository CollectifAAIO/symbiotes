/// @author gm
/// @copyright gm 2018
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

#ifndef _HELPERS_H_
#define _HELPERS_H_

static float Lerp(const float lhs, const float rhs, const float interpolationFactor) {
  return lhs * interpolationFactor + (1.0f - interpolationFactor) * rhs;
}
static int Lerp(const int lhs, const int rhs, const float interpolationFactor) {
  return static_cast<int>(interpolationFactor * lhs + (1.0f - interpolationFactor) * rhs);
}
static bool Lerp(const bool lhs, const bool rhs, const float interpolationFactor) {
  return static_cast<bool>(interpolationFactor * lhs + (1.0f - interpolationFactor) * rhs);
}

float getRandom(float mean, float halfRange) const {
  if(halfRange == 0.0f) {
    return mean;
  }
  const float rnd = static_cast<float>(random(2 * halfRange + 1));
  const float val = (mean - halfRange) + rnd;
  return abs(val);
}

int getRandom(int mean, int halfRange) const {
  return getRandom(static_cast<float>(mean), static_cast<float>(halfRange));
}

#endif // _HELPERS_H_

