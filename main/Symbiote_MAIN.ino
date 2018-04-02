#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MedianFilter.h>   //https://github.com/daPhoosa/MedianFilter
#include <RunningAverage.h> //https://playground.arduino.cc/Main/RunningAverage
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=92,311
AudioMixer4              mixer1;         //xy=385,325
AudioMixer4              mixer2;         //xy=385,393
AudioInputI2S            i2s2;           //xy=537,164
AudioAnalyzeRMS          rms1;           //xy=712,158
AudioOutputI2S           i2s1;           //xy=845,386
AudioConnection          patchCord1(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord2(playSdWav1, 1, mixer2, 0);
AudioConnection          patchCord3(mixer1, 0, i2s1, 0);
AudioConnection          patchCord4(mixer2, 0, i2s1, 1);
AudioConnection          patchCord5(i2s2, 0, rms1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=88,95
// GUItool: end automatically generated code

//Use these with the audio adaptor board

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#define LED 13
#define ProxiPin A3

// DECLARATION & INIT VARIABLES

// Settings fonctions

MedianFilter MedianProx(15, 0);
RunningAverage MicroRA(15);

// Déclaration des fonctions

void CalibProxiMic();
int CountingFile (File DirName);
float JaugePeak (bool DetectEvent, float TimeWindow);
int URN(int nbtotfile);
void TrigFile(const char *filename);
void LoopFile(const char *filename);
void initSampleSelecteur(int nbtotfile);

// Variables données

int Proxi;
float MicroRms;

float deriveeNoise;
float deriveeMic;

int ProxiMedian = 0;
float Micro_Moyenne = 0;

// Variables calib Proxi + Micro

int TempsCalib = 5000 ;            // REGLAGE  : durée calibration.

int ProxiMin = 1023;
float ProxiMax = 0;
float MicroMin = 1023;
float MicroMax = 0;

// Variable Compteur nombre de fichiers par dossier

File root;
int NbFiles[4] = {0};

// Variables JAUGES

int NbPeak = 0;
int ThreshPeak = 20 ;              //(10 par défaut par rapport à la dérivée)
bool DetectPeak = 0;
int TpsStockagePeak = 1500;       //REGLAGE (Temps de stockage de la jauge Micro)
float jaugePeak = 0;

// Valeurs de seuil de changement d'humeur + changement de rapidité de déclenchement

int SeuilProxi;
int Condition = 0;
int SeuilJaugeMicro = 1;              //REGLAGE (Seuil de changement d'humeur Micro)

// Variables PROXI Range

float ProxiRangeMin = 0;
float ProxiRangeMax = 0;
float ProxiValueMin = 0;
float ProxiValueMax = 0;

int RangeMin;
int RangeMax;
int BorneMinMin ;
int BorneMinMax ;
int BorneMaxMax ;

int randomMin;
int randomMax;

// Variables fonction TRIGFILE
int resetMillis = 0;
int threshTrig = 0;
int corpusSampleNumber = 10;   // Nombre de son dans chaque dossier

int sample_rand = 1;           // tirage au sort du numéro du sample.
String SoundFile, SoundType;
String HumeurFolder[4]  = {"serein", "timide", "hilare", "hostile"};
int selecthumeur = 0;
int previoushumeur = 1000;
int statechangeselecthumeur = 0;

// Variables Timide
int FreezeTimideTime = 60000;           // REGLAGE (durée du mode timide)

//-------------
//-------------

//                                 >>>>> SETUP <<<<<

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A9));  //passer la fonction random en réelle fonction aléatoire.

  //AUDIO

  AudioMemory(70);             // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
  // setup audio board
  SoundFile = String();
  SoundType = String(".wav");
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);                         //REGLAGE
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(50);

  //Setup SD Card
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(1000);
    }
  }
  //Compte le nombre de fichiers dans chacun des dossiers d'humeur.

  for (int i = 0; i <= 3; i++) {
    String FileRemove = HumeurFolder[i] + "/" + "DS_STO~1";
    SD.remove(FileRemove.c_str());
    String FolderName = HumeurFolder[i] + "/";
    root = SD.open(FolderName.c_str());
    NbFiles[i] = CountingFile(root);
  }
  Serial.print("Le nombre de fichiers dans le dossier serein est de : ");
  Serial.println(NbFiles[0]);
  Serial.print("Le nombre de fichiers dans le dossier timide est de : ");
  Serial.println(NbFiles[1]);
  Serial.print("Le nombre de fichiers dans le dossier hilare est de : ");
  Serial.println(NbFiles[2]);
  Serial.print("Le nombre de fichiers dans le dossier hostile est de : ");
  Serial.println(NbFiles[3]);
  Serial.println();

  // Calibration Proximètre et Microphone
  CalibProxiMic();
}

// >>>>> MAIN LOOP <<<<<

