#pragma once

#include "..\Math\Vector2D.h"

class BoundingBox2D{
private:
    Vector2D min;
    Vector2D max;

public:
    BoundingBox2D(){}

    void UpdateBounds(Vector2D current){
        min = min.Minimum(current);
        max = max.Maximum(current);
    }
    
    Vector2D GetMinimum(){
        return min;
    }

    Vector2D GetMaximum(){
        return max;
    }
};
