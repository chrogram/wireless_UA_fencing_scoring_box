#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#define RF24_SPI_SPEED 5000000


RF24 radio(0, 1); // CE, CSN
RF24Network network(radio);


const uint16_t this_node = 01;
const uint16_t node00 = 00;

void setup(){
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  network.begin(90, this_node);
  
  
  Serial.println("Transmitter 1 on 00001");
  
  
}

int data[3] = {1, 101, 201}; //ID weapon lame

void loop()
{
  network.update();
  RF24NetworkHeader header7(node00);  
  
  bool status = network.write(header7, &data, sizeof(data));

  data[1]++;
  data[2]++;
  
  if (status) {
    Serial.println(millis());
  }

}