void loop() {
  // LECTURE DES DONNEES

  Proxi = analogRead(ProxiPin);              // DOIT ETRE COMPRIS DE 0 à 1
  MedianProx.in(Proxi);
  ProxiMedian = MedianProx.out();

  // SELECTIONNE LA BONNE  HUMEUR ET LES BONNES PLAGES DE RANDOM RETRIG

  // Serein
  if (ProxiMedian > SeuilProxi) {
    if (Condition != 1) {
      corpusSampleNumber = NbFiles[0] ;          //Met à jour le nombre total de fichiers disponible dans le dossier serein
      RangeMin = SeuilProxi;
      RangeMax = ProxiMax;
      BorneMinMin = 5000 ;                        // REGLAGE
      BorneMinMax = 10000;                        // REGLAGE
      BorneMaxMax = 20000;                        // REGLAGE
      selecthumeur = 0 ;
      Condition = 1;
    }
  }

  // Agressif
  if (ProxiMedian < SeuilProxi) {
    if (Condition != 2) {
      corpusSampleNumber = NbFiles[3];          //Met à jour le nombre total de fichiers disponible dans le dossier timide
      RangeMin = ProxiMin;
      RangeMax = SeuilProxi;
      BorneMinMin = 100 ;                        // REGLAGE
      BorneMinMax = 200;                        // REGLAGE
      BorneMaxMax = 1000;                        // REGLAGE                                // REGLAGE
      selecthumeur = 3;
      Condition = 2;
    }
  }

  ProxiRangeMin = map(ProxiMedian, RangeMin, RangeMax, BorneMinMin, BorneMinMax) ;
  ProxiRangeMax = map(ProxiMedian, RangeMin, RangeMax, BorneMinMax, BorneMaxMax) ;
  randomMin = constrain(ProxiRangeMin, BorneMinMin, BorneMinMax) ;
  randomMax = constrain(ProxiRangeMax, BorneMinMax, BorneMaxMax) ;
  threshTrig = random(randomMin, randomMax);

  // PLAYER AUDIO

  // Random Retrig

  if (millis() - resetMillis > threshTrig) {
    resetMillis = millis();
    sample_rand = URN(corpusSampleNumber) + 1;              // TIRAGE ALEATOIRE SANS REDECLENCHER LE MEME SON.
    SoundFile = HumeurFolder[selecthumeur] + "/" + sample_rand + SoundType;  //inttochar
    TrigFile(SoundFile.c_str());                                //JOUE LE fichier après un temps tiré aléatoirement définie dans une fourchette qui varie selon la somme des deux capteurs. .c_str() passe un string en char (en gros...)
  }
  delay(10);
}

//-------------

// FONCTIONS

//-------------

// CALIBRATION PROXI & MICRO

void CalibProxiMic () {

  int InitTimerCalib = millis();
  Serial.println(" >>>>>>>>>>> Début de Calibration <<<<<<<<<< ");
  while (millis() - InitTimerCalib < TempsCalib) {
 
    Proxi = analogRead(ProxiPin);
  
    // record the minimum Proxi Value
    if (Proxi < ProxiMin) {
      ProxiMin = Proxi;
    }
    // record the maximum Proxi value
    if (Proxi > ProxiMax) {
      ProxiMax = Proxi;
    }
    delay(10);
  }
  if (ProxiMin > ProxiMax / 2) ProxiMin = ProxiMax / 2 ;
  SeuilProxi = ProxiMax / 2;

  Serial.print("ProxiMin & ProxiMax : ");
  Serial.print(ProxiMin);
  Serial.print(" & ");
  Serial.println(ProxiMax);

  Serial.print("  !!  Seuil Proxi : ");
  Serial.print(SeuilProxi);
  Serial.print("  !!  ThresPeak : ");
  Serial.println(ThreshPeak);
  Serial.println(" >>>>>>>>>>> Fin de Calibration <<<<<<<<<< ");
}

//-------------

// COMPTAGE DU NOMBRE DE FICHIERS PAR DOSSIER

int CountingFile (File DirName) {
  int FileNumber = 0;
  while (true) {
    File entry =  DirName.openNextFile();
    Serial.println(entry.name());

    if (! entry) {
      // no more files to scan
      break;
    }
    FileNumber++;
    //    if (!entry.isHidden()) {
    //      FileNumber++;
    //    }
    entry.close();
  }
  return FileNumber;
}

// FONCTION URN Thomas (tire aléatoirement un son qui n'a pas encore été joué)

int index_tab = 0; // nombre de samples deja jouées;
int sampleDejaJoues[100]; //Défini la taille du tableau et initialise toutes les valeurs à 0
int unplayedSample = 0;
int lastPlayedSample = -1;

int URN(int nbtotfile) {
  boolean etat = true;

  while (etat) {
    etat = false;
    unplayedSample = int(random(nbtotfile));
    for (int i = 0; i < index_tab; i++) {
      if (sampleDejaJoues[i] == unplayedSample) {
        etat = true;
        break;
      }
    }
    if (index_tab == 0 && unplayedSample == lastPlayedSample) {
      etat = true;
    }
  }

  if (index_tab < nbtotfile) {
    sampleDejaJoues[index_tab] = unplayedSample;
    index_tab ++;
  }

  if (index_tab == nbtotfile) {
    index_tab = 0;
    lastPlayedSample = unplayedSample;
    initSampleSelecteur(nbtotfile);
  }
  return unplayedSample;
}

//Init Sample

void initSampleSelecteur(int nbtotfile) {
  for (int i = 0; i < nbtotfile; i++) {
    sampleDejaJoues[i] = -1;
  }
}

//-------------
//-------------

// TRIG UN FICHIER SON EN COUPANT LE PRECEDENT

void TrigFile(const char *filename)
{
  Serial.println("");
  Serial.print(" >>>>>>>>>>>>>>> Playing file : ");
  Serial.print(filename);
  Serial.println(" <<<<<<<<<<<<<<< ");
  Serial.println("");
  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playSdWav1.play(filename);
  // A brief delay for the library read WAV info
  delay(10);
}

