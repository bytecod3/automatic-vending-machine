#include "defines.h"
#include "volumeAnalysis.h"
#include "switch-state.h"

void setup() {
  Serial.begin(9600);

  // switch pins and LEDs
  pinMode(STATUS_LED, OUTPUT);
  // digitalWrite(STATUS_LED, LOW); // dispensing LED indicator is initially low
  pinMode(SWITCH_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(COIN_ACCEPTOR_PIN), readCoin(), FALLING); // read the coin value every time a coin is inserted
  attachInterrupt(digitalPinToInterrupt(FLOW_METER), dispensedVolume(), RISING); // keep track of the total volume that has been dispensed
   
}

void loop() {

  // read the coin value
  
  newSwitchState = digitalRead(SWITCH_PIN);

  if(newSwitchState != oldSwitchState){
    // has the start been pressed?
    if(newSwitchState ==HIGH){
      if(STATUS_LED == LOW){
        digitalWrite(STATUS_LED, HIGH);
        ledStatus = HIGH;
      }else{
        digitalWrite(STATUS_LED, LOW);
        STATUS_LED = LOW;
      }
    }
    oldSwitchState = newSwitchState;
  }

  count =0; // reset the counter to start counting from 0 again
  interrupts(); // enable interrupts on the arduino 
  delay(1000); //wait 1 second
  noInterrupts(); // disable interrupts on the arduino
  
}

int readCoin(){
  /*
   * ISR
   * Read the value from the coin acceptor 
   * Return the value
   */

}
