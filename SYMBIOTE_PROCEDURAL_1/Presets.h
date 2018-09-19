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

#ifndef _PRESETS_H_
#define _PRESETS_H_

static const String c_tokens[] = {
  "waveform",
  "glide",
  "Pitch",
  "glide_rand",
  "Pitch_rand",
  "FM_Osc1",
  "FM_Osc2",
  "FM_Osc3",
  "FM_Osc4",
  "FM_Osc1_rand",
  "FM_Osc2_rand",
  "FM_Osc3_rand",
  "FM_Osc4_rand",
  "AM_Waveform",
  "AM_Depth",
  "AM_Freq",
  "AM_Depth_rand",
  "AM_Freq_rand",
  "PADSR_Dlay",
  "PADSR_Amp",
  "PADSR_Atk",
  "PADSR_Dcay",
  "PADSR_Sus",
  "PADSR_Rel",
  "PADSR_Dlay_rand",
  "PADSR_Amp_rand",
  "PADSR_Atk_rand",
  "PADSR_Dcay_rand",
  "PADSR_Sus_rand",
  "PADSR_Rel_rand",
  "LADSR_Dlay",
  "LADSR_Amp",
  "LADSR_Atk",
  "LADSR_Dcay",
  "LADSR_Sus",
  "LADSR_Rel",
  "LADSR_Dlay_rand",
  "LADSR_Amp_rand",
  "LADSR_Atk_rand",
  "LADSR_Dcay_rand",
  "LADSR_Sus_rand",
  "LADSR_Rel_rand",
  "Vol"
};
enum parameterIndex {
  waveform,
  glide,
  Pitch,
  glide_rand,
  Pitch_rand,
  FM_Osc1,
  FM_Osc2,
  FM_Osc3,
  FM_Osc4,
  FM_Osc1_rand,
  FM_Osc2_rand,
  FM_Osc3_rand,
  FM_Osc4_rand,
  AM_Waveform,
  AM_Depth,
  AM_Freq,
  AM_Depth_rand,
  AM_Freq_rand,
  PADSR_Dlay,
  PADSR_Amp,
  PADSR_Atk,
  PADSR_Dcay,
  PADSR_Sus,
  PADSR_Rel,
  PADSR_Dlay_rand,
  PADSR_Amp_rand,
  PADSR_Atk_rand,
  PADSR_Dcay_rand,
  PADSR_Sus_rand,
  PADSR_Rel_rand,
  LADSR_Dlay,
  LADSR_Amp,
  LADSR_Atk,
  LADSR_Dcay,
  LADSR_Sus,
  LADSR_Rel,
  LADSR_Dlay_rand,
  LADSR_Amp_rand,
  ADSR_Atk_rand,
  LADSR_Dcay_rand,
  LADSR_Sus_rand,
  LADSR_Rel_rand,
  Vol
};
constexpr int c_tokensCount = sizeof(c_tokens) / sizeof(String);

bool ParseParameter(int & _outStripIndex, parameterIndex & _outParmIndex, float & _outParmValue) {
  if(Serial.available()) {
    const String data = Serial.readString();
    char stripIndexChar = data[0];
    if (isDigit(stripIndexChar)) {
      // 48 => "0" in ASCII
      const int stripIndex = stripIndexChar - 48;

      int cursor = 1;
      const int dataLength = data.length();
      String token;
      token.reserve(dataLength);
      while(cursor < dataLength && data[cursor] != ' ' && data[cursor] != '\n' && !isDigit(data[cursor])) {
        token += data[cursor];
        cursor += 1;
      }
      //Serial.printf("%d - %s\n", stripIndex, token.c_str());
      int tokenIdx = 0;
      for (; tokenIdx < c_tokensCount; ++tokenIdx) {
        //Serial.printf("%s %s %d\n", token.c_str(), c_tokens[tokenIdx].c_str(), tokenIdx);
        if(token == c_tokens[tokenIdx]) {
          break;
        }
      }
      if(tokenIdx >= c_tokensCount) {
        Serial.println("Token not found");
        return false;
      }
      if(cursor == dataLength) {
        Serial.println("Missing value!");
        return false;
      }
      //Serial.printf("%s",data.substring(cursor).c_str());
      const float value = data.substring(cursor).toFloat();
      _outStripIndex = stripIndex;
      _outParmIndex = static_cast<parameterIndex>(tokenIdx);
      _outParmValue = value;
      Serial.printf("%d - %s - %d - %f\n", stripIndex, token.c_str(), tokenIdx, value);
      return true;
    }
  }
  return false;
}

#endif // _PRESETS_H_

