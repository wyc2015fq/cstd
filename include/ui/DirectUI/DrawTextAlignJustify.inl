
typedef struct _string {LPCTSTR s; int l;} string;
typedef struct _stringinfo {
  string first;
  RECT second;
}stringinfo;

//判断文本是否具有换行符号
BOOL Text_HasReturnChar(LPCTSTR lpText,int nCount) {
  int i;
  for( i=0;i<nCount-1;++i){
    if(0x0a==lpText[i])
      return TRUE;
  }
  return FALSE;
}

#define STRINGADD(word, str)    if (word.s) {word.l=(str)-word.s+1; } else {word.s = str;word.l=1;}
//分割文本，如果是汉字则按字进行分割，如果是英文则以空格
//为标志分割，SIZE存储分割项的宽度和高度
int split_text(HDC hDC,LPCTSTR lpText,int nCount,stringinfo* vl, int maxvl, int* pnvl) {
  int height=0;
  string line;
  int i=0, nvl=0;
  stringinfo pr;
  for(;i<nCount-1;++i) {
    //if(0x0d==lpText[i] && 0x0a==lpText[i+1]){
    if(0x0a==lpText[i]){
      if(0!=line.l) {
        pr.first=line;
        DrawText(hDC,line.s,line.l,&pr.second,DT_CALCRECT);
        vl[nvl++]=pr;
        height+=pr.second.bottom - pr.second.top;
        line.s=0, line.l=0;
        //++i;
      }
    } else {
      //line+=lpText[i];
      STRINGADD(line, lpText+i);
    }
  }
  STRINGADD(line, lpText+nCount-1);
  //if(!(0x0d==lpText[i] && 0x0a==lpText[i+1])){
  if(!(0x0a==lpText[i])){
    pr.first=line;
    DrawText(hDC,line.s,line.l,&pr.second,DT_CALCRECT);
    vl[nvl++]=pr;
    height+=pr.second.bottom - pr.second.top;
  }
  *pnvl = nvl;
  return height;
}
bool DrawTextAlignJustify(HDC hDC,LPCRECT lprc,LPCTSTR lpText,int nCount,long nFormat) {
  int i, cx=0;
  string word = {0,0};
  stringinfo vec[1000];
  int nvec=0;
  int cxSpace;
  for(i=0;i<nCount;){
    if(_T(' ')==lpText[i] || _T('\t')==lpText[i]|| _T('\r')==lpText[i] || _T('\n')==lpText[i] || lpText[i]<0)
    {
      if(0!=word.l){
        stringinfo pr;
        pr.first=word;
        DrawText(hDC,word.s,word.l,&pr.second,DT_CALCRECT);
        cx+=pr.second.right-pr.second.left;
        vec[nvec++]=(pr);
        word.s=0, word.l=0;
      }
      if(lpText[i]<0){
        stringinfo pr;
        pr.first.s=lpText+i; ++i;
        pr.first.l=2;
        DrawText(hDC,pr.first.s,pr.first.l,&pr.second,DT_CALCRECT);
        cx+=pr.second.right-pr.second.left;
        vec[nvec++]=(pr);
      }
    } else {
      //word+=lpText[i];
      STRINGADD(word, lpText+i);
    }
    ++i;
  }
  if(0!=word.l){
    stringinfo pr;
    pr.first=word;
    DrawText(hDC,word.s,word.l,&pr.second,DT_CALCRECT);
    cx+=pr.second.right-pr.second.left;
    vec[nvec++]=(pr);
    word.s=0, word.l=0;
  }

  //CRect rc(*lprc);
  cxSpace=(int)(((lprc->right - lprc->left)-cx)*1./(nvec-1));
  if(cxSpace>=0 && nvec>1){
    RECT r=*lprc;
    stringinfo* it;
    for( it=vec;it!=vec+nvec;++it)
    {
      r.right=r.left+it->second.right-it->second.left;
      DrawText(hDC,it->first.s,it->first.l,&r, nFormat);
      r.left=r.right+cxSpace;
    }
    return true;
  }
  //nFormat|=DT_CENTER;
  return false;
}

typedef struct _CellFontSetupInfo {
  HDC dc;
  HFONT hOldFont,hNewFont_;
  COLORREF oldTextColor;
}CellFontSetupInfo;
CellFontSetup(CellFontSetupInfo* info, HDC hDC,const LOGFONT* pLogFont,COLORREF clrFont)
{
  int mode=SetMapMode(hDC,MM_TEXT);
  LONG lfHeight=0;
  info->dc = (hDC), info->hOldFont = (NULL);
  if(pLogFont){
    lfHeight=pLogFont->lfHeight;
    if(MM_TEXT==GetMapMode(hDC))
      ((LOGFONT*)pLogFont)->lfHeight=-MulDiv(pLogFont->lfHeight, GetDeviceCaps(hDC, LOGPIXELSY), 72);
    info->hNewFont_= CreateFontIndirect(pLogFont);
    if(info->hNewFont_){
      info->oldTextColor=SetTextColor(info->dc, clrFont);
      info->hOldFont=SelectFont(info->dc, info->hNewFont_);
    }
  }
  if(mode)
    SetMapMode(hDC,mode);
  if(pLogFont)
    ((LOGFONT*)pLogFont)->lfHeight=lfHeight;
  return 0;
}

int CellFontUnSetup(CellFontSetupInfo* info)
{
  if(info->hOldFont){
    SelectFont(info->dc, info->hOldFont);
    SetTextColor(info->dc, info->oldTextColor);
  }
  if(info->hNewFont_)
    DeleteObject(info->hNewFont_);
  return 0;
}

