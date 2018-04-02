// Symbiote MAIN : Sans compensation de délai. Mesure des peaks sur le signal moyenné du microphone.

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MedianFilter.h>   //https://github.com/daPhoosa/MedianFilter
#include <RunningAverage.h> //https://playground.arduino.cc/Main/RunningAverage
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav2;     //xy=96,371
AudioPlaySdWav           playSdWav1;     //xy=98,256
AudioSynthWaveformSine   AMSine;         //xy=161,436
AudioEffectMultiply      multiply2;      //xy=302,423
AudioEffectMultiply      multiply1;      //xy=306,317
AudioMixer4              mixer1;         //xy=463,270
AudioMixer4              mixer2;         //xy=463,384
AudioInputI2S            i2s2;           //xy=531,155
AudioMixer4              mixer3;         //xy=657.77783203125,284.4444580078125
AudioMixer4              mixer4;         //xy=661.1111450195312,387.77777099609375
AudioAnalyzeRMS          rms1;           //xy=706,149
AudioOutputI2S           i2s1;           //xy=790.77783203125,316.7778015136719
AudioConnection          patchCord1(playSdWav2, 0, multiply1, 0);
AudioConnection          patchCord2(playSdWav2, 1, multiply2, 0);
AudioConnection          patchCord3(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord4(playSdWav1, 1, mixer2, 0);
AudioConnection          patchCord5(AMSine, 0, multiply2, 1);
AudioConnection          patchCord6(AMSine, 0, multiply1, 1);
AudioConnection          patchCord7(multiply2, 0, mixer2, 1);
AudioConnection          patchCord8(multiply1, 0, mixer1, 1);
AudioConnection          patchCord9(mixer1, 0, mixer3, 0);
AudioConnection          patchCord10(mixer2, 0, mixer4, 0);
AudioConnection          patchCord11(i2s2, 0, rms1, 0);
AudioConnection          patchCord12(mixer3, 0, i2s1, 0);
AudioConnection          patchCord13(mixer4, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=82,86
// GUItool: end automatically generated code


//Use these with the audio adaptor board

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#define BUTTON 0
#define LED 13
#define ProxiPin A3

// DECLARATION & INIT VARIABLES

// Settings fonctions

MedianFilter MedianProx(15, 0);
RunningAverage MicroRA(15);
Bounce boutonCalib = Bounce(BUTTON, 15 );

// Déclaration des fonctions

void CalibProxiMic();
int CountingFile (File DirName);
float JaugePassage (bool DetectEvent, float TimeWindow);
float JaugePeak (bool DetectEvent, float TimeWindow);
int URN(int nbtotfile);
void TrigFile(const char *filename);
void LoopFile(const char *filename);
void initSampleSelecteur(int nbtotfile);
float Line(float arrivee, float TimeInterpol);


// Variables données

int Proxi;
float MicroRms;

int deriveeNoise;
int deriveeMic;
int MemNoise = 0;
int MemMic = 0;

int ProxiMedian = 0;
int Micro_Moyenne = 0;

// Variables bouton calib

int BoutonCal;
int PressInstant = 0;
int LastButtonValue = 0;

// Variable Compteur nombre de fichiers par dossier

File root;
int NbFiles[4] = {0};

// Variables calib Proxi + Micro

int ProxiMin = 1023;
int ProxiMax = 0;
int MicroMin = 1023;
int MicroMax = 0;

int ThreshPassage = 180 ; // AUTOMATIQUE (180, par défaut)
int ThreshPeak = 60 ;     // AUTOMATIQUE : Seuil déclenchement peak
int TransitionTrigLoop = 0;

int TempsCalib = 5000 ;   // REGLAGE  : durée calibration quand le bouton est déclenché.

// Variables JAUGES

int NbPassage = 0;
int NbPeak = 0;

bool DetectPassage = 0;
bool DetectPeak = 0;

int TpsStockagePassage = 10000 ;  //REGLAGE (Temps de stockage de la jauge Proxi)
int TpsStockagePeak = 2000;       //REGLAGE (Temps de stockage de la jauge Micro)

float jaugePassage = 0;
float jaugePeak = 0;

// Valeurs de seuil de changement d'humeur + changement de rapidité de déclenchement

int Condition = 1;
int SeuilJaugeMicro = 1;              //REGLAGE (Seuil de changement d'humeur Micro)
int SeuilJaugePassage = 2.5;          //REGLAGE (Seuil de changement d'humeur Proxi)

// Variables fonction TRIGFILE

int resetMillis = 0;
int threshTrig = 0;
int randomMin = 50;
int randomMax = 34000;
int corpusSampleNumber = 10;   // AUTOMATIQUE : 10 par défaut.

int sample_rand = 1;           // tirage au sort du numéro du sample.
String SoundFile, SoundType;
String HumeurFolder[4]  = {"serein", "timide", "hilare", "hostile"};
int selecthumeur = 0;
int previoushumeur = 1000;
int statechangeselecthumeur = 0;

// Variables Loop Tremblement
int StagnationPersonne = 0;
int ActiveTremblingTime = 2000; // REGLAGE (Temps après lequel on déclenche le tremblement dès qu'une personne est resté à une certaine distance du symbiote)
float ProxiRange = 0;
float ProxiValue = 0;
float ProxiLine = 0;

int TempsLineTremble = 500 ;   // REGLAGE (Temps du line)
int AMfreqMax = 15;           // REGLAGE (fréquence max de la modulation d'amplitude)

//-------------

// MAIN CODE

// >>>>> SETUP <<<<<

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A9));  //passer la fonction random en réelle fonction aléatoire.

  //AUDIO

  AudioMemory(64);             // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
  // setup audio board
  SoundFile = String();
  SoundType = String(".wav");
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.8);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(50);

  //Setup Bouton Calibration en input mode
  pinMode(BUTTON, INPUT);

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

  boutonCalib.update();
  BoutonCal = boutonCalib.read();

  Proxi = analogRead(ProxiPin);              // DOIT ETRE COMPRIS DE 0 à 1
  MedianProx.in(Proxi);
  ProxiMedian = MedianProx.out();

  //Micro Moyenne

  MicroRms = rms1.read() * 1000.0f;            // DOIT ETRE COMPRIS DE 0 à 1
  MicroRA.addValue(MicroRms);
  Micro_Moyenne = MicroRA.getAverage();
  //  deriveeMic = MicroRms - MemMic;
  //  MemMic = MicroRms;&

  // RECALIBRATION AVEC BOUTON

  if (BoutonCal != LastButtonValue) { // Detection d'un front montant
    if (BoutonCal == 1) {
      ProxiMin = 1023;
      ProxiMax = 0;
      MicroMin = 1023;
      MicroMax = 0;
      ThreshPassage = 0;
      ThreshPeak = 0;
      CalibProxiMic();
      LastButtonValue = 1;
    }
  }
  else {
    LastButtonValue = 0;
  }

  // JAUGES

  // Jauge du nombre de passage toutes les x secondes

  if (ProxiMedian < ThreshPassage) {
    DetectPassage = 1;
  }
  else {
    DetectPassage = 0;
  }

  jaugePassage = JaugePassage(DetectPassage, TpsStockagePassage);

  // Jauge du nombre de peak toutes les x secondes

  if (Micro_Moyenne > ThreshPeak) {
    DetectPeak = 1;
  }
  else {
    DetectPeak = 0;
  }

  jaugePeak = JaugePeak(DetectPeak, TpsStockagePeak);

  // SELECTIONNE LA BONNE  HUMEUR ET LES BONNES PLAGES DE RANDOM RETRIG

  // Serein
  if (jaugePeak < SeuilJaugeMicro && jaugePassage < SeuilJaugePassage) {
    if (Condition != 1) {
      corpusSampleNumber = NbFiles[0];          //Met à jour le nombre total de fichiers disponible dans le dossier serein
      randomMin = 2000 ;                        // REGLAGE
      randomMax = 14000 ;                       // REGLAGE
      selecthumeur = 0;
      threshTrig = random(randomMin, randomMax);
      Condition = 1;
    }
  }

  // Timide
  if (jaugePeak < SeuilJaugeMicro && jaugePassage > SeuilJaugePassage) {
    if (Condition != 2) {
      corpusSampleNumber = NbFiles[1];          //Met à jour le nombre total de fichiers disponible dans le dossier timide
      randomMin = 14000 ;                       // REGLAGE
      randomMax = 34000 ;                       // REGLAGE
      selecthumeur = 1;
      threshTrig = random(randomMin, randomMax);
      Condition = 2;
    }
  }

  // Hilare
  if (jaugePeak > SeuilJaugeMicro && jaugePassage < SeuilJaugePassage) {
    if (Condition != 3) {
      corpusSampleNumber = NbFiles[2];         //Met à jour le nombre total de fichiers disponible dans le dossier hilare
      randomMin = 500 ;                        // REGLAGE Hilare
      randomMax = 2000 ;                       // REGLAGE
      selecthumeur = 2;
      threshTrig = random(randomMin, randomMax);
      Condition = 3;
    }
  }

  // Agressif
  if (jaugePeak > SeuilJaugeMicro && jaugePassage > SeuilJaugePassage) {
    if (Condition != 4) {
      corpusSampleNumber = NbFiles[3];        //Met à jour le nombre total de fichiers disponible dans le dossier hostile
      randomMin = 100 ;                       // REGLAGE Agressif
      randomMax = 500 ;                       // REGLAGE
      selecthumeur = 3;
      threshTrig = random(randomMin, randomMax);
      Condition = 4;
    }
  }

  // AUDIO PLAYERS

  // RANDOM RETRIG ("Screaming" sounds)

  if (millis() - resetMillis > threshTrig) {
    resetMillis = millis();
    sample_rand = URN(corpusSampleNumber) + 1;              // TIRAGE ALEATOIRE SANS REDECLENCHER LE MEME SON.
    SoundFile = HumeurFolder[selecthumeur] + "/" + sample_rand + SoundType;  //inttochar
    TrigFile(SoundFile.c_str());                                //JOUE LE fichier après un temps tiré aléatoirement définie dans une fourchette qui varie selon la somme des deux capteurs. .c_str() passe un string en char (en gros...)
    threshTrig = random(randomMin, randomMax);
  }

  // TREMBLING LOOP

