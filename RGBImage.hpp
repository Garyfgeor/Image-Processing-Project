#ifndef RGBIMAGE_H
#define RGBIMAGE_H
#include <iostream>
#include "Image.hpp"
#include "Pixel.hpp"
#include "RGBPixel.hpp"
using namespace std;

class RGBImage : public Image {
private : 
  RGBPixel** rgb_array;
public:
  RGBImage(); 
  RGBImage(const RGBImage& img);
  RGBImage(istream& stream);
  ~RGBImage();

  RGBPixel** get_rgb_array() const;
  RGBImage& operator = (const RGBImage& img);
   
  virtual Image& operator += (int ) override ;
  virtual Image& operator *= (double factor) override;
  virtual Image& operator !() override;
  virtual Image& operator ~() override;
  virtual Image& operator *() override;
  
  virtual Pixel& getPixel(int row, int col) const override;
  friend ostream& operator << (ostream& out, Image& image);
  virtual int clip(int num) override;
  void free_array(RGBPixel** array, int height);
};
#endif
