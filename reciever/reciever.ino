#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address1[6] = "00001";
const byte address2[6] = "00002";
byte pipe1 = 0;
byte pipe2 = 1;

const uint8_t pipe1LED  =  2;    
const uint8_t pipe2LED  =  3;

void setup() {

  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address1);
  radio.openReadingPipe(1, address2);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  pinMode(pipe1LED, OUTPUT);
  pinMode(pipe2LED, OUTPUT);
  radio.startListening();
  
}

void loop() {
  if (radio.available(&pipe1)) {

    // digitalWrite(pipe1LED, HIGH);
    // delay(100);
    // digitalWrite(pipe1LED, LOW);

    int data1;
    radio.read(&data1, sizeof(data1));
      
    Serial.print("Pipe 0 - ");
    // Serial.print(millis());
    // Serial.print(" ");
    Serial.println(data1);
      
  }


 if(radio.available(&pipe2)) {

  int data2;
  radio.read(&data2, sizeof(data2));
  Serial.print(millis());
  Serial.print(" ");
  Serial.print("Pipe 1 - ");
  Serial.println(data2);
   
 }
}
