#include <iostream>
#include "Token.hpp"
#include "Image.hpp"
#include "RGBImage.hpp"
#include "GSCImage.hpp"
#include "RGBPixel.hpp"
#include "GSCPixel.hpp"

Token::Token(const string& str, Image* img){
    this->name = str;
    this->ptr = img;
}

Token::Token(const Token& token){
    if(dynamic_cast<RGBImage*>(token.ptr) != nullptr){//egxrwmh
        this->name = token.name;
        RGBImage* rgb = dynamic_cast<RGBImage*>(token.ptr);
        this->ptr = new RGBImage(*rgb);
    }
    else if(dynamic_cast<GSCImage*>(token.ptr) != nullptr){//aspromavri
        this->name = token.name;
        GSCImage* gsc = dynamic_cast<GSCImage*>(token.ptr);
        this->ptr = new GSCImage(*gsc);
    }
}

Token::~Token(){
    delete this->ptr;
}
string Token::getName() const{
    return this->name;
}
Image* Token::getPtr() const{
    return this->ptr;
}
void Token::setName(const string& name){
    this->name = name;
}
void Token::setPtr(Image* ptr){
    this->ptr = ptr;
}
