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


// CALIBRATION PROXI RETURN VALUE BETWEEN 0 & 100

MedianFilter MedianProx(5, 0);

void ProxiSetup() {
  elapsedMillis TimeElapsed = 0;
  Serial.println(" >>>>>>>>>>> Début de Calibration <<<<<<<<<< ");

  while (TimeElapsed < TEMPS_CALIB) {

    //    MicroRms = rms1.read() * 1000.0;
    //    MicroRA.addValue(MicroRms);
    //    Micro_Moyenne = MicroRA.getAverage();

    Proximeter = analogRead(PROXI_PIN);
    MedianProx.in(Proximeter);
    ProxiMedian = MedianProx.out();

    // record the minimum Proxi Value
    if (Proximeter < ProxiMin) {
      ProxiMin = Proximeter;
    }
    if (ProxiMedian > ProxiMax) {
      ProxiMax = ProxiMedian;
    }
    delay(10);
  }
  if (ProxiMin > ProxiMax / 2) ProxiMin = ProxiMax / 2; // If the calibration doesn't occure, setup a value which is equal to the maximum range divided by 2.
  Serial.print("ProxiMin : ");
  Serial.println(ProxiMin);
  Serial.print("ProxiMax : ");
  Serial.println(ProxiMax);

  Serial.println(" >>>>>>>>>>> Fin de Calibration <<<<<<<<<< ");
}

  // DATA SENSORS
float Proxi() {
  Proximeter = analogRead(PROXI_PIN);
  MedianProx.in(Proximeter);
  ProxiMedian = MedianProx.out();
  ProxiClip = constrain(ProxiMedian, ProxiMin, ProxiMax);
  ProxiScale = map(ProxiClip, ProxiMin, ProxiMax, 0, 1.0);
  return ProxiScale;
}

// CALIB MICRO & RETURN VALUE

// record the minimum Micro Value
//    if (Micro_Moyenne < MicroMin) {
//      MicroMin = Micro_Moyenne;
//    }

