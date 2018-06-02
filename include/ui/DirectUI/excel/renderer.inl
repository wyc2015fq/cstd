
#ifndef WIDTHBYTES 
//#define WIDTHBYTES(w,b) ((((w)*(b)+31)/32)*4)
#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)
#endif
BOOL SaveDCToFile( LPCTSTR lpszFileName, HDC saveDC, RECT rc ) {
  BITMAPFILEHEADER bf = { 0 };
  BITMAPINFOHEADER bi = { 0 };
  HDC hMemDC;
  HBITMAP hBitmap;
  LPBYTE lpBits;
  HBITMAP hOldBmp;
  DWORD dwWrite;
  HANDLE fh;
  HDC hdc;
  bi.biSize = sizeof( BITMAPINFOHEADER );
  bi.biBitCount = 24; // 保存为true color
  bi.biWidth = rc.right - rc.left;
  bi.biHeight = rc.bottom - rc.top;
  bi.biPlanes = 1;
  bi.biCompression = BI_RGB;
  bi.biSizeImage = WIDTHBYTES( bi.biWidth * bi.biBitCount ) * bi.biHeight;
  hdc = GetDC( NULL );
  if ( !hdc )
    return FALSE;
  hBitmap = CreateDIBSection( hdc, ( CONST BITMAPINFO * ) & bi, DIB_RGB_COLORS,
    ( VOID ** ) & lpBits, NULL, 0 );
  if ( !hBitmap ) {
    ReleaseDC( NULL, hdc );
    return FALSE;
  }
  hMemDC = CreateCompatibleDC( hdc );
  hOldBmp = ( HBITMAP ) SelectObject( hMemDC, hBitmap );
  BitBlt( hMemDC, 0, 0, bi.biWidth, bi.biHeight, saveDC, rc.left, rc.top, SRCCOPY );
  SelectObject( hMemDC, hOldBmp );
  ReleaseDC( NULL, hdc );
  DeleteDC( hMemDC );
  bf.bfType = 0x4d42;
  bf.bfSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER )
    + bi.biSizeImage;
  bf.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );
  fh = CreateFileA( ( LPCSTR ) lpszFileName,
    GENERIC_READ | GENERIC_WRITE,
    ( DWORD ) 0,
    NULL,
    CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    ( HANDLE ) NULL );
#if 0
  if ( fh == INVALID_HANDLE_VALUE ) {
    CHAR szBuf[ 80 ];
    DWORD dw = GetLastError();
    sprintf( szBuf, "failed: GetLastError returned %u\n", dw );
    MessageBoxA( NULL, szBuf, "Error", MB_OK );
  }
#endif
  WriteFile( fh, ( LPBYTE ) & bf, sizeof( BITMAPFILEHEADER ), &dwWrite, NULL );
  WriteFile( fh, ( LPBYTE ) & bi, sizeof( BITMAPINFOHEADER ), &dwWrite, NULL );
  WriteFile( fh, ( LPBYTE ) lpBits, bi.biSizeImage, &dwWrite, NULL );
  DeleteObject( hBitmap );
  CloseHandle( fh );
  return TRUE;
}

#define HeaderSetupDC_begin(hDC)  \
  { HDC hDC_=hDC; \
    int nBKMode_ = SetBkMode(hDC,TRANSPARENT); \
    HFONT hFont_ = (HFONT)SelectObject(hDC,GetStockObject(DEFAULT_GUI_FONT));

#define HeaderSetupDC_end()  \
    if(nBKMode_) SetBkMode(hDC_,nBKMode_);  \
    if(hFont_ && HGDI_ERROR!=hFont_) SelectObject(hDC_,GetStockObject(DEFAULT_GUI_FONT));  }

ESelectionState SetupRowHeaderDE(const Worksheet* pSheet,int row,COLORREF* backColor)
{
  if(get_ShowSelection(pSheet)) {
    BOOL const bFullInselection=IsFullColSelectedAtRow(pSheet, row) ;//>CellInSelections(row,HEADER_COL);
    const bool bInSelection=bFullInselection?true:RowInSelections(pSheet, row);
    if(bFullInselection){
      *backColor=HCLR_ACTIVE;
      return EST_IN_SELECTION;
    }else if(bInSelection||row==pSheet->activeCell_.row){
      *backColor=HCLR_SELECTION;//RGB(0,0,0xff):RGB(255,192,111);
      return EST_IN_PROJECTION_OF_SELECTION;
    }else
      *backColor=HCLR_BKGND;//RGB(200,200,200);
  }else
    *backColor=HCLR_BKGND;//RGB(200,200,200);
  return EST_NOT_IN_SELECTION;
}
#include "DefaultSymbols.inl"
//[firstRow,lastRow]
void draw_RowHeader(const Worksheet* pSheet, HDC hDC,LPCRECT lprc,int firstRow,int lastRow,CRgnLight* pRgnClip)
{
  int i;
  RECT rcCell;
  DrawCellInfo dci;
  int topRow=firstRow;
  //Cell activeCell=pSheet->activeCell_;
  BOOL bAutoAppendRow=get_AutoAppendRow(pSheet);
  //const Selections* pSelections=pSheet->get_Selections();
  const RowHeader* rh=pSheet->rh_;
  if(0==pSheet->nrh_) return ;
  
  _ASSERT(topRow<pSheet->nrh_);
  HeaderSetupDC_begin(hDC);
  //GdiSelectObject<> gso(dc.m_hDC,CreatePen(PS_SOLID,0,HCLR_GRID));
  SetRect(&rcCell, lprc->left,lprc->top,lprc->right,lprc->top);
  
  //const CComPtr<ICellSymbol> pSymbol=cellTypeFactory_.GetSymbol(ECT_ROWHEADER);
  ZeroMemory(&dci,sizeof(dci));
  for( i=topRow;i<=lastRow && rcCell.bottom<=lprc->bottom;++i,rcCell.top=rcCell.bottom)
  {
    int hi=get_RowHeight(pSheet,i);
    rcCell.bottom=rcCell.top+hi;
    
    if(RectInRgn(pRgnClip, rcCell)) {
      int nCount=get_RowHeaderText(pSheet, i, dci.text, MAX_CELLTEXT);
      if(bAutoAppendRow && i==pSheet->nrh_-1){
        lstrcpy(dci.text,_T("*"));
        nCount=1;
      }
      dci.cellType=ECT_ROWHEADER;
      dci.rcCell=rcCell;
      dci.nData=0;
      dci.nSelState=SetupRowHeaderDE(pSheet,i,&dci.clrBkgnd);
      Symbol_Draw(hDC, &dci);
    }
  }
  HeaderSetupDC_end();
  return ;
}

