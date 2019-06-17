#include <iostream>
#include <fstream>

#include "TransportationTheorySolver.h"

using namespace std;
  int main(void)
  {
    ifstream mCstream("tasks/transp_mC_multi.txt");
    ifstream vAstream("tasks/transp_vA_multi.txt");
    ifstream vBstream("tasks/transp_vB_multi.txt");

    /** read matrix */
    int mCrows, mCcols;
    mCstream >> mCrows;
    mCstream >> mCcols;
    MatrixInt mC = MatrixInt(mCrows, mCcols);
    for (int iRow = 0; iRow < mCrows; ++iRow)
      for (int iCol = 0; iCol < mCcols; ++iCol)
        mCstream >> mC[iRow][iCol];
    mCstream.close();

    /** read A vect */
    int vArows;
    vAstream >> vArows;
    VecInt vA = VecInt(vArows);
    for (int iRow = 0; iRow < vArows; ++iRow)
        vAstream >> vA[iRow];
    vAstream.close();

    /** read B vect */
    int vBRows;
    vBstream >> vBRows;
    VecInt vB = VecInt(vBRows);
    for (int iRow = 0; iRow < vBRows; ++iRow)
      vBstream >> vB[iRow];
    vBstream.close();

    // mC.Print("Default C:");
    // vA.Print("Default a:");
    //vB.Print("Default b:");

    /** Solving */
    MatrixInt result = TransportationTheorySolver::solve(mC, vA, vB);
    result.PrintPlan("###############_RESULT____##############");

    /** Counting optimal plan price */
    int sum = 0;
    for (int iRow = 0; iRow < mCrows; ++iRow)
      for (int iCol = 0; iCol < mCcols; ++iCol)
        if(result[iRow][iCol] != -1)
          sum += result[iRow][iCol] * mC[iRow][iCol];
    cout << "PRICE: " << sum << endl;

  return 0;
}