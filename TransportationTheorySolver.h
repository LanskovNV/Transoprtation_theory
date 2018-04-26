#pragma once
#include "MatrixInt.h" 
#include "VecInt.h"
#include <list>
class TransportationTheorySolver
{
private:
  static bool TryDirection(std::list<IndexPair> &recalcLoop, MatrixInt &mRes, IndexPair & cur, IndexPair& target, IndexPair& dir, int sizeN, int sizeM);
public:
  static MatrixInt solve(MatrixInt &mC, VecInt vA, VecInt vB);
};

