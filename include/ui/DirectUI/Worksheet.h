
#pragma once
#include <map>
using namespace std;
#include "base.h"
#include "header.h"
#include "SelectionsHandler.h"
#include "MergeCellMgr.h"
#include "msg.h"
#include "CellTypeMgr.h"
#include "StylesMgr.h"
#include "SelectionBound.h"
#include "Selections.h"


class Worksheet;

enum ECalcCellAxisInfoMask
{ CCAI_CALC_HAXIS=1<<0,
CCAI_CALC_VAXIS=1<<1,
CCAI_CALC_HREMAINAXIS=1<<2,
CCAI_CALC_VREMAINAXIS=1<<3,
CCAI_CALC_ALL=0xFF,
};

//参见doc/MyCell坐标轴及区间图.png
struct CellAxisInfo
{
  short VHeaderAxis;
  short VFreezeAxis;
  short VRemainAxis;
  short VClientAxis;
  short HHeaderAxis;
  short HFreezeAxis;
  short HRemainAxis;
  short HClientAxis;
public:
  //包括1区的客户区坐标
  RECT& get_HeaderAlthonogalRect(RECT& rc)const
  {
    SetRect(&rc,0,0,VHeaderAxis,HHeaderAxis);
    return rc;
  }
  //包括2区的客户区坐标
  RECT& get_HeaderHFreezeRect(RECT& rc)const
  {
    SetRect(&rc,VHeaderAxis,0,VFreezeAxis,HHeaderAxis);
    return rc;
  }
  //包括3区的客户区坐标
  RECT& get_HeaderHScrollRect(RECT& rc)const
  {
    SetRect(&rc,VFreezeAxis,0,VRemainAxis,HHeaderAxis);
    return rc;
  }
  //包括5区的客户区坐标
  RECT& get_HeaderVFreezeRect(RECT& rc)const
  {
    SetRect(&rc,0,HHeaderAxis,VHeaderAxis,HFreezeAxis);
    return rc;
  }
  //包括9区的客户区坐标
  RECT& get_HeaderVScrollRect(RECT& rc)const
  {
    SetRect(&rc,0,HFreezeAxis,VHeaderAxis,HRemainAxis);
    return rc;
  }
  //包括6区的客户区坐标
  RECT& get_FreezeAlthonogalRect(RECT& rc)const
  {
    SetRect(&rc,VHeaderAxis,HHeaderAxis,VFreezeAxis,HFreezeAxis);
    return rc;
  }
  //包括7，8区的客户区坐标
  RECT& get_FreezeHScrollRect(RECT& rc)const
  {
    SetRect(&rc,VFreezeAxis,HHeaderAxis,VClientAxis,HFreezeAxis);
    return rc;
  }
  //包括10，14区的客户区坐标
  RECT& get_FreezeVScrollRect(RECT& rc)const
  {
    SetRect(&rc,VHeaderAxis,HFreezeAxis,VFreezeAxis,HClientAxis);
    return rc;
  }
  
  
  //返回客户区坐标，
  //  其bottom和right与客户区的bottom和right相同。
  //包括11，12，15，16区的客户区坐标
  
  RECT& get_ScrollRect(RECT& rc)const
  {
    SetRect(&rc,VFreezeAxis,HFreezeAxis,VClientAxis,HClientAxis);
    //SetRect(&rc,VFreezeAxis,HFreezeAxis,VRemainAxis,HRemainAxis);
    return rc;
  }
  
  //注解参考RECT& get_ScrollRect(RECT& rc)const
  
  RECT get_ScrollRect()const
  {
    RECT rc;
    return get_ScrollRect(rc);
  }
  //RECT& get_FreezeAlthonogalRect(RECT& rc)const
  //{
  //  SetRect(&rc,VHeaderAxis,HHeaderAxis,VFreezeAxis,HFreezeAxis);
  //  return rc;
  //}
private:
  //将其构造函数保护起来;将Worksheet设为friend class
  //该对象由Worksheet负责生成，a Worksheet has a CellAxisInfo
  //构造函数不做任何操作，正确的结构内容通过
  //Worksheet::RecalcCellAxisInfo方法获得。
  CellAxisInfo()
  {
  }
  friend class Worksheet;
};
template<class T>
class ActiveCell
{
  //friend class Worksheet;
  //Worksheet* pSheet_;
  CCell activeCell_;
protected:
  ActiveCell()
  {
    activeCell_.set(0,0);
  }
public:
  //活动单元格允许的范围是[HEADER_ROW,rows),[HEADER_COL,cols)
  CCell SetActiveCell(int row,int col)
  {
    T* pT=static_cast<T*>(this);
#ifdef _DEBUG
    int const rows=pT->get_RowHeader().get_rows();
    int const cols=pT->get_ColHeader().get_cols();
    _ASSERT(row>=-1);
    _ASSERT(row<rows);
    _ASSERT(col>=-1);
    _ASSERT(col<cols);
#endif
    int const oldRow=activeCell_.row;
    int const oldCol=activeCell_.col;
    activeCell_.set(row,col);
    pT->SendNotifyMessageToListener(oldRow,oldCol,OCN_ACTIVECELLCHANGE,(LPARAM)&activeCell_);
    return activeCell_;
  }
  
  CCell SetActiveRow(int row)
  {
    return SetActiveCell(row,max(0,activeCell_.col));
  }
  CCell SetActiveCol(int col)
  {
    return SetActiveCell(max(0,activeCell_.row),col);
  }
  int GetActiveRow()const
  {
    return activeCell_.row;
  }
  int GetActiveCol()const
  {
    return activeCell_.col;
  }
  CCell GetActiveCell()const
  {
    return activeCell_;
  }
  void GetActiveCell(int& row,int& col)const
  {
    row=activeCell_.row;
    col=activeCell_.col;
  }
};


