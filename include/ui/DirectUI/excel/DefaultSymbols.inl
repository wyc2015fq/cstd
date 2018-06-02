
void FillSolidRect2( HDC m_hDC, LPCRECT lpRect, COLORREF clr ) {
  ASSERT( m_hDC != NULL );
  SetBkColor( m_hDC, clr );
  ExtTextOut( m_hDC, 0, 0, ETO_OPAQUE, lpRect, NULL, 0, NULL );
}
int SimpleFillSymbol_Draw(HDC hDC, const DrawCellInfo* pDCI) {
  FillSolidRect2(hDC, &pDCI->rcCell, pDCI->clrBkgnd);
  return 0;
}
typedef struct CString {
  LPCTSTR s;
  int l;
}CString;
typedef struct pair {
  CString first;
  RECT second;
}pair;
bool DrawTextAlignJustify(HDC hDC,LPCRECT lprc,LPCTSTR lpText,int nCount,long nFormat)
{
  int cxSpace;
  int i, cx=0;
  CString word={0,0};
  pair vec[100];
  int nvec=0;
  for(i=0;i<nCount;){
    if(_T(' ')==lpText[i] || _T('\t')==lpText[i]|| _T('\r')==lpText[i] || _T('\n')==lpText[i] || lpText[i]<0)
    {
      if(0!=word.l){
        pair pr;
        pr.first=word;
        DrawText(hDC,word.s,word.l,&pr.second,DT_CALCRECT);
        cx+=pr.second.right-pr.second.left;
#define push_back(vec, x)  vec[n##vec++]=x
#define Empty(x)  x.s=0, x.l=0
        push_back(vec,pr);
        Empty(word);
      }
      if(lpText[i]<0){
        pair pr = {0,0,0};
        pr.first.s=lpText+i;
        pr.first.l++;
        DrawText(hDC,pr.first.s,pr.first.l,&pr.second,DT_CALCRECT);
        cx+=pr.second.right-pr.second.left;
        push_back(vec,pr);
      }
    } else {
      if (0==word.s) {word.s=lpText+i;}
      word.l++;
    }
    ++i;
  }
  if(0!=word.l){
    pair pr;
    pr.first=word;
    DrawText(hDC,word.s,word.l,&pr.second,DT_CALCRECT);
    cx+=pr.second.right-pr.second.left;
    push_back(vec,pr);
    Empty(word);
  }
  
  //RECT rc(*lprc);
  cxSpace=(int)(((lprc->right - lprc->left)-cx)/(double)(nvec-1));
  if(cxSpace>=0 && nvec>1){
    RECT r=*lprc;
    int i;
    for(i=0;i<nvec;++i)
    {
      r.right=r.left+vec[i].second.right-vec[i].second.left;
      DrawText(hDC,vec[i].first.s,vec[i].first.l,&r, nFormat);
      r.left=r.right+cxSpace;
    }
    return true;
  }
  //nFormat|=DT_CENTER;
  return false;
}

