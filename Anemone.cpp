/**
* @Author: Alcwyn Parker <alcwynparker>
* @Date:   2017-04-28T21:06:23+01:00
* @Email:  alcwynparker@gmail.com
* @Project: Anemone
* @Filename: Anemone.cpp
* @Last modified by:   alcwynparker
* @Last modified time: 2017-05-10T00:00:10+01:00
*/

#include "Arduino.h"
#include "Anemone.h"

Anemone::Anemone(int pin)
{
  ssout = pin;
  // put your setup code here, to run once:
  pinMode(ssout, OUTPUT);


  for (int i = 0; i < BITNUMBER; i++){

  }

}

void Anemone::start(){

  clear();
  saveSwitchStates();


  delay(1000);
}

/**
 * void ledsOff - Loop thorugh the char array and set of the char
 * back to zero
 *
 */
void Anemone::clear(){

  // loop through all sendBytes and set the bits to on (OFF for the bus);
  for (int i=0; i < BYTENUMBER; i++)
  {
    sendBytes[i] = 255;
  }

  updateLEDs();
}


/**
 * void updateLEDa - Handles the SPI transaction. Sends each byte individually
 * and receives a byte each time (the switch state);
 *
 */
void Anemone::updateLEDs()
{
   // Start transaction
   SPI.beginTransaction(SPISettings(150000, MSBFIRST, SPI_MODE3));
   digitalWrite(ssout, LOW);

   // loop through and send each byte
   for (int i = 0; i < BYTENUMBER; i++){

     byte sendByte = sendBytes[i];
     // flip bits
     sendByte = ((sendByte >> 1) & 0x55) | ((sendByte << 1) & 0xaa);
     sendByte = ((sendByte >> 2) & 0x33) | ((sendByte << 2) & 0xcc);
     sendByte = ((sendByte >> 4) & 0x0f) | ((sendByte << 4) & 0xf0);

     returnBytes[i] = SPI.transfer(sendByte);

   }
   // end transaction
   SPI.endTransaction();
   digitalWrite(ssout, HIGH);

   processReturnBytes();
}


/**
 * void Anemone - reverses the returnBytes array so that it is in the
 * right order / flips each byte at the same time!
 *
 * FOUND: http://forum.arduino.cc/index.php?topic=70463.0
 */
void Anemone::processReturnBytes(){
  for (int i=0, j = BYTENUMBER-1; i< BYTENUMBER/2; i++, j--)
  {
    byte iByte = returnBytes[i];
    byte jByte = returnBytes[j];

    /*
    // flip bits
    iByte = ((iByte >> 1) & 0x55) | ((iByte << 1) & 0xaa);
    iByte = ((iByte >> 2) & 0x33) | ((iByte << 2) & 0xcc);
    iByte = ((iByte >> 4) & 0x0f) | ((iByte << 4) & 0xf0);

    jByte = ((jByte >> 1) & 0x55) | ((jByte << 1) & 0xaa);
    jByte = ((jByte >> 2) & 0x33) | ((jByte << 2) & 0xcc);
    jByte = ((jByte >> 4) & 0x0f) | ((jByte << 4) & 0xf0);
    */

    returnBytes[i] = jByte;
    returnBytes[j] = iByte;

  }
}



/**
 * void saveSwitchStates - save this frames switch states for the debounce
 * process in the next frame
 *
 */
void Anemone::saveSwitchStates(){

  // Save returnBytes to prevReturnBytes for debounce processing
  for (int i = 0; i < BYTENUMBER; i++){

    prevReturnBytes[i] = returnBytes[i];

  }
}

/**
 * void ledWrite - alter a single LED in the char array
 *
 * @param  {int} index of the LED to change
 * @param  {int} the state to change |  0 on / 1 off
 *
 */
void Anemone::ledWrite(int index, int state){
  // invert the index to allow for right to left declaration
  //index = BITNUMBER - index - 1;

  int nodeByte = index / 8;     // which byte
  int nodeBit = index % 8;     // which bit

  // update char to bit
  bitWrite(sendBytes[nodeByte], nodeBit, state);
}



/**
 * ledRead - returns true if the led is on or false if it is off
 *
 * @param  {int} index of the led
 * @return {bool}       state of the led
 */
bool Anemone::ledRead(int index){
  int nodeByte = index / 8;     // which byte
  int nodeBit = index % 8;     // which bit

  // update char to bit
  int val = bitRead(sendBytes[nodeByte], nodeBit);

  if (val == 0){
    return true;
  }else{
    return false;
  }
}


/**
 * void checkActiveSwitches - loops through and compares the returnBytes to the
 * prevBytes and updates the activeSwitches byte array.
 * TODO: Eventually this should be done with a bit operator to speed things up
 *
 * processBytes must be called prior to returnActiveSwitches
 * @return {type}  description
 */
void Anemone::checkActiveSwitches(){

  for (int i = 0; i < BITNUMBER; i++){

    int nodeByte = i / 8;     // which byte
    int nodeBit = i % 8;     // which bit

    int prevBit = bitRead(prevReturnBytes[nodeByte], nodeBit);
    int currentBit = bitRead(returnBytes[nodeByte], nodeBit);

    if (prevBit != currentBit){

      bitWrite(activeSwitches[nodeByte], nodeBit, 0);
    }else{
      bitWrite(activeSwitches[nodeByte], nodeBit, 1);
    }
  }
}


/**
 * bool checkSwitch - returns true or false depending on whether the switch Handles
 * been toggled
 *
 * @param  {type} int index of the switch
 * @return {bool}           a flag for if the switch has been toggled
 */
bool Anemone::checkSwitch(int index){

  int nodeByte = index / 8;     // which byte
  int nodeBit = index % 8;     // which bit

  int switchToggleState = bitRead(activeSwitches[nodeByte], nodeBit);

  if (switchToggleState == 0){
    return true;
  }else{
    return false;
  }
}