class Workbook;
class Worksheet : public CLabelUI
,public MergeCellMgr
,public ActiveCell<Worksheet>
{
  typedef CellTypeMgr<Worksheet>  CellTypeMgr_t;
  typedef ActiveCell<Worksheet> ActiveCell_t;
  friend class SelectionsHandler;
  friend class GridRenderer;
  friend class EditImpl;
  friend class SelectionBound;
  friend class ScrollImp;
  friend class HeaderDragImpl;
  friend class Workbook;
  friend class GridWnd;
  enum eBits
  {
    B_SHOWROWHEADER=1<<0,
      B_SHOWCOLHEADER=1<<1,
      B_SHOWGRID=1<<2,
      B_AllowColumnResize=1<<3,
      B_HandleTabKey=1<<4,
      B_AutoAppendRow=1<<5,
      //B_AllowColumEdit=1<<6,
      B_SHOWSELECTION=1<<6,
      //B_ALLOWMULTISELECTION=1<<7,
  };
protected:
  Worksheet(Workbook* pWorkbook);
  ~Worksheet()
  {
  }
public:
  Workbook* GetWorkbook()
  {
    return pWorkbook_;
  }
  const Workbook* GetWorkbook()const
  {
    return pWorkbook_;
  }
  LPCTSTR GetName1()const
  {
    return szName_;
  }
  
  // 以下注释摘自Excel
  //确认输入的名称不多于31个字符。
  //确认名称中不含下列任一字符：:/?*[或]。
  //确认工作表名称不为空。
  
  BOOL SetName1(LPCTSTR name);
  //void SetSelection(ASelection* pSel)
  //{
  //  if(pSel!=pSelection_){
  //    if(pSelection_)
  //      delete pSelection_;
  //    pSelection_=pSel;
  //  }
  //}
  //HWND put_listener(HWND hWnd)
  //{
  //  HWND hWndListenerOld=hWndListener_;
  //  hWndListener_=hWnd;
  //  return hWndListenerOld; 
  //}
  //HWND get_listener()const
  //{
  //  return hWndListener_;
  //}
  const ActiveSelectionBounds& get_ActiveSelectionBounds()const
  {
    return asbounds_;
  }
  ActiveSelectionBounds& get_ActiveSelectionBounds()
  {
    return asbounds_;
  }
  
  const CellAxisInfo& get_CellAxisInfo()const
  {
    return cellAxisInfo_;
  }
  void clear();
  void UpdateWindow() {}
  void ClearCellTypes();
  /*
  //nCalcMask为如下掩码位的组合
  //CCAI_CALC_HAXIS=1<<0,       计算垂直轴线的掩码位
  //CCAI_CALC_VAXIS=1<<1,       计算水平轴线的掩码位
  //CCAI_CALC_HREMAINAXIS=1<<2, 计算HRemainAxis轴线的掩码位
  //CCAI_CALC_VREMAINAXIS=1<<3, 计算VRemainAxis轴线的掩码位
  
    const CellAxisInfo& get_CellAxisInfo(LPCRECT lprcClient=NULL,UINT nMask=CCAI_CALC_ALL)
    {
    if(lprcClient){
    RecalcCellAxisInfo(*lprcClient,nMask);
    }else{
    RECT rcCli={0};
    if(m_hWnd)
    GetClientRect(&rcCli);
    RecalcCellAxisInfo(rcCli,nMask);
    }
    return cellAxisInfo_;
    }
  */
  void ValidateSelections()
  {
    selectionImp_.ValidateSelections();
  }
  //const CellAxisInfo& get_CellAxisInfoWithCalc()
  //{
  //  RECT rc;
  //  GetClientRect(&rc);
  //  RecalcCellAxisInfo();
  //  return cellAxisInfo_;
  //  //return static_cast<const CellAxisInfo&>(*this);
  //}
  void put_ShowRowHeader(BOOL bShow)
  {
    //render_.SetDirty();
    bShow?bits_|=B_SHOWROWHEADER:bits_&=~B_SHOWROWHEADER;
  }
  BOOL get_ShowRowHeader()const
  {
    return bits_&B_SHOWROWHEADER;
  }
  void put_ShowColHeader(BOOL bShow)
  {
    //render_.SetDirty();
    bShow?bits_|=B_SHOWCOLHEADER:bits_&=~B_SHOWCOLHEADER;
  }
  BOOL ScrollWindow( int xAmount, int yAmount, LPCRECT lpRect = NULL, LPCRECT lpClipRect = NULL ) {
    return 0;
  }
  void SetCapture(){}
  BOOL HasFocus() const {
    return IsFocused();
  }
  BOOL IsWindow() const {return TRUE;}
  BOOL get_ShowColHeader()const
  {
    return bits_&B_SHOWCOLHEADER;
  }
  //void put_AllowMultiSelectionMode(BOOL bAllow)
  //{
  //  bAllow?bits_|=B_ALLOWMULTISELECTION:bits_&=~B_ALLOWMULTISELECTION;
  //}
  //BOOL get_AllowMultiSelectionMode()const
  //{
  //  return bits_&B_ALLOWMULTISELECTION;
  //}
  //void put_AllowColumnEdit(BOOL bAllow)
  //{
  //  bAllow?bits_|=B_AllowColumEdit:bits_&=~B_AllowColumEdit;
  //}
  //BOOL get_AllowColumnEdit()const
  //{
  //  return bits_&B_AllowColumEdit;
  //}
  RowHeader& get_RowHeader()
  {
    return rh_;
  }
  RowHeader const& get_RowHeader()const
  {
    return rh_;
  }
  ColHeader& get_ColHeader()
  {
    return ch_;
  }
  ColHeader const& get_ColHeader()const
  {
    return ch_;
  }
  bool GetFreezeAlthogonalPart(CCellRange& cr)const;
  bool GetFreezeRowsPart(CCellRange& cr)const;
  bool GetFreezeColsPart(CCellRange& cr)const;
  bool GetScrollPart(CCellRange& cr)const;
  //cr需满足条件：cr必须包含在由冻结线分割的四个区间中的某一个区间中
  RECT get_RangeRectPlain(const CCellRange& cr)const
  {
    _ASSERT(cr.IsNormalize());
#ifdef _DEBUG
    CCellRange cr1,cr2,cr3,cr4;
    GetFreezeAlthogonalPart(cr1);
    GetFreezeRowsPart(cr2);
    GetFreezeColsPart(cr3);
    GetScrollPart(cr4);
    _ASSERT(cr1.Inside(cr)||cr2.Inside(cr)||cr3.Inside(cr)||cr4.Inside(cr));
#endif
    RECT rc1=raw_GetCellRect(cr.TopLeft());
    RECT rc2=raw_GetCellRect(cr.RightBottom());
    RECT const rc={min(rc1.left,rc2.left),min(rc1.top,rc2.top),max(rc1.right,rc2.right),max(rc1.bottom,rc2.bottom)};
    return rc;
  }
  //[firstRow,lastRow] and [firstCol,lastCol]
  //参数构成的区域可以跨冻结线的四个区
  RECT get_RangeRect(int firstRow,int firstCol,int lastRow,int lastCol)const
  {
    //RECT rc1=raw_GetCellRect(firstRow,fistrCol);
    //RECT rc2=raw_GetCellRect(lastRow,lastCol);
    //RECT const rc={min(rc1.left,rc2.left),min(rc1.top,rc2.top),max(rc1.right,rc2.right),max(rc1.bottom,rc2.bottom)};
    //RECT rc;
    //UnionRect(&rc,&rc1,&rc2);
    return get_RangeRect(CCellRange(firstRow,firstCol,lastRow,lastCol));
  }
  //pcr可以跨冻结线的四个区
  RECT get_RangeRect(CCellRange const& pcr)const;
  //SelectionsHandler& get_Selection()
  //{
  //  return selection_;
  //}
  BOOL IsSelectAll(/*CCellRange const& cr*/)const
  {
    //CCellRange const cr=get_Selection();
    //return selection_.IsSelectAll(cr);
    //return selections_.IsSelectAll();
    return selectionImp_.IsSelectAll();
  }
  //用GetActiveCell()const代替
  //CCell GetActiveCell()const
  //{
  //  //CCell cell;
  //  //selection_.GetActiveCell(cell.row,cell.col);
  //  //return cell;
  //  return pSelection_->GetActiveCell();
  //}
  /*
  ESelectionMode get_SelectionMode()const
  {
  return selectionMode_;
  }
  void put_SelectionMode(ESelectionMode esm)
  {
  selectionMode_=esm;
  }
  */
  //ECellState get_CellState(int row,int col)const;
  //void GetActiveCell(int& row,int& col)
  //{
  //  GetActiveCell(row,col);
  //  //row=c.row;
  //  //col=c.col;
  //  //selection_.GetActiveCell(row,col);
  //}
  template<bool bInvalidate>
    CCell SetActiveCell(int row,int col)
  {
    return SetActiveCell(row,col,Int2Type<bInvalidate>());
  }
  template<bool bInvalidate>
    CCell SetActiveCell(CCell cell)
  {
    return SetActiveCell<bInvalidate>(cell.row,cell.col);
  }
  template<bool bInvalidate>
    CCell SetActiveCell(CCell cell,Int2Type<bInvalidate> b)
  {
    return SetActiveCell(cell.row,cell.col,b);
  }
  
  void get_Selection(Ranges& rngs/*vector<CCellRange>& vec*/)const
  {
    selectionImp_.EnumSelection/*<true>*/(rngs.vec_);
    //for(vector<CCellRange>::iterator it=vec.begin();it!=vec.end();++it)
    //  AdjustSelection(*it);
    //pSelection_->get_Range(vec);
    //if(!vec.empty()){
    //  AdjustSelection(vec[0]);
    //}
  }
  bool IsFreezed()const
  {
    return get_RowHeader().IsFreezeRowVisible() || get_ColHeader().IsFreezeColVisible();
  }
  void Freeze(int nFreezeTopRow,int nFreezeBottomRow,int nFreezeLeftCol,int nFreezeRightCol);
  void UnFreeze();
  
  
  //x,y should be screen coords
  //其他注解参考GetMouseCell()
  
  CCell GetCellByScreenPos(int x,int y)const
  {
    POINT pt={x,y};
    ScreenToClient(&pt);
    return GetCellByPos(pt.x,pt.y);
  }
  
  //返回row的取值范围：
  //1,-1表示行表头；
  //2,-2表示在表格上方；应视为无效
  //3,[FreezeTopRow,FreezeBottomRow]区间的某一值
  //4,[TopVisScrollRow,BottomVisScrollRow]区间的某一值
  //5,rows,；表示在可见行下方，应视为无效
  //返回col的取值范围类似。
  
  CCell GetMouseCell()const
  {
    POINT pt;
    GetCursorPos(&pt);
    return GetCellByScreenPos(pt.x,pt.y);
  }
  //void get_Selection(
  //void get_Selection(int& leftCol,int& topRow,int& rightCol,int& bottomRow) const
  //{
  //  CCellRange cr=get_Selection();
  //  leftCol=cr.first.col;
  //  topRow=cr.first.row;
  //  rightCol=cr.second.col;
  //  bottomRow=cr.second.row;
  //}
  void put_Selection(int leftCol,int topRow,int rightCol,int bottomRow)
  {
    const int rows=rh_.get_rows();
    const int cols=ch_.get_cols();
    _ASSERT(leftCol>=HEADER_COL);
    _ASSERT(topRow>=HEADER_ROW);
    if(0==cols || 0==rows)
      return;
    //_ASSERT(rightCol<ch_.get_cols());
    //_ASSERT(bottomRow<rh_.get_rows());
    //selection_.put_Range(leftCol,topRow,rightCol,bottomRow);
    //pSelection_->SetActiveSelection(leftCol,topRow,rightCol,bottomRow);
    //selectionImp_.clear();
    selectionImp_.SetActiveSelection(leftCol,topRow,rightCol,bottomRow);
  }
  void put_ShowGrid(BOOL bShow)
  {  
    bShow?bits_|=B_SHOWGRID:bits_&=~B_SHOWGRID;
  }
  BOOL get_ShowGrid()const
  {
    return bits_&B_SHOWGRID?TRUE:FALSE;
  }
  void put_ShowSelection(BOOL bShow)
  {
    bShow?bits_|=B_SHOWSELECTION:bits_&=~B_SHOWSELECTION;
  }
  BOOL get_ShowSelection()const
  {
    return bits_&B_SHOWSELECTION?TRUE:FALSE;
  }
  /*
  RECT get_ScrollRect()
  {
  _ASSERT(IsWindow());
  RECT rc;
  GetClientRect(&rc);
  if(get_ShowColHeader())
  rc.top=get_ColHeader().get_height();
  if(get_ShowRowHeader())
  rc.left=get_RowHeader().get_width();
  return rc;
  }
  */
  //void get_ScrollRect(LPRECT lprc)const;
  BOOL get_AllowColResize()const
  {
    return bits_&B_AllowColumnResize;
  }
  void put_AllowColResize(BOOL bAllow)
  {
    bAllow?bits_|=B_AllowColumnResize:bits_&=~B_AllowColumnResize;
  }
  void put_HandleTabKey(BOOL bHandleTab = TRUE)  
  {
    bHandleTab?bits_|=B_HandleTabKey:bits_&=~B_HandleTabKey;
  }
  BOOL get_HandleTabKey() const 
  {
    return (bits_&B_HandleTabKey)?TRUE:FALSE;
  }
  
  BOOL get_AllowRowResize()const
  {
    return rh_.get_AllowRowResize();
    //return bits_&B_AllowRowResize;
  }
  void put_AllowRowResize(BOOL bAllow)
  {
    rh_.put_AllowRowResize(bAllow);
  }
  //EMouseMode get_MouseMode()const
  //{
  //  return mouseMode_;
  //}
  //void put_MouseMode(EMouseMode emm)
  //{
  //  mouseMode_=emm;
  //}
  BOOL get_AutoAppendRow() const
  {
    return B_AutoAppendRow&bits_?TRUE:FALSE;
  }
  
  void put_AutoAppendRow(BOOL bAutoAppend)
  {
    bAutoAppend?bits_|=B_AutoAppendRow:bits_&=~B_AutoAppendRow;
    //if(IsWindow())
    {
      InvalidateCell(CCell(rh_.get_rows()-1,HEADER_COL));
      //UpdateWindow();
    }
  }
  
  //return lenght of buf
  BOOL GetCellText(CCell cell,VARIANT* pval,EGetCellValDistination bRender/*=EGCVD_NORMAL*/)const;
  int GetCellText(CCell cell,LPTSTR buf,EGetCellValDistination bRender/*=EGCVD_NORMAL*/)const
  {
    buf[0]=0;
    CComVariant val;
    if(GetCellText(cell,&val,bRender) && SUCCEEDED(val.ChangeType(VT_BSTR))){
      return lstrlen(lstrcpy(buf,CString(val.bstrVal)));
    }
    return 0;
  }
  /*
  */
  HRESULT SetCellText(CCell cell,const VARIANT& lpszText,BOOL fRedraw);
  
  //注解参考get_ScrollRect(LPRECT)
  //RECT get_ScrollRect()const
  //{
  //  RECT rc;
  //  //GetClientRect(&rc);
  //  get_ScrollRect(rc);
  //  return rc;
  //}
  
  
  BOOL IsRowPartialVisible(int row)const
  {
    if(row>=0){
      const RowHeader& rh=get_RowHeader();
      if(row>=rh.get_TopVisScrollRow() && row<=rh.get_BottomVisScrollRow())
        return TRUE;
      if(rh.IsFreezeRowVisible()&&row>=rh.get_FreezeTopRow() && row<=rh.get_FreezeBottomRow())
        return TRUE;
    }
    return FALSE;
  }
  BOOL IsColPartialVisible(int col)const
  {
    if(col>=0){
      _ASSERT(col>=0);
      const ColHeader& ch=get_ColHeader();
      if(col>=ch.get_LeftVisScrollCol() && col<=ch.get_RightVisScrollCol())
        return TRUE;
      if(/*ch.IsFreezeColVisible()&&*/col>=ch.get_FreezeLeftCol() && col<=ch.get_FreezeRightCol())
        return TRUE;
    }
    return FALSE;
  }
  BOOL IsRowFullVisible(int row)const;
  BOOL IsColFullVisible(int col)const;
  
  BOOL IsCellFullVisible(int row,int col)const
  {
    return IsRowFullVisible(row) && IsColFullVisible(col);
  }
  BOOL IsCellFullVisible(CCell cell)const
  {
    return IsCellFullVisible(cell.row,cell.col);
  }
  BOOL IsCellPartialVisible(int row,int col)const
  {
    return IsRowPartialVisible(row) && IsColPartialVisible(col);
  }
  void EnsureVisible(int row,int col,BOOL bUpdateWindow=TRUE,BOOL bFullVisible=TRUE);//,BOOL bPartial=FALSE,bool bUpdateWindow=true);
  void EnsureVisible(CCell cell,BOOL bUpdateWindow=TRUE,BOOL bFullVisible=TRUE)//,BOOL bPartial=FALSE,bool bUpdateWindow=true)
  {
    EnsureVisible(cell.row,cell.col,bUpdateWindow,bFullVisible);//,/*bPartial,*/);
  }
  void Scroll_ResetHScrollBar(LPCRECT lprcClient=NULL);
  void Scroll_ResetVScrollBar(LPCRECT lprcClient=NULL);
  //void Scroll_ResetScrollBars(BOOL bResetHCrollBar,BOOL bResetVScrollBar,LPCRECT lprcClient=NULL);//const CellAxisInfo* pcai=NULL);
  //int GetScrollPos32(int nBar, BOOL bGetTrackPos  = FALSE );
  //int Scroll_GetHScrollPos32(int nBar,BOOL bGetTrackPos=FALSE)const
  //{
  //  SCROLLINFO si={sizeof(SCROLLINFO),bGetTrackPos?SIF_TRACKPOS:SIF_POS};
  //  hScrollBar_.GetScrollInfo(&si);
  //  return si.fMask?si.nTrackPos:si.nPos;
  //}
  //int Scroll_GetVScrollPos32(int nBar/*,BOOL bGetTrackPos=FALSE*/)const
  //{
  //  SCROLLINFO si={sizeof(SCROLLINFO),bGetTrackPos?SIF_TRACKPOS:SIF_POS};
  //  vScrollBar_.GetScrollInfo(&si);
  //  return si.fMask?si.nTrackPos:si.nPos;
  //}
  //BOOL Scroll_SetHScrollPos32(/*int nBar, CScrollBar scrollBar,*/int nPos, BOOL bRedraw  = TRUE )
  //{
  //  SCROLLINFO si={sizeof(SCROLLINFO),SIF_POS,0,0,0,nPos};
  //  RECT rc;GetClientRect(&rc);
  //  RecalcCellAxisInfo(rc,CCAI_CALC_ALL);
  //  return hScrollBar_.SetScrollInfo(/*nBar,*/ &si, bRedraw);
  //}
  //BOOL Scroll_SetVScrollPos32(/*int nBar, CScrollBar scrollBar,*/int nPos, BOOL bRedraw  = TRUE )
  //{
  //  SCROLLINFO si={sizeof(SCROLLINFO),SIF_POS,0,0,0,nPos};
  //  RECT rc;GetClientRect(&rc);
  //  RecalcCellAxisInfo(rc,CCAI_CALC_ALL);
  //  return vScrollBar_.SetScrollInfo(/*nBar,*/ &si, bRedraw);
  //}
  void Scroll_SetScrollBars(CScrollBar hScrollBar,CScrollBar vScrollBar)
  {
    //hScrollBar_=hScrollBar;
    //vScrollBar_=vScrollBar;
  }
  BOOL Scroll_GetHScrollInfo(LPSCROLLINFO lpScrollInfo) const
  {
    return 0;//hScrollBar_.GetScrollInfo(lpScrollInfo);
  }
  BOOL Scroll_GetVScrollInfo(LPSCROLLINFO lpScrollInfo) const
  {
    return 0;//vScrollBar_.GetScrollInfo(lpScrollInfo);
  }
  BOOL Scroll_SetHScrollInfo(LPSCROLLINFO lpScrollInfo,BOOL bRedraw=1)
  {
    return 0;//hScrollBar_.SetScrollInfo(lpScrollInfo,bRedraw);
  }
  BOOL Scroll_SetVScrollInfo(LPSCROLLINFO lpScrollInfo,BOOL bRedraw=1)
  {
    return 0;//vScrollBar_.SetScrollInfo(lpScrollInfo,bRedraw);
  }
  
  //template<bool bScroll>
  //si.fMask should contain SIF_POS|SIF_RANGE|SIF_PAGE
  
  int put_TopVisScrollRow(int nTopVisScrollRow,const CellAxisInfo* pcai=NULL/*,bool bScroll=true*/);
  int put_LeftVisScrollCol(int nLeftVisScrollCol,const CellAxisInfo* pcai=NULL,bool bResetScrollBar=true);
  
  //与ColHeader之put_RightVisScrollCol相比，考虑了滚动窗口和
  //滚动滚动条
  //si.fMask should contain SIF_POS|SIF_RANGE|SIF_PAGE
  //返回新的LeftVisScrollCol
  
  int put_RightVisScrollCol(int nRightVisScrollCol,BOOL bFullVisible=TRUE,const CellAxisInfo* pcai=NULL/*,bool bScroll=true*/)
  {
    if(!pcai)
      pcai=&get_CellAxisInfo();//NULL,CCAI_CALC_VAXIS);
    int const nLeftVisScrollCol=ch_.calc_LeftVisScrollCol(nRightVisScrollCol,pcai->VClientAxis-pcai->VFreezeAxis,bFullVisible);
    
    //注意put_LeftVisScrollCol会发出OnVisibleColChanged事件
    //故该处无需调用OnVisibleColChanged
    return put_LeftVisScrollCol(nLeftVisScrollCol,pcai/*,bScroll*/);
  }
  
  //cai.HFreezeAxis和cai.HClientAxis应该有效
  //返回新的TopVisScrollRow
  int put_BottomVisScrollRow(int nBottomVisScrollRow,BOOL bFullVisible=TRUE,const CellAxisInfo* pcai=NULL/*,bool bScroll=true*/)
  {
    if(!pcai)
      pcai=&get_CellAxisInfo();//NULL,CCAI_CALC_HAXIS);
    int const nTopVisScrollRow=rh_.calc_TopVisScrollRow(nBottomVisScrollRow,pcai->HClientAxis-pcai->HFreezeAxis,bFullVisible);
    return put_TopVisScrollRow(nTopVisScrollRow,pcai/*,bScroll*/);
    //return rh_.get_BottomVisScrollRow();
  }
  
  void put_RowHeight(int row,int newHeight,BOOL fRedrawHeader=TRUE,bool bUpdateWindow=true);
  void put_RowsHeight(int beg,int end,int newHeight,bool bRedrawWindow=true);
  void put_ColWidth(int col,int wi,BOOL fRedrawHeader=TRUE,bool bUpdateWindow=true);
  void put_ColsWidth(int beg,int end,int newWidth,bool bRedrawWindow=true);
  
  
  void CopyToClipboard(int y1,int x1,int y2,int x2);
  bool CanPaste();
  HRESULT PasteFromClipboard(int row1,int col1,int row2,int col2);
  HRESULT SendNotifyMessageToListener(int nRow, int nCol, int nNotifyCode,LPARAM lParam,BOOL* pbHandled=NULL) const;
  void ColumnAutoFit(int col);
  void RowHeaderAutoFit();
  void insert_rows(int beg,int nCount,bool bFireMsg=TRUE);
  void insert_cols(int beg,int nCount,bool bFireMsg=TRUE);
  BOOL delete_rows(int begin,int end,bool bFireMsg=TRUE);
  BOOL delete_cols(int begin,int end,bool bFireMsg=TRUE);
  
  void InvalidateRect(const RECT* rc=NULL, BOOL b=0) {
    Invalidate();
  }
  //自动剪裁，即你可以传一个很大的范围，方法
  //会自动和由冻结线分割的四个可见区间剪裁
  
  void InvalidateCellRange(CCellRange const* pcr)
  {
    //_ASSERT(FALSE);
    RECT const rc=get_RangeRect(*pcr);//pcr->first.row,pcr->first.col,pcr->second.row,pcr->second.col);
    InvalidateRect(&rc,FALSE);
  }
  
  
  //自动剪裁，即你可以传一个很大的范围，方法
  //会自动和由冻结线分割的四个可见区间剪裁
  //仅使边框部分无效
  
  void InvalidateCellRangeBorder(CCellRange const* pcr);
  //{
  //  RECT const rc=get_RangeRect(pcr);//pcr->first.row,pcr->first.col,pcr->second.row,pcr->second.col);
  //  InvalidateRect(&rc,FALSE);
  //}
  
  //要考虑单元格处于合并单元格的情况
  //如果单元格处于合并单元格中，则使
  //整个合并单元格无效，此通过GetVisibleCellRectEx实现
  
  //不考虑合并单元格的情况
  
  void InvalidateCell(CCell cell)
  {
    RECT const rc=raw_GetCellRect(cell);//GetVisibleCellRectEx(row,col);
    InvalidateRect(&rc,FALSE);
  }
  void put_rows(int rows);
  void put_cols(int cols);
  VisibleMergeCellMgr& GetVisibleMergeCellMgr()
  {
    return visMergeCells_;
  }
  const VisibleMergeCellMgr& GetVisibleMergeCellMgr()const
  {
    return visMergeCells_;
  }
  //  /*
  //  RowHeader& rh=get_RowHeader();
  //  ColHeader& ch=get_ColHeader();
  //  CCellRange cr;
  //  cr.set(rh.get_TopVisScrollRow(),ch.get_LeftVisScrollCol(),rh.get_BottomVisScrollRow(),ch.get_RightVisScrollCol());
  //  visMergeCells_.ReCalculate(cr);
  //  */
  //  switch(ePart)
  //  {
  //  case EGP_FREEZE_ALTHOGONAL:
  //    break;
  //  case EGP_FREEZE_ROWS:
  //    break;
  //  case EGP_FREEZE_COLS:
  //    break;
  //  case EGP_SCROLLABLE:
  //    break;
  //  }
  //  return visMergeCells_;
  //
  
  //HRESULT ImportFromXml(EXmlType nType=EXT_UNKNOWN,LPCTSTR Filename=NULL,LPTSTR outFileName=NULL,LPARAM lParam=0);
  ////HRESULT ImportFromXml(LPCTSTR Filename=NULL,LPTSTR outFileName=NULL);
  ////HRESULT ImportFromExcel(LPCTSTR Filename=NULL,LPTSTR outFileName=NULL);
  //HRESULT ExportToXml(EXmlType nType,LPCTSTR Filename=NULL,LPTSTR outFileName=NULL)const;
  //HRESULT ExportToExcel(LPCTSTR Filename=NULL,LPTSTR outFileName=NULL)const;
  //返回受影响的范围
  CMergeCell* Merge(int minR,int minC,int maxR,int maxC);
  CMergeCell* Merge(CCellRange cr)
  {
    return Merge(cr.TopRow(),cr.LeftCol(),cr.BottomRow(),cr.RightCol());
  }
  //返回受影响的范围
  CCellRange Unmerge(int minR,int minC,int maxR,int maxC);
  //BOOL draw_HeaderCell(HDC hDC,int col,/*LPCTSTR lpText,int nCount,*/LPCRECT lprc/*,DrawEnvironment& de*/);
  EGridPart QueryPart(int row,int col)const;
  
  public:
    //x,y should be client coords
    //CCell HitTest(int x,int y,ECellHitTestConstants& hct)const;
    
    //chi.pt must validate and chi.pt should be client coords.
    CELLHITTESTINFO& HitTest(CELLHITTESTINFO& chi)const;
    //x,y should be client coords
    CELLHITTESTINFO HitTest(int x,int y)const
    {
      CELLHITTESTINFO chi;
      chi.pt.x=x;
      chi.pt.y=y;
      return HitTest(chi);
    }
    //x,y should be client coords
    CCell GetCellByPos(int x,int y)const;
    
    //返回客户区坐标。
    //该方法将会判断单元格是否处于合并单元格区域
    //中，如果是将返回整个合并单元格的范围。
    
    //RECT GetCellRectEx(int row,int col)const
    //{
    //  const CCell cell(row,col);
    //  CCellRange const* pmerge=GetMergeCells(cell);
    //  if(!pmerge)
    //    return raw_GetCellRect(cell);
    //  return get_RangeRect(*pmerge);
    //}
    
    //返回客户区坐标。
    //该方法将会判断单元格是否处于合并单元格区域
    //中，如果是将返回整个合并单元格的范围。
    //注意该方法与GetCellRectEx的区别：
    //该方法直接利用VisibleMergeCellMgr::GetMergeCells方法
    //获得合并单元格，因此所计算的单元格因在可见
    //区域才能得到正确的结果否则和直接调用
    //Worksheet::raw_GetCellRect没有区别
    
    RECT GetVisibleCellRectEx(int row,int col)const
    {
#ifdef _DEBUG
      IsCellPartialVisible(row,col);
#endif
      const CCell cell(row,col);
      const VisibleMergeCellMgr& vmm=GetVisibleMergeCellMgr();
      CCellRange const* pmerge=vmm.GetMergeCells(cell);
      if(!pmerge)
        return raw_GetCellRect(cell);
      return get_RangeRect(*pmerge);
    }
    //注解同GetVisibleCellRectEx(int row,int col)const
    RECT GetVisibleCellRectEx(CCell cell)const
    {
      return GetVisibleCellRectEx(cell.row,cell.col);
    }
    
    //返回客户区坐标，该方法应作为低层次调用
    //将不考虑单元格是否处于合并单元格中而
    //扩大到整个合并单元格范围的问题。如果需要
    //得到整个合并单元格的范围，因调用GetCellRectEx
    //方法或直接调用Worksheet的get_RangeRect方法
    
    RECT raw_GetCellRect(CCell cell/*int row,int col*/)const;
    
    //返回客户区坐标，该方法应作为低层次调用
    //将不考虑单元格是否处于合并单元格中而
    //扩大到整个合并单元格范围的问题。如果需要
    //得到整个合并单元格的范围，因调用GetCellRectEx
    //方法或直接调用Worksheet的get_RangeRect方法
    
    //RECT raw_GetCellRect(CCell cell)const
    //{
    //  return raw_GetCellRect(cell.row,cell.col);
    //}
    CellStorePack GetCellStore(CCell cell,UINT nMask)const
    {
      const CellStore* pcs=NULL;
      if(cell.col>=ch_.get_cols() || cell.row>=rh_.get_rows()
        ||cell.row<0 || cell.col<0)
        return CellStorePack();
      return GetCellStore(cell,nMask,GetMergeCells(cell));
    }
    CellStorePack GetCellStore(CCell cell,UINT nMask,const CMergeCell* pMerge)const;
    /*
    private:
    template<>
    VisibleMergeCellMgr* GetVisibleMergeCellMgr<EGP_SCROLLABLE>()
    {
    return &visMergeCells_;
    }
    template<>
    VisibleMergeCellMgr* GetVisibleMergeCellMgr<EGP_FREEZE_ALTHOGONAL>()
    {
    return &visFreezeAlthogonalMergeCells_;
    }
    template<>
    VisibleMergeCellMgr* GetVisibleMergeCellMgr<EGP_FREEZE_ROWS>()
    {
    return &visFreezeRowsMergeCells_;
    }
    template<>
    VisibleMergeCellMgr* GetVisibleMergeCellMgr<EGP_FREEZE_COLS>()
    {
    return &visFreezeColsMergeCells_;
    }
    template<>
    VisibleMergeCellMgr* GetVisibleMergeCellMgr<EGP_VISIBLE_PART>()
    {
    //return &visMergeCells_;
    }
    */
  public:
    //void GetCellStyle(int row,int col,StyleDesc& styleDesc)const
    //{
    //  ColHeader const& ch=get_ColHeader();
    //  CellStylesMgr::GetCellStyle(&ch,row,col,styleDesc);
    //}
    const SelectionsHandler* get_Selections()const
    {
      return &selectionImp_;
    }
    SelectionsHandler* get_Selections()
    {
      return &selectionImp_;
    }
    
    bool InSelection(int row,int col)const
    {
      return selectionImp_.CellInSelections(row,col);
      //return pSelection_->InSelection(row,col);
    }
    
    
    //ECellType GetCellType(int row,int col,StyleDesc const& styleDesc)const
    //pair<ECellType,ECellStorePosition>  GetCellType(CCell cell,ECellStorePosition eStartPos)const;
    CellTypePack  GetColType(int col)const
    {
      CellTypePack cp;
      cp.cellType=0;
      cp.eStorePos=ESP_STYLE_DEFAULT;
      const CellStore* pcs=ch_.GetColStore(col);
      if(pcs){
        cp.cellType=pcs->GetCellType();
        cp.eStorePos=ESP_STYLE_COL_OWN;
      }else{
        cp.cellType=tableDefCellStore_.GetCellType();
        cp.eStorePos=ESP_STYLE_TABLE_OWN;
      }
      return cp;
    }
    CellTypePack  GetCellType(CCell cell)const
    {
      CellTypePack cp;
      cp.cellType=0;
      cp.eStorePos=ESP_STYLE_DEFAULT;
      if(HEADER_COL==cell.col&&HEADER_ROW==cell.row){
        cp.cellType=ECT_HEADERCORNER;
      }else if(HEADER_COL==cell.col){
        cp.cellType=ECT_ROWHEADER;
      }else if(HEADER_ROW==cell.row){
        cp.cellType=ECT_COLHEADER;
      }
      if(cell.row<MIN_ROW || cell.col<MIN_COL)
        return cp;
      return GetCellStore(cell,ECSM_CELLTYPE_VALID).GetCellTypePack();
      //if(cs.CellStore().cellType){
      //  return make_pair((ECellType)cs.CellStore().cellType,cs.StorePos());
      //}else
      //  return GetCellType(cell,cs.StorePos());
    }
    void SetCellType(CCell cell,ECellType cellType)
    {
      _ASSERT(cellType<ECT_MAX);
      CMergeCell* pMerge=GetMergeCells(cell);
      if(pMerge){
        pMerge->SetCellType(cellType);
      }else{
        cellStores_.SetCellType(cell,cellType);
      }
      //tableDefCellStore_.cellType=cellType;
      //StyleDesc style=Style_GetCellStyle(cell);
      //style.CellType_SetType((USHORT)cellType);
      //Style_SetCellStyle(cell,style);
    }
    
    void Style_ClearStyles();
    
    void Style_ClearCellStyle(CCell cell)
    {
      CMergeCell* pMerge=GetMergeCells(cell);
      if(pMerge){
        pMerge->ClearStyle(styleStore_);
      }else
        cellStores_.Style_ClearCellStyle(cell);
    }
    
    //pair<StyleID_t,ECellStorePosition> Style_GetCellStyleID(CCell cell)const
    //{
    //  const CellStorePack cs=GetCellStore(cell);
    //  return make_pair(cs.CellStore().nStyleID,cs.StorePos());
    //}
    //StylePack Style_GetCellStyleID(CCell cell,const CellStore& cs)const
    //{
    //  CellStorePack csp(cell,cs,cs.
    //}
    
    StylePack Style_GetCellStyleID(CCell cell)const
    {
      return GetCellStore(cell,ECSM_STYLE_VALID).GetStylePack();
      //if(cr.CellStore().nStyleID){
      //  return make_pair(cr.CellStore().nStyleID,cr.StorePos());
      //}else{
      //  return Style_GetCellStyleID(cell,cr.StorePos());
      //}
    }
    //StyleDesc Style_GetCellStyle(CCell cell,ECellStorePosition eStorePos)const
    //{
    //  const pair<StyleID_t,ECellStorePosition> pr=Style_GetCellStyleID(cell,eStorePos);
    //  const StyleDesc& sd=Style_GetStyleByID(pr.first);
    //  return StyleDesc(sd,pr.second);
    //}
    const StyleDesc& Style_GetCellStyle(CCell cell)const
    {
      return Style_GetStyleByID(GetCellStore(cell,ECSM_STYLE_VALID).GetStylePack().nStyleID);
      //const pair<StyleID_t,ECellStorePosition> pr= Style_GetCellStyleID(cell);
      //const StyleDesc& sd=Style_GetStyleByID(pr.first);
      //return StyleDesc(sd,pr.second);
    }
    //StyleDesc Style_GetColStyle(int col)const
    //{
    //  ECellStorePosition eStorePos=ESP_STYLE_COL_OWN;
    //  StyleID_t sid=get_ColHeader().Style_GetColStyleID(col);
    //  if(!sid){
    //    sid=tableDefCellStore_.GetStyleID();
    //    eStorePos=sid?ESP_STYLE_TABLE_OWN:ESP_STYLE_DEFAULT;
    //  }
    //  const StyleDesc& sd= sid?Style_GetStyleByID(sid):styleStore_.get_DefStyle();
    //  return StyleDesc(sd,eStorePos);
    //}
    const StyleDesc& Style_GetRowStyle(int row)const
    {
      StyleID_t sid=get_RowHeader().Style_GetRowStyleID(row);
      if(!sid)
        sid=tableDefCellStore_.GetStyleID();
      return sid?Style_GetStyleByID(sid):styleStore_.get_DefStyle();
    }
    StyleID_t Style_SetCellStyle(CCell cell,const StyleDesc& style);
    
    const StyleDesc& Style_GetStyleByID(StyleID_t sid)const
    {
      return styleStore_.get_Style(sid);
    }
    StyleID_t Style_GetTableStyleID()const
    {
      return tableDefCellStore_.GetStyleID();
    }
    const CellStore& GetTableDefCellStore()const
    {
      return tableDefCellStore_;
    }
    void SetTableDefCellType(ECellType ect)
    {
      tableDefCellStore_.SetCellType(ect);
    }
    const StyleDesc& Style_GetTableStyle()const
    {
      return tableDefCellStore_.GetStyleID()?Style_GetStyleByID(tableDefCellStore_.GetStyleID()):styleStore_.get_DefStyle();
    }
    //template<int>
    //const StyleDesc& Style_GetTableStyle()const
    //{
    //  return nTableStyleID_?Style_GetStyleByID(nTableStyleID_):pWorkbook_->GetStyles().get_DefStyle();
    //}
    
    /*
    StyleDesc const& Style_GetDefaultStyle()const
    {
    return styleStore_.get_DefStyle();
    }
    */
    //StyleID_t Style_SetSheetStyle(const StyleDesc& style)
    //{
    //  if(nTableStyleID_){
    //    styleStore_.DecrementRefCount(nTableStyleID_);
    //  }
    //  nTableStyleID_=styleStore_.add_style(style);
    //  return nTableStyleID_;
    //}
    
    StyleID_t Style_SetTableStyle(StyleDesc const& style)
    {
      return tableDefCellStore_.SetStyle(styleStore_,style);
    }
    void Style_EnumStyles(vector<pair<StyleID_t,const StyleDesc*> >& vec)const
    {
      styleStore_.EnumStyles(vec);
    }
    StyleID_t Style_SetColStyle(int col,const StyleDesc& style);
    StyleID_t Style_SetRowStyle(int row,const StyleDesc& style)
    {
      if(Style_GetTableStyle()==style){
        rh_.style_ClearRowStyle(styleStore_,row);
        return 0;
      }else{
        return rh_.style_SetRowStyle(styleStore_,row,style);
      }
    }
    //取单元格cell的上Border
    Border Style_GetTopBorder(CCell cell)const;
    
    Border Style_GetBottomBorder(CCell cell)const;
    //{
    //  typedef GetBordersPolicyNormal<Worksheet> TPolicy;
    //  TPolicy policy(this);
    //  BorderGetter<TPolicy> bg(policy);
    //  return bg.GetBottomBorder(this,cell);
    //}
    
    Border Style_GetLeftBorder(CCell cell)const;
    //{
    //  typedef GetBordersPolicyNormal<Worksheet> TPolicy;
    //  TPolicy policy(this);
    //  BorderGetter<TPolicy> bg(policy);
    //  return bg.GetLeftBorder(this,cell);
    //  //Border bdr;
    //  //bdr=Style_GetCellStyle(cell).GetBorders()->left;
    //  //if(bdr.IsNull()&&cell.col>0){
    //  //  --cell.col;
    //  //  bdr=Style_GetCellStyle(cell).GetBorders()->right;
    //  //}
    //  //return bdr;
    //}
    Border Style_GetRightBorder(CCell cell)const;
    //{
    //  typedef GetBordersPolicyNormal<Worksheet> TPolicy;
    //  TPolicy policy(this);
    //  BorderGetter<TPolicy> bg(policy);
    //  return bg.GetRightBorder(this,cell);
    //  //Border bdr;
    //  //bdr=Style_GetCellStyle(cell).GetBorders()->right;
    //  //if(bdr.IsNull()){
    //  //  if(pMerge)
    //  //    cell.col=pMerge->RightCol();
    //  //  ++cell.col;
    //  //  if(cell.col<get_ColHeader().get_cols()-1)
    //  //    bdr=Style_GetCellStyle(cell).GetBorders()->left;
    //  //}
    //  //return bdr;
    //}
    
    const CComVariant* Range_GetFirstValidData(const CCellRange& cr)const;
    BOOL Range_GetFirstValidData(const CCellRange& cr,CComVariant& val);
    BOOL Range_GetTopBorder(const CCellRange& cr,Border& bdr)const;
    BOOL Range_GetBottomBorder(const CCellRange& cr,Border& bdr)const;
    BOOL Range_GetLeftBorder(const CCellRange& cr,Border& bdr)const;
    BOOL Range_GetRightBorder(const CCellRange& cr,Border& bdr)const;
    BOOL Range_GetInsideHorizontalBorder(const CCellRange& cr,Border& bdr)const;
    BOOL Range_GetInsideVerticalBorder(const CCellRange& cr,Border& bdr)const;
    //StyleID_t Style_GetSheetStyleID()const
    //{
    //  return nTableStyleID_;
    //}
    void EnumCells(vector<pair<CCell,const CellStore*> >& vec,const CCellRange* pClipRange=NULL)const;
    
    void Range_ClearStyles(const CCellRange& cr);
    void Range_ClearContents(const CCellRange& cr);
    void Range_Clear(const CCellRange& cr);
    void Range_SetAlignment(const CCellRange& cr,StyleDesc _style,UINT nAlignmentSetMask);
    void Range_SetPattern(const CCellRange& cr,StyleDesc _style,UINT nPatternMask=0);
    void Range_SetFont(const CCellRange& cr,const LOGFONT& lf,ColorTable::TIndex clrIdx,UINT nFontSetMask);
    void Range_SetBorder(const CCellRange& cr,CellBorder cellbdr,Border InsideHorizontal,Border InsideVertical,UINT nMask);
    void Range_SetBorderLeft(const CCellRange& cr,Border bdr);
    void Range_SetBorderTop(const CCellRange& cr,Border bdr);
    void Range_SetBorderRight(const CCellRange& cr,Border bdr);
    void Range_SetBorderBottom(const CCellRange& cr,Border bdr);
    void Range_SetBorderInsideHorizontal(const CCellRange& cr,Border bdr);
    void Range_SetBorderInsideVertical(const CCellRange& cr,Border bdr);
    void Range_SetBorderDiagonalDown(const CCellRange& cr,Border bdr);
    void Range_SetBorderDiagonalUp(const CCellRange& cr,Border bdr);
  public:
    //return 行号小于row的第一个行高大于0的行号
    //如果无满足条件的行则返回row本身。
    int GetPreRow(int row)const ;
    
    //return 行号大于row的第一个行高大于0的行号
    //如果无满足条件的行则返回row本身。
    int GetNextRow(int row)const;
    
    //return 列号小于col的第一个列宽大于0的列号
    //如果无满足条件的列则返回col本身。
    int GetPreCol(int col)const;
    
    //return 列号大于col的第一个列宽大于0的列号
    //如果无满足条件的列则返回col本身。
    int GetNextCol(int col)const ;
    
    //return col前一个可以滚动到的列
    //遍历中应跳过列宽<=0的列
    //如果无满足条件的列则返回col本身。
    int GetPreScrollableCol(int col)const ;
    
    //return row前一个可以滚动到的行
    //遍历中应跳过行高<=0的行
    //如果无满足条件的行则返回row本身。
    int GetPreScrollableRow(int row)const ;
    
    //pair<TOOLINFO,CToolTipCtrl>* GetTooltipCtrl()
    //{
    //  return &tooltip_;
    //}
    //HTHEME get_Theme()const
    //{
    //  return hTheme_;
    //}
  public:
    //void RegisterEditor(CellType cellType,ICellEditor* pEditor)
    //{
    //  mapEditor_[cellType]=pEditor;
    //}
    //bool FindEditor(CellType cellType,ICellEditor** ppEditor)const
    //{
    //  map<CellType,CComPtr<ICellEditor> >::const_iterator it=mapEditor_.find(cellType);
    //  if(it!=mapEditor_.end()){
    //    *ppEditor=it->second.p;
    //    (*ppEditor)->AddRef();
    //    return true;
    //  }
    //  return false;
    //}
    int ShowSetupDialog(HWND hWndParent,int flag=0);
    void Refresh();
    //void Draw(HDC hDC,GridRenderer& render,const CRgnLight& rgnClip)const;
    //void ChangeCellToolTip(const CELLHITTESTINFO& chi);
    //HeaderEvents
  public:
    //void OnThemeChanged()
    //{
    //  GridRenderer::OnThemeChanged(m_hWnd);
    //}
    void OnVisibleRowChanged(int oldTopVisScrollRow,int oldBottomVisScrollRow)//,int newTopRow,int newBottomRow)
    {
      int const nTopVisScrollRow=rh_.get_TopVisScrollRow();
      int const nBottomVisScrollRow=rh_.get_BottomVisScrollRow();
      if(!(nTopVisScrollRow==oldTopVisScrollRow && nBottomVisScrollRow==oldBottomVisScrollRow))
      {
        //render_.SetDirty();
        RecalcVisibleMergeCells();
      }
    }
    void OnVisibleColChanged(int oldLeftVisScrollCol,int oldRightVisScrollCol)//,int newLeftCol,int newRightCol)
    {
      int const nLeftVisScrollCol=ch_.get_LeftVisScrollCol();
      int const nRightVisScrollCol=ch_.get_RightVisScrollCol();
      if(!(nLeftVisScrollCol==oldLeftVisScrollCol&&nRightVisScrollCol==oldRightVisScrollCol))
      {
        //render_.SetDirty();
        RecalcVisibleMergeCells();
      }
    }
  private:
    void TrackDragCorner(const CELLHITTESTINFO& chi);
    void TrackDragColDivider(const CELLHITTESTINFO& chi);
    void TrackDragRowDivider(const CELLHITTESTINFO& chi);
    
    void Tooltip_ChangeCellToolTip(const CELLHITTESTINFO& chi);
    void Tooltip_ChangeDragDividerToolTip(const CClientDC& dc,const CRect& rcCell,int width,bool bDragCol=true,bool bUpdateOnly=true);
    void Tooltip_ChangeDragColToolTip(const CClientDC& dc,const CRect& rcCell,int width,bool bUpdateOnly=true)
    {
      Tooltip_ChangeDragDividerToolTip(dc,rcCell,width,true,bUpdateOnly);
    }
    void Tooltip_ChangeDragRowToolTip(const CClientDC& dc,const CRect& rcCell,int height,bool bUpdateOnly=true)
    {
      Tooltip_ChangeDragDividerToolTip(dc,rcCell,height,false,bUpdateOnly);
    }
  private:
    LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseWheel(UINT,WPARAM,LPARAM,BOOL&);
    LRESULT OnMouseEvent(UINT uMsg,WPARAM,LPARAM lParam,BOOL& bHandled);
  protected:
    //int put_TopVisScrollRowNoScroll(int nTopVisScrollRow,const CellAxisInfo* pcai=NULL);
    
    //LRESULT OnPaint(UINT,WPARAM,LPARAM,BOOL&);
    void RecalcVisibleMergeCells();
    //CCellRange get_Selection()const
    //{
    //  vector<CCellRange> vcr;
    //  get_Selection(vcr);
    //  if(!vcr.empty())
    //    return vcr[0];
    //  CCellRange cr;
    //  cr.SetNull();
    //  return cr;
    //}
    //SelectionBound& GetSelectionBound()
    //{
    //  return selectionBound_;
    //}
    //被废弃的方法
    void ExpandDirtyActiveSelection(const CCellRange* poldSelection,const CCellRange* pnewSelection)
    {
    }
  public:
    //void TrackDragCorner(const CELLHITTESTINFO& chi);
    //void TrackDragColDivider(const CELLHITTESTINFO& chi);
    //void TrackDragRowDivider(const CELLHITTESTINFO& chi);
    
  private:
    //需要对old active cell和new active cell的区域执行InvalidateRect操作
    CCell SetActiveCell(int row,int col,Int2Type<true>);
    //不需要对old active cell和new active cell的区域执行InvalidateRect操作
    CCell SetActiveCell(int row,int col,Int2Type<false>)
    {
      const int cols=get_ColHeader().get_cols();
      const int rows=get_RowHeader().get_rows();
      if(col>=cols)
        col=cols-1;
      if(row>=rows)
        row=rows-1;
#ifdef _DEBUG
      _ASSERT(col>=HEADER_COL);
      _ASSERT(col<cols);
      _ASSERT(row>=HEADER_ROW);
      _ASSERT(row<rows);
#endif
      CCell const oldCell=ActiveCell_t::GetActiveCell();
      if(col<HEADER_COL||col>=cols||row<HEADER_ROW||row>=rows)
        return oldCell;
      if(row!=oldCell.row || col!=oldCell.col){
        return ActiveCell_t::SetActiveCell(row,col);
      }
      return oldCell;
    }
  protected:
    const CellAxisInfo& RecalcCellAxisInfo(const RECT& rcli,UINT nCalcMask=CCAI_CALC_ALL);//bool bCalcVAxis=true,bool bCalcHAxis=true,bool bCalcRemainAxis=true);
  protected:
    SelectionsHandler selectionImp_;
    CellAxisInfo cellAxisInfo_;
    //GridRenderer render_;
    //ASelection* pSelection_;
    //SelectionBound selectionBound_;
    //HTHEME hTheme_;
    
    
  };
  
  ////helper
  //inline void put_ColType(Worksheet& grid,int col,ECellType cellType)
  //{
  //  //StyleID_t sid=grid.get_ColHeader().Style_GetColStyleID(col);
  //  //if(!sid)
  //  //  sid=gird.Style_GetTableStyleID();
  //  //StyleDesc style =sid?grid.Style_GetStyleByID(sid):grid.Style_GetDefaultStyle();
  //  //StyleDesc style=grid.Style_GetColStyle(col);
  //  //style.CellType_SetType(cellType);
  //  //grid.Style_SetColStyle(col,style);
  //}
  
  inline HRESULT WINAPI AtlReportError( const CLSID& clsid, UINT nID, const IID& iid,
    HRESULT hRes, HINSTANCE hInst ) {
    return 0;
  }
  inline HRESULT WINAPI AtlReportError( const CLSID& clsid, LPCSTR lpszDesc,
    DWORD dwHelpID, LPCSTR lpszHelpFile, const IID& iid, HRESULT hRes ) {
    ATLASSERT( lpszDesc != NULL );
    USES_CONVERSION;
    return 0;
  }
  inline HRESULT WINAPI AtlReportError( const CLSID& clsid, LPCSTR lpszDesc,
    const IID& iid, HRESULT hRes ) {
    return 0;
  }
  inline HRESULT WINAPI AtlReportError( const CLSID& clsid, LPCOLESTR lpszDesc,
    const IID& iid, HRESULT hRes ) {
    return 0;
  }
  inline HRESULT WINAPI AtlReportError( const CLSID& clsid, LPCOLESTR lpszDesc, DWORD dwHelpID,
    LPCOLESTR lpszHelpFile, const IID& iid, HRESULT hRes ) {
    return 0;
  }
