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

struct ParameterValues{
  static constexpr unsigned c_itemsCount = 16;

  ParameterValues() : data_{} {
  }

  ParameterValues(const unsigned count, const float data[c_itemsCount] ) : data_{} {
    for(unsigned i = 0; i < count; ++i) {
      data_[i] = data[i];
    }
  }

  const ParameterValues & operator=(const ParameterValues & rhs) {
    memcpy(&data_[0], &rhs.data_[0], sizeof(data_));
    return *this;
  }

  void dump() const {
    Serial.printf("%f %f %f %f\n",
                  data_[0], data_[1], data_[2], data_[3]);
  }

  float data_[c_itemsCount];
};

static float Lerp(const float lhs, const float rhs, const float interpolationFactor) {
  return lhs * interpolationFactor + (1.0f - interpolationFactor) * rhs;
}
static unsigned Lerp(const unsigned lhs, const unsigned rhs, const float interpolationFactor) {
  return static_cast<int>(interpolationFactor * lhs + (1.0f - interpolationFactor) * rhs);
}
static bool Lerp(const bool lhs, const bool rhs, const float interpolationFactor) {
  return static_cast<bool>(interpolationFactor * lhs + (1.0f - interpolationFactor) * rhs);
}

float getRandom(float mean, float halfRange) {
  if(halfRange == 0.0f) {
    return mean;
  }
  const float rnd = static_cast<float>(random(2 * halfRange + 1));
  const float val = (mean - halfRange) + rnd;
  return abs(val);
}

unsigned getRandom(unsigned mean, unsigned halfRange) {
  return getRandom(static_cast<float>(mean), static_cast<float>(halfRange));
}

#endif // _HELPERS_H_

