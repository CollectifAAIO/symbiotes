/// @author rb
/// @copyright rb 2018
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


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 3:
  int proxiValue = analogRead(A3);
  int micValue = analogRead(A13);
  // print out the value you read:
  int proxi = map(proxiValue, 0, 1023, 0, 100); //mapping values
  int mic = map(micValue, 0, 1023, 101, 200);
  Serial.write(proxi);
  Serial.write(mic);
  delay(50);        // delay in between reads for stability
}
