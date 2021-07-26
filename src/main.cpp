#include <Arduino.h>
#include <OctoWS2811.h>

#include "Render/Camera.h"
#include "Math/Rotation.h"
#include "Filter/KalmanFilter.h"
#include "Flash/CameraObjs.h"
#include "Render/ObjectDeformer.h"
#include "Animation/Boot.h"
#include "Morph/Face.h"
//#include "Sensors/MotionProcessor.h"
#include "Materials/BMP.h"
#include "Flash/BMPImages.h"
#include "Flash/CrashObjs.h"
#include "Flash/MiscObjs.h"
#include "Physics/PhysicsSimulator.h"

#include "Morph/ProtoDRMorphAnimator.h"

//MotionProcessor motionProcessor;

const int ledsPerStrip = 306;
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);
long previousTime = micros();

Boot boot;
ProtoDRMorphAnimator protoMorph;

BMP bootBMP = BMP(Vector2D(800, 3200), Vector2D(-200, -3200), bootImage, 2);
BMP crashBMP = BMP(Vector2D(400, 300), Vector2D(-200, 0), crashImage, 0);
BMP dedBMP = BMP(Vector2D(200, 200), Vector2D(20, 20), dedImage, 0);

const uint8_t MaxBrightness = 40;
long screensaverTime = 0;

Camera camFronTop = Camera(Vector3D(-45, 0, 180), Vector3D(90, -220, -500),  306, &primaryPixelString, true, false, false);
Camera camRearTop = Camera(Vector3D(45, 0, 0),    Vector3D(90, 90, -500),    306, &primaryPixelString, false, false, false);
Camera camFronBot = Camera(Vector3D(0, 0, 0),     Vector3D(-5, 0, -500),     306, &primaryPixelString, true, false, false);
Camera camRearBot = Camera(Vector3D(0, 0, 180),   Vector3D(-20, -131, -500), 306, &primaryPixelString, false, false, false);

//Right Face
//Camera camMiddTop = Camera(Vector3D(45, 0, 0),   Vector3D(300, 0, -500), 89, &secondaryPixelString, true, false, false);
//Camera camMiddBot = Camera(Vector3D(-135, 0, 0),  Vector3D(-100, -90, -500), 89, &secondaryPixelString, true, false, false);

//Left Face
Camera camMiddTop = Camera(Vector3D(45, 180, 0),   Vector3D(100, 190, -500), 89, &secondaryPixelString, true, false, false);
Camera camMiddBot = Camera(Vector3D(-135, 180, 0),  Vector3D(0, -190, -500), 89, &secondaryPixelString, true, false, false);

void printRenderTime(){
  Serial.print("in ");
  float dif = ((float)(micros() - previousTime)) / 1000000.0f;
  Serial.print(dif, 5);
  Serial.println("s.");
  
  previousTime = micros();
}

void renderCameras(Scene* scene){
  camFronTop.Rasterize(scene, 1.0f, MaxBrightness);
  camRearTop.Rasterize(scene, 1.0f, MaxBrightness);
  camFronBot.Rasterize(scene, 1.0f, MaxBrightness);
  camRearBot.Rasterize(scene, 1.0f, MaxBrightness);
  camMiddTop.Rasterize(scene, 1.0f, MaxBrightness);
  camMiddBot.Rasterize(scene, 1.0f, MaxBrightness);
}

void renderCameras(BMP* bmp, uint8_t brightnessLevel){
  camFronTop.BMPRasterize(bmp, 1.0f, brightnessLevel);
  camRearTop.BMPRasterize(bmp, 1.0f, brightnessLevel);
  camFronBot.BMPRasterize(bmp, 1.0f, brightnessLevel);
  camRearBot.BMPRasterize(bmp, 1.0f, brightnessLevel);
  camMiddTop.BMPRasterize(bmp, 1.0f, brightnessLevel);
  camMiddBot.BMPRasterize(bmp, 1.0f, brightnessLevel);
}

void updateLEDS(){
  for (int i = 0; i < 306; i++) {
    leds.setPixel(i + 918, camRearTop.GetPixels()[i].Color.R, camRearTop.GetPixels()[i].Color.G, camRearTop.GetPixels()[i].Color.B);
    leds.setPixel(i + 306, camFronBot.GetPixels()[i].Color.R, camFronBot.GetPixels()[i].Color.G, camFronBot.GetPixels()[i].Color.B);
    leds.setPixel(i + 1530, camRearBot.GetPixels()[i].Color.R, camRearBot.GetPixels()[i].Color.G, camRearBot.GetPixels()[i].Color.B);
    leds.setPixel(i + 2142, camFronTop.GetPixels()[i].Color.R, camFronTop.GetPixels()[i].Color.G, camFronTop.GetPixels()[i].Color.B);
    
    if(i < 89){
      leds.setPixel(i + 1224, camMiddBot.GetPixels()[i].Color.R, camMiddBot.GetPixels()[i].Color.G, camMiddBot.GetPixels()[i].Color.B);
      leds.setPixel(i + 1836, camMiddTop.GetPixels()[i].Color.R, camMiddTop.GetPixels()[i].Color.G, camMiddTop.GetPixels()[i].Color.B);
    }
  }
  
  leds.show();
  printRenderTime();
}

void bootAnimation(){
  for (float i = 0.0f; i < 1.0f; i += 1.0f / 1260.0f){
    boot.Update(i);
    boot.FadeIn(0.0125f);
    
    renderCameras(boot.GetScene());
    updateLEDS();
  }
}

void setup() {
  leds.begin();
  leds.show();

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting...");
  delay(50);

  previousTime = micros();
  screensaverTime = millis();
}

void faceAnimation(){
    for (float i = 0.0f; i < 1.0f; i += 1.0f / 720.0f) {
        protoMorph.Update(i);

        renderCameras(protoMorph.GetScene());

        updateLEDS();
    }
}

void deathAnimation(){
    for (float i = 0; i < 1200; i += 1.2f) {
        uint8_t glitchValue = (int)i % 20 > 16 ? (int)i / 4 : 0;
        
        if (i > 720){
            renderCameras(&bootBMP, MaxBrightness);
            bootBMP.ShiftPosition(Vector2D(0, 7));
        }
        else if (i > 180){
            renderCameras(&dedBMP, MaxBrightness);
        }
        else{
            renderCameras(&crashBMP, MaxBrightness);
        }
        
        updateLEDS();
        
        bootBMP.Glitch(glitchValue);
        dedBMP.Glitch(glitchValue);
        crashBMP.Glitch(glitchValue);
        
        Serial.print(i);
        Serial.print(" ");
    }
    
    bootBMP.ResetShift();
    crashBMP.ResetShift();
}

void loop() {
  //bootAnimation();
  faceAnimation();
  //faceAnimation();
  //deathAnimation();
}
