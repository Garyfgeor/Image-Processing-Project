#ifndef GSCIMAGE_H
#define GSCIMAGE_H
#include <iostream>
#include "RGBImage.hpp"
#include "Image.hpp"
#include "Pixel.hpp"
#include "GSCPixel.hpp"
using namespace std;

class GSCImage : public Image {
private : 
GSCPixel** gsc_array;
public:
  GSCImage();
  GSCImage(const GSCImage& img);
  GSCImage(const RGBImage& grayscaled);
  GSCImage(istream& stream);
  ~GSCImage();
  GSCPixel** get_gsc_array() const;
  GSCImage& operator = (const GSCImage& img);  
    
  virtual Image& operator += (int ) override ;
  virtual Image& operator *= (double factor) override;
  virtual Image& operator !() override;
  virtual Image& operator ~  () override;
  virtual Image& operator *() override;
  virtual Pixel& getPixel(int row, int col) const override;
  friend ostream& operator << (ostream& out, Image& image) ;
  virtual int clip(int num) override;
  void free_array(GSCPixel** array, int height);
};
#endif
