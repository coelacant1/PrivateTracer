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
  Vector2D rotationOffset;//point to rotate about
  float gradientPeriod;
  float rotationAngle;//rotate input xyPosition
  
public:
  GradientMaterial(uint8_t colorCount, RGBColor* rgbColors, float gradientPeriod){
    this->rgbColors = rgbColors;
    this->colorCount = colorCount;
    this->gradientPeriod = gradientPeriod;
  }

  //x 0->1 mapping all counts of colors, linearly interpolating

  //xy offset
  //rotation offset
  //gradient period

  void SetPositionOffset(Vector2D positionOffset){
    this->positionOffset = positionOffset;
  }
  
  void SetRotationOffset(Vector2D rotationOffset){
    this->rotationOffset = rotationOffset;
  }

  void SetRotationAngle(float rotationAngle){
    this->rotationAngle = rotationAngle;
  }
  
  
  RGBColor GetRGB(Vector2D xyPosition){
    if(rotationAngle != 0){
      Quaternion temp = Rotation(EulerAngles(Vector3D(0, 0, rotationAngle), EulerConstants::EulerOrderXYZS)).GetQuaternion();

      xyPosition = temp.RotateVector(xyPosition);
    }
    
    //from x position, fit into bucket ratio
    //modulo x value into x range from start position to end position
    float pos = fabs(fmodf(xyPosition.X + positionOffset.X, gradientPeriod));

    //map from modulo'd x value to color count minimum
    float ratio = Mathematics::Map(pos, 0, gradientPeriod, 0, colorCount);
    int startBox = floor(ratio);
    int endBox = floor(ratio) >= colorCount ? 0 : colorCount;
    float mu = Mathematics::Map(ratio, startBox, endBox, 0.0f, 1.0f);//calculate mu between boxes

    RGBColor rgb = RGBColor::InterpolateColors(rgbColors[startBox], rgbColors[endBox], mu);

    Serial.print(xyPosition.X);
    Serial.print(",");
    Serial.print(ratio);
    Serial.print(",");
    Serial.print(startBox);
    Serial.print(",");
    Serial.print(endBox);
    Serial.print(",");
    Serial.print(mu);
    Serial.println(",");
    
    return rgb;
  }
};
