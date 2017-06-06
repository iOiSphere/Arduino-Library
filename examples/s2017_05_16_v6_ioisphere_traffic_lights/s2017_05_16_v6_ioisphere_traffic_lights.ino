//client.get("http://192.168.0.150/1");
// lightwave sendI2C(150);

#include <Wire.h>
#include <Anemone.h>
#include <SPI.h>
#include <Bridge.h>
#include <Process.h>

Anemone anemone(8);

int line0[] = {9, 18, 32, 52, 76, 103, 134, 169, 204};
int line1[] = {11, 21, 36, 37, 58, 84, 113, 145, 179};
int line2[] = {7, 13, 24, 41, 63, 64, 90, 153,   187};
int line3[] = {8, 15, 28, 47, 69, 96, 126, 160, 195};
int line4[] = {10, 16, 30, 49, 72, 99, 129, 163, 198};


// feature pins
const int blowerSwitch    = 118;
const int blowerPin       = 5;
int blowerTimer = 0;

const int lightbulb = 192;

// RING 10
int launchCount = 0;
int sphereArmed = false;

// RINGS 7,8,9 pattern
int patternStart;
int patternEnd;

// PATTERN 0
int pattern0Length = 3;
int pattern0Pointer = 0;
int pattern0[] = {28, 29, 30};

// PATTERN 1
int pattern1Length = 3;
int pattern1Pointer = 0;
int pattern1[] = {25, 13, 7};

// PATTERN 2
int pattern2Length = 3;
int pattern2Pointer = 0;
int pattern2[] = {9, 10, 11};


// RING 6 Chase
int pongStart;
int pongEnd;
int pongPos;
int pongDir;
int pongCount = 0;        // play traffic lights on 20

// RING 4, 5 Chase
int chaseStart;
int chaseEnd;
int chaseTotalLEDs;

int chaseCount = 0;
int chase1;
int chase2;
int chase3;

// RING 3 - SoundBoard
int soundboardStart = anemone.getRingStart(2);
int soundboardEnd   = anemone.getRingEnd(2);

// RING 1,2 Seqquencer
int playHead0 = anemone.getRingStart(0);
int playHead1 = anemone.getRingStart(1);
int playHead0Sound = random(10);
int playHead1Sound = random(10, 20);
bool playHead0PrevState = false;
bool playHead1PrevState = false;
int sequencerStart;
int sequencerEnd;


int tickDelay = 500;
unsigned long prevTick;

int countNum = 0;
int startDelay = 50;    // USED TO CANCEL THE NOISE ON BOOT  - !! IMPORTANT !!

void setup() {
  Bridge.begin();  // Initialize the Bridge
  SerialUSB.begin(9600);  // Initialize the Serial

  // Wait until a Serial Monitor is connected.
  //while (!SerialUSB);
  SPI.begin();
  Serial.begin(115200);
  Wire.begin(); // join i2c bus (address optional for master)


  anemone.clear();
  anemone.saveSwitchStates();

  // init pong
  pongStart = anemone.getRingStart(5);
  pongEnd =   anemone.getRingEnd(5);
  pongPos = random(pongStart, pongEnd);
  pongDir = -1;

  // init chase
  int ring3num    = anemone.getRingNodeCount(4);
  int ring4num    = anemone.getRingNodeCount(5);
  chaseTotalLEDs  = ring3num + ring4num;
  chaseStart      = 63;
  chaseEnd        = 108;
  chase1        = 76;
  chase2        = 86;
  chase3        = 68;
  //anemone.ledWrite(chase1, LEDHIGH);
  //anemone.ledWrite(chase2, LEDHIGH);
  //anemone.ledWrite(chase3, LEDHIGH);

  // init sequencer
  sequencerStart  = anemone.getRingStart(1);
  sequencerEnd    = anemone.getRingEnd(0);

  pinMode(blowerPin, OUTPUT);

  Serial.println("Anemone Ready!");
  randomSeed(analogRead(A3));
  delay(500);
  sendI2C(61);
}

void loop() {

  // process, send and receive everything!
  anemone.update();

  // pause on boot up to reduce serial noise
  if (startDelay > 0) {

    startDelay--;
    return;
  }



  // SPHERE HEART BEAT
  if (millis() - prevTick > tickDelay) {

    stepSequencer();
    pongStep();
    prevTick = millis();

  }

  // normal going on the sphere won't be armed
  if (sphereArmed == false) {

    soundboardTimerTick();
    checkChaseReset();
    checkSequencerReset();

    // Process all interactions that don't rely on a timer
    processSwitches();

  } else {

    // SPHERE ARMED - STOP EVERYTHING AND WAIT FOR PIN 6!!!!!
    anemone.clear();
    anemone.ledWrite(6, LEDHIGH);
    anemone.updateLEDs();
    if (anemone.node(6) == true) launch();
  }
}


void processSwitches() {

  pongInput();
  chaseInput();
  sequencerInput();
  soundboardInput();
  patternInput();
  blower();

  if (anemone.node(67) == true){

    // 1 - red     2 - amber      3 - green
    String trafficLightRed = "python /mnt/sda1/arduino/www/http.py 1";
    callPython(trafficLightRed);

  }

  // Interaction with all pins
  for (int i = 0; i < BITNUMBER; i++) {
    if (anemone.node(i) == true) {
      Serial.println(i);
      launchStep();
    }
  }
}





