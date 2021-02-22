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

BMP testBMP = BMP(Vector2D(100, 100), Vector2D(-50, 0), protoTest);

const int MaxBrightness = 20;

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
    leds.setPixel(i + 1224, camFronTop.GetPixels()[i].Color.R, camFronTop.GetPixels()[i].Color.G, camFronTop.GetPixels()[i].Color.B);
    leds.setPixel(i + 306, camRearTop.GetPixels()[i].Color.R, camRearTop.GetPixels()[i].Color.G, camRearTop.GetPixels()[i].Color.B);
    leds.setPixel(i + 612, camFronBot.GetPixels()[i].Color.R, camFronBot.GetPixels()[i].Color.G, camFronBot.GetPixels()[i].Color.B);
    leds.setPixel(i + 918, camRearBot.GetPixels()[i].Color.R, camRearBot.GetPixels()[i].Color.G, camRearBot.GetPixels()[i].Color.B);
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
  AudioMemory(100);
  amp1.gain(4.0);

  filter1.frequency(750);
  filter1.octaveControl(4);
  
  leds.begin();
  leds.show();

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting...");
  delay(50);

  previousTime = micros();

  //bootAnimation();
}

void loop() {
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
    
    motionProcessor.Update();

    //Serial.println(camRearTop.GetPictureCenter().ToString());
    
    face.Update(i);
    face.FadeIn(0.0125f);
    face.Drift(motionProcessor.GetLocalAcceleration(), motionProcessor.GetLocalAngularVelocity());

    //Serial.println(motionProcessor.GetLocalAngularVelocity().ToString());

    updateLEDs(face.GetScene());
    Serial.print(i);
    Serial.print(" ");
  }
}
