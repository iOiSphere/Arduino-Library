// PATTERN RECOGNITION AT THE TOP OF THE SPHERE


void patternInput() {

  for (int i = anemone.getRingStart(8); i <= anemone.getRingEnd(6); i++) {
    if (anemone.node(i) == true) {

      Serial.println(i);
      pattern0Step(i);
      pattern1Step(i);
      pattern2Step(i);
    }
  }
}



void pattern0Step(int i) {

  if (i !=  pattern0[pattern0Pointer]) {
    for (int c = 0; c < pattern0Length; c++) {
      anemone.ledWrite(pattern0[c], LEDLOW);
    }
    pattern0Pointer = 0;
    return;
  }

  pattern0Pointer++;
  Serial.println("STEP 0");
  sendI2C(2);
  anemone.ledWrite(i, LEDHIGH);


  if (pattern0Pointer == pattern0Length) {
    Serial.println("SEQUENCE 0: Complete");
    pattern0Pointer = 0;
    sendI2C(3);
    delay(10);
    sendI2C(150);
  }
}


void pattern1Step(int i) {


  if (i !=  pattern1[pattern1Pointer]) {
    for (int c = 0; c < pattern1Length; c++) {
      anemone.ledWrite(pattern1[c], LEDLOW);
    }
    pattern1Pointer = 0;
    return;
  }

  pattern1Pointer++;
  Serial.println("STEP 1");
  sendI2C(2);
  anemone.ledWrite(i, LEDHIGH);

  if (pattern1Pointer == pattern1Length) {
    Serial.println("SEQUENCE 1: Complete");
    pattern1Pointer = 0;
    sendI2C(3);
    delay(10);
    sendI2C(151);

  }
}


void pattern2Step(int i) {


  if (i !=  pattern2[pattern2Pointer]) {
    for (int c = 0; c < pattern2Length; c++) {
      anemone.ledWrite(pattern2[c], LEDLOW);
    }
    pattern2Pointer = 0;
    return;
  }

  pattern2Pointer++;
  Serial.println("STEP 2");
  sendI2C(2);
  anemone.ledWrite(i, LEDHIGH);

  if (pattern2Pointer == pattern2Length) {
    Serial.println("SEQUENCE 2: Complete");
    pattern2Pointer = 0;
    sendI2C(3);
    delay(10);
    sendI2C(152);

  }
}

