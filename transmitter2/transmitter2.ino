#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
//RF24 radio(0, 1); //xiao config

const byte address[6] = "00002";

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.stopListening();
}

void loop() {
//  const char text[] = "Hello World";
//  radio.write(&text, sizeof(text));
  int data = 10;
  radio.write(&data, sizeof(data));
   Serial.println(millis());
   Serial.println("Transmitter 2 on 00002");
}
