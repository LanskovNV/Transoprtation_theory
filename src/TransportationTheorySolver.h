#pragma once
#include "MatrixInt.h" 
#include "VecInt.h"
#include <list>

#define LOG_INITIAL_PLAN
#define LOG_CUR_STEP_PLAN
// #define LOG_CUR_POTENTIALS
// #define LOG_FAILED_CELLS
#define LOG_RECALC_LOOP
#define LOG_THEHA

class TransportationTheorySolver
{
private:
  static bool TryDirection(std::list<IndexPair> &recalcLoop, const MatrixInt &mRes,
    const IndexPair & cur, const IndexPair & target, const IndexPair& dir,
    int sizeN, int sizeM);
public:
  static MatrixInt solve(const MatrixInt &mC, const VecInt& vA, const VecInt& vB);
};

