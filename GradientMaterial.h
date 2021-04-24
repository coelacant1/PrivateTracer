#pragma once

#include "Material.h"
#include "Mathematics.h"
#include "Vector2D.h"

//Converts gif to RGB XY pixel matrix
class GradientMaterial : public Material {
private:
  RGBColor* rgbColors;
  uint8_t colorCount;
  Vector2D positionOffset;
  Vector2D rotationPoint;//point to rotate about
  float gradientPeriod;
  float rotationOffset;//rotate input xyPosition
  
public:
  GradientMaterial(uint8_t colorCount, RGBColor* rgbColors){
    this->rgbColors = rgbColors;
    this->colorCount = colorCount;
  }

  //x 0->1 mapping all counts of colors, linearly interpolating

  //xy offset
  //rotation offset
  //gradient period
  
  
  
  RGBColor GetRGB(Vector2D xyPosition){
    //from x position, fit into bucket ratio
    //modulo x value into x range from start position to end position
    float pos = fmod(xyPosition.X + positionOffset.X, gradientPeriod);

    //map from modulo'd x value to color count minimum
    float ratio = Mathematics::Map(pos, 0, gradientPeriod, 0, colorCount);
    int startBox = floor(ratio);
    int endBox = floor(ratio) >= colorCount ? 0 : colorCount;
    float mu = Mathematics::Map(ratio, startBox, endBox, 0.0f, 1.0f);//calculate mu between boxes

    RGBColor::InterpolateColors(rgbColors[startBox], rgbColors[endBox], mu);
    
    return rgb;
  }
};
