#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#define RF24_SPI_SPEED 5000000


RF24 radio(0, 1); // CE, CSN
RF24Network network(radio);

const uint16_t this_node = 02;
const uint16_t node00 = 00;

//============
// Pin Setup
//============

const uint8_t groundPin = A4;    // Ground  - Analog
const uint8_t lamePin   = A5;    // Lame    - Analog (Epee return path)
const uint8_t weaponPin = A6;    // Weapon  - Analog


//=========================
// values of analog reads
//=========================
int weapon = 0;
int lame   = 0;
int ground = 0;

int data[3] = {2, -1, -1}; //ID weapon lame



//================
// Configuration
//================
void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  network.begin(90, this_node);  

   
}


//============
// Main Loop
//============
void loop() {
  while(1) {
  
  
    // read analog pins
    weapon = analogRead(weaponPin);
    lame   = analogRead(lamePin);
    
    network.update();
    RF24NetworkHeader header8(node00);

    data[1] = weapon;
    data[2] = lame;

    bool status = network.write(header8, &data, sizeof(data));
    
    if (status) {
      Serial.println(millis());
    }
  
        
  
   }
}
