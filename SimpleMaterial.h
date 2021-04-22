#pragma once

#include "Material.h"
#include "Vector2D.h"

//Converts gif to RGB XY pixel matrix
class SimpleMaterial : public Material {
private:
  RGBColor rgb;
  
public:
  SimpleMaterial(RGBColor rgb){
    this->rgb = rgb;
  }
  
  RGBColor GetRGB(Vector2D xyPosition){
    return rgb;
  }
};
