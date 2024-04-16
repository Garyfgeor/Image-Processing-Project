#include <iostream>
#include "GSCPixel.hpp"

GSCPixel::GSCPixel(const GSCPixel& p){
    this->value = p.value;
}

GSCPixel::GSCPixel(unsigned char value){
    this->value = value;
}

unsigned char GSCPixel::getValue(){
    return this->value;
}
  
void GSCPixel::setValue(unsigned char value){
    this->value = value;
}