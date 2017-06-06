// BASIC GAME OF PONG

// command to trigger the traffic light
String trafficLight = "python /mnt/sda1/arduino/www/http.py 9";

void pongInput() {
  // pong interaction
  if (anemone.node(pongPos) == true) {
    pongDir = pongDir * -1;     // change dir
    sendI2C(4);                 // play sound
    pongCount++;                // progress

    // check to see if pong is conplete
    if (pongCount == 30) {

      pongCount = 0;
      sendI2C(5);
      Serial.println("TRAFFIC");
      callPython(trafficLight);

    }
  }
}


void pongStep() {

  // clear the last LED
  anemone.ledWrite(pongPos, LEDLOW);

  // Step the pong
  pongPos += pongDir;

  // check boundaries
  if ( pongPos > pongEnd) pongPos = pongStart;
  if ( pongPos < pongStart) pongPos = pongEnd;

  // turn new LED on
  anemone.ledWrite(pongPos, LEDHIGH);


}

