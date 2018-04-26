#include "TransportationTheorySolver.h"
#include <math.h>
#include <iostream>
#include <iomanip>
using namespace std;
bool TransportationTheorySolver::TryDirection(list<IndexPair> &recalcLoop, MatrixInt &mRes, IndexPair & cur, IndexPair & target, IndexPair& dir, int sizeN, int sizeM)
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
MatrixInt TransportationTheorySolver::solve(MatrixInt & mC, VecInt  vA, VecInt  vB)
{
  MatrixInt res(mC.getRowCnt(), mC.getColCnt());

  int sizeN = vB.getSize();
  int sizeM = vA.getSize();


  //составление начального опроного плана

  IndexPair curCell(0, 0);
  do {
    int val = min(vA[curCell.iRow], vB[curCell.iCol]);


    if (vA[curCell.iRow] == val && vB[curCell.iCol] == val)
    {

      res[curCell] = val;
      vB[curCell.iCol] -= val;
      vA[curCell.iRow] -= val;
      if (curCell.iRow + 1 < sizeM)
      {
        for (int iCol = curCell.iCol + 1; iCol < sizeN; iCol++)
          res.data[curCell.iRow][iCol] = -1;
        curCell.iRow++;
      }
      else
      {
        for (int iRow = curCell.iRow + 1; iRow < sizeM; iRow++)
          res.data[iRow][curCell.iCol] = -1;
        curCell.iCol++;
      }
      continue;
    }

    res[curCell] = val;
    if (vA[curCell.iRow] == val)
    {
      // закрыть строку
      for (int iCol = curCell.iCol + 1; iCol < sizeN; iCol++)
        res.data[curCell.iRow][iCol] = -1;
      vB[curCell.iCol] -= val;
      curCell.iRow++;
    }
    else
    {
      // закрыть столбец
      for (int iRow = curCell.iRow + 1; iRow < sizeM; iRow++)
        res.data[iRow][curCell.iCol] = -1;
      vA[curCell.iRow] -= val;
      curCell.iCol++;
    }
  } while (curCell != IndexPair(sizeM - 1, sizeN - 1));

  if (vA[curCell.iRow] != vB[curCell.iCol])
  {
    cout << "Not Closed Task!" << endl;
  }
  res[curCell] = vA[curCell.iRow];
  /// начальный базисный план построен 
  res.PrintPlan("Initial Basic Plan:");

  VecInt vPotU(sizeM);
  VecInt vPotV(sizeN);

  bool* potUFilled = new bool[sizeM];
  bool* potVFilled = new bool[sizeN];

  for (;;)
  {
    cout << "______________________________________________________________________" << endl;
    res.PrintPlan("New Plan: ");

    //рассчитать потенциалы
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

    res.PrintPlanWithPot("Potentials: ", mC, vPotV, vPotU);

    //проверка плана на оптимальность
    bool optimalPlan = true;
    IndexPair clearCell(0, 0);
    int max = 0;
    for (int iRow = 0; iRow < sizeM; ++iRow)
    {
      for (int iCol = 0; iCol < sizeN; ++iCol) {
        if (res.data[iRow][iCol] != -1)
          continue;
        if (vPotV[iCol] - vPotU[iRow] - mC.data[iRow][iCol] > 0)
        {
          optimalPlan = false;
          cout << "Bad X: " << iRow << " Bad Y: " << iCol << endl;
          if (vPotV[iCol] - vPotU[iRow] - mC.data[iRow][iCol] > max)
          {
            max = vPotV[iCol] - vPotU[iRow] - mC.data[iRow][iCol];
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
    for (auto pair : recalcLoop)
      pair.Print("");

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

    cout << "Theta: " << theta << endl;

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

  }

  list<IndexPair> zeroDelta;
  list<IndexPair> zeroDeltaBackup;

  cout << "Delta C ZERO:" << endl;
  for (int iRow = 0; iRow < sizeM; ++iRow)
  {
    for (int iCol = 0; iCol < sizeN; ++iCol) {
      if (res.data[iRow][iCol] != -1)
        cout << " V  ";
      else
      {
        if (vPotV[iCol] - vPotU[iRow] - mC.data[iRow][iCol] == 0)
        {
          zeroDelta.push_back(IndexPair(iRow, iCol));
          zeroDeltaBackup.push_back(IndexPair(iRow, iCol));
        }
        cout << setw(3) << vPotV[iCol] - vPotU[iRow] - mC.data[iRow][iCol] << " ";
      }
    }
    cout << endl;
  }

  res.PrintPlan("PLAN 0");
  int sum = 0;
  for (int iRow = 0; iRow < 4; ++iRow)
  {
    for (int iCol = 0; iCol < 5; ++iCol)
    {
      if (res.data[iRow][iCol] != -1)
        sum += res.data[iRow][iCol] * mC.data[iRow][iCol];
    }
  }
  cout << "PRICE: " << sum << endl;
  IndexPair nextCell = zeroDelta.front();
  IndexPair otherCell = zeroDelta.back();

  bool loopEnded = false;
  while (!loopEnded)
  {
    //TESTING RECALC
    list<IndexPair> recalcLoop;
    recalcLoop.clear();

    IndexPair clearCell = nextCell;
    nextCell = otherCell;

    clearCell.Print("StartingCell");
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


    cout << "Delta C ZERO:" << endl;
    for (int iRow = 0; iRow < sizeM; ++iRow)
    {
      for (int iCol = 0; iCol < sizeN; ++iCol) {
        if (res.data[iRow][iCol] != -1)
          cout << " V  ";
        else
        {
          if (vPotV[iCol] - vPotU[iRow] - mC.data[iRow][iCol] == 0)
          {
            if (IndexPair(iRow, iCol) != nextCell)
              otherCell = IndexPair(iRow, iCol);
          }
          cout << setw(3) << vPotV[iCol] - vPotU[iRow] - mC.data[iRow][iCol] << " ";
        }
      }
      cout << endl;
    }

    res.PrintPlan("PLAN:");
    int sum = 0;
    for (int iRow = 0; iRow < 4; ++iRow)
    {
      for (int iCol = 0; iCol < 5; ++iCol)
      {
        if (res.data[iRow][iCol] != -1)
          sum += res.data[iRow][iCol] * mC.data[iRow][iCol];
      }
    }
    cout << "PRICE: " << sum << endl;
    if (zeroDeltaBackup.front() == nextCell && zeroDeltaBackup.back() == otherCell)
      loopEnded = true;
    if (zeroDeltaBackup.back() == nextCell && zeroDeltaBackup.front() == otherCell)
      loopEnded = true;
  }
  return res;
}
