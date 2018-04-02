#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MedianFilter.h>   //https://github.com/daPhoosa/MedianFilter
#include <RunningAverage.h> //https://playground.arduino.cc/Main/RunningAverage
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=143,276
AudioPlaySdWav           playSdWav2;     //xy=143,307
AudioSynthNoiseWhite     noise1;         //xy=159,201
AudioMixer4              mixer2;         //xy=293,291
AudioMixer4              mixer1;         //xy=295,220
AudioMixer4              mixer3;         //xy=435,259
AudioInputI2S            i2s2;           //xy=482,107
AudioEffectDelay         delay1;         //xy=592,259
AudioAnalyzeRMS          rms1;           //xy=631,101
AudioAnalyzePeak         peak1;          //xy=631,132
AudioAnalyzeRMS          rms2;           //xy=749,205
AudioAnalyzePeak         peak2;          //xy=750,237
AudioOutputI2S           i2s1;           //xy=751,269
AudioConnection          patchCord1(playSdWav1, 0, mixer2, 0);
AudioConnection          patchCord2(playSdWav1, 1, mixer2, 1);
AudioConnection          patchCord3(playSdWav2, 0, mixer2, 2);
AudioConnection          patchCord4(playSdWav2, 1, mixer2, 3);
AudioConnection          patchCord5(noise1, 0, mixer1, 0);
AudioConnection          patchCord6(mixer2, 0, mixer3, 1);
AudioConnection          patchCord7(mixer1, 0, mixer3, 0);
AudioConnection          patchCord8(mixer3, delay1);
AudioConnection          patchCord9(i2s2, 0, rms1, 0);
AudioConnection          patchCord10(i2s2, 0, peak1, 0);
AudioConnection          patchCord11(delay1, 0, rms2, 0);
AudioConnection          patchCord12(delay1, 0, peak2, 0);
AudioConnection          patchCord13(delay1, 0, i2s1, 0);
AudioConnection          patchCord14(delay1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=376,425
// GUItool: end automatically generated code




//Use these with the audio adaptor board

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#define BUTTON 0
#define LED 13
#define ProxiPin A0

// DECLARATION & INIT VARIABLES

// Settings fonctions

MedianFilter MedianProx(15, 0);
RunningAverage MicroRA(10);
Bounce boutonCalib = Bounce(BUTTON, 15 );
elapsedMillis msecs;

// Variables données

int Proxi;
int MicroRms;
uint8_t MicroValue;
uint8_t SignalInterneRms;
uint8_t deriveeNoise;
uint8_t deriveeMic;
uint8_t MemNoise;
uint8_t MemMic;
uint8_t MicroSig;
uint8_t InternalSig;
int   conditionRms = 0;
int resetmillisrms = 0;
int freqRms = 50;              //REGLAGE
int LastConditionRms = 0;

int Proxi_Median = 0;
int ProxiRange = 0;
int ProxiValue = 0;
int Micro_Moyenne = 0;

// Variables calib

int InitTimerCalib = 0 ;
int TempsCalib = 5000 ; // REGLAGE  : durée calibration quand le bouton est déclenché.
int ProxiMin = 1023;        // minimum sensor value
int ProxiMax = 0;           // maximum sensor value
int MicroMin = 1023;        // minimum sensor value
int MicroMax = 0;           // maximum sensor value


// Variables Comptage Passages

int NbPassage = 0;
int NbPeak = 0;

bool DetectPassage = 0;
bool DetectPeak = 0;

int ThreshPassage = 100;          //REGLAGE
int ThreshPeak = 20 ;             //REGLAGE

int TpsStockagePassage = 10000 ;  //REGLAGE
int TpsStockagePeak = 3000;       //REGLAGE

float JaugePassage = 0;
float JaugePeak = 0;

// Valeurs de seuil de changement d'humeur + changement de rapidité de déclenchement

int SeuilMicro = 50;          //REGLAGE (entre 0 et 100)
int SeuilProxi = 50;          //REGLAGE (entre 0 et 100)

// Variables fonction RANDOMTRIG

int treshTrig = 0;            //REGLAGE
int randomMin = 50;         //REGLAGE
int randomMax = 34000;       //REGLAGE

// Valeurs de timing.

int tresh = 0;
int TempsCamouflage = 10000;

// Variables fonction URN

int corpusSampleNumber = 5;   //REGLAGE

int index_tab = 0; // nombre de samples deja jouées;
int sampleDejaJoues[100]; //Défini la taille du tableau et initialise toutes les valeurs à 0
int i = 0;
int unplayedSample = 0;
boolean etat = true;
int lastPlayedSample = -1;
int

// Variables play file

String SoundFile, SoundType;
String HumeurFolder[4]  = {"Serein", "Timide", "Euphorique", "Agressif"};
int selecthumeur = 0;
int previoushumeur = 1000;
int statechangeselecthumeur = 0;

// -----

int sample_rand = 0;  // tirage au sort du numéro du sample.
boolean seuil = false;
int hysteresis = 0;


//-------------

// MAIN CODE

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A9));  //passer la fonction random en réelle fonction aléatoire.

  //AUDIO

  AudioMemory(15);             // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
  SoundFile = String();
  SoundType = String(".WAV");
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(50);

  //Setup Bouton Calibration en input mode
  pinMode(BUTTON, INPUT);
  
  //Calibration Micro / Signal interne.
  MicroRms = rms1.read();
  SignalInterneRms = rms2.read();

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
}

