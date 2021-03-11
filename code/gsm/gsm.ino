#include <SoftwareSerial.h>

// create software serial object on pin 3 and 2 to communicate with sim800l
SoftwareSerial mySimCard(3, 2); //3 = Tx , 2 = Rx

void setup() {
  Serial.begin(9600);
  
  mySimCard.begin(9600); // 9600 bps

  Serial.println("---Initializing---");
  delay(1000);

  mySimCard.println("AT"); // once the handshake is successful, it will be back to OK
  updateSerial();
  mySimCard.println("AT+CSQ"); // signal quality test, value range 0-31, 31 the best
  updateSerial();
  mySimCard.println("AT+CCID"); // read simcard to confirm whether the SIM is plugged
  updateSerial();
  mySimCard.println("AT+CREG?"); // check whether it has registered in the network
  updateSerial();


}

void loop() {
  updateSerial();

}

void updateSerial(){
  delay(500);
  while (Serial.available()){
    mySimCard.write(Serial.read()); // forward what serial received to software serial port 
  }

  while(mySimCard.available()){
    Serial.write(mySimCard.read()); // forward what software serial received to serial port
    
  }
}
