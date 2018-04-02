#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <MedianFilter.h>   //https://github.com/daPhoosa/MedianFilter
#include <RunningAverage.h> //https://playground.arduino.cc/Main/RunningAverage
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav1;     //xy=108,284
AudioMixer4              mixer1;         //xy=401,298
AudioMixer4              mixer2;         //xy=401,366
AudioOutputI2S           i2s1;           //xy=861,359
AudioConnection          patchCord1(playSdWav1, 0, mixer1, 0);
AudioConnection          patchCord2(playSdWav1, 1, mixer2, 0);
AudioConnection          patchCord3(mixer1, 0, i2s1, 0);
AudioConnection          patchCord4(mixer2, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=104,68
// GUItool: end automatically generated code


//Use these with the audio adaptor board

#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

#define LED 13

// DECLARATION & INIT VARIABLES

// Settings fonctions

MedianFilter MedianProx(15, 0);
RunningAverage MicroRA(15);

// functions declaration

void CalibProxiMic();
int CountingFile (File DirName);
float JaugePeak (bool DetectEvent, float TimeWindow);
int URN(int nbtotfile);
void TrigFile(const char *filename);
void initSampleSelecteur(int nbtotfile);

// Data variables

int Proxi;
float MicroRms;
int ProxiMedian = 0;
float Micro_Moyenne = 0;

// Variables calib Proxi + Micro

int TempsCalib = 5000 ;            // REGLAGE  : durée calibration.
int ProxiMin = 1023;
float ProxiMax = 0;
float MicroMin = 1023;
float MicroMax = 0;

// Counting files per folder variables

File root;
int NbFiles[4] = {0};

// GAUGE variables

int NbPeak = 0;
int ThreshPeak = 20 ;              //(10 par défaut par rapport à la dérivée)
bool DetectPeak = 0;
int TpsStockagePeak = 1500;       //REGLAGE (Temps de stockage de la jauge Micro)
float jaugePeak = 0;

// Threshold of changing folder and retriger timing.

int SeuilProxi;
int Condition = 0;
int SeuilJaugeMicro = 1;              //REGLAGE (Seuil de changement d'humeur Micro)

// retriger Range.

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

// TRIGFILE function variable

int resetMillis = 0;
int threshTrig = 0;
int corpusSampleNumber = 10;   // Number of sounds in each folder (automatically defined)

int sample_rand = 1;           // Random sample number.
String SoundFile, SoundType;
String HumeurFolder[4]  = {"serein", "timide", "hilare", "hostile"};
int selecthumeur = 0;
int previoushumeur = 1000;
int statechangeselecthumeur = 0;

// Variables Timide
int FreezeTimideTime = 60000;           // REGLAGE (Shy mode time for freezing loop)

//-------------
//-------------

//                                   >>>>> SETUP <<<<<

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A9));

  //AUDIO

  AudioMemory(70);             // Audio connections require memory to work.  For more detailed information, see the MemoryAndCpuUsage example
  // setup audio board
  SoundFile = String();
  SoundType = String(".wav");
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7);                         //REGLAGE
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
  // Counting files in each folder

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

  // Calibration (see the function bellow)
  CalibProxiMic();
}

// >>>>> MAIN LOOP <<<<<

void loop() {
  // DATA READING

  Proxi = random(20, 300);              // DOIT ETRE COMPRIS DE 0 à 1
  MedianProx.in(Proxi);
  ProxiMedian = MedianProx.out();

  MicroRms = random(20, 50);            // DOIT ETRE COMPRIS DE 0 à 1
  MicroRA.addValue(MicroRms);
  Micro_Moyenne = MicroRA.getAverage();



  // GAUGE PEAK DETECTION

  // Detect peak
  if (Micro_Moyenne > ThreshPeak) {
    DetectPeak = 1;
  }
  else {
    DetectPeak = 0;
  }

  // Count the number of peak every x secondes and linearly interpole values (see funtion bellow)

  jaugePeak = JaugePeak(DetectPeak, TpsStockagePeak);

  // SELECT THE RIGHT FOLDER (corresponding to the right mood) AND RETRIG SETTING ACCORDING TO DATA.

  // Serein
  if (jaugePeak < SeuilJaugeMicro && ProxiMedian > SeuilProxi) {
    if (Condition != 1) {
      corpusSampleNumber = NbFiles[0] ;          // Update the number of available soundfiles in "serein" folder
      RangeMin = SeuilProxi;
      RangeMax = ProxiMax;
      BorneMinMin = 5000 ;                        // REGLAGE
      BorneMinMax = 10000;                        // REGLAGE
      BorneMaxMax = 20000;                        // REGLAGE
      selecthumeur = 0 ;
      Condition = 1;
    }
  }

  // Hostile
  if (jaugePeak < SeuilJaugeMicro && ProxiMedian < SeuilProxi) {
    if (Condition != 2) {
      corpusSampleNumber = NbFiles[3];          // Update the number of available soundfiles in "hostile" folder
      RangeMin = ProxiMin;
      RangeMax = SeuilProxi;
      BorneMinMin = 100 ;                        // REGLAGE
      BorneMinMax = 200;                        // REGLAGE
      BorneMaxMax = 1000;                        // REGLAGE
      selecthumeur = 3;
      Condition = 2;
    }
  }

  // Hilare
  if (jaugePeak > SeuilJaugeMicro && ProxiMedian > SeuilProxi) {
    if (Condition != 3) {
      corpusSampleNumber = NbFiles[2];          // Update the number of available soundfiles in "hilare" folder
      RangeMin = SeuilProxi;
      RangeMax = ProxiMax;
      BorneMinMin = 500 ;                        // REGLAGE
      BorneMinMax = 1000;                        // REGLAGE
      BorneMaxMax = 3000;                        // REGLAGE
      selecthumeur = 2;
      Condition = 3;
    }
  }

  // Timide
  if (jaugePeak > SeuilJaugeMicro && ProxiMedian < SeuilProxi) {
    if (Condition != 4) {
      corpusSampleNumber = NbFiles[1];                     // Update the number of available soundfiles in "timide" folder
      RangeMin = ProxiMin;
      RangeMax = SeuilProxi;
      BorneMinMin = 500 ;                        // REGLAGE
      BorneMinMax = 1000;                        // REGLAGE
      BorneMaxMax = 3000;                        // REGLAGE
      selecthumeur = 1;
      Condition = 4;
    }
  }

  //Changing the limits of the random number which define the time after playing another sound. Depends on the data.

  ProxiRangeMin = map(ProxiMedian, RangeMin, RangeMax, BorneMinMin, BorneMinMax) ;
  ProxiRangeMax = map(ProxiMedian, RangeMin, RangeMax, BorneMinMax, BorneMaxMax) ;
  randomMin = constrain(ProxiRangeMin, BorneMinMin, BorneMinMax) ;
  randomMax = constrain(ProxiRangeMax, BorneMinMax, BorneMaxMax) ;
  threshTrig = random(randomMin, randomMax);

  // PLAYER AUDIO

  // Random Retrig : PLAY A RANDOM NO REPEAT SOUND AFTER A CERTAIN AMOUNT OF TIME DETERMINED BY THE LIMIT (see above)

  if (millis() - resetMillis > threshTrig) {
    resetMillis = millis();
    sample_rand = URN(corpusSampleNumber) + 1;                  // RANDOM NO REPEAT TO CHOOSE A NEW SAMPLE IN THE RIGHT FOLDER
    SoundFile = HumeurFolder[selecthumeur] + "/" + sample_rand + SoundType;  //inttochar
    TrigFile(SoundFile.c_str());                                // TRIG THE GOOD SOUNDFILE
  }
  delay(500);
}

