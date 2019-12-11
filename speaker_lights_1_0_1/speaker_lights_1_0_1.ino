#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Adafruit_NeoPixel.h>

#define finalePIN 2        //activates star and song
#define noteDecisionPIN 7  //keeps track of which note to play before finale
#define playNotePIN 8      //plays a note before finale

int count;   //track which note to play before finale
bool finaleStart;  //no longer play notes once song is playing
bool current, previous, previous1, temp;

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

Adafruit_NeoPixel star = Adafruit_NeoPixel(40, 3, NEO_GRB + NEO_KHZ800);

void setup () {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  count = 0;
  finaleStart = false;
  current = false;
  previous = false;
  temp=true;

  pinMode(finalePIN, INPUT);
  pinMode(noteDecisionPIN, INPUT);
  pinMode(playNotePIN, INPUT);
  
  star.begin();
   for(int i=0; i<40; i++) {
    star.setPixelColor(i, star.Color(0,0,0));
  }
  star.show();

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

    if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  //myDFPlayer.play(1);  //Play the first mp3 
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(finalePIN) && temp) {
    Serial.println(digitalRead(finalePIN));
    temp=false;
    finaleStart = true;
    for(int i=0; i<40; i++){
       star.setPixelColor(i, star.Color(100,100,100));
    }
    star.show();
    myDFPlayer.play(1);  //play 0001.mp3 (finale music)
  }

  if (digitalRead(noteDecisionPIN) == LOW && previous) {
    previous = false;
  }

  if (digitalRead(noteDecisionPIN) && !previous) {
    count++;
    previous = true;
    Serial.print(count);
  }

if (digitalRead(playNotePIN) == LOW && previous1) {
  previous1 = false;
}

  if (digitalRead(playNotePIN) && !finaleStart && !previous1) {
    Serial.print("NotePlay");
    previous1 = true;
    switch(count) {
      case 1: myDFPlayer.play(2); break;
      case 2: myDFPlayer.play(3); break;
      case 3: myDFPlayer.play(4); break; 
      case 4: myDFPlayer.play(5); break;
      case 5: myDFPlayer.play(2); break;
      case 6: myDFPlayer.play(3); break;
      case 7: myDFPlayer.play(4); break;
      case 8: myDFPlayer.play(5); break;
    }
  }
}
