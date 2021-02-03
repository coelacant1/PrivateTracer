#pragma once

#include "Eye.h"
#include "Mouth.h"
#include "FaceObjs.h"
#include "Animation.h"

class Face : public Animation{
private:
  Light lights[6];
  Object3D* objects[4];
  Object3D faceObj = Object3D(faceString, 100, 50);
  Object3D eyeBrowObj = Object3D(eyeBrowString, 20, 10);
  Object3D mouthObj = Object3D(mouthString, 50, 25);
  ObjectDeformer sineDeformer = ObjectDeformer(objects, 4);
  
  Eye eyeTest;
  Mouth mouthTest;
  const int frames = 720;
  
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
  
    scene = new Scene(objects, lights, 4, 6);
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

  void Update(float ratio){
    ratio = Mathematics::Constrain(ratio, 0.0f, 1.0f);
    int i = (int)(ratio * (float)frames);
    
    objects[0]->Enable();
    objects[1]->Enable();
    objects[2]->Enable();
    objects[3]->Enable();
    
    objects[0]->ResetVertices();
    objects[2]->ResetVertices();
    
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

    
    objects[0]->Move(Vector3D(-35, 5, 0));
    objects[2]->Move(Vector3D(-40, 30, 0));

    objects[2]->Scale(Vector3D(1.0f + sinf(i * 3.14159f / 180.0f * 1.0f) * 0.025f, 1.0f + sinf(i * 3.14159f / 180.0f * 10.0f) * 0.025f, 1.0f), Vector3D(0, 0, 0));
    
    sineDeformer.SineWaveSurfaceDeform(Vector3D(-150, 100, 0), sinf(i * 3.14159f / 180.0f * 1.0f) * 100.0f, sinf(-i * 3.14159f / 180.0f * 0.1f), 0.1f, 200.0f, ObjectDeformer::ZAxis);
    sineDeformer.SineWaveSurfaceDeform(Vector3D(-150, 100, 0), sinf(i * 3.14159f / 180.0f * 1.0f) * 2.0f, sinf(i * 3.14159f / 180.0f * 0.1f), 0.02f, 1.0f, ObjectDeformer::XAxis);
  }
};
