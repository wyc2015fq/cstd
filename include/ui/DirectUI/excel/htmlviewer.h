enum TYPES
{ TEXT,
IMAGE,
BREAK,
COMPOUND
};
#include "HTMLStringVector.inl"
#include "HTMLStringTable.inl"

typedef struct CHTMLItem {
  int m_nType;
  SIZE m_scBounds;
  POINT m_pPos;
  int m_nFrames;
  int m_nFrameSize;
  COLORREF m_colFrame;
  char m_szFont[ 100 ];
  int m_nFontSize;
  int m_nFontBold;
  int m_nFontItalic;
  COLORREF m_colText;
  COLORREF m_colBkg;
  int m_nHFormat;
  int m_nVFormat;
  char *m_szText;
  int m_nBmp;
  int m_nBmpBkg;
  int m_nRow;
  int m_nCol;
  int m_nColSpan;
  int m_nRowSpan;
  int m_nWidth;
  int m_nTableWidth;
  int m_nTableFrameSize;
  int m_nHeight;
  int m_nTableHeight;
  int m_nCellPadding;
  int m_nCellSpacing;
  int m_nLink;
  HANDLE m_hEvent;
  int m_nMinColumnWidth;
  int m_nAMinColumnWidth;
  int m_nMaxColumnWidth;
  int m_nColumnWidth;
  BOOL m_bSelected;
  BOOL m_bNoBreak;
  int m_nControlID;
  int m_nAbsoluteAlign;
  int m_bClear;
  struct CHTMLItem **m_ppSubItems;
  int m_nSubItems;
} CHTMLItem;

typedef struct _CHTMLProgress_vtable CHTMLProgress_vtable;

typedef struct _CHTMLProgress {
  CHTMLProgress_vtable* vtable;
} CHTMLProgress;

typedef struct _CHTMLProgress_vtable {
  void (*Reading)( CHTMLProgress* me, BOOL bImage, int nPos, int nSize );
  void (*LinkClicked)( CHTMLProgress* me, char *szLink );
  void (*SetCursor)( CHTMLProgress* me, BOOL bLink, char *szLink );
  void (*SetTitle)( CHTMLProgress* me, char *szTitle );
  char* (*ReadHTTPData)( CHTMLProgress* me, char *szUrl, int *pnSize );
  void (*ImageLoaded)( CHTMLProgress* me, int nDone, int nCount );
  // Control functions, return -1 if no control support.
  int (*CreateForm)( CHTMLProgress* me, char *szAction, char *szMethod );
  int (*CreateTextBox)( CHTMLProgress* me, int nSize, int nMaxChar, SIZE scSize, char *szText, BOOL bMultiline, char *szName, int nForm, BOOL bPassword );
  int (*CreateButton)( CHTMLProgress* me, SIZE scSize, char *szText, char *szName, int nForm );
  int (*CreateRadio)( CHTMLProgress* me, SIZE scSize, char *szValue, char *szName, int nForm );
  int (*CreateCheckbox)( CHTMLProgress* me, SIZE scSize, char *szValue, char *szName, int nForm );
  int (*CreateListbox)( CHTMLProgress* me, SIZE scSize, CHTMLStringTable* stOptions, int nSelected, char *szName, int nForm );
  void (*MoveControl)( CHTMLProgress* me, int nControlID, POINT pPos );
  void (*UpdateScreen)( CHTMLProgress* me, RECT* r );
} CHTMLProgress_vtable;

typedef struct _CHTMLBitmap CHTMLBitmap;
typedef struct _CHTMLBitmaps CHTMLBitmaps;

#define GetEmptyBitmap(me)  (me)->m_bmEmpty
typedef struct _CHTMLBitmap {
  BOOL m_bEmptyBitmap;
  HBITMAP m_bmBmp;
  int m_nCounter;
  char *m_szImageName;
  char *m_szImageData;
  int m_nImageData;
  HANDLE m_hDone;
  SIZE m_Size;
  CHTMLProgress *m_pProgress;
  CHTMLBitmaps *m_pParent;
  BOOL m_bLoaded;
  BOOL m_bBackground;
} CHTMLBitmap;

