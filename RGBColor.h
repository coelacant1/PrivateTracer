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
    
    String ToString(){
      String r = String(this->R);
      String g = String(this->G);
      String b = String(this->B);
      
      return "[" + r + ", " + g + ", " + b + "]";
    }
};
