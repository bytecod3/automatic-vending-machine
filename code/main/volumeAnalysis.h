/*
 * Written by Edwin Mwiti. March 2021
 * This code contains functions to be used to control the 
 * pump and the flow meter
 */

#ifndef VOLUMEANALYSIS
#define VOLUMEANALYSIS

double flowRate;
unsigned int volume;
volatile int count; // volatile to update correctly

int calculateVolume(int coinValue){
  /* This function calculates the volume to be dispensed based on
  * the value of the coin passed to it
  * return : volume in millitres
  */  

  // todo : callibrate the values of coins
  switch (coinValue){
    case 5:
      volume = 20;
      break;

    case 10:
      volume = 100;
      break;

    case 20:
      volume = 200;
      break;

    case 40:
      volume = 250;
      break;

    default:
      volume = 0;
  }

  return volume;
  
}

int dispensedVolume(){
  /*
   * ISR
   * Keep track of the total amount of volume that has been dispensed
   * Return currentVolume
   */

   count++;
   
}

#endif
