#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MedianFilter.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav2;     //xy=151,351
AudioPlaySdWav           playSdWav1;     //xy=156,265
AudioInputI2S            i2s2;           //xy=238,178
AudioMixer4              mixer1;         //xy=390,262
AudioMixer4              mixer2;         //xy=390,329
AudioAnalyzeRMS          rms1;           //xy=403,172
AudioOutputI2S           i2s1;           //xy=674,266
AudioConnection          patchCord1(playSdWav2, 0, mixer1, 1);
AudioConnection          patchCord2(playSdWav2, 1, mixer2, 1);
AudioConnection          patchCord3(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord4(playSdWav1, 1, mixer2, 0);
AudioConnection          patchCord5(i2s2, 0, rms1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 0);
AudioConnection          patchCord7(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=367,405
// GUItool: end automatically generated code

//Use these with the audio adaptor board

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

//FILTRES DONNEES

MedianFilter test(31, 0);

// DECLARATION VARIABLES

String SoundFile, SoundType;
String HumeurFolder[8]  = {"Serein", "Normal_expressif", "Timide", "Peureux", "Heureux", "Euphorique", "Enervé", "Agressif"};
int selecthumeur = 0;
int previoushumeur = 1000;
int statechangeselecthumeur = 0;

// Fonction URN

int index_tab = 0; // nombre de samples deja jouées;
int sampleDejaJoues[100]; //Défini la taille du tableau et initialise toutes les valeurs à 0
int i = 0;
int unplayedSample = 0;
boolean etat = true;
int lastPlayedSample = -1;
int corpusSampleNumber = 5;


//Valeurs de seuil de changement de dossier

int Seuil1Micro = 0.5;
int Seuil2Micro = 0.75;
int Seuil1Proxi = 0.25;
int Seuil2Proxi = 0.5;
int Seuil3Proxi = 0.75;
int Seuil4Proxi = 0.9;

// Valeurs de timing.

int tresh = 0;
int TempsCamouflage = 10000;

//init capteurs.

int ProxiMin = 1023;        // minimum sensor value
int ProxiMax = 0;           // maximum sensor value
int MicroMin = 1023;        // minimum sensor value
int MicroMax = 0;           // maximum sensor value


int sensorsum = 0;

//char SoundFile[] = "TEST1.WAV"
//char SoundName[] = "TEST"
//char SoundType[] = ".WAV";
int sample_rand = 0;  // tirage au sort du numéro du sample.
boolean seuil = false;
int hysteresis = 0;


//int resetMillis = 0;
//int tresh = 500;
//int freq = 500;
//int trRd = 0;

//-------------

// DEBUT CODE

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
  sgtl5000_1.micGain(36);

  //CALIBRATION pendant les 10 premières secondes
  while (millis() < 10000) {
    int ProxiValue = analogRead(A0);
    uint8_t MicroRms = rms1.read();

    // record the maximum sensor value
    if (ProxiValue > ProxiMax) {
      ProxiMax = ProxiValue;
    }
    if (MicroRms > MicroMax) {
      MicroMax = MicroRms;
    }
    // record the minimum sensor value
    if (ProxiValue < ProxiMin) {
      ProxiMin = ProxiValue;
    }
    if (MicroRms < MicroMin) {
      MicroMin = MicroRms;
    }
  }

  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
}

void loop() {
  int Lightsensortest = analogRead(A0);
  int ProxiValue = analogRead(A1);              // DOIT ETRE COMPRIS DE 0 à 1
  uint8_t MicroRms = rms1.read();               // DOIT ETRE COMPRIS DE 0 à 1
  //  Serial.print("Valeur Capteur :");
  //  Serial.println(Lightsensortest);

  //FILTRAGE DONNEES.

  //  test.in(i);
  //  j = test.out();

  //MAPPING VAL MIN MAX CALIBRATION de 0 à 1
  ProxiValue = map(ProxiValue, ProxiMin, ProxiMax, 0, 1);
  ProxiValue = constrain(ProxiValue, 0, 1);
  MicroRms = map(MicroRms, ProxiMin, ProxiMax, 0, 1);
  MicroRms = constrain(MicroRms, 0, 1);
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

  if (selecthumeur != previoushumeur) {        //State change Detection

    // SELECTIONNE LE BON FICHIER

    playSdWav1.stop();

    sample_rand = random(1, 5);                // URN A IMPLEMENTER ICI

    SoundFile = '/' + HumeurFolder[selecthumeur] + '/' + SelectUnplayedSample(corpusSampleNumber) + SoundType;  //inttochar
    Serial.println(SoundFile);
    //    playFile(SoundFile.c_str());            //JOUE LE SON .c_str() passe un string en char (en gros...)
    previoushumeur = selecthumeur;            //State change Detection
  }

  //JOUE LE FICHIER APRES UN TEMPS TIRE ALEATOIREMENT DANS UNE FOURCHETTE QUI VARIE SELON CAPTEUR
  randomTrig(sensorSum, 50, 200, 2950, 19800);

  delay(5);
}

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

// FONCTION URN Thomas (tire aléatoirement un son qui n'a pas encore été joué)
int SelectUnplayedSample(int nbtotfile) {
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

