#pragma once

#include "Material.h"
#include "..\Math\Vector2D.h"

//Converts gif to RGB XY pixel matrix
class SimpleMaterial : public Material {
private:
    RGBColor rgb;
    RGBColor baseRGB;
  
public:
    SimpleMaterial(RGBColor rgb){
        this->rgb = rgb;
        this->baseRGB = rgb;
    }

    void HueShift(float hueDeg){
        rgb = baseRGB.HueShift(hueDeg);
    }
    
    RGBColor GetRGB(Vector2D xyPosition){
        return rgb;
    }
};
