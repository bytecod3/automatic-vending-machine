#include "defines.h"
#include "volumeAnalysis.h"
#include "switch.h"

// volume analysis variables
volatile int flowPulse; // measuring the rising edges of the signal
int calc;
double flowRate;
// using interrupt pin 2 for flow sensor

void setup() {
  Serial.begin(9600);

  // switch pins and LEDs
  pinMode(STATUS_LED, OUTPUT);
  // digitalWrite(STATUS_LED, LOW); // dispensing LED indicator is initially low
  pinMode(SWITCH_PIN, INPUT);

  pinMode(FLOW_METER, INPUT); // initialize digital pin 2 as an input
  attachInterrupt(digitalPinToInterrupt(FLOW_METER), rpm, RISING); // keep track of the total volume that has been dispensed
  
  //attachInterrupt(digitalPinToInterrupt(COIN_ACCEPTOR_PIN), readCoin(), FALLING); // read the coin value every time a coin is inserted
  
   
}

void loop() {

  reading = digitalRead(SWITCH_PIN); // read switch pin

  if(reading != switchState){
    // if switch state has changed
    if(reading == HIGH && switchState == LOW) ledState = !ledState;
    digitalWrite(STATUS_LED, ledState); // turn the LED on or off
    
    switchState = reading; // update switch state
  }

  flowPulse = 0;
  sei(); // enable 
  delay(1000); // wait 1 second
  cli(); // disable interrupts

  //notes
  // pulses per second x 2.25 ml per pulse = mls per second
  // mls per second x 60 seconds = mls per minute
  // ml per minute / 1000 = liters per minute
  
  flowRate = (flowPulse * 2.25); // take counted pulses in the last second and multiply by 2.25
  flowRate = flowRate * 60; // convert seconds to minutes, giving you mls per minute
  flowRate = flowRate / 1000; // convert ml to liters, giving you liters per minute

  Serial.print(flowRate);
  Serial.println(" L/Min");
  
}

//
//int calculateVolume(int coinValue){
//  /* This function calculates the volume to be dispensed based on
//  * the value of the coin passed to it
//  * return : volume in millitres
//  */  
//
//  // todo : callibrate the values of coins
//  switch (coinValue){
//    case 5:
//      volume = 20;
//      break;
//
//    case 10:
//      volume = 100;
//      break;
//
//    case 20:
//      volume = 200;
//      break;
//
//    case 40:
//      volume = 250;
//      break;
//
//    default:
//      volume = 0;
//  }
//
//   // return volume;
//  
//}

int rpm(){
  /*
   * ISR
   * Keep track of the total amount of volume that has been dispensed
   * Return currentVolume
   */

   flowPulse++;
   
}

int readCoin(){
  /*
   * ISR
   * Read the value from the coin acceptor 
   * Return the value
   */

}
