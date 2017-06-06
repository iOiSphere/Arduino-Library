// TWO CHANNEL SEQUENCER

unsigned long sequencerLastActive = millis();
int sequencerResetDelay = 30000;
bool sequencerActive = false;


void sequencerInput() {

  // if active check to see if we can activate this puppy!
  if (sequencerActive == false) {

    int i;
    // loop through all the switches first sequencer

    for (i = anemone.getRingStart(1); i <= anemone.getRingEnd(0); i++) {
      // if one has been toggled
      if (anemone.node(i) == true) {
        anemone.toggleLED(i);
        activateSequencer();
        return;
      }
    }


  } else {

    // Sequencer channel 1
    for (int s1 = anemone.getRingStart(0); s1 <= anemone.getRingEnd(0); s1++) {
      if (anemone.node(s1) == true) {
        anemone.toggleLED(s1);
        sequencerLastActive = millis();
        sequencerActive = true;
      }
    }

    // Sequencer channel 2
    for (int s2 = anemone.getRingStart(1); s2 <= anemone.getRingEnd(1); s2++) {
      if (anemone.node(s2) == true) {
        anemone.toggleLED(s2);
        sequencerLastActive = millis();
        sequencerActive = true;
      }
    }
  }
}



void stepSequencer() {


  // clear previous position if it was not already on
  if (playHead0PrevState == false) anemone.ledWrite(playHead0, LEDLOW);
  if (playHead1PrevState == false) anemone.ledWrite(playHead1, LEDLOW);


  // move play head along
  playHead0++;
  playHead1++;

  // RESET PLAYHEAD 0
  if (playHead0 > anemone.getRingEnd(0)) {
    playHead0 = anemone.getRingStart(0);
    playHead0Sound = random(40, 51);
  }

  // RESET PLAYHEAD 1
  if (playHead1 > anemone.getRingEnd(1)) {
    playHead1 = anemone.getRingStart(1);
    playHead1Sound = random(50, 61);
  }

  // PLAYHEAD 0: check if the play head has landed on a active node
  if (anemone.ledRead(playHead0) == true) {
    //Serial.print("trigger 0: ");
    //Serial.println(playHead0Sound);
    sendI2C(playHead0Sound);
    playHead0PrevState = true;
  } else {
    playHead0PrevState = false;
  }

  // PLAYHEAD 1: check if the play head has landed on a active node
  if (anemone.ledRead(playHead1) == true) {
    //Serial.print("trigger 1: ");
    //Serial.println(playHead1Sound);
    sendI2C(playHead1Sound);
    playHead1PrevState = true;
  } else {
    playHead1PrevState = false;
  }


  anemone.ledWrite(playHead0, LEDHIGH);
  anemone.ledWrite(playHead1, LEDHIGH);
  anemone.updateLEDs();

}

void activateSequencer() {

  Serial.println("SEQUENCER ACTIVE");       // activate the chase game

  anemone.ledWrite(playHead0, LEDHIGH);
  anemone.ledWrite(playHead1, LEDHIGH);

  sequencerLastActive = millis();           // reset millis to now.
  sequencerActive = true;

}

void deactivateSequencer() {
  //Serial.println("SEQUENCER DEACTIVATED");       // activate the chase game
  sequencerActive = false;
  anemone.clearRing(0);
  anemone.clearRing(1);

}


void checkSequencerReset() {

  if (sequencerActive == true) {
    if (millis() - sequencerLastActive > sequencerResetDelay) {
      deactivateSequencer();
    }
  }
}
