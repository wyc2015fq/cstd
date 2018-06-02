
#include "DrawTextAlignJustify.inl"

void draw_text(HDC dc,EXlHAlign xa,LPCTSTR lpText, int nCount,LPCRECT lprc)//const
{
  RECT rc=*lprc;
  long nFormat=DT_VCENTER|DT_SINGLELINE;///*|DT_CENTER*/|DT_EDITCONTROL;//DT_END_ELLIPSIS;
  DeflateRect(&rc, 1,1);
  if(1/*!styleDesc.IsEmptyStyle() && !styleDesc.align.IsNull()*/){
    //EXlHAlign const xa=styleDesc.GetHAlign();
    switch(xa)
    {
    case xlHAlignCenter:
      nFormat|=DT_CENTER;
      break;
    case xlHAlignLeft:
      nFormat|=DT_LEFT;
      break;
    case xlHAlignRight:
      nFormat|=DT_RIGHT;
      break;
    case xlHAlignJustify:////·ÖÉ¢¶ÔÆë
      {  
        if(DrawTextAlignJustify(dc,&rc,lpText,nCount,nFormat)){
          return;
        }else{
          nFormat|=DT_CENTER;
        }
      }break;;
    }
  }
  DrawText(dc,lpText, nCount, &rc, nFormat);
}
TextSymbol_Draw(HDC hDC, int row,int col,const DrawCellInfo* pDCI)
{
  HDC dc=(hDC);
  LPCRECT lprc=&pDCI->rcCell;
  LPCTSTR lpText;
  int nCount;
  if(!pDCI->pdata)
    return S_FALSE;
  lpText = pDCI->pdata;

  nCount =strlen(lpText);
  
  if(nCount>0){
    CellFontSetupInfo cfs;
    EXlHAlign const xa=pDCI->hAlign;
    CellFontSetup(&cfs, dc,(LOGFONT*)pDCI->pLogFont,pDCI->clrFont);
    if(xlHAlignJustify==xa || pDCI->bWrapText){
      stringinfo vl[100];
      int nvl=0;
      int hi=split_text(dc,lpText,nCount,vl, 100, &nvl);
      int nRcHeight=lprc->bottom-lprc->top;
      int iTop=max(0,(nRcHeight-hi)>>1);
      int const MARGIN=2;
      stringinfo* it;
      for( it=vl;it!=vl+nvl;++it) {
        RECT _rc={lprc->left,lprc->top+iTop,lprc->right,lprc->top+iTop+rcHeight((*it).second)+MARGIN};
        draw_text(dc,xa,(*it).first.s,(*it).first.l,&_rc);
        iTop+=_rc.bottom-_rc.top;
        if(iTop>nRcHeight)
          break;
      }
    } else {
      draw_text(dc,xa,lpText,nCount,lprc);
    }
    CellFontUnSetup(&cfs);
  }
  return S_OK;
}
int ThemeCheckBoxSymbol_Draw(HDC hDC, int row,int col,const DrawCellInfo* pDCI, BOOL bCheck)
{
  LPCRECT lprc=&pDCI->rcCell;
  RECT rc=*lprc;
  POINT _pt=CenterPoint(&rc);
  int cx=CHECKBOX_HALF_WIDTH;
  UINT uFlag=DFCS_BUTTONCHECK|DFCS_FLAT;
  if(bCheck)
    uFlag|=DFCS_CHECKED;
  SetRect(&rc, _pt.x-cx,_pt.y-cx,_pt.x+cx,_pt.y+cx);
  DrawFrameControl(hDC,&rc,DFC_BUTTON,uFlag);
  return S_OK;
}
int ColorSymbol_Draw(HDC hDC, int row,int col,const DrawCellInfo* pDCI, COLORREF crColor )
{
  HDC dc=(hDC);
  LPCRECT lprc=&pDCI->rcCell;
  RECT rc={lprc->left+1,lprc->top+1,lprc->right-1,lprc->bottom-1};
  HBRUSH br;
  br=CreateSolidBrush(crColor);
  FillRect(dc, &rc,br);
  DeleteObject(br);
  return S_OK;
}
int HeaderSymbol_Draw(HDC hDC, int row,int col,const DrawCellInfo* pDCI)
{
  HDC dc=(hDC);
  LPCRECT lprc=&pDCI->rcCell;
  RECT rc=*lprc;
  if(!(lprc->right > lprc->left && lprc->bottom > lprc->top))
    return S_OK;
  {
    HBRUSH solidBrush;
    solidBrush=CreateSolidBrush(pDCI->clrBkgnd);
    FillRect(dc, &rc,solidBrush);
    //»­Ïß
    MoveTo(dc, rc.left,rc.bottom-1);
    LineTo(dc, rc.right-1,rc.bottom-1);
    LineTo(dc, rc.right-1,rc.top-1);
    DeleteObject(solidBrush);
  }
  if(pDCI->pdata && lprc->right > lprc->left+2 && lprc->bottom > lprc->top+2)
  {
    long nFormat=DT_VCENTER|DT_SINGLELINE|DT_CENTER;
    LPCTSTR str = pDCI->pdata;
    DeflateRect(&rc, 1,1);
    DrawText(dc, str,strlen(str), &rc, nFormat);
  }
  return S_OK;
}
