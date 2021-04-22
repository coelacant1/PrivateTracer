#pragma once

#include "Eye.h"
#include "Mouth.h"
#include "FaceObjs.h"
#include "Animation.h"
#include "SimpleMaterial.h"

class Face : public Animation{
private:
  Light lights[6];
  Object3D* objects[6];
  Object3D* faceRight[4];
  Object3D faceLeft = Object3D(60, 30);
  Object3D faceObj = Object3D(faceString, 25, 10);
  Object3D eyeBrowObj = Object3D(eyeBrowString, 4, 2);
  Object3D mouthObj = Object3D(mouthString, 12, 10);
  Object3D fftPlaneObj = Object3D(openHappyString, 120, 120);
  ObjectDeformer sineDeformer = ObjectDeformer(objects, 5);
  ObjectDeformer fftDeformer = ObjectDeformer(&fftPlaneObj);
  
  Eye eyeTest;
  Mouth mouthTest;
  const int frames = 720;
  
  float fftData[12];

  SimpleMaterial sMat = SimpleMaterial(RGBColor(50, 0, 0));
  
public:
  Face(){
    lights[0].Set(Vector3D(1000, 0, 0), Vector3D(255, 0, 0), 1000.0f, 0.75f, 0.25f);//Set lights position, color intensity, falloff distance, and falloff curvature
    lights[1].Set(Vector3D(0, 1000, 0), Vector3D(0, 255, 0), 1000.0f, 0.75f, 0.25f);
    lights[2].Set(Vector3D(0, 0, 1000), Vector3D(0, 0, 255), 1000.0f, 0.75f, 0.25f);
    lights[3].Set(Vector3D(-2000, 0, 0), Vector3D(120, 0, 120), 1000.0f, 0.75f, 0.25f);
    lights[4].Set(Vector3D(0, -1000, 0), Vector3D(120, 120, 0), 1000.0f, 0.75f, 0.25f);
    lights[5].Set(Vector3D(0, 0, -1000), Vector3D(0, 120, 120), 1000.0f, 0.75f, 0.25f);
  
    objects[0] = &faceObj;
    objects[1] = eyeTest.GetObject();
    objects[2] = &eyeBrowObj;
    objects[3] = mouthTest.GetObject();//&mouthObj;
    objects[4] = &fftPlaneObj;
    objects[5] = &faceLeft;

    faceRight[0] = &faceObj;
    faceRight[1] = eyeTest.GetObject();
    faceRight[2] = &eyeBrowObj;
    faceRight[3] = mouthTest.GetObject();//&mouthObj;

    faceLeft.SetMaterial(&sMat);
  
    scene = new Scene(objects, lights, 6, 6);

    for (int i=0; i < 12; i++) {  // print the first 20 bins
      fftData[i] = 0.0f;
    }
  }

  void FadeIn(float stepRatio){
    if (fade < 1.0f){
      fade += stepRatio;

      float ratio = Mathematics::CosineInterpolation(0.0f, 1.0f, fade);

      objects[0]->Scale(Vector3D(ratio, 1.0f, 1.0f), Vector3D(-40, 130, 0));
      objects[1]->Scale(Vector3D(ratio, 1.0f, 1.0f), Vector3D(-40, 130, 0));
      objects[2]->Scale(Vector3D(ratio, 1.0f, 1.0f), Vector3D(-40, 130, 0));
      objects[3]->Scale(Vector3D(ratio, 1.0f, 1.0f), Vector3D(-40, 130, 0));
    }
  }

  void FadeOut(float stepRatio){
    if (fade > 0.0f){
      fade += stepRatio;

      float ratio = Mathematics::CosineInterpolation(0.0f, 1.0f, fade);

      objects[0]->Scale(Vector3D(ratio, 1.0f, 1.0f), Vector3D(-40, 130, 0));
      objects[1]->Scale(Vector3D(ratio, 1.0f, 1.0f), Vector3D(-40, 130, 0));
      objects[2]->Scale(Vector3D(ratio, 1.0f, 1.0f), Vector3D(-40, 130, 0));
      objects[3]->Scale(Vector3D(ratio, 1.0f, 1.0f), Vector3D(-40, 130, 0));
    }
  }

