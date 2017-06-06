
void sendI2C(int num) {
  Wire.beginTransmission(4); // transmit to device #4

  Wire.write(num);           // sends one byte

  Wire.endTransmission();    // stop transmitting
}



void callPython(String command) {

  Process p; // Create a process and call it "p"
  p.runShellCommandAsynchronously(command);
  SerialUSB.flush();

  //p.begin("python"); // Process that launch the "python" command
  //p.addParameter("/mnt/sda1/arduino/www/http.py"); // Add the path parameter
  //p.addParameter(param);
  //p.run(); // Run the process and wait for its termination

}
