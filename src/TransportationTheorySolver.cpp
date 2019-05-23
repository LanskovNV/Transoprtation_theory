#include "TransportationTheorySolver.h"
#include <math.h>
#include <iostream>
#include <iomanip>

using namespace std;

bool TransportationTheorySolver::TryDirection(list<IndexPair> &recalcLoop, const MatrixInt &mRes,
                                              const IndexPair & cur, const IndexPair & target, const IndexPair& dir,
                                              int sizeN, int sizeM)
{
  if (cur.iCol < 0 || cur.iCol >= sizeN)
    return false;
  if (cur.iRow < 0 || cur.iRow >= sizeM)
    return false;

  if (cur == target)
    return true;

  if (mRes[cur] == -1)
    return TryDirection(recalcLoop, mRes, cur + dir, target, dir, sizeN, sizeM);

  if (TryDirection(recalcLoop, mRes, cur + dir, target, dir, sizeN, sizeM))
    return true;
  if (dir.iCol == 0)
  {
    if (TryDirection(recalcLoop, mRes, cur + IndexPair(0, 1), target, IndexPair(0, 1), sizeN, sizeM))
    {
      recalcLoop.push_front(cur);
      return true;
    }

    if (TryDirection(recalcLoop, mRes, cur + IndexPair(0, -1), target, IndexPair(0, -1), sizeN, sizeM))
    {
      recalcLoop.push_front(cur);
      return true;
    }
  }
  else
  {
    if (TryDirection(recalcLoop, mRes, cur + IndexPair(1, 0), target, IndexPair(1, 0), sizeN, sizeM))
    {
      recalcLoop.push_front(cur);
      return true;
    }
    if (TryDirection(recalcLoop, mRes, cur + IndexPair(-1, 0), target, IndexPair(-1, 0), sizeN, sizeM))
    {
      recalcLoop.push_front(cur);
      return true;
    }
  }

  return false;
}
MatrixInt TransportationTheorySolver::solve(const MatrixInt &mC, const VecInt& vA_, const VecInt& vB_)
{
  int stepNum = 0;
  MatrixInt res(mC.getRowCnt(), mC.getColCnt());

  VecInt vA(vA_);
  VecInt vB(vB_);

  int sizeN = vB.getSize();
  int sizeM = vA.getSize();


  /*
   * Creating initial plan
   */

  IndexPair curCell(0, 0);
  do {
    int val = min(vA[curCell.iRow], vB[curCell.iCol]);

    if (vA[curCell.iRow] == val && vB[curCell.iCol] == val)
    {
      // income and outcome are equal
      res[curCell] = val;
      vB[curCell.iCol] -= val;
      vA[curCell.iRow] -= val;
      if (curCell.iRow + 1 < sizeM)
      {
        for (int iCol = curCell.iCol + 1; iCol < sizeN; iCol++)
          res[curCell.iRow][iCol] = -1;
        curCell.iRow++;
      }
      else
      {
        for (int iRow = curCell.iRow + 1; iRow < sizeM; iRow++)
          res[iRow][curCell.iCol] = -1;
        curCell.iCol++;
      }
      continue;
    }

    res[curCell] = val;
    if (vA[curCell.iRow] == val)
    {
      // close row
      for (int iCol = curCell.iCol + 1; iCol < sizeN; iCol++)
        res[curCell.iRow][iCol] = -1;
      vB[curCell.iCol] -= val;
      curCell.iRow++;
    }
    else
    {
      // close column
      for (int iRow = curCell.iRow + 1; iRow < sizeM; iRow++)
        res[iRow][curCell.iCol] = -1;
      vA[curCell.iRow] -= val;
      curCell.iCol++;
    }
  } while (curCell != IndexPair(sizeM - 1, sizeN - 1));

  if (vA[curCell.iRow] != vB[curCell.iCol])
  {
    cout << "Not Closed Task!" << endl;
    res[0][0] = -1;
    return res;
  }
  res[curCell] = vA[curCell.iRow];

#ifdef LOG_INITIAL_PLAN
  res.PrintPlan("Initial Basic Plan:");
#endif // LOG_INITIAL_PLAN


  VecInt vPotU(sizeM);
  VecInt vPotV(sizeN);

  bool* potUFilled = new bool[sizeM];
  bool* potVFilled = new bool[sizeN];

  for (;;)
  {
    cout << "___________________________________________" << endl;
    cout << "Starting step no: " << stepNum++ << endl;
#ifdef LOG_CUR_STEP_PLAN
    res.PrintPlan("Current Plan: ");
#endif // LOG_CUR_STEP_PLAN

    for (int i = 0; i < sizeM; ++i)
      potUFilled[i] = false;
    for (int i = 0; i < sizeN; ++i)
      potVFilled[i] = false;

    vPotV[0] = 0;
    potVFilled[0] = true;
    bool smthSetted = true;

    while (smthSetted)
    {
      smthSetted = false;
      for (int iCol = 0; iCol < sizeN; ++iCol)
      {
        if (!potVFilled[iCol])
          continue;

        for (int iRow = 0; iRow < sizeM; ++iRow)
        {
          if (res.data[iRow][iCol] != -1)
          {
            if (!potUFilled[iRow])
            {
              vPotU[iRow] = vPotV[iCol] - mC.data[iRow][iCol];
              potUFilled[iRow] = true;
              smthSetted = true;
            }
          }
        }
      }
      for (int iRow = 0; iRow < sizeM; ++iRow)
      {
        if (!potUFilled[iRow])
          continue;

        for (int iCol = 0; iCol < sizeN; ++iCol)
        {
          if (res.data[iRow][iCol] != -1)
          {
            if (!potVFilled[iCol])
            {
              vPotV[iCol] = vPotU[iRow] + mC.data[iRow][iCol];
              potVFilled[iCol] = true;
              smthSetted = true;
            }
          }
        }
      }
    }

#ifdef LOG_CUR_POTENTIALS
    res.PrintPlanWithPot("Potentials: ", mC, vPotV, vPotU);
#endif // LOG_CUR_POTENTIALS


#ifdef LOG_FAILED_CELLS
    cout << "Failed cells:" << endl;
#endif // LOG_FAILED_CELLS

    bool optimalPlan = true;
    IndexPair clearCell(0, 0);
    int max = 0;
    for (int iRow = 0; iRow < sizeM; ++iRow)
    {
      for (int iCol = 0; iCol < sizeN; ++iCol) {
        if (res[iRow][iCol] != -1)
          continue;
        if (vPotV[iCol] - vPotU[iRow] - mC[iRow][iCol] > 0)
        {
          optimalPlan = false;
#ifdef LOG_FAILED_CELLS
          cout << "Bad X: " << iRow << " Bad Y: " << iCol << endl;
#endif // LOG_FAILED_CELLS

          if (vPotV[iCol] - vPotU[iRow] - mC[iRow][iCol] > max)
          {
            max = vPotV[iCol] - vPotU[iRow] - mC[iRow][iCol];
            clearCell = IndexPair(iRow, iCol);
          }
        }

      }
    }
    if (optimalPlan)
      break;

    list<IndexPair> recalcLoop;
    recalcLoop.clear();

    for (;;)
    {
      if (TryDirection(recalcLoop, res, clearCell + IndexPair(0, 1), clearCell, IndexPair(0, 1), sizeN, sizeM))
        break;
      if (TryDirection(recalcLoop, res, clearCell + IndexPair(0, -1), clearCell, IndexPair(0, -1), sizeN, sizeM))
        break;
      if (TryDirection(recalcLoop, res, clearCell + IndexPair(1, 0), clearCell, IndexPair(1, 0), sizeN, sizeM))
        break;
      if (TryDirection(recalcLoop, res, clearCell + IndexPair(-1, 0), clearCell, IndexPair(-1, 0), sizeN, sizeM))
        break;
      break;
    }

    recalcLoop.push_front(clearCell);

#ifdef LOG_RECALC_LOOP
    cout << "Recalc Loop Cells: " << endl;
    for (auto pair : recalcLoop)
      pair.Print();
#endif // LOG_RECALC_LOOP

    list<IndexPair>::iterator iter = recalcLoop.begin();
    iter++;

    int theta = res[*iter];
    int k = 0;
    while (iter != recalcLoop.end())
    {
      k = 1 - k;
      if (k % 2 == 0)
      {
        iter++;
        continue;
      }
      if (res[*iter] < theta)
        theta = res[*iter];

      iter++;
    }

#ifdef LOG_THEHA
    cout << "Theta: " << theta << endl;
#endif // LOG_THEHA


    if (theta != 0)
    {
      bool cellWasRemoved = false;
      k = 1;
      res[clearCell] = 0;
      for (auto pair : recalcLoop)
      {
        res[pair] += theta * k;

        if (res[pair] == 0 && !cellWasRemoved)
        {
          res[pair] = -1;
          cellWasRemoved = true;
        }
        k *= -1;
      }
    }
    else
    {
      bool cellWasRemoved = false;
      res[clearCell] = 0;
      recalcLoop.pop_front();
      for (auto pair : recalcLoop)
      {
        if (res[pair] == 0 && !cellWasRemoved)
        {
          res[pair] = -1;
          cellWasRemoved = true;
        }
      }
    }
  }// END OFF MAIN LOOP

  cout << "--------------------------------------------" << endl;
  cout << "Optimal Plan founded." << endl;

  return res;
}
