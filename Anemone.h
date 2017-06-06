/**
* @Author: Alcwyn Parker <alcwynparker>
* @Date:   2017-04-28T21:05:43+01:00
* @Email:  alcwynparker@gmail.com
* @Project: Anemone
* @Filename: Anemone.h
* @Last modified by:   alcwynparker
* @Last modified time: 2017-05-09T23:32:09+01:00
*/
#ifndef Anemone_h
#define Anemone_h

#define BYTENUMBER 26
#define BITNUMBER 208
#define LEDLOW 1
#define LEDHIGH 0

#include "Arduino.h"
#include <SPI.h>

class Anemone
{
  public:
    Anemone(int pin);
    void start();


    // rings
    int getRingStart(int ring);
    int getRingEnd(int ring);
    int getRingNodeCount(int ring);
    void lightRing(int ring);
    void clearRing(int ring);
    void updateLEDs();


    // Utils
    void update();      // debounce array the final location
    void clear();
    void ledWrite(int index, int state);
    bool ledRead(int index);
    void toggleLED(int index);
    void saveSwitchStates();
    bool node(int);    // debounce output for single node

    void debugArray(byte *a);
    void debugByte(byte b);

  private:

    int ssout;

    // ring placements
    int ringStarts[9] =     {174, 140, 109, 81,  57, 37, 22, 12, 7};
    int ringEnds[9] =       {207, 173, 139, 108, 80, 56, 36, 21, 11};
    int ringNodeCount[9] =  {33,  33,  30,  27,  23, 19, 14, 9,  4 };

    // OUT
    byte sendBytes[BYTENUMBER];

    // IN
    byte returnBytes[BYTENUMBER];
    byte prevReturnBytes[BYTENUMBER];

    // DEPBOUNCE PROCESSING
    byte activeSwitches[BYTENUMBER];
    uint8_t switchTimers[BITNUMBER];
    byte debounce[BYTENUMBER];

    void checkActiveSwitches();
    bool checkSwitch(int index);

    void processReturnBytes();

    // debounce
    uint8_t getSwitchTimer(int index);
    void setSwitchTimer(int index, int val);
    void stepSwitchTimer(int index);
    void overrideActiveSwitch(int index, int val);
    void clearDebounce();

};

#endif