/**
 * void toggleLED - simply toggles an LEDs bit
 *
 * TODO: Replace this operation with a bit operator
 *
 * @param  {int} index of the LED
 */
void Anemone::toggleLED(int index){

  int nodeByte = index / 8;     // which byte
  int nodeBit = index % 8;     // which bit

  int ledState = bitRead(sendBytes[nodeByte], nodeBit);

  if (ledState == 1){
    bitWrite(sendBytes[nodeByte], nodeBit, 0);
  }else{
    bitWrite(sendBytes[nodeByte], nodeBit, 1);
  }
}



/**
 * getSwitchTimer - returns the timer from a specific switchTimers
 * used to implement the debounce timer.
 *
 * @param  {int}
 * @return {type}           timer value - counting down to activate
 */
uint8_t Anemone::getSwitchTimer(int index){
  return switchTimers[index];
}


/**
 * setSwitchTimer - sets the switch timer
 * used to implement the debounce timer.
 * @param  {int}  index of the switch
 * @param  {type} value to start the timer at
 */
void Anemone::setSwitchTimer(int index, int val){
  switchTimers[index] = val;
}


/**
 * stepSwitchTimer - counts the timer down a step
 * used to implement the debounce timer.
 * @param  {int}  index of the switch
 */
void Anemone::stepSwitchTimer(int index){
  switchTimers[index]--;
}


/**
 * overrideActiveSwitch - used for debounce to stop misreadings
 *
 * @param  {int} index of switch
 * @param  {int} value to set the active switch to
 *
 */
void Anemone::overrideActiveSwitch(int index, int val){

  int nodeByte = index / 8;     // which byte
  int nodeBit = index % 8;     // which bit

  bitWrite(activeSwitches[nodeByte], nodeBit, val);
}



/**
 * void clearDebounce - reset the debounce from the next frame
 *
 *
 */
void Anemone::clearDebounce(){
  for (int i = 0; i < BYTENUMBER; i++){

    debounce[i] = 255;

  }
}


/**
 * node - This is the public facing function that returns the result of the debounce
 * for interaction with the switches
 *
 * @param  {int} index of the switch
 * @return {bool}        is the switch on or off
 */
bool Anemone::node(int index){

  int switchByte = index / 8;     // which byte
  int switchBit = index % 8;     // which bit

  if (bitRead(debounce[switchByte], switchBit) == 0){
    return true;
  }else{
    return false;
  }
}


/**
 * void update - this is called everyframe to update everything
 *
 */
void Anemone::update(){

  clearDebounce();

  updateLEDs();

  checkActiveSwitches();

  // debounce process (BYTES 1 OFF / 0 ON)
  for (int i = 0; i < BITNUMBER; i++){

    // Find position in the byte array
    int switchByte = i / 8;     // which byte
    int switchBit = i % 8;     // which bit

    // Get debounce specific values
    bool  stateChange = checkSwitch(i);
    int   timerVal =    getSwitchTimer(i);

    // check for cancellation through noise
    if (stateChange == true && timerVal > 0) {
      setSwitchTimer(i, 0);
      overrideActiveSwitch(i, 1);
      //Serial.println("RESET");
    }


    if(stateChange == true && timerVal == 0){
      setSwitchTimer(i, 9);
      //Serial.println("START");
    }

    // count down to trigger
    if (timerVal > 1 && stateChange == false) {
      stepSwitchTimer(i);
      //Serial.println("COUNT DOWN");
    }

    // Trigger Action
    if (timerVal == 1) {

      setSwitchTimer(i, 0);
      bitWrite(debounce[switchByte], switchBit, 0);
      //Serial.println("ACTIVATE");
    }

  }

  // save for next time
  saveSwitchStates();
}


/**
 * getRingStart - Returns the first index of a node in a ring
 *
 * @param  {int} the number of the ring (bottom=0 top =8)
 * @return {int}         indec of first node
 */
int Anemone::getRingStart(int ring){
  return ringStarts[ring];
}


/**
 * getRingEnd - Returns the last index of a node in a ring
 *
 * @param  {int} the number of the ring (bottom=0 top =8)
 * @return {int}         index of last node
 */
int Anemone::getRingEnd(int ring){
  return ringEnds[ring];
}



/**
 * void lightRing - set all the LEDs in a ring to be on when the next update
 * is called
 *
 * @param  {int} index of the ring
 */
void Anemone::lightRing(int ring){

  int start = getRingStart(ring);
  int end = getRingEnd(ring);

  for (int i = start; i <= end; i++){
    ledWrite(i, 0);
  }

}

/**
 * void clearRing - set all the LEDs in a ring to be off when the next update
 * is called
 *
 * @param  {int} index of the ring
 */
void Anemone::clearRing(int ring){

  int start = getRingStart(ring);
  int end = getRingEnd(ring);

  for (int i = start; i <= end; i++){
    ledWrite(i, 1);
  }

}

/**
 * void getRingNodeCount - returns the number of leds in the ring
 *
 * @param  {int} number of leds
 */
int Anemone::getRingNodeCount(int ring){

  return ringNodeCount[ring];
}



void Anemone::debugArray(byte *a) {


  for (int i = 0; i < BITNUMBER; i++) {

    int switchByte = i / 8;     // which byte
    int switchBit = i % 8;     // which bit

    int val = bitRead(a[switchByte], switchBit);
    Serial.print(val);
    if (switchBit == 7) Serial.print("-");
  }

  Serial.println("");
}

void Anemone::debugByte(byte b) {


  for (int i = 0; i < 8; i++) {

    int val = bitRead(b, i);
    Serial.print(val);
  }

}
