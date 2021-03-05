#include "volumeAnalysis.h"
#include "switch-state.h"

#define COIN_ACCEPTOR_PIN 3 // assign interrupt pin 3 to coin acceptor

void setup() {
  
  Serial.begin(9600);

  // switch pins and LEDs
  pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, LOW);// dispensing LED indicator is initially low
  pinMode(switchPin, INPUT);

  attachInterrupt(digitalPinToInterrupt(COIN_ACCEPTOR_PIN), readCoin(), FALLING); // read the coin value every time a coin is inserted
  attachInterrupt(digitalPinToInterrupt(FLOWPUMP), dispensedVolume(), RISING); // keep track of the total volume that has been dispensed
   
}

void loop() {

  // read the coin value
  
  newSwitchState = digitalRead(switchPin);

  if(newSwitchState != oldSwitchState){
    // has the start been pressed?
    if(newSwitchState ==HIGH){
      if(statusLed == LOW){
        digitalWrite(statusLed, HIGH);
        ledStatus = HIGH;
      }else{
        digitalWrite(statusLed, LOW);
        statusLed = LOW;
      }
    }
    oldSwitchState = newSwitchState;
  }
 
}

int readCoin(){
  /*
   * ISR
   * Read the value from the coin acceptor 
   * Return the value
   */

}

int dispensedVolume(){
  /*
   * ISR
   * Keep track of the total amount of volume that has been dispensed
   * Return currentVolume
   */
   
}
