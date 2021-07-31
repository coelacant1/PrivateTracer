#pragma once

#include "BoundingBox2D.h"
#include "Pixel.h"

class PixelGroup{
public:
    enum Direction{
        ZEROTOMAX,
        MAXTOZERO
    };

private:
    const unsigned int pixelCount;
    BoundingBox2D bounds;
    Direction direction;
    Pixel** pixels;

public:
    PixelGroup(Vector2D* pixelLocations, unsigned int pixelCount, Direction direction = ZEROTOMAX) : pixelCount(pixelCount){
        this->direction = direction;

        pixels = new Pixel*[pixelCount];

        if(direction == ZEROTOMAX){
            for(unsigned int i = 0; i < pixelCount; i++){
                pixels[i] = new Pixel(&pixelLocations[i]);
                bounds.UpdateBounds(pixelLocations[i]);
            }
        }
        else{
            for(unsigned int i = 0; i < pixelCount; i++){
                pixels[i] = new Pixel(&pixelLocations[pixelCount - i]);
                bounds.UpdateBounds(pixelLocations[i]);
            }
        }
    }

    ~PixelGroup(){
        for (unsigned int i = 0; i < pixelCount; i++){
            delete pixels[i];
        }

        delete pixels;
    }

    Pixel* GetPixel(unsigned int count){
        count = Mathematics::Constrain(count, 0, pixelCount);

        return pixels[count];
    }

    unsigned int GetPixelCount(){
        return pixelCount;
    }

    bool ContainsVector2D(Transform *t, Vector2D v){
        /*
        //rotate and move bounding box to transform
         Serial.print(v.ToString());
         Serial.print("\t");
         Serial.print(bounds.GetMinimum().ToString());
         Serial.print("\t");
         Serial.println(bounds.GetMaximum().ToString());
         delay(50);
        */

        return v.CheckBounds(bounds.GetMinimum(), bounds.GetMaximum());
    }
};