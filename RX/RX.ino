#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24Network.h>
#define RF24_SPI_SPEED 5000000

#define DEBUG
#define BUZZERTIME 1000
#define LIGHTTIME 3000

// -----Radio Setup-------------
RF24 radio(1, 7); //CE, CSN

RF24Network network(radio);

const uint16_t this_node = 00;
//------------------------------

// analog reads
int weaponA = 0;
int lameA = 0;
int ground1 = 0;
int weaponB = 0;
int lameB = 0;
int ground2 = 0;

// depress and timeouts
long depressAtime = 0;
long depressBtime = 0;
bool lockedOut = false;

// depress and lockout
const long lockout[] = {300000, 45000, 120000};
const long depress[] = {14000, 2000, 1000};

// states
bool depressedA  = false;
bool depressedB  = false;
bool hitOnTargA  = false;
bool hitOffTargA = false;
bool hitOnTargB  = false;
bool hitOffTargB = false;

bool one = false;
bool two = false;

// Pin Setup
const uint8_t onTargetA = 2;
const uint8_t offTargetA = 3;
const uint8_t onTargetB = 4;
const uint8_t offTargetB = 5;

const uint8_t buzzerPin = 6;

void setup()
{

  Serial.begin(115200);

  pinMode(onTargetA, OUTPUT);
  pinMode(offTargetA, OUTPUT);
  pinMode(onTargetB, OUTPUT);
  pinMode(offTargetB, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

//  testLights();

  radio.begin();
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  network.begin(90, this_node);
  
  
  radio.startListening();
  
  Serial.println("Foil Scoring Box");
  Serial.println("================");

  resetValues();
  
}

void loop(){
  
  network.update();


  int data[32] = {0};
//  Serial.println("test");


  
  if(network.available()){
    
    RF24NetworkHeader header;
    network.read(header, &data, sizeof(data));

//    packetDebug(data);

    if(data[0] == 1){
//      Serial.println("packet 1");
      weaponA = data[1];
      lameA = data[2];
      one = true;
    }
  
    if(data[0] == 2){
//      Serial.println("packet 2");
      weaponB = data[1];
      lameB = data[2];
      two = true;
    }
  }



  if((one == true) && (two == true)){
    Serial.println("BOTH TX GOOD");
    printData();
    
    signalHits();
    foil();
    

    one = two = false;
    resetPackets();
  }

}



//===================
// Main foil method
//===================
void foil() {

   long now = micros();
   if (((hitOnTargA || hitOffTargA) && (depressAtime + lockout[0] < now)) || 
       ((hitOnTargB || hitOffTargB) && (depressBtime + lockout[0] < now))) {
      lockedOut = true;
   }
//      Serial.print("weaoon A:"); 
//      Serial.println(weaponA);
//      Serial.print("Lame B:"); 
//      Serial.println(lameB);
   // weapon A
   if (hitOnTargA == false && hitOffTargA == false) { // ignore if A has already hit
      // off target
        if (weaponA > 900 && lameB < 100){
         if (!depressedA) {
            depressAtime = micros();
            depressedA   = true;
         } else {
            if (depressAtime + depress[0] <= micros()) {
               hitOffTargA = true;
            }
            
         }
      } else {
      // on target
      
         if (400 < weaponA && weaponA < 600 && 400 < lameB && lameB < 600) {
            if (!depressedA) {
               depressAtime = micros();
               depressedA   = true;
            } else {
               if (depressAtime + depress[0] <= micros()) {
                  hitOnTargA = true;
               }
            }
         } else {
            // reset these values if the depress time is short.
            depressAtime = 0;
            depressedA   = 0;
         }
      }
   }

   // weapon B
   if (hitOnTargB == false && hitOffTargB == false) { // ignore if B has already hit
      // off target
      if (900 < weaponB && lameA < 100) {
         if (!depressedB) {
            depressBtime = micros();
            depressedB   = true;
         } else {
            if (depressBtime + depress[0] <= micros()) {
               hitOffTargB = true;
            }
         }
      } else {
      // on target
         if (400 < weaponB && weaponB < 600 && 400 < lameA && lameA < 600) {
            if (!depressedB) {
               depressBtime = micros();
               depressedB   = true;
            } else {
               if (depressBtime + depress[0] <= micros()) {
                  hitOnTargB = true;
               }
            }
         } else {
            // reset these values if the depress time is short.
            depressBtime = 0;
            depressedB   = 0;
         }
      }
   }
}

void printData() {
  
  Serial.println("-----------");
  Serial.print("timestamp: ");
  Serial.println(millis());
  
  Serial.print("weaponA: ");
  Serial.println(weaponA);

  Serial.print("lameA: ");
  Serial.println(lameA);

  Serial.print("weaponB: ");
  Serial.println(weaponB);

  Serial.print("lameB: ");
  Serial.println(lameB);
  Serial.println("-----------\n\n");
  
}

void resetPackets() {
  weaponA = 0;
  lameA = 0;
  weaponB = 0;
  lameB = 0;
}

void resetValues() {
  

  delay(BUZZERTIME);             // wait before turning off the buzzer
//  noTone(buzzerPin);
//  digitalWrite(buzzerPin,  LOW);
  tone(buzzerPin, 0, 1);
  delay(LIGHTTIME-BUZZERTIME);   // wait before turning off the lights
  digitalWrite(onTargetA,  LOW);
  digitalWrite(offTargetA, LOW);
  digitalWrite(offTargetB, LOW);
  digitalWrite(onTargetB,  LOW);


  lockedOut    = false;
  depressAtime = 0;
  depressedA   = false;
  depressBtime = 0;
  depressedB   = false;

  hitOnTargA  = false;
  hitOffTargA = false;
  hitOnTargB  = false;
  hitOffTargB = false;
}

void signalHits() {
   // non time critical, this is run after a hit has been detected
   if (lockedOut) {
      digitalWrite(onTargetA,  hitOnTargA);
      digitalWrite(offTargetA, hitOffTargA);
      digitalWrite(offTargetB, hitOffTargB);
      digitalWrite(onTargetB,  hitOnTargB);
//      digitalWrite(buzzerPin,  HIGH);
      tone(buzzerPin, 2349);


#ifdef DEBUG
      String serData = String("hitOnTargA  : ") + hitOnTargA  + "\n"
                            + "hitOffTargA : "  + hitOffTargA + "\n"
                            + "hitOffTargB : "  + hitOffTargB + "\n"
                            + "hitOnTargB  : "  + hitOnTargB  + "\n"
                            + "Locked Out  : "  + lockedOut   + "\n";
      Serial.println(serData);
#endif
      resetValues();
   }
}

void packetDebug(int data[]) {
  Serial.print("timestamp: ");
  Serial.println(millis());
  
  Serial.print("header: ");
  Serial.println(data[0]);
  
  Serial.print("weapon: ");
  Serial.println(data[1]);
  
  Serial.print("lame: ");
  Serial.println(data[2]);
}

void testLights() {
   digitalWrite(offTargetA, HIGH);
   digitalWrite(onTargetA,  HIGH);
   digitalWrite(offTargetB, HIGH);
   digitalWrite(onTargetB,  HIGH);
   delay(300);
   resetValues();
}
