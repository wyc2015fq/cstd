
#if 0
//#include <atltypes.h>
//#include <atlgdi.h>
#include <algorithm>
#include "renderer.h"
#include "Worksheet.h"
#include "RgnLight.h"
#include "rect_xor.h"
#include "GdiSelectObject.h"
#include "msg.h"
#include "DrawTextAlignJustify.h"
#include "DefaultSymbols.h"
#include "CellFontSetup.h"
#include "Workbook.h"

class HeaderSetupDC
{
  HDC hDC_;
  int nBKMode_;
  HFONT hFont_;
  //HPEN hPen_;
public:
  HeaderSetupDC(HDC hDC):hDC_(hDC)
  {
    CDCHandle dc(hDC);
    nBKMode_=::SetBkMode(hDC,TRANSPARENT);
    hFont_=(HFONT)SelectObject(hDC,::GetStockObject(DEFAULT_GUI_FONT));
    //hPen_=(HPEN)SelectObject(hDC,::CreatePen(PS_SOLID,0,RGB(0,0,0)));
  }
  ~HeaderSetupDC()
  {
    if(nBKMode_)
      ::SetBkMode(hDC_,nBKMode_);
    if(hFont_ && HGDI_ERROR!=hFont_)
      SelectObject(hDC_,::GetStockObject(DEFAULT_GUI_FONT));
    //DeleteObject(SelectObject(hDC_,hPen_));
  }
};

ESelectionState GridRenderer::SetupRowHeaderDE(const Worksheet* pSheet,const Selections* pCurSelections,int row,COLORREF& backColor)const
{
  //bFullInselection=FALSE;
  //int const leftCol=activeSelection_.LeftCol();
  //int const minRow=activeSelection_.TopRow();
  //int const maxRow=activeSelection_.BottomRow();
  if(pSheet->get_ShowSelection()){
    //const Selections* pCurSelections=pSheet->get_Selections();
    BOOL const bFullInselection=pCurSelections->IsFullColSelectedAtRow(row) ;//>CellInSelections(row,HEADER_COL);
    const bool bInSelection=bFullInselection?true:pCurSelections->RowInSelections(row);
    if(bFullInselection){
      backColor=HeaderColors::CLR_ACTIVE;
      return EST_IN_SELECTION;
    }else if(bInSelection||row==pSheet->GetActiveRow()){
      backColor=HeaderColors::CLR_SELECTION;//RGB(0,0,0xff):RGB(255,192,111);
      return EST_IN_PROJECTION_OF_SELECTION;
    }else
      backColor=HeaderColors::CLR_BKGND;//RGB(200,200,200);
  }else
    backColor=HeaderColors::CLR_BKGND;//RGB(200,200,200);
  return EST_NOT_IN_SELECTION;
}

//[firstRow,lastRow]
void GridRenderer::draw_RowHeader(CDCHandle& dc,LPCRECT lprc,int firstRow,int lastRow,CRgnLight const* pRgnClip)
{
  const Selections* pSelections=pSheet_->get_Selections();
  const RowHeader& rh=pSheet_->get_RowHeader();
  //int const rh.get_rows()=rh.get_rows();
  if(0==rh.get_rows()) return;
  BOOL const bAutoAppendRow=pSheet_->get_AutoAppendRow();

  int const topRow=firstRow;//rh.get_TopVisScrollRow();
  //CCellRange const selection=pSheet_->get_Selection();

  CCell const activeCell=pSheet_->GetActiveCell();
  _ASSERT(topRow<rh.get_rows());
  HeaderSetupDC setupDC(dc);
  GdiSelectObject<> gso(dc.m_hDC,CreatePen(PS_SOLID,0,HeaderColors::CLR_GRID));
  RECT rcCell={lprc->left,lprc->top,lprc->right,lprc->top};


  TCHAR text[MAX_CELLTEXT];
  /*
  DrawHeaderCellStruct dhcs;
  DrawTextStruct1 dts;
  dts.lpszCellText=text;
  dts.uDrawTextFormat=DT_VCENTER|DT_SINGLELINE|DT_CENTER;

  dhcs.pDTS=&dts;
  dhcs.hDC=dc.m_hDC;
  //dhcs.bHeaderCorner=FALSE;
  //dhcs.bRowHeader=TRUE;
  dhcs.headerType=MHT_ROW_HEADER;
  dhcs.activeRow=activeCell.row;
  dhcs.activeCol=activeCell.col;
  */
  const CComPtr<ICellSymbol> pSymbol=cellTypeFactory_.GetSymbol(ECT_ROWHEADER);
  DrawCellInfo dci;
  ZeroMemory(&dci,sizeof(dci));
  for(int i=topRow;i<=lastRow && rcCell.bottom<=lprc->bottom;++i,rcCell.top=rcCell.bottom)
  {
    int const hi=rh.get_RowHeight(i);
    rcCell.bottom=rcCell.top+hi;

    if(RectInRgn(pRgnClip, rcCell)){
      int nCount=rh.get_text(i,text);
      if(bAutoAppendRow && i==rh.get_rows()-1){
        lstrcpy(text,_T("*"));
        nCount=1;
      }
      CComVariant val(text);
      dci.rcCell=rcCell;
      dci.pdata=&val;
      dci.nSelState=SetupRowHeaderDE(pSheet_,pSelections,i,dci.clrBkgnd);
      pSymbol->Draw(dc,i,HEADER_COL,&dci);
      ////dhcs.bkclr=backColor;
      //dhcs.fillRect=rcCell;
      //dts.nTextLen=nCount;
      //pSheet_->SendNotifyMessageToListener(i,-1,OCN_DRAWHEADCELL,(LPARAM)&dhcs);
      //if(!dhcs.bHandled){
      //  //backColor=dhcs.bkclr;
      //  draw_HeaderCell(dc,text,nCount,&rcCell,dhcs.bkclr);
      //}
      //AtlTrace(_T("\nRowHeader::draw row=%d"),i+1);
    }
  }
  //draw remain part
  //if(i==rh.get_rows()){
  //  rcCell.bottom=lprc->bottom;
  //  //if(rect_intersect(*pRgnClip,&rcCell))
  //  FillRect(dc.m_hDC,&rcCell,(HBRUSH) GetStockObject(GRAY_BRUSH));
  //}
}
/*
void SetupColHeaderDE(Worksheet* pSheet,CCellRange const& selection,CCell activeCell,int col,BOOL& bFullInselection)
{
  bFullInselection=FALSE;
  if(pSheet->get_ShowSelection()){
    if(col>=selection.LeftCol() && col<=selection.RightCol()){
      bFullInselection=HEADER_ROW==selection.TopRow();
      backColor=HEADER_ROW==selection.TopRow()?HeaderColors::CLR_ACTIVE:HeaderColors::CLR_SELECTION;
    }else if(col==activeCell.col){
      backColor=HeaderColors::CLR_SELECTION;
    }else
      backColor=HeaderColors::CLR_BKGND;
  }else{
    backColor=HeaderColors::CLR_BKGND;
  }
}
*/
ESelectionState GridRenderer::SetupColHeaderDE(const Worksheet* pSheet,const Selections* pCurSelections,int col,COLORREF& backColor)const
{
  if(pSheet->get_ShowSelection()){
    //const Selections* pCurSelections=pSheet->get_Selections();
    BOOL const bFullInselection=pCurSelections->IsFullRowSelectedAtCol(col);//CellInSelections(HEADER_ROW,col);
    const bool bInSelection=bFullInselection?true:pCurSelections->ColInSelections(col);
    if(bFullInselection){
      backColor=HeaderColors::CLR_ACTIVE;
      return EST_IN_SELECTION;
    }else if(bInSelection||col==pSheet->GetActiveCol()){
      backColor=HeaderColors::CLR_SELECTION;//RGB(0,0,0xff):RGB(255,192,111);
      return EST_IN_PROJECTION_OF_SELECTION;
    }else
      backColor=HeaderColors::CLR_BKGND;//RGB(200,200,200);
  }else
      backColor=HeaderColors::CLR_BKGND;//RGB(200,200,200);
  return EST_NOT_IN_SELECTION;
}