  void Boop(float ratio){
    ratio = Mathematics::Constrain(ratio, 0.0f, 1.0f);
    int i = (int)(ratio * (float)frames);
    
    objects[0]->Enable();
    objects[1]->Enable();
    objects[2]->Enable();
    objects[3]->Enable();
    objects[4]->Disable();
    
    objects[0]->ResetVertices();
    objects[2]->ResetVertices();
    objects[4]->ResetVertices();
    
    if (i > 360){
      eyeTest.Update(Eye::Surprised, 0.01f);
      mouthTest.Update(Mouth::Confuse, 0.08f);

      float lookDir = Mathematics::Constrain(sinf(i * 3.14159f / 180.0f * 2.0f) * 1.5f, -1.0f, 1.0f);
      eyeTest.Look(lookDir);
      
      objects[0]->Rotate(Vector3D(3 + sinf(i * 3.14159f / 180.0f * 2.0f) * 1.0f, sinf(i * 3.14159f / 180.0f * 1.0f) * 1.0f, 0), Vector3D(0, 100, 0));
      objects[1]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 2.0f) * 1.0f, sinf(i * 3.14159f / 180.0f * 1.0f) * 1.0f, 0), Vector3D(0, 100, 0));
      objects[2]->Rotate(Vector3D(7 + sinf(i * 3.14159f / 180.0f * 1.0f) * 1.0f, sinf(i * 3.14159f / 180.0f * 1.0f) * 1.0f, 0), Vector3D(0, 100, 0));
      objects[3]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 2.0f) * 1.0f, sinf(i * 3.14159f / 180.0f * 1.0f) * 1.0f, 0), Vector3D(0, 100, 0));
    }
    else{
      eyeTest.Look(-1.0f);
      eyeTest.Update(Eye::Happy, 0.02f);
      mouthTest.Update(Mouth::Mlem, 0.03f);
      //objects[3]->Scale(Vector3D(1.0f + sinf(i * 3.14159f / 180.0f * 1.0f) * 0.3f, 1.0f + sinf(i * 3.14159f / 180.0f * 10.0f) * 0.025f, 1.0f), Vector3D(-170, 0, 0));
      //objects[1]->Scale(Vector3D(1.0f, 1.0f + sinf(i * 3.14159f / 180.0f * 1.5f) * 0.4f - 0.4f, 1.0f), Vector3D(-40, 130, 0));
      
      objects[0]->Rotate(Vector3D(3 + sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
      objects[1]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
      objects[2]->Rotate(Vector3D(7 + sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
      objects[3]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
    }
    
    objects[0]->MoveRelative(Vector3D(-35, 5, 0));
    objects[2]->MoveRelative(Vector3D(-40, 30, 0));

    objects[2]->Scale(Vector3D(1.0f + sinf(i * 3.14159f / 180.0f * 1.0f) * 0.025f, 1.0f + sinf(i * 3.14159f / 180.0f * 10.0f) * 0.025f, 1.0f), Vector3D(0, 0, 0));
    
    sineDeformer.SineWaveSurfaceDeform(Vector3D(-150, 100, 0), sinf(i * 3.14159f / 180.0f * 1.0f) * 100.0f, sinf(-i * 3.14159f / 180.0f * 0.1f), 0.1f, 200.0f, ObjectDeformer::ZAxis);
    sineDeformer.SineWaveSurfaceDeform(Vector3D(-150, 100, 0), sinf(i * 3.14159f / 180.0f * 1.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 0.1f), 0.02f, 1.0f, ObjectDeformer::XAxis);
  }

  void Rave(float ratio){
    ratio = Mathematics::Constrain(ratio, 0.0f, 1.0f);
    int i = (int)(ratio * (float)frames);
    
    objects[0]->Enable();
    objects[1]->Enable();
    objects[2]->Enable();
    objects[3]->Disable();
    objects[4]->Enable();
    
    eyeTest.Update(Eye::Neutral, 0.02f);
    mouthTest.Update(Mouth::Mlem, 0.03f);
    
    float lookDir = Mathematics::Constrain(sinf(i * 3.14159f / 180.0f * 2.0f) * 0.25f, -1.0f, 1.0f);
    eyeTest.Look(lookDir);
    
    objects[0]->ResetVertices();
    objects[2]->ResetVertices();
    objects[4]->ResetVertices();
    
    objects[0]->MoveRelative(Vector3D(-35, 5, 0));
    objects[2]->MoveRelative(Vector3D(-40, 30, 0));
    
    
    //fftDeformer.CosineInterpolationDeformer(fftData, 10, 10.0f, -200.0f, 200.0f, ObjectDeformer::XAxis, ObjectDeformer::YAxis);
    mouthTest.Talk(fftData);

    //objects[4]->Move(Vector3D(0.0f, 40.0f, 0.0f)); Vector3D(-110, 70, 0)
    objects[4]->MoveRelative(Vector3D(-110, 70, 0));
    objects[4]->Rotate(Vector3D(0.0f, 0.0f, 45.0f), Vector3D(0, 50, 0));

    
    objects[4]->Rotate(Vector3D(9,0, 0), Vector3D(0, 100, 0));
    //objects[4]->Scale(Vector3D(0.4f, 0.9f, 1.0f), Vector3D(-200, 0, 0));
    
    sineDeformer.SineWaveSurfaceDeform(Vector3D(-150, 100, 0), sinf(i * 3.14159f / 180.0f * 1.0f) * 100.0f, sinf(-i * 3.14159f / 180.0f * 0.1f), 0.1f, 200.0f, ObjectDeformer::ZAxis);
    sineDeformer.SineWaveSurfaceDeform(Vector3D(-150, 100, 0), sinf(i * 3.14159f / 180.0f * 1.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 0.1f), 0.02f, 1.0f, ObjectDeformer::XAxis);
    
    objects[0]->Rotate(Vector3D(3 + sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
    objects[1]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
    objects[2]->Rotate(Vector3D(7 + sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
    objects[3]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
    objects[4]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
    
    //lights[0].MoveTo(Vector3D(sinf(i * 3.14159f / 180.0f * 2.0f) * 1000.0f, 0, -cosf(i * 3.14159f / 180.0f * 2.0f) * 1000.0f));
    //lights[1].MoveTo(Vector3D(sinf(i * 3.14159f / 180.0f * 4.0f) * 1000.0f, -cosf(i * 3.14159f / 180.0f * 4.0f) * 1000.0f, 0));
    //lights[2].MoveTo(Vector3D(0, sinf(i * 3.14159f / 180.0f * 6.0f) * 1000.0f, cosf(i * 3.14159f / 180.0f * 6.0f) * 1000.0f));
    //lights[3].MoveTo(Vector3D(-sinf(i * 3.14159f / 180.0f * 2.0f) * 1000.0f, 0, cosf(i * 3.14159f / 180.0f * 2.0f) * 1000.0f));
    //lights[4].MoveTo(Vector3D(-sinf(i * 3.14159f / 180.0f * 4.0f) * 1000.0f, cosf(i * 3.14159f / 180.0f * 4.0f) * 1000.0f, 0));
    //lights[5].MoveTo(Vector3D(0, sinf(i * 3.14159f / 180.0f * 6.0f) * 1000.0f, -cosf(i * 3.14159f / 180.0f * 6.0f) * 1000.0f));
  }

  void Default(float ratio){
    ratio = Mathematics::Constrain(ratio, 0.0f, 1.0f);
    int i = (int)(ratio * (float)frames);
    
    objects[0]->Enable();
    objects[1]->Enable();
    objects[2]->Enable();
    objects[3]->Enable();
    objects[4]->Disable();
    
    objects[0]->ResetVertices();
    objects[2]->ResetVertices();
    
    if (i > 550){
      eyeTest.Update(Eye::Surprised, 0.03f);
      mouthTest.Update(Mouth::OhNo, 0.05f);
    }
    else if (i > 400){
      eyeTest.Update(Eye::Happy, 0.05f);
      mouthTest.Update(Mouth::HappyRed, 0.03f);
    }
    else if (i > 200){
      eyeTest.Update(Eye::Neutral, 0.07f);
      mouthTest.Update(Mouth::Neutral, 0.1f);
    }
    else{
      eyeTest.Update(Eye::Happy, 0.02f);
      mouthTest.Update(Mouth::ClosedHappy, 0.03f);
    }
    
    objects[0]->Rotate(Vector3D(3 + sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
    objects[1]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
    objects[2]->Rotate(Vector3D(7 + sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
    objects[3]->Rotate(Vector3D(sinf(i * 3.14159f / 180.0f * 4.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 2.0f) * 4.0f), Vector3D(0, 100, 0));
    
    objects[0]->MoveRelative(Vector3D(-35, 5, 0));
    objects[2]->MoveRelative(Vector3D(-40, 30, 0));

    //objects[2]->Scale(Vector3D(1.0f + sinf(i * 3.14159f / 180.0f * 1.0f) * 0.025f, 1.0f + sinf(i * 3.14159f / 180.0f * 10.0f) * 0.025f, 1.0f), Vector3D(0, 0, 0));
    
    sineDeformer.SineWaveSurfaceDeform(Vector3D(-150, 100, 0), sinf(i * 3.14159f / 180.0f * 1.0f) * 100.0f, sinf(-i * 3.14159f / 180.0f * 0.1f), 0.1f, 200.0f, ObjectDeformer::ZAxis);
    sineDeformer.SineWaveSurfaceDeform(Vector3D(-150, 100, 0), sinf(i * 3.14159f / 180.0f * 1.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 0.1f), 0.02f, 1.0f, ObjectDeformer::XAxis);

  }

  void UpdateFFT(float value, int index){
    fftData[index] = value;
  }

  void Drift(Vector3D acceleration, Vector3D angularVelocity){
    angularVelocity = angularVelocity.Divide(10);
    angularVelocity = Vector3D(-angularVelocity.X, -angularVelocity.Z, angularVelocity.Y);
    acceleration = acceleration.Multiply(15);
    acceleration = Vector3D(acceleration.Y, acceleration.Z, acceleration.X);
    Vector3D zMod = Vector3D(1.0f + acceleration.Z * 0.005f, 1.0f + acceleration.Z * 0.005f, 1.0f);
    
    objects[0]->Rotate(angularVelocity, Vector3D(0, 100, 0));
    objects[1]->Rotate(angularVelocity, Vector3D(0, 100, 0));
    objects[2]->Rotate(angularVelocity, Vector3D(0, 100, 0));
    objects[3]->Rotate(angularVelocity, Vector3D(0, 100, 0));
    objects[4]->Rotate(angularVelocity, Vector3D(0, 100, 0));
    
    objects[0]->MoveRelative(acceleration);
    objects[1]->MoveRelative(acceleration);
    objects[2]->MoveRelative(acceleration);
    objects[3]->MoveRelative(acceleration);
    objects[4]->MoveRelative(acceleration);

    objects[0]->Scale(zMod, Vector3D(0, 100, 0));
    objects[1]->Scale(zMod, Vector3D(0, 100, 0));
    objects[2]->Scale(zMod, Vector3D(0, 100, 0));
    objects[3]->Scale(zMod, Vector3D(0, 100, 0));
    objects[4]->Scale(zMod, Vector3D(0, 100, 0));
  }

  void Update(float ratio){
    //Boop(ratio);
    //Rave(ratio);
    Default(ratio);
    
    mouthTest.Talk(fftData);

    faceLeft.Copy(faceRight, 4);

    faceLeft.Scale(Vector3D(1.0f, 1.0f, -1.0f), Vector3D(0, 100, 0));

    float x = sinf(ratio * 3.14159f / 180.0f * 720.0f) * 40.0f;
    float y = 0;//cosf(ratio * 3.14159f / 180.0f * 1440.0f) * 40.0f;

    objects[5]->MoveRelative(Vector3D(x, y, 600.0f));

    //Serial.print(x);
    //Serial.print(" ");
    //Serial.println(y);
    
    objects[0]->Enable();
    objects[1]->Enable();
    objects[2]->Enable();
    objects[3]->Enable();
    objects[4]->Disable();
    objects[5]->Enable();
  }
};
