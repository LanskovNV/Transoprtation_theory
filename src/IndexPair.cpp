#include "IndexPair.h"

#include <iostream>

using namespace std;

IndexPair::IndexPair(int iRow_, int iCol_)
{
  iRow = iRow_;
  iCol = iCol_;
}



void IndexPair::Print(char const * s) const
{
 cout << s << endl;
 cout << "X: " << iRow << " Y: " << iCol << endl;
}

void IndexPair::Print() const
{
  cout << "X: " << iRow << " Y: " << iCol << endl;
}

bool operator==(const IndexPair & left, const IndexPair & right)
{
  return left.iRow == right.iRow && left.iCol == right.iCol;
}

bool operator!=(const IndexPair & left, const IndexPair & right)
{
  return !(left == right);
}

IndexPair operator+(const IndexPair & left, const IndexPair & right)
{
  return IndexPair(left.iRow + right.iRow, left.iCol + right.iCol);
}