//[leftCol,rightCol]
void GridRenderer::draw_ColHeader(CDCHandle& dc,LPCRECT lprc,int leftCol,int rightCol,CRgnLight const* pRgnClip)
{
  const CComPtr<ICellSymbol> pSymbol=cellTypeFactory_.GetSymbol(ECT_COLHEADER);
  if(!pSymbol){
    return;
  }
  //SelectionsHandler& selection=pSheet->get_Selection();
  const Selections* pSelections=pSheet_->get_Selections();

  GdiSelectObject<> gso(dc.m_hDC,CreatePen(PS_SOLID,0,HeaderColors::CLR_GRID));

  //CCellRange const selection=pSheet_->get_Selection();
  CCell const activeCell=pSheet_->GetActiveCell();
  const ColHeader& ch=pSheet_->get_ColHeader();
  //int const leftCol=ch.get_LeftVisScrollCol();
  int const cols=ch.get_cols();

  HeaderSetupDC setupDC(dc.m_hDC);
  RECT rcCell={lprc->left,lprc->top,lprc->left,lprc->bottom};
  TCHAR text[MAX_CELLTEXT];

  /*
  DrawHeaderCellStruct dhcs;
  DrawTextStruct1 dts;
  dts.lpszCellText=text;
  dts.uDrawTextFormat=DT_VCENTER|DT_SINGLELINE|DT_CENTER;
  dhcs.pDTS=&dts;
  dhcs.hDC=dc.m_hDC;
  //dhcs.bHeaderCorner=FALSE;
  //dhcs.bRowHeader=FALSE;
  dhcs.headerType=MHT_COL_HEADER;
  dhcs.activeRow=activeCell.row;
  dhcs.activeCol=activeCell.col;
  */
  DrawCellInfo dci;
  ZeroMemory(&dci,sizeof(dci));
  for(int i=leftCol;i<=rightCol && rcCell.right<=lprc->right;++i,rcCell.left=rcCell.right)
  {
    int const wi=ch.get_ColWidth(i);
    rcCell.right=rcCell.left+wi;
    //if(rect_intersect(*pRgnClip,&rcCell)){
    if(RectInRgn(pRgnClip, rcCell)){
      int const nCount=ch.get_text(i,text);
      //selection.SetupColHeaderDE(i);
      //dhcs.bHandled=FALSE;

      CComVariant val(text);
      dci.rcCell=rcCell;
      dci.pdata=nCount>0?&val:NULL;
      dci.nSelState=SetupColHeaderDE(pSheet_,pSelections,i,dci.clrBkgnd);
      pSymbol->Draw(dc,HEADER_ROW,i,&dci);

      //if(bFullInSelection){
      //  dhcs.selType=MHST_IN_SELECTION;
      //}else if(selection.ColInside(i)){
      //  dhcs.selType=MHST_IN_PROJECTION_OF_SELECTION;
      //}else
      //  dhcs.selType=MHST_NOT_IN_SELECTION;
      //dhcs.bInSelection=selection.ColInside(i);
      //dhcs.bFullInSelection=HEADER_ROW==selection.TopRow();
      //dhcs.bkclr=backColor;
      /*
      dhcs.fillRect=rcCell;
      dts.nTextLen=nCount;
      pSheet_->SendNotifyMessageToListener(-1,i,OCN_DRAWHEADCELL,(LPARAM)&dhcs);
      if(!dhcs.bHandled){
        //backColor=dhcs.bkclr;
        draw_HeaderCell(dc,text,nCount,&rcCell,dhcs.bkclr);
      }
      */
      //AtlTrace(_T("\nColHeader::draw col=%s rcCell(%d,%d,%d,%d)"),text,rcCell.left,rcCell.top,rcCell.right,rcCell.bottom);
    }
  }
  //if(i==cols){//draw remain part
  //  rcCell.right=lprc->right;
  //  FillRect(dc.m_hDC,&rcCell,(HBRUSH) GetStockObject(GRAY_BRUSH));
  //}
}
void GridRenderer::calc_cells_session(CRgnLight const* pRgnClip,int firstCol,int firstRow,int lastCol,int lastRow,LPCRECT lprcContent,VisibleCellsSession& session)const
{
  const RowHeader& rh=pSheet_->get_RowHeader();
  const ColHeader& ch=pSheet_->get_ColHeader();
  session.clear();
  int const topRow=firstRow;//rh.get_TopVisScrollRow();
  int const bottomRow=lastRow;//rh.get_BottomVisScrollRow();
  int const leftCol=firstCol;//ch.get_LeftVisScrollCol();
  int const rightCol=lastCol;//ch.get_RightVisScrollCol();
  _ASSERT(bottomRow>=topRow);
  _ASSERT(rightCol>=leftCol);
  session.Reset(bottomRow-topRow+1,rightCol-leftCol+1,topRow,leftCol);

  RECT rcCell=*lprcContent;
  //CMergeCell* merge=NULL;
  const VisibleMergeCellMgr& visMergeCells=pSheet_->GetVisibleMergeCellMgr();
  for(int row=topRow;row<=bottomRow;++row,rcCell.top=rcCell.bottom){
    rcCell.bottom=rcCell.top+rh.get_RowHeight(row);

    //if(rcCell.bottom==rcCell.top)
    //  continue;

    rcCell.left=lprcContent->left;
    for(int col=leftCol;col<=rightCol;++col,rcCell.left=rcCell.right)
    {
      const CCell cell(row,col);
      CellItem&   item=session.item(cell);
      item.clear();
      rcCell.right=rcCell.left+ch.get_ColWidth(col);

      //item.pMerge=pMerge;
      item.rcCell=rcCell;
      if(RectInRgn(pRgnClip, rcCell)){
        const CMergeCell* pMerge=visMergeCells.GetMergeCells(cell);
        if(pMerge){
          int const minR=pMerge->TopRow();
          int const minC=pMerge->LeftCol();
          int const maxR=pMerge->BottomRow();
          int const maxC=pMerge->RightCol();
          RECT rcMerge=rcCell;//pSheet_->raw_GetCellRect(cell);
          if(minC!=col){
            rcMerge.left-=ch.get_DiffWidths(minC,col);
          }
          if(maxC!=col){
            rcMerge.right+=ch.get_DiffWidths(col+1,maxC+1);
          }
          if(minR!=row){
            rcMerge.top-=rh.get_DiffHeights(minR,row);
          }
          if(maxR!=row)
            rcMerge.bottom+=rh.get_DiffHeights(row+1,maxR+1);

          item.put_MergeRect(rcMerge);
        }else{
          item.put_MergeRect(rcCell);
        }
        item.SetInClip();
        item.SetCSPack(pSheet_->GetCellStore(cell,ECSM_STYLE_VALID|ECSM_CELLTYPE_VALID,pMerge));
      }
      //session.push_back(item);
    }
    //bNotCalcVisibleCols=false;
  }
  //session.put_cols(nVisibleCols);
  //session.Sort();
}

struct SessionSorter
{
  Worksheet* pSheet;
  SessionSorter(Worksheet* pSheet):pSheet(pSheet)
  {
  }
  template<class T>
    bool operator()(T const& l,T const& r)const
  {
    style::StyleDesc const& s1=l.raw_style();
    style::StyleDesc const& s2=r.raw_style();
    return s1.GetBorders()->max_weight()<s2.GetBorders()->max_weight();
  }
};

UINT GetDrawTextFormat(EXlHAlign xa)
{
  UINT nFormat=DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
  switch(xa)
  {
  case xlHAlignJustify:
  case xlHAlignCenter:
    nFormat|=DT_CENTER;
    break;
  case xlHAlignLeft:
    nFormat|=DT_LEFT;
    break;
  case xlHAlignRight:
    nFormat|=DT_RIGHT;
    break;
  //case xlHAlignJustify:////分散对齐
  //  {  
  //    if(DrawTextAlignJustify(hDC,rc,lpText,nCount,nFormat)){
  //      return;
  //    }else{
  //      nFormat|=DT_CENTER;
  //    }
  //  }break;;
  }
  return nFormat;
}
//用来代替方法draw_content
//void GridRenderer::draw_session(const Worksheet* pSheet,const Selections* pCurSelections,CDCHandle& dc,VisibleCellsSession& session,const RowHeader& rh,const ColHeader& ch,LPCRECT /*lprcRange*/,CRgnLight const* pRgnClip,EGridPart ePart,const CCellRange& activeSelection/*,const CCellRange& clipRange*/)
void GridRenderer::draw_session(CDCHandle& dc,VisibleCellsSession& session,EGridPart ePart,const CCellRange& activeSelection)
{
  const Selections* pSelections=pSheet_->get_Selections();
  const RowHeader& rh=pSheet_->get_RowHeader();
  const ColHeader& ch=pSheet_->get_ColHeader();
  HeaderSetupDC setupDC(dc);
  GdiSelectObjectPlain gso(dc,PenFactory::Instance().gridLinePen);

  const int nBottomRow=session.BottomRow();
  const int nLeftCol=session.LeftCol();
  const int nRightCol=session.RightCol();
  //const int nCols=session.get_cols();
  //const int nRows=session.get_rows();
  //int const rh.get_FreezeBottomRow()=pSheet_->get_RowHeader().get_FreezeBottomRow();
  //int const ch.get_FreezeRightCol()=pSheet_->get_ColHeader().get_FreezeRightCol();

  //for(size_t i=0;i<session.size();++i)
  //const CCell topLeft=session.TopLeft();
  for(int row=session.TopRow();row<=nBottomRow;++row)
  {
    //const int row=topLeft.row+r;//cell.row;
    //if(row<clipRange.TopRow())
    //  continue;
    //else if(row>clipRange.BottomRow())
    //  break;

    for(int col=nLeftCol;col<=nRightCol;++col)
    {
      //if(col<clipRange.LeftCol())
      //  continue;
      //else if(col>clipRange.RightCol())
      //  break;
      const CCell cell(row,col);
      CellItem& item=session.item(cell);
      if(item.IsInClip()){
        draw_cell(dc,cell,item,activeSelection,pSelections,session);
      }
    }
  }
}