ESelectionState SetupColHeaderDE(const Worksheet* pSheet,int col,COLORREF* backColor)
{
  if(get_ShowSelection(pSheet)){
    //const Selections* pCurSelections=pSheet->get_Selections();
    BOOL const bFullInselection=IsFullRowSelectedAtCol(pSheet, col);//CellInSelections(HEADER_ROW,col);
    const bool bInSelection=bFullInselection?true:ColInSelections(pSheet, col);
    if(bFullInselection){
      *backColor=HCLR_ACTIVE;
      return EST_IN_SELECTION;
    }else if(bInSelection||col==pSheet->activeCell_.col){
      *backColor=HCLR_SELECTION;//RGB(0,0,0xff):RGB(255,192,111);
      return EST_IN_PROJECTION_OF_SELECTION;
    }else
      *backColor=HCLR_BKGND;//RGB(200,200,200);
  }else
    *backColor=HCLR_BKGND;//RGB(200,200,200);
  return EST_NOT_IN_SELECTION;
}

//[leftCol,rightCol]
void draw_ColHeader(const Worksheet* pSheet,HDC hDC,LPCRECT lprc,int leftCol,int rightCol,CRgnLight* pRgnClip)
{
  int i;
  DrawCellInfo dci;
  RECT rcCell={lprc->left,lprc->top,lprc->left,lprc->bottom};
  HeaderSetupDC_begin(hDC);
  ZeroMemory(&dci,sizeof(dci));
  for(i=leftCol;i<=rightCol && rcCell.right<=lprc->right;++i,rcCell.left=rcCell.right)
  {
    int const wi=get_ColWidth(pSheet, i);
    rcCell.right=rcCell.left+wi;
    //if(rect_intersect(*pRgnClip,&rcCell)){
    if(RectInRgn(pRgnClip, rcCell)){
      int nCount=get_CowHeaderText(pSheet, i,dci.text, MAX_CELLTEXT);
      dci.cellType=ECT_COLHEADER;
      dci.rcCell=rcCell;
      dci.nSelState=SetupColHeaderDE(pSheet,i,&dci.clrBkgnd);
      Symbol_Draw(hDC,&dci);
    }
  }
  HeaderSetupDC_end();
}

StyleDesc* Style_GetStyleByID(StyleDesc* Styles, int nStyles, StrID_t nStyleID) {
  int i;
  for (i=0; i<nStyles; ++i) {
    if (nStyleID==Styles[i].nStyleID) {
      return Styles+i;
    }
  }
  return NULL;
}

COLORREF GetBKColor(const Worksheet* pSheet, int idx, COLORREF bkColor)
{
  if(get_ShowSelection(pSheet)) {
    CellStore* Cells = pSheet->Cells+idx;
    int activeidx = pSheet->activeCell_.row*pSheet->nColCount+pSheet->activeCell_.col;
    
    if(activeidx==idx){
      bkColor^=CLR_ACTIVE;
    } else if (CellInSelections(*Cells)){
      bkColor^=CLR_SELECTION;
    }
  }
  return bkColor;
}

void draw_cell_inner(HDC hDC, DrawCellInfo* dci, StyleDesc* style)
{
  LPCRECT lprc=&dci->rcCell;
  _ASSERT(lprc->right > lprc->left+1 && lprc->bottom > lprc->top+1);
  
  if(!Font_IsNull(*style)){
    dci->pLogFont=(void*)&Font_GetLogFont(*style);
    dci->clrFont=Font_GetColor(*style);
  }
  dci->bWrapText=Align_IsWrapText(*style);
  
  //if(pFillSymbol){  pFillSymbol->Draw(dc.m_hDC,row,col,&dci); }
  if(!(lprc->right - lprc->left<=5 || lprc->bottom-lprc->top<=5)) {
    Symbol_Draw(hDC, dci);
  }
}

void draw_cell_shrink(const Worksheet* pSheet,HDC hDC,DrawCellInfo* dcs)//,CMergeCell* pmerge)
{
  LPCRECT lprc=&dcs->rcCell;
  BOOL bMultiSelectionMode = 1;
  BYTE bActiveCell=dcs->nSelState&EST_ACTIVECELL;
  int N_SHRINK=bMultiSelectionMode?1:(IsSelectAll(pSheet)?1:2);
  int eShrink=0;
  int ecs=eShrink;
#if 0
  if(!(lprc->right > lprc->left+1 && lprc->bottom > lprc->top+1))
    return;
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
          MoveTo(hDC,left,top);
          //LineTo(hDC,left,bottom-(row==pmerge->BottomRow()?0:-1));
          LineTo(hDC,left,row==pmerge->BottomRow()?bottom:(bottom+1));
        }
        if(row==pmerge->TopRow())
        {
          MoveTo(hDC,left,top);
          //LineTo(hDC,right-(col==pmerge->RightCol()?1:0),top);
          LineTo(hDC,col==pmerge->RightCol()?right:right+1,top);
        }
        if(col==pmerge->RightCol())
        {
          MoveTo(hDC,right-1,top);
          //LineTo(hDC,right-1,bottom-(row==pmerge->BottomRow()?1:0));
          LineTo(hDC,right-1,row==pmerge->BottomRow()?bottom:(bottom+1));
        }
        if(row==pmerge->BottomRow())
        {
          MoveTo(hDC,left,bottom-1);
          //LineTo(hDC,right-(col==pmerge->RightCol()?1:0),bottom-1);
          LineTo(hDC,col==pmerge->RightCol()?right:right+1,bottom-1);
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
#endif
}

