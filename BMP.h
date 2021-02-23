#pragma once

#include "Pixel.h"
#include "Vector2D.h"

//Converts gif to RGB XY pixel matrix
class BMP{
private:
  Vector2D imageSize;
  Vector2D offsetPosition;
  uint8_t* file;
  int xPixels;
  int yPixels;
  long imagePixels;
  const int displayWidth = 320;

  RGBColor ConvertRGB565(uint16_t color){
    
  }
  
public:
  BMP(Vector2D imageSize, Vector2D offsetPosition, const uint8_t* bmpArray){
    this->file = bmpArray;
    Serial.println("READING BMP FILE");
    this->imageSize = imageSize;
    this->offsetPosition = offsetPosition;

    xPixels = *(int*)&file[18];
    yPixels = *(int*)&file[22];

    imagePixels = 3 * xPixels * yPixels;
    /*
    int xPos = 0;
    int yPos = 0;
    
    for(int i = 54; i < imagePixels + 54; i += 3){
      //pixels[i - 54].X = Mathematics::Map(xPos, 0, xPixels, 0 + offsetPosition.X, imageSize.X + 1 + offsetPosition.X);
      //pixels[i - 54].Y = Mathematics::Map(yPos, 0, yPixels, 0 + offsetPosition.Y, imageSize.Y + 1 + offsetPosition.Y);
      //pixels[i - 54].Color = RGBColor(file[i + 2], file[i + 1], file[i]);

      Serial.print(file[i + 2]);
      Serial.print(" ");
      Serial.print(file[i + 1]);
      Serial.print(" ");
      Serial.print(file[i]);
      Serial.print(" ");
      Serial.print(xPos);
      Serial.print(" ");
      Serial.println(yPos);
      
      xPos++;
      if(xPos >= xPixels){
        xPos = 0;
        yPos++;
      }
    }
    */
  }

  //maps XY position to an rgb value on the 
  RGBColor GetRGB(Vector2D xyPosition){
    //convert xyposition to integer position on image plane
    int xPosition, yPosition;

    xPosition = Mathematics::Map(xyPosition.X, offsetPosition.X, imageSize.X + 1 + offsetPosition.X, 0, xPixels);//scale to fit image size
    yPosition = Mathematics::Map(xyPosition.Y, offsetPosition.Y, imageSize.Y + 1 + offsetPosition.Y, 0, yPixels);
    
    Serial.print(xyPosition.ToString());
    Serial.print(" ");
    Serial.print(xPosition);
    Serial.print(" ");
    Serial.print(yPosition);
    
    //map xy to to imagesize with offset position
    if (xPosition < 0 || xPosition > xPixels) return RGBColor(0, 0, 0);
    if (yPosition < 0 || yPosition > yPixels) return RGBColor(0, 0, 0);


    long pixelStart = (xPosition + 1) + (xPixels + 1) * yPosition;
    Serial.print(" ");
    Serial.println(pixelStart);
    
    return RGBColor(file[pixelStart + 2], file[pixelStart + 1], file[pixelStart]);
  }
};