Border GridRenderer::VisibleCellsSession::GetLeftBorder(const Worksheet* pSheet,CCell cell)
{
  CellItem& it=item(cell);
  pair<CellBorder,UINT> pr=it.GetBorder();
  if(xlEdgeLeft&pr.second){
    return pr.first.left;
  }else{
    Border bdr;
    if(!it.IsCSPackValid()){
      _ASSERT(!it.IsInClip());
      it.SetCSPack(pSheet->GetCellStore(cell,ECSM_STYLE_VALID));
    }
    //const CCell _cell(cell.row,it.pMergeCells?it.pMergeCells->LeftCol()-1:cell.col-1);
    const CellStorePack& csp=it.GetCSPack();
    const CMergeCell* pMerge=csp.GetMerge();//it.pMerge;
    if(pMerge)
      cell.col=pMerge->LeftCol();
    const CCell _cell(cell.row,pSheet->GetPreCol(cell.col));
    if(_cell.col==cell.col){
      bdr=pSheet->Style_GetStyleByID(it.GetCSPack().GetStyleID()).GetBorders()->left;
    }else{
      CellItem& itl=item(CCell(_cell.row,max(_cell.col,LeftCol()-1)));
      if(!itl.IsCSPackValid()){
        _ASSERT(!itl.IsInClip());
        itl.SetCSPack(pSheet->GetCellStore(_cell,ECSM_STYLE_VALID));
      }
      StylePack sp=it.GetCSPack().GetStylePack();
      StylePack spLeft=itl.GetCSPack().GetStylePack();
      if(sp.eStorePos>spLeft.eStorePos){
        bdr=pSheet->Style_GetStyleByID(spLeft.nStyleID).GetBorders()->right;
        if(bdr.IsNull())
          bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->left;
      }else{
        bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->left;
        if(bdr.IsNull())
          bdr=pSheet->Style_GetStyleByID(spLeft.nStyleID).GetBorders()->right;
      }
      itl.SetRightBorder(bdr);
    }
    it.SetLeftBorder(bdr);
    return bdr;
  }
}
Border GridRenderer::VisibleCellsSession::GetRightBorder(const Worksheet* pSheet,CCell cell)
{
  CellItem& it=item(cell);
  pair<CellBorder,UINT> pr=it.GetBorder();
  if(xlEdgeRight&pr.second){
    return pr.first.right;
  }else{
    Border bdr;
    if(!it.IsCSPackValid()){
      _ASSERT(!it.IsInClip());
      it.SetCSPack(pSheet->GetCellStore(cell,ECSM_STYLE_VALID));
    }
    //const CCell _cell(cell.row,it.pMergeCells?it.pMergeCells->RightCol()+1:cell.col+1);
    const CellStorePack& csp=it.GetCSPack();
    const CMergeCell* pMerge=csp.GetMerge();//it.pMerge;
    if(pMerge)
      cell.col=pMerge->RightCol();
    const CCell _cell(cell.row,pSheet->GetNextCol(cell.col));
    if(_cell.col==cell.col/*get_ColHeader(pSheet).get_cols()*/){
      bdr=pSheet->Style_GetStyleByID(it.GetCSPack().GetStyleID()).GetBorders()->right;
    }else{
      CellItem& itr=item(CCell(_cell.row,min(_cell.col,RightCol()+1)));
      if(!itr.IsCSPackValid()){
        _ASSERT(!itr.IsInClip());
        itr.SetCSPack(pSheet->GetCellStore(_cell,ECSM_STYLE_VALID));
      }
      StylePack sp=it.GetCSPack().GetStylePack();
      StylePack spRight=itr.GetCSPack().GetStylePack();
      if(sp.eStorePos>spRight.eStorePos){
        bdr=pSheet->Style_GetStyleByID(spRight.nStyleID).GetBorders()->left;
        if(bdr.IsNull()){
          bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->right;
        }
      }else{
        bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->right;
        if(bdr.IsNull())
          bdr=pSheet->Style_GetStyleByID(spRight.nStyleID).GetBorders()->left;
      }
      itr.SetLeftBorder(bdr);
    }
    it.SetRightBorder(bdr);
    return bdr;
  }
}
Border GridRenderer::VisibleCellsSession::GetTopBorder(const Worksheet* pSheet,CCell cell)
{
  CellItem& it=item(cell);
  pair<CellBorder,UINT> pr=it.GetBorder();
  if(xlEdgeTop&pr.second){
    return pr.first.top;
  }else{
    Border bdr;
    if(!it.IsCSPackValid()){
      _ASSERT(!it.IsInClip());
      it.SetCSPack(pSheet->GetCellStore(cell,ECSM_STYLE_VALID));
    }
    //const CCell _cell(it.pMergeCells?it.pMergeCells->TopRow()-1:cell.row-1,cell.col);
    const CellStorePack& csp=it.GetCSPack();
    const CMergeCell* pMerge=csp.GetMerge();//it.pMerge;
    if(pMerge)
      cell.row=pMerge->TopRow();
    const CCell _cell(pSheet->GetPreRow(cell.row),cell.col);
    if(_cell.row==cell.row){
      bdr=pSheet->Style_GetStyleByID(it.GetCSPack().GetStyleID()).GetBorders()->top;
    }else{
      CellItem& itl=item(CCell(max(_cell.row,TopRow()-1),_cell.col));
      if(!itl.IsCSPackValid()){
        _ASSERT(!itl.IsInClip());
        itl.SetCSPack(pSheet->GetCellStore(_cell,ECSM_STYLE_VALID));
      }
      StylePack sp=it.GetCSPack().GetStylePack();
      StylePack spTop=itl.GetCSPack().GetStylePack();
      if(sp.eStorePos>spTop.eStorePos){
        bdr=pSheet->Style_GetStyleByID(spTop.nStyleID).GetBorders()->bottom;
        if(bdr.IsNull())
          bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->top;
      }else{
        bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->top;
        if(bdr.IsNull())
          bdr=pSheet->Style_GetStyleByID(spTop.nStyleID).GetBorders()->bottom;
      }
      itl.SetBottomBorder(bdr);
    }
    it.SetTopBorder(bdr);
    return bdr;
  }
}
Border GridRenderer::VisibleCellsSession::GetBottomBorder(const Worksheet* pSheet,CCell cell)
{
  CellItem& it=item(cell);
  pair<CellBorder,UINT> pr=it.GetBorder();
  if(xlEdgeBottom&pr.second){
    return pr.first.bottom;
  }else{
    Border bdr;
    if(!it.IsCSPackValid()){
      _ASSERT(!it.IsInClip());
      it.SetCSPack(pSheet->GetCellStore(cell,ECSM_STYLE_VALID));
    }
    const CellStorePack& csp=it.GetCSPack();
    const CMergeCell* pMerge=csp.GetMerge();//it.pMerge;
    if(pMerge)
      cell.row=pMerge->BottomRow();
    const CCell _cell(pSheet->GetNextRow(cell.row),cell.col);
    if(_cell.row==cell.row/*get_RowHeader(pSheet).get_rows()*/){
      bdr=pSheet->Style_GetStyleByID(it.GetCSPack().GetStyleID()).GetBorders()->bottom;
    }else{
      CellItem& itr=item(CCell(min(_cell.row,BottomRow()+1),_cell.col));
      if(!itr.IsCSPackValid()){
        _ASSERT(!itr.IsInClip());
        itr.SetCSPack(pSheet->GetCellStore(_cell,ECSM_STYLE_VALID));
      }
      StylePack sp=it.GetCSPack().GetStylePack();
      StylePack spBottom=itr.GetCSPack().GetStylePack();
      if(sp.eStorePos>spBottom.eStorePos){
        bdr=pSheet->Style_GetStyleByID(spBottom.nStyleID).GetBorders()->top;
        if(bdr.IsNull()){
          bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->bottom;
        }
      }else{
        bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->bottom;
        if(bdr.IsNull())
          bdr=pSheet->Style_GetStyleByID(spBottom.nStyleID).GetBorders()->top;
      }
      itr.SetTopBorder(bdr);
    }
    it.SetBottomBorder(bdr);
    return bdr;
  }
}