//判断文本是否具有换行符号
BOOL Text_HasReturnChar(LPCTSTR lpText,int nCount)
{
  int i;
  for(i=0;i<nCount-1;++i){
    if(0x0a==lpText[i])
      return TRUE;
  }
  return FALSE;
}
//分割文本，如果是汉字则按字进行分割，如果是英文则以空格
//为标志分割，SIZE存储分割项的宽度和高度
int split_text(HDC hDC,LPCTSTR lpText,int nCount,pair* vl, int *pnvl)
{
  int height=0;
  RECT _rc={0};
  CString line;
  pair si;
  int nvl=0;
  int i=0;
  for(;i<nCount-1;++i){
    //if(0x0d==lpText[i] && 0x0a==lpText[i+1]){
    if(0x0a==lpText[i]){
      if(0!=line.l)
      {
        DrawText(hDC,line.s,line.l,&_rc,DT_CALCRECT);
        si.first = line;
        si.second = _rc;
        push_back(vl, si);
        height+=_rc.bottom-_rc.top;
        Empty(line);
        //++i;
      }
    }else{
      if (0==line.s) {line.s=lpText+i;}
      line.l++;
#define ADDCH(line, ch)  {if (0==line.s) {line.s=&(ch);} line.l++;}
    }
  }
  if (0==line.s) {line.s=lpText+nCount-1;}
  line.l++;
  //if(!(0x0d==lpText[i] && 0x0a==lpText[i+1])){
  if(!(0x0a==lpText[i])){
    DrawText(hDC,line.s,line.l,&_rc,DT_CALCRECT);
    si.first = line;
    si.second = _rc;
    push_back(vl, si);
    height+=_rc.bottom-_rc.top;
  }
  *pnvl = nvl;
  return height;
}
#define DeflateRect( lprc, cx, cy ) InflateRect( lprc, -(cx), -(cy) )
void draw_text(HDC hDC,EXlHAlign xa,LPCTSTR lpText, int nCount,LPCRECT lprc)
{
  RECT rc=*lprc;
  long nFormat=DT_VCENTER|DT_SINGLELINE;///*|DT_CENTER*/|DT_EDITCONTROL;//DT_END_ELLIPSIS;
  DeflateRect(&rc,1,1);
  
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
    case xlHAlignJustify:////分散对齐
      {  
        if(DrawTextAlignJustify(hDC,&rc,lpText,nCount,nFormat)){
          return;
        }else{
          nFormat|=DT_CENTER;
        }
      }break;;
    }
  }
  DrawText(hDC,lpText, nCount, &rc, nFormat);
}
typedef struct CellFontSetup {
  HDC dc;
  HFONT hOldFont,hNewFont_;
  COLORREF oldTextColor;
}CellFontSetup;

int CellFontSetup_begin(CellFontSetup* me, HDC hDC,const LOGFONT* pLogFont,COLORREF clrFont)
{
  int mode=SetMapMode(hDC,MM_TEXT);
  LONG lfHeight=0;
  me->dc=(hDC),me->hOldFont=(NULL);
  if(pLogFont){
    lfHeight=pLogFont->lfHeight;
    if(MM_TEXT==GetMapMode(hDC))
      ((LOGFONT*)pLogFont)->lfHeight=-MulDiv(pLogFont->lfHeight, GetDeviceCaps(hDC, LOGPIXELSY), 72);
    me->hNewFont_=CreateFontIndirect(pLogFont);
    if(me->hNewFont_){
      me->oldTextColor=SetTextColor(me->dc, clrFont);
      me->hOldFont=SelectFont(me->dc, me->hNewFont_);
    }
  }
  if(mode)
    SetMapMode(hDC,mode);
  if(pLogFont)
    ((LOGFONT*)pLogFont)->lfHeight=lfHeight;
  return 0;
}
int CellFontSetup_end(CellFontSetup* me)
{
  if(me->hOldFont){
    SelectFont(me->dc, me->hOldFont);
    SetTextColor(me->dc, me->oldTextColor);
  }
  if(me->hNewFont_)
    DeleteObject(me->hNewFont_);
  return 0;
}
int TextSymbol_Draw(HDC hDC, const DrawCellInfo* pDCI)
{
  LPCRECT lprc=&pDCI->rcCell;
  //LPCTSTR lpText=(LPCTSTR)pDCI->pdata;
  LPCTSTR lpText = (pDCI->text);
  int nCount;

  if (NULL==lpText) return 0;
  nCount = lstrlen(lpText);
  
  if(nCount>0){
    CellFontSetup cfs;
    EXlHAlign xa;
    CellFontSetup_begin(&cfs, hDC, (LOGFONT*)pDCI->pLogFont,pDCI->clrFont);
    xa =pDCI->hAlign;
    if(xlHAlignJustify==xa || pDCI->bWrapText){
      pair vl[100];
      int i, nvl=0;
      int hi=split_text(hDC,lpText,nCount,vl, &nvl);
      int nRcHeight=lprc->bottom-lprc->top;
      int iTop=MAX(0,(nRcHeight-hi)>>1);
      int MARGIN=2;
      for(i; i<nvl; ++i){
        RECT _rc={lprc->left,lprc->top+iTop,lprc->right,lprc->top+iTop+vl[i].second.bottom-vl[i].second.top+MARGIN};
        draw_text(hDC,xa,vl[i].first.s,vl[i].first.l,&_rc);
        iTop+=_rc.bottom-_rc.top;
        if(iTop>nRcHeight)
          break;
      }
    }else{
      draw_text(hDC,xa,lpText,nCount,lprc);
    }
    CellFontSetup_end(&cfs);
  }
  return S_OK;
}
static POINT cPOINT( int initX, int initY ) {
  POINT pt;
  pt.x = initX, pt.y = initY;
  return pt;
}
#define CenterPoint( rect )  cPOINT( ( ( rect ).left + ( rect ).right ) / 2, ( ( rect ).top + ( rect ).bottom ) / 2 );

