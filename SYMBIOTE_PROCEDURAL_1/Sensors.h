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

enum MicDetectionParameterIndex {
  mic_Threshold,
  mic_Hysteresis,
  mic_WindowLength,
  mic_Count
};

class MicDetection {
public:
  static constexpr float c_extremeMin = 0.0;
  static constexpr float c_extremeMax = 1.0;
  MicDetection(
    float Threshold = 0.5,
    float Hysteresis = 0.8,
    unsigned WindowLength = 50) :
  Threshold_(Threshold),
  Hysteresis_(Hysteresis),
  WindowLength_(WindowLength),
  median_(5, 0),
  average_(50),
  calibMin_(1.0),
  calibMax_(0.0),
  lastValue_(0.0),
  lastAverage_(0.0),
  hasHitMax_(false) {
    average_.clear();
  }

  void setIndexedParameter(const MicDetectionParameterIndex parmIndex, const ParameterValues & parmValues ) {
    switch(parmIndex) {
    case mic_Threshold:{
      Threshold_ = parmValues.data_[0];
      break;
    }
    case mic_Hysteresis:{
      Hysteresis_ = parmValues.data_[0];
      break;
    }
    case mic_WindowLength:{
      WindowLength_ = static_cast<unsigned>(parmValues.data_[0]);
      break;
    }
    default:{
      Serial.println("Bad parameter index.");
      break;
    }
    }
  }

  void update() {
    delay(50);
    const float current = rms.read();
    // Median filter
    const float currentMedian = current;//0.01 * median_.in(current * 100);
    // Autocalibration
    calibMin_ = min(currentMedian, calibMin_);
    calibMax_ = max(currentMedian, calibMax_);

    // invert direction
    lastValue_ = currentMedian;//1.0 - currentMedian;

    // Running average
    average_.addValue(currentMedian);
    const float average = average_.getAverage();

    // clip
    const float clippedCurrent = min(c_extremeMax, max(c_extremeMin, average));
    // scale
    const float normalisedCurrent = c_extremeMin + clippedCurrent * (c_extremeMax - c_extremeMin);
    lastAverage_ = normalisedCurrent;

    // Variability detection
    const float peakValue = abs(lastAverage_ - lastValue_);
    Serial.println(peakValue);
    if (hasHitMax_) {
     if (peakValue < (Hysteresis_ * Threshold_)) {
        hasDetected_ = true;
        // reset state
        hasHitMax_ = false;
     }
    } else {
      hasDetected_ = false;
      hasHitMax_ = peakValue >= Threshold_;
    if (hasHitMax_) {
      Serial.println("hasHitMax_");
    }
    }
  }

  bool hasDetected() const {
    if (hasDetected_) {
      Serial.println("trigger");
    }
    return hasDetected_;
  }

  void dump() const {
    Serial.printf("Threshold_: %f; Hysteresis_: %f; WindowLength_: %d; calibMin_: %f; calibMax_: %f; lastValue_: %f; lastAverage_: %f;\n",
                  Threshold_, Hysteresis_, WindowLength_, calibMin_, calibMax_, lastValue_, lastAverage_);
  }

private:
  float Threshold_;
  float Hysteresis_;
  unsigned WindowLength_;
  MedianFilter median_;
  RunningAverage average_;
  float calibMin_;
  float calibMax_;
  float lastValue_;
  float lastAverage_;
  bool hasHitMax_;
  bool hasDetected_;
};


// CALIB MICRO & RETURN VALUE



