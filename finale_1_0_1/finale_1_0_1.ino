#include <Adafruit_NeoPixel.h>
#include <time.h>

#define starSongPIN 22 //to get star and song going
#define noteDecisionPIN 24 //to update uno on which note to play
#define playNotePIN 26  //to get uno to play a note

#define branchOnePin 2  //branch 1
#define branchOneSensor 30
#define branchOnePixelNumber 53  //53
 
#define branchTwoPin 3  //branch 2
#define branchTwoSensor 33
#define branchTwoPixelNumber 57  //57

#define branchThreePin 4  //branch 3
#define branchThreeSensor 34
#define branchThreePixelNumber 59  //59

#define branchFourPin 5  //branch 4
#define branchFourSensor 37
#define branchFourPixelNumber 76  //76

#define branchFivePin 6   //branch 5
#define branchFiveSensor 38
#define branchFivePixelNumber 84  //84

#define branchSixPin 7   //branch 6
#define branchSixSensor 41
#define branchSixPixelNumber 94  //94

#define branchSevenPin 8   //branch 7
#define branchSevenSensor 42
#define branchSevenPixelNumber 107  //107

#define branchEightPin 9   //branch 8
#define branchEightSensor 45
#define branchEightPixelNumber 114  //114   finished declaring all the pins to be used for sensor, neopixel strips, and number of lights; can be edited here

int randomColor; //temporary variable to store a randomised colour integer
int check; //tracks how many branches have lit up, triggers finale state if 8 branches lit up
int state; //keeps track of current system state
int finaleState; //enables finale rainbow colour shift
int currentLightColours[] = {-1,-1,-1,-1,-1,-1,-1,-1}; //keep track of the current colour of each branch; -1 is no colour
int branchPixelNumber[] = {branchOnePixelNumber, branchTwoPixelNumber, branchThreePixelNumber, branchFourPixelNumber, branchFivePixelNumber, branchSixPixelNumber, branchSevenPixelNumber, branchEightPixelNumber}; //number of lights for each branch in an array
int sensorPins[] = {branchOneSensor, branchTwoSensor, branchThreeSensor, branchFourSensor, branchFiveSensor, branchSixSensor, branchSevenSensor, branchEightSensor}; //sensor pin number for each branch

