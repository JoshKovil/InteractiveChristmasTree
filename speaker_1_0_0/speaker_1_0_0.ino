#include <SD.h>  
#include <TMRpcm.h>
#include <SPI.h>

#define SD_ChipSelectPin 4 
TMRpcm tmrpcm;

#define branchOneSensor     2
#define branchTwoSensor     3
#define branchThreeSensor   4
#define branchFourSensor    6
#define branchFiveSensor    7
#define branchSixSensor     8
#define branchSevenSensor   9
#define branchEightSensor   10

const int numSensors = 8;
int sensor[numSensors] = {
  branchOneSensor, branchTwoSensor, branchThreeSensor, 
  branchFourSensor, branchFiveSensor, branchSixSensor,
  branchSevenSensor, branchEightSensor
}; 

bool sensorState[numSensors];
int currentLightColours[numSensors];

int index;

void setup() {

    Serial.begin(9600);

    tmrpcm.speakerPin = 9;
    tmrpcm.setVolume(6);
    if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
        Serial.println("SD failure");  
        return;
    } else {
        Serial.println("SD initialized"); 
    }

    tmrpcm.play("finale.wav");
    

    for (index = 0; index < numSensors; index ++){

        pinMode(sensor[index], INPUT);
        sensorState[index] = false;
        delay(100);

        Serial.print("Pin ");
        Serial.print(sensor[index]);
        Serial.print(": ");
        Serial.println(sensorState[index]);
        delay(1000);
      
    }

}

void loop() {
    for (index = 0; index < numSensors; index ++ ){
        bool previousState = sensorState[index];
        sensorState[index] = digitalRead(sensorState[index]);

        Serial.print("Pin ");                                               
        Serial.print(sensor[index]);                
        Serial.print(": ");                             
        Serial.println(sensorState[index]);
        delay(100);

        if (sensorState[index] != previousState){
            if (sensorState[index]){
                playMusic(index);
                currentLightColours[index]++;
                delay(100);

            }
        }
    }
    
    if (notZero(currentLightColours)){
    tmrpcm.play("finale.wav");
  }

}

void playMusic(int s){
    switch(s){
        case 0: tmrpcm.play("tone1.wav"); break;
        case 1: tmrpcm.play("tone2.wav"); break;
        case 2: tmrpcm.play("tone3.wav"); break; 
        case 3: tmrpcm.play("tone4.wav"); break;
        case 4: tmrpcm.play("tone1.wav"); break;
        case 5: tmrpcm.play("tone2.wav"); break;
        case 6: tmrpcm.play("tone3.wav"); break;
        case 7: tmrpcm.play("tone4.wav"); break;
    }
}

boolean notZero(int arr[]){
  int sum = 1;
  for (int i = 0; i < numSensors; i ++){
    sum = sum * arr[i];
  }
  
  if (sum == 0){
    return false;
  }
  return true;
}