typedef struct _CHTMLBitmaps {
  CHTMLBitmap **m_ppBitmaps;
  int m_nBitmaps;
  CHTMLProgress *m_pProgress;
  //CHTMLViewer *m_pViewer;
  HBITMAP m_bmEmpty;
  long m_nDownloads;
  HANDLE m_hMutex;
  BOOL m_bStop;
} CHTMLBitmaps;

typedef struct CHTMLViewer {
  int m_nItemCount;
  int m_nLevelCount;
  CHTMLItem **m_ppItems;
  int m_nItems;
  CHTMLProgress *m_pProgress;
  CHTMLBitmaps* m_Images;
  CHTMLStringTable* m_Links;
  COLORREF m_bgCol;
  int m_nBmpBkg;
  int m_nActiveLink;
  int m_nListLevel;
  int m_nCurrForm;
  BOOL m_bReading;
  BOOL m_bWaitForImages;
}CHTMLViewer;

#include <olectl.h>
#ifdef __cplusplus
#define OLELOADPICTURE(pstm, dwFileSize, FALSE, IID_IPicture, gpPicture) OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&gpPicture)
#define OLECALL0(obj, fun) obj->fun()
#define OLECALL1(obj, fun, a1) obj->fun(a1)
#else
#define OLELOADPICTURE(pstm, dwFileSize, FALSE, IID_IPicture, gpPicture) OleLoadPicture(pstm, dwFileSize, FALSE, &(IID_IPicture), (LPVOID *)&gpPicture)
#define OLECALL0(obj, fun) obj->lpVtbl->fun(obj)
#define OLECALL1(obj, fun, a1) obj->lpVtbl->fun(obj, a1)
#endif 
//提取图片文件 系统支持的所有图片类型
//szFilename  --- 目的文件名
//pBitmap --- 返回的HBITMAP对象
//无返回值
HBITMAP LoadPicture( LPCTSTR szFilename ) {
  HANDLE hFile;
  DWORD dwFileSize;
  DWORD dwBytesRead = 0;
  BOOL bRead;
  HGLOBAL hGlobal;
  LPVOID pvData = NULL;
  LPPICTURE gpPicture;
  LPSTREAM pstm = NULL;
  HRESULT hr;
  OLE_HANDLE m_picHandle;
  ASSERT( szFilename );
  hFile = CreateFile( szFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL );
  //_ASSERTE(INVALID_HANDLE_VALUE != hFile);
  if ( INVALID_HANDLE_VALUE == hFile )                                                      //无效文件句柄
    return 0;
  // get file size
  dwFileSize = GetFileSize( hFile, NULL );
  _ASSERTE( -1 != dwFileSize );
  // alloc memory based on file size
  hGlobal = GlobalAlloc( GMEM_MOVEABLE, dwFileSize );
  _ASSERTE( NULL != hGlobal );
  pvData = GlobalLock( hGlobal );
  _ASSERTE( NULL != pvData );
  // read file and store in global memory
  bRead = ReadFile( hFile, pvData, dwFileSize, &dwBytesRead, NULL );
  _ASSERTE( FALSE != bRead );
  GlobalUnlock( hGlobal );
  CloseHandle( hFile );
  // create IStream* from global memory
  hr = CreateStreamOnHGlobal( hGlobal, TRUE, &pstm );
  _ASSERTE( SUCCEEDED( hr ) && pstm );
  // Create IPicture from image file
  hr = OLELOADPICTURE( pstm, dwFileSize, FALSE, IID_IPicture, gpPicture );
  _ASSERTE( SUCCEEDED( hr ) && gpPicture );
  OLECALL0( pstm, Release );
  OLECALL1( gpPicture, get_Handle, &m_picHandle );
  return ( HBITMAP ) m_picHandle;
}
#include "HTMLBitmap.inl"
#include "HTMLBitmaps.inl"
#include "HTMLItem.inl"
#if 0
CHTMLStringVector();
~CHTMLStringVector();
void Clean();
int AddString( char *szString, int nSize = -1 );
int SetString( int nIndex, char *szString, int nSize = -1 );
char *GetString( int nIndex, int *pnSize = NULL );
int GetCount();
int SetCount( int nCount );
char *operator [] ( int nIndex );
CHTMLStringVector &operator =( CHTMLStringVector &s );
int MakeIndex( char *szString, int nLenght );
int GetIndex( int nIndex );
void DeleteString( int nIndex );
int MoveString( int nFrom, int nTo );

