#pragma once

#include "IndexPair.h"
#include "VecInt.h"
typedef int mT;

#define CLOSED_CELL_SYMB "  X "

class MatrixInt
{
private:
  int row = 0 , col = 0;
public:
  mT ** data;
  MatrixInt();
  MatrixInt(const MatrixInt& m);
  MatrixInt(int row, int col);
  MatrixInt& operator=(const MatrixInt& m);
  ~MatrixInt();

  void Print(const char * s) const;
  void PrintPlan(const char * s) const;
  void PrintPlanWithPot(const char * s, const MatrixInt &mC, const VecInt & vV, const VecInt &vU) const;
  

  mT& operator[](const IndexPair & cell);
  mT operator[](const IndexPair & cell) const;
  mT* operator[](size_t ind);
  const mT* operator[](size_t ind) const;

  int getRowCnt() const
  {
    return row;
  }
  int getColCnt()const
  {
    return col;
  }
};

