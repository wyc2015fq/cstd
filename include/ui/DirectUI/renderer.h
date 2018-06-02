
#pragma once
#include <algorithm>
#include "base.h"
#include "MergeCellMgr.h"
#include "StyleDesc.h"
#include "msg.h"
#include "header.h"
#include "private_constants.h"
//#include "mycell.h"
#include "PenFactory.h"
#include "GetBorder.h"
#include "CellTypeFactory.h"
class CRgnLight;

using namespace style;
class Worksheet;
class Selections;

struct CellAxisInfo;

struct CellItem
{
  enum {
    MASK_CELL_INCLIP=1<<16,
      MASK_CSPACK=1<<17,
  };
  //const CMergeCell* pMerge;
  //BOOL fInClip;//是否与剪裁区相交
  //int row,col;
  ECellBGShrinkState eShrink;
  RECT rcCell;          //单元格自身的范围，不考虑合并的情况
  //RECT rcRender;      //用于绘制该单元格的范围，也叫绘制剪裁区
private:
  RECT rcMerge;        //单元格所处合并区的范围
  CellBorder cellBorder;
  UINT nMask;
  CellStorePack csPack;
public:
  CellItem()
  {
    clear();
  }
  RECT get_MergeRect()const
  {
    return rcMerge;
  }
  void put_MergeRect(RECT rc)
  {
    rcMerge=rc;
  }
  void SetCSPack(const CellStorePack& cs)
  {
    csPack=cs;
    nMask|=MASK_CSPACK;
  }
  const CellStorePack& GetCSPack()const
  {
    return csPack;
  }
  BOOL IsCSPackValid()const
  {
    return nMask&MASK_CSPACK;
  }
  BOOL IsInClip()const
  {
    return MASK_CELL_INCLIP&nMask;
  }
  void SetInClip()
  {
    nMask|=MASK_CELL_INCLIP;
  }
  pair<CellBorder,UINT> GetBorder()const
  {
    return make_pair(cellBorder,nMask);
  }
  void SetLeftBorder(Border bdr)
  {
    nMask|=xlEdgeLeft;
    cellBorder.left=bdr;
  }
  void SetTopBorder(Border bdr)
  {
    nMask|=xlEdgeTop;
    cellBorder.top=bdr;
  }
  void SetRightBorder(Border bdr)
  {
    nMask|=xlEdgeRight;
    cellBorder.right=bdr;
  }
  void SetBottomBorder(Border bdr)
  {
    nMask|=xlEdgeBottom;
    cellBorder.bottom=bdr;
  }
  void clear()
  {
    eShrink=ECBSS_UNKNOWN;
    rcMerge.left=rcMerge.right=0;
    nMask=0;
  }
};

//该结构用在BeginPaint,EndPaint之间，其作用主要是
//用来一次性计算一整块单元格区域中各单元格
//的范围、合并范围、绘制剪裁范围。这样有利于
//减少不必要的重复计算。

struct VisibleCellsSession
{
  void Reset(int rows,int cols,int topRow,int leftCol)
  {
    vec_.clear();
    //vec_.reserve(rows*cols);
    vec_.resize((rows+2)*(cols+2));
    rows_=rows;
    cols_=cols;
    topLeft_.set(topRow,leftCol);
  }
  int get_cols()const
  {
    return cols_;
  }
  int get_rows()const
  {
    //_ASSERT(cols_>=0);
    return rows_;
    //_ASSERT(cols_>=0);
    //if(0==cols_)
    //  return 0;
    //int rows=(int)vec_.size()/cols_;
    //if((rows*cols_)<(int)vec_.size())
    //  ++rows;
    //return rows;
  }
  void clear()
  {
    vec_.clear();
  }
  bool empty()const
  {
    return vec_.empty();
  }
  Border GetLeftBorder(const Worksheet* pSheet,CCell cell);
  Border GetRightBorder(const Worksheet* pSheet,CCell cell);
  Border GetTopBorder(const Worksheet* pSheet,CCell cell);
  Border GetBottomBorder(const Worksheet* pSheet,CCell cell);
  CellBorder GetCellBorder(const Worksheet* pSheet,CCell cell)
  {
    CellBorder cellBorder;
    cellBorder.left=GetLeftBorder(pSheet,cell);
    cellBorder.right=GetRightBorder(pSheet,cell);
    cellBorder.top=GetTopBorder(pSheet,cell);
    cellBorder.bottom=GetBottomBorder(pSheet,cell);
    return cellBorder;
  }
  CellItem& item(CCell cell)
  {
    _ASSERT(cell.row>=TopRow()-1 && cell.row<=BottomRow()+1);
    _ASSERT(cell.col>=LeftCol()-1 && cell.col<=RightCol()+1);
    const int idx=(cell.row-(TopRow()-1))*(cols_+2)+(cell.col-(LeftCol()-1));
    _ASSERT(idx<int(vec_.size()));
    return vec_[idx];  
  }
  int TopRow()const
  {
    return topLeft_.row;
  }
  int BottomRow()const
  {
    return topLeft_.row+rows_-1;
  }
  int LeftCol()const
  {
    return topLeft_.col;
  }
  int RightCol()const
  {
    return topLeft_.col+cols_-1;
  }
  
private:
  CCell topLeft_;
  int rows_,cols_;
  vector<CellItem>  vec_;
};