void loop() {

  // LECTURE DES DONNEES

  //Proxi
  Proxi = analogRead(ProxiPin);              // DOIT ETRE COMPRIS DE 0 à 1
  MedianProx.in(Proxi);
  ProxiMedian = MedianProx.out();

  //Micro
  MicroRms = rms1.read() * 1000;              // DOIT ETRE COMPRIS DE 0 à 1
  boutonCalib.update();
  int boutonCal = boutonCalib.read();

  // CALIBRATION MICRO / SIGNAL INTERNE

  // A faire avant dans un autre programme et rentrer les valeur de compensation à la main ???

  // TRAITEMENT DONNEES

  //Signal interne

  //  conditionRms = millis() - resetmillisrms  < freqRms;
  //
  //  if (conditionRms) {
  //    if (conditionRms != LastConditionRms) {
  //      InternalSig = rms2.read() * 10;
  //      LastConditionRms = 1;
  //    }
  //  }
  //  else {
  //    LastConditionRms = 0;
  //    resetmillisrms = millis();
  //  }

  // Micro

  //Tester si la dérivée est suffisamment réactive avec le Micro_Moyenne à la place du MicroRms.
  //  MicroRA.addValue(MicroRms);
  //  Micro_Moyenne = MicroRA.getAverage();

  //  if (InternalSig > 50) {  // Check si il y a un signal qui sort du mixer Master  >>>> A REGLER <<<<
  //    MicroSig = MicroCompens - InternalSig; // Soustraction signal micro avec compensation de gain - signal interne avec compensation de delay à réaliser ici.
  //  }
  //  else {
  //    MicroSig = MicroRms;
  //  }
  //
  //  deriveeMic = MicroSig - MemMic;
  //  MemMic = MicroSig;

  // CALIBRATION MAPPING VALEURS

  // Proxi

  if (boutonCal != LastButtonValue) { //Detection d'un Front
    if (boutonCal == 1) { //front montant
      LastButtonValue = 1; // "change"
      InitTimerCalib = millis();
      while (millis() - InitTimerCalib < TempsCalib) {
        // record the minimum Proxi Value
        if (ProxiMedian < ProxiMin) {
          ProxiMin = ProxiMedian;
        }
        // record the maximum Proxi value
        if (ProxiMedian > ProxiMax) {
          ProxiMax = ProxiMedian;
        }

        //        // record the minimum Micro Value
        //        if (deriveeMic < MicMin) {
        //          MicroMin = deriveeMic;
        //        }
        //        // record the maximum sensor value
        //        if (deriveeMic > MicMax) {
        //          MicroMax = deriveeMic;
        //        }
      }
    }
  }
  else {
    LastButtonValue = 0;//remise à 0 pour ne pas repasser dans le if
  }

  //MAPPING VAL MIN-MAX de 0 à 100.

  ProxiRange = map(ProxiMedian, ProxiMin, ProxiMax, 0, 100);
  MicroRange = map(deriveeMic, MicroMin, MicroMax, 0, 100);

  // CLIP
  ProxiValue = constrain(ProxiRange, 0, 100);
  MicroValue = constrain(MicroRange, 0, 100);
  -
  // --------------

  // JAUGES / COMPTAGE

  // Jauge du nombre de passage toutes les x secondes

  if (ProxiValue < ThreshPassage) {
    DetectPassage = 1;
  }
  else {
    DetectPassage = 0;
  }

  jaugePassage = Jauge(DetectPassage, TpsStockagePassage);

  Serial.print("La valeur de la jauge est de : ");
  Serial.print(jaugePassage);

  // Jauge de Peak detection micro toutes les x secondes
  
//  if (MicroValue > ThreshPeak) {
//    DetectPeak = 1;
//  }
//  else {
//    DetectPeak = 0;
//  }
//
//  JaugePeak = Jauge(DetectPeak, TpsStockagePeak);
//
//  Serial.print("La valeur de la jauge est de : ");
//  Serial.print(JaugePeak);


  // SELECTIONNE LA BONNE  HUMEUR ET LES BONNES PLAGES DE RANDOM RETRIG


  // Serein
  if (JaugePeak < SeuilMicro && JaugePassage < SeuilProxi) {
    if (Condition != 1) {                                 // Détection de changement d'état
      Condition = 1;
    }
  }
  // Timide
  if (JaugePeak < SeuilMicro && JaugePassage > SeuilProxi) {
    if (Condition != 2) {
      Condition = 2;
    }
  }
  // Hilare
  if (JaugePeak > SeuilMicro && JaugePassage < SeuilProxi) {
    if (Condition != 3) {
      Condition = 3;
    }
  }
  // Agressif
  if (JaugePeak > SeuilMicro && JaugePassage > SeuilProxi) {
    if (Condition != 4) {
      Condition = 4;
    }
  }

  switch (Condition) {
    case 1 :
      randomMin = 2000 ;                        // REGLAGE Serein
      randomMax = 14000 ;                       // REGLAGE
      selecthumeur = 1;
      treshTrig = random(randomMin, randomMax);
      break
    case 2 :
      randomMin = 14000 ;                       // REGLAGE Timide
      randomMax = 34000 ;                       // REGLAGE
      selecthumeur = 2;
      treshTrig = random(randomMin, randomMax);
      break
    case 3 :
      randomMin = 500 ;                        // REGLAGE Hilare
      randomMax = 2000 ;                       // REGLAGE
      selecthumeur = 3;
      treshTrig = random(randomMin, randomMax);
      break
    case 4 :
      randomMin = 100 ;                       // REGLAGE Agressif
      randomMax = 500 ;                       // REGLAGE
      selecthumeur = 4;
      treshTrig = random(randomMin, randomMax);
      break
  }

  // BOUCLE TREMBLEMENT
  //
  //  while (ProxiValue > Seuil4Proxi) {
  //    playSdWav1.stop();
  //    delay(TempsCamouflage);
  //  }

  // SELECTIONNE UN FICHIER ALEATOIREMENT DANS LE DOSSIER DE LA BONNE HUMEUR, SANS LE REPETER .

  sample_rand = URN(corpusSampleNumber);                // URN A IMPLEMENTER ICI
  SoundFile = '/' + HumeurFolder[selecthumeur] + '/' + sample_rand + SoundType;  //inttochar
  Serial.println(SoundFile);
  //  sensor = (ProxiValue + MicroValue) / 2 ;

  // RANDOM RETRIG

  if (millis() - resetMillis > treshTrig) {
    resetMillis = millis();
    playFile(SoundFile.c_str()); //JOUE LE fichier après un temps tiré aléatoirement définie dans une fourchette qui varie selon la somme des deux capteurs. .c_str() passe un string en char (en gros...)
    treshTrig = random(randomMin, randomMax);
  }
}

