
int pinLed = 7;
int pinSwitch = 9;

// hold button states
boolean oldSwitchState = LOW;
boolean newSwitchState = LOW;
boolean ledStatus = LOW;

// interrupt pin  controls
volatile int output = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);
  pinMode(pinSwitch, INPUT);
  
}

void loop() {
  newSwitchState = digitalRead(pinSwitch);

  if(newSwitchState != oldSwitchState){
    // has the button been pressed
    if(newSwitchState ==HIGH){
      if(ledStatus == LOW){
        digitalWrite(pinLed, HIGH);
        ledStatus = HIGH;
      }else{
        digitalWrite(pinLed, LOW);
        ledStatus = LOW;
      }
    }
    oldSwitchState = newSwitchState;
  }
  
}
