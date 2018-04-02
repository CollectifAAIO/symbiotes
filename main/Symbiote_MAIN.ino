#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MedianFilter.h>   //https://github.com/daPhoosa/MedianFilter
#include <RunningAverage.h> //https://playground.arduino.cc/Main/RunningAverage
#include <Bounce.h>

// GUItool: begin automatically generated code
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=253,378
AudioPlaySdWav           playSdWav2;     //xy=253,409
AudioSynthNoiseWhite     noise1;         //xy=269,303
AudioMixer4              mixer2;         //xy=403,393
AudioMixer4              mixer1;         //xy=405,322
AudioMixer4              mixer3;         //xy=545,361
AudioInputI2S            i2s2;           //xy=554,209
AudioAnalyzeRMS          rms2;            //xy=740,194
AudioAnalyzePeak         peak2;          //xy=740,225
AudioAnalyzeRMS          rms1;           //xy=742,291
AudioAnalyzePeak         peak1;          //xy=743,324
AudioOutputI2S           i2s1;           //xy=749,368
AudioConnection          patchCord1(playSdWav1, 0, mixer2, 0);
AudioConnection          patchCord2(playSdWav1, 1, mixer2, 1);
AudioConnection          patchCord3(playSdWav2, 0, mixer2, 2);
AudioConnection          patchCord4(playSdWav2, 1, mixer2, 3);
AudioConnection          patchCord5(noise1, 0, mixer1, 0);
AudioConnection          patchCord6(mixer2, 0, mixer3, 1);
AudioConnection          patchCord7(mixer1, 0, mixer3, 0);
AudioConnection          patchCord8(mixer3, 0, i2s1, 0);
AudioConnection          patchCord9(mixer3, 0, i2s1, 1);
AudioConnection          patchCord10(mixer3, rms1);
AudioConnection          patchCord11(mixer3, peak1);
AudioConnection          patchCord12(i2s2, 0, peak2, 0);
AudioConnection          patchCord13(i2s2, 0, rms2, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=448,527
// GUItool: end automatically generated code


// Settings filtrage

MedianFilter MedianProx(15, 0);
RunningAverage MicroRA(50);


//Use these with the audio adaptor board

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#define BUTTON 0
#define LED 13
#define ProxiPin A0;

// DECLARATION & INIT VARIABLES


//init Entrées capteurs.

Bounce boutonCalib = Bounce(BUTTON, 15 );

int Proxi;
uint8_t MicroRms;

int Proxi_Median = 0;
int Micro_Moyenne = 0;
int Proxi_map = 0;

int ProxiMin = 1023;        // minimum sensor value
int ProxiMax = 0;           // maximum sensor value
int MicroMin = 1023;        // minimum sensor value
int MicroMax = 0;           // maximum sensor value

// Variables calib

int TempsAvantCalibAuto = 60 ; //REGLAGE// temps avant la calibration automatique, en minutes
int DuréeCalib = 20000;        //REGLAGE

int resetTimeCalib = 0;
int resetTimeCalibButton = 0;
int TpsPressBouton = 500;

//Valeurs de seuil de changement de dossier

int Seuil1Micro = 50;         //REGLAGE
int Seuil2Micro = 75;         //REGLAGE
int Seuil1Proxi = 25;         //REGLAGE
int Seuil2Proxi = 50;         //REGLAGE
int Seuil3Proxi = 75;         //REGLAGE
int Seuil4Proxi = 90;         //REGLAGE

// Variables fonction LINE

float coef = 0;               //REGLAGE
float depart = 10.0;          //REGLAGE
float arrivee = 0.0;          //REGLAGE
float line = 0;               //REGLAGE
float Ramp = 10000;           //REGLAGE

// Variables fonction RANDOMTRIG

int treshTrig = 0;            //REGLAGE
int borneMinMin = 50;         //REGLAGE
int borneMinMax = 1000;       //REGLAGE
int borneMaxMin = 10000;      //REGLAGE
int borneMaxMax = 20000;      //REGLAGE

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
String HumeurFolder[8]  = {"Serein", "Normal_expressif", "Timide", "Peureux", "Heureux", "Euphorique", "Enervé", "Agressif"};
int selecthumeur = 0;
int previoushumeur = 1000;
int statechangeselecthumeur = 0;

// Variables Moyenne Passages

bool etatPassage = 0;
bool ConditionStockage = 0;
bool LastConditionStockage = 0;

int TimeSnapshot = 5000; //Temps auquel on scan le nombre de passages en ms.
int nbPassage = 0;
int Temps = 0;
int SizeArrayPassage = 9;
int StockPass[10] = {0};
int seuilPassage = 150; // Seuil sous lequel le proxi déclenche le comptage d'un passage. A Calibrer à 1/3 de la valeur Max de Proxi.
bool LastPassageValue = 0;
int i = 0; //incrément du tableau
int x = 0; //lire le tableau via l'incrément de la boucle for
int SumTab = 0;
int Moy_Pass = 0;

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
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(50);
  pinMode(BUTTON, INPUT);
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

  Proxi = analogRead(ProxiPin);              // DOIT ETRE COMPRIS DE 0 à 1
  MicroRms = rms1.read() * 1000;              // DOIT ETRE COMPRIS DE 0 à 1
  boutonCalib.update();
  int boutonCal = boutonCalib.read();

  //FILTRAGE DONNEES
  MedianProx.in(Proxi);
  Proxi_Median = MedianProx.out();

  if (! playSdWav1.isPlaying()) {  // Coupe l'arrivée de donnée du microphone.
    MicroRA.addValue(MicroRms);
  }
  Micro_Moyenne = MicroRA.getAverage();

  //Sortie capteurs à utiliser dans la suite : Proxi_Median & Micro_Moyenne.


  // COMPTAGE PASSAGES

 etatPassage = Proxi_Median < seuilPassage; //Condition pour déclencher le comptage du nombre de passages.

  if (etatPassage != LastPassageValue) { //Detection d'un Front
    if (etatPassage) { //front montant
      nbPassage++;  // Incrémentation du nombre de passage. 
      LastPassageValue = 1;
    }
    else {
      LastPassageValue = 0;
    }
  }

  ConditionStockage = millis() - Temps > TimeSnapshot; // Condition de temps pour stocker la valeur du nombre de passages (toutes les X secondes) 

  if (ConditionStockage != LastConditionStockage) {
    if (ConditionStockage) {
      Temps = millis();  //On remet le compteur de temps à 0 pour la prochaine boucle et avoir un snapshot régulier.
      StockPass[i] = nbPassage;  //On stock le nombre de passage du dernier intervalle de temps dans le tableau.
      nbPassage = 0; //On remet le nombre de passages à 0. 
      LastConditionStockage = 1;

      for (x = 0; x < SizeArrayPassage + 1; x++) {  
        SumTab += StockPass[x];          //On additionne toutes les valeurs du tableau. 
      }
    }
    else {
      LastConditionStockage = 0;
      Moy_Pass = SumTab / (SizeArrayPassage); //On fait la MOYENNE des valeurs du dernier tableau.
      SumTab = 0; // On remet la somme à 0. 
      i++; //On incrémente le numéro du tableau pour remplir la case suivante. 
      if (i > SizeArrayPassage) {  
        i = 0;              //On remet l'incrément à 0 si on dépasse le nombre de case max du tableau.
      }
    }
  }


  // CALIBRATION

  //Recalibre les valeurs toutes les heures.
  if (millis - resetTimeCalib > TempsAvantCalibAuto * 60 * 1000) {
    resetTimeCalib = millis
    while (millis - resetTimeCalib < DuréeCalib) {

      if (Proxi_Median < ProxiMin) {
        ProxiMin = Proxi_Median;
      }
      if (Micro_Moyenne < MicroMin) {
        MicroMin = Micro_Moyenne;
      }
      // record the maximum sensor value
      if (Proxi_Median > ProxiMax) {
        ProxiMax = Proxi_Median;
      }
      if (Micro_Moyenne > MicroMax) {
        MicroMax = Micro_Moyenne;
      }
    }
  }

  //Recalibre les valeurs quand on presse le bouton plus de deux secondes.

  if (boutonCalib != LastButtonValue) { //Detection d'un Front
    if (boutonCalib == 1) { //front montant
      PressInstant = millis(); //photo du temps
      LastButtonValue = 1; // "change"
    }
    else { //front descendant
      if (millis() - PressInstant < TpsPressBouton) {//relachement court
        Serial.println(     "Change Skin");
        LastButtonValue = 0;//remise à 0 pour ne pas repasser dans le if
      }
      else {
        resetTimeCalib = millis();
        while (millis - resetTimeCalib < DuréeCalib) {

          if (Proxi_Median < ProxiMin) {
            ProxiMin = Proxi_Median;
          }
          if (Micro_Moyenne < MicroMin) {
            MicroMin = Micro_Moyenne;
          }
          // record the maximum sensor value
          if (Proxi_Median > ProxiMax) {
            ProxiMax = Proxi_Median;
          }
          if (Micro_Moyenne > MicroMax) {
            MicroMax = Micro_Moyenne;
          }
        }
        LastButtonValue = 0;//remise à 0 pour ne pas repasser dans le if
      }
    }
  }








  //MAPPING VAL MIN MAX CALIBRATION de 0 à 100.

  Proxi_map = map(ProxiValue, ProxiMin, ProxiMax, 0, 100);
  Micro_Moyenne_map = map(Micro_Moyenne, ProxiMin, ProxiMax, 0, 100);

  // CLIP
  ProxiValue = constrain(ProxiValue, 0, 100);
  MicroValue = constrain(Micro_Moyenne, 0, 100);






  // somme les deux capteurs pour ne faire qu'une variable qui sert à déterminer la fourchette de temps dans laquelle le nombre aléatoire va être piocher. Ce nombre déterminera après combien de temps le player rejouera un nouveau son.
  int sensorSum = ProxiValue + MicroRms ;  //on somme pour avoir qu'une seule valeur de capteur qui définie la plage de valeur dans laquelle choisir un temps aléatoire.
  sensorSum = (2 - sensorSum) / 2;        // on divise par 2 pour garder un nombre compris entre 0 et 1.

  // SELECTIONNE LA BONNE  HUMEUR

  if (MicroRms < Seuil1Micro) {
    if (ProxiValue < Seuil1Proxi) {
      selecthumeur = 0;

    }
    if (ProxiValue > Seuil1Proxi && ProxiValue < Seuil2Proxi) {
      selecthumeur = 1;
    }
    if (ProxiValue > Seuil2Proxi && ProxiValue < Seuil3Proxi) {
      selecthumeur = 2;
    }
    if (ProxiValue > Seuil3Proxi && ProxiValue < Seuil4Proxi) {
      selecthumeur = 3;
    }
    while (ProxiValue > Seuil4Proxi) {
      playSdWav1.stop();
      delay(TempsCamouflage);
    }
  }

  if (MicroRms > Seuil1Micro && MicroRms < Seuil2Micro) {
    if (ProxiValue < Seuil2Proxi) {
      selecthumeur = 4;
    }
    if (ProxiValue > Seuil2Proxi) {
      selecthumeur = 6;
    }
  }

  if (MicroRms > Seuil2Micro) {
    if (ProxiValue < Seuil2Proxi) {
      selecthumeur = 5;
    }
    if (ProxiValue > Seuil2Proxi) {
      selecthumeur = 7;
    }
  }

  // SELECTIONNE UN FICHIER ALEATOIREMENT SANS LE REPETER DANS LE DOSSIER DE LA BONNE HUMEUR.

  if (selecthumeur != previoushumeur) {        //State change Detection

    sample_rand = URN(corpusSampleNumber);                // URN A IMPLEMENTER ICI
    SoundFile = '/' + HumeurFolder[selecthumeur] + '/' + sample_rand + SoundType;  //inttochar
    Serial.println(SoundFile);
    previoushumeur = selecthumeur;            //State change Detection
  }

  sensor = (ProxiValue + MicroValue) / 2 ;
  borneMin = map(sensor, 0, 100, borneMinMin, borneMinMax);
  borneMax = map(sensor, 0, 100, borneMaxMin, borneMaxMax);

  // RANDOM TRIG

  if (millis() - resetMillis > treshTrig) {
    resetMillis = millis();
    playFile(SoundFile.c_str()); //JOUE LE fichier après un temps tiré aléatoirement définie dans une fourchette qui varie selon la somme des deux capteurs. .c_str() passe un string en char (en gros...)
    treshTrig = random(borneMin, borneMax);
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

//-------------

//-------------

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

void RandomTrig (int sensor, borneMinMin, borneMinMax, borneMaxMin, borneMaxMin) {

  borneMin = map(sensor, 0, 100, borneMinMin, borneMinMax);
  borneMax = map(sensor, 0, 100, borneMaxMin, borneMaxMax);

  if (millis() - resetMillis > treshTrig) {
    resetMillis = millis();
    playFile(SoundFile.c_str()); //JOUE LE SON .c_str() passe un string en char (en gros...)
    treshTrig = random(borneMin, borneMax);
  }
}

// ------------

// FONCTION LINE : Le temps est à initialiser dans la condition avant le line qui le déclenche.

float Line(bool Trig, float depart, float arrivee, float Ramp) {
  coef = (arrivee - depart) / Ramp ;
  if (Trig) {
    if ((float)millis() - Temps <= Ramp) {
      line = (coef * ((float)millis() - Temps) + depart);
    }
    else {
      Trig = 0; // remet la condition à 0 pour que le line s'arrête.
    }
  }
  return line;
}