void draw_cell_border_right(HDC hDC,LPCRECT lprc, CellBorder cellBorder)
{
  Border bdr=cellBorder.right;
  //Border bdr=GetRightBorder(pSheet,cell);
  if(!Border_IsNull(bdr)){
    int wi=MIN(2,Border_get_Weight(bdr));
    HPEN hPenOld=SelectPen(hDC, MyCell_CreatePen(hDC,bdr,wi));
    if(hPenOld){
      MoveTo(hDC, lprc->right-1,lprc->top+wi-1);
      LineTo(hDC, lprc->right-1,lprc->bottom);
      DeleteObject(SelectPen(hDC, hPenOld));
    }
  }
}
void draw_cell_border_left(HDC hDC,LPCRECT lprc,CellBorder cellBorder)
{
  Border bdr=cellBorder.left;
  if(!Border_IsNull(bdr)){
    const int wi=Border_get_Weight(bdr);
    if(wi>2){
      HPEN hPenOld=SelectPen(hDC,MyCell_CreatePen(hDC,bdr,1));
      if(hPenOld){
        MoveTo(hDC,lprc->left,lprc->top);
        LineTo(hDC,lprc->left,lprc->bottom);
        DeleteObject(SelectPen(hDC,hPenOld));
      }
    }
  }
}
void draw_cell_border_bottom(HDC hDC,LPCRECT lprc,CellBorder cellBorder)
{
  Border bdr=cellBorder.bottom;
  //Border  bdr=GetBottomBorder(pSheet,cell);
  if(!Border_IsNull(bdr)){
    switch(Border_get_LineStyle(bdr))
    {
    case xlDouble:
      {
      }break;
    case xlSlantDashDot:
      break;
    default:
      {
        int wi=MIN(2,Border_get_Weight(bdr));
        HPEN hPenOld=SelectPen(hDC,MyCell_CreatePen(hDC,bdr,wi));
        if(hPenOld){
          MoveTo(hDC,lprc->left+wi-1,lprc->bottom-1);
          LineTo(hDC,lprc->right,lprc->bottom-1);
          DeleteObject(SelectPen(hDC,hPenOld));
        }
      }
    }
  }
}
void draw_cell_border_top(HDC hDC,LPCRECT lprc,CellBorder cellBorder)
{
  Border bdr=cellBorder.top;
  //Border bdr=GetTopBorder(pSheet,cell);
  if(!Border_IsNull(bdr)){
    const int wi=Border_get_Weight(bdr);
    if(3==wi){
      HPEN hPenOld=SelectPen(hDC,MyCell_CreatePen(hDC,bdr,1));
      if(hPenOld){
        MoveTo(hDC,lprc->left,lprc->top);
        LineTo(hDC,lprc->right,lprc->top);
        DeleteObject(SelectPen(hDC,hPenOld));
      }
    }
  }
}

typedef struct PenFactory {
  HPEN whitePen1;
  HPEN squareWhitePen2;
  HPEN squareBlackPen1;
  HPEN squareBlackPen2;
  HPEN squareBlackPen3;
	HPEN gridLinePen;
}PenFactory;
void PenFactory_begin(PenFactory* pf) {
  LOGBRUSH lb;
  lb.lbColor=RGB(0,0,0);
  lb.lbStyle=BS_SOLID;
  lb.lbHatch=0;
  pf->squareBlackPen2=(ExtCreatePen(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE,2,&lb,0,NULL));
  pf->squareBlackPen3=(ExtCreatePen(PS_GEOMETRIC|PS_SOLID|PS_ENDCAP_SQUARE,3,&lb,0,NULL));
  lb.lbColor=RGB(255,255,255);
  pf->squareWhitePen2=(ExtCreatePen(PS_GEOMETRIC|PS_INSIDEFRAME|PS_ENDCAP_SQUARE,2,&lb,0,NULL));
  
  pf->gridLinePen=CreatePen(PS_SOLID,0,CLR_GRID);
  pf->whitePen1=CreatePen(PS_SOLID,1,RGB(255,255,255));
  pf->squareBlackPen1=CreatePen(PS_SOLID,1,RGB(0,0,0));
}
void PenFactory_end(PenFactory* pf) {
  DeleteObject(pf->whitePen1);
  DeleteObject(pf->squareWhitePen2);
  DeleteObject(pf->squareBlackPen1);
  DeleteObject(pf->squareBlackPen2);
  DeleteObject(pf->squareBlackPen3);
  DeleteObject(pf->gridLinePen);
}

