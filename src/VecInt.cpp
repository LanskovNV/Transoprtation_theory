#include "VecInt.h"
#include <iostream>
#include <iomanip>
using namespace std;
VecInt::VecInt()
{
}

VecInt::VecInt(int size_)
{
  data = new mT[size_];
  for (int i = 0; i < size_; ++i) {
    data[i] = 0;
  }
  size = size_;
}

VecInt::VecInt(const VecInt & v)
{
  size = v.getSize();
  data = new mT[size];
  for (int i = 0; i < size; ++i)
    data[i] = v[i];
}

VecInt & VecInt::operator=(const VecInt & v)
{
  if (size != v.getSize())
  {
    delete[] data;
    size = v.getSize();
    data = new mT[size];
  }

  for (int i = 0; i < size; ++i)
    data[i] = v[i];
  return (*this);
}

VecInt::~VecInt()
{
  delete[] data;
}

mT & VecInt::operator[](int ind)
{
  return data[ind];
}

mT VecInt::operator[](int ind) const
{
  return data[ind];
}


void VecInt::Print(const char * s) const
{
  cout << s << endl;
  for (int iRow = 0; iRow < size; iRow++)
  {
    cout << setw(5) << data[iRow] << endl;
  }
}


mT operator*(const VecInt & v1, const VecInt & v2)
{
  if (v1.getSize() != v2.getSize())
  {
    cout << "VEC mul VEC DIMENSION ERROR" << endl;
    return 0;
  }

  mT sum = 0;
  for (int i = 0; i < v1.getSize(); ++i)
    sum += v1[i] * v2[i];
  return sum;
}