Adafruit_NeoPixel branchPins[] = { //the neopixel pin for each branch using array
  Adafruit_NeoPixel(branchOnePixelNumber, branchOnePin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(branchTwoPixelNumber, branchTwoPin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(branchThreePixelNumber, branchThreePin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(branchFourPixelNumber, branchFourPin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(branchFivePixelNumber, branchFivePin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(branchSixPixelNumber, branchSixPin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(branchSevenPixelNumber, branchSevenPin, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(branchEightPixelNumber, branchEightPin, NEO_GRB + NEO_KHZ800),
};
 
/* Setup() runs only once */
void setup() {
  Serial.begin(9600);
   state=1; //start state at 1; not finale mode. finale mode; state is 0
   check = 0; //start check at 0, no branches lit up
   finaleState = 0; //when finale state first entered, this variable will be 0
   for (int count = 0; count < 8; count++) { //get sensor pins and neopixel pins ready
      pinMode(sensorPins[count], INPUT);
      branchPins[count].begin();                              // All branches have been enabled
      branchPins[count].show();                               // Turn off all pixels ASAP
      branchPins[count].setBrightness(30);                  // Set Brightness to 50 percent = lower power consumption
   }

   pinMode(starSongPIN, OUTPUT);
   pinMode(noteDecisionPIN, OUTPUT);
   pinMode(playNotePIN, OUTPUT);

   digitalWrite(starSongPIN, LOW);
   digitalWrite(noteDecisionPIN, LOW);
   digitalWrite(playNotePIN, LOW);
   //randomSeed(analogRead(0)); //supposed to keep the lights a different set of randoms each time
}

/* loop() repeats forever */
void loop() {
  boolean sensorState = false; //variable to track which sensor has been pressed

  if (state) {  //if state is 1, before finale mode
    for (int count=0; count<8; count++) {
         sensorState = digitalRead(sensorPins[count]);  //check which branch pin/sensor is high/pressed
         Serial.print("Pin ");                                               
         Serial.print(sensorPins[count]);                
         Serial.print(": ");                             
         Serial.println(sensorState);
         delay(100);
         if (sensorState) {
             if (currentLightColours[count] == -1){
                 digitalWrite(noteDecisionPIN, HIGH); //update uno on which note to play                 
                 Serial.print(noteDecisionPIN);
                 check++;
             }
             while(1) {
                 randomColor = randomColors(); //get x number of random colours
                 if (randomColor != currentLightColours[count]) {
                     currentLightColours[count] = randomColor;
                     break;
                 }
             }
             for(int count1 = 0; count1 < branchPixelNumber[count]; count1++) {
                 branchPins[count].setPixelColor(count1, randomColor);
                 branchPins[count].show(); 
                 delay(10);
             }
             
             digitalWrite(playNotePIN, HIGH);  //get uno to play a note
             delay(500);
             digitalWrite(noteDecisionPIN, LOW);
             digitalWrite(playNotePIN, LOW);
             /* write code here to play sound. Use the check variable to play a note according to the number of branches currently lit up. */
         }
    }
  }
  
  if (check == 8) { //set check integer to the number of lit up branches required to reach finale
    state = 0; //set to finale mode so non-finale mode will no longer run
    digitalWrite(noteDecisionPIN, LOW);
    digitalWrite(playNotePIN, LOW);
    digitalWrite(starSongPIN, HIGH);  //get star and song started
    finaleLightColour(1); finaleLightColour(2); finaleLightColour(3); finaleLightColour(4); finaleLightColour(5); finaleLightColour(6); finaleLightColour(7); finaleLightColour(8); //sets all pixels on all branches to the correct light state
    for(int count = 0; count < 8; count++) { //show the lights
      branchPins[count].show();
    }
    finaleState++; //increment finale state for the next colour shift
    if(finaleState == 7) {
      finaleState = 0;
    }
    delay(100); //time delay before rainbow shifts
  }
}

uint32_t randomColors(){
  uint32_t color;
  
  uint32_t magenta = branchPins[1].Color(255, 0, 255);
  uint32_t pink = branchPins[1].Color(255, 153, 255);
  uint32_t red = branchPins[1].Color(255, 102, 102);
  uint32_t green = branchPins[1].Color(0, 255, 0);
  uint32_t blue = branchPins[1].Color(0, 128, 255);
  
  uint32_t colours[5] = {magenta, pink, red, green, blue};
  color = colours[random(5)];
    
  return color;
}

void finaleLightColour(int branchNumber) { //corresponding array number is branchNumber-1. This method sets the light colours for a branch in the finale state
   switch(finaleState) {
      case 0: for(int count = 0; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //red
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,65,27));
              }
              for(int count = 1; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //orange
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,162,27));
              }
              for(int count = 2; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //yellow
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,248,27));
              }
              for(int count = 3; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //green
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,255,100));
              }
              for(int count = 4; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //blue
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,139,255));
              }
              for(int count = 5; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //indigo
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(120,62,255));
              }
              for(int count = 6; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //violet
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(238,32,255));
              } 
              break;
      case 1: for(int count = 1; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //red
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,65,27));
              }
              for(int count = 2; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //orange
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,162,27));
              }
              for(int count = 3; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //yellow
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,248,27));
              }
              for(int count = 4; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //green
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,255,100));
              }
              for(int count = 5; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //blue
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,139,255));
              }
              for(int count = 6; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //indigo
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(120,62,255));
              }
              for(int count = 0; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //violet
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(238,32,255));
              } 
              break;
      case 2: for(int count = 2; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //red
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,65,27));
              }
              for(int count = 3; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //orange
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,162,27));
              }
              for(int count = 4; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //yellow
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,248,27));
              }
              for(int count = 5; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //green
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,255,100));
              }
              for(int count = 6; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //blue
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,139,255));
              }
              for(int count = 0; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //indigo
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(120,62,255));
              }
              for(int count = 1; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //violet
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(238,32,255));
              } 
              break;
      case 3: for(int count = 3; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //red
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,65,27));
              }
              for(int count = 4; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //orange
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,162,27));
              }
              for(int count = 5; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //yellow
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,248,27));
              }
              for(int count = 6; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //green
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,255,100));
              }
              for(int count = 0; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //blue
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,139,255));
              }
              for(int count = 1; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //indigo
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(120,62,255));
              }
              for(int count = 2; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //violet
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(238,32,255));
              } 
              break;
      case 4: for(int count = 4; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //red
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,65,27));
              }
              for(int count = 5; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //orange
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,162,27));
              }
              for(int count = 6; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //yellow
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,248,27));
              }
              for(int count = 0; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //green
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,255,100));
              }
              for(int count = 1; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //blue
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,139,255));
              }
              for(int count = 2; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //indigo
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(120,62,255));
              }
              for(int count = 3; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //violet
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(238,32,255));
              } 
              break;
      case 5: for(int count = 5; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //red
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,65,27));
              }
              for(int count = 6; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //orange
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,162,27));
              }
              for(int count = 0; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //yellow
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,248,27));
              }
              for(int count = 1; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //green
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,255,100));
              }
              for(int count = 2; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //blue
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,139,255));
              }
              for(int count = 3; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //indigo
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(120,62,255));
              }
              for(int count = 4; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //violet
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(238,32,255));
              } 
              break;
      case 6: for(int count = 6; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //red
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,65,27));
              }
              for(int count = 0; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //orange
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,162,27));
              }
              for(int count = 1; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //yellow
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(255,248,27));
              }
              for(int count = 2; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //green
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,255,100));
              }
              for(int count = 3; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //blue
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(0,139,255));
              }
              for(int count = 4; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //indigo
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(120,62,255));
              }
              for(int count = 5; count<branchPixelNumber[branchNumber - 1]; count = count + 7) {   //violet
                branchPins[branchNumber - 1].setPixelColor(count, branchPins[branchNumber - 1].Color(238,32,255));
              } 
              break;
   }
}
