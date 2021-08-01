#pragma once

#include "Material.h"
#include "..\Math\Vector2D.h"
#include "GradientMaterial.h"
#include "Arduino.h"

//Converts gif to RGB XY pixel matrix
class DepthMaterial : public Material {
private:
    RGBColor spectrum[4] = {RGBColor(0, 255, 0), RGBColor(255, 0, 0), RGBColor(0, 255, 0), RGBColor(0, 0, 255)};
    GradientMaterial gNoiseMat = GradientMaterial(4, spectrum, 2.0f, false);
    float depth = 0.0f;
    float zOffset = 0.0f;
  
public:
    DepthMaterial(float depth, float zOffset){
        this->depth = depth;
        this->zOffset = zOffset;
    }
    
    RGBColor GetRGB(Vector3D position, Vector3D normal){
        float pos = Mathematics::Map(position.Z, -depth / 2.0f + zOffset, depth / 2.0f + zOffset, 0.0f, 1.0f);
        
        return gNoiseMat.GetRGB(Vector3D(pos, 0, 0), Vector3D());
    }
};
