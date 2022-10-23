#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#define RF24_SPI_SPEED 5000000

RF24 radio(0, 1); //CE, CSN

RF24Network network(radio);

const uint16_t this_node = 00;

void setup()
{

  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  network.begin(90, this_node);
  
  
  radio.startListening();
  Serial.println("Begin");
}

int weapon1 = -1;
int lame1 = -1;
int weapon2 = -1;
int lame2 = -1;

int oldWeapon1 = -1;
int oldLame1 = -1;
int oldWeapon2 = -1;
int oldLame2 = -1;
bool one = false;
bool two = false;

void loop(){
  
  network.update();


  int data[32] = {-1};

  oldWeapon1 = weapon1;
  oldLame1 = lame1;
  oldWeapon2 = weapon1;
  oldLame2 = lame2;
  

  
  if(network.available()){
    
    RF24NetworkHeader header;
    network.read(header, &data, sizeof(data));

//    Serial.print("timestamp: ");
//    Serial.println(millis());
//    
//    Serial.print("header: ");
//    Serial.println(data[0]);
//
//    Serial.print("weapon: ");
//    Serial.println(data[1]);
//
//    Serial.print("lame: ");
//    Serial.println(data[2]);

    if(data[0] == 1){
//      Serial.println("packet 1");
      weapon1 = data[1];
      lame1 = data[2];
      one = true;
    }
  
    if(data[0] == 2){
//      Serial.println("packet 2");
      weapon2 = data[1];
      lame2 = data[2];
      two = true;
    }
  }



  if((one == true) && (two == true)){
    Serial.println("BOTH TX GOOD");
    printData();
    //TODO implement foilbox
    one = two = false;
    resetValues();
  }

  

}

void printData() {
  
  Serial.println("-----------");
  Serial.print("timestamp: ");
  Serial.println(millis());
  
  Serial.print("weapon 1: ");
  Serial.println(weapon1);

  Serial.print("lame1: ");
  Serial.println(lame1);

  Serial.print("weapon2: ");
  Serial.println(weapon2);

  Serial.print("lame2: ");
  Serial.println(lame2);
  Serial.println("-----------\n\n");
  
}

void resetValues() {
  weapon1 = -1;
  lame1 = -1;
  weapon2 = -1;
  lame2 = -1;
}