//-------------

//FONCTIONS

//-------------

// CALIBRATION PROXI & MICRO

void CalibProxiMic () {

  int InitTimerCalib = millis();
  Serial.println(" >>>>>>>>>>> Début de Calibration <<<<<<<<<< ");
  while (millis() - InitTimerCalib < TempsCalib) {
    Proxi = random(20, 300);
    MicroRms = random(20, 50);
    MicroRA.addValue(MicroRms);
    Micro_Moyenne = MicroRA.getAverage();
    // record the minimum Proxi Value
    if (Proxi < ProxiMin) {
      ProxiMin = Proxi;
    }
    // record the maximum Proxi value
    if (Proxi > ProxiMax) {
      ProxiMax = Proxi;
    }
    // record the minimum Micro Value
    if (Micro_Moyenne < MicroMin) {
      MicroMin = Micro_Moyenne;
    }
    delay(10);
  }
  if (ProxiMin > ProxiMax / 2) ProxiMin = ProxiMax / 2 ;
  SeuilProxi = ProxiMax / 2;
  ThreshPeak = MicroMin * 3;

  Serial.print("ProxiMin & ProxiMax : ");
  Serial.print(ProxiMin);
  Serial.print(" & ");
  Serial.println(ProxiMax);
  Serial.print("MicroMin : ");
  Serial.print(MicroMin);
  Serial.print("  !!  Seuil Proxi : ");
  Serial.print(SeuilProxi);
  Serial.print("  !!  ThresPeak : ");
  Serial.println(ThreshPeak);
  Serial.println(" >>>>>>>>>>> Fin de Calibration <<<<<<<<<< ");
}

//-------------

// COUNTING THE NUMBER OF FILE PER FOLDER.

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

// GAUGE FUNCTION : Counting event number in a certain range of time. Linearly interpol the past value to the present in this range of time.

float JaugePeak (bool DetectEvent, float TimeWindow) {

  static float nbEvent = 0;
  static float arrivee = 0;
  static float depart = 0;
  static float Temps = 0;
  static bool LastDetectEvent = 0;
  static bool LastStockageEvent = 0;
  static bool StockageEvent = 0;

  if (DetectEvent != LastDetectEvent) {
    if (DetectEvent) {
      nbEvent++;  // Increase peak number.
      LastDetectEvent = 1;
    }
    else {
      LastDetectEvent = 0;
    }
  }

  StockageEvent = millis() - Temps > TimeWindow; // Time condition before returning the next gauge value (every X secondes)

  if (StockageEvent != LastStockageEvent) {
    if (StockageEvent) {
      depart = arrivee;
      arrivee = nbEvent; // return the event number
      Temps = (float)millis();  // Reset the timing counter until the next loop.
      nbEvent = 0; // Reset event number
      LastStockageEvent = 1;
    }
    else {
      LastStockageEvent = 0;
    }
  }

  float coef = (arrivee - depart) / TimeWindow ;
  float Line = (coef * ((float)millis() - Temps) + depart); // linear Interpolation (= pure data Line object)
  return Line;
}

//-------------
//-------------

// FONCTION URN (Random No Repeat)

int index_tab = 0; // samples already played;
int sampleDejaJoues[100];
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

// TRIG Sound File and cut the previous one.

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
