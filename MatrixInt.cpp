#include "MatrixInt.h"
#include <iostream>
#include <iomanip>
using namespace std;
MatrixInt::MatrixInt(const MatrixInt & m)
{
  this->row = m.getRowCnt();
  this->col = m.getColCnt();

  data = new mT*[row];
  for (int rI = 0; rI < row; ++rI)
  {
    data[rI] = new mT[col];
    for (int cI = 0; cI < col; ++cI)
      data[rI][cI] = m.data[rI][cI];
  }
}

MatrixInt::MatrixInt(int row, int col)
{
  data = new mT*[row];
  for (int rI = 0; rI < row; ++rI)
  {
    data[rI] = new mT[col];
    for (int cI = 0; cI < col; ++cI)
      data[rI][cI] = 0;
  }
  this->row = row;
  this->col = col;
}



void MatrixInt::Print(const char * s)
{
  std::cout << s << std::endl;
  for (int iRow = 0; iRow < row; iRow++)
  {
    for (int iCol = 0; iCol < col; iCol++)
    {
      std::cout << std::setw(5) << data[iRow][iCol] << " ";
    }
    std::cout << "" << std::endl;
  }
}

void MatrixInt::PrintPlan(const char * s)
{
  std::cout << s << std::endl;
  for (int iRow = 0; iRow < row; iRow++)
  {
    for (int iCol = 0; iCol < col; iCol++)
    {
      if (data[iRow][iCol] == -1)
        std::cout << " X  ";
      else
        std::cout << std::setw(3) << data[iRow][iCol] << " ";
    }
    std::cout << "" << std::endl;
  }
}

void MatrixInt::PrintPlanWithPot(const char * s, MatrixInt &mC, VecInt & vV, VecInt &vU)
{
  std::cout << s << std::endl;
  for (int iRow = 0; iRow < row; iRow++)
  {
    for (int iCol = 0; iCol < col; iCol++)
    {
      if (data[iRow][iCol] == -1)
        std::cout << " X  ";
      else
        std::cout << std::setw(3) << mC.data[iRow][iCol] << " ";
    }
    std::cout << std::setw(3) << vU[iRow] << " ";
    std::cout << "" << std::endl;
  }
  for (int iCol = 0; iCol < col; iCol++)
  {
    std::cout << std::setw(3) << vV[iCol] << " ";
  }
  std::cout << "" << std::endl;
}

mT & MatrixInt::operator[](IndexPair & cell)
{
  return data[cell.iRow][cell.iCol];
}

//void MatrixInt::Print(const char * s, IndexSet & ColSet)
//{
//  std::cout << s << std::endl;
//  for (int iRow = 0; iRow < row; iRow++)
//  {
//    for (auto iCol : ColSet.data)
//    {
//      std::cout << std::setw(5) << data[iRow][iCol] << " ";
//    }
//    std::cout << "" << std::endl;
//  }
//}