ECellSelectionRelations GetCellSelectionRelations(const Worksheet* pSheet,const CellRange cr)
{
  int ecsr=0;
#if 0
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
#endif
  return (ECellSelectionRelations)ecsr;
}
void draw_cell_DragBounds(const Worksheet* pSheet,HDC hDC,const CellRange activeSelection,RECT rcCell,COLORREF bkclr)
{
  PenFactory pf;
  //_ASSERT(!(activeSelection.LeftCol()>=cols || activeSelection.TopRow()>=rows|| activeSelection.RightCol()<0 || activeSelection.BottomRow()<0));
  LPCRECT lprc=&rcCell;
  //HPEN hOldPen = SelectPen(hDC, pf.squareBlackPen1);
  int ecsr=(int)GetCellSelectionRelations(pSheet,activeSelection);
  //const GDISetROP2 rop2(dc.m_hDC,R2_XORPEN);
  
  PenFactory_begin(&pf);
  //左上外角
  if(ECSR_OUTER_LEFTTOP==ecsr){
    HPEN hOldPen = SelectPen(hDC, pf.squareWhitePen2);
    MoveTo(hDC,lprc->right-1,lprc->bottom-1);
    LineTo(hDC,lprc->right,lprc->bottom-1);
    SelectPen(hDC, hOldPen);
    return;
    
  }
  //
  if(ECSR_OUTER_RIGHTTOP==ecsr){
    HPEN hOldPen = SelectPen(hDC, pf.whitePen1);
    MoveTo(hDC,lprc->left,lprc->bottom-2);
    LineTo(hDC,lprc->left+1,lprc->bottom);
    SelectPen(hDC, hOldPen);
    return;
  }
  if(ECSR_OUTER_LEFTBOTTOM==ecsr){
    HPEN hOldPen = SelectPen(hDC, pf.whitePen1);
    MoveTo(hDC,lprc->right-2,lprc->top);
    LineTo(hDC,lprc->right,lprc->top);
    SelectPen(hDC, hOldPen);
    return;
  }
  if(ECSR_OUTER_RIGHTBOTTOM==ecsr){
    HPEN hOldPen = SelectPen(hDC, pf.whitePen1);
    MoveTo(hDC,lprc->left,lprc->top);
    LineTo(hDC,lprc->left+2,lprc->top);
    MoveTo(hDC,lprc->left,lprc->top+1);
    LineTo(hDC,lprc->left+2,lprc->top+1);
    SelectPen(hDC, hOldPen);
    return;
  }
  
  if(ECSR_INNER_LEFT&ecsr){
    
    HPEN hOldPen = SelectPen(hDC, pf.whitePen1);
    //MoveTo(hDC,lprc->left,row==activeSelection.TopRow()?lprc->top+1:lprc->top);
    MoveTo(hDC,lprc->left,(ECSR_INNER_TOP&ecsr)?lprc->top+1:lprc->top);
    LineTo(hDC,lprc->left,lprc->bottom);
    SelectPen(hDC, hOldPen);
  }
  if(ECSR_OUTER_LEFT&ecsr){
    HPEN hOldPen = SelectPen(hDC, pf.squareWhitePen2);
    MoveTo(hDC,lprc->right-1,lprc->top+1);
    LineTo(hDC,lprc->right-1,lprc->bottom-1);
    SelectPen(hDC, hOldPen);
  }
  if(ECSR_INNER_RIGHT&ecsr){
    int bottom=((ECSR_INNER_BOTTOM&ecsr))?(lprc->bottom-5):lprc->bottom;
    HPEN hOldPen = SelectPen(hDC, pf.squareWhitePen2);
    //MoveTo(hDC,lprc->right-1,row==activeSelection.TopRow()?lprc->top+2:lprc->top+1);
    MoveTo(hDC,lprc->right-1,(ECSR_INNER_TOP&ecsr)?lprc->top+2:lprc->top+1);
    LineTo(hDC,lprc->right-1,bottom);
    SelectPen(hDC, hOldPen);
  }
  if(ECSR_OUTER_RIGHT&ecsr){
    int bottom=lprc->bottom;
    HPEN hOldPen = SelectPen(hDC, pf.whitePen1);
    MoveTo(hDC,lprc->left,lprc->top);//row==activeSelection.TopRow()?lprc->top+1:lprc->top);
    LineTo(hDC,lprc->left,bottom);
    {
      //sop.SelectObject(PenFactory::Instance().squareWhitePen2);
      MoveTo(hDC,lprc->left+1,bottom+1);
      LineTo(hDC,lprc->left+1,bottom+4);
      MoveTo(hDC,lprc->left,bottom+1);
      LineTo(hDC,lprc->left,bottom+4);
    }
    SelectPen(hDC, hOldPen);
  }
  if(ECSR_INNER_TOP&ecsr){
    HPEN hOldPen = SelectPen(hDC, pf.whitePen1);
    MoveTo(hDC,lprc->left,lprc->top);
    LineTo(hDC,lprc->right,lprc->top);
    SelectPen(hDC, hOldPen);
  }
  if(ECSR_OUTER_TOP&ecsr){
    HPEN hOldPen = SelectPen(hDC, pf.squareWhitePen2);
    MoveTo(hDC,lprc->left+1,lprc->bottom-1);
    LineTo(hDC,lprc->right-1,lprc->bottom-1);
    SelectPen(hDC, hOldPen);
  }
  if(ECSR_INNER_BOTTOM&ecsr){
    int right=lprc->right-1;
    HPEN hOldPen = SelectPen(hDC, pf.squareWhitePen2);
    //MoveTo(hDC,lprc->left+(col==activeSelection.LeftCol()?2:1),lprc->bottom-1);
    MoveTo(hDC,lprc->left+((ECSR_INNER_LEFT&ecsr)?2:1),lprc->bottom-1);
    LineTo(hDC,right,lprc->bottom-1);
    if(ECSR_INNER_RIGHT&ecsr){
      MoveTo(hDC,right+3,lprc->bottom-2);
      LineTo(hDC,lprc->right,lprc->bottom-2);

      SelectPen(hDC, pf.whitePen1);
      MoveTo(hDC,right+2,lprc->bottom-1);
      LineTo(hDC,lprc->right,lprc->bottom-1);
      SelectPen(hDC, pf.squareWhitePen2);
    }
    SelectPen(hDC, hOldPen);
  }
  if(ECSR_OUTER_BOTTOM&ecsr){
    int right=lprc->right;
    HPEN hOldPen = SelectPen(hDC, pf.whitePen1);
    MoveTo(hDC,lprc->left,lprc->top);
    LineTo(hDC,right,lprc->top);
    {
      MoveTo(hDC,right+1,lprc->top);
      LineTo(hDC,lprc->right,lprc->top);
      MoveTo(hDC,right+1,lprc->top+1);
      LineTo(hDC,lprc->right,lprc->top+1);
    }
    SelectPen(hDC, hOldPen);
  }
  PenFactory_end(&pf);
}

