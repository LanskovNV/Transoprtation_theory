#include "MatrixInt.h"

#include <iostream>
#include <iomanip>

using namespace std;
MatrixInt::MatrixInt()
{
}
MatrixInt::MatrixInt(const MatrixInt & m)
{
  this->row = m.getRowCnt();
  this->col = m.getColCnt();

  data = new mT*[row];
  for (int rI = 0; rI < row; ++rI)
  {
    data[rI] = new mT[col];
    for (int cI = 0; cI < col; ++cI)
      data[rI][cI] = m[rI][cI];
  }
}

MatrixInt::MatrixInt(int row_, int col_)
{
  row = row_;
  col = col_;
  data = new mT*[row];
  for (int rI = 0; rI < row; ++rI)
  {
    data[rI] = new mT[col];
    for (int cI = 0; cI < col; ++cI)
      data[rI][cI] = 0;
  }
}

MatrixInt & MatrixInt::operator=(const MatrixInt & m)
{
  if (row != m.getRowCnt() || col != m.getColCnt())
  {
    // if matrix has same size
    row = m.getRowCnt();
    col = m.getColCnt();

    for (int rI = 0; rI < row; ++rI)
      delete[] data[rI];
    delete[] data;

    data = new mT*[row];
    for (int rI = 0; rI < row; ++rI)
    {
      data[rI] = new mT[col];
      for (int cI = 0; cI < col; ++cI)
        data[rI][cI] = m[rI][cI];
    }
  }
  else
  {
    for (int rI = 0; rI < row; ++rI)
    {
      for (int cI = 0; cI < col; ++cI)
        data[rI][cI] = m[rI][cI];
    }
  }

  return (*this);
}

MatrixInt::~MatrixInt()
{
  for (int rI = 0; rI < row; ++rI)
    delete[] data[rI];
  delete[] data;
}



void MatrixInt::Print(const char * s) const
{
  cout << s << endl;
  for (int iRow = 0; iRow < row; iRow++)
  {
    for (int iCol = 0; iCol < col; iCol++)
    {
      cout << setw(5) << data[iRow][iCol] << " ";
    }
    cout << endl;
  }
}

void MatrixInt::PrintPlan(const char * s) const
{
  std::cout << s << std::endl;
  for (int iRow = 0; iRow < row; iRow++)
  {
    for (int iCol = 0; iCol < col; iCol++)
    {
      if (data[iRow][iCol] == -1)
        cout << CLOSED_CELL_SYMB;
      else
        cout << setw(3) << data[iRow][iCol] << " ";
    }
    cout << endl;
  }
}

void MatrixInt::PrintPlanWithPot(const char * s, const MatrixInt &mC, const VecInt & vV, const VecInt &vU) const
{
  cout << s << endl;
  for (int iRow = 0; iRow < row; iRow++)
  {
    for (int iCol = 0; iCol < col; iCol++)
    {
      if (data[iRow][iCol] == -1)
        cout << CLOSED_CELL_SYMB;
      else
        cout << setw(3) << mC[iRow][iCol] << " ";
    }
    cout << setw(3) << vU[iRow] << " ";
    cout << endl;
  }
  for (int iCol = 0; iCol < col; iCol++)
  {
    cout << setw(3) << vV[iCol] << " ";
  }
  cout << endl;
}

mT & MatrixInt::operator[](IndexPair & cell)
{
  return data[cell.iRow][cell.iCol];
}

mT * MatrixInt::operator[](size_t ind)
{
  return data[ind];
}

const mT * MatrixInt::operator[](size_t ind) const
{
  return data[ind];
}

