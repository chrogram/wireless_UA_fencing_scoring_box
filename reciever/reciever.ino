#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#define RF24_SPI_SPEED 5000000

//RF24 radio(7, 8); // CE, CSN
RF24 radio(0, 1); //xiao config

const byte address1[6] = "00001";
const byte address2[6] = "00002";

RF24Network network(radio);

const uint16_t this_node = 00;

void setup()
{

  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(1, address1);
  radio.openReadingPipe(2, address2);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
//  radio.setChannel(108);
  network.begin(90, this_node);
  
  
  radio.startListening();
  Serial.println("Begin");
}

int weapon1 = -1;
int lame1 = -1;
int weapon2 = -1;
int lame2 = -1;

void loop(){
  
  network.update();
  //default weapon values



  for(int i = 0; i < 4; i++){
    if(network.available()){
      RF24NetworkHeader header;
      int data[32] = {-1};
      network.read(header, &data, sizeof(data));
//      Serial.print("Packet ID: ");
//      Serial.println(data[0]);
//      Serial.println(data[1]);
//      Serial.println(data[2]);
//      Serial.println(millis());
      if(data[0] == 1){
//        Serial.println("packet 1");
        weapon1 = data[1];
        lame1 = data[2];
      }
    
      else if(data[0] == 2){
//        Serial.println("packet 2");
        weapon2 = data[1];
        lame2 = data[2];
      }
    }
  }

//
  Serial.print("timestamp: ");
  Serial.println(millis());
  
  Serial.print("weapon1: ");
  Serial.println(weapon1);
  
  Serial.print("lame1: ");
  Serial.println(lame1);

  Serial.print("weapon2: ");
  Serial.println(weapon2);

  Serial.print("lame2: ");
  Serial.println(lame2);

//  weapon1 = -1;
// lame1 = -1;
// weapon2 = -1;
// lame2 = -1;

  

}
