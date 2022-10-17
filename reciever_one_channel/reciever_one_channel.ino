#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.startListening();
}

void loop() {
//  Serial.println(radio.available());
  if (radio.available()) {
//    Serial.println(count);
    
//    char text[32] = "";
//    radio.read(&text, sizeof(text));
      int data;
      radio.read(&data, sizeof(data));
      Serial.print(millis());
      Serial.print(" ");
      Serial.println(data);
  }
}
