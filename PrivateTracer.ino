#include <OctoWS2811.h>
#include <Audio.h>

#include "Camera.h"
#include "Rotation.h"
#include "KalmanFilter.h"
#include "CameraObjs.h"
#include "ObjectDeformer.h"
#include "Boot.h"
#include "Face.h"
#include "MotionProcessor.h"
#include "BMP.h"
#include "BMPImages.h"
#include "CrashObjs.h"
#include "PhysicsSimulator.h"

MotionProcessor motionProcessor;

const int ledsPerStrip = 306;
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);
AudioInputAnalog         adc1(A3);
AudioAmplifier           amp1;
AudioFilterStateVariable filter1;        //xy=496,419
AudioAnalyzeFFT256       fft256_1;
AudioOutputMQS           mqs1;
AudioConnection          patchCord1(adc1, amp1);
AudioConnection          patchCord2(amp1, 0, filter1, 0);
AudioConnection          patchCord3(filter1, 1, mqs1, 0);
AudioConnection          patchCord4(filter1, 1, mqs1, 1);
AudioConnection          patchCord5(filter1, 1, fft256_1, 0);
long previousTime = micros();

Boot boot;
Face face;
PhysicsSimulator physicsSim;

//BMP openInvaderBMP = BMP(Vector2D(250, 250), Vector2D(-150, 20), openInvader, 0);
//BMP closeInvaderBMP = BMP(Vector2D(250, 250), Vector2D(-150, 20), closeInvader, 0);
//BMP colorTestBMP = BMP(Vector2D(400, 300), Vector2D(-200, 0), colorTest, 0);

BMP bootBMP = BMP(Vector2D(800, 3200), Vector2D(-200, -3200), bootImage, 2);
BMP crashBMP = BMP(Vector2D(400, 300), Vector2D(-200, 0), crashImage, 0);
BMP dedBMP = BMP(Vector2D(200, 200), Vector2D(20, 20), dedImage, 0);

const uint8_t MaxBrightness = 30;
long screensaverTime = 0;

Camera camFronTop = Camera(Vector3D(-45, 0, 180), Vector3D(90, -220, -500),  306, &primaryPixelString, true, false);
Camera camRearTop = Camera(Vector3D(45, 0, 0),    Vector3D(90, 90, -500),    306, &primaryPixelString, false, false);
Camera camFronBot = Camera(Vector3D(0, 0, 0),     Vector3D(-5, 0, -500),     306, &primaryPixelString, true, false);
Camera camRearBot = Camera(Vector3D(0, 0, 180),   Vector3D(-20, -131, -500), 306, &primaryPixelString, false, false);

Camera camMiddTop = Camera(Vector3D(45, 0, 0),   Vector3D(300, 0, -500), 89, &secondaryPixelString, true, false);
Camera camMiddBot = Camera(Vector3D(-135, 0, 0),  Vector3D(-100, -90, -500), 89, &secondaryPixelString, true, false);

void printRenderTime(){
  Serial.print("in ");
  float dif = ((float)(micros() - previousTime)) / 1000000.0f;
  Serial.print(dif, 5);
  Serial.println("s.");
  
  previousTime = micros();
}

void updateLEDs(Scene* scene){
  camFronTop.Rasterize(scene, 1.0f, MaxBrightness);
  camRearTop.Rasterize(scene, 1.0f, MaxBrightness);
  camFronBot.Rasterize(scene, 1.0f, MaxBrightness);
  camRearBot.Rasterize(scene, 1.0f, MaxBrightness);
  camMiddTop.Rasterize(scene, 1.0f, MaxBrightness);
  camMiddBot.Rasterize(scene, 1.0f, MaxBrightness);
  
  for (int i = 0; i < 306; i++) {
    leds.setPixel(i + 1224, camFronTop.GetPixels()[i].Color.R, camFronTop.GetPixels()[i].Color.G, camFronTop.GetPixels()[i].Color.B);
    leds.setPixel(i + 306, camRearTop.GetPixels()[i].Color.R, camRearTop.GetPixels()[i].Color.G, camRearTop.GetPixels()[i].Color.B);
    leds.setPixel(i + 612, camFronBot.GetPixels()[i].Color.R, camFronBot.GetPixels()[i].Color.G, camFronBot.GetPixels()[i].Color.B);
    leds.setPixel(i + 918, camRearBot.GetPixels()[i].Color.R, camRearBot.GetPixels()[i].Color.G, camRearBot.GetPixels()[i].Color.B);

    if(i < 89){
      leds.setPixel(i + 1530, camMiddBot.GetPixels()[i].Color.R, camMiddBot.GetPixels()[i].Color.G, camMiddBot.GetPixels()[i].Color.B);
    }
    else if (i < 178){
      leds.setPixel(i + 1530, camMiddTop.GetPixels()[i - 89].Color.R, camMiddTop.GetPixels()[i - 89].Color.G, camMiddTop.GetPixels()[i - 89].Color.B);
    }
  }
  
  leds.show();
  //printRenderTime();
}

