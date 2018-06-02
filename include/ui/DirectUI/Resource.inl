
typedef union STRINGorID{
  LPCTSTR s;
  int i;
  struct {WORD h, l;};
}STRINGorID;
#define IsSTRING(strorid) (HIWORD( strorid.s ) != 0)


#define STATICMENBERDEF(_T, _N, _V) static _T* get##_N() {static _T _N=_V; return &_N;}
#define m_hLangInst *get_hLangInst()
#define m_hInstance *get_hInstance()
STATICMENBERDEF( HINSTANCE, _hLangInst, NULL );
STATICMENBERDEF( HINSTANCE, _hInstance, NULL );
static HINSTANCE GetResourceInstance() {
  return m_hInstance;
}
static HINSTANCE GetModuleInstance() {
  return m_hInstance;
}
static HINSTANCE GetLanguageInstance() {
  return m_hLangInst;
}

#include "UILoadImage.inl"
#include "stb_image.inl"
HANDLE LoadImageT( STRINGorID lpszName, UINT uType, int cxDesired, int cyDesired ) {
  HINSTANCE hinst = GetResourceInstance();
  UINT fuLoad = IsSTRING(lpszName) ? LR_LOADFROMFILE : 0;
  return LoadImage( hinst, lpszName.s, uType, cxDesired, cyDesired, fuLoad );
}
HBITMAP LoadBitmapT( STRINGorID lpszName, int nCxDesired, int nCyDesired ) {
  return ( HBITMAP ) LoadImageT( lpszName, IMAGE_BITMAP, nCxDesired, nCyDesired );
}
HICON LoadIconT( STRINGorID lpszName, int nCxDesired, int nCyDesired ) {
  return ( HICON ) LoadImageT( lpszName, IMAGE_ICON, nCxDesired, nCyDesired );
}
HCURSOR LoadCursorT( STRINGorID lpszName ) {
  return ( HCURSOR ) LoadImageT( lpszName, IMAGE_CURSOR, 0, 0 );
}
HIMAGELIST LoadImageListT( STRINGorID lpszName, int cx, int cGrow, COLORREF crMask, UINT uType, UINT uFlags ) {
  HINSTANCE hinst = GetResourceInstance();
  if ( IsSTRING(lpszName) ) {
    uFlags |= LR_LOADFROMFILE;
  } else {
    uFlags &= ~LR_LOADFROMFILE;
  }
  return ImageList_LoadImage( hinst, lpszName.s, cx, cGrow, crMask, uType, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
}
int LoadStringT( UINT nID, LPTSTR lpBuffer, int nBufferMax ) {
  HINSTANCE hinst = GetResourceInstance();
  return LoadString( hinst, nID, lpBuffer, nBufferMax );
}


TImageInfo* UIResource_GetImage( UIResource* res, LPCTSTR bitmap ) {
  TImageInfo * data = (TImageInfo*)( StringPtrMap_Find( res->m_mImageHash, bitmap ) );
  return data;
}
TImageInfo* UIResource_AddImage( UIResource* res, LPCTSTR bitmap, LPCTSTR type, DWORD mask ) {
  TImageInfo * data = NULL;
  STRINGorID sbitmap;
  STRINGorID stype;
  sbitmap.s = bitmap;
  stype.s = type;
  if ( type != NULL ) {
    if ( isdigit( *bitmap ) ) {
      LPTSTR pstr = NULL;
      sbitmap.i = _tcstol( bitmap, &pstr, 10 );
      data = UILoadImage( sbitmap, stype, mask );
    }
  } else {
    data = UILoadImage( sbitmap, stype, mask );
  }
  if ( !data )
    return NULL;
  if ( !StringPtrMap_Insert( res->m_mImageHash, bitmap, data ) ) {
    DeleteObject( data->hBitmap );
    free(data);
  }
  return data;
}
TImageInfo* UIResource_GetImageEx( UIResource* res, LPCTSTR bitmap, LPCTSTR type, DWORD mask ) {
  TImageInfo* data = (TImageInfo*)( StringPtrMap_Find( res->m_mImageHash, bitmap ) );
  if ( !data ) {
    if ( UIResource_AddImage( res, bitmap, type, mask ) ) {
      data = (TImageInfo*)( StringPtrMap_Find( res->m_mImageHash, bitmap ) );
    }
  }
  return data;
}

bool UIResource_RemoveImage( UIResource* res, LPCTSTR bitmap ) {
  TImageInfo * data = UIResource_GetImage( res, bitmap );
  if ( !data )
    return false;
  DeleteObject( data->hBitmap );
  free(data);
  return StringPtrMap_Remove( res->m_mImageHash, bitmap );
}
void UIResource_RemoveAllImages(UIResource* res) {
  TImageInfo * data;
  int i;
  LPCTSTR key;
  for ( i = 0; i < StringPtrMap_GetSize(res->m_mImageHash); i++ ) {
    if ( key = StringPtrMap_GetAt( res->m_mImageHash, i ) ) {
      data = (TImageInfo*)( StringPtrMap_Find( res->m_mImageHash, key ) );
      DeleteObject( data->hBitmap );
      free(data);
    }
  }
  StringPtrMap_Resize(res->m_mImageHash, 0);
}
bool UIResource_AddFont( UIResource* res, HFONT hFont ) {
  TFontInfo* pFontInfo;
  if ( hFont == NULL )
    return false;
  pFontInfo = NEW(TFontInfo, 1);
  if ( !pFontInfo )
    return false;
  ZeroMemory( pFontInfo, sizeof( TFontInfo ) );
  pFontInfo->hFont = hFont;
  if ( res->m_hDcPaint ) {
    HFONT hOldFont = ( HFONT ) SelectObject( res->m_hDcPaint, hFont );
    GetTextMetrics( res->m_hDcPaint, &pFontInfo->tm );
    SelectObject( res->m_hDcPaint, hOldFont );
  }
  if ( !PtrArray_Add(res->m_aCustomFonts, pFontInfo ) ) {
    DEL(pFontInfo);
    return false;
  }
  return true;
}
HFONT UIResource_AddFontByName( UIResource* res, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic ) {
  LOGFONT lf = { 0 };
  HFONT hFont;
  GetObject( GetStockObject( DEFAULT_GUI_FONT ), sizeof( LOGFONT ), &lf );
  _tcscpy( lf.lfFaceName, pStrFontName );
  lf.lfHeight = -nSize;
  if ( bBold )
    lf.lfWeight += FW_BOLD;
  if ( bUnderline )
    lf.lfUnderline = TRUE;
  if ( bItalic )
    lf.lfItalic = TRUE;
  hFont = CreateFontIndirect( &lf );
  if ( hFont == NULL )
    return NULL;
  if ( !UIResource_AddFont( res, hFont ) ) {
    DeleteObject( hFont );
    return NULL;
  }
  return hFont;
}
bool UIResource_AddFontAt( UIResource* res, int index, HFONT hFont ) {
  TFontInfo* pFontInfo;
  if ( hFont == NULL )
    return false;
  pFontInfo = NEW(TFontInfo, 1);
  if ( !pFontInfo )
    return false;
  ZeroMemory( pFontInfo, sizeof( TFontInfo ) );
  pFontInfo->hFont = hFont;
  if ( res->m_hDcPaint ) {
    HFONT hOldFont = ( HFONT ) SelectObject( res->m_hDcPaint, hFont );
    GetTextMetrics( res->m_hDcPaint, &pFontInfo->tm );
    SelectObject( res->m_hDcPaint, hOldFont );
  }
  if ( !PtrArray_InsertAt(res->m_aCustomFonts, index, pFontInfo ) ) {
    DEL(pFontInfo);
    return false;
  }
  return true;
}
HFONT UIResource_AddFontAtByName( UIResource* res, int index, LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline ) {
  LOGFONT lf = { 0 };
  HFONT hFont;
  GetObject( GetStockObject( DEFAULT_GUI_FONT ), sizeof( LOGFONT ), &lf );
  _tcscpy( lf.lfFaceName, pStrFontName );
  lf.lfHeight = -nSize;
  if ( bBold )
    lf.lfWeight += FW_BOLD;
  if ( bUnderline )
    lf.lfUnderline = TRUE;
  hFont = CreateFontIndirect( &lf );
  if ( hFont == NULL )
    return NULL;
  if ( !UIResource_AddFontAt( res, index, hFont ) ) {
    DeleteObject( hFont );
    return NULL;
  }
  return hFont;
}
DWORD UIResource_GetDefaultDisabledColor(UIResource* res) {
  return res->m_dwDefalutDisabledColor;
}
void UIResource_SetDefaultDisabledColor( UIResource* res, DWORD dwColor ) {
  res->m_dwDefalutDisabledColor = dwColor;
}
HFONT UIResource_GetDefaultFont(UIResource* res) {
  return res->m_hDefalutFont;
}
DWORD UIResource_GetDefaultFontColor(UIResource* res) {
  return res->m_dwDefalutFontColor;
}
HFONT UIResource_GetFont( UIResource* res, int index ) {
  TFontInfo* pFontInfo;
  if ( index < 0 || index >= PtrArray_GetSize(res->m_aCustomFonts) )
    return UIResource_GetDefaultFont(res);
  pFontInfo = (TFontInfo*)( PtrArray_GetAt(res->m_aCustomFonts, index ) );
  return pFontInfo->hFont;
}
bool UIResource_FindFont( UIResource* res, HFONT hFont ) {
  int it;
  TFontInfo* pFontInfo = NULL;
  for ( it = 0; it < PtrArray_GetSize(res->m_aCustomFonts); it++ ) {
    pFontInfo = (TFontInfo*)( PtrArray_GetAt(res->m_aCustomFonts, it ) );
    if ( pFontInfo->hFont == hFont )
      return true;
  }
  return false;
}
bool UIResource_RemoveFont( UIResource* res, HFONT hFont ) {
  TFontInfo * pFontInfo = NULL;
  int it;
  for ( it = 0; it < PtrArray_GetSize(res->m_aCustomFonts); it++ ) {
    pFontInfo = (TFontInfo*)( PtrArray_GetAt(res->m_aCustomFonts, it ) );
    if ( pFontInfo->hFont == hFont ) {
      DeleteObject( pFontInfo->hFont );
      DEL(pFontInfo);
      return PtrArray_RemoveAt(res->m_aCustomFonts, it );
    }
  }
  return false;
}
bool UIResource_RemoveFontAt( UIResource* res, int index ) {
  TFontInfo* pFontInfo;
  if ( index < 0 || index >= PtrArray_GetSize(res->m_aCustomFonts) )
    return false;
  pFontInfo = (TFontInfo*)( PtrArray_GetAt(res->m_aCustomFonts, index ) );
  DeleteObject( pFontInfo->hFont );
  DEL(pFontInfo);
  return PtrArray_RemoveAt(res->m_aCustomFonts, index );
}
void UIResource_RemoveAllFonts(UIResource* res) {
  TFontInfo * pFontInfo;
  int it;
  for ( it = 0; it < PtrArray_GetSize(res->m_aCustomFonts); it++ ) {
    pFontInfo = (TFontInfo*)( PtrArray_GetAt(res->m_aCustomFonts, it ) );
    DeleteObject( pFontInfo->hFont );
    DEL(pFontInfo);
  }
  PtrArray_RemoveAll(res->m_aCustomFonts);
}
TEXTMETRIC UIResource_GetDefaultFontInfo(UIResource* res) {
  if ( res->m_hDefalutFontTextMetric.tmHeight == 0 ) {
    HFONT hOldFont = ( HFONT ) SelectObject( res->m_hDcPaint, res->m_hDefalutFont );
    GetTextMetrics( res->m_hDcPaint, &res->m_hDefalutFontTextMetric );
    SelectObject( res->m_hDcPaint, hOldFont );
  }
  return res->m_hDefalutFontTextMetric;
}
TEXTMETRIC UIResource_GetFontInfo( UIResource* res, int index ) {
  TFontInfo * pFontInfo;
  if ( index < 0 || index >= PtrArray_GetSize(res->m_aCustomFonts) )
    return UIResource_GetDefaultFontInfo(res);
  pFontInfo = (TFontInfo*)( PtrArray_GetAt(res->m_aCustomFonts, index ) );
  if ( pFontInfo->tm.tmHeight == 0 ) {
    HFONT hOldFont = ( HFONT ) SelectObject( res->m_hDcPaint, pFontInfo->hFont );
    GetTextMetrics( res->m_hDcPaint, &pFontInfo->tm );
    SelectObject( res->m_hDcPaint, hOldFont );
  }
  return pFontInfo->tm;
}

void UIResource_SetDefaultFont( UIResource* res, HFONT hFont, DWORD dwColor ) {
  if ( hFont == NULL )
    return ;
  if ( res->m_hDcPaint ) {
    HFONT hOldFont = ( HFONT ) SelectObject( res->m_hDcPaint, hFont );
    GetTextMetrics( res->m_hDcPaint, &res->m_hDefalutFontTextMetric );
    SelectObject( res->m_hDcPaint, hOldFont );
  } else {
    ZeroMemory( &res->m_hDefalutFontTextMetric, sizeof( res->m_hDefalutFontTextMetric ) );
  }
  if ( res->m_hDefalutFont && !UIResource_FindFont( res, res->m_hDefalutFont ) )
    DeleteObject( res->m_hDefalutFont );
  res->m_hDefalutFont = hFont;
  res->m_dwDefalutFontColor = dwColor;
}
HFONT UIResource_GetDefaultBoldFont(UIResource* res) {
  return res->m_hDefalutBoldFont;
}
DWORD UIResource_GetDefaultBoldFontColor(UIResource* res) {
  return res->m_dwDefalutBoldFontColor;
}
TEXTMETRIC UIResource_GetDefaultBoldFontInfo(UIResource* res) {
  if ( res->m_hDefaluttBoldFontTextMetric.tmHeight == 0 ) {
    HFONT hOldFont = ( HFONT ) SelectObject( res->m_hDcPaint, res->m_hDefalutBoldFont );
    GetTextMetrics( res->m_hDcPaint, &res->m_hDefaluttBoldFontTextMetric );
    SelectObject( res->m_hDcPaint, hOldFont );
  }
  return res->m_hDefaluttBoldFontTextMetric;
}
void UIResource_SetDefaultBoldFont( UIResource* res, HFONT hFont, DWORD dwColor ) {
  if ( hFont == NULL )
    return ;
  if ( res->m_hDcPaint ) {
    HFONT hOldFont = ( HFONT ) SelectObject( res->m_hDcPaint, hFont );
    GetTextMetrics( res->m_hDcPaint, &res->m_hDefaluttBoldFontTextMetric );
    SelectObject( res->m_hDcPaint, hOldFont );
  } else {
    ZeroMemory( &res->m_hDefaluttBoldFontTextMetric, sizeof( res->m_hDefaluttBoldFontTextMetric ) );
  }
  if ( res->m_hDefalutBoldFont && !UIResource_FindFont( res, res->m_hDefalutBoldFont ) )
    DeleteObject( res->m_hDefalutBoldFont );
  res->m_hDefalutBoldFont = hFont;
  res->m_dwDefalutBoldFontColor = dwColor;
}
HFONT UIResource_GetDefaultLinkFont(UIResource* res) {
  return res->m_hDefalutLinkFont;
}
DWORD UIResource_GetDefaultLinkFontColor(UIResource* res) {
  return res->m_dwDefalutLinkFontColor;
}
DWORD UIResource_GetDefaultLinkFontHoverColor(UIResource* res) {
  return res->m_dwDefalutLinkFontHoverColor;
}
TEXTMETRIC UIResource_GetDefaultLinkFontInfo(UIResource* res) {
  if ( res->m_hDefalutLinkFontTextMetric.tmHeight == 0 ) {
    HFONT hOldFont = ( HFONT ) SelectObject( res->m_hDcPaint, res->m_hDefalutLinkFont );
    GetTextMetrics( res->m_hDcPaint, &res->m_hDefalutLinkFontTextMetric );
    SelectObject( res->m_hDcPaint, hOldFont );
  }
  return res->m_hDefalutLinkFontTextMetric;
}
void UIResource_SetDefaultLinkFont( UIResource* res, HFONT hFont, DWORD dwColor, DWORD dwHoverColor ) {
  if ( hFont == NULL )
    return ;
  if ( res->m_hDcPaint ) {
    HFONT hOldFont = ( HFONT ) SelectObject( res->m_hDcPaint, hFont );
    GetTextMetrics( res->m_hDcPaint, &res->m_hDefalutLinkFontTextMetric );
    SelectObject( res->m_hDcPaint, hOldFont );
  } else {
    ZeroMemory( &res->m_hDefalutLinkFontTextMetric, sizeof( res->m_hDefalutLinkFontTextMetric ) );
  }
  if ( res->m_hDefalutLinkFont && !UIResource_FindFont( res, res->m_hDefalutLinkFont ) )
    DeleteObject( res->m_hDefalutLinkFont );
  res->m_hDefalutLinkFont = hFont;
  res->m_dwDefalutLinkFontColor = dwColor;
  res->m_dwDefalutLinkFontHoverColor = dwHoverColor;
}
void UIResource_AddDefaultAttributeList( UIResource* res, LPCTSTR pStrControlName, LPCTSTR pStrControlAttrList ) {
  StringPtrMap_Set( res->m_DefaultAttrHash, pStrControlName, ( LPVOID ) strdup(pStrControlAttrList) );
}
LPCTSTR UIResource_GetDefaultAttributeList( UIResource* res, LPCTSTR pStrControlName ) {
  LPCTSTR pDefaultAttr = (LPCTSTR)( StringPtrMap_Find( res->m_DefaultAttrHash, pStrControlName ) );
  if ( pDefaultAttr )
    return pDefaultAttr;
  else
    return NULL;
}
bool UIResource_RemoveDefaultAttributeList( UIResource* res, LPCTSTR pStrControlName ) {
  LPTSTR pDefaultAttr = (LPTSTR)( StringPtrMap_Find( res->m_DefaultAttrHash, pStrControlName ) );
  if ( !pDefaultAttr )
    return false;
  DEL(pDefaultAttr);
  return StringPtrMap_Remove( res->m_DefaultAttrHash, pStrControlName );
}
void UIResource_RemoveAllDefaultAttributeList(UIResource* res) {
  LPTSTR pDefaultAttr;
  LPCTSTR key;
  int i;
  for ( i = 0; i < StringPtrMap_GetSize(res->m_DefaultAttrHash); i++ ) {
    if ( key = StringPtrMap_GetAt( res->m_DefaultAttrHash, i ) ) {
      pDefaultAttr = (LPTSTR)( StringPtrMap_Find( res->m_DefaultAttrHash, key ) );
      DEL(pDefaultAttr);
    }
  }
  StringPtrMap_Resize(res->m_DefaultAttrHash, 0);
}
void UIResource_Release(UIResource* res) {
  //int i;
  // Delete the control-tree structures
  //for ( i = 0; i < res->m_aDelayedCleanup.GetSize(); i++ )
  //  delete static_cast<CControlUI*>( res->m_aDelayedCleanup[ i ] );
  //res->m_mNameHash.Resize( 0 );
  ReleaseDC(0, res->m_hDcPaint);
  DeleteObject( res->m_hDefalutFont );
  DeleteObject( res->m_hDefalutBoldFont );
  DeleteObject( res->m_hDefalutLinkFont );
  UIResource_RemoveAllFonts(res);
  UIResource_RemoveAllImages(res);
  UIResource_RemoveAllDefaultAttributeList(res);
  PtrArray_Release(res->m_aCustomFonts);
  StringPtrMap_Release(res->m_mImageHash);
  StringPtrMap_Release(res->m_DefaultAttrHash);
  free(res);
}
HPEN m_hUpdateRectPen = NULL;
UIResource* CreateUIResource() {
  LOGFONT lf = { 0 };
  UIResource* res = NEW(UIResource, 1);
  res->m_aCustomFonts = CreatePtrArray(0);
  res->m_mImageHash = CreateStringPtrMap(0);
  res->m_DefaultAttrHash = CreateStringPtrMap(0);
  res->m_dwDefalutDisabledColor = 0xFFA7A6AA;
  GetObject( GetStockObject( DEFAULT_GUI_FONT ), sizeof( LOGFONT ), &lf );
  lf.lfHeight = -12;
  res->m_hDefalutFont = CreateFontIndirect( &lf );
  res->m_dwDefalutFontColor = 0xFF000000;
  ZeroMemory( &res->m_hDefalutFontTextMetric, sizeof( res->m_hDefalutFontTextMetric ) );
  lf.lfUnderline = TRUE;
  res->m_hDefalutLinkFont = CreateFontIndirect( &lf );
  res->m_dwDefalutLinkFontColor = 0xFF0000FF;
  res->m_dwDefalutLinkFontHoverColor = 0xFFD3215F;
  ZeroMemory( &res->m_hDefaluttBoldFontTextMetric, sizeof( res->m_hDefaluttBoldFontTextMetric ) );
  lf.lfUnderline = FALSE;
  lf.lfWeight += FW_BOLD;
  res->m_hDefalutBoldFont = CreateFontIndirect( &lf );
  res->m_dwDefalutBoldFontColor = 0xFF000000;
  ZeroMemory( &res->m_hDefalutLinkFontTextMetric, sizeof( res->m_hDefalutLinkFontTextMetric ) );
  if ( m_hUpdateRectPen == NULL ) {
    m_hUpdateRectPen = CreatePen( PS_SOLID, 1, RGB( 220, 0, 0 ) );
    // Boot Windows Common Controls (for the ToolTip control)
    InitCommonControls();
    LoadLibrary( _T( "msimg32.dll" ) );
  }
  res->m_hDcPaint = GetDC(0);
  return res;
}

