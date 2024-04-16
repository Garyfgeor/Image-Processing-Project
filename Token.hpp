#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <fstream>
#include "Image.hpp"
using namespace std;

class Token {
private:
  string name;
  Image* ptr;
public:
  Token(const string& ="", Image* = nullptr);
  Token(const Token& token);
  ~Token();
  string getName() const;
  Image* getPtr() const;
  void setName(const string& );
  void setPtr(Image* ptr);
};


#endif
