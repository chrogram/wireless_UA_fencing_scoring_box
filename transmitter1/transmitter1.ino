#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#define RF24_SPI_SPEED 5000000


// RF24 radio(7, 8); // CE, CSN
RF24 radio(0, 1); // xiao config
RF24Network network(radio);

const byte address[6] = "00001";

const uint16_t this_node = 01;
const uint16_t node00 = 00;

void setup(){
  Serial.begin(115200);
  radio.begin();
//  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
//  radio.setChannel(108);

  network.begin(90, this_node);
  
//  radio.stopListening();
  
  Serial.println("Transmitter 1 on 00001");
  
  
}

void loop()
{
  network.update();
  RF24NetworkHeader header7(node00);  
  int data[3] = {1, 101, 103}; //ID weapon lame
  bool status = network.write(header7, &data, sizeof(data));
  
  
  if (status) {
    Serial.println(millis());
  }

}
