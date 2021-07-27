#pragma once

#include "Material.h"
#include "..\Math\Mathematics.h"
#include "..\Math\Vector2D.h"

//Converts gif to RGB XY pixel matrix
class GradientMaterial : public Material {
private:
    RGBColor* rgbColors;
    RGBColor* baseRGBColors;
    uint8_t colorCount;
    Vector2D positionOffset;
    Vector2D rotationOffset;//point to rotate about
    float gradientPeriod;
    float rotationAngle;//rotate input xyPosition
    bool isRadial;
  
public:
    GradientMaterial(uint8_t colorCount, RGBColor* rgbColors, float gradientPeriod, bool isRadial){
        this->colorCount = colorCount;
        this->gradientPeriod = gradientPeriod;
        this->isRadial = isRadial;

        this->rgbColors = new RGBColor[colorCount];
        this->baseRGBColors = new RGBColor[colorCount];

        for(int i = 0; i < colorCount; i++){
            this->rgbColors[i] = rgbColors[i];
            this->baseRGBColors[i] = rgbColors[i];
        }
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

    void SetGradientPeriod(float gradientPeriod){
        this->gradientPeriod = gradientPeriod;  
    }

    void HueShift(float hueDeg){
        for(int i = 0; i < colorCount; i++){
        rgbColors[i] = baseRGBColors[i].HueShift(hueDeg);
        }
    }
    
    RGBColor GetRGB(Vector3D position, Vector3D normal){
        if(rotationAngle != 0){
            Quaternion temp = Rotation(EulerAngles(Vector3D(0, 0, rotationAngle), EulerConstants::EulerOrderXYZS)).GetQuaternion();

            position = temp.RotateVector(position);
        }

        float pos = 0;
        position = position - Vector3D(positionOffset.X, positionOffset.Y, 0);
        
        if (isRadial){
            pos = sqrtf(position.X * position.X + position.Y * position.Y);
            pos = fabs(fmodf(pos, gradientPeriod));
        }
        else{
            //from x position, fit into bucket ratio
            //modulo x value into x range from start position to end position
            pos = fabs(fmodf(position.X, gradientPeriod));
        }
        
        
        //map from modulo'd x value to color count minimum
        float ratio = Mathematics::Map(pos, 0, gradientPeriod, 0, colorCount);
        int startBox = floor(ratio);
        int endBox = startBox + 1 >= colorCount ? 0 : startBox + 1;

        float mu = Mathematics::Map(ratio, startBox, startBox + 1, 0.0f, 1.0f);//calculate mu between boxes

        RGBColor rgb = RGBColor::InterpolateColors(rgbColors[startBox], rgbColors[endBox], mu);

        return rgb;
    }
};
