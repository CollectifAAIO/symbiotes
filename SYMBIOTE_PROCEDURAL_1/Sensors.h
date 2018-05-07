// CALIBRATION PROXI RETURN VALUE BETWEEN 0 & 100

float Proxi() {
  if (Calib == true) {
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
    Calib = false;
  }

  // DATA SENSORS

  Proximeter = analogRead(PROXI_PIN);
  MedianProx.in(Proximeter);
  ProxiMedian = MedianProx.out();
  ProxiClip = constrain(ProxiMedian, ProxiMin, ProxiMax);
  ProxiScale = map(ProxiClip, ProxiMin, ProxiMax, 0, 100);
  return ProxiScale;
}


// CALIB MICRO & RETURN VALUE



// record the minimum Micro Value
//    if (Micro_Moyenne < MicroMin) {
//      MicroMin = Micro_Moyenne;
//    }