void draw_cell_border(HDC hDC,LPCRECT lprc,CellBorder cellBorder)
{
  draw_cell_border_left(hDC,lprc,cellBorder);
  draw_cell_border_right(hDC,lprc,cellBorder);
  draw_cell_border_top(hDC,lprc,cellBorder);
  draw_cell_border_bottom(hDC,lprc,cellBorder);
}

void draw_cell(HDC hDC, const Worksheet* pSheet, RECT rcCell, int idx, ESelectionMode esm, StyleDesc* Styles, int nStyles, BOOL bActive)
{
  CellStore* Cells = pSheet->Cells+idx;
  StyleDesc* style=Style_GetStyleByID(Styles, nStyles, Cells->nStyleID);
  BOOL bShowGrid=get_ShowGrid(pSheet);
  BOOL bShowSelection=get_ShowSelection(pSheet);
  CellBorder cellBorder=style->borders;
  
  /*DrawCellStruct*/
  DrawCellInfo dcs;
  COLORREF cellBKClr;
  ZeroMemory(&dcs,sizeof(dcs));

  dcs.rcCell=rcCell;
  dcs.nSelState=bActive?(EST_ACTIVECELL):(CellInSelections(*Cells)?EST_IN_SELECTION:EST_NOT_IN_SELECTION);
  
  cellBKClr = BKGND_HasColor(*style)?BKGND_GetColor(*style):CLR_BKGND;
  dcs.clrBkgnd=GetBKColor(pSheet,idx,cellBKClr);

  if(dcs.rcCell.right > dcs.rcCell.left+1 && dcs.rcCell.bottom > dcs.rcCell.top+1){
    HRGN rgn;
    GetCellText(Cells, dcs.text, MAX_CELLTEXT);
    
    dcs.cellType=(ECellType)(Cells->cellType);//_pStyle->CellType_GetType();
    dcs.hAlign=Align_GetHorizontal(*style);
    dcs.vAlign=Align_GetVertical(*style);
    
    //if(merge)
    
    rgn =CreateRectRgn(rcCell.left,rcCell.top,rcCell.right,rcCell.bottom);
    SelectClipRgn(hDC, rgn);
    
    draw_cell_inner(hDC,&dcs,style);
    if(bShowSelection)
      draw_cell_shrink(pSheet,hDC,&dcs);
    //if(merge)
    SelectClipRgn(hDC, NULL);
    DeleteObject(rgn);
  }
  //CellBorder const& cellBorder=*style.GetBorders();
  //cellBorder.top=session.
  //画网格线
  if(bShowGrid){
    RECT _rc=rcCell;
    if(Border_IsNull(cellBorder.bottom)||xlLineStyleNone==Border_get_LineStyle(cellBorder.bottom)){
      MoveTo(hDC, _rc.left,_rc.bottom-1);
      LineTo(hDC, _rc.right+1,_rc.bottom-1);
    }
    if(Border_IsNull(cellBorder.right)||xlLineStyleNone==Border_get_LineStyle(cellBorder.right)){
      bool bDrawRight=1;
      if(bDrawRight) {
        MoveTo(hDC, _rc.right-1,_rc.top);
        LineTo(hDC, _rc.right-1,_rc.bottom+1);
      }
    }
  }
  draw_cell_border(hDC,&rcCell,cellBorder);
  
  if(bShowSelection) {
    if(ESM_SINGLECELL==esm){
      {
        const RECT rc={rcCell.left,rcCell.top,rcCell.right-1,rcCell.bottom-1};
        DrawFocusRect(hDC, &rc);
      }
    }else{
      //if(!(CellRange_LeftCol(activeSelection)>=pSheet->nColCount || CellRange_TopRow(activeSelection)>=pSheet->nrh_
      //  || CellRange_RightCol(activeSelection)<0 || CellRange_BottomRow(activeSelection)<0))
      {
        CellRange activeSelection={0};
        draw_cell_DragBounds(pSheet,hDC,activeSelection,rcCell,cellBKClr);
      }
    }
  }
  //画冻结线
  //HPEN hOldPen = SelectPen(hDC, pf.squareBlackPen1);
  //if(ch.get_FreezeRightCol()==cell.col)
  {
    MoveTo(hDC,rcCell.right-1,rcCell.top);
    LineTo(hDC,rcCell.right-1,rcCell.bottom);
  }
  //if(rh.get_FreezeBottomRow()==cell.row)
  {
    MoveTo(hDC,rcCell.left,rcCell.bottom-1);
    LineTo(hDC,rcCell.right,rcCell.bottom-1);
  }
}

