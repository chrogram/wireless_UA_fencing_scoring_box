/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define RF24_SPI_SPEED 5000000


RF24 radio(1, 7); // CE, CSN

const byte address[6] = "00001";

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
  const char text[] = "Hello World";
  bool report = radio.write(&text, sizeof(text));
  
  if (report) {
    Serial.println(millis());
  }

//  radio.write(&text, sizeof(text));
//  Serial.println(millis());
}
