#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Vector.h>
#include "defines.h"


// volume analysis variables
volatile int flowPulse; 
int calc;
int volume;
double volumePassed;
double volumeToSell;

// 3x4 membrane keypad variables
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {9,8,7,6}; 
byte colPins[COLS] = {5,4,3};
String amount_entered = ""; // String to hold the characters entered
// create an object of keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


// coin acceptor variables
boolean insert = false;
volatile int pulse = 0;

// vector variables for string validation
Vector<int> stringIndices;
int significantAmount;

// LCD Screen Variables. 16x2 LCD screen at I2C adress 0x27. (Texas Instruments)
LiquidCrystal_I2C LCDDisplay(0x27, 16, 2);
const byte glyph[8] ={ // custom glyph
  0b11111,
  0b10101,
  0b11111,
  0b11111,
  0b01110,
  0b01010,
  0b11011,
  0b00000
};

void setup() {
  Serial.begin(9600);

  // switch pins and LEDs
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW); // dispensing LED indicator is initially low
  pinMode(SWITCH_PIN, INPUT);

  pinMode(FLOW_METER, INPUT); // initialize digital pin 2 as an input
  attachInterrupt(digitalPinToInterrupt(FLOW_METER), rpm, RISING); // keep track of the total volume that has been dispensed
  
  attachInterrupt(digitalPinToInterrupt(COIN_ACCEPTOR_PIN), readCoin(), FALLING); // read the coin value every time a coin is inserted
  
  // initialize LCD
  LCDDisplay.init();
  LCDDisplay.clear();
  LCDDisplay.backlight();

  // custom glyph
  LCDDisplay.createChar(0, glyph);
  LCDDisplay.setCursor(7, 0);
  LCDDisplay.write(0);

  // welcome messsage
  lcd.setCursor(4, 1);
  lcd.print("WELCOME"); 
}

void loop() {

  //====================Flow meter===================
  flowPulse = 0;
  sei(); // enable 
  delay(1000); // wait 1 second
  cli(); // disable interrupts

  //notes
  // pulses per second x 2.25 ml per pulse = mls per second
  // mls per second x 60 seconds = mls per minute
  // ml per minute / 1000 = liters per minute
  
  volumePassed = (flowPulse * 2.25); // take counted pulses in the last second and multiply by 2.25
  volumePassed = volumePassed * 60; // convert seconds to minutes, giving you mls per minute
  volumePassed = volumePassed / 1000; // convert ml to liters, giving you liters per minute

  
  //=====================Read Coin=====================
  if(insert){
    insert = false;
    Serial.println("Coin detected"); 
    //todo: figure out coin value

    // send this value to the function to calculate the volume
    // assume value = val
    volumeToSell = calculateVolume(30)/1000;

    // display coin value entered
    LCDDisplay.clear();
    LCDDisplay.setCursor(0,0);
    LCDDisplay.print("Amount: ");
    //LCDDisplay.print(coinValue); // print the coin value
    
    
    // wait for buyer to press the start 
    while(checkStartButton == 0){
      // blink the orange LED at 200ms
      digitalWrite(STATUS_LED, HIGH);
      delay(200);
      digitalWrite(STATUS_LED, LOW);
      delay(200);
    }
      
    // otherwise run the pump and measure the volume passed through the flowmeter simultaneously
    runPump(volumeToSell, volumePassed);
    
  }else if(insert ==0){
    // if there is no coin, check input from the keypad
      char key = keypad.getKey(); //read the key
      
        // if the key pressed is a number, create a string containing the numbers pressed
        while((key != '*') && (key != '#')){
          
          // append the key pressed to the amount variable
          amount_entered += key;
        
        }
        
        
        // remove leading zeroes and call the calculateVolume Function.
        volumeToSell = calculateVolFromKeypad(removeLeadingZeros(amount_entered));

        // display coin value entered
        LCDDisplay.clear();
        LCDDisplay.setCursor(0,0);
        LCDDisplay.print("Amount: ");
        LCDDisplay.print(amount_entered); // print the coin value
        
        // wait for buyer to press the start. Todo: refactor this code into a function
        while(checkStartButton == 0){
          // blink the orange LED at 200ms
          digitalWrite(STATUS_LED, HIGH);
          delay(200);
          digitalWrite(STATUS_LED, LOW);
          delay(200);
        }
      
        // otherwise run the pump and measure the volume passed through the flowmeter simultaneously
        runPump(volumeToSell, volumePassed);  
        
        /*
         * todo:
         * Long pressing for delete/backspace
         * pressing # as an enter key
         */
        
        } else{
           // PROCESS MOBILE MONEY
        }
            
    }
    
    
int runPump(double targetVolume, double pumpedVolume){
  /*
   * params: targetVolume -> Volume to sell
   *          pumpedVolume -> volume that has passed through the flowmeter
   *          
   */

   // print the volume to sell
    LCDDisplay.clear();
    LCDDisplay.setCursor(0,0);
    LCDDisplay.print("Volume: "); 
    LCDDisplay.print(targetVolume);
    LCDDisplay.print(" L");
   
  // start pumping the oil
  while(pumpedVolume != targetVolume){
    digitalWrite(PUMP_SIGNAL, HIGH);

    // display running status
    LCDDisplay.setCursor(0,1);
    LCDDisplay.print("Running...");
    LCDDisplay.blink();
    
    // blink orange LED while oil is being pumped
    digitalWrite(STATUS_LED, HIGH);
    delay(300);
    digitalWrite(STATUS_LED, LOW);
    delay(300);  
  }

  // once done, set orange LED high and display message done on the screen
  digitalWrite(STATUS_LED, HIGH);

  LCDDisplay.createChar(0, glyph);
  LCDDisplay.setCursor(7, 0);
  LCDDisplay.write(0);
  
  LCDDisplay.setCursor(0,1);
  LCDDisplay.print("Done!");
  LCDDisplay.blink();
  
  
  // reset the volume passed through the flowmeter to 0 for the next round
  volumePassed = 0;
  
}
    
int checkStartButton(){
    // read switch pin
    
    if(digitalRead(SWITCH_PIN) == HIGH){
      return 1;
    } else{
      return 0;
    }

}

double calculateVolFromKeypad(int amountPressed){
  // this function calculates volume to dispense from the amt entered on the keypad
  // assumption: 500mls of oil cost 130/=, which means 1ml of oil costs 0.26/=
  
  return amountPressed/0.26;
}

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

int rpm(){
  /*
   * Keep track of the total amount of volume that has been dispensed
   * Return currentVolume
   */

   flowPulse++;
   
}

// coin acceptor interrupt
int readCoin(){
  /*
   * Read the value from the coin acceptor 
   * Return an impulse to arduino
   */

   pulse++;
   insert = true;
  
   

}

int removeLeadingZeros(String parameter){
  /*
  This function removes leading zeroes from the string parameter
  */
  for(int i=0; i < parameter.length();i++){
    if(parameter[i] != 0){
      stringIndices.push_back(i);
    }else{
      continue;
    }
  }
  
  significantAmount = parameter.substring(stringIndices[0]).toInt();
}