#if 0
void calc_cells_session(CRgnLight* pRgnClip,int firstCol,int firstRow,int lastCol,int lastRow,LPCRECT lprcContent,VisibleCellsSession& session)
{
  const RowHeader& rh=pSheet->get_RowHeader();
  const ColHeader& ch=pSheet->get_ColHeader();
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
  const VisibleMergeCellMgr& visMergeCells=pSheet->GetVisibleMergeCellMgr();
  for(int row=topRow;row<=bottomRow;++row,rcCell.top=rcCell.bottom){
    rcCell.bottom=rcCell.top+get_RowHeight(pSheet,row);
    
    //if(rcCell.bottom==rcCell.top)
    //  continue;
    
    rcCell.left=lprcContent->left;
    for(int col=leftCol;col<=rightCol;++col,rcCell.left=rcCell.right)
    {
      const Cell cell(row,col);
      CellItem&   item=session.item(cell);
      item.clear();
      rcCell.right=rcCell.left+ch.get_ColWidth(col);
      
      //item.pMerge=pMerge;
      item.rcCell=rcCell;
      if(pRgnClip->RectInRgn(rcCell)){
        const CMergeCell* pMerge=visMergeCells.GetMergeCells(cell);
        if(pMerge){
          int const minR=pMerge->TopRow();
          int const minC=pMerge->LeftCol();
          int const maxR=pMerge->BottomRow();
          int const maxC=pMerge->RightCol();
          RECT rcMerge=rcCell;//pSheet->raw_GetCellRect(cell);
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
        item.SetCSPack(pSheet->GetCellStore(cell,ECSM_STYLE_VALID|ECSM_CELLTYPE_VALID,pMerge));
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
    bool operator()(T const& l,T const& r)
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
    
  }
  return nFormat;
}
//用来代替方法draw_content
//void draw_session(const Worksheet* pSheet,const Selections* pCurSelections,HDC hDC,const RowHeader& rh,const ColHeader& ch,LPCRECT /*lprcRange*/,CRgnLight const* pRgnClip,EGridPart ePart,const CellRange& activeSelection/*,const CellRange& clipRange*/)
void draw_session(HDC hDC,EGridPart ePart,const CellRange& activeSelection)
{
  const Selections* pSelections=pSheet->get_Selections();
  const RowHeader& rh=pSheet->get_RowHeader();
  const ColHeader& ch=pSheet->get_ColHeader();
  HeaderSetupDC setupDC(dc);
  GdiSelectObjectPlain gso(hDC,PenFactory::Instance().gridLinePen);
  
  const int nBottomRow=session.BottomRow();
  const int nLeftCol=session.LeftCol();
  const int nRightCol=session.RightCol();
  
  for(int row=session.TopRow();row<=nBottomRow;++row)
  {
    for(int col=nLeftCol;col<=nRightCol;++col)
    {
      const Cell cell(row,col);
      CellItem& item=session.item(cell);
      if(item.IsInClip()){
        draw_cell(hDC,cell,item,activeSelection,pSelections,session);
      }
    }
  }
}

Border VisibleCellsSession::GetLeftBorder(const Worksheet* pSheet,Cell cell)
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
    //const Cell _cell(cell.row,it.pMergeCells?it.pMergeCells->LeftCol()-1:cell.col-1);
    const CellStorePack& csp=it.GetCSPack();
    const CMergeCell* pMerge=csp.GetMerge();//it.pMerge;
    if(pMerge)
      cell.col=pMerge->LeftCol();
    const Cell _cell(cell.row,pSheet->GetPreCol(cell.col));
    if(_cell.col==cell.col){
      bdr=pSheet->Style_GetStyleByID(it.GetCSPack().GetStyleID()).GetBorders()->left;
    }else{
      CellItem& itl=item(Cell(_cell.row,max(_cell.col,LeftCol()-1)));
      if(!itl.IsCSPackValid()){
        _ASSERT(!itl.IsInClip());
        itl.SetCSPack(pSheet->GetCellStore(_cell,ECSM_STYLE_VALID));
      }
      StylePack sp=it.GetCSPack().GetStylePack();
      StylePack spLeft=itl.GetCSPack().GetStylePack();
      if(sp.eStorePos>spLeft.eStorePos){
        bdr=pSheet->Style_GetStyleByID(spLeft.nStyleID).GetBorders()->right;
        if(Border_IsNull(bdr))
          bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->left;
      }else{
        bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->left;
        if(Border_IsNull(bdr))
          bdr=pSheet->Style_GetStyleByID(spLeft.nStyleID).GetBorders()->right;
      }
      itl.SetRightBorder(bdr);
    }
    it.SetLeftBorder(bdr);
    return bdr;
  }
}
Border VisibleCellsSession::GetRightBorder(const Worksheet* pSheet,Cell cell)
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
    //const Cell _cell(cell.row,it.pMergeCells?it.pMergeCells->RightCol()+1:cell.col+1);
    const CellStorePack& csp=it.GetCSPack();
    const CMergeCell* pMerge=csp.GetMerge();//it.pMerge;
    if(pMerge)
      cell.col=pMerge->RightCol();
    const Cell _cell(cell.row,pSheet->GetNextCol(cell.col));
    if(_cell.col==cell.col/*pSheet->get_ColHeader().get_cols()*/){
      bdr=pSheet->Style_GetStyleByID(it.GetCSPack().GetStyleID()).GetBorders()->right;
    }else{
      CellItem& itr=item(Cell(_cell.row,MIN(_cell.col,RightCol()+1)));
      if(!itr.IsCSPackValid()){
        _ASSERT(!itr.IsInClip());
        itr.SetCSPack(pSheet->GetCellStore(_cell,ECSM_STYLE_VALID));
      }
      StylePack sp=it.GetCSPack().GetStylePack();
      StylePack spRight=itr.GetCSPack().GetStylePack();
      if(sp.eStorePos>spRight.eStorePos){
        bdr=pSheet->Style_GetStyleByID(spRight.nStyleID).GetBorders()->left;
        if(Border_IsNull(bdr)){
          bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->right;
        }
      }else{
        bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->right;
        if(Border_IsNull(bdr))
          bdr=pSheet->Style_GetStyleByID(spRight.nStyleID).GetBorders()->left;
      }
      itr.SetLeftBorder(bdr);
    }
    it.SetRightBorder(bdr);
    return bdr;
  }
}
Border VisibleCellsSession::GetTopBorder(const Worksheet* pSheet,Cell cell)
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
    //const Cell _cell(it.pMergeCells?it.pMergeCells->TopRow()-1:cell.row-1,cell.col);
    const CellStorePack& csp=it.GetCSPack();
    const CMergeCell* pMerge=csp.GetMerge();//it.pMerge;
    if(pMerge)
      cell.row=pMerge->TopRow();
    const Cell _cell(pSheet->GetPreRow(cell.row),cell.col);
    if(_cell.row==cell.row){
      bdr=pSheet->Style_GetStyleByID(it.GetCSPack().GetStyleID()).GetBorders()->top;
    }else{
      CellItem& itl=item(Cell(max(_cell.row,TopRow()-1),_cell.col));
      if(!itl.IsCSPackValid()){
        _ASSERT(!itl.IsInClip());
        itl.SetCSPack(pSheet->GetCellStore(_cell,ECSM_STYLE_VALID));
      }
      StylePack sp=it.GetCSPack().GetStylePack();
      StylePack spTop=itl.GetCSPack().GetStylePack();
      if(sp.eStorePos>spTop.eStorePos){
        bdr=pSheet->Style_GetStyleByID(spTop.nStyleID).GetBorders()->bottom;
        if(Border_IsNull(bdr))
          bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->top;
      }else{
        bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->top;
        if(Border_IsNull(bdr))
          bdr=pSheet->Style_GetStyleByID(spTop.nStyleID).GetBorders()->bottom;
      }
      itl.SetBottomBorder(bdr);
    }
    it.SetTopBorder(bdr);
    return bdr;
  }
}
Border VisibleCellsSession::GetBottomBorder(const Worksheet* pSheet,Cell cell)
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
    const Cell _cell(pSheet->GetNextRow(cell.row),cell.col);
    if(_cell.row==cell.row/*pSheet->get_RowHeader().get_rows()*/){
      bdr=pSheet->Style_GetStyleByID(it.GetCSPack().GetStyleID()).GetBorders()->bottom;
    }else{
      CellItem& itr=item(Cell(MIN(_cell.row,BottomRow()+1),_cell.col));
      if(!itr.IsCSPackValid()){
        _ASSERT(!itr.IsInClip());
        itr.SetCSPack(pSheet->GetCellStore(_cell,ECSM_STYLE_VALID));
      }
      StylePack sp=it.GetCSPack().GetStylePack();
      StylePack spBottom=itr.GetCSPack().GetStylePack();
      if(sp.eStorePos>spBottom.eStorePos){
        bdr=pSheet->Style_GetStyleByID(spBottom.nStyleID).GetBorders()->top;
        if(Border_IsNull(bdr)){
          bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->bottom;
        }
      }else{
        bdr=pSheet->Style_GetStyleByID(sp.nStyleID).GetBorders()->bottom;
        if(Border_IsNull(bdr))
          bdr=pSheet->Style_GetStyleByID(spBottom.nStyleID).GetBorders()->top;
      }
      itr.SetTopBorder(bdr);
    }
    it.SetBottomBorder(bdr);
    return bdr;
  }
}

