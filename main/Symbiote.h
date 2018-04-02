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
#define PROXI_PIN        A1

// DECLARATION & INIT VARIABLES

// Settings fonctions

MedianFilter MedianProx(15, 0);
RunningAverage MicroRA(15);

// Déclaration des fonctions

void CalibProxiMic();
int CountingFile (File DirName);
float JaugePeak (bool DetectEvent, const int TimeWindow);
int URN(int nbtotfile);
void TrigFile(const char *filename);
void LoopFile(const char *filename);
void initSampleSelecteur(int nbtotfile);

// Variables données

int Proxi = 0;

float MicroRms = 0.0;
float Micro_Moyenne = 0.0;
int ProxiMedian = 0;

// Variables calib Proxi + Micro

const int TEMPS_CALIB = 5000 ;            // REGLAGE  : durée calibration.

int ProxiMin = 1023;
int ProxiMax = 0;
float MicroMin = 1023.0;

// Variable Compteur nombre de fichiers par dossier

File root;
int NbFiles[4] = {0};

// Variables JAUGES

int NbPeak = 0;
float ThreshPeak = 20.0 ;              //(10 par défaut par rapport à la dérivée)
bool DetectPeak = false;
const int TP_STOCKAGE_PEAK = 1500;       //REGLAGE (Temps de stockage de la jauge Micro)
float jaugePeak = 0.0;

// Valeurs de seuil de changement d'humeur + changement de rapidité de déclenchement

int SeuilProxi;
int Condition = 0;
int SeuilJaugeMicro = 1;              //REGLAGE (Seuil de changement d'humeur Micro)
uint32_t resetTimeThreshpeak = 0;
int conditionReset = 0;

// Variables PROXI Range

int ProxiRangeMin = 0;
int ProxiRangeMax = 0;
int ProxiValueMin = 0;
int ProxiValueMax = 0;

int RangeMin;
int RangeMax;
int BorneMinMin ;
int BorneMinMax ;
int BorneMaxMax ;

int randomMin;
int randomMax;

// Variables fonction TRIGFILE

uint32_t resetMillis = 0;
int threshTrig = 0;

int corpusSampleNumber = 10;   // Nombre de son dans chaque dossier

int sample_rand = 1;           // tirage au sort du numéro du sample.
String SoundFile, SoundType;
String HumeurFolder[4]  = {"serein", "timide", "hilare", "hostile"};
int selecthumeur = 0;
int previoushumeur = 1000;
int statechangeselecthumeur = 0;

// Variables Freeze Timide

const int FREEZ_TIMIDE_TIME = 60000;           // REGLAGE (durée du mode timide)

