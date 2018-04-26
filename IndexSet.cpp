#include "IndexSet.h"
#include <iostream>
#include "Matrix.h"

IndexSet::IndexSet()
{
}

IndexSet::IndexSet(int cnt)
{
  for (int i = 0; i < cnt; ++i)
    data.push_back(i);
}

IndexSet::IndexSet(const IndexSet &is)
{
  data = is.data;
}

void IndexSet::Print(const char * s)
{
  std::cout << s << std::endl;
  for (auto ind : data)
    std::cout << ind << " ";
  std::cout << "" << std::endl;
}

bool NextSet(int *a, int n, int m)
{
  for (int i = m - 1; i >= 0; --i)
    if (a[i] < n - m + i)
    {
      ++a[i];
      for (int j = i + 1; j < m; ++j)
        a[j] = a[j - 1] + 1;
      return true;
    }
  return false;
}
void IndexSet::CompleteToSize(Matrix & m)
{
  int num = 0;
  IndexSet inv = (*this).GetInvertedSet(m.getColCnt());
  int invSetSize = inv.data.size();
  int *indArr = new int[invSetSize];

  int i = 0;
  for (auto ind : inv.data)
  {
    indArr[i++] = ind;
  }


  int toCompletecnt = m.getRowCnt() - data.size();

  int *indPosesInArr = new int[invSetSize];
  for (int i = 0; i < invSetSize; i++)
    indPosesInArr[i] = i;

  IndexSet part1 = IndexSet(*this);
  IndexSet part2 = IndexSet();
  IndexSet iSetFullRows = IndexSet(m.getRowCnt());
  do {
    part2.data.clear();
    for (int i = 0; i < toCompletecnt; ++i)
    {
      part2.data.push_back(indArr[indPosesInArr[0]]);
    }


    part1.data.splice(part1.data.end(), part2.data);


    if (m.FullRang(iSetFullRows, part1))
    {
      for (int i = 0; i < toCompletecnt; ++i)
      {
        data.push_back(indArr[indPosesInArr[0]]);
      }
      delete indArr;
      delete indPosesInArr;
      return;
    }

  } while (NextSet(indPosesInArr, invSetSize, toCompletecnt));
}

bool IndexSet::Compare(IndexSet &iSet) {
  IndexSet TempSet(*this);
  TempSet.data.sort();
  iSet.data.sort();
  auto iter = TempSet.data.begin();
  for (auto ind : iSet.data)
  {
    if (ind != *(iter++))
      return false;
  }
  return true;
}


void IndexSet::ChangeBasis(Matrix & m, IndexSet& iSetPos, int * removedInd, int * addedInd, int * removedIndPos)
{
  IndexSet iSetOutter = (*this).GetInvertedSet(m.getColCnt());
  IndexSet iSetInner = IndexSet(*this);

  IndexSet iSetTest = IndexSet(*this);

  for (auto ind : iSetPos.data)
  {
    iSetInner.data.remove(ind);
  }

  int iSetOutterSize = iSetOutter.data.size();
  int iSetInnerSize = iSetInner.data.size();
  int *indOutterArr = new int[iSetOutterSize];
  int *indInnerArr = new int[iSetInnerSize];

  int i = 0;
  for (auto ind : iSetOutter.data)
  {
    indOutterArr[i++] = ind;
  }
  i = 0;
  for (auto ind : iSetInner.data)
  {
    indInnerArr[i++] = ind;
  }

  int indPosO = 0;
  int indPosI = 0;
  int changedIndRelativePos = 0;

  IndexSet iSetFullRows = IndexSet(m.getRowCnt());

  while (indPosI < iSetOutterSize)
  {
    iSetTest.data.remove(indInnerArr[indPosI]);
    iSetTest.data.push_back(indOutterArr[indPosO]);

    if (m.FullRang(iSetFullRows, iSetTest))
    {
      /*data.remove(indInnerArr[indPosI]);
      *removedInd = indInnerArr[indPosI];
      data.push_back(indOutterArr[indPosO]);
      *addedInd = indOutterArr[indPosO];
      *removedIndPos = indPosI;*/
      std::list<int>::iterator iter = data.begin();
      for (; iter != data.end(); iter++)
      {
        if (*iter == indInnerArr[indPosI])
        {
          *iter = indOutterArr[indPosO];
          break;
        }
        changedIndRelativePos++;
      }
      *removedInd = indInnerArr[indPosI];
      *addedInd = indOutterArr[indPosO];
      *removedIndPos = changedIndRelativePos;
      delete indOutterArr;
      delete indInnerArr;
      return;
    }
    iSetTest.data.remove(indOutterArr[indPosO]);
    iSetTest.data.push_back(indInnerArr[indPosI]);


    indPosO++;
    if (indPosO == iSetOutterSize)
    {
      indPosO = 0;
      indPosI++;
    }
  }
  std::cout << "Cant change basis" << std::endl;

}




IndexSet IndexSet::GetInvertedSet(int size_)
{
  int num = 0;
  std::list<int>::iterator it = data.begin();
  IndexSet res;

  //data.sort();
  while (it != data.end() && num < size_)
  {
    if (num < *it)
      res.data.push_back(num);
    else
      it++;
    num++;
  }

  if (it == data.end())
    while (num < size_)
      res.data.push_back(num++);

  return res;
}
