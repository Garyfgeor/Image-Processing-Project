#ifndef GSCPIXEL_H
#define GSCPIXEL_H
#include <iostream>
#include "Pixel.hpp"
using namespace std;

class GSCPixel : public Pixel {
private:
  unsigned char value;
public:
  GSCPixel() = default;
  GSCPixel(const GSCPixel& p);
  GSCPixel(unsigned char value);
  unsigned char getValue();
  void setValue(unsigned char value);
};
#endif