COLORREF SetupHeaderCornerDE(const Worksheet* pSheet,BOOL isSelectAll)
{
  //_ASSERT(FALSE);
  return get_ShowSelection(pSheet)?(isSelectAll?HCLR_ACTIVE:HCLR_BKGND):(HCLR_BKGND);
}

void draw(const Worksheet* pSheet,const CellAxisInfo* pcai,HDC hDC,LPCRECT lprcClient,const CRgnLight* pRgnClip)
{
  pSheet=pSheet;
  HDC hDC(hDC);
  
  //const Selections* pSelections=pSheet->get_Selections();
  VisibleCellsSession session_;
  //SetMapMode(hDC,MM_TWIPS);
  //CInvalidateRectSession irs(pSheet->m_hWnd,lprcClient);
  const CellAxisInfo& cai=*pcai;//pSheet->get_CellAxisInfo(/*lprcClient*/);
  const RowHeader& rh=pSheet->get_RowHeader();
  const ColHeader& ch=pSheet->get_ColHeader();
  int const rows=pSheet->nrh_;
  int const cols=pSheet->nColCount;
  int const nFreezeBottomRow=rh.get_FreezeBottomRow();
  int const nFreezeTopRow=rh.get_FreezeTopRow();
  int const nFreezeRightCol=ch.get_FreezeRightCol();
  int const nFreezeLeftCol=ch.get_FreezeLeftCol();
  int const nTopVisScrollRow=rh.get_TopVisScrollRow();
  int const nBottomVisScrollRow=rh.get_BottomVisScrollRow();
  int const nLeftVisScrollCol=ch.get_LeftVisScrollCol();
  int const nRightVisScrollCol=ch.get_RightVisScrollCol();
  BOOL const bShowRowHeader=pSheet->get_ShowRowHeader();
  BOOL const bShowColHeader=pSheet->get_ShowColHeader();
  BOOL const IsFreezeRowVisible=rh.IsFreezeRowVisible();
  BOOL const IsFreezeColVisible=ch.IsFreezeColVisible();
  
  CFont hFont=pSheet->Style_GetTableStyle().Font_GetHFont(hDC);
  
  //HGDIOBJ hOldFont=SelectObject(hDC,hFont);
  GdiSelectObjectPlain selFont(hDC,hFont);
  
  //about draw active selection bound
  CellRange activeSelection=pSheet->get_Selections()->GetActiveSelection();
  _ASSERT(activeSelection.IsNormalize());
  //activeSelection.Normalize();
  pSheet->AdjustSelection(activeSelection);
  //:~about draw active selection bound
  
  RECT rcTmp;
  
  
  //draw header parts
  if(bShowRowHeader && bShowColHeader &&pRgnClip->RectInRgn(cai.get_HeaderAlthonogalRect(rcTmp)))
  {
    const CComPtr<ICellSymbol> pSymbol=cellTypeFactory_.GetSymbol(ECT_HEADERCORNER);
    if(pSymbol){
      GdiSelectObject<> gso(hDC,CreatePen(PS_SOLID,0,HCLR_GRID));
      DrawCellInfo dci;
      ZeroMemory(&dci,sizeof(dci));
      BOOL const bSelectAll=pSheet->IsSelectAll();
      dci.clrBkgnd=SetupHeaderCornerDE(pSheet,bSelectAll);
      dci.nSelState=bSelectAll?EST_IN_SELECTION:EST_NOT_IN_SELECTION;
      dci.rcCell=rcTmp;
      pSymbol->Draw(hDC,HEADER_ROW,HEADER_COL,&dci);
    }
  }
  if(bShowColHeader && cols>0){
    if(IsFreezeColVisible && pRgnClip->RectInRgn(cai.get_HeaderHFreezeRect(rcTmp)))
      draw_ColHeader(hDC,&rcTmp,nFreezeLeftCol,nFreezeRightCol,pRgnClip);
    if(pRgnClip->RectInRgn(cai.get_HeaderHScrollRect(rcTmp))){
      draw_ColHeader(hDC,&rcTmp,nLeftVisScrollCol,cols-1,pRgnClip);
    }
  }
  if(bShowRowHeader && rows>0){
    if(IsFreezeRowVisible && pRgnClip->RectInRgn(cai.get_HeaderVFreezeRect(rcTmp)))
      draw_RowHeader(hDC,&rcTmp,nFreezeTopRow,nFreezeBottomRow,pRgnClip);
    if(pRgnClip->RectInRgn(cai.get_HeaderVScrollRect(rcTmp)))
      draw_RowHeader(hDC,&rcTmp,nTopVisScrollRow,rows-1/*nBottomVisScrollRow*/,pRgnClip);
  }
  //:~draw header parts
  
  //draw EGP_FREEZE_ALTHOGONAL 画冻结正交区
  if( IsFreezeRowVisible && IsFreezeColVisible && pRgnClip->RectInRgn(cai.get_FreezeAlthonogalRect(rcTmp)) ) {
    calc_cells_session(pRgnClip,nFreezeLeftCol,nFreezeTopRow,nFreezeRightCol,nFreezeBottomRow,&rcTmp,session_);
    //const CellRange clip(nFreezeTopRow,nFreezeLeftCol,nFreezeBottomRow,nFreezeRightCol);
    draw_session(hDC,session_,EGP_SCROLLABLE,activeSelection);
  }
  if(IsFreezeRowVisible && pRgnClip->RectInRgn(cai.get_FreezeHScrollRect(rcTmp))) {
    calc_cells_session(pRgnClip,nLeftVisScrollCol,nFreezeTopRow,nRightVisScrollCol,nFreezeBottomRow,&rcTmp,session_);
    //const CellRange clip(nFreezeTopRow,nLeftVisScrollCol,nFreezeBottomRow,nRightVisScrollCol);
    draw_session(hDC,session_,EGP_SCROLLABLE,activeSelection);
  }
  if(IsFreezeColVisible && pRgnClip->RectInRgn(cai.get_FreezeVScrollRect(rcTmp))) {
    calc_cells_session(pRgnClip,nFreezeLeftCol,nTopVisScrollRow,nFreezeRightCol,nBottomVisScrollRow,&rcTmp,session_);
    //const CellRange clip(nTopVisScrollRow,nFreezeLeftCol,nBottomVisScrollRow,nFreezeRightCol);
    draw_session(hDC,session_,EGP_SCROLLABLE,activeSelection);
  }
  if(rows>0 && cols>0 && pRgnClip->RectInRgn(cai.get_ScrollRect(rcTmp))) {
    //calc_scrollCellsSession(rh,ch,&de.rScroll);
    //draw_content(hDC,&de.rScroll,pRgnClip);
    calc_cells_session(pRgnClip,nLeftVisScrollCol,nTopVisScrollRow,nRightVisScrollCol,nBottomVisScrollRow,&rcTmp,session_);
    //const CellRange clip(nTopVisScrollRow,nLeftVisScrollCol,nBottomVisScrollRow,nRightVisScrollCol);
    draw_session(hDC,session_,EGP_SCROLLABLE,activeSelection);
  }
  
  //draw remain parts
  int bottomRow=nBottomVisScrollRow;
  //if(0==rows || bottomRow==rows-1)
  {
    SetRect(&rcTmp,0,cai.HRemainAxis,cai.VClientAxis,cai.HClientAxis);
    if(pRgnClip->RectInRgn(rcTmp))
      FillRect(hDC,&rcTmp,(HBRUSH) GetStockObject(WHITE_BRUSH));
  }
  //if(0==cols || nRightVisScrollCol==cols-1)
  {
    SetRect(&rcTmp,cai.VRemainAxis,0,cai.VClientAxis,cai.HRemainAxis);
    if(pRgnClip->RectInRgn(rcTmp))
      FillRect(hDC,&rcTmp,(HBRUSH) GetStockObject(WHITE_BRUSH));
  }
  
}
  
#endif