delay(5);
}

//End LOOP



//-------------
//-------------

//FONCTIONS

//-------------
//-------------

// ------------

// FONCTIONS JAUGE : COMPTE NB D'EVENEMENTS DANS UN INTERVALLE DE TEMPS DONNE ET L'INTERPOLE LINEAIREMENT PAR RAPPORT A LA VALEUR PRECEDENTE ET PENDANT L'INTERVALLE DE TEMPS DETERMINE.

bool DetectEvent;
bool LastDetectEvent = 0;
bool StockageEvent = 0;
bool LastStockageEvent = 0;
float nbEvent = 0;
float arrivee = 0;
float depart;
float Line = 0;
float Temps = 0;
float coef = 0;
float jaugePassage = 0;

float Jauge (bool DetectEvent, float TimeWindow) {
  if (DetectEvent != LastDetectEvent) { //Detection d'un front montant
    if (DetectEvent) {
      nbEvent++;  // Incrémentation du nombre de passage.
      LastDetectEvent = 1;
    }
    else {
      LastDetectEvent = 0;
    }
  }

  StockageEvent = millis() - Temps > TimeWindow; // Condition de temps pour retourner la future valeur de la jauge (toutes les X secondes)

  if (StockageEvent != LastStockageEvent) {
    if (StockageEvent) {
      depart = arrivee;
      arrivee = nbEvent; // On retourne le nombre d'évènements.
      Temps = (float)millis();  //On remet le compteur de temps à 0 pour la prochaine boucle et avoir un snapshot régulier.
      nbEvent = 0; // On remet le nombre d'évènements à 0.
      LastStockageEvent = 1;
    }
    else {
      LastStockageEvent = 0;
    }
  }
  coef = (arrivee - depart) / TimeWindow ;
  Line = (coef * ((float)millis() - Temps) + depart); // Interpolation linéaire (= Line pure data)
  return Line;
}



//-------------

//JOUER UN FICHIER SON (dans le premier player depuis la carte SD)

// FONCTION URN Thomas (tire aléatoirement un son qui n'a pas encore été joué)

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

void initSampleSelecteur(int nbtotfile) {
  for (int i = 0; i < nbtotfile; i++) {
    sampleDejaJoues[i] = -1;
  }
}

//Init Sample

void initSampleSelecteur(int nbtotfile) {
  for (int i = 0; i < nbtotfile; i++) {
    sampleDejaJoues[i] = -1;
  }
}

//-------------

// FONCTION RANDOM TRIG (déclenche un sample après un temps aléatoire tiré dans un plage de valeur variable.

void RandomTrig (int sensor, randomMin, randomMax) {

  if (millis() - resetMillis > treshTrig) {
    resetMillis = millis();
    playFile(SoundFile.c_str()); //JOUE LE SON .c_str() passe un string en char (en gros...)
    treshTrig = random(randomMin, randomMax);
  }
}

//-------------

//JOUER UN FICHIER SON (dans le premier player depuis la carte SD)

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);
  if (playSdWav1.isPlaying() == false) {
    Serial.println("Start playing");
    playSdWav1.play(filename);
    delay(5); // wait for library to parse WAV info
  }
}


