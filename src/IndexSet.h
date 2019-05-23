#pragma once
#include <list>

class Matrix;

class IndexSet
{
public:
  std::list<int> data;
  IndexSet();

  /*
  Create indexSet filled with el-s from 0 to cnt-1
  e.g. 0, 1, 2, ....  , (cnt-1)
  */
  IndexSet(int cnt);
  IndexSet(const IndexSet &is);

  void Print(const char * s);

  void CompleteToSize(Matrix &m);
  void ChangeBasis(Matrix &m, IndexSet& iSetPos, int * removedInd, int * addedInd, int * removedIndPos);
  IndexSet GetInvertedSet(int size_);
private:
  bool Compare(IndexSet& iSet);
};