//void GridRenderer::draw_cell(CDCHandle dc,const Worksheet* pSheet,CCell cell,CellItem& item,const CCellRange& activeSelection,CRgnLight const* pRgnClip,const Selections* pCurSelections,VisibleCellsSession& session)
void GridRenderer::draw_cell(CDCHandle dc,CCell cell,CellItem& item,const CCellRange& activeSelection,const Selections* pSelections,VisibleCellsSession& session)
{
  const RowHeader& rh=pSheet_->get_RowHeader();
  const ColHeader& ch=pSheet_->get_ColHeader();
  //const Selections* pSelections=pSheet_->get_Selections();
  const BOOL bOwnerData=pSheet_->pWorkbook_->IsOwnerData();
  //const CMergeCell* merge=item.GetCSPack().GetMerge();
  const CMergeCell* merge=item.GetCSPack().GetMerge();
  //const pair<const StyleDesc*,const CComVariant*> pr=cellStoreCache_.GetCellStore(pSheet_,merge?merge->TopLeft():CCell(row,col));
  //const pair<const StyleDesc*,const CComVariant*> pr=merge?mergeCellCache_.GetCellStore(pSheet_,merge):cellStoreCache_.GetCellStore(pSheet_,cell);
  const CellStorePack& pr=item.GetCSPack();//merge?mergeCellCache_.GetCellStore(pSheet_,merge):cellStoreCache_.GetCellStore(pSheet_,cell);
  const style::StyleDesc& style=pSheet_->Style_GetStyleByID(pr.GetStylePack().nStyleID);//item.GetStyle(pSheet_,row,col);
  const RECT rcCell= item.get_MergeRect();//item.rcRender;

  const ESelectionMode esm=pSheet_->GetWorkbook()->get_SelectionMode();
  //test

  //TCHAR text[MAX_CELLTEXT];
  BOOL const bShowGrid=pSheet_->get_ShowGrid();
  BOOL const bShowSelection=pSheet_->get_ShowSelection();
  
  /*DrawCellStruct*/
  CComVariant vData;
  DrawCellInfo dcs;
  ZeroMemory(&dcs,sizeof(dcs));
  //dcs.hDC=dc.m_hDC;
  //pSheet_->GetActiveCell(dcs.activeRow,dcs.activeCol);
  //dcs.textRect=rcCell;
  //dcs.fMergeCell=NULL!=merge;
  //dcs.nCellType=pr.GetCellTypePack().cellType;//style.CellType_GetType();
  dcs.pMerge=merge?merge->get():NULL;
  //dcs.cellType=pr.GetCellTypePack().cellType;
  dcs.rcCell=rcCell;
  const BOOL bActive=pSheet_->GetActiveCell()==cell;
  dcs.nSelState=bActive?(EST_ACTIVECELL):(pSelections->CellInSelections(cell)?EST_IN_SELECTION:EST_NOT_IN_SELECTION);
  //dcs.bSelected=pSelections->CellInSelections(cell);

  const COLORREF cellBKClr=style.BKGND_HasColor()?style.BKGND_GetColor():Colors::CLR_BKGND;
  if(merge&&(activeSelection.Inside(merge->TopLeft())&&activeSelection.Inside(cell))){
    dcs.clrBkgnd=GetBKColor(pSheet_,merge->TopRow(),merge->LeftCol(),merge,cellBKClr);
  }else
    dcs.clrBkgnd=GetBKColor(pSheet_,cell.row,cell.col,merge,cellBKClr);

  //dcs.rcCell=item.rcCell;
  if(dcs.rcCell.right > dcs.rcCell.left+1 && dcs.rcCell.bottom > dcs.rcCell.top+1){
  ////draw text
  //if(dcs.textRect.bottom>dcs.textRect.top-1 && dcs.textRect.right>dcs.textRect.left-1){
    int const _row=merge?merge->TopRow():cell.row;
    int const _col=merge?merge->LeftCol():cell.col;
    //int nCount=0;
    if(bOwnerData){
      //nCount=pSheet_->GetCellText(merge?merge->TopLeft():cell,text,true);
      if(pSheet_->GetCellText(merge?merge->TopLeft():cell,&vData,EGCVD_RENDER))
        dcs.pdata=&vData;
    }else{
      dcs.pdata=pr.GetCellValPack().pVal;
      //text[0]=0;
      //CComVariant vText=*pr.GetCellValPack().pVal;
      //if(SUCCEEDED(vText.ChangeType(VT_BSTR))){
      //  CString str(vText.bstrVal);
      //  lstrcpy(text,str);
      //  nCount=str.GetLength();
      //}
    }
    //DrawTextStruct dts;
    //dts.lpszCellText=text;
    //dts.nTextLen=nCount;
    //dcs.pDTS=&dts;
    dcs.cellType=static_cast<ECellType>(pr.GetCellTypePack().cellType);//_pStyle->CellType_GetType();
    dcs.hAlign=style.Align_GetHorizontal();
    dcs.vAlign=style.Align_GetVertical();

    //if(merge)
    {
      CRgn rgn=CreateRectRgn(item.rcCell.left,item.rcCell.top,item.rcCell.right,item.rcCell.bottom);
      dc.SelectClipRgn(rgn);
    }
    draw_cell_inner(dc,merge,_row,_col,dcs,style);
    if(bShowSelection)
      draw_cell_shrink(pSheet_,cell.row,cell.col,pSelections,dc,activeSelection,item,dcs);
    //if(merge)
    {
      dc.SelectClipRgn(NULL);
    }
  }
  //CellBorder const& cellBorder=*style.GetBorders();
  const CellBorder cellBorder=session.GetCellBorder(pSheet_,cell);
  //cellBorder.top=session.
  //画网格线
  if(bShowGrid){
    RECT _rc=item.rcCell;
    if(cellBorder.bottom.IsNull()||xlLineStyleNone==cellBorder.bottom.get_LineStyle()){
      if(!merge || merge && cell.row==merge->BottomRow()){
        dc.MoveTo(_rc.left,_rc.bottom-1);
        dc.LineTo(_rc.right+1,_rc.bottom-1);
      }
    }
    if(cellBorder.right.IsNull()||xlLineStyleNone==cellBorder.right.get_LineStyle()){
      const bool bDrawRight=!merge || cell.col==merge->RightCol()
        ||pSheet_->GetNextCol( cell.col)== cell.col;
      if(bDrawRight){
        dc.MoveTo(_rc.right-1,_rc.top);
        dc.LineTo(_rc.right-1,_rc.bottom+1);
      }
    }
  }
  draw_cell_border(dc,cell,session,&item.rcCell,cellBorder,merge);

  if(bShowSelection && !pSelections->IsMultiSelectionMode()){
    if(ESM_SINGLECELL==esm){
      if(activeSelection.TopRow()== cell.row && activeSelection.LeftCol()== cell.col){
        const RECT rc={item.rcCell.left,item.rcCell.top,item.rcCell.right-1,item.rcCell.bottom-1};
        dc.DrawFocusRect(&rc);
      }
    }else{
      if(!(activeSelection.LeftCol()>=ch.get_cols() || activeSelection.TopRow()>=rh.get_rows()
        || activeSelection.RightCol()<0 || activeSelection.BottomRow()<0))
      {
        draw_cell_DragBounds(pSheet_,dc,activeSelection,merge,item.rcCell,cell.row,cell.col,rh.get_rows(),ch.get_cols(),cellBKClr);
      }
    }
  }
  //画冻结线
  GdiSelectObjectPlain sop(dc,PenFactory::Instance().squareBlackPen1);
  if(ch.get_FreezeRightCol()==cell.col){
    dc.MoveTo(item.rcCell.right-1,item.rcCell.top);
    dc.LineTo(item.rcCell.right-1,item.rcCell.bottom);
  }
  if(rh.get_FreezeBottomRow()==cell.row){
    dc.MoveTo(item.rcCell.left,item.rcCell.bottom-1);
    dc.LineTo(item.rcCell.right,item.rcCell.bottom-1);
  }
}


