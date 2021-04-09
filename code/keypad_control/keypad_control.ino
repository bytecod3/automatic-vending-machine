/*
 * Control code for 3X4 membrane code
 */

#include <Keypad.h>

const byte ROWS = 4; // 4 rows
const byte COLS = 3; // 3 columns

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {9,8,7,6}; 
byte colPins[COLS] = {5,4,3};

// String to hold the numbers entered
String amount_entered = "";

// create an object of keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey(); //read the key

  // print which key pressed
  if(key){
    Serial.print(key);
  }

  // if the key pressed is a number, create a string containing the numbers pressed
  while((key != '*') && (key != '#')){
    
    // append the key pressed to the amount variable
    amount_entered += key; 
    
  }

  // number combination validation
  // check if it starts with a zero
  

  /*
   * issues to check
   * Long pressing for delete/backspace
   * If the number starts with a zero
   * pressing # as an enter key
   */
  
  
}
