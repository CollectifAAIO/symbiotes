/// @author fb
/// @copyright fb 2017
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

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MedianFilter.h>   // https://github.com/daPhoosa/MedianFilter
#include <RunningAverage.h> // https://github.com/RobTillaart/Arduino/tree/master/libraries/RunningAverage

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=66,217
AudioPlaySdWav           playSdWav1;     //xy=77,263
AudioAnalyzeRMS          rms1;           //xy=241,211
AudioOutputI2S           i2s1;           //xy=242,262
AudioConnection          patchCord1(i2s2, 0, rms1, 0);
AudioConnection          patchCord2(playSdWav1, 0, i2s1, 0);
AudioConnection          patchCord3(playSdWav1, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=74,170
// GUItool: end automatically generated code

//Use these with the audio adaptor board

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#define LED              13
#define PROXI_PIN        A3

// DECLARATION & INIT VARIABLES

// Settings fonctions

MedianFilter MedianProx(5, 0);
RunningAverage MicroRA(15);

// Déclaration des fonctions

void CalibProxiMic();
int CountingFile (File DirName);
float JaugePeak (bool DetectEvent, const int TimeWindow);
float JaugePassage (bool DetectEvent, float TimeWindow);
int URN(int nbtotfile);
void TrigFile(const char *filename);
void initSampleSelecteur(int nbtotfile);

// Variables données

int Proxi = 0;
uint32_t ResetTimeRandProxi = 0;
const int RANDTIMEPROXI = 1000; //  REGLAGE     Temps après lequel on lit une nouvelle valeur de random

float MicroRms = 0.0;
float Micro_Moyenne = 0.0;
int ProxiMedian = 0;

// Variables calib Proxi + Micro

const int TEMPS_CALIB = 5000 ;            // REGLAGE  : durée calibration.

int ProxiMin = 1023;                      // REGLAGE
int ProxiMax = 0;                     // REGLAGE
float MicroMin = 1023.0;

// Variable Compteur nombre de fichiers par dossier

File root;
int NbFiles[4] = {0};

// Variables JAUGES

const int TP_STOCKAGE_PASSAGE = 5000;      // REGLAGE (Temps de stockage de la jauge passage)
const int SEUILJAUGEPROXI = 9;            // REGLAGE (Seuil de changement d'humeur Proxi)
int ThreshPassage = 150;
bool DetectPassage = false;
float jaugePassage = 0.0;

const int TP_STOCKAGE_PEAK = 2000;          // REGLAGE (Temps de stockage de la jauge Micro)
const int SEUILJAUGEMICRO = 1;                    // REGLAGE (Seuil de changement d'humeur Micro)
float ThreshPeak = 20.0 ;                
bool DetectPeak = false;
float jaugePeak = 0.0;

// Valeurs de seuil de changement d'humeur + changement de rapidité de déclenchement

int Condition = 0;
uint32_t resetTimeThreshpeak = 0;
int conditionReset = 0;

// Variables PROXI Range

int randomMin;
int randomMax;

// Variables fonction TRIGFILE

uint32_t resetMillis = 0;
int threshTrig = 0;

int corpusSampleNumber = 10;   // Nombre de son dans chaque dossier

int sample_rand = 1;           // tirage au sort du numéro du sample.
String SoundFile, SoundType;
String HumeurFolder[4]  = {"serein", "timide", "hilare", "hostile"}; // ATTENTION !!! CHANGER LES ARRAYS DANS FONCTION URN SI ON AUGMENTE LE NOMBRE DE DOSSIERS D HUMEUR
int selecthumeur = 0;
int previoushumeur = 1000;
int statechangeselecthumeur = 0;

// Variables Freeze Timide

const int FREEZ_TIMIDE_TIME = 60000;           // REGLAGE (durée du mode timide)

