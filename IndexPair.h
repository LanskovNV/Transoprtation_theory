#pragma once
class IndexPair
{
public:
  int iRow;
  int iCol;
  IndexPair(int iRow_, int iCol_);
  void Print(char const * s) const;
};

bool operator==(const IndexPair & left, const IndexPair & right);

bool operator!=(const IndexPair & left, const IndexPair & right);

IndexPair operator+(const IndexPair & left, const IndexPair & right);