ECellSelectionRelations GridRenderer::GetCellSelectionRelations(const Worksheet* pSheet,const CCellRange& cr,int row,int col)const
{
  int ecsr=0;
  int left=pSheet->GetNextCol(cr.LeftCol()-1);
  if(!cr.ColInside(left))
    left=cr.LeftCol();
  int top=pSheet->GetNextRow(cr.TopRow()-1);
  if(!cr.RowInside(top))
    top=cr.TopRow();
  int right=pSheet->GetPreCol(cr.RightCol()+1);
  if(!cr.ColInside(right))
    right=cr.RightCol();
  int bottom=pSheet->GetPreRow(cr.BottomRow()+1);
  if(!cr.RowInside(bottom))
    bottom=cr.BottomRow();
  //const SelectionsHandler& ss=pSheet->get_Selections();
  int outerLeft=pSheet->GetPreCol(left);
  int outerRight=pSheet->GetNextCol(right);
  int outerTop=pSheet->GetPreRow(top);
  int outerBottom=pSheet->GetNextRow(bottom);
  if(left==outerLeft) --outerLeft;
  if(right==outerRight)++outerRight;
  if(top==outerTop)--outerTop;
  if(bottom==outerBottom)++outerBottom;
  if(outerTop==row&&outerLeft==col)
    return ECSR_OUTER_LEFTTOP;
  else if(outerTop==row&&outerRight==col)
    return ECSR_OUTER_RIGHTTOP;
  else if(outerBottom==row&&outerLeft==col)
    return ECSR_OUTER_LEFTBOTTOM;
  else if(outerBottom==row&&outerRight==col)
    return ECSR_OUTER_RIGHTBOTTOM;
  if(cr.RowInside(row)){
    if(left==col)
      ecsr|=ECSR_INNER_LEFT;
    if(outerLeft==col)
      ecsr|=ECSR_OUTER_LEFT;
    if(right==col)
      ecsr|=ECSR_INNER_RIGHT;
    if(outerRight==col)
      ecsr|=ECSR_OUTER_RIGHT;
  }
  if(cr.ColInside(col)){
    if(top==row)
      ecsr|=ECSR_INNER_TOP;
    if(outerTop==row)
      ecsr|=ECSR_OUTER_TOP;
    if(bottom==row)
      ecsr|=ECSR_INNER_BOTTOM;
    if(outerBottom==row)
      ecsr|=ECSR_OUTER_BOTTOM;
  }
  return (ECellSelectionRelations)ecsr;
}
void GridRenderer::draw_cell_DragBounds(const Worksheet* pSheet,CDCHandle& dc,const CCellRange activeSelection,const CMergeCell* merge,RECT rcCell,int row,int col,int /*rows*/,int /*cols*/,COLORREF bkclr)const
{
  //_ASSERT(!(activeSelection.LeftCol()>=cols || activeSelection.TopRow()>=rows|| activeSelection.RightCol()<0 || activeSelection.BottomRow()<0));
  LPCRECT lprc=&rcCell;
  //GdiSelectObjectPlain sop(dc,PenFactory::Instance().squareBlackPen1);
  const int ecsr=(int)GetCellSelectionRelations(pSheet,activeSelection,row,col);
  const GDISetROP2 rop2(dc.m_hDC,R2_XORPEN);

  //左上外角
  if(ECSR_OUTER_LEFTTOP==ecsr){
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().squareWhitePen2);
    dc.MoveTo(lprc->right-1,lprc->bottom-1);
    dc.LineTo(lprc->right,lprc->bottom-1);
    return;
    /*
    GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
    dc.MoveTo(lprc->right-2,lprc->bottom-2);
    dc.LineTo(lprc->right-1,lprc->bottom-2);
    sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
    dc.MoveTo(lprc->right-2,lprc->bottom-1);
    dc.LineTo(lprc->right-1,lprc->bottom-1);
    //sop.SelectObject(CreatePen(PS_SOLID,1,RGB(255,0,0)));
    dc.MoveTo(lprc->right-1,lprc->bottom-2);
    dc.LineTo(lprc->right-1,lprc->bottom);
    return;
    */
  }

  if(ECSR_OUTER_RIGHTTOP==ecsr){
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().whitePen1);
    dc.MoveTo(lprc->left,lprc->bottom-2);
    dc.LineTo(lprc->left+1,lprc->bottom);
    /*
    GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
    dc.MoveTo(lprc->left,lprc->bottom-2);
    dc.LineTo(lprc->left+1,lprc->bottom-2);
    sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
    dc.MoveTo(lprc->left,lprc->bottom-1);
    dc.LineTo(lprc->left+1,lprc->bottom-1);
    */
    return;
  }
  if(ECSR_OUTER_LEFTBOTTOM==ecsr){
    /*
    GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
    dc.MoveTo(lprc->right-2,lprc->top);
    dc.LineTo(lprc->right-1,lprc->top);
    sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
    dc.LineTo(lprc->right+1,lprc->top);
    */
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().whitePen1);
    dc.MoveTo(lprc->right-2,lprc->top);
    dc.LineTo(lprc->right,lprc->top);
    return;
  }
  if(ECSR_OUTER_RIGHTBOTTOM==ecsr){
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().whitePen1);
    dc.MoveTo(lprc->left,lprc->top);
    dc.LineTo(lprc->left+2,lprc->top);
    dc.MoveTo(lprc->left,lprc->top+1);
    dc.LineTo(lprc->left+2,lprc->top+1);
    /*
    GdiSelectObjectAutoDel sop(dc,PenFactory::CreateSquarePen(PS_SOLID,2,bkclr^RGB(255,255,255)));//CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
    dc.MoveTo(lprc->left,lprc->top+1);
    dc.LineTo(lprc->left+1,lprc->top+1);
    //dc.MoveTo(lprc->left,lprc->top+2);
    //dc.LineTo(lprc->left+2,lprc->top+2);
    sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
    dc.MoveTo(lprc->left-1,lprc->top+1);
    dc.LineTo(lprc->left-1,lprc->top-1);
    dc.LineTo(lprc->left+2,lprc->top-1);
    */
    return;
  }

  if(ECSR_INNER_LEFT&ecsr){
    /*
    if(merge && merge->LeftCol()!=col){
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->left,lprc->top);
      dc.LineTo(lprc->left,lprc->bottom);
    }else{
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->left,lprc->top);
      dc.LineTo(lprc->left,lprc->bottom-1);
      sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
      dc.MoveTo(lprc->left,lprc->bottom-1);
      dc.LineTo(lprc->left+1,lprc->bottom-1);
    }
    */
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().whitePen1);
    //dc.MoveTo(lprc->left,row==activeSelection.TopRow()?lprc->top+1:lprc->top);
    dc.MoveTo(lprc->left,(ECSR_INNER_TOP&ecsr)?lprc->top+1:lprc->top);
    dc.LineTo(lprc->left,lprc->bottom);
  }
  if(ECSR_OUTER_LEFT&ecsr){
      /*
    if(merge && merge->LeftCol()-1!=col&&merge->RightCol()!=col){
      GdiSelectObjectAutoDel sop(dc,PenFactory::CreateSquarePen(PS_SOLID,2,bkclr^RGB(255,255,255)));//CreatePen(PS_SOLID,2,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->right-1,lprc->top);
      dc.LineTo(lprc->right-1,lprc->bottom);
    }else{
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->right-2,lprc->top);
      dc.LineTo(lprc->right-2,lprc->bottom-1);
      sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
      dc.MoveTo(lprc->right-1,lprc->top);
      dc.LineTo(lprc->right-1,lprc->bottom-1);
      dc.MoveTo(lprc->right-2,lprc->bottom-1);
      dc.LineTo(lprc->right,lprc->bottom-1);
    }
      */
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().squareWhitePen2);
    dc.MoveTo(lprc->right-1,lprc->top+1);
    dc.LineTo(lprc->right-1,lprc->bottom-1);
  }
  if(ECSR_INNER_RIGHT&ecsr){
    /*
    if(merge && merge->RightCol()!=col){
      GdiSelectObjectAutoDel sop(dc,PenFactory::CreateSquarePen(PS_SOLID,2,bkclr^RGB(255,255,255)));//CreatePen(PS_SOLID,2,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->right-1,lprc->top);
      dc.LineTo(lprc->right-1,lprc->bottom);
    }else{
      const int bottom=(ECSR_INNER_BOTTOM&ecsr)?(lprc->bottom-5):lprc->bottom;
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->right-2,lprc->top);
      dc.LineTo(lprc->right-2,bottom);
      sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
      dc.MoveTo(lprc->right-1,lprc->top);
      dc.LineTo(lprc->right-1,bottom);
      if(!(ECSR_INNER_BOTTOM&ecsr)){
        dc.MoveTo(lprc->right-2,bottom);
        dc.LineTo(lprc->right,bottom);
      }
    }
    */
    const int bottom=((ECSR_INNER_BOTTOM&ecsr)&&!(merge && merge->RightCol()!=col))?(lprc->bottom-5):lprc->bottom;
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().squareWhitePen2);
    //dc.MoveTo(lprc->right-1,row==activeSelection.TopRow()?lprc->top+2:lprc->top+1);
    dc.MoveTo(lprc->right-1,(ECSR_INNER_TOP&ecsr)?lprc->top+2:lprc->top+1);
    dc.LineTo(lprc->right-1,bottom);
  }
  if(ECSR_OUTER_RIGHT&ecsr){
    /*
    if(merge && merge->RightCol()+1!=col&&merge->LeftCol()!=col){
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->left,lprc->top);
      dc.LineTo(lprc->left,lprc->bottom);
    }else{
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      const int bottom=(activeSelection.BottomRow()==row)?(lprc->bottom-4):lprc->bottom-1;
      dc.MoveTo(lprc->left,lprc->top);
      dc.LineTo(lprc->left,bottom);
      if(activeSelection.BottomRow()!=row){
        sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
        dc.MoveTo(lprc->left,bottom);
        dc.LineTo(lprc->left+1,bottom);
      }else{
        dc.MoveTo(lprc->left,bottom+1);
        dc.LineTo(lprc->left+2,bottom+1);
        dc.MoveTo(lprc->left,bottom+2);
        dc.LineTo(lprc->left+2,bottom+2);
        sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
        dc.MoveTo(lprc->left,bottom+3);
        dc.LineTo(lprc->left+2,bottom+3);
      }
    }
    */
    const int bottom=(activeSelection.BottomRow()==row&&!(merge && merge->RightCol()+1!=col&&merge->LeftCol()!=col))?(lprc->bottom-4):lprc->bottom;
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().whitePen1);
    dc.MoveTo(lprc->left,lprc->top);//row==activeSelection.TopRow()?lprc->top+1:lprc->top);
    dc.LineTo(lprc->left,bottom);
    if(activeSelection.BottomRow()==row){
      //sop.SelectObject(PenFactory::Instance().squareWhitePen2);
      dc.MoveTo(lprc->left+1,bottom+1);
      dc.LineTo(lprc->left+1,bottom+4);
      dc.MoveTo(lprc->left,bottom+1);
      dc.LineTo(lprc->left,bottom+4);
    }
  }
  if(ECSR_INNER_TOP&ecsr){
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().whitePen1);
    dc.MoveTo(lprc->left,lprc->top);
    dc.LineTo(lprc->right,lprc->top);

    /*
    if(merge && merge->TopRow()!=row){
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->left,lprc->top);
      dc.LineTo(lprc->right,lprc->top);
    }else{
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->left,lprc->top);
      dc.LineTo(lprc->right-1,lprc->top);
      sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
      dc.MoveTo(lprc->right-1,lprc->top);
      dc.LineTo(lprc->right-1,lprc->top+1);
    }
    */
  }
  if(ECSR_OUTER_TOP&ecsr){
      /*
    if(merge && merge->TopRow()-1!=row&&merge->BottomRow()!=row){
      GdiSelectObjectAutoDel sop(dc,PenFactory::CreateSquarePen(PS_SOLID,2,bkclr^RGB(255,255,255)));//CreatePen(PS_SOLID,2,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->left,lprc->bottom-1);
      dc.LineTo(lprc->right,lprc->bottom-1);
    }else{
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->left,lprc->bottom-2);
      dc.LineTo(lprc->right-1,lprc->bottom-2);
      sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
      dc.MoveTo(lprc->left,lprc->bottom-1);
      dc.LineTo(lprc->right,lprc->bottom-1);
      //sop.SelectObject(CreatePen(PS_SOLID,1,RGB(255,0,0)));
      dc.MoveTo(lprc->right-1,lprc->bottom-2);
      dc.LineTo(lprc->right-1,lprc->bottom+1);
    }
      */
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().squareWhitePen2);
    dc.MoveTo(lprc->left+1,lprc->bottom-1);
    dc.LineTo(lprc->right-1,lprc->bottom-1);
  }
  if(ECSR_INNER_BOTTOM&ecsr){
    const int right=((ECSR_INNER_RIGHT&ecsr)&&!(merge && merge->BottomRow()!=row))?(lprc->right-5):lprc->right-1;
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().squareWhitePen2);
    //dc.MoveTo(lprc->left+(col==activeSelection.LeftCol()?2:1),lprc->bottom-1);
    dc.MoveTo(lprc->left+((ECSR_INNER_LEFT&ecsr)?2:1),lprc->bottom-1);
    dc.LineTo(right,lprc->bottom-1);
    if(ECSR_INNER_RIGHT&ecsr){
      dc.MoveTo(right+3,lprc->bottom-2);
      dc.LineTo(lprc->right,lprc->bottom-2);

      sop.SelectObject(PenFactory::Instance().whitePen1);
      dc.MoveTo(right+2,lprc->bottom-1);
      dc.LineTo(lprc->right,lprc->bottom-1);
    }
    /*
    if(merge && merge->BottomRow()!=row){
      GdiSelectObjectAutoDel sop(dc,PenFactory::CreateSquarePen(PS_SOLID,2,bkclr^RGB(255,255,255)));//CreatePen(PS_SOLID,2,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->left,lprc->bottom-1);
      dc.LineTo(lprc->right,lprc->bottom-1);
    }else{
      const int right=(ECSR_INNER_RIGHT&ecsr)?(lprc->right-4):lprc->right-1;
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->left,lprc->bottom-2);
      dc.LineTo(right,lprc->bottom-2);
      sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
      dc.MoveTo(lprc->left,lprc->bottom-1);
      dc.LineTo(right,lprc->bottom-1);
      if(!(ECSR_INNER_RIGHT&ecsr)){
        dc.MoveTo(right,lprc->bottom-2);
        dc.LineTo(right,lprc->bottom);
      }else{
        dc.MoveTo(right+1,lprc->bottom-1);
        dc.LineTo(lprc->right,lprc->bottom-1);
        dc.MoveTo(lprc->right-1,lprc->bottom-3);
        dc.LineTo(lprc->right-1,lprc->bottom+1);
        sop.SelectObject(PenFactory::CreateSquarePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
        dc.MoveTo(right+1,lprc->bottom-3);
        dc.LineTo(lprc->right-2,lprc->bottom-3);
        dc.MoveTo(right+1,lprc->bottom-2);
        dc.LineTo(lprc->right-2,lprc->bottom-2);
      }
    }
    */
  }
  if(ECSR_OUTER_BOTTOM&ecsr){
    const int right=((activeSelection.RightCol()==col)&&!(merge && merge->BottomRow()+1!=row&&merge->TopRow()!=row))?(lprc->right-4):lprc->right;
    GdiSelectObjectPlain sop(dc,PenFactory::Instance().whitePen1);
    dc.MoveTo(lprc->left,lprc->top);
    dc.LineTo(right,lprc->top);
    if(activeSelection.RightCol()==col){
      dc.MoveTo(right+1,lprc->top);
      dc.LineTo(lprc->right,lprc->top);
      dc.MoveTo(right+1,lprc->top+1);
      dc.LineTo(lprc->right,lprc->top+1);
    }
    /*
    if(merge && merge->BottomRow()+1!=row&&merge->TopRow()!=row){
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      dc.MoveTo(lprc->left,lprc->top);
      dc.LineTo(lprc->right,lprc->top);
    }else{
      GdiSelectObjectAutoDel sop(dc,CreatePen(PS_SOLID,1,bkclr^RGB(255,255,255)));
      const int right=(activeSelection.RightCol()==col)?(lprc->right-4):lprc->right-1;
      dc.MoveTo(lprc->left,lprc->top);
      dc.LineTo(right,lprc->top);
      if(activeSelection.RightCol()!=col){
        sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
        dc.MoveTo(right,lprc->top-1);
        dc.LineTo(right,lprc->top+1);
      }else{
        dc.MoveTo(right+1,lprc->top);
        dc.LineTo(right+3,lprc->top);
        dc.MoveTo(right+1,lprc->top+1);
        dc.LineTo(right+3,lprc->top+1);
        sop.SelectObject(CreatePen(PS_SOLID,1,bkclr^Colors::CLR_GRID));
        dc.MoveTo(lprc->right-1,lprc->top);
        dc.LineTo(lprc->right-1,lprc->top+2); 
      }
    }
    */
  }

  /*
  {
  //if(row==activeSelection.TopRow() //内上边
  //  || row==activeSelection.BottomRow()+1//外下边
  //  )
  if((ECSR_INNER_TOP&ecsr)||(ECSR_OUTER_BOTTOM&ecsr))
  {
  bool const bRightBottom=col==activeSelection.RightCol() && row==activeSelection.BottomRow()+1;
  int const x2=lprc->right-(bRightBottom?5:1);//lprc->right-1;
  dc.MoveTo(lprc->left,lprc->top);
  dc.LineTo(x2,lprc->top);

  if(bRightBottom){
  dc.MoveTo(x2+3,lprc->top+1);
  dc.LineTo(lprc->right-1,lprc->top+1);
  }

  }
  //if(row==activeSelection.TopRow()-1 //外上边
  //  || row==activeSelection.BottomRow())//内下边
  if((ECSR_OUTER_TOP&ecsr)||(ECSR_INNER_BOTTOM&ecsr))
  {
  bool const bRightBottom=col==activeSelection.RightCol() && row==activeSelection.BottomRow();
  int const x2=lprc->right-(bRightBottom?5:1);
  dc.MoveTo(lprc->left-1,lprc->bottom-1);
  dc.LineTo(x2,lprc->bottom-1);

  if(bRightBottom){
  //GdiSelectObjectPlain _sop(dc,PenFactory::Instance().squareBlackPen3);
  dc.MoveTo(x2+3,lprc->bottom-2);
  dc.LineTo(lprc->right-1,lprc->bottom-2);
  dc.MoveTo(x2+3,lprc->bottom-1);
  dc.LineTo(lprc->right-1,lprc->bottom-1);
  }
  }
  }
  //if(activeSelection.RowInside(row))
  {
  //if(col==activeSelection.LeftCol() //左内边
  //  || col==activeSelection.RightCol()+1)//右外边
  if((ECSR_INNER_LEFT&ecsr)||(ECSR_OUTER_RIGHT&ecsr))
  {
  bool const bRightBottom=row==activeSelection.BottomRow() && col==activeSelection.RightCol()+1;
  int const y2=lprc->bottom - (bRightBottom?5:1);
  //dc.MoveTo(lprc->left,lprc->top-1);
  dc.MoveTo(lprc->left,lprc->top);
  dc.LineTo(lprc->left,y2);

  if(bRightBottom){
  dc.MoveTo(lprc->left+1,y2+3);
  dc.LineTo(lprc->left+1,lprc->bottom-1);
  }
  }
  //if(col==activeSelection.LeftCol()-1 //左外边
  //  || col==activeSelection.RightCol())//右内边
  if((ECSR_INNER_RIGHT&ecsr)||(ECSR_OUTER_LEFT&ecsr))
  {
  bool const bRightBottom=row==activeSelection.BottomRow() && col==activeSelection.RightCol();
  int const y2=lprc->bottom-(bRightBottom?5:1);
  dc.MoveTo(lprc->right-1,lprc->top-1);
  dc.LineTo(lprc->right-1,y2);
  if(bRightBottom){
  //dc.MoveTo(lprc->right-2,y2+3);
  //dc.LineTo(lprc->right-2,lprc->bottom);
  }
  }
  }
  //if(col==activeSelection.RightCol()+1)
  //右下外角
  //if(row==activeSelection.BottomRow()+1)
  if(ECSR_OUTER_RIGHTBOTTOM==ecsr){
    dc.MoveTo(lprc->left-1,lprc->top+1);
    dc.LineTo(lprc->left+1,lprc->top+1);
  }
  //}
  //左下外角
  //if(row==activeSelection.BottomRow()+1 && col==activeSelection.LeftCol()-1)
  if(ECSR_OUTER_LEFTBOTTOM==ecsr){
    dc.MoveTo(lprc->right-1,lprc->top);
    dc.LineTo(lprc->right,lprc->top);
  }
  */
}


void GridRenderer::draw_cell_border_right(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder)
{
  //CCell tmpCell(cell.row,cell.col+1);
  Border bdr=cellBorder.right;
  //Border bdr=GetRightBorder(pSheet,cell);
  if(!bdr.IsNull()){
    const int wi=min(2,bdr.get_Weight());
    HPEN hPenOld=dc.SelectPen(MyCell_CreatePen(dc,bdr,wi));
    if(hPenOld){
      dc.MoveTo(lprc->right-1,lprc->top+wi-1);
      dc.LineTo(lprc->right-1,lprc->bottom);
      DeleteObject(dc.SelectPen(hPenOld));
    }
  }
}
void GridRenderer::draw_cell_border_left(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder)
{
  CCell tmpCell(cell.row,cell.col-1);
  Border bdr=cellBorder.left;
  if(!bdr.IsNull()){
    const int wi=bdr.get_Weight();
    if(wi>2){
      HPEN hPenOld=dc.SelectPen(MyCell_CreatePen(dc,bdr,1));
      if(hPenOld){
        dc.MoveTo(lprc->left,lprc->top);
        dc.LineTo(lprc->left,lprc->bottom);
        DeleteObject(dc.SelectPen(hPenOld));
      }
    }
  }
}
void GridRenderer::draw_cell_border_bottom(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder)
{
  Border bdr=cellBorder.bottom;
  //Border  bdr=GetBottomBorder(pSheet,cell);
  if(!bdr.IsNull()){
    switch(bdr.get_LineStyle())
    {
    case xlDouble:
      {

      }break;
    case xlSlantDashDot:
      break;
    default:
      const int wi=min(2,bdr.get_Weight());
      HPEN hPenOld=dc.SelectPen(MyCell_CreatePen(dc,bdr,wi));
      if(hPenOld){
        dc.MoveTo(lprc->left+wi-1,lprc->bottom-1);
        dc.LineTo(lprc->right,lprc->bottom-1);
        DeleteObject(dc.SelectPen(hPenOld));
      }
    }
  }
}
void GridRenderer::draw_cell_border_top(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder)
{
  Border bdr=cellBorder.top;
  //Border bdr=GetTopBorder(pSheet,cell);
  if(!bdr.IsNull()){
    const int wi=bdr.get_Weight();
    if(3==wi){
      HPEN hPenOld=dc.SelectPen(MyCell_CreatePen(dc,bdr,1));
      if(hPenOld){
        dc.MoveTo(lprc->left,lprc->top);
        dc.LineTo(lprc->right,lprc->top);
        DeleteObject(dc.SelectPen(hPenOld));
      }
    }
  }
}
void GridRenderer::draw_cell_border_rightbottom(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder)
{
}
void GridRenderer::draw_cell_border_leftbottom(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder)
{
}
void GridRenderer::draw_cell_border_righttop(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder)
{
}
void GridRenderer::draw_cell_border_lefttop(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,const CellBorder& cellBorder)
{
}

void GridRenderer::draw_cell_border(CDCHandle& dc,CCell cell,VisibleCellsSession& session,LPCRECT lprc,CellBorder cellBorder,const CMergeCell* pmerge)
{
  //const RowHeader& rh=pSheet_->get_RowHeader();
  //const ColHeader& ch=pSheet_->get_ColHeader();
  //if(cellBorder.left.IsNull())
  //  cellBorder.left=session.GetLeftBorder(pSheet_,cell/*,cellBorder*/);
  //if(cellBorder.top.IsNull())
  //  cellBorder.top=GetTopBorder(pSheet_,cell,cellBorder);
  //if(cellBorder.right.IsNull())
  //  cellBorder.right=GetRightBorder(pSheet_,cell,cellBorder);
  //if(cellBorder.bottom.IsNull())
  //  cellBorder.bottom=GetBottomBorder(pSheet_,cell,cellBorder);

  if(!pmerge || pmerge->LeftCol()==cell.col)
    draw_cell_border_left(dc,cell,session,lprc,cellBorder);
  if(!pmerge || pmerge->RightCol()==cell.col)
    draw_cell_border_right(dc,cell,session,lprc,cellBorder);
  if(!pmerge || pmerge->TopRow()==cell.row)
    draw_cell_border_top(dc,cell,session,lprc,cellBorder);
  if(!pmerge || pmerge->BottomRow()==cell.row)
    draw_cell_border_bottom(dc,cell,session,lprc,cellBorder);
  /*
  draw_cell_border_rightbottom(pSheet_,dc,cell,session,lprc,cellBorder);
  draw_cell_border_leftbottom(pSheet_,dc,cell,session,lprc,cellBorder);
  draw_cell_border_righttop(pSheet_,dc,cell,session,lprc,cellBorder);
  draw_cell_border_lefttop(pSheet_,dc,cell,session,lprc,cellBorder);
  */
  //if(!bottom.IsNull()){
  //  //if(!merge || merge && cell.row==merge->BottomRow()){
  //  dc.MoveTo(lprc->left,lprc->bottom-1);
  //  bottom.LineTo(dc,lprc->right,lprc->bottom-1);
  //  //dc.LineTo(_rc.right+1,_rc.bottom-1);
  //  //}
  //}
  //if(!right.IsNull()){
  //  //const bool bDrawRight=!merge || cell.col==merge->RightCol()
  //  //  ||pSheet_->GetNextCol(cell.col)==cell.col;
  //  //if(bDrawRight){
  //  dc.MoveTo(lprc->right-1,lprc->top);
  //  right.LineTo(dc,lprc->right-1,lprc->bottom);
  //  //dc.LineTo(_rc.right-1,_rc.bottom+1);
  //  //}
  //}
}
COLORREF GridRenderer::GetBKColor(const Worksheet* pSheet,int row,int col,const CCellRange* /*pmerge*/,COLORREF bkColor)const//StyleDesc const& style)const
{
  const CCell cell(row,col);
  //COLORREF bkColor=style.HasColor()?style.GetColor():Colors::CLR_BKGND;//HasBkColor()?style.GetBkColor():Colors::CLR_BKGND;
  if(pSheet->get_ShowSelection()){
    CCell acell=pSheet->GetActiveCell();
    const CCellRange* pCR=pSheet->GetMergeCells(acell);
    if(pCR){
      acell=pCR->TopLeft();
    }
    //const bool bActive=acell==cell && !pmerge?(TRUE):
    //  (pSheet->get_Selections()->CellInSelections());

    if(acell==cell){
      bkColor^=Colors::CLR_ACTIVE;
    }else if(pSheet->InSelection(row,col)){
      bkColor^=Colors::CLR_SELECTION;
    }
  }
  return bkColor;
}

