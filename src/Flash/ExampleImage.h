#pragma once

#include "..\Materials\Image.h"

class ExampleImage : public Image{
private:
    uint8_t rgbMemory[48] = {};

public:
    ExampleImage(Vector2D size, Vector2D offset) : Image(size, offset) {
        rgbData = &rgbMemory[0];
    }
};
