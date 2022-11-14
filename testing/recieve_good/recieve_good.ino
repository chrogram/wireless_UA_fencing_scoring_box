#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define RF24_SPI_SPEED 5000000


RF24 radio(1, 7); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.startListening();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.print(millis());
    Serial.print(" ");
    Serial.println(text);
    if (text[0] == 'H'){
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
    }
    
  }
}
