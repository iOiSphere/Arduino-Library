// THREE PLAYER PONG
unsigned long chaseLastActive = millis();
int chaseResetDelay = 10000;
bool chaseActive;

void chaseInput() {

  // if active check to see if we can activate this puppy!
  if (chaseActive == false) {

    // loop through all the switches on the rings
    for (int i = chaseStart; i <= chaseEnd; i++) {
      // if one has been toggled
      if (anemone.node(i) == true) {
        activateChase(i);
        return;
      }
    }
  } else {

    // keep active if any of the switches have been pressed
    for (int i = chaseStart; i <= chaseEnd; i++) {
      if (anemone.node(i) == true) {
        chaseLastActive = millis();
        chaseActive = true;
      }
    }

    // chase 1 interaction
    if (anemone.node(chase1) == true) {
      chaseCount++;
      anemone.ledWrite(chase1, LEDLOW);
      chase1 = newChasePos(chase1);
      playChaseClip();
      anemone.ledWrite(chase1, LEDHIGH);
    }

    // chase 1 interaction
    if (anemone.node(chase2) == true) {
      chaseCount++;
      anemone.ledWrite(chase2, LEDLOW);
      chase2 = newChasePos(chase2);
      playChaseClip();
      anemone.ledWrite(chase2, LEDHIGH);
    }

    // chase 2 interaction
    if (anemone.node(chase3) == true) {
      chaseCount++;
      anemone.ledWrite(chase3, LEDLOW);
      chase3 = newChasePos(chase3);
      playChaseClip();
      anemone.ledWrite(chase3, LEDHIGH);
    }
  }
}

void playChaseClip(){
//  Serial.println("play chase clip");
    // handle the chaseCount
    if (chaseCount == 30) {
      chaseCount = 0;
      sendI2C(10);
      Serial.println("chase complete!");
      chaseComplete();
    } else {
      int choice = random(6, 10);
      sendI2C(choice);
      Serial.println("playing chase sound clip " + String(choice) +", chase count: " + String(chaseCount));
    }
}

void checkChaseReset() {
  if (chaseActive) {
    if (millis() - chaseLastActive > chaseResetDelay) deactivateChase();
  }
}

void deactivateChase() {
  Serial.println("CHASE DEACTIVATED");       // activate the chase game

  anemone.ledWrite(chase1, LEDLOW);    // show leds
  anemone.ledWrite(chase2, LEDLOW);
  anemone.ledWrite(chase3, LEDLOW);

  chaseActive = false;
}

void activateChase(int i) {
  Serial.println("CHASE ACTIVE");       // activate the chase game

  chase1 = newChasePos(i);              // position chase1 near user
  anemone.ledWrite(chase1, LEDHIGH);    // show leds
  anemone.ledWrite(chase2, LEDHIGH);
  anemone.ledWrite(chase3, LEDHIGH);

  chaseLastActive = millis();           // reset millis to now.
  chaseActive = true;
}

int newChasePos(int pos) {


  // pick a new random place
  int newPos = random(pos - 3, pos + 3);

  // make sure it is not the same
  if (newPos == pos) newPos++;

  // check new place boundaries
  // LOWER
  if (newPos < chaseStart) {
    int offset = chaseStart - newPos;
    newPos = chaseEnd - offset;
  }
  // HIGH
  if (newPos > chaseEnd) {
    int offset = newPos - chaseEnd;
    newPos = chaseStart + offset;
  }

  return newPos;
}


void chaseComplete() {
  anemone.clear();

  int i;
  for (i = 0; i < 9; i++) anemone.ledWrite(line1[i], LEDHIGH);
  anemone.updateLEDs();
  delay(500);
  for (i = 0; i < 9; i++) anemone.ledWrite(line1[i], LEDLOW);
  for (i = 0; i < 9; i++) anemone.ledWrite(line2[i], LEDHIGH);
  anemone.updateLEDs();
  delay(500);
  for (i = 0; i < 9; i++) anemone.ledWrite(line1[i], LEDLOW);
  for (i = 0; i < 9; i++) anemone.ledWrite(line2[i], LEDHIGH);
  anemone.updateLEDs();
  delay(500);
  for (i = 0; i < 9; i++) anemone.ledWrite(line2[i], LEDLOW);
  for (i = 0; i < 9; i++) anemone.ledWrite(line3[i], LEDHIGH);
  anemone.updateLEDs();
  delay(500);
  for (i = 0; i < 9; i++) anemone.ledWrite(line3[i], LEDLOW);
  for (i = 0; i < 9; i++) anemone.ledWrite(line4[i], LEDHIGH);
  anemone.updateLEDs();
  delay(500);
  for (i = 0; i < 9; i++) anemone.ledWrite(line4[i], LEDLOW);
  for (i = 0; i < 9; i++) anemone.ledWrite(line1[i], LEDHIGH);
  delay(500);
  anemone.clear();
  anemone.ledWrite(chase1, LEDHIGH);
  anemone.ledWrite(chase2, LEDHIGH);
  anemone.ledWrite(chase3, LEDHIGH);
  anemone.updateLEDs();

  pattern0Pointer = 0;
  pattern1Pointer = 0;
}