//  if (ProxiMedian <= TransitionTrigLoop) {
//    StagnationPersonne = millis();
//    if (millis() - StagnationPersonne >= ActiveTremblingTime) {
//
//    }
//    else {
//
//    }
//  }

  ProxiRange = map(ProxiMedian, ProxiMin, TransitionTrigLoop, 100, 0);
  ProxiValue = constrain(ProxiRange, 0, 100) / 100;
  ProxiLine = Line (ProxiValue, TempsLineTremble);

  // stop processing while configuring things
  AudioNoInterrupts();
  mixer1.gain(1, ProxiLine);
  mixer2.gain(1, ProxiLine);
  AMSine.amplitude(ProxiLine);
  AMSine.frequency(ProxiLine * AMfreqMax);

  // DYNAMIC MIX (stop hearing screaming sound while trembling)

  if (ProxiLine > 0.1 ) {
    mixer1.gain(0, 0);
    mixer2.gain(0, 0);
  }
  else {
    mixer1.gain(0, 1);
    mixer2.gain(0, 1);
  }
  //   resume processing
  AudioInterrupts();

  LoopFile("tremble.wav");  // Call and play the looping sound of trembling.

  // MONITORING

  //  Serial.print("Millis - Reset Millis : ");
  //  Serial.println(millis() - resetMillis);
  //  Serial.print("Micro : ");
  //  Serial.println(Micro_Moyenne);
  //  Serial.print("Proxi Median : ");
  //  Serial.println(ProxiMedian);
  //  Serial.print("DETECT PASSAGE : ");
  //  Serial.print(DetectPassage);
  //  Serial.print("  !!  JAUGE PASSAGE : ");
  //  Serial.println(jaugePassage);
  //  Serial.print("DETECT PEAK : ");
  //  Serial.print(DetectPeak);
  //  Serial.print("  !!  JAUGE PEAK : ");
  //  Serial.println(jaugePeak);
  //  Serial.println("----------");
  //    AudioProcessorUsageMaxReset();
  //    AudioMemoryUsageMaxReset();
  //    Serial.print("CPU =");
  //    Serial.print(AudioProcessorUsage());
  //    Serial.print(",");
  //    Serial.print(AudioProcessorUsageMax());
  //    Serial.print("    ");
  //    Serial.print("Memory: ");
  //    Serial.print(AudioMemoryUsage());
  //    Serial.print(",");
  //    Serial.print(AudioMemoryUsageMax());
  //    Serial.println("    ");

  delay(10);
}