void updateLEDs(BMP* bmp, uint8_t brightnessLevel){
  camFronTop.BMPRasterize(bmp, 1.0f, brightnessLevel);
  camRearTop.BMPRasterize(bmp, 1.0f, brightnessLevel);
  camFronBot.BMPRasterize(bmp, 1.0f, brightnessLevel);
  camRearBot.BMPRasterize(bmp, 1.0f, brightnessLevel);
  camMiddTop.BMPRasterize(bmp, 1.0f, brightnessLevel);
  camMiddBot.BMPRasterize(bmp, 1.0f, brightnessLevel);
  
  for (int i = 0; i < 306; i++) {
    leds.setPixel(i + 1224, camFronTop.GetPixels()[i].Color.R, camFronTop.GetPixels()[i].Color.G, camFronTop.GetPixels()[i].Color.B);
    leds.setPixel(i + 306, camRearTop.GetPixels()[i].Color.R, camRearTop.GetPixels()[i].Color.G, camRearTop.GetPixels()[i].Color.B);
    leds.setPixel(i + 612, camFronBot.GetPixels()[i].Color.R, camFronBot.GetPixels()[i].Color.G, camFronBot.GetPixels()[i].Color.B);
    leds.setPixel(i + 918, camRearBot.GetPixels()[i].Color.R, camRearBot.GetPixels()[i].Color.G, camRearBot.GetPixels()[i].Color.B);
    
    if(i < 89){
      leds.setPixel(i + 1530, camMiddBot.GetPixels()[i].Color.R, camMiddBot.GetPixels()[i].Color.G, camMiddBot.GetPixels()[i].Color.B);
    }
    else if (i < 178){
      leds.setPixel(i + 1530, camMiddTop.GetPixels()[i - 89].Color.R, camMiddTop.GetPixels()[i - 89].Color.G, camMiddTop.GetPixels()[i - 89].Color.B);
    }
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
  //AudioMemory(100);
  //amp1.gain(4.0);

  //filter1.frequency(750);
  //filter1.octaveControl(4);
  
  leds.begin();
  leds.show();

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting...");
  delay(50);

  //bootAnimation();
  previousTime = micros();
  screensaverTime = millis();
}

void faceAnimation(){
  for (float i = 0.0f; i < 1.0f; i += 1.0f / 720.0f) {
    
    if (fft256_1.available()) {
      for (int i=4; i < 16; i++) {  // print the first 20 bins
        float fftOut = fft256_1.read(i);

        fftOut = fftOut < 0.007f ? 0.0f : fftOut;
        
        fftOut = Mathematics::Constrain(fftOut * (1600.0f - i * 20.0f) + 1.0f, 1.0f, 30.0f);
        face.UpdateFFT(fftOut, i - 4);
        
        //Serial.print(fftOut, 3);
        //Serial.print(" ");
      }
    }
    

    //motionProcessor.Update();
    
    Vector3D angularVelocity = motionProcessor.GetLocalAngularVelocity();
    
    physicsSim.Update(angularVelocity * 10.0f, Quaternion());

    if(fabs(angularVelocity.X) > 90 || fabs(angularVelocity.Y) > 90 || fabs(angularVelocity.Z) > 90){
      screensaverTime = millis();
    }

    //Serial.println(camRearTop.GetPictureCenter().ToString());
    
    face.Update(i);
    face.FadeIn(0.0125f);
    //face.Drift(motionProcessor.GetLocalAccelerationFiltered(), motionProcessor.GetLocalAngularVelocityFiltered());
    face.Drift(motionProcessor.GetLocalAcceleration(), Vector3D());//, motionProcessor.GetLocalAngularVelocity() * 4.0f);

    //Serial.println(motionProcessor.GetLocalAcceleration().ToString());

    //if not much change for 5 seconds, physics sim

    if(millis() - screensaverTime > 10000 && false){//DISABLED PHYSICS ANIMATION
      updateLEDs(physicsSim.GetScene());
    }
    else{
      updateLEDs(face.GetScene());
    }
    
    //Serial.print(i);
    //Serial.print(" ");
  }
}

void deathAnimation(){
  for (float i = 0; i < 1200; i += 1.2f) {
    uint8_t glitchValue = (int)i % 20 > 16 ? (int)i / 4 : 0;
    
    if (i > 720){
      updateLEDs(&bootBMP, MaxBrightness);
      bootBMP.ShiftPosition(Vector2D(0, 7));
    }
    else if (i > 180){
      updateLEDs(&dedBMP, MaxBrightness);
    }
    else{
      updateLEDs(&crashBMP, MaxBrightness);
    }
    
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
