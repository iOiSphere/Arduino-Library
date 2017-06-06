String tweet = "python /mnt/sda1/arduino/www/status.py";

void launchStep() {
  launchCount++;;

  if (launchCount % 1000 == 0) {
    sphereArmed = true;
  }
}


void launch() {
  sphereArmed = false;
  //Process p; // Create a process and call it "p"
  // p.begin("python /mnt/sda1/arduino/www/status.py"); // Process that launch the "python" command
  //p.addParameter("/mnt/sda1/arduino/www/status.py"); // Add the path parameter
  //p.run(); // Run the process and wait for its termination

  sendI2C(1);       // play sound
  callPython(tweet);

  for (int m = 0; m < 3; m++) {
    for (int i = 9; i >= 0; i--) {

      anemone.clear();
      anemone.lightRing(i);
      anemone.updateLEDs();
      delay(100);
      anemone.clear();
      anemone.ledWrite(chase1, LEDHIGH);
      anemone.ledWrite(chase2, LEDHIGH);
      anemone.ledWrite(chase3, LEDHIGH);
      anemone.updateLEDs();

      pattern0Pointer = 0;
      pattern1Pointer = 0;

    }
  }
}