int CheckBoxSymbol_Draw(HDC hDC, const DrawCellInfo* pDCI)
{
  LPCRECT lprc;
  RECT rc;
  POINT _pt;
  UINT uFlag;
  int cx;
  BOOL bCheck=FALSE;
  lprc =&pDCI->rcCell;
  //LPCTSTR lpText=(LPCTSTR)pDCI->pdata;
  //int nCount=pDCI->nDataLen;
  bCheck=pDCI->nData;
  uFlag = DFCS_BUTTONCHECK|DFCS_FLAT;
  if(bCheck)
    uFlag|=DFCS_CHECKED;
  rc = *lprc;

  _pt = CenterPoint(rc);
  cx = CHECKBOX_HALF_WIDTH;
  SetRect(&rc, _pt.x-cx,_pt.y-cx,_pt.x+cx,_pt.y+cx);
  DrawFrameControl(hDC,&rc,DFC_BUTTON,uFlag);
  return 0;
}
int ColorSymbol_Draw(HDC hDC, const DrawCellInfo* pDCI)
{
  LPCRECT lprc=&pDCI->rcCell;
  //LPCTSTR lpText=(LPCTSTR)pDCI->pdata;
  COLORREF crColor = pDCI->nData;
  HBRUSH br;
  RECT rc={lprc->left+1,lprc->top+1,lprc->right-1,lprc->bottom-1};
  br=CreateSolidBrush(crColor);
  FillRect(hDC, &rc,br);
  DeleteObject(br);
  return 0;
}
int HeaderSymbol_Draw(HDC hDC, const DrawCellInfo* pDCI)
{
  HBRUSH solidBrush;
  LPCRECT lprc=&pDCI->rcCell;
  RECT rc=*lprc;
  if(!(lprc->right > lprc->left && lprc->bottom > lprc->top))
    return S_OK;
  solidBrush = CreateSolidBrush(pDCI->clrBkgnd);
  FillRect(hDC, &rc, solidBrush);
  //画线
  MoveTo(hDC, rc.left,rc.bottom-1);
  LineTo(hDC, rc.right-1,rc.bottom-1);
  LineTo(hDC, rc.right-1,rc.top-1);
  DeleteObject(solidBrush);
  
  if(lprc->right > lprc->left+2 && lprc->bottom > lprc->top+2)
  {
    LPCTSTR str = (pDCI->text);
    if(str){
      long nFormat=DT_VCENTER|DT_SINGLELINE|DT_CENTER;
      DeflateRect(&rc, 1,1);
      DrawText(hDC, str, lstrlen(str), &rc, nFormat);
    }
  }
  return S_OK;
}
#define countof(arr) (sizeof(arr)/sizeof((arr)[0]))
#define SetSymbol(type, fun)  funtable[type]=fun##_Draw
int Symbol_Draw(HDC hDC, const DrawCellInfo* pDCI) {
  typedef int (*Symbol_Draw_Fun_t)(HDC hDC, const DrawCellInfo* pDCI);
  static Symbol_Draw_Fun_t funtable[100];
  ECellType type = pDCI->cellType;
  int inited=0;
  if (0==inited) {
    int i;
    inited=1;
    memset(funtable, 0, sizeof(funtable));
    for (i=0; i<countof(funtable); ++i) {
      if (i>ECT_NONE && i<ECT_HEADERCORNER) {
        SetSymbol(i,TextSymbol);
      } else {
        SetSymbol(i,SimpleFillSymbol);
      }
    }
    SetSymbol(ECT_ROWHEADER,HeaderSymbol);
    SetSymbol(ECT_COLHEADER,HeaderSymbol);
    SetSymbol(ECT_HEADERCORNER,HeaderSymbol);
    SetSymbol(ECT_CHECKBOX,CheckBoxSymbol);
    SetSymbol(ECT_COLOR,ColorSymbol);
  }
  if (type<countof(funtable) && funtable[type]) {
    funtable[type](hDC, pDCI);
  }
  return 0;
}
