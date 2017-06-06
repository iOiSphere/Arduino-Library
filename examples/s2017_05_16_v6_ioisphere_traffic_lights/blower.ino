void blower() {

  // if blower switch is not active but being triggered
  if (anemone.node(blowerSwitch) == true && blowerTimer == 0) {

    Serial.println("BLOWER ACTIVE");
    // set timer for count down
    blowerTimer = 200;

    // activate blower pin
    digitalWrite(blowerPin, HIGH);

  }

  // step down blower timer
  if (blowerTimer > 0) blowerTimer--;

  // turn blower off on timer.
  if (blowerTimer == 0) {
    digitalWrite(blowerPin, LOW);
  }
}

