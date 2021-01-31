#include "Object3D.h"
#include "Vector2D.h"

class Eye{
public:
  enum Emotion {
      Neutral,
      Sleepy,
      Surprised,
      Happy,
      Angry,
      Sad
  };
  
private:
  Vector3D center = Vector3D();
  Vector2D current[4] = {Vector2D(0, 0), Vector2D(50, 50), Vector2D(50, 0), Vector2D(100, 0)};
  Vector2D neutral[4] = {Vector2D(0, 0), Vector2D(50, 50), Vector2D(50, 0), Vector2D(100, 0)};
  Vector2D sleepy[4] = {Vector2D(0, 30), Vector2D(50, 10), Vector2D(50, 0), Vector2D(100, 10)};
  Vector2D surprised[4] = {Vector2D(0, 25), Vector2D(25, 50), Vector2D(50, 0), Vector2D(100, 25)};
  Vector2D happy[4] = {Vector2D(0, 0), Vector2D(50, 50), Vector2D(50, 25), Vector2D(100, 0)};
  Vector2D angry[4] = {Vector2D(0, 0), Vector2D(50, 25), Vector2D(100, 0), Vector2D(100, 50)};
  Vector2D sad[4] = {Vector2D(0, 10), Vector2D(50, 10), Vector2D(0, 0), Vector2D(100, 0)};
  
  Object3D eye = Object3D(4, 2);
  Quaternion offsetRotation = Rotation(EulerAngles(Vector3D(25, 0, 0), EulerConstants::EulerOrderXYZR)).GetQuaternion();
  Vector3D offsetPosition = Vector3D(-40, 130, 0);

  Vector2D* GetEmotion(Emotion e){
    switch (e){
      case Sleepy:
        return sleepy;
      case Surprised:
        return surprised;
      case Happy:
        return happy;
      case Angry:
        return angry;
      case Sad:
        return sad;
      default:
        return neutral;
    }
  }

  void Interpolate(Emotion e, float rate){
    rate = Mathematics::Constrain(rate, 0.0f, 1.0f);
    
    for (int i = 0; i < 4; i++){
      current[i] = Vector2D::LERP(current[i], GetEmotion(e)[i], rate);
    }
    
    for (int i = 0; i < 4; i++){
      eye.GetVertices()[i].X = -current[i].X;
      eye.GetVertices()[i].Y = current[i].Y;
      eye.GetVertices()[i].Z = 0;
      
      eye.GetVerticesOriginal()[i].X = -current[i].X;
      eye.GetVerticesOriginal()[i].Y = current[i].Y;
      eye.GetVerticesOriginal()[i].Z = 0;
    }
  }

  void LinkEye(){
    eye.GetTriangles()[0].p1 = &(eye.GetVertices()[0]);
    eye.GetTriangles()[0].p2 = &(eye.GetVertices()[1]);
    eye.GetTriangles()[0].p3 = &(eye.GetVertices()[2]);
    
    eye.GetTriangles()[1].p1 = &(eye.GetVertices()[1]);
    eye.GetTriangles()[1].p2 = &(eye.GetVertices()[2]);
    eye.GetTriangles()[1].p3 = &(eye.GetVertices()[3]);
  }
  
public:
  Eye(){
    LinkEye();
  }

  Object3D* GetObject(){
    return &eye;
  }

  void Update(Emotion e, float rate){
    Interpolate(e, rate);

    eye.Rotate(offsetRotation);
    eye.Move(offsetPosition);
  }
};
