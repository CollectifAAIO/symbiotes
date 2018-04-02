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

MedianFilter MedianProx(10, 0);
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

// REGLAGES

const int TEMPS_CALIB = 5000 ;          

const int TP_STOCKAGE_PASSAGE = 10000;     
const int SEUILJAUGEPROXI = 1;           

const int TP_STOCKAGE_PEAK = 3000;          
const int SEUILJAUGEMICRO = 3;                    

const int TIMIDEMIN = 20000;
const int TIMIDEMAX = 40000;
const int SEREINMIN = 12000;
const int SEREINMAX = 20000;
const int HILAREMIN = 800;
const int HILAREMAX = 2500;
const int HOSTILEMIN = 100;
const int HOSTILEMAX = 800;

const int COEF_THRESPEAK = 3;
const int POURCENTAGE_PROXI = 50;

// -------

// AUTRES 

int Proxi = 0;
uint32_t ResetTimeRandProxi = 0;

// ----
float MicroRms = 0.0;
float Micro_Moyenne = 0.0;
int ProxiMedian = 0;

// Variables calib Proxi + Micro

int ProxiMin = 1023;                      // REGLAGE
int ProxiMax = 0;                     // REGLAGE
float MicroMin = 1023.0;

// Variable Compteur nombre de fichiers par dossier

File root;
int NbFiles[4] = {0};

// Variables JAUGES


int ThreshPassage = 150;
bool DetectPassage = false;
float jaugePassage = 0.0;

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

