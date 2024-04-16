#ifndef RGBPIXEL_H
#define RGBPIXEL_H
#include <iostream>
#include "Pixel.hpp"
#define MAX_LUMINOCITY 255
using namespace std;

class RGBPixel : public Pixel {

private:
  unsigned char red;
  unsigned char green;
  unsigned char blue;
public:
  RGBPixel() = default;
  RGBPixel(const RGBPixel& p);
  RGBPixel(unsigned char r, unsigned char g, unsigned char b);
  unsigned char getRed() const;
  unsigned char getGreen() const;
  unsigned char getBlue() const;
  void setRed(int r);
  void setGreen(int g);
  void setBlue(int b);
};
#endif

