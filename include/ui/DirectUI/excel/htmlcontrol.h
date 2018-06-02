#include <windows.h>
#include <wininet.h>
typedef struct _CHTMLControl {
  CHTMLProgress_vtable* vtable;
  char m_szCurrUrl[ 2048 ];
  char m_szCurrStatus[ 128 ];
  CHTMLStringTable m_History;
  CHTMLViewer *m_pViewer;
  HBITMAP m_bmBmp;
  POINT m_pSelectStart;
  POINT m_pSelectStop;
  //CHTMLStringTable* m_Controls;
  //CHTMLStringTable* m_Forms;
  char *m_szActiveUrl;
  BOOL m_bMainRequest;
  int m_nFontSize;
  char m_szFontName[ 50 ];
} CHTMLControl;
#if 0
CHTMLControl( HWND hWnd, HWND hwParent, HWND hwStatus, HWND hwLink, HWND hwHistory );
~CHTMLControl();
void Reading( BOOL bImage, int nPos, int nSize );
void LinkClicked( char *szLink );
void SetCursor( BOOL bLink, char *szLink );
void SetTitle( char *szTitle );
char *ReadHTTPData( char *szUrl, int *pnSize );
void ImageLoaded( int nDone, int nCount );
char *PostHTTPData( char *szUrl, char *szPostData, int *pnSize );
int CreateForm( char *szAction, char *szMethod );
int CreateTextBox( int nSize, int nMaxChar, SIZE scSize, char *szText, BOOL bMultiline, char *szName, int nForm, BOOL bPassword );
int CreateButton( SIZE scSize, char *szText, char *szName, int nForm );
int CreateRadio( SIZE scSize, char *szValue, char *szName, int nForm );
int CreateCheckbox( SIZE scSize, char *szValue, char *szName, int nForm );
int CreateListbox( SIZE scSize, CHTMLStringTable &stOptions, int nSelected, char *szName, int nForm );
void MoveControl( int nControlID, POINT pPos );
void UpdateScreen( RECT &r );
void RemoveControls();
void SetHTML( char *szHTMLData, char *szBaseUrl );
int ProcessMessage( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
int EnableHistory();
void GoBack();
void Copy();
void CalcPos();
void Submit( int nForm, HWND hWnd = NULL, int nControlID = 0 );
char *UrlEncode( char *szStr );
void Tick( int nPos, int nSize );
#endif
