# @author gm
# @copyright gm 2018
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser Public License for more details.
#
# You should have received a copy of the GNU Lesser Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import os
import re

c_tokens = [
  "waveform",
  "glide",
  "Pitch",
  "glide_rand",
  "Pitch_rand",
  "ListenSeq",
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
  "Vol",
  "RandomizeSeqOrSound",
  "loop",
  "bpm",
  "RestartFrom0",
  "StepNumber",
  "RandomSpeed",
  "octave",
  "arpeg",
  "random-trig",
  "min-trigger-time",
  "max-trigger-time"
]

def DumpFloatArrayValues(data):
  out = str(data).strip('[]').replace(' ', ',')
  return (out.count(',') + 1,out)

def ParseInputFile(path):
  thisScriptDir = os.path.dirname(os.path.realpath(__file__))
  outFilePath = os.path.join(os.path.dirname(thisScriptDir), "preset_data.h")
  if os.path.exists(path):
    reference = re.compile(r'([1-4]*)\-?pst\-([a-zA-Z\_\-0-9]+) ([0-9\.\- ]+)')
    out = []
    inFile = open(path, 'r')
    for line in inFile:
      foundMatches = reference.match(line)
      if(foundMatches):
        out.append(foundMatches.groups())
    outFile = open(outFilePath, 'w')
    for item in out:
      (numValues, values) = DumpFloatArrayValues(item[2])
      if(item[0]):
        outFile.write("{{{0}, {1}, {2}, {{ {3} }} }},\n".format(item[0], c_tokens.index(item[1]), numValues, values))
      else:
        outFile.write("{{9, {0}, {1}, {{ {2} }} }},\n".format(c_tokens.index(item[1]), numValues, values))
    print("Written {0} items.\n".format(len(out)))

if __name__ == "__main__":
  import sys

  ParseInputFile(sys.argv[1])
