#include "VecInt.h"
using namespace std;
#include <iostream>
#include <iomanip>
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
    data[i] = v.data[i];
}

mT & VecInt::operator[](int ind)
{
  return data[ind];
}




mT VecInt::VecVec(VecInt & v)
{
  mT sum = 0;
  for (int i = 0; i < size; ++i)
    sum += data[i] * v.data[i];
  return sum;
}

void VecInt::Print(const char * s)
{
  std::cout << s << std::endl;
  for (int iRow = 0; iRow < size; iRow++)
  {
    std::cout << std::setw(5) << data[iRow] << endl;
  }
}

//void VecInt::Print(const char * s, IndexSet & rowSet)
//{
//  std::cout << s << std::endl;
//  for (auto iRow : rowSet.data)
//  {
//    std::cout << std::setw(5) << data[iRow] << endl;
//  }
//}