CHTMLStringTable();
~CHTMLStringTable();
int AddRow( char **pszRow, int nRow );
int AddRow();
int InsertRow( int nRow );
int AddString( int nRow, char *szString, int nString = -1 );
int DeleteRow( int nRow );
int SetString( int nRow, int nIndex, char *szString, int nString = -1 );
char* GetString( int nRow, int nIndex, int *pnSize = NULL );
int GetCount();
CHTMLStringVector &operator [] ( int nRow );
CHTMLStringTable &operator = ( CHTMLStringTable &st );
int MoveRow( int nFrom, int nTo );
void Reset();
int SplitString( char *szData, char *szFieldSep, char *szRowSep );

typedef struct CHTMLBitmaps;
typedef struct CHTMLViewer;

CHTMLItem();
~CHTMLItem();
static void GetStringSize( char *szFont, int nFontSize, int bBold, int bItalic, SIZE *sc, char *szString, int nMaxWidth, int nLen );
void Offset( int nX, int nY );
// Text-object
static CHTMLItem *NewText( char *szText, int nFrames, int nFrameSize, COLORREF colFrame, char *szFont, int nFontSize,
                          int nFontBold, int nFontItalic, COLORREF colText, COLORREF colBkg, int nHFormat, int nVFormat, int nLink, BOOL bNoBreak );
