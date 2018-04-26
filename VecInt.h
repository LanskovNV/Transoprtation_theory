#pragma once

#include <array>
#include <list>
typedef int mT;


class VecInt
{
private:
  int size;
public:
  mT * data;
  VecInt();
  VecInt(int size);
  VecInt(const VecInt &v);
  VecInt& operator=(const VecInt &v);
  ~VecInt();

  mT& operator[](int ind);
  mT operator[](int ind) const;

  void Print(const char * s) const;

   int getSize() const {
    return size;
  }

};

mT operator*(const VecInt& v1, const VecInt& v2);