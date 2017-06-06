// SOUND BOARD - Ring 3
const int soundboardTimersLength = 31;
int soundboardTimers[soundboardTimersLength];
int soundboardTriggerDelay = 200;
int soundboardMaxActive = 5;


// only reliant on user input and not the tick
// play sound when user toggles switch
void soundboardInput() {

  // find out how many are active
  int activeCount = getTotalActive();

  // if less than max allowance then start processing input
  if (activeCount < soundboardMaxActive){

    // loop through all soundboard switches
    for (int i = soundboardStart; i <= soundboardEnd; i++) {
      
      if (anemone.node(i) == true) {      // if switch active

        int arrayPos = i - anemone.getRingStart(2);    // position in array
  
        if (soundboardTimers[arrayPos] == 0){
          anemone.ledWrite(i, LEDHIGH);         // activate led
          soundboardTimers[arrayPos] = soundboardTriggerDelay;    // start timer
  
          // play sound
          
          int track = 11 + (i - anemone.getRingStart(2)) ;
          
        
          Serial.print("SOUND-");
          Serial.println(track);
          sendI2C(track);
        }
      }
    }
  }
}

void soundboardTimerTick(){

  for (int i = 0; i < soundboardTimersLength; i ++){
    // increment through all active timers
    if (soundboardTimers[i] > 0) soundboardTimers[i]--;

    // turn off led at 1 
    if (soundboardTimers[i] == 1){
      int pos = anemone.getRingStart(2) + i;
      anemone.ledWrite(pos, LEDLOW);
    }

    // final resting place 0
  }
}

int getTotalActive(){
  int activeCount = 0; 
  for (int i = 0; i < soundboardTimersLength; i++){
    if (soundboardTimers[i] > 1) activeCount++;    
  }
  return activeCount;
}

