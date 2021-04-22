#pragma once

#include "RGBColor.h"
#include "Vector2D.h"

class Material{
public:
  virtual RGBColor GetRGB(Vector2D xyPosition) = 0;
  
};
