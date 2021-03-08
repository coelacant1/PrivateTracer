#pragma once

#include "BoundaryMotionSimulator.h"
#include "MiscObjs.h"
#include "Object3D.h"
#include "Quaternion.h"
#include "Scene.h"

class PhysicsSimulator{
private:
  BoundaryCube bC = BoundaryCube(Vector3D(0, 150, 0), Vector3D(200, 200, 200));
  BoundaryMotionSimulator* bMS;
  Scene* scene;
  Light lights[6];
  Object3D* objects[4];
  Object3D icosaSphere1Obj = Object3D(icosaSphereString, 25, 25);
  Object3D icosaSphere2Obj = Object3D(icosaSphereString, 25, 25);
  Object3D icosaSphere3Obj = Object3D(icosaSphereString, 25, 25);
  Object3D icosaSphere4Obj = Object3D(icosaSphereString, 25, 25);
  long previousTime;
  bool startedSim = false;
  
public:
  PhysicsSimulator(){
    lights[0].Set(Vector3D(1000, 0, 0), Vector3D(255, 0, 0), 1000.0f, 0.75f, 0.25f);//Set lights position, color intensity, falloff distance, and falloff curvature
    lights[1].Set(Vector3D(0, 1000, 0), Vector3D(0, 255, 0), 1000.0f, 0.75f, 0.25f);
    lights[2].Set(Vector3D(0, 0, 1000), Vector3D(0, 0, 255), 1000.0f, 0.75f, 0.25f);
    lights[3].Set(Vector3D(-2000, 0, 0), Vector3D(120, 0, 120), 1000.0f, 0.75f, 0.25f);
    lights[4].Set(Vector3D(0, -1000, 0), Vector3D(120, 120, 0), 1000.0f, 0.75f, 0.25f);
    lights[5].Set(Vector3D(0, 0, -1000), Vector3D(0, 120, 120), 1000.0f, 0.75f, 0.25f);
    
    objects[0] = &icosaSphere1Obj;
    objects[1] = &icosaSphere2Obj;
    objects[2] = &icosaSphere3Obj;
    objects[3] = &icosaSphere4Obj;
    
    icosaSphere1Obj.MoveRelative(Vector3D(-60, 150, 0));
    icosaSphere2Obj.MoveRelative(Vector3D(-30, 300, 0));
    icosaSphere3Obj.MoveRelative(Vector3D(30, 300, 0));
    icosaSphere4Obj.MoveRelative(Vector3D(60, 300, 0));
    
    scene = new Scene(objects, lights, 4, 6);
    bMS = new BoundaryMotionSimulator(objects, 1, &bC, 25.0f, 0.5f);

    bMS->Randomize(500.0f);
  }
  
  Scene* GetScene(){
    return scene;
  }

  void Update(Vector3D acceleration, Quaternion rotation){
    if(!startedSim){
      previousTime = micros();
      startedSim = true;
    }
    
    long currentTime = micros();
    float dT = (float)(currentTime - previousTime) / 1000000.0f;

    bMS->Update(dT, acceleration, rotation);

    previousTime = currentTime;

    Serial.println(icosaSphere1Obj.GetPosition().ToString());
    //Serial.println(acceleration.ToString());
  }
  
};
