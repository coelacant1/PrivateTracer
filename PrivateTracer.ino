#include <OctoWS2811.h>

#include "Camera.h"
#include "Rotation.h"
#include "KalmanFilter.h"
#include "TemporaryObjCamera.h"
#include "Eye.h"
#include "ObjectDeformer.h"

const int ledsPerStrip = 306;
DMAMEM int displayMemory[ledsPerStrip * 6];
int drawingMemory[ledsPerStrip * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

Eye eyeTest;
Light lights[6];
Object3D* objects[4];
Object3D faceObj = Object3D(face, 100, 50);
Object3D dvdObj = Object3D(dvd, 100, 100);
Object3D eyeBrowObj = Object3D(eyeBrow, 20, 10);
Object3D mouthObj = Object3D(mouth, 50, 25);
ObjectDeformer sineDeformer = ObjectDeformer(objects, 4);
Scene* scene;
Camera camFrontTop = Camera(Vector3D(-45, 0, 180), Vector3D(90, -220, -500), 306, &pixelString, true, false);
Camera camRearTop = Camera(Rotation(EulerAngles(Vector3D(45, 0, 0), EulerConstants::EulerOrderXYZR)).GetQuaternion(), Vector3D(90, 90, -500), 306, &pixelString, false, false);
Camera camFrontBottom = Camera(Rotation(EulerAngles(Vector3D(0, 0, 0), EulerConstants::EulerOrderXYZR)).GetQuaternion(), Vector3D(-5, 0, -500), 306, &pixelString, true, false);
Camera camRearBottom = Camera(Rotation(EulerAngles(Vector3D(0, 0, 180), EulerConstants::EulerOrderXYZR)).GetQuaternion(), Vector3D(-20, -131, -500), 306, &pixelString, false, false);

void setup() {
  leds.begin();
  leds.show();

  pinMode(A3, INPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting...");

  lights[0].Set(Vector3D(1000, 0, 0), Vector3D(255, 0, 0), 1000.0f, 0.5f, 0.5f);//Set lights position, color intensity, falloff distance, and falloff curvature
  lights[1].Set(Vector3D(0, 1000, 0), Vector3D(0, 255, 0), 1000.0f, 0.5f, 0.5f);
  lights[2].Set(Vector3D(0, 0, 1000), Vector3D(0, 0, 255), 1000.0f, 0.5f, 0.5f);
  lights[3].Set(Vector3D(-2000, 0, 0), Vector3D(120, 0, 120), 1000.0f, 0.5f, 0.5f);
  lights[4].Set(Vector3D(0, -1000, 0), Vector3D(120, 120, 0), 1000.0f, 0.5f, 0.5f);
  lights[5].Set(Vector3D(0, 0, -1000), Vector3D(0, 120, 120), 1000.0f, 0.5f, 0.5f);

  Serial.println("Linking objects: ");
  objects[0] = &faceObj;
  objects[1] = eyeTest.GetObject();
  objects[2] = &eyeBrowObj;
  objects[3] = &mouthObj;

  scene = new Scene(objects, lights, 4, 6);
  Serial.println("Objects linked, scene created: ");
  delay(50);
}

void loop() {
  for (int i = 0; i < 720; i++) {
    Serial.print("Rendering frame ");
    Serial.print(i);
    Serial.print(" of 720 at ");
    
    if (i/2 > 300 * 2) eyeTest.Update(Eye::Sleepy, 0.06f);
    else if (i/2 > 240 * 2) eyeTest.Update(Eye::Surprised, 0.06f);
    else if (i/2 > 180 * 2) eyeTest.Update(Eye::Happy, 0.06f);
    else if (i/2 > 120 * 2) eyeTest.Update(Eye::Angry, 0.06f);
    else if (i/2 > 60 * 2) eyeTest.Update(Eye::Sad, 0.06f);
    else eyeTest.Update(Eye::Neutral, 0.06f);
    
    objects[0]->Enable();
    objects[2]->Enable();
    objects[3]->Enable();
    
    objects[0]->ResetVertices();
    objects[2]->ResetVertices();
    objects[3]->ResetVertices();
    
    objects[0]->Move(Vector3D(-35, 5, 0));
    objects[2]->Move(Vector3D(-40, 30, 0));
    objects[3]->Move(Vector3D(-40, 20, 0));

    sineDeformer.SineWaveSurfaceDeform(Vector3D(-150, 100, 0), sinf(i * 3.14159f / 180.0f * 1.0f) * 150.0f, sinf(i * 3.14159f / 180.0f * 0.1f), 0.02f, 1000.0f, ObjectDeformer::ZAxis);
    sineDeformer.SineWaveSurfaceDeform(Vector3D(-150, 100, 0), sinf(i * 3.14159f / 180.0f * 1.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 0.1f), 0.02f, 1.0f, ObjectDeformer::XAxis);
    
    objects[2]->Scale(Vector3D(1.0f + sin(i * 3.14159f / 180.0f * 1.0f) * 0.025f, 1.0f + sin(i * 3.14159f / 180.0f * 10.0f) * 0.025f, 1.0f), Vector3D(0, 0, 0));
    
    //objects[0]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 1.0f) * 20.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 20.0f, sinf(i * 3.14159f / 180.0f * 4.0f) * 90.0f), Vector3D(0, 100, 0));
    //objects[0]->Scale(Vector3D(1.0f + sinf(i * 3.14159f / 180.0f * 2.0f) * 0.2f, 1.0f + sinf(i * 3.14159f / 180.0f * 2.0f) * 0.2f, 1.0f + sinf(i * 3.14159f / 180.0f * 2.0f) * 0.2f), Vector3D(-50, 60, 0));
    
    //objects[0]->Scale(Vector3D(1.4f + sinf(i * 3.14159f / 180.0f * 6.0f) * 0.1f, 1.4f + sinf(i * 3.14159f / 180.0f * 6.0f) * 0.1f, 1.0f), Vector3D(-50, 60, 0));
    //objects[0]->Move(Vector3D(180.0f + sinf(i * 3.14159f / 180.0f * 10.0f) * 15.0f, 40 + cosf(i * 3.14159f / 180.0f * 10.0f) * 15.0f, 0.0f));

    //objects[0]->Scale(Vector3D(1.3f + sin(i * 3.14159f / 180.0f * 6.0f) * 0.1f, 1.3f + sin(i * 3.14159f / 180.0f * 6.0f) * 0.1f, 1.0f), Vector3D(-50, 60, 0));
    //objects[1]->Scale(Vector3D(1.3f + sin(i * 3.14159f / 180.0f * 6.0f) * 0.1f, 1.3f + sin(i * 3.14159f / 180.0f * 6.0f) * 0.1f, 1.0f), Vector3D(-50, 60, 0));
    
    //objects[1]->Move(Vector3D(-60.0f + sin(i * 3.14159f / 180.0f * 18.0f) * 10.0f, 20 + cos(i * 3.14159f / 180.0f * 36.0f) * 10.0f, 0.0f));
    
    objects[0]->Rotate(Vector3D(3 + sinf(i * 3.14159f / 180.0f * 4.0f) * 1.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 1.0f, 0), Vector3D(0, 100, 0));
    objects[1]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 4.0f) * 3.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 3.0f, 0), Vector3D(0, 100, 0));
    objects[2]->Rotate(Vector3D(7 + sinf(i * 3.14159f / 180.0f * 4.0f) * 3.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 3.0f, 0), Vector3D(0, 100, 0));
    objects[3]->Rotate(Vector3D(7 + sinf(i * 3.14159f / 180.0f * 4.0f) * 3.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 3.0f, 0), Vector3D(0, 100, 0));

    //lights[0].MoveTo(Vector3D(sinf(i * 3.14159f / 180.0f * 2.0f) * 1000.0f, 0, -cosf(i * 3.14159f / 180.0f * 2.0f) * 1000.0f));//Lights can be moved to any vector coordinate
    //lights[1].MoveTo(Vector3D(sinf(i * 3.14159f / 180.0f * 4.0f) * 1000.0f, -cosf(i * 3.14159f / 180.0f * 4.0f) * 1000.0f, 0));
    //lights[2].MoveTo(Vector3D(0, sinf(i * 3.14159f / 180.0f * 6.0f) * 1000.0f, cosf(i * 3.14159f / 180.0f * 6.0f) * 1000.0f));
    //lights[3].MoveTo(Vector3D(-sinf(i * 3.14159f / 180.0f * 2.0f) * 1000.0f, 0, cosf(i * 3.14159f / 180.0f * 2.0f) * 1000.0f));
    //lights[4].MoveTo(Vector3D(-sinf(i * 3.14159f / 180.0f * 4.0f) * 1000.0f, cosf(i * 3.14159f / 180.0f * 4.0f) * 1000.0f, 0));
    //lights[5].MoveTo(Vector3D(0, sinf(i * 3.14159f / 180.0f * 6.0f) * 1000.0f, -cosf(i * 3.14159f / 180.0f * 6.0f) * 1000.0f));

    long prev = micros();//Used to calculate the render time in seconds

    camFrontTop.Rasterize(scene, 1.0f, 15);
    camRearTop.Rasterize(scene, 1.0f, 15);
    camFrontBottom.Rasterize(scene, 1.0f, 15);
    camRearBottom.Rasterize(scene, 1.0f, 15);

    float dif = ((float)(micros() - prev)) / 1000000.0f;
    prev = micros();
    
    for (int i = 0; i < 306; i++) {
      leds.setPixel(i,       (byte)camFrontTop.GetPixels()[i].RGB.X, (byte)camFrontTop.GetPixels()[i].RGB.Y, (byte)camFrontTop.GetPixels()[i].RGB.Z);
      leds.setPixel(i + 306,   (byte)camRearTop.GetPixels()[i].RGB.X, (byte)camRearTop.GetPixels()[i].RGB.Y, (byte)camRearTop.GetPixels()[i].RGB.Z);
      leds.setPixel(i + 306 * 2, (byte)camFrontBottom.GetPixels()[i].RGB.X, (byte)camFrontBottom.GetPixels()[i].RGB.Y, (byte)camFrontBottom.GetPixels()[i].RGB.Z);
      leds.setPixel(i + 306 * 3, (byte)camRearBottom.GetPixels()[i].RGB.X, (byte)camRearBottom.GetPixels()[i].RGB.Y, (byte)camRearBottom.GetPixels()[i].RGB.Z);
    }

    leds.show();

    Serial.print(dif, 5);
    Serial.print(" ");
    Serial.print(((float)(micros() - prev)) / 1000000.0f, 5);
    Serial.println(" seconds.");
  }

}
