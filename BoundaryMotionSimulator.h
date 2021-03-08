#pragma once

#include "Object3D.h"
#include "Rotation.h"
#include "Vector3D.h"

class BoundarySphere{
private:
  Object3D* object;
  Vector3D centerPosition;
  Quaternion previousRotation;
  float radius = 1.0f;
  
public:
  Vector3D velocity;
  
  BoundarySphere(Object3D* object, float radius){
    this->object = object;
    this->centerPosition = object->GetVertexCenter();
    this->radius = radius;
  }
  
  BoundarySphere(Object3D* object, Vector3D centerPosition, float radius){
    this->object = object;
    this->centerPosition = centerPosition;
    this->radius = radius;
  }

  Vector3D GetPosition(){
    return object->GetPosition();
  }

  float GetRadius(){
    return radius;
  }

  void Update(float dT, Vector3D acceleration, Quaternion rotation){
    Quaternion rotationChange = rotation.Multiply(previousRotation.MultiplicativeInverse());
    velocity = rotationChange.RotateVector(velocity) + acceleration * dT;//maintain momentum of existing velocity, apply local acceleration
    //position = position + velocity * dT;

    //non constant dt causes a switch in states and hold on wall, need to shift inward

    Serial.println(velocity.ToString());

    object->MoveRelative(velocity * dT);

    previousRotation = rotation;
  }
  
  bool IsIntersecting(BoundarySphere *bO){
    return radius + bO->GetRadius() < fabs((GetPosition() - bO->GetPosition()).Magnitude());
  }
  
  void Collide(float elasticity, BoundarySphere *bO){
    if(IsIntersecting(bO)){
      //collision
      Vector3D difference = GetPosition() - bO->GetPosition();
      Vector3D normal = difference / difference.Absolute();
      Vector3D vrelative = this->velocity - bO->velocity;
      Vector3D vnormal = vrelative.Multiply(normal).Multiply(normal);

      this->velocity = this->velocity - vnormal;
      bO->velocity = bO->velocity + vnormal;
    }//else{//no collision}
  }
};

class BoundaryCube{
private:
  Quaternion previousRotation;
  Vector3D centerPosition;
  Vector3D maximum;
  Vector3D minimum;
  
public:
  Vector3D velocity;
  Vector3D position;
  
  BoundaryCube(Object3D* object){//calculates center position and object size
    this->centerPosition = object->GetVertexCenter();
    object->GetMinMaxDimensions(minimum, maximum);
  }
  
  BoundaryCube(Vector3D centerPosition, Vector3D objectSize){
    this->centerPosition = centerPosition;

    minimum = centerPosition - objectSize / 2.0f;
    maximum = centerPosition + objectSize / 2.0f;
  }

  Vector3D GetPosition(){
    return centerPosition;
  }
  
  Vector3D GetSize(){
    return maximum - minimum;
  }

  Vector3D GetMaximum(){
    return maximum;
  }

  Vector3D GetMinimum(){
    return minimum;
  }

  void Update(float dT, Vector3D acceleration, Quaternion rotation){
    Quaternion rotationChange = rotation.Multiply(previousRotation.MultiplicativeInverse());
    velocity = rotationChange.RotateVector(velocity) + acceleration * dT;//maintain momentum of existing velocity, apply local acceleration
    position = position + velocity * dT;

    previousRotation = rotation;
  }
  
  Vector3D IsIntersecting(BoundaryCube *bO){
    Vector3D collision;

    collision.X = maximum.X >= bO->GetMinimum().X ? 0 : 1;
    collision.X = bO->GetMaximum().X >= minimum.X ? 0 : -1;
    collision.Y = maximum.Y >= bO->GetMinimum().Y ? 0 : 1;
    collision.Y = bO->GetMaximum().Y >= minimum.Y ? 0 : -1;
    collision.Z = maximum.Z >= bO->GetMinimum().Z ? 0 : 1;
    collision.Z = bO->GetMaximum().Z >= minimum.Z ? 0 : -1;
    
    return collision;
  }
  
  Vector3D IsIntersecting(BoundarySphere *bO){
    Vector3D collision;
    Vector3D minimumExt, maximumExt;
    Vector3D sphereSize = Vector3D(bO->GetRadius(), bO->GetRadius(), bO->GetRadius());

    minimumExt = bO->GetPosition() - sphereSize / 2.0f;
    maximumExt = bO->GetPosition() + sphereSize / 2.0f;

    collision.X = maximum.X >= minimumExt.X ? 0 : 1;
    collision.X = minimumExt.X >= minimum.X ? 0 : -1;
    collision.Y = maximum.Y >= minimumExt.Y ? 0 : 1;
    collision.Y = minimumExt.Y >= minimum.Y ? 0 : -1;
    collision.Z = maximum.Z >= minimumExt.Z ? 0 : 1;
    collision.Z = minimumExt.Z >= minimum.Z ? 0 : -1;
    
    return collision;
  }
  
  void CollideSphere(float elasticity, BoundarySphere *bO){
    //if sphere is not within the bounds of the prism, collide
    Vector3D collision = IsIntersecting(bO);
    
    bO->velocity.X = collision.X == 0 ? bO->velocity.X : -bO->velocity.X * elasticity;
    bO->velocity.Y = collision.Y == 0 ? bO->velocity.Y : -bO->velocity.Y * elasticity;
    bO->velocity.Z = collision.Z == 0 ? bO->velocity.Z : -bO->velocity.Z * elasticity;
  }
};

class BoundaryMotionSimulator{
private:
  BoundaryCube* bC;
  BoundarySphere** bS;
  int sphereCount;
  float elasticity = 0.8f;
  
public:
  BoundaryMotionSimulator(Object3D** objects, int objectCount, BoundaryCube* bC, float radius, float elasticity){
    this->bC = bC;
    this->sphereCount = objectCount;
    this->elasticity = elasticity;

    bS = new BoundarySphere*[objectCount];
    
    for(int i = 0; i < objectCount; i++){
      bS[i] = new BoundarySphere(objects[i], radius);
    }
  }

  void Randomize(float range){//mm/s
    //randomize velocities
    for (int i = 0; i < sphereCount; i++){
      bS[i]->velocity.X = ((float)random(0, 999) / 1000.0f) * range - range / 2.0f;
      bS[i]->velocity.Y = ((float)random(0, 999) / 1000.0f) * range - range / 2.0f;
      bS[i]->velocity.Z = ((float)random(0, 999) / 1000.0f) * range - range / 2.0f;
    }
  }

  void Update(float dT, Vector3D acceleration, Quaternion rotation){
    //check if objects are colliding
    //object to object collisions
    for (int i = 0; i < sphereCount; i++){
      for (int j = 0; j < sphereCount; j++){
        if (i == j || i > j || j < i) break;//only above determinant line, only compare once
        
        bS[i]->Collide(elasticity, bS[j]);
      }
    }
    
    //box collisions
    for (int i = 0; i < sphereCount; i++){
      bS[i]->Update(dT, acceleration, rotation);
      bC->CollideSphere(elasticity, bS[i]);
    }
  }
};
