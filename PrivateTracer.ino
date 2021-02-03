#include <OctoWS2811.h>

#include "Camera.h"
#include "Rotation.h"
#include "KalmanFilter.h"
#include "CameraObjs.h"
#include "ObjectDeformer.h"
#include "Boot.h"
#include "Face.h"

const int ledsPerStrip = 306;
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);
long previousTime = micros();

Boot boot;
Face face;

const int MaxBrightness = 15;

Camera camFronTop = Camera(Vector3D(-45, 0, 180), Vector3D(90, -220, -500),  306, &pixelString, true, false);
Camera camRearTop = Camera(Vector3D(45, 0, 0),    Vector3D(90, 90, -500),    306, &pixelString, false, false);
Camera camFronBot = Camera(Vector3D(0, 0, 0),     Vector3D(-5, 0, -500),     306, &pixelString, true, false);
Camera camRearBot = Camera(Vector3D(0, 0, 180),   Vector3D(-20, -131, -500), 306, &pixelString, false, false);

void printRenderTime(){
  Serial.print("Frame rendered in ");
  float dif = ((float)(micros() - previousTime)) / 1000000.0f;
  Serial.print(dif, 10);
  Serial.println(" seconds.");
  
  previousTime = micros();
}

void updateLEDs(Scene* scene){
  camFronTop.Rasterize(scene, 1.0f, MaxBrightness);
  camRearTop.Rasterize(scene, 1.0f, MaxBrightness);
  camFronBot.Rasterize(scene, 1.0f, MaxBrightness);
  camRearBot.Rasterize(scene, 1.0f, MaxBrightness);
  
  for (int i = 0; i < 306; i++) {
    leds.setPixel(i,       camFronTop.GetPixels()[i].R, camFronTop.GetPixels()[i].G, camFronTop.GetPixels()[i].B);
    leds.setPixel(i + 306, camRearTop.GetPixels()[i].R, camRearTop.GetPixels()[i].G, camRearTop.GetPixels()[i].B);
    leds.setPixel(i + 612, camFronBot.GetPixels()[i].R, camFronBot.GetPixels()[i].G, camFronBot.GetPixels()[i].B);
    leds.setPixel(i + 918, camRearBot.GetPixels()[i].R, camRearBot.GetPixels()[i].G, camRearBot.GetPixels()[i].B);
  }
  
  leds.show();
  printRenderTime();
}

void bootAnimation(){
  for (float i = 0.0f; i < 1.0f; i += 1.0f / 1260.0f){
    boot.Update(i);
    boot.FadeIn(0.0125f);
    
    updateLEDs(boot.GetScene());
  }
}

void setup() {
  leds.begin();
  leds.show();

  pinMode(A3, INPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting...");
  delay(50);

  previousTime = micros();

  //bootAnimation();
}

void loop() {
  for (float i = 0.0f; i < 1.0f; i += 1.0f / 720.0f) {
    face.Update(i);
    face.FadeIn(0.0125f);

    updateLEDs(face.GetScene());
    Serial.print(i);
    Serial.print(" ");
  }
}