//void GridRenderer::draw_cell(HDC hDC,int row,int col,LPCTSTR lpText,int nCount,LPCRECT lprc,StyleDesc styleDesc,COLORREF bkColor)
//void GridRenderer::draw_cell_shrink(CDCHandle& dc,int row,int col,DrawCellStruct& dcs,ECellBGShrinkState* peShrink,CMergeCell* pmerge)
//void GridRenderer::draw_cell_shrink(CDCHandle& dc,int row,int col,CellItem& cell,DrawCellStruct& dcs)
void GridRenderer::draw_cell_shrink(const Worksheet* pSheet,int row,int col,const Selections* pCurSelections,CDCHandle& dc,const CCellRange& activeSelection,CellItem& item,DrawCellInfo& dcs)const//,CMergeCell* pmerge)const
{
  LPCRECT lprc=&dcs.rcCell;
  //_ASSERT(lprc->right > lprc->left+1 && lprc->bottom > lprc->top+1);
  if(!(lprc->right > lprc->left+1 && lprc->bottom > lprc->top+1))
    return;
  //int const row=cell.row;
  //int const col=cell.col;
  const SelectionsHandler* pSelection=pSheet->get_Selections();
  bool const bMultiSelectionMode=pSelection->IsMultiSelectionMode();
  const CMergeCell* pmerge=item.GetCSPack().GetMerge();
  //ECellBGShrinkState* peShrink=&cell.eShrink;
  _ASSERT(pmerge?pmerge->IsNormalize():TRUE);
  BYTE const bActiveCell=dcs.nSelState&EST_ACTIVECELL;//pmerge?(pmerge->Inside(dcs.activeRow,dcs.activeCol)):(row== dcs.activeRow && col==dcs.activeCol);
  const int N_SHRINK=bMultiSelectionMode?1:(pSheet->IsSelectAll()?1:2);
  int ecs=item.eShrink;
  if(ECBSS_UNKNOWN==ecs){
    //if(!pmerge){
    //  ecs=pCurSelections->get_CellBGShrinkState(row,col);
    //}else
    if(pmerge&&activeSelection==*pmerge){
      ecs=0;
      if(col==pmerge->LeftCol() && (ECBSS_SHRINK_LEFT&pCurSelections->get_CellBGShrinkState(/*row*/pmerge->TopRow(),col)))
        ecs|=ECBSS_SHRINK_LEFT;
      if(row==pmerge->TopRow() && (ECBSS_SHRINK_TOP&pCurSelections->get_CellBGShrinkState(row,pmerge->LeftCol()/*col*/)))
        ecs|=ECBSS_SHRINK_TOP;
      if(col==pmerge->RightCol()&&(ECBSS_SHRINK_RIGHT&pCurSelections->get_CellBGShrinkState(pmerge->TopRow(),col)))
        ecs|=ECBSS_SHRINK_RIGHT;
      if(row==pmerge->BottomRow() && (ECBSS_SHRINK_BOTTOM&pCurSelections->get_CellBGShrinkState(row,pmerge->LeftCol())))
        ecs|=ECBSS_SHRINK_BOTTOM;
      if(row==pmerge->TopRow()&&col==pmerge->LeftCol()&&(ECBSS_SHRINK_LEFTTOP&pCurSelections->get_CellBGShrinkState(row,col)))
        ecs|=ECBSS_SHRINK_LEFTTOP;
      if(row==pmerge->TopRow()&&col==pmerge->RightCol()&&(ECBSS_SHRINK_RIGHTTOP&pCurSelections->get_CellBGShrinkState(row,col)))
        ecs|=ECBSS_SHRINK_RIGHTTOP;
      if(row==pmerge->BottomRow()&&col==pmerge->LeftCol()&&(ECBSS_SHRINK_LEFTBOTTOM&pCurSelections->get_CellBGShrinkState(row,col)))
        ecs|=ECBSS_SHRINK_LEFTBOTTOM;
      if(row==pmerge->BottomRow()&&col==pmerge->RightCol()&&(ECBSS_SHRINK_RIGHTBOTTOM&pCurSelections->get_CellBGShrinkState(row,col)))
        ecs|=ECBSS_SHRINK_RIGHTBOTTOM;
    }else{
      ecs=pCurSelections->get_CellBGShrinkState(row,col);
    }
    item.eShrink=(ECellBGShrinkState)ecs;
  }
  if(bActiveCell){
    if(bMultiSelectionMode){
      //HGDIOBJ oldPen=SelectObject(dc.m_hDC,PenFactory::Instance().squareBlackPen1);
      GdiSelectObjectPlain soPen(dc.m_hDC,PenFactory::Instance().squareBlackPen1);
      const int left=lprc->left+((ECBSS_SHRINK_LEFT&ecs)?N_SHRINK:(N_SHRINK-1));
      const int top=lprc->top+((ECBSS_SHRINK_TOP&ecs)?N_SHRINK:(N_SHRINK-1));
      const int right=lprc->right-((ECBSS_SHRINK_RIGHT&ecs)?(N_SHRINK+1):N_SHRINK);
      const int bottom=lprc->bottom-((ECBSS_SHRINK_BOTTOM&ecs)?(N_SHRINK+1):N_SHRINK);
      if(!pmerge){
        GdiSelectObjectPlain soPen(dc.m_hDC,PenFactory::Instance().squareBlackPen1);
        GdiSelectObjectPlain soBrush(dc.m_hDC,GetStockObject(NULL_BRUSH));
        dc.Rectangle(left,top,right,bottom);
      }else{
        //++right;
        //++bottom;
        if(col==pmerge->LeftCol())
        {
          dc.MoveTo(left,top);
          //dc.LineTo(left,bottom-(row==pmerge->BottomRow()?0:-1));
          dc.LineTo(left,row==pmerge->BottomRow()?bottom:(bottom+1));
        }
        if(row==pmerge->TopRow())
        {
          dc.MoveTo(left,top);
          //dc.LineTo(right-(col==pmerge->RightCol()?1:0),top);
          dc.LineTo(col==pmerge->RightCol()?right:right+1,top);
        }
        if(col==pmerge->RightCol())
        {
          dc.MoveTo(right-1,top);
          //dc.LineTo(right-1,bottom-(row==pmerge->BottomRow()?1:0));
          dc.LineTo(right-1,row==pmerge->BottomRow()?bottom:(bottom+1));
        }
        if(row==pmerge->BottomRow())
        {
          dc.MoveTo(left,bottom-1);
          //dc.LineTo(right-(col==pmerge->RightCol()?1:0),bottom-1);
          dc.LineTo(col==pmerge->RightCol()?right:right+1,bottom-1);
        }
      }
      //SelectObject(dc.m_hDC,oldPen);
    }
  }//else
  if(ECBSS_SHRINK_NONE!=ecs)
  {
    //const GDISetROP2 rop2(dc.m_hDC,R2_XORPEN);
    const COLORREF crShrink=RGB(255,255,255);//240,240);
    GdiSelectObjectPlain soPen(dc.m_hDC,PenFactory::Instance().squareBlackPen1);
    if((ECBSS_SHRINK_LEFT&ecs)){
      int nShrink=N_SHRINK;
      if(!bMultiSelectionMode){
        if(0==col && pSelection->CellInSelections(row,-1))
          --nShrink;
      }
      //dc.FillSolidRect(lprc->left,lprc->top,nShrink,lprc->bottom-lprc->top,RGB(255,255,255));
      dc.FillSolidRect(lprc->left,lprc->top,nShrink,lprc->bottom-lprc->top,crShrink);
    }
    if((ECBSS_SHRINK_TOP&ecs)){
      int nShrink=N_SHRINK;
      if(!bMultiSelectionMode){
        if(0==row && pSelection->CellInSelections(-1,col))
          --nShrink;
      }
      dc.FillSolidRect(lprc->left,lprc->top,lprc->right-lprc->left,nShrink,crShrink);
    }
    if((ECBSS_SHRINK_RIGHT&ecs)){
      dc.FillSolidRect(lprc->right-(1+N_SHRINK),lprc->top,N_SHRINK+1,lprc->bottom-lprc->top,crShrink);
    }
    if((ECBSS_SHRINK_BOTTOM&ecs)){
      dc.FillSolidRect(lprc->left,lprc->bottom-(1+N_SHRINK),lprc->right-lprc->left,N_SHRINK+1,crShrink);
    }
    if(ECBSS_SHRINK_LEFTTOP&ecs){
      dc.FillSolidRect(lprc->left,lprc->top,N_SHRINK,N_SHRINK,crShrink);
    }
    if(ECBSS_SHRINK_RIGHTTOP&ecs){
      dc.FillSolidRect(lprc->right-(1+N_SHRINK),lprc->top,N_SHRINK,N_SHRINK,crShrink);
    }
    if(ECBSS_SHRINK_LEFTBOTTOM&ecs){
      dc.FillSolidRect(lprc->left,lprc->bottom-(1+N_SHRINK),N_SHRINK,N_SHRINK,crShrink);
    }
    if(ECBSS_SHRINK_RIGHTBOTTOM&ecs){
      dc.FillSolidRect(lprc->right-(1+N_SHRINK),lprc->bottom-(1+N_SHRINK),N_SHRINK,N_SHRINK,crShrink);
    }
  }
}
void GridRenderer::draw_cell_inner(CDCHandle& dc,const CMergeCell* /*merge*/,int row,int col,/*CRgnLight const* pRgnClip,*/DrawCellInfo& dci,style::StyleDesc const& style)const
{
  LPCRECT lprc=&dci.rcCell;
  _ASSERT(lprc->right > lprc->left+1 && lprc->bottom > lprc->top+1);
  
  /*
  if(style.IsOwnerDraw()){
    CellFontSetup cfs(dc,style.Font_GetLogFont(),style.Font_GetColor());
    dcs.pDTS->uDrawTextFormat=GetDrawTextFormat((EXlHAlign)dcs.hAlignment);
    pSheet->SendNotifyMessageToListener(row,col,OCN_DRAWCELL,(LPARAM)&dcs);
    return;
  }
  */

  //DrawCellInfo dci;
  //dci.pMerge=merge?merge->get():NULL;
  //dci.hAlign=(EXlHAlign)dcs.hAlign;
  //dci.vAlign=(EXlVAlign)dcs.vAlignment;
  //dci.lprcCell=&dcs.fillRect;
  //dci.nDataLen=dcs.pDTS->nTextLen;
  //dci.pdata=(BYTE*)dcs.pDTS->lpszCellText;

  //const StyleDesc* psd=&style;
  //if(psd->Font_IsNull())
  //  psd=&pSheet_->Style_GetStyleByID(pSheet_->get_RowHeader().Style_GetRowStyleID(row));
  //if(psd->Font_IsNull())
  //  psd=&pSheet_->Style_GetStyleByID(pSheet_->get_ColHeader().Style_GetColStyleID(col));
  //if(psd->Font_IsNull())
  //  psd=&pSheet_->Style_GetTableStyle();
  if(!style.Font_IsNull()){
    dci.pLogFont=(void*)&style.Font_GetLogFont();
    dci.clrFont=style.Font_GetColor();
  }
  //dci.clrBkgnd=dcs.bkclr;
  dci.bWrapText=style.Align_IsWrapText();

  CComPtr<ICellSymbol> pFillSymbol=cellTypeFactory_.GetFillSymbol(dci.cellType);
  //fill rect
  if(pFillSymbol){
    //if(merge)
    //  pFillSymbol_->SetMergeInfo(merge->LeftCol(),merge->TopRow(),merge->RightCol(),merge->BottomRow());
    pFillSymbol->Draw(dc.m_hDC,row,col,&dci);
  }
  //CBrush br(CreateSolidBrush(bkclr));
  //::FillRect(dcs.hDC,&dcs.fillRect,br);
  //:~fill rect
  //lprc=&dcs.textRect;
  if(!(lprc->right - lprc->left<=5 || lprc->bottom-lprc->top<=5))
    //&& RectInRgn(pRgnClip, *lprc))
  {
    CComPtr<ICellSymbol> pSymbol=cellTypeFactory_.GetSymbol(dci.cellType);
    if(!pSymbol){
      _ASSERT(FALSE);
      return;
    }
    //if(merge)
    //  pSymbol->SetMergeInfo(merge->LeftCol(),merge->TopRow(),merge->RightCol(),merge->BottomRow());
    //dci.lprcCell=&dcs.textRect;
    pSymbol->Draw(dc,row,col,&dci);
  }
}
COLORREF SetupHeaderCornerDE(const Worksheet* pSheet,BOOL isSelectAll)
{
  //_ASSERT(FALSE);
  return pSheet->get_ShowSelection()?(isSelectAll?HeaderColors::CLR_ACTIVE:HeaderColors::CLR_BKGND):(HeaderColors::CLR_BKGND);
}

#endif
void GridRenderer_draw(const Worksheet* pSheet,const CellAxisInfo* pcai,HDC hDC,
                        LPCRECT lprcClient,const CRgnLight* pRgnClip)
{
  RECT rcTmp;
  VisibleCellsSession session_;
  const CellAxisInfo cai=*pcai;
  mycellinfo info;
  
#if 0
  //draw header parts
  if(bShowRowHeader && bShowColHeader &&RectInRgn(pRgnClip, cai.get_HeaderAlthonogalRect(rcTmp)))
  {
    const ICellSymbol* pSymbol=cellTypeFactory_.GetSymbol(ECT_HEADERCORNER);
    if(pSymbol){
      GdiSelectObject<> gso(dc,CreatePen(PS_SOLID,0,HeaderColors::CLR_GRID));
      DrawCellInfo dci;
      ZeroMemory(&dci,sizeof(dci));
      BOOL const bSelectAll=pSheet->IsSelectAll();
      dci.clrBkgnd=SetupHeaderCornerDE(pSheet,bSelectAll);
      dci.nSelState=bSelectAll?EST_IN_SELECTION:EST_NOT_IN_SELECTION;
      dci.rcCell=rcTmp;
      pSymbol->Draw(dc,HEADER_ROW,HEADER_COL,&dci);
    }
  }
  if(bShowColHeader && cols>0){
    if(IsFreezeColVisible && RectInRgn(pRgnClip, cai.get_HeaderHFreezeRect(rcTmp)))
      draw_ColHeader(dc,&rcTmp,nFreezeLeftCol,nFreezeRightCol,pRgnClip);
    if(RectInRgn(pRgnClip, cai.get_HeaderHScrollRect(rcTmp))){
      draw_ColHeader(dc,&rcTmp,nLeftVisScrollCol,cols-1,pRgnClip);
    }
  }
  if(bShowRowHeader && rows>0){
    if(IsFreezeRowVisible && RectInRgn(pRgnClip, cai.get_HeaderVFreezeRect(rcTmp)))
      draw_RowHeader(dc,&rcTmp,nFreezeTopRow,nFreezeBottomRow,pRgnClip);
    if(RectInRgn(pRgnClip, cai.get_HeaderVScrollRect(rcTmp)))
      draw_RowHeader(dc,&rcTmp,nTopVisScrollRow,rows-1/*nBottomVisScrollRow*/,pRgnClip);
  }
  
  //draw EGP_FREEZE_ALTHOGONAL 画冻结正交区
  if(IsFreezeRowVisible 
    && IsFreezeColVisible 
    && RectInRgn(pRgnClip, cai.get_FreezeAlthonogalRect(rcTmp))
    )
  {
    calc_cells_session(pRgnClip,nFreezeLeftCol,nFreezeTopRow,nFreezeRightCol,nFreezeBottomRow,&rcTmp,session_);
    //const CCellRange clip(nFreezeTopRow,nFreezeLeftCol,nFreezeBottomRow,nFreezeRightCol);
    draw_session(dc,session_,EGP_SCROLLABLE,activeSelection);
  }
  if(IsFreezeRowVisible && RectInRgn(pRgnClip, cai.get_FreezeHScrollRect(rcTmp))){
    calc_cells_session(pRgnClip,nLeftVisScrollCol
      ,nFreezeTopRow
      ,nRightVisScrollCol
      ,nFreezeBottomRow,&rcTmp,session_);
    //const CCellRange clip(nFreezeTopRow,nLeftVisScrollCol,nFreezeBottomRow,nRightVisScrollCol);
    draw_session(dc,session_,EGP_SCROLLABLE,activeSelection);
  }
  if(IsFreezeColVisible && RectInRgn(pRgnClip, cai.get_FreezeVScrollRect(rcTmp))){
    calc_cells_session(pRgnClip,nFreezeLeftCol
      ,nTopVisScrollRow
      ,nFreezeRightCol
      ,nBottomVisScrollRow,&rcTmp,session_);
    //const CCellRange clip(nTopVisScrollRow,nFreezeLeftCol,nBottomVisScrollRow,nFreezeRightCol);
    draw_session(dc,session_,EGP_SCROLLABLE,activeSelection);
  }
  
  if(rows>0 && cols>0 && RectInRgn(pRgnClip, cai.get_ScrollRect(rcTmp))){
    //calc_scrollCellsSession(rh,ch,&de.rScroll);
    //draw_content(hDC,&de.rScroll,pRgnClip);
    calc_cells_session(pRgnClip,nLeftVisScrollCol, nTopVisScrollRow
      ,nRightVisScrollCol, nBottomVisScrollRow,&rcTmp,session_);
    //const CCellRange clip(nTopVisScrollRow,nLeftVisScrollCol,nBottomVisScrollRow,nRightVisScrollCol);
    draw_session(dc,session_,EGP_SCROLLABLE,activeSelection);
  }
  
  //draw remain parts
  int const bottomRow=nBottomVisScrollRow;
  //if(0==rows || bottomRow==rows-1)
  {
    SetRect(&rcTmp,0,cai.HRemainAxis,cai.VClientAxis,cai.HClientAxis);
    if(RectInRgn(pRgnClip, rcTmp))
      FillRect(hDC,&rcTmp,(HBRUSH) GetStockObject(WHITE_BRUSH));
  }
  //if(0==cols || nRightVisScrollCol==cols-1)
  {
    SetRect(&rcTmp,cai.VRemainAxis,0,cai.VClientAxis,cai.HRemainAxis);
    if(RectInRgn(pRgnClip, rcTmp))
      FillRect(hDC,&rcTmp,(HBRUSH) GetStockObject(WHITE_BRUSH));
  }
  //:~draw border parts
  
  //----------------draw header
  //pSheet->GetSelectionBound().Draw(hDC,lprcClient);
  //SelectObject(hDC,hOldFont);
#endif
}
