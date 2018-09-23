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

#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

class Sequencer {
 public:
  Sequencer()
  : counter_(0)
  , periodMs_(1000.0)
  , isNoteOn_(false) {}

  void setBpm(int bpm) {
    periodMs_ = 1000.0 * 60.0 / static_cast<float>(bpm);
  }

  void start() {
    counter_ = 0;
  }

  void update(FM4 & synth_) {
    if (counter_ >= periodMs_) {
      if (!isNoteOn_) {
        synth_.noteOn();
        isNoteOn_ = true;
        counter_ = 0;
      } else {
        synth_.noteOff();
        isNoteOn_ = false;
        counter_ = 0;
      }
    }
  }

 private:
  elapsedMillis counter_;
  float periodMs_;
  bool isNoteOn_;
};

#endif // _SEQUENCER_H_

