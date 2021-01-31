#include "Object3D.h"

class ObjectDeformer{
  private:
    Object3D** objects;
    int objectCount = 0;
    
  public:
    enum Axis {
        XAxis,
        YAxis,
        ZAxis
    };
    
    ObjectDeformer(Object3D* object){
      objects = new Object3D*[1];
      
      objects[0] = object;
      
      objectCount = 1;
    }

    ObjectDeformer(Object3D** objects, int objectCount){
      this->objects = objects;
      this->objectCount = objectCount;
    }

    void SinusoidalDeform(float magnitude, float timeRatio, float periodModifier, float frequencyModifier, Axis axis){
      for(int i = 0; i < objectCount; i++){
        for(int j = 0; j < objects[i]->GetVertexAmount(); j++){
          Vector3D base = objects[i]->GetVertices()[j];
          
          switch(axis){
            case XAxis:
              objects[i]->GetVertices()[j].X = (sinf((base.Y) + timeRatio * frequencyModifier) * periodModifier + cosf((base.Z) + timeRatio * frequencyModifier) * periodModifier) * magnitude;
              break;
            case YAxis:
              objects[i]->GetVertices()[j].Y = (sinf((base.X) + timeRatio * frequencyModifier) * periodModifier + cosf((base.Z) + timeRatio * frequencyModifier) * periodModifier) * magnitude;
              break;
            case ZAxis:
              objects[i]->GetVertices()[j].Z = (sinf((base.X) + timeRatio * frequencyModifier) * periodModifier + cosf((base.Y) + timeRatio * frequencyModifier) * periodModifier) * magnitude;
              break;
            default:
              break;
          }
        }
      }
    }
    
    void DropwaveDeform(float magnitude, float timeRatio, float periodModifier, float frequencyModifier, Axis axis){
      for(int i = 0; i < objectCount; i++){
        for(int j = 0; j < objects[i]->GetVertexAmount(); j++){
          Vector3D base = objects[i]->GetVertices()[j];
          
          switch(axis){
            case XAxis:
              objects[i]->GetVertices()[j].X = -(1.0f + cosf(12.0f*sqrt(base.Y * base.Y + base.Z + base.Z) + timeRatio * frequencyModifier) * periodModifier) / (0.5f * (base.Y * base.Y + base.Z + base.Z) + 2.0f) * magnitude;
              break;
            case YAxis:
              objects[i]->GetVertices()[j].Y = -(1.0f + cosf(12.0f*sqrt(base.X * base.X + base.Z + base.Z) + timeRatio * frequencyModifier) * periodModifier) / (0.5f * (base.X * base.X + base.Z + base.Z) + 2.0f) * magnitude;
              break;
            case ZAxis:
              objects[i]->GetVertices()[j].Z = -(1.0f + cosf(12.0f*sqrt(base.X * base.X + base.Y * base.Y) + timeRatio * frequencyModifier) * periodModifier) / (0.5f * (base.X * base.X + base.Y * base.Y) + 2.0f) * magnitude;
              break;
            default:
              break;
          }
        }
      }
    }

    void SineWaveSurfaceDeform(Vector3D offset, float magnitude, float timeRatio, float periodModifier, float frequencyModifier, Axis axis){
      for(int i = 0; i < objectCount; i++){
        for(int j = 0; j < objects[i]->GetVertexAmount(); j++){
          Vector3D base = objects[i]->GetVertices()[j] - offset;
        
          switch(axis){
            case XAxis:
              objects[i]->GetVertices()[j].X = objects[i]->GetVertices()[j].X + sinf((sqrtf(base.Y * base.Y + base.Z * base.Z) + timeRatio * frequencyModifier) * periodModifier) * magnitude;
              break;
            case YAxis:
              objects[i]->GetVertices()[j].Y = objects[i]->GetVertices()[j].Y + sinf((sqrtf(base.X * base.X + base.Z * base.Z) + timeRatio * frequencyModifier) * periodModifier) * magnitude;
              break;
            case ZAxis:
              objects[i]->GetVertices()[j].Z = objects[i]->GetVertices()[j].Z + sinf((sqrtf(base.X * base.X + base.Y * base.Y) + timeRatio * frequencyModifier) * periodModifier) * magnitude;
              break;
            default:
              break;
          }
        }
      }
    }
};