//-------------

//FONCTIONS

//-------------

// CALIBRATION PROXI ET MICRO

void CalibProxiMic () {
  ProxiMin = 1023;
  ProxiMax = 0;
  MicroMin = 1023;
  MicroMax = 0;
  ThreshPassage = 180 ;
  ThreshPeak = 60 ;
  mixer3.gain(0, 0);
  mixer4.gain(0, 0);
  int InitTimerCalib = millis();
  Serial.println(" >>>>>>>>>>> Début de Calibration <<<<<<<<<< ");
  while (millis() - InitTimerCalib < TempsCalib) {
    Proxi = analogRead(ProxiPin);
    MicroRms = rms1.read() * 1000;
    MicroRA.addValue(MicroRms);
    Micro_Moyenne = MicroRA.getAverage();

    // record the minimum Proxi Value
    if (Proxi < ProxiMin) {
      ProxiMin = Proxi;
    }
    // record the maximum Proxi value
    if (Proxi > ProxiMax) {
      ProxiMax = Proxi;
      ThreshPassage = ProxiMax - ProxiMax * 5 / 100 ;    // REGLAGE auto du seuil de détection d'un passage à -10% de la valeur max

    }

    if (Micro_Moyenne < MicroMin) {
      MicroMin = Micro_Moyenne;
    }
    // record the maximum Proxi value
    if (Micro_Moyenne > MicroMax) {
      MicroMax = Micro_Moyenne;
      ThreshPeak = MicroMin * 2;                        // REGLAGE auto du seuil de détection d'un PEAK
    }
    delay(10);
  }
  mixer3.gain(0, 1);
  mixer4.gain(0, 1);
  if (ProxiMin > ProxiMax / 2) ProxiMin = ProxiMax / 2 ;
  TransitionTrigLoop = (ProxiMax - ProxiMin) / 2;

  Serial.print("ProxiMin & ProxiMax : ");
  Serial.print(ProxiMin);
  Serial.print(" & ");
  Serial.println(ProxiMax);
  Serial.print("MicroMin & MicroMax : ");
  Serial.print(MicroMin);
  Serial.print(" & ");
  Serial.println(MicroMax);
  Serial.print("ThresPassage : ");
  Serial.print(ThreshPassage);
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


//-------------

// FONCTIONS JAUGE : COMPTE NB D'EVENEMENTS DANS UN INTERVALLE DE TEMPS DONNE ET L'INTERPOLE LINEAIREMENT PAR RAPPORT A LA VALEUR PRECEDENTE ET PENDANT L'INTERVALLE DE TEMPS DETERMINE.

float JaugePassage (bool DetectPass, const int TimeWindow) {

  static int nbPass = 0;
  static int arriveePass = 0;
  static int departPass = 0;
  static uint32_t TempsPass = 0;
  static bool LastDetectPass = 0;
  static bool LastStockagePass = 0;
  static bool StockagePass = 0;

  if (DetectPass != LastDetectPass) { //Detection d'un front montant
    if (DetectPass) {
      nbPass++;  // Incrémentation du nombre de passage.
      LastDetectPass = 1;
    }
    else {
      LastDetectPass = 0;
    }
  }

  StockagePass = millis() - TempsPass > TimeWindow; // Condition de Temps pour retourner la future valeur de la jauge (toutes les X secondes)

  if (StockagePass != LastStockagePass) {
    if (StockagePass) {
      departPass = arriveePass;
      arriveePass = nbPass; // On retourne le nombre d'évènements.
      TempsPass = (float)millis();  //On remet le compteur de temps à 0 pour la prochaine boucle et avoir un snapshot régulier.
      nbPass = 0; // On remet le nombre d'évènements à 0.
      LastStockagePass = 1;
    }
    else {
      LastStockagePass = 0;
    }
  }
  float coef = (arriveePass - departPass) / TimeWindow ;
  float Line = (coef * ((float)millis() - TempsPass) + departPass); // Interpolation linéaire (= Line pure data)
  return Line;
}

//-------------
//-------------

float JaugePeak (bool DetectEvent, float TimeWindow) {

  static float nbEvent = 0;
  static float arrivee = 0;
  static float depart = 0;
  static float Temps = 0;
  static bool LastDetectEvent = 0;
  static bool LastStockageEvent = 0;
  static bool StockageEvent = 0;

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

  float coef = (arrivee - depart) / TimeWindow ;
  float Line = (coef * ((float)millis() - Temps) + depart); // Interpolation linéaire (= Line pure data)
  return Line;
}

//-------------
//-------------

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
  delay(20);
}

//-------------
//-------------

// LOOP UN FICHIER SON

void LoopFile(const char *filename)
{
  if (playSdWav2.isPlaying() == false) {
    playSdWav2.play(filename);
    delay(20); // wait for library to parse WAV info
  }
}

//-------------
//-------------

// INTERPOLATION LINEAIRE EN UN TEMPS DONNE (=LINE dans Pure Data)

float Line(float arrivee, float TimeInterpol) {
  static float Temps = 0;
  static float depart = 0;
  static float coef = 0;
  static float line = 0;
  static float PreviousArrivee = 0;

  if (arrivee != PreviousArrivee) {
    PreviousArrivee = arrivee ;
    depart = line;
    Temps = (float)millis();
  }
  coef = (ProxiValue - depart) / TimeInterpol ;
  if ((float)millis() - Temps <= TimeInterpol) {
    line = (coef * ((float)millis() - Temps) + depart);
  }
  return line;
}
