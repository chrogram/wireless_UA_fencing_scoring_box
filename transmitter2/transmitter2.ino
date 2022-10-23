#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#define R24_SPI_SPEED 5000000

RF24 radio(0, 1); //CE, CSN
RF24Network network(radio);


const uint16_t this_node = 02;
const uint16_t node00 = 00;

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  network.begin(90, this_node);
  Serial.println("Transmitter 2 on 00002");

}

int data[3] = {2, 12001, 13001}; //ID weapon lame

void loop() {
  
  network.update();
  RF24NetworkHeader header8(node00);
  
  bool status = network.write(header8, &data, sizeof(data));

  data[1]++;
  data[2]++;
    
  if (status) {
    Serial.println(millis());
  }
}
