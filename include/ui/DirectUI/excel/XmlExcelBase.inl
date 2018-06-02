
//#define atobool(v) (0==strcmp(v, "true"))
#define atobool(v) ((0==strcmp(v, "0")||0==stricmp(v, "false"))?FALSE:TRUE)
#define SetAttribute(X, N, V, D)  {LPCTSTR v = GetAttributeValueByName(pE, N); if(v){X = V;} else {X=D;}}

BOOL ReadIntFromAttribute(CMarkupNode* pE, LPCTSTR LPCTSTRAttrName,int* val)
{
  LPCTSTR v;
  LPTSTR v2;
  v = GetAttributeValueByName(pE, LPCTSTRAttrName);
  *val = strtol(v, &v2, 10);
  if (v!=v2) {
    return TRUE;
  }
  return FALSE;
}
BOOL ReadDoubleFromAttribute(CMarkupNode* pE,LPCTSTR LPCTSTRAttrName,double* val)
{
  LPCTSTR v;
  LPTSTR v2;
  v = GetAttributeValueByName(pE, LPCTSTRAttrName);
  *val = strtod(v, &v2);
  if (v!=v2) {
    return TRUE;
  }
  return FALSE;
}
LPCTSTR EXlLineStyleToString(EXlLineStyle ls)
{
  LPCTSTR str;
  switch(ls)
  {
  case xlLineStyleNone:  break;
  case xlContinuous:str="Continuous";break;
  case xlDash:str="Dash";break;
  case xlDashDot:str="DashDot";break;
  case xlDashDotDot:str="DashDotDot";break;
  case xlDot:str="Dot";break;
  case xlDouble:str="Double";break;
  case xlSlantDashDot:  break;
  }
  return str;
}
EXlLineStyle EXlLineStyleFromString(LPCTSTR LPCTSTRLineStyle)
{
  LPCTSTR str=(LPCTSTRLineStyle);
  if(0==strcmp(str,"Continuous")) return xlContinuous;
  if(0==strcmp(str,"Dash")) return xlDash;
  if(0==strcmp(str,"DashDot")) return xlDashDot;
  if(0==strcmp(str,"DashDotDot")) return xlDashDotDot;
  if(0==strcmp(str,"Dot")) return xlDot;
  if(0==strcmp(str,"Double")) return xlDouble;
  //case xlSlantDashDot:
  //  break;
  //}
  return xlLineStyleNone;
}
LPCTSTR EXlHAlignToStr(EXlHAlign ha)
{
  LPCTSTR LPCTSTRHAlignment=("Center");
  switch(ha)
  {
  case xlHAlignCenter:LPCTSTRHAlignment="Center";break;// = 0,        //居中        
  case xlHAlignCenterAcrossSelection:LPCTSTRHAlignment="CenterAcrossSelection";break;//,  //跨列居中
  case xlHAlignDistributed:LPCTSTRHAlignment="Justify";break;//,      //两端对齐
  case xlHAlignFill:LPCTSTRHAlignment="Fil";break;//,          //填充
  case xlHAlignGeneral:LPCTSTRHAlignment="Top";break;//,        //常规
  case xlHAlignJustify:LPCTSTRHAlignment="Distributed";break;//,        //分散对齐
  case xlHAlignLeft:LPCTSTRHAlignment="Left";break;//,          //靠左（缩近）
  case xlHAlignRight:LPCTSTRHAlignment="Right";break;//        //靠右（缩近）
  }
  return LPCTSTRHAlignment;
}
EXlHAlign EXlHAlignFromStr(LPCTSTR _LPCTSTRHAlignment)
{
  LPCTSTR LPCTSTRHAlignment=(_LPCTSTRHAlignment);
  if(0==strcmp(LPCTSTRHAlignment,"Center")) return xlHAlignCenter;
  if(0==strcmp(LPCTSTRHAlignment,"CenterAcrossSelection")) return xlHAlignCenterAcrossSelection;  //跨列居中
  if(0==strcmp(LPCTSTRHAlignment,"Justify")) return xlHAlignDistributed;//,      //两端对齐
  if(0==strcmp(LPCTSTRHAlignment,"Fil")) return xlHAlignFill;//,          //填充
  if(0==strcmp(LPCTSTRHAlignment,"Top")) return xlHAlignGeneral;//,        //常规
  if(0==strcmp(LPCTSTRHAlignment,"Distributed")) return xlHAlignJustify;//,        //分散对齐
  if(0==strcmp(LPCTSTRHAlignment,"Left")) return xlHAlignLeft;//,          //靠左（缩近）
  if(0==strcmp(LPCTSTRHAlignment,"Right")) return xlHAlignRight;//        //靠右（缩近）
  return xlHAlignCenter;
}

LPCTSTR EXlVAlignToStr(EXlVAlign va)
{
  LPCTSTR LPCTSTRVAlignment=("Bottom");
  switch(va)
  {
  case xlVAlignBottom:LPCTSTRVAlignment="Bottom";break;// = 0,  //靠下
  case xlVAlignCenter:LPCTSTRVAlignment="Center";break;//,    //居中
  case xlVAlignDistributed:LPCTSTRVAlignment="Justify";break;//,//两端对齐
  case xlVAlignJustify:LPCTSTRVAlignment="Distributed";break;//,  //分散对齐
  case xlVAlignTop:LPCTSTRVAlignment="Top";break;//        //靠上
  }
  return LPCTSTRVAlignment;
}
EXlVAlign EXlVAlignFromStr(LPCTSTR _LPCTSTRVAlignment)
{
  LPCTSTR LPCTSTRVAlignment=(_LPCTSTRVAlignment);
  if(0==strcmp(LPCTSTRVAlignment,"Bottom")) return xlVAlignBottom;// = 0,  //靠下
  if(0==strcmp(LPCTSTRVAlignment,"Center")) return xlVAlignCenter;//,    //居中
  if(0==strcmp(LPCTSTRVAlignment,"Justify")) return xlVAlignDistributed;//,//两端对齐
  if(0==strcmp(LPCTSTRVAlignment,"Distributed")) return xlVAlignJustify;//,  //分散对齐
  if(0==strcmp(LPCTSTRVAlignment,"Top")) return xlVAlignTop;//        //靠上
  return xlVAlignCenter;
}
//水平方向磅转换到像素
int X_Point2Pixel(double nPoint)
{
  HDC hDC=GetDC(NULL);
  const double dpi=GetDeviceCaps(hDC,LOGPIXELSX);
  ReleaseDC(NULL,hDC);
  return (int)(nPoint*dpi/72);
}
//垂直方向磅转换到像素
int Y_Point2Pixel(double nPoint)
{
  HDC hDC=GetDC(NULL);
  const double dpi=GetDeviceCaps(hDC,LOGPIXELSY);
  ReleaseDC(NULL,hDC);
  return (int)(nPoint*dpi/72);
}
//
//水平方向像素转换到磅
double X_Pixel2Point(int nPixel)
{
  HDC hDC=GetDC(NULL);
  const double dpi=GetDeviceCaps(hDC,LOGPIXELSX);
  ReleaseDC(NULL,hDC);
  return (nPixel*72.0f)/dpi;
}
//垂直方向像素转换到磅
double Y_Pixel2Point(int nPixel)
{
  HDC hDC=GetDC(NULL);
  const double dpi=GetDeviceCaps(hDC,LOGPIXELSY);
  ReleaseDC(NULL,hDC);
  return (nPixel*72.0f)/dpi;
}

/**
strColor like #808080
*/
COLORREF RGBFromHtmlString(LPCTSTR strColor)
{
  char* stopstring=NULL;
  BYTE r, g, b;
  char szR[3],szG[3],szB[3];
  //USES_CONVERSION;
  //const char* string=OLE2CA(strColor);
  if(strlen(strColor)<7) return RGB(0,0,0);
  memcpy(szR,&strColor[1],2);
  memcpy(szG,&strColor[3],2);
  memcpy(szB,&strColor[5],2);
  szR[2]=szG[2]=szB[2]='\0';
  r =(BYTE)strtol(szR,&stopstring,16);
  g=(BYTE)strtol(szG,&stopstring,16);
  b=(BYTE)strtol(szB,&stopstring,16);
  return RGB(r,g,b);
}
/**
strColor like #808080
在atlutil.h中有一个函数RGBToHtml与此类似
*/
LPCTSTR RGBToHtmlString(COLORREF color)
{
  static TCHAR buf[8]={0};
  wsprintf(buf, _T("#%0.2x%0.2x%0.2x"), GetRValue(color), GetGValue(color), GetBValue(color));
  return buf;
}

void StyleE2BorderStyle(CMarkupNode* pE,Border* bdr)
{
  LPCTSTR v;
  v = GetAttributeValueByName(pE, "ss:LineStyle");
  if(v){
    EXlLineStyle ls=EXlLineStyleFromString(v);
    Border_put_LineStyle(*bdr, ls);
  }
  
  v = GetAttributeValueByName(pE, "ss:Weight");
  if(v){
    Border_put_Weight(*bdr, atol(v));
  }
  
  v = GetAttributeValueByName(pE, "ss:Color");
  if(v){
    COLORREF clr=RGBFromHtmlString(v);
    TIndex idx=find_color(clr);
    if(idx>=0){
      Border_put_ColorIndex(*bdr, idx);
    }
  }
}
int StyleE2StyleDesc(CMarkupNode* pStyleN,StyleDesc* sd)
{
  LPCTSTR v;
  LPCTSTR pstrName = NULL;
  LPCTSTR pstrValue = NULL;
  LPTSTR pstr = NULL;
  LPCTSTR pstrClass;
  CMarkupNode node;
  CMarkupNode* pE = &node;
  node = GetChildNode(pStyleN, 0);
  for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    pstrClass = GetName(&node);
    if (0==strcmp("Alignment",pstrClass)) {//Alignment
      SetAttribute(sd->hAlign, "ss:Horizontal", EXlHAlignFromStr(v), 0);
      SetAttribute(sd->vAlign, "ss:Vertical", EXlVAlignFromStr(v), 0);
      SetAttribute(sd->bWrapText, "ss:WrapText", atobool(v), 0);
    }
    if (0==strcmp("Font",pstrClass)) {//Font
      LOGFONT* lf=&Font_GetLogFont(*sd);
      v = GetAttributeValueByName(pE, "ss:FontName");
      if(v){
        lstrcpy(lf->lfFaceName, (v));
      }
      
      SetAttribute(lf->lfCharSet, "x:CharSet", (BYTE)atol(v), 0);
      SetAttribute(lf->lfHeight, "ss:Size", atol(v), 0);
      v = GetAttributeValueByName(pE, "ss:Bold");
      if(v){
        Font_SetBold(*sd, atol(v));
      }
      SetAttribute(lf->lfStrikeOut, "ss:StrikeThrough", (BYTE)atol(v), 0);
      SetAttribute(lf->lfItalic, "ss:Italic", (BYTE)atol(v), 0);
    }
    if (0==strcmp("Borders",pstrClass) || 0==strcmp("Border",pstrClass)) 
    {//Borders
      CMarkupNode node1;
      CMarkupNode* pNL = &node1;
      Borders* pbdrs=&(*sd).borders;
      node1 = GetChildNode(pE, 0);
      for ( ; IsValidNode(&node1); node1 = GetSiblingNode(&node1) ) {
        LPCTSTR bstrPos;
        bstrPos = GetAttributeValueByName(pE, "ss:Position");
        if (bstrPos) {
          if(0==strcmp(bstrPos,"Left"))
            StyleE2BorderStyle(pE,&pbdrs->left);
          if(0==strcmp(bstrPos,"Top"))
            StyleE2BorderStyle(pE,&pbdrs->top);
          if(0==strcmp(bstrPos,"Right"))
            StyleE2BorderStyle(pE,&pbdrs->right);
          if(0==strcmp(bstrPos,"Bottom"))
            StyleE2BorderStyle(pE,&pbdrs->bottom);
          if(0==strcmp(bstrPos,"DiagonalRight"))
            StyleE2BorderStyle(pE,&pbdrs->DiagonalRight);
          if(0==strcmp(bstrPos,"DiagonalLeft"))
            StyleE2BorderStyle(pE,&pbdrs->DiagonalLeft);
        }
      }
    }
    if (0==strcmp("Interior",pstrClass)) {//<Interior ss:Color="#FFFF99" ss:Pattern="Solid"/>
      v = GetAttributeValueByName(pE, "ss:Color");
      if(v) {
        const COLORREF clr=RGBFromHtmlString(v);
        BKGND_SetColor(*sd, clr);
      }
    }
  }
  return 0;
}

int LoadCells(CMarkupNode* pRowE, Worksheet* pSheet, int row)
{
  int nPreCol=-1;
  LPCTSTR pstrName = NULL;
  LPCTSTR pstrValue = NULL;
  LPTSTR pstr = NULL;
  LPCTSTR pstrClass;
  CMarkupNode node;
  StyleDesc* pStyle=NULL;
  CMarkupNode* pE = &node;
  node = GetChildNode(pRowE, 0);
  for (; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    CellStore* cell;
    pstrClass = GetName(&node);
    if (0==strcmp("Cell",pstrClass)) {
      int i, j, idx;
      cell = pSheet->Cells+pSheet->nCells;
      SetAttribute(cell->nMergeAcross, "ss:MergeAcross", atol(v), 0);
      SetAttribute(cell->nMergeDown, "ss:MergeDown", atol(v), 0);
      SetAttribute(cell->nIndex, "ss:Index", atol(v)-1, nPreCol+1);
      nPreCol = cell->nIndex+cell->nMergeAcross;
      SetAttribute(cell->nStyleID, "ss:StyleID", strtoid(v), 0);
      {//load text
        CMarkupNode DataN;
        CMarkupNode* pDataN = &DataN;
        DataN = GetChildNode(pE, 0);
        for ( ; IsValidNode(&DataN); DataN = GetSiblingNode(&DataN) ) {
          pstrClass = GetName(&DataN);
          if(0==strcmp("Data",pstrClass)) {
            pstrClass = GetValue(&DataN);
            cell->cellVal = strtoid(pstrClass);
            SetAttribute(cell->cellType, "ss:Type", strtoid(v), 0);
            //pE->selectSingleNode("ss:Data",&pDataN);
          }
        }
      }
      idx = -1;
      for (i=0; i<pSheet->nCells; ++i) {
        if (0==memcmp(cell, pSheet->Cells+i, sizeof(*cell))) {
          idx = i;
          break;
        }
      }
      if (-1==idx) {
        idx=pSheet->nCells++;
      }
      for (j=0; j<=cell->nMergeDown; ++j) {
        for (i=0; i<=cell->nMergeAcross; ++i) {
          ASSERT(cell->nIndex+i<pSheet->nColCount);
          pSheet->pCell[(row+j)*pSheet->nColCount+cell->nIndex+i] = idx;
          if (27==idx) {
            int asdf=0;
          }
        }
      }
    }
  }
  return 0;
}
#include "macro.h"
int LoadRows(CMarkupNode* pTableN, Worksheet* pSheet)
{
  int i=0;
	int nPreRow=-1;
  LPCTSTR pstrName = NULL;
  LPCTSTR pstrValue = NULL;
  LPTSTR pstr = NULL;
  LPCTSTR pstrClass;
  CMarkupNode node;
  StyleDesc* pStyle=NULL;
  CMarkupNode* pE = &node;
  node = GetChildNode(pTableN, 0);
  pSheet->nrh_=0;
  for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    pstrClass = GetName(&node);
    if (0==strcmp("Row",pstrClass)) {
      pSheet->nrh_++;
    }
  }
  pSheet->rh_ = realloc(pSheet->rh_, pSheet->nrh_*sizeof(RowHeader));
  memset(pSheet->rh_, 0, pSheet->nrh_*sizeof(RowHeader));
  node = GetChildNode(pTableN, 0);
  for ( i=0; IsValidNode(&node); node = GetSiblingNode(&node)) {
    pstrClass = GetName(&node);
    if (0==strcmp("Row",pstrClass)) {
      RowHeader* rowh = pSheet->rh_ + i++;
      SetAttribute(rowh->nIndex, "ss:Index", atol(v)-1, nPreRow+1);
      nPreRow=rowh->nIndex;
      
      SetAttribute(rowh->nHeight, "ss:Height", Y_Point2Pixel(atof(v)), 0);
      SetAttribute(rowh->nStyleID, "ss:StyleID", strtoid(v), 0);
      LoadCells(pE, pSheet, rowh->nIndex);
    }
  }
  //FPRINT2D("pCell.txt", "%3d,", "\r\n", pSheet->nRowCount, pSheet->nColCount, pSheet->pCell, pSheet->nColCount, 1);
  return 0;
}
int LoadColumns(CMarkupNode* pTableN, Worksheet* pSheet)
{
  int i=0;
  int nPreCol=-1;
  LPCTSTR pstrName = NULL;
  LPCTSTR pstrValue = NULL;
  LPTSTR pstr = NULL;
  LPCTSTR pstrClass;
  CMarkupNode node;
  StyleDesc* pStyle=NULL;
  CMarkupNode* pE = &node;
  BOOL bHide=FALSE;
  node = GetChildNode(pTableN, 0);
  pSheet->nch_=0;
  for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    pstrClass = GetName(&node);
    if (0==strcmp("Column",pstrClass)) {
      pSheet->nch_++;
    }
  }
  if (0==pSheet->nch_) return 0;
  pSheet->ch_ = realloc(pSheet->ch_, pSheet->nch_*sizeof(ColHeader));
  memset(pSheet->ch_, 0, pSheet->nch_*sizeof(ColHeader));
  node = GetChildNode(pTableN, 0);
  for ( i=0; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    pstrClass = GetName(&node);
    if (0==strcmp("Column",pstrClass)) {
      ColHeader* colh = pSheet->ch_ + i++;
      SetAttribute(colh->nIndex, "ss:Index", atol(v)-1, nPreCol+1);
      nPreCol=colh->nIndex;
      
      SetAttribute(colh->nStyleID, "ss:StyleID", strtoid(v), 0);
      SetAttribute(bHide, "ss:Hidden", atobool(v), 0);
      SetAttribute(colh->nWidth, "ss:Width", X_Point2Pixel(atof(v)), 0);
      if(bHide) {
        colh->nWidth = 0;
      }
    }
  }
  return 0;
}

HRESULT LoadWorksheetOptions(CMarkupNode* pOptions, Worksheet* pSheet)
{
  int i=0;
  int nPreCol=-1;
  LPCTSTR pstrClass;
  CMarkupNode node;
  StyleDesc* pStyle=NULL;
  CMarkupNode* pE = &node;
  BOOL bHide=FALSE;
  node = GetChildNode(pOptions, 0);
  pSheet->bShowGrid = true;
  for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    pstrClass = GetName(&node);
    if (0==strcmp(pstrClass, "DoNotDisplayGridlines")) {
      pSheet->bShowGrid = false;
    }
  }
  return 0;
}
int LoadWorkSheet(CMarkupNode* pSheetN, Worksheet* pSheet)
{
  int i=0;
  int nPreCol=-1;
  LPCTSTR pstrClass;
  CMarkupNode node;
  StyleDesc* pStyle=NULL;
  CMarkupNode* pE = &node;
  BOOL bHide=FALSE;
  node = GetChildNode(pSheetN, 0);
  for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    pstrClass = GetName(&node);
    if (0==strcmp(pstrClass, "WorksheetOptions")) {
      LoadWorksheetOptions(&node, pSheet);
    }
    if (0==strcmp(pstrClass, "Table")) {
      //load Table properties
      CMarkupNode* pTableN=&node;
      int rows=0,cols=0, n;
      SetAttribute(pSheet->nRowCount, "ss:ExpandedRowCount", atol(v), 0);
      SetAttribute(pSheet->nColCount, "ss:ExpandedColumnCount", atol(v), 0);
      SetAttribute(pSheet->defWidth, "ss:DefaultColumnWidth", X_Point2Pixel(atof(v)), 0);
      SetAttribute(pSheet->defHeight, "ss:DefaultRowHeight", Y_Point2Pixel(atof(v)), 0);
      SetAttribute(pSheet->nStyleID, "ss:StyleID", strtoid(v), 0);
      n = pSheet->nRowCount*pSheet->nColCount;
      pSheet->Cells = realloc(pSheet->Cells, n*sizeof(CellStore));
      memset(pSheet->Cells, 0, n*sizeof(CellStore));
      pSheet->pCell = realloc(pSheet->pCell, n*sizeof(int));
      memset(pSheet->pCell, 0, n*sizeof(int));
      pSheet->nCells = 1;
      LoadColumns(pTableN, pSheet);
      LoadRows(pTableN, pSheet);
    }
  }
  
  return 0;
}
HRESULT LoadStyles(CMarkupNode* pStylesE, Workbook* pWorkbook)
{
  int i=0;
  LPCTSTR pstrClass;
  CMarkupNode node;
  CMarkupNode* pE = &node;
  pWorkbook->nStyles=0;
  node = GetChildNode(pStylesE, 0);
  for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    pstrClass = GetName(&node);
    if (0==strcmp(pstrClass, "Style")) {
      pWorkbook->nStyles++;
    }
  }
  pWorkbook->Styles = realloc(pWorkbook->Styles, pWorkbook->nStyles*sizeof(StyleDesc));
  memset(pWorkbook->Styles, 0, pWorkbook->nStyles*sizeof(StyleDesc));
  node = GetChildNode(pStylesE, 0);
  for ( i=0; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    pstrClass = GetName(&node);
    if (0==strcmp(pstrClass, "Style")) {
      StyleDesc* pStyle=pWorkbook->Styles+i++;
      SetAttribute(pStyle->nStyleID, "ss:ID", strtoid(v), 0);
      StyleE2StyleDesc(&node,pStyle);
    }
  }
  return 0;
}

HRESULT LoadWorkbook(Workbook* pWorkbook,LPCTSTR lpszFile)
{
  CMarkup xml = {0};
  CMarkupNode root;
  CMarkupNode node;
  CMarkupNode* pXmlDoc = &root;
  size_t nActiveSheet=0;
  int i=0;
  LPCTSTR pstrClass;
  CMarkupNode* pE = &node;
  memset(pWorkbook, 0, sizeof(Workbook));
  if (0==LoadFromFile(&xml, lpszFile, XMLFILE_ENCODING_UTF8)) {
    return 0;
  }
  root = GetRoot(&xml);
  pWorkbook->nSheets=0;
  node = GetChildNode(&root, 0);
  for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    pstrClass = GetName(&node);
    if (0==strcmp(pstrClass, "Worksheet")) {
      pWorkbook->nSheets++;
    }
  }
  pWorkbook->Sheets = (Worksheet*)realloc(pWorkbook->Sheets, pWorkbook->nSheets*sizeof(Worksheet));
  memset(pWorkbook->Sheets, 0, pWorkbook->nSheets*sizeof(Worksheet));
  node = GetChildNode(pXmlDoc, 0);
  for ( ; IsValidNode(&node); node = GetSiblingNode(&node) ) {
    pstrClass = GetName(&node);
#define GetChildValue(P, X, N, V, D)  {CMarkupNode node2, *pNode2 = GetChildNode2(P, N, &node2); LPCTSTR v = GetValue(pNode2); X = (v) ? V : D;}
    
    if (0==strcmp(pstrClass, "ExcelWorkbook")) {
      GetChildValue(&node, pWorkbook->nWindowHeight, "WindowHeight", atol(v), 0);
      GetChildValue(&node, pWorkbook->nWindowWidth, "WindowWidth", atol(v), 0);
      GetChildValue(&node, pWorkbook->nWindowTopX, "WindowTopX", atol(v), 0);
      GetChildValue(&node, pWorkbook->nWindowTopY, "WindowTopY", atol(v), 0);
      GetChildValue(&node, pWorkbook->bProtectStructure, "ProtectStructure", atobool(v), 0);
      GetChildValue(&node, pWorkbook->bProtectWindows, "ProtectWindows", atobool(v), 0);
    }
    if (0==strcmp(pstrClass, "DocumentProperties")) {}
    if (0==strcmp(pstrClass, "ActiveSheet")) {}
    if (0==strcmp(pstrClass, "Styles")) {
      LoadStyles(&node, pWorkbook);
    }
    if (0==strcmp(pstrClass, "Worksheet")) {
      Worksheet* pSheet=pWorkbook->Sheets+i++;
      SetAttribute(pSheet->nName, "ss:Name", strtoid(v), 0);
      LoadWorkSheet(&node,pSheet);
    }
  }
  Release(&xml);
  return true;
}
