#pragma once

//#include "IndexSet.h"
#include "IndexPair.h"
#include "VecInt.h"
typedef int mT;
class MatrixInt
{
private:
  int row, col;
public:
  mT ** data;
  MatrixInt(const MatrixInt& m);
  MatrixInt(int row, int col);


  void Print(const char * s);
  void PrintPlan(const char * s);
  void PrintPlanWithPot(const char * s, MatrixInt& mC, VecInt& vV, VecInt &vU);
  //void Print(const char * s, IndexSet & ColSet);

  mT& operator[](IndexPair & cell);

  int getRowCnt() const
  {
    return row;
  }
  int getColCnt()const
  {
    return col;
  }
};

