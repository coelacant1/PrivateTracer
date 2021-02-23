#pragma once

class RGBColor{
  private:

  public:
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;

    RGBColor(){
      
    }

    RGBColor(uint8_t R, uint8_t G, uint8_t B){
      this->R = R;
      this->G = G;
      this->B = B;
    }
    
    RGBColor(const RGBColor& rgbColor){
      this->R = rgbColor.R;
      this->G = rgbColor.G;
      this->B = rgbColor.B;
    }

    RGBColor Scale(uint8_t maxBrightness){
      int sR, sG, sB;
      
      sR = (int)R * (int)maxBrightness / 255;
      sG = (int)G * (int)maxBrightness / 255;
      sB = (int)B * (int)maxBrightness / 255;
      
      sR = sR > 255 ? 255 : sR;
      sG = sG > 255 ? 255 : sG;
      sB = sB > 255 ? 255 : sB;
      
      sR = sR < 0 ? 0 : sR;
      sG = sG < 0 ? 0 : sG;
      sB = sB < 0 ? 0 : sB;
      
      return RGBColor(sR, sG, sB);
    }
    
    String ToString(){
      String r = String(this->R);
      String g = String(this->G);
      String b = String(this->B);
      
      return "[" + r + ", " + g + ", " + b + "]";
    }
};
