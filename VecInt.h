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
  VecInt(int size);
  VecInt(const VecInt &v);

  mT& operator[](int ind);

  mT VecVec(VecInt& v);

  void Print(const char * s);
  //void Print(const char * s, IndexSet & rowSet);

   int getSize() const {
    return size;
  }

};

