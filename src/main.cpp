#include <iostream>
#include <fstream>

#include "TransportationTheorySolver.h"

using namespace std;
  int main(void)
  {
    ifstream mCstream("tasks/transp_mC.txt");
    ifstream vAstream("tasks/transp_vA.txt");
    ifstream vBstream("tasks/transp_vB.txt");

    int mCrows, mCcols;
    mCstream >> mCrows;
    mCstream >> mCcols;
    MatrixInt mC = MatrixInt(mCrows, mCcols);
    for (int iRow = 0; iRow < mCrows; ++iRow)
      for (int iCol = 0; iCol < mCcols; ++iCol)
        mCstream >> mC[iRow][iCol];
    mCstream.close();

    int vArows;
    vAstream >> vArows;
    VecInt vA = VecInt(vArows);
    for (int iRow = 0; iRow < vArows; ++iRow)
        vAstream >> vA[iRow];
    vAstream.close();

    int vBRows;
    vBstream >> vBRows;
    VecInt vB = VecInt(vBRows);
    for (int iRow = 0; iRow < vBRows; ++iRow)
      vBstream >> vB[iRow];
    vBstream.close();

    mC.Print("Default C:");
    vA.Print("Default a:");
    vB.Print("Default b:");

    MatrixInt result = TransportationTheorySolver::solve(mC, vA, vB);
    result.PrintPlan("###############_RESULT____##############");
    
    int sum = 0;
    for (int iRow = 0; iRow < mCrows; ++iRow)
    {
      for (int iCol = 0; iCol < mCcols; ++iCol)
      {
        if(result[iRow][iCol] != -1)
          sum += result[iRow][iCol] * mC[iRow][iCol];
      }
    }
    cout << "PRICE: " << sum << endl;
  return 0;
}