// Image-object
static CHTMLItem *NewImage( char *szImageName, int nFrames, int nFrameSize, COLORREF colFrame, int nHFormat, CHTMLBitmaps &Images, int nLink, BOOL bNoBreak, BOOL bBackground, int nBorder );
// Break-object
static CHTMLItem *NewBreak( int nHeight, int nWidth, int nHFormat, int bClear );
// Compound-object
static CHTMLItem *NewCompound( int nRow, int nCol, int nWidth, int nHFormat, int nVFormat, COLORREF colBkg, BOOL bBorder, int nColSpan, int nRowSpan, int nCellPadding, int nCellSpacing, int nTableWidth, int nBmpBkg, int nHeight, int nTableHeight, COLORREF colBorder, int nAbsoluteAlign );
BOOL WaitForImage( CHTMLBitmaps &Images, BOOL bWaitForImages );

  CHTMLBitmap( char *szImageName, CHTMLProgress *pProgress, CHTMLBitmaps *pParent, BOOL bBackground );
  ~CHTMLBitmap();
  void LoadBitmap();
  char *GetImageName();
  BOOL GetBackground();
  BOOL GetBitmap( BOOL bWaitForImage = FALSE );
  BOOL IsEmptyBitmap();
  BOOL IsLoaded();
  BOOL IsCreated();
  void DestroyBitmap();
  void GetSize( SIZE &sc );
  int Width();
  int Height();
  void BitBlt( HDC hDC, int x, int y, int wx = -1, int wy = -1 );
  void StretchBlt( HDC hDC, int x, int y, int wx, int wy );
  int TimerTick();
  
  CHTMLBitmaps( CHTMLProgress *pProgress );
  ~CHTMLBitmaps();
  void SetViewer( CHTMLViewer *pViewer );
  int GetCount();
  int AddBitmap( char *szImageName, BOOL bBackground );
  CHTMLBitmap &operator[] ( int nIndex );
  void ImageLoaded();
  HBITMAP GetEmptyBitmap();
  void WaitForDownload();
  void ThTimer();
  void Stop();

  void AddItem( CHTMLItem **&ppItems, int &nItems );
  void AddHTML( char *szHTMLData, CHTMLItem **&ppItems, int &nItems, char *szFont, int nFontSize, COLORREF colText, char *szBaseUrl, int nLevel, int nHFormat, BOOL bNoBreak );
  void CalcPositions( RECT &rPos, CHTMLItem **&ppItems, int &nItems );
  void Draw( HDC hDC, RECT &r, CHTMLItem **&ppItems, int &nItems, int nX, int nY );
  int OnMouseOver( CHTMLItem **&ppItems, int &nItems, int nX, int nY, BOOL bClick );
  void GetProperties( char *szData, CHTMLStringTable &st );
  COLORREF GetCol( char *szData );
  void AdjustHR( int nWidth, CHTMLItem **&ppItems, int &nItems );
  void Select( RECT &r, CHTMLItem **&ppItems, int &nItems );
  void CopySelected( CHTMLItem **&ppItems, int &nItems, char *&szCopy, int &nLen );
  void UpdateImage( CHTMLItem **&ppItems, int &nItems, int nImage );
  void UpdateLink( CHTMLItem **&ppItems, int &nItems, int nLink );

  CHTMLViewer( CHTMLProgress *pProgress, BOOL bWaitForImages = FALSE );
  ~CHTMLViewer();
  void SetHTML( char *szHTMLData, char *szFont, int nFontSize, COLORREF colText, char *szBaseUrl );
  void CalcPositions( RECT &rPos );
  void Draw( HDC hDC, RECT &r, int nX, int nY );
  void OnMouseOver( int x, int y );
  void OnMouseClick( int x, int y );
  void Statistics( int &nItems, int &nImages, int &nLinks, int &nLevel );
  void Select( RECT &r );
  void CopySelected();
  void AddBaseToUrl( char *szUrl, char *szFullUrl, char *szBase );
  void UpdateImage( int nImage );
  void UpdateLink( char *szLink );
  BOOL IsReading();

#endif


  
#define ABSOLUTE_ALIGN

  typedef struct HTMLTAGS {
    char szName[ 25 ];
    int bEndRequired;
    int nParent;
  }HTMLTAGS;
  char *pszTags[] = //
  {"<br",     //  0
  "<i",     //  1
  "</i",     //  2
  "<b",     //  3
  "</b",     //  4
  "<strong",    //  5
  "</strong",    //  6
  "<u",     //  7
  "</u",     //  8
  "<a",     //  9
  "</a",     // 10
  "<small",    // 11
  "</small",    // 12
  "<big",     // 13
  "</big",    // 14
  "<center",    // 15
  "</center",    // 16
  "<p",     // 17
  "<font",    // 18
  "</font",    // 19
  "<img",     // 20
  "<h1",     // 21
  "</h1",     // 22
  "<div",     // 23
  "</div",    // 24
  "<hr",     // 25
  "<table",    // 26
  "</table",    // 27
  "<tr",     // 28
  "</tr",     // 29
  "<td",     // 30
  "</td",     // 31
  "<h2",     // 32
  "</h2",     // 33
  "<h3",     // 34
  "</h3",     // 35
  "<pre",     // 36
  "</pre",    // 37
  "<input",    // 38
  "<ul",     // 39
  "<ol",     // 40
  "<li",     // 41
  "</ul",     // 42
  "</ol",     // 43
  "<sub",     // 44
  "</sub",    // 45
  "<sup",     // 46
  "</sup",    // 47
  "<strike",    // 48
  "</strike",    // 49
  "<select",    // 50
  "</select",    // 51
  "<option",    // 52
  "<textarea",   // 53
  "</textarea",   // 54
  "<th",     // 55
  "</th",     // 56
  "<form",    // 57
  "</form",    // 58
  "<blockquote",   // 59
  "</blockquote",   // 60
  "</p",           // 61
  NULL
  };
