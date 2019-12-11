#include <Adafruit_NeoPixel.h>

const int numBranch = 3;
Adafruit_NeoPixel branch[numBranch] = {
  Adafruit_NeoPixel(4, 8, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(4, 9, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(4, 10, NEO_GRB + NEO_KHZ800)
};

const int numSensors = 3;
const int sensor[numSensors] = {2,3,4};

int index;
bool sensorState[numSensors];

uint32_t randColor;
int currentLightColors[numBranch];

void setup(){
  
  Serial.begin(9600);
  
  for (index = 0; index < numBranch; index ++){
    
    currentLightColors[index] = 0;
    
    branch[index].begin();
    branch[index].show();
    branch[index].setBrightness(100);
    
    Serial.print("branch ");
    Serial.print(index);
    Serial.println(" initialised");
    delay(1000);
  }
  
  Serial.println();
  
  for (index = 0; index < numSensors; index ++){
    pinMode(sensor[index], INPUT_PULLUP);
    sensorState[index] = false;
    
    Serial.print("Pin ");
    Serial.print(sensor[index]);
    Serial.print(": ");
    Serial.println(sensorState[index]);
    delay(1000);
  }
  
  Serial.println();
  
}

uint32_t randomColors(){
  
  uint32_t color;
  
  uint32_t magenta = branch[index].Color(255, 0, 255);
  uint32_t pink = branch[index].Color(255, 153, 255);
  uint32_t red = branch[index].Color(255, 102, 102);
  uint32_t green = branch[index].Color(0, 255, 0);
  uint32_t blue = branch[index].Color(0, 128, 255);
  
  uint32_t colours[5] = {magenta, pink, red, green, blue};
  color = colours[random(5)];
    
  return color; 
}

boolean notZero(int arr[]){
  int sum = 1;
  for (int i = 0; i < numBranch; i ++){
    sum = sum * arr[i];
  }
  
  if (sum == 0){
    return false;
  }
  return true;
}

void finale(int wait){
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256){
    for (int j =0; j < numBranch; j++){
      for(int i=0; i<branch[j].numPixels(); i++) {
        int pixelHue = firstPixelHue + (i * 65536L / branch[j].numPixels());
        branch[j].setPixelColor(i, branch[j].gamma32(branch[j].ColorHSV(pixelHue)));
      } 
      branch[j].show();
      delay(wait);
    }
  }
}

void loop(){
  
   for (index = 0; index < numSensors; index ++){
     bool previousState = sensorState[index];
     sensorState[index] = digitalRead(sensor[index]);

     Serial.print("Pin ");                                               
     Serial.print(sensor[index]);                
     Serial.print(": ");                             
     Serial.println(sensorState[index]);
     delay(1000);
     
     if (sensorState[index]!= previousState){
       if (sensorState[index]){
         
         randColor = randomColors();
         
         for (int j = 0; j < 4; j ++) {
           branch[index].setPixelColor(j, randColor);
           branch[index].show();
           delay(10);
         } 
         currentLightColors[index]++;
         delay(150);
         
         Serial.print("Current: [");
         for (int index = 0; index < numBranch; index ++){
           Serial.print(currentLightColors[index]);
           Serial.print(", ");
         }
         Serial.print("]");
         
         Serial.println();          
       }
     }
  }
  
  if (notZero(currentLightColors)){
    Serial.println("COME ON FINALE");
    finale(10);
  }
}
