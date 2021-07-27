#include <Arduino.h>
#include <OctoWS2811.h>

#include "Animation/Boot.h"
#include "Filter/KalmanFilter.h"
#include "Flash/CrashObjs.h"
#include "Flash/MiscObjs.h"
#include "Render/Camera.h"
#include "Math/Rotation.h"
#include "Morph/ProtoDRMorphAnimator.h"
#include "Render/ObjectDeformer.h"

#include "Flash/ProtoDR.h"
#include "Flash/ProtoDRMini.h"

const int ledsPerStrip = 306;
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

Boot boot;
ProtoDRMorphAnimator protoMorph;

const uint8_t MaxBrightness = 40;
long previousTime = 0;
/*
Transform camFronTopTransform = Transform(Vector3D( -45,   0, 180), Vector3D(  90, -220, -500), Vector3D(-1, 1, 1));
Transform camRearTopTransform = Transform(Vector3D(  45,   0,   0), Vector3D(  90,   90, -500), Vector3D( 1, 1, 1));
Transform camFronBotTransform = Transform(Vector3D(   0,   0,   0), Vector3D(  -5,    0, -500), Vector3D(-1, 1, 1));
Transform camRearBotTransform = Transform(Vector3D( -45,   0, 180), Vector3D( -20, -131, -500), Vector3D( 1, 1, 1));*/

Transform camFronTopTransform = Transform(Vector3D(   0,   0,   0), Vector3D(  0, 0, -500), Vector3D( -1, 1, 1));
Transform camRearTopTransform = Transform(Vector3D(   0,   0,   0), Vector3D(  0, 0, -500), Vector3D(1, 1, 1));
Transform camFronBotTransform = Transform(Vector3D(   0,   0,   0), Vector3D(  0, 0, -500), Vector3D( -1, 1, 1));
Transform camRearBotTransform = Transform(Vector3D(   0,   0,   0), Vector3D(  0, 0, -500), Vector3D(1, 1, 1));

//Right
Transform camRearMidTransform = Transform(Vector3D(  0, 0,   0), Vector3D( 0,  0, -500), Vector3D(-1, 1, 1));
Transform camFronMidTransform = Transform(Vector3D(0, 0,   0), Vector3D( 0,  0, -500), Vector3D(-1, 1, 1));

//Left
//Transform camMiddTopTransform = Transform(Vector3D(  45, 180,   0), Vector3D(  -5,    0, -500), Vector3D(-1, 1, 1));
//Transform camMiddBotTransform = Transform(Vector3D(-135, 180,   0), Vector3D( -20, -131, -500), Vector3D(-1, 1, 1));

PixelGroup camFronTopPixels = PixelGroup(ProtoDR, 306);
PixelGroup camRearTopPixels = PixelGroup(ProtoDR, 306);
PixelGroup camFronBotPixels = PixelGroup(ProtoDR, 306);
PixelGroup camRearBotPixels = PixelGroup(ProtoDR, 306);
PixelGroup camRearMidPixels = PixelGroup(ProtoDRMini, 89);
PixelGroup camFronMidPixels = PixelGroup(ProtoDRMini, 89);

Camera camFronTop = Camera(&camFronTopTransform, &camFronTopPixels);
Camera camRearTop = Camera(&camRearTopTransform, &camRearTopPixels);
Camera camFronBot = Camera(&camFronBotTransform, &camFronBotPixels);
Camera camRearBot = Camera(&camRearBotTransform, &camRearBotPixels);
Camera camRearMid = Camera(&camRearMidTransform, &camRearMidPixels);
Camera camFronMid = Camera(&camFronMidTransform, &camFronMidPixels);

void printRenderTime(){
    Serial.print("Rendered in ");
    float dif = ((float)(micros() - previousTime)) / 1000000.0f;
    Serial.print(dif, 5);
    Serial.println("s.");
    
    previousTime = micros();
}

void renderCameras(Scene* scene){
    previousTime = micros();
    camFronTop.Rasterize(scene);
    camRearTop.Rasterize(scene);
    camFronBot.Rasterize(scene);
    camRearBot.Rasterize(scene);
    camRearMid.Rasterize(scene);
    camFronMid.Rasterize(scene);
    printRenderTime();
}

void updateLEDS(){
    for (int i = 0; i < 306; i++) {
        camFronTopPixels.GetPixel(i)->Color = camFronTopPixels.GetPixel(i)->Color.Scale(MaxBrightness);
        camRearTopPixels.GetPixel(i)->Color = camRearTopPixels.GetPixel(i)->Color.Scale(MaxBrightness);
        camFronBotPixels.GetPixel(i)->Color = camFronBotPixels.GetPixel(i)->Color.Scale(MaxBrightness);
        camRearBotPixels.GetPixel(i)->Color = camRearBotPixels.GetPixel(i)->Color.Scale(MaxBrightness);

        //0 nothing
        //306 front bottom
        //612 mid panel front
        //918 rear top panel
        //1224 nothing
        //1530 rear bottom panel
        //1836 mid panel rear
        //2142 front top panel


        leds.setPixel(i + 2142,  camFronTopPixels.GetPixel(i)->Color.R, camFronTopPixels.GetPixel(i)->Color.G, camFronTopPixels.GetPixel(i)->Color.B);//918
        leds.setPixel(i + 918,  camRearTopPixels.GetPixel(i)->Color.R, camRearTopPixels.GetPixel(i)->Color.G, camRearTopPixels.GetPixel(i)->Color.B);//306
        leds.setPixel(i + 306, camFronBotPixels.GetPixel(i)->Color.R, camFronBotPixels.GetPixel(i)->Color.G, camFronBotPixels.GetPixel(i)->Color.B);//1530
        leds.setPixel(i + 1530, camRearBotPixels.GetPixel(i)->Color.R, camRearBotPixels.GetPixel(i)->Color.G, camRearBotPixels.GetPixel(i)->Color.B);//2142
        
        if(i < 89){
            camRearMidPixels.GetPixel(i)->Color = camRearMidPixels.GetPixel(i)->Color.Scale(MaxBrightness);
            camFronMidPixels.GetPixel(i)->Color = camFronMidPixels.GetPixel(i)->Color.Scale(MaxBrightness);
            
            leds.setPixel(i + 1836, camRearMidPixels.GetPixel(i)->Color.R, camRearMidPixels.GetPixel(i)->Color.G, camRearMidPixels.GetPixel(i)->Color.B);//1224
            leds.setPixel(i + 612, camFronMidPixels.GetPixel(i)->Color.R, camFronMidPixels.GetPixel(i)->Color.G, camFronMidPixels.GetPixel(i)->Color.B);//1836
        }
    }
    
    leds.show();
}

void setup() {
    leds.begin();
    leds.show();

    Serial.begin(115200);
    Serial.println();
    Serial.println("Starting...");
    delay(250);

    previousTime = micros();
}

void bootAnimation(){
    for (float i = 0.0f; i < 1.0f; i += 1.0f / 1260.0f){
        boot.Update(i);
        boot.FadeIn(0.0125f);
        
        renderCameras(boot.GetScene());
        updateLEDS();
    }
}

void faceAnimation(){
    for (float i = 0.0f; i < 1.0f; i += 1.0f / 720.0f) {
        protoMorph.Update(i);

        protoMorph.GetScene();

        renderCameras(protoMorph.GetScene());

        updateLEDS();
    }
}

void loop() {
    //bootAnimation();
    faceAnimation();
    //faceAnimation();
    //deathAnimation();
}
