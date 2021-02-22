#include "Pixel.h"
#include "Vector2D.h"

//Converts gif to RGB XY pixel matrix
class BMP{
private:
  Vector2D imageSize;
  Vector2D offsetPosition;
  Pixel* pixels;
  int xPixels;
  int yPixels;
  const int displayWidth = 320;

  RGBColor ConvertRGB565(uint16_t color){
    
  }
  
public:
  BMP(Vector2D imageSize, Vector2D offsetPosition, uint8_t* file){
    Serial.println("READING BMP FILE");
    this->imageSize = imageSize;
    this->offsetPosition = offsetPosition;

    xPixels = *(int*)&file[18];
    yPixels = *(int*)&file[22];
    
    pixels = new Pixel[xPixels * yPixels];

    int imgSize = 3 * xPixels * yPixels;

    int xPos = 0;
    int yPos = 0;

    for(int i = 54; i < imgSize + 54; i += 3){
      pixels[i - 54].X = Mathematics::Map(xPos, 0, xPixels, 0 + offsetPosition.X, imageSize.X + 1 + offsetPosition.X);
      pixels[i - 54].Y = Mathematics::Map(yPos, 0, yPixels, 0 + offsetPosition.Y, imageSize.Y + 1 + offsetPosition.Y);
      pixels[i - 54].Color = RGBColor(file[i + 2], file[i + 1], file[i]);

      Serial.print(pixels[i - 54].X);
      Serial.print(" ");
      Serial.print(pixels[i - 54].Y);
      Serial.print(" ");
      Serial.print(pixels[i - 54].Color.ToString());
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
  }

  //maps XY position to an rgb value on the 
  RGBColor GetRGB(Vector2D xyPosition){
    //map xy to to imagesize with offset position
    
    
    return RGBColor();
  }
};
