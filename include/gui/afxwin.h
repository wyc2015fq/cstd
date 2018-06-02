#ifndef _AFXWIN_H_
#define _AFXWIN_H_
/////////////////////////////////////////////////////////////////////////////
// Classes declared in this file
class CSize;
class CPoint;
class CRect;
//CObject
//CException
//CSimpleException
class CResourceException;// Win resource failure exception
class CUserException;    // Message Box alert and stop operation
class CGdiObject;            // CDC drawing tool
class CPen;              // a pen / HPEN wrapper
class CBrush;            // a brush / HBRUSH wrapper
class CFont;             // a font / HFONT wrapper
class CBitmap;           // a bitmap / HBITMAP wrapper
class CPalette;          // a palette / HPALLETE wrapper
class CRgn;              // a region / HRGN wrapper
class CDC;                   // a Display Context / HDC wrapper
class CClientDC;         // CDC for client of window
class CWindowDC;         // CDC for entire window
class CPaintDC;          // embeddable BeginPaint struct helper
class CMenu;                 // a menu / HMENU wrapper
class CCmdTarget;            // a target for user commands
class CWnd;                 // a window / HWID wrapper
class CDialog;          // a dialog
// standard windows controls
class CStatic;          // Static control
class CButton;          // Button control
class CListBox;         // ListBox control
class CCheckListBox;// special listbox with checks
class CComboBox;        // ComboBox control
class CEdit;            // Edit control
class CScrollBar;       // ScrollBar control
// frame windows
class CFrameWnd;        // standard SDI frame
class CMDIFrameWnd; // standard MDI frame
class CMDIChildWnd; // standard MDI child
class CMiniFrameWnd;// half-height caption frame wnd
// views on a document
class CView;            // a view on a document
class CScrollView;  // a scrolling view
class CWinThread;           // thread base class
class CWinApp;          // application base class
class CDocTemplate;         // template for document creation
class CSingleDocTemplate;// SDI support
class CMultiDocTemplate; // MDI support
class CDocument;            // main document abstraction
// Helper classes
class CCmdUI;           // Menu/button enabling
class CDataExchange;    // Data exchange and validation context
class CCommandLineInfo; // CommandLine parsing helper
class CDocManager;      // CDocTemplate manager object

//TOOLINFO
class CToolInfo;

//CObject
class CImageList;
//CCmdTarget;
//CWnd
// class CListBox;
class CDragListBox;
class CListCtrl;
class CTreeCtrl;
class CSpinButtonCtrl;
class CHeaderCtrl;
class CSliderCtrl;
class CProgressCtrl;
// class CComboBox;
class CComboBoxEx;
class CHotKeyCtrl;
class CToolTipCtrl;
class CTabCtrl;
class CAnimateCtrl;
class CToolBarCtrl;
class CStatusBarCtrl;
class CRichEditCtrl;
class CIPAddressCtrl;

/////////////////////////////////////////////////////////////////////////////
// Type modifier for message handlers
#ifndef afx_msg
#define afx_msg         // intentional placeholder
#endif
/////////////////////////////////////////////////////////////////////////////
// CSize - An extent, similar to Windows SIZE structure.
class CSize : public tagSIZE
{
public:
  // Constructors
  // construct an uninitialized size
  CSize();
  // create from two integers
  CSize(int initCX, int initCY);
  // create from another size
  CSize(SIZE initSize);
  // create from a point
  CSize(POINT initPt);
  // create from a DWORD: cx = LOWORD(dw) cy = HIWORD(dw)
  CSize(DWORD dwSize);
  // Operations
  BOOL operator==(SIZE size) const;
  BOOL operator!=(SIZE size) const;
  void operator+=(SIZE size);
  void operator-=(SIZE size);
  // Operators returning CSize values
  CSize operator+(SIZE size) const;
  CSize operator-(SIZE size) const;
  CSize operator-() const;
  // Operators returning CPoint values
  CPoint operator+(POINT point) const;
  CPoint operator-(POINT point) const;
  // Operators returning CRect values
  CRect operator+(const RECT* lpRect) const;
  CRect operator-(const RECT* lpRect) const;
};
/////////////////////////////////////////////////////////////////////////////
// CPoint - A 2-D point, similar to Windows POINT structure.
class CPoint : public tagPOINT
{
public:
  // Constructors
  // create an uninitialized point
  CPoint();
  // create from two integers
  CPoint(int initX, int initY);
  // create from another point
  CPoint(POINT initPt);
  // create from a size
  CPoint(SIZE initSize);
  // create from a dword: x = LOWORD(dw) y = HIWORD(dw)
  CPoint(DWORD dwPoint);
  // Operations
  // translate the point
  void Offset(int xOffset, int yOffset);
  void Offset(POINT point);
  void Offset(SIZE size);
  BOOL operator==(POINT point) const;
  BOOL operator!=(POINT point) const;
  void operator+=(SIZE size);
  void operator-=(SIZE size);
  void operator+=(POINT point);
  void operator-=(POINT point);
  // Operators returning CPoint values
  CPoint operator+(SIZE size) const;
  CPoint operator-(SIZE size) const;
  CPoint operator-() const;
  CPoint operator+(POINT point) const;
  // Operators returning CSize values
  CSize operator-(POINT point) const;
  // Operators returning CRect values
  CRect operator+(const RECT* lpRect) const;
  CRect operator-(const RECT* lpRect) const;
};
/////////////////////////////////////////////////////////////////////////////
// CRect - A 2-D rectangle, similar to Windows RECT structure.
typedef const RECT* LPCRECT;    // pointer to read/only RECT
class CRect : public tagRECT
{
public:
  // Constructors
  // uninitialized rectangle
  CRect();
  // from left, top, right, and bottom
  CRect(int l, int t, int r, int b);
  // copy constructor
  CRect(const RECT& srcRect);
  // from a pointer to another rect
  CRect(LPCRECT lpSrcRect);
  // from a point and size
  CRect(POINT point, SIZE size);
  // from two points
  CRect(POINT topLeft, POINT bottomRight);
  // Attributes (in addition to RECT members)
  // retrieves the width
  int Width() const;
  // returns the height
  int Height() const;
  // returns the size
  CSize Size() const;
  // reference to the top-left point
  CPoint& TopLeft();
  // reference to the bottom-right point
  CPoint& BottomRight();
  // const reference to the top-left point
  const CPoint& TopLeft() const;
  // const reference to the bottom-right point
  const CPoint& BottomRight() const;
  // the geometric center point of the rectangle
  CPoint CenterPoint() const;
  // swap the left and right
  void SwapLeftRight();
  static void SwapLeftRight(LPRECT lpRect);
  // convert between CRect and LPRECT/LPCRECT (no need for &)
  operator LPRECT();
  operator LPCRECT() const;
  // returns TRUE if rectangle has no area
  BOOL IsRectEmpty() const;
  // returns TRUE if rectangle is at (0,0) and has no area
  BOOL IsRectNull() const;
  // returns TRUE if point is within rectangle
  BOOL PtInRect(POINT point) const;
  // Operations
  // set rectangle from left, top, right, and bottom
  void SetRect(int x1, int y1, int x2, int y2);
  void SetRect(POINT topLeft, POINT bottomRight);
  // empty the rectangle
  void SetRectEmpty();
  // copy from another rectangle
  void CopyRect(LPCRECT lpSrcRect);
  // TRUE if exactly the same as another rectangle
  BOOL EqualRect(LPCRECT lpRect) const;
  // inflate rectangle's width and height without
  // moving its top or left
  void InflateRect(int x, int y);
  void InflateRect(SIZE size);
  void InflateRect(LPCRECT lpRect);
  void InflateRect(int l, int t, int r, int b);
  // deflate the rectangle's width and height without
  // moving its top or left
  void DeflateRect(int x, int y);
  void DeflateRect(SIZE size);
  void DeflateRect(LPCRECT lpRect);
  void DeflateRect(int l, int t, int r, int b);
  // translate the rectangle by moving its top and left
  void OffsetRect(int x, int y);
  void OffsetRect(SIZE size);
  void OffsetRect(POINT point);
  void NormalizeRect();
  // set this rectangle to intersection of two others
  BOOL IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2);
  // set this rectangle to bounding union of two others
  BOOL UnionRect(LPCRECT lpRect1, LPCRECT lpRect2);
  // set this rectangle to minimum of two others
  BOOL SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2);
  // Additional Operations
  void operator=(const RECT& srcRect);
  BOOL operator==(const RECT& rect) const;
  BOOL operator!=(const RECT& rect) const;
  void operator+=(POINT point);
  void operator+=(SIZE size);
  void operator+=(LPCRECT lpRect);
  void operator-=(POINT point);
  void operator-=(SIZE size);
  void operator-=(LPCRECT lpRect);
  void operator&=(const RECT& rect);
  void operator|=(const RECT& rect);
  // Operators returning CRect values
  CRect operator+(POINT point) const;
  CRect operator-(POINT point) const;
  CRect operator+(LPCRECT lpRect) const;
  CRect operator+(SIZE size) const;
  CRect operator-(SIZE size) const;
  CRect operator-(LPCRECT lpRect) const;
  CRect operator&(const RECT& rect2) const;
  CRect operator|(const RECT& rect2) const;
  CRect MulDiv(int nMultiplier, int nDivisor) const;
};
#ifdef _DEBUG
// Diagnostic Output
CDumpContext& AFXAPI operator<<(CDumpContext& dc, SIZE size);
CDumpContext& AFXAPI operator<<(CDumpContext& dc, POINT point);
CDumpContext& AFXAPI operator<<(CDumpContext& dc, const RECT& rect);
#endif //_DEBUG
// Serialization
CArchive& AFXAPI operator<<(CArchive& ar, SIZE size);
CArchive& AFXAPI operator<<(CArchive& ar, POINT point);
CArchive& AFXAPI operator<<(CArchive& ar, const RECT& rect);
CArchive& AFXAPI operator>>(CArchive& ar, SIZE& size);
CArchive& AFXAPI operator>>(CArchive& ar, POINT& point);
CArchive& AFXAPI operator>>(CArchive& ar, RECT& rect);
/////////////////////////////////////////////////////////////////////////////
// Standard exceptions
class CResourceException : public CSimpleException    // resource failure
{
  static const AFX_DATA CRuntimeClass fasdasdf;
  DECLARE_DYNAMIC(CResourceException)
public:
  CResourceException();
  // Implementation
public:
  CResourceException(BOOL bAutoDelete);
  CResourceException(BOOL bAutoDelete, UINT nResourceID);
  virtual ~CResourceException();
};
class CUserException : public CSimpleException   // general user visible alert
{
  DECLARE_DYNAMIC(CUserException)
public:
  CUserException();
  // Implementation
public:
  CUserException(BOOL bAutoDelete);
  CUserException(BOOL bAutoDelete, UINT nResourceID);
  virtual ~CUserException();
};
void AFXAPI AfxThrowResourceException();
void AFXAPI AfxThrowUserException();
/////////////////////////////////////////////////////////////////////////////
// CGdiObject abstract class for CDC SelectObject
class CGdiObject : public CObject
{
  DECLARE_DYNCREATE(CGdiObject)
public:
  // Attributes
  HGDIOBJ m_hObject;                  // must be first data member
  operator HGDIOBJ() const;
  HGDIOBJ GetSafeHandle() const;
  static CGdiObject* PASCAL FromHandle(HGDIOBJ hObject);
  static void PASCAL DeleteTempMap();
  BOOL Attach(HGDIOBJ hObject);
  HGDIOBJ Detach();
  // Constructors
  CGdiObject(); // must Create a derived class object
  BOOL DeleteObject();
  // Operations
  int GetObject(int nCount, LPVOID lpObject) const;
  UINT GetObjectType() const;
  BOOL CreateStockObject(int nIndex);
  BOOL UnrealizeObject();
  BOOL operator==(const CGdiObject& obj) const;
  BOOL operator!=(const CGdiObject& obj) const;
  // Implementation
public:
  virtual ~CGdiObject();
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
  virtual void AssertValid() const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CGdiObject subclasses (drawing tools)
class CPen : public CGdiObject
{
  DECLARE_DYNAMIC(CPen)
public:
  static CPen* PASCAL FromHandle(HPEN hPen);
  // Constructors
  CPen();
  CPen(int nPenStyle, int nWidth, COLORREF crColor);
  CPen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
      int nStyleCount = 0, const DWORD* lpStyle = NULL);
  BOOL CreatePen(int nPenStyle, int nWidth, COLORREF crColor);
  BOOL CreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush,
      int nStyleCount = 0, const DWORD* lpStyle = NULL);
  BOOL CreatePenIndirect(LPLOGPEN lpLogPen);
  // Attributes
  operator HPEN() const;
  int GetLogPen(LOGPEN* pLogPen);
  int GetExtLogPen(EXTLOGPEN* pLogPen);
  // Implementation
public:
  virtual ~CPen();
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
#endif
};
class CBrush : public CGdiObject
{
  DECLARE_DYNAMIC(CBrush)
public:
  static CBrush* PASCAL FromHandle(HBRUSH hBrush);
  // Constructors
  CBrush();
  CBrush(COLORREF crColor);             // CreateSolidBrush
  CBrush(int nIndex, COLORREF crColor); // CreateHatchBrush
  CBrush(CBitmap* pBitmap);          // CreatePatternBrush
  BOOL CreateSolidBrush(COLORREF crColor);
  BOOL CreateHatchBrush(int nIndex, COLORREF crColor);
  BOOL CreateBrushIndirect(const LOGBRUSH* lpLogBrush);
  BOOL CreatePatternBrush(CBitmap* pBitmap);
  BOOL CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);
  BOOL CreateDIBPatternBrush(const void* lpPackedDIB, UINT nUsage);
  BOOL CreateSysColorBrush(int nIndex);
  // Attributes
  operator HBRUSH() const;
  int GetLogBrush(LOGBRUSH* pLogBrush);
  // Implementation
public:
  virtual ~CBrush();
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
#endif
};
class CFont : public CGdiObject
{
  DECLARE_DYNAMIC(CFont)
public:
  static CFont* PASCAL FromHandle(HFONT hFont);
  // Constructors
  CFont();
  BOOL CreateFontIndirect(const LOGFONT* lpLogFont);
  BOOL CreateFont(int nHeight, int nWidth, int nEscapement,
      int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
      BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
      BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
      LPCTSTR lpszFacename);
  BOOL CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, CDC* pDC = NULL);
  BOOL CreatePointFontIndirect(const LOGFONT* lpLogFont, CDC* pDC = NULL);
  // Attributes
  operator HFONT() const;
  int GetLogFont(LOGFONT* pLogFont);
  // Implementation
public:
  virtual ~CFont();
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
#endif
};
class CBitmap : public CGdiObject
{
  DECLARE_DYNAMIC(CBitmap)
public:
  static CBitmap* PASCAL FromHandle(HBITMAP hBitmap);
  // Constructors
  CBitmap();
  BOOL LoadBitmap(LPCTSTR lpszResourceName);
  BOOL LoadBitmap(UINT nIDResource);
  BOOL LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_
  BOOL LoadMappedBitmap(UINT nIDBitmap, UINT nFlags = 0,
      LPCOLORMAP lpColorMap = NULL, int nMapSize = 0);
  BOOL CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitcount,
      const void* lpBits);
  BOOL CreateBitmapIndirect(LPBITMAP lpBitmap);
  BOOL CreateCompatibleBitmap(CDC* pDC, int nWidth, int nHeight);
  BOOL CreateDiscardableBitmap(CDC* pDC, int nWidth, int nHeight);
  // Attributes
  operator HBITMAP() const;
  int GetBitmap(BITMAP* pBitMap);
  // Operations
  DWORD SetBitmapBits(DWORD dwCount, const void* lpBits);
  DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const;
  CSize SetBitmapDimension(int nWidth, int nHeight);
  CSize GetBitmapDimension() const;
  // Implementation
public:
  virtual ~CBitmap();
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
#endif
};
class CPalette : public CGdiObject
{
  DECLARE_DYNAMIC(CPalette)
public:
  static CPalette* PASCAL FromHandle(HPALETTE hPalette);
  // Constructors
  CPalette();
  BOOL CreatePalette(LPLOGPALETTE lpLogPalette);
  BOOL CreateHalftonePalette(CDC* pDC);
  // Attributes
  operator HPALETTE() const;
  int GetEntryCount();
  UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors) const;
  UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors);
  // Operations
  void AnimatePalette(UINT nStartIndex, UINT nNumEntries,
      LPPALETTEENTRY lpPaletteColors);
  UINT GetNearestPaletteIndex(COLORREF crColor) const;
  BOOL ResizePalette(UINT nNumEntries);
  // Implementation
  virtual ~CPalette();
};
class CRgn : public CGdiObject
{
  DECLARE_DYNAMIC(CRgn)
public:
  static CRgn* PASCAL FromHandle(HRGN hRgn);
  operator HRGN() const;
  // Constructors
  CRgn();
  BOOL CreateRectRgn(int x1, int y1, int x2, int y2);
  BOOL CreateRectRgnIndirect(LPCRECT lpRect);
  BOOL CreateEllipticRgn(int x1, int y1, int x2, int y2);
  BOOL CreateEllipticRgnIndirect(LPCRECT lpRect);
  BOOL CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
  BOOL CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,
      int nCount, int nPolyFillMode);
  BOOL CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
  BOOL CreateFromPath(CDC* pDC);
  BOOL CreateFromData(const XFORM* lpXForm, int nCount,
      const RGNDATA* pRgnData);
  // Operations
  void SetRectRgn(int x1, int y1, int x2, int y2);
  void SetRectRgn(LPCRECT lpRect);
  int CombineRgn(CRgn* pRgn1, CRgn* pRgn2, int nCombineMode);
  int CopyRgn(CRgn* pRgnSrc);
  BOOL EqualRgn(CRgn* pRgn) const;
  int OffsetRgn(int x, int y);
  int OffsetRgn(POINT point);
  int GetRgnBox(LPRECT lpRect) const;
  BOOL PtInRegion(int x, int y) const;
  BOOL PtInRegion(POINT point) const;
  BOOL RectInRegion(LPCRECT lpRect) const;
  int GetRegionData(LPRGNDATA lpRgnData, int nCount) const;
  // Implementation
  virtual ~CRgn();
};
/////////////////////////////////////////////////////////////////////////////
// The device context
class CDC : public CObject
{
  DECLARE_DYNCREATE(CDC)
public:
  // Attributes
  HDC m_hDC;          // The output DC (must be first data member)
  HDC m_hAttribDC;    // The Attribute DC
  operator HDC() const;
  HDC GetSafeHdc() const; // Always returns the Output DC
  CWnd* GetWindow() const;
  static CDC* PASCAL FromHandle(HDC hDC);
  static void PASCAL DeleteTempMap();
  BOOL Attach(HDC hDC);   // Attach/Detach affects only the Output DC
  HDC Detach();
  virtual void SetAttribDC(HDC hDC);  // Set the Attribute DC
  virtual void SetOutputDC(HDC hDC);  // Set the Output DC
  virtual void ReleaseAttribDC();     // Release the Attribute DC
  virtual void ReleaseOutputDC();     // Release the Output DC
  BOOL IsPrinting() const;            // TRUE if being used for printing
  CPen* GetCurrentPen() const;
  CBrush* GetCurrentBrush() const;
  CPalette* GetCurrentPalette() const;
  CFont* GetCurrentFont() const;
  CBitmap* GetCurrentBitmap() const;
  // for bidi and mirrored localization
  DWORD GetLayout() const;
  DWORD SetLayout(DWORD dwLayout);
  // Constructors
  CDC();
  BOOL CreateDC(LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName,
      LPCTSTR lpszOutput, const void* lpInitData);
  BOOL CreateIC(LPCTSTR lpszDriverName, LPCTSTR lpszDeviceName,
      LPCTSTR lpszOutput, const void* lpInitData);
  BOOL CreateCompatibleDC(CDC* pDC);
  BOOL DeleteDC();
  // Device-Context Functions
  virtual int SaveDC();
  virtual BOOL RestoreDC(int nSavedDC);
  int GetDeviceCaps(int nIndex) const;
  UINT SetBoundsRect(LPCRECT lpRectBounds, UINT flags);
  UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags);
  BOOL ResetDC(const DEVMODE* lpDevMode);
  // Drawing-Tool Functions
  CPoint GetBrushOrg() const;
  CPoint SetBrushOrg(int x, int y);
  CPoint SetBrushOrg(POINT point);
  int EnumObjects(int nObjectType,
      int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData);
  // Type-safe selection helpers
public:
  virtual CGdiObject* SelectStockObject(int nIndex);
  CPen* SelectObject(CPen* pPen);
  CBrush* SelectObject(CBrush* pBrush);
  virtual CFont* SelectObject(CFont* pFont);
  CBitmap* SelectObject(CBitmap* pBitmap);
  int SelectObject(CRgn* pRgn);       // special return for regions
  CGdiObject* SelectObject(CGdiObject* pObject);
  // CGdiObject* provided so compiler doesn't use SelectObject(HGDIOBJ)
  // Color and Color Palette Functions
  COLORREF GetNearestColor(COLORREF crColor) const;
  CPalette* SelectPalette(CPalette* pPalette, BOOL bForceBackground);
  UINT RealizePalette();
  void UpdateColors();
  // Drawing-Attribute Functions
  COLORREF GetBkColor() const;
  int GetBkMode() const;
  int GetPolyFillMode() const;
  int GetROP2() const;
  int GetStretchBltMode() const;
  COLORREF GetTextColor() const;
  virtual COLORREF SetBkColor(COLORREF crColor);
  int SetBkMode(int nBkMode);
  int SetPolyFillMode(int nPolyFillMode);
  int SetROP2(int nDrawMode);
  int SetStretchBltMode(int nStretchMode);
  virtual COLORREF SetTextColor(COLORREF crColor);
  BOOL GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const;
  BOOL SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust);
  // Mapping Functions
  int GetMapMode() const;
  CPoint GetViewportOrg() const;
  virtual int SetMapMode(int nMapMode);
  // Viewport Origin
  virtual CPoint SetViewportOrg(int x, int y);
  CPoint SetViewportOrg(POINT point);
  virtual CPoint OffsetViewportOrg(int nWidth, int nHeight);
  // Viewport Extent
  CSize GetViewportExt() const;
  virtual CSize SetViewportExt(int cx, int cy);
  CSize SetViewportExt(SIZE size);
  virtual CSize ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom);
  // Window Origin
  CPoint GetWindowOrg() const;
  CPoint SetWindowOrg(int x, int y);
  CPoint SetWindowOrg(POINT point);
  CPoint OffsetWindowOrg(int nWidth, int nHeight);
  // Window extent
  CSize GetWindowExt() const;
  virtual CSize SetWindowExt(int cx, int cy);
  CSize SetWindowExt(SIZE size);
  virtual CSize ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom);
  // Coordinate Functions
  void DPtoLP(LPPOINT lpPoints, int nCount = 1) const;
  void DPtoLP(LPRECT lpRect) const;
  void DPtoLP(LPSIZE lpSize) const;
  void LPtoDP(LPPOINT lpPoints, int nCount = 1) const;
  void LPtoDP(LPRECT lpRect) const;
  void LPtoDP(LPSIZE lpSize) const;
  // Special Coordinate Functions (useful for dealing with metafiles and OLE)
  void DPtoHIMETRIC(LPSIZE lpSize) const;
  void LPtoHIMETRIC(LPSIZE lpSize) const;
  void HIMETRICtoDP(LPSIZE lpSize) const;
  void HIMETRICtoLP(LPSIZE lpSize) const;
  // Region Functions
  BOOL FillRgn(CRgn* pRgn, CBrush* pBrush);
  BOOL FrameRgn(CRgn* pRgn, CBrush* pBrush, int nWidth, int nHeight);
  BOOL InvertRgn(CRgn* pRgn);
  BOOL PaintRgn(CRgn* pRgn);
  // Clipping Functions
  virtual int GetClipBox(LPRECT lpRect) const;
  virtual BOOL PtVisible(int x, int y) const;
  BOOL PtVisible(POINT point) const;
  virtual BOOL RectVisible(LPCRECT lpRect) const;
  int SelectClipRgn(CRgn* pRgn);
  int ExcludeClipRect(int x1, int y1, int x2, int y2);
  int ExcludeClipRect(LPCRECT lpRect);
  int ExcludeUpdateRgn(CWnd* pWnd);
  int IntersectClipRect(int x1, int y1, int x2, int y2);
  int IntersectClipRect(LPCRECT lpRect);
  int OffsetClipRgn(int x, int y);
  int OffsetClipRgn(SIZE size);
  int SelectClipRgn(CRgn* pRgn, int nMode);
  // Line-Output Functions
  CPoint GetCurrentPosition() const;
  CPoint MoveTo(int x, int y);
  CPoint MoveTo(POINT point);
  BOOL LineTo(int x, int y);
  BOOL LineTo(POINT point);
  BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
  BOOL Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
  BOOL Polyline(LPPOINT lpPoints, int nCount);
  BOOL AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle);
  BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
  BOOL ArcTo(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
  int GetArcDirection() const;
  int SetArcDirection(int nArcDirection);
  BOOL PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount);
  BOOL PolylineTo(const POINT* lpPoints, int nCount);
  BOOL PolyPolyline(const POINT* lpPoints,
      const DWORD* lpPolyPoints, int nCount);
  BOOL PolyBezier(const POINT* lpPoints, int nCount);
  BOOL PolyBezierTo(const POINT* lpPoints, int nCount);
  // Simple Drawing Functions
  void FillRect(LPCRECT lpRect, CBrush* pBrush);
  void FrameRect(LPCRECT lpRect, CBrush* pBrush);
  void InvertRect(LPCRECT lpRect);
  BOOL DrawIcon(int x, int y, HICON hIcon);
  BOOL DrawIcon(POINT point, HICON hIcon);
#if (WINVER >= 0x400)
  BOOL DrawState(CPoint pt, CSize size, HBITMAP hBitmap, UINT nFlags,
      HBRUSH hBrush = NULL);
  BOOL DrawState(CPoint pt, CSize size, CBitmap* pBitmap, UINT nFlags,
      CBrush* pBrush = NULL);
  BOOL DrawState(CPoint pt, CSize size, HICON hIcon, UINT nFlags,
      HBRUSH hBrush = NULL);
  BOOL DrawState(CPoint pt, CSize size, HICON hIcon, UINT nFlags,
      CBrush* pBrush = NULL);
  BOOL DrawState(CPoint pt, CSize size, LPCTSTR lpszText, UINT nFlags,
      BOOL bPrefixText = TRUE, int nTextLen = 0, HBRUSH hBrush = NULL);
  BOOL DrawState(CPoint pt, CSize size, LPCTSTR lpszText, UINT nFlags,
      BOOL bPrefixText = TRUE, int nTextLen = 0, CBrush* pBrush = NULL);
  BOOL DrawState(CPoint pt, CSize size, DRAWSTATEPROC lpDrawProc,
      LPARAM lData, UINT nFlags, HBRUSH hBrush = NULL);
  BOOL DrawState(CPoint pt, CSize size, DRAWSTATEPROC lpDrawProc,
      LPARAM lData, UINT nFlags, CBrush* pBrush = NULL);
#endif
  // Ellipse and Polygon Functions
  BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3,
      int x4, int y4);
  BOOL Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
  void DrawFocusRect(LPCRECT lpRect);
  BOOL Ellipse(int x1, int y1, int x2, int y2);
  BOOL Ellipse(LPCRECT lpRect);
  BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
  BOOL Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
  BOOL Polygon(LPPOINT lpPoints, int nCount);
  BOOL PolyPolygon(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount);
  BOOL Rectangle(int x1, int y1, int x2, int y2);
  BOOL Rectangle(LPCRECT lpRect);
  BOOL RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
  BOOL RoundRect(LPCRECT lpRect, POINT point);
  // Bitmap Functions
  BOOL PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop);
  BOOL BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
      int xSrc, int ySrc, DWORD dwRop);
  BOOL StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
      int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
  COLORREF GetPixel(int x, int y) const;
  COLORREF GetPixel(POINT point) const;
  COLORREF SetPixel(int x, int y, COLORREF crColor);
  COLORREF SetPixel(POINT point, COLORREF crColor);
  BOOL FloodFill(int x, int y, COLORREF crColor);
  BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType);
  BOOL MaskBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC,
      int xSrc, int ySrc, CBitmap& maskBitmap, int xMask, int yMask,
      DWORD dwRop);
  BOOL PlgBlt(LPPOINT lpPoint, CDC* pSrcDC, int xSrc, int ySrc,
      int nWidth, int nHeight, CBitmap& maskBitmap, int xMask, int yMask);
  BOOL SetPixelV(int x, int y, COLORREF crColor);
  BOOL SetPixelV(POINT point, COLORREF crColor);
  // Text Functions
  virtual BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount);
  BOOL TextOut(int x, int y, const CString& str);
  virtual BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
      LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths);
  BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
      const CString& str, LPINT lpDxWidths);
  virtual CSize TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount,
      int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
  CSize TabbedTextOut(int x, int y, const CString& str,
      int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
  virtual int DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect,
      UINT nFormat);
  int DrawText(const CString& str, LPRECT lpRect, UINT nFormat);
  CSize GetTextExtent(LPCTSTR lpszString, int nCount) const;
  CSize GetTextExtent(const CString& str) const;
  CSize GetOutputTextExtent(LPCTSTR lpszString, int nCount) const;
  CSize GetOutputTextExtent(const CString& str) const;
  CSize GetTabbedTextExtent(LPCTSTR lpszString, int nCount,
      int nTabPositions, LPINT lpnTabStopPositions) const;
  CSize GetTabbedTextExtent(const CString& str,
      int nTabPositions, LPINT lpnTabStopPositions) const;
  CSize GetOutputTabbedTextExtent(LPCTSTR lpszString, int nCount,
      int nTabPositions, LPINT lpnTabStopPositions) const;
  CSize GetOutputTabbedTextExtent(const CString& str,
      int nTabPositions, LPINT lpnTabStopPositions) const;
  virtual BOOL GrayString(CBrush* pBrush,
      BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData,
      int nCount, int x, int y, int nWidth, int nHeight);
  UINT GetTextAlign() const;
  UINT SetTextAlign(UINT nFlags);
  int GetTextFace(int nCount, LPTSTR lpszFacename) const;
  int GetTextFace(CString& rString) const;
  BOOL GetTextMetrics(LPTEXTMETRIC lpMetrics) const;
  BOOL GetOutputTextMetrics(LPTEXTMETRIC lpMetrics) const;
  int SetTextJustification(int nBreakExtra, int nBreakCount);
  int GetTextCharacterExtra() const;
  int SetTextCharacterExtra(int nCharExtra);
  // Advanced Drawing
#if (WINVER >= 0x400)
  BOOL DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags);
  BOOL DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState);
#endif
  // Scrolling Functions
  BOOL ScrollDC(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip,
      CRgn* pRgnUpdate, LPRECT lpRectUpdate);
  // Font Functions
  BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
  BOOL GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
  DWORD SetMapperFlags(DWORD dwFlag);
  CSize GetAspectRatioFilter() const;
  BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const;
  DWORD GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpData, DWORD cbData) const;
  int GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const;
  UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpotm) const;
  DWORD GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
      DWORD cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const;
  BOOL GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
      LPABCFLOAT lpABCF) const;
  BOOL GetCharWidth(UINT nFirstChar, UINT nLastChar,
      float* lpFloatBuffer) const;
  // Printer/Device Escape Functions
  virtual int Escape(int nEscape, int nCount,
      LPCSTR lpszInData, LPVOID lpOutData);
  int Escape(int nEscape, int nInputSize, LPCSTR lpszInputData,
      int nOutputSize, LPSTR lpszOutputData);
  int DrawEscape(int nEscape, int nInputSize, LPCSTR lpszInputData);
  // Escape helpers
  int StartDoc(LPCTSTR lpszDocName);  // old Win3.0 version
  int StartDoc(LPDOCINFO lpDocInfo);
  int StartPage();
  int EndPage();
  int SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int));
  int AbortDoc();
  int EndDoc();
  // MetaFile Functions
  BOOL PlayMetaFile(HMETAFILE hMF);
  BOOL PlayMetaFile(HENHMETAFILE hEnhMetaFile, LPCRECT lpBounds);
  BOOL AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData);
  // can be used for enhanced metafiles only
  // Path Functions
  BOOL AbortPath();
  BOOL BeginPath();
  BOOL CloseFigure();
  BOOL EndPath();
  BOOL FillPath();
  BOOL FlattenPath();
  BOOL StrokeAndFillPath();
  BOOL StrokePath();
  BOOL WidenPath();
  float GetMiterLimit() const;
  BOOL SetMiterLimit(float fMiterLimit);
  int GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const;
  BOOL SelectClipPath(int nMode);
  // Misc Helper Functions
  static CBrush* PASCAL GetHalftoneBrush();
  void DrawDragRect(LPCRECT lpRect, SIZE size,
      LPCRECT lpRectLast, SIZE sizeLast,
      CBrush* pBrush = NULL, CBrush* pBrushLast = NULL);
  void FillSolidRect(LPCRECT lpRect, COLORREF clr);
  void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
  void Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
  void Draw3dRect(int x, int y, int cx, int cy,
      COLORREF clrTopLeft, COLORREF clrBottomRight);
  // Implementation
public:
  virtual ~CDC();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  // advanced use and implementation
  BOOL m_bPrinting;
  HGDIOBJ SelectObject(HGDIOBJ);      // do not use for regions
protected:
  // used for implementation of non-virtual SelectObject calls
  static CGdiObject* PASCAL SelectGdiObject(HDC hDC, HGDIOBJ h);
};
/////////////////////////////////////////////////////////////////////////////
// CDC Helpers
class CPaintDC : public CDC
{
  DECLARE_DYNAMIC(CPaintDC)
  // Constructors
public:
  CPaintDC(CWnd* pWnd);   // BeginPaint
  // Attributes
protected:
  HWID m_hWnd;
public:
  PAINTSTRUCT m_ps;       // actual paint struct!
  // Implementation
public:
  virtual ~CPaintDC();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};
class CClientDC : public CDC
{
  DECLARE_DYNAMIC(CClientDC)
  // Constructors
public:
  CClientDC(CWnd* pWnd);
  // Attributes
protected:
  HWID m_hWnd;
  // Implementation
public:
  virtual ~CClientDC();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};
class CWindowDC : public CDC
{
  DECLARE_DYNAMIC(CWindowDC)
  // Constructors
public:
  CWindowDC(CWnd* pWnd);
  // Attributes
protected:
  HWID m_hWnd;
  // Implementation
public:
  virtual ~CWindowDC();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CMenu
class CMenu : public CObject
{
  DECLARE_DYNCREATE(CMenu)
public:
  // Constructors
  CMenu();
  BOOL CreateMenu();
  BOOL CreatePopupMenu();
  BOOL LoadMenu(LPCTSTR lpszResourceName);
  BOOL LoadMenu(UINT nIDResource);
  BOOL LoadMenuIndirect(const void* lpMenuTemplate);
  BOOL DestroyMenu();
  // Attributes
  HMENU m_hMenu;          // must be first data member
  HMENU GetSafeHmenu() const;
  operator HMENU() const;
  static CMenu* PASCAL FromHandle(HMENU hMenu);
  static void PASCAL DeleteTempMap();
  BOOL Attach(HMENU hMenu);
  HMENU Detach();
  // CMenu Operations
  BOOL DeleteMenu(UINT nPosition, UINT nFlags);
  BOOL TrackPopupMenu(UINT nFlags, int x, int y,
      CWnd* pWnd, LPCRECT lpRect = 0);
  BOOL operator==(const CMenu& menu) const;
  BOOL operator!=(const CMenu& menu) const;
  // CMenuItem Operations
  BOOL AppendMenu(UINT nFlags, UINT nIDNewItem = 0,
      LPCTSTR lpszNewItem = NULL);
  BOOL AppendMenu(UINT nFlags, UINT nIDNewItem, const CBitmap* pBmp);
  UINT CheckMenuItem(UINT nIDCheckItem, UINT nCheck);
  UINT EnableMenuItem(UINT nIDEnableItem, UINT nEnable);
  UINT GetMenuItemCount() const;
  UINT GetMenuItemID(int nPos) const;
  UINT GetMenuState(UINT nID, UINT nFlags) const;
  int GetMenuString(UINT nIDItem, LPTSTR lpString, int nMaxCount,
      UINT nFlags) const;
  int GetMenuString(UINT nIDItem, CString& rString, UINT nFlags) const;
  BOOL GetMenuItemInfo(UINT nIDItem, LPMENUITEMINFO lpMenuItemInfo,
      BOOL fByPos = FALSE);
  CMenu* GetSubMenu(int nPos) const;
  BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0,
      LPCTSTR lpszNewItem = NULL);
  BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem,
      const CBitmap* pBmp);
  BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0,
      LPCTSTR lpszNewItem = NULL);
  BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem,
      const CBitmap* pBmp);
  BOOL RemoveMenu(UINT nPosition, UINT nFlags);
  BOOL SetMenuItemBitmaps(UINT nPosition, UINT nFlags,
      const CBitmap* pBmpUnchecked, const CBitmap* pBmpChecked);
  BOOL CheckMenuRadioItem(UINT nIDFirst, UINT nIDLast, UINT nIDItem, UINT nFlags);
  BOOL SetDefaultItem(UINT uItem, BOOL fByPos = FALSE);
  UINT GetDefaultItem(UINT gmdiFlags, BOOL fByPos = FALSE);
  // Context Help Functions
  BOOL SetMenuContextHelpId(DWORD dwContextHelpId);
  DWORD GetMenuContextHelpId() const;
  // Overridables (must override draw and measure for owner-draw menu items)
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
  // Implementation
public:
  virtual ~CMenu();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  static CMenu* PASCAL CMenu::FromHandlePermanent(HMENU hMenu);
};
/////////////////////////////////////////////////////////////////////////////
// Window message map handling
struct AFX_MSGMAP_ENTRY;       // declared below after CWnd
struct AFX_MSGMAP {
#ifdef _AFXDLL
  const AFX_MSGMAP* (PASCAL* pfnGetBaseMap)();
#else
  const AFX_MSGMAP* pBaseMap;
#endif
  const AFX_MSGMAP_ENTRY* lpEntries;
};
#ifdef _AFXDLL
#define DECLARE_MESSAGE_MAP() \
  private: \
  static const AFX_MSGMAP_ENTRY _messageEntries[]; \
  protected: \
  static AFX_DATA const AFX_MSGMAP messageMap; \
  static const AFX_MSGMAP* PASCAL _GetBaseMessageMap(); \
  virtual const AFX_MSGMAP* GetMessageMap() const;
#else
#define DECLARE_MESSAGE_MAP() \
  private: \
  static const AFX_MSGMAP_ENTRY _messageEntries[]; \
  protected: \
  static AFX_DATA const AFX_MSGMAP messageMap; \
  virtual const AFX_MSGMAP* GetMessageMap() const;
#endif
#ifdef _AFXDLL
#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
    const AFX_MSGMAP* PASCAL theClass::_GetBaseMessageMap() \
    { return &baseClass::messageMap; } \
    const AFX_MSGMAP* theClass::GetMessageMap() const \
    { return &theClass::messageMap; } \
    AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
        { &theClass::_GetBaseMessageMap, &theClass::_messageEntries[0] }; \
    AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = {
#else
#define BEGIN_MESSAGE_MAP(theClass, baseClass) \
    const AFX_MSGMAP* theClass::GetMessageMap() const \
    { return &theClass::messageMap; } \
    AFX_COMDAT AFX_DATADEF const AFX_MSGMAP theClass::messageMap = \
        { &baseClass::messageMap, &theClass::_messageEntries[0] }; \
    AFX_COMDAT const AFX_MSGMAP_ENTRY theClass::_messageEntries[] = {
#endif
#define END_MESSAGE_MAP() {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }   };
// Message map signature values and macros in separate header

enum AfxSig {
  AfxSig_end = 0,     // [marks end of message map]

  AfxSig_bD,      // BOOL (CDC*)
  AfxSig_bb,      // BOOL (BOOL)
  AfxSig_bWww,    // BOOL (CWnd*, UINT, UINT)
  AfxSig_hDWw,    // HBRUSH (CDC*, CWnd*, UINT)
  AfxSig_hDw,     // HBRUSH (CDC*, UINT)
  AfxSig_iwWw,    // int (UINT, CWnd*, UINT)
  AfxSig_iww,     // int (UINT, UINT)
  AfxSig_iWww,    // int (CWnd*, UINT, UINT)
  AfxSig_is,      // int (LPTSTR)
  AfxSig_lwl,     // LRESULT (WPARAM, LPARAM)
  AfxSig_lwwM,    // LRESULT (UINT, UINT, CMenu*)
  AfxSig_vv,      // void (void)

  AfxSig_vw,      // void (UINT)
  AfxSig_vww,     // void (UINT, UINT)
  AfxSig_vvii,    // void (int, int) // wParam is ignored
  AfxSig_vwww,    // void (UINT, UINT, UINT)
  AfxSig_vwii,    // void (UINT, int, int)
  AfxSig_vwl,     // void (UINT, LPARAM)
  AfxSig_vbWW,    // void (BOOL, CWnd*, CWnd*)
  AfxSig_vD,      // void (CDC*)
  AfxSig_vM,      // void (CMenu*)
  AfxSig_vMwb,    // void (CMenu*, UINT, BOOL)

  AfxSig_vW,      // void (CWnd*)
  AfxSig_vWww,    // void (CWnd*, UINT, UINT)
  AfxSig_vWp,     // void (CWnd*, CPoint)
  AfxSig_vWh,     // void (CWnd*, HANDLE)
  AfxSig_vwW,     // void (UINT, CWnd*)
  AfxSig_vwWb,    // void (UINT, CWnd*, BOOL)
  AfxSig_vwwW,    // void (UINT, UINT, CWnd*)
  AfxSig_vwwx,    // void (UINT, UINT)
  AfxSig_vs,      // void (LPTSTR)
  AfxSig_vOWNER,  // void (int, LPTSTR), force return TRUE
  AfxSig_iis,     // int (int, LPTSTR)
  AfxSig_wp,      // UINT (CPoint)
  AfxSig_wv,      // UINT (void)
  AfxSig_vPOS,    // void (WINDOWPOS*)
  AfxSig_vCALC,   // void (BOOL, NCCALCSIZE_PARAMS*)
  AfxSig_vNMHDRpl,    // void (NMHDR*, LRESULT*)
  AfxSig_bNMHDRpl,    // BOOL (NMHDR*, LRESULT*)
  AfxSig_vwNMHDRpl,   // void (UINT, NMHDR*, LRESULT*)
  AfxSig_bwNMHDRpl,   // BOOL (UINT, NMHDR*, LRESULT*)
  AfxSig_bHELPINFO,   // BOOL (HELPINFO*)
  AfxSig_vwSIZING,    // void (UINT, LPRECT) -- return TRUE

  // signatures specific to CCmdTarget
  AfxSig_cmdui,   // void (CCmdUI*)
  AfxSig_cmduiw,  // void (CCmdUI*, UINT)
  AfxSig_vpv,     // void (void*)
  AfxSig_bpv,     // BOOL (void*)

  // Other aliases (based on implementation)
  AfxSig_vwwh,                // void (UINT, UINT, HANDLE)
  AfxSig_vwp,                 // void (UINT, CPoint)
  AfxSig_bw = AfxSig_bb,      // BOOL (UINT)
  AfxSig_bh = AfxSig_bb,      // BOOL (HANDLE)
  AfxSig_iw = AfxSig_bb,      // int (UINT)
  AfxSig_ww = AfxSig_bb,      // UINT (UINT)
  AfxSig_bv = AfxSig_wv,      // BOOL (void)
  AfxSig_hv = AfxSig_wv,      // HANDLE (void)
  AfxSig_vb = AfxSig_vw,      // void (BOOL)
  AfxSig_vbh = AfxSig_vww,    // void (BOOL, HANDLE)
  AfxSig_vbw = AfxSig_vww,    // void (BOOL, UINT)
  AfxSig_vhh = AfxSig_vww,    // void (HANDLE, HANDLE)
  AfxSig_vh = AfxSig_vw,      // void (HANDLE)
  AfxSig_viSS = AfxSig_vwl,   // void (int, STYLESTRUCT*)
  AfxSig_bwl = AfxSig_lwl,
  AfxSig_vwMOVING = AfxSig_vwSIZING,  // void (UINT, LPRECT) -- return TRUE

  AfxSig_vW2,                 // void (CWnd*) (CWnd* comes from lParam)
  AfxSig_bWCDS,               // BOOL (CWnd*, COPYDATASTRUCT*)
  AfxSig_bwsp,                // BOOL (UINT, short, CPoint)
  AfxSig_vws,
};

/////////////////////////////////////////////////////////////////////////////
// Command notifications for CCmdTarget notifications

#define CN_COMMAND              0               // void ()
#define CN_UPDATE_COMMAND_UI    ((UINT)(-1))    // void (CCmdUI*)
#define CN_EVENT                ((UINT)(-2))    // OLE event
#define CN_OLECOMMAND           ((UINT)(-3))    // OLE document command
#define CN_OLE_UNREGISTER       ((UINT)(-4))    // OLE unregister
// > 0 are control notifications
// < 0 are for MFC's use

#define ON_COMMAND(id, memberFxn) \
  { WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)id, AfxSig_vv, (AFX_PMSG)&memberFxn },
// ON_COMMAND(id, OnFoo) is the same as
//   ON_CONTROL(0, id, OnFoo) or ON_BN_CLICKED(0, id, OnFoo)

#define ON_COMMAND_RANGE(id, idLast, memberFxn) \
  { WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)idLast, AfxSig_vw, \
    (AFX_PMSG)(void (AFX_MSG_CALL CCmdTarget::*)(UINT))&memberFxn },
// ON_COMMAND_RANGE(id, idLast, OnFoo) is the same as
//   ON_CONTROL_RANGE(0, id, idLast, OnFoo)

#define ON_COMMAND_EX(id, memberFxn) \
  { WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)id, AfxSig_bw, \
    (AFX_PMSG)(BOOL (AFX_MSG_CALL CCmdTarget::*)(UINT))&memberFxn },

#define ON_COMMAND_EX_RANGE(id, idLast, memberFxn) \
  { WM_COMMAND, CN_COMMAND, (WORD)id, (WORD)idLast, AfxSig_bw, \
    (AFX_PMSG)(BOOL (AFX_MSG_CALL CCmdTarget::*)(UINT))&memberFxn },

// update ui's are listed as WM_COMMAND's so they get routed like commands
#define ON_UPDATE_COMMAND_UI(id, memberFxn) \
  { WM_COMMAND, CN_UPDATE_COMMAND_UI, (WORD)id, (WORD)id, AfxSig_cmdui, \
    (AFX_PMSG)(void (AFX_MSG_CALL CCmdTarget::*)(CCmdUI*))&memberFxn },

#define ON_UPDATE_COMMAND_UI_RANGE(id, idLast, memberFxn) \
  { WM_COMMAND, CN_UPDATE_COMMAND_UI, (WORD)id, (WORD)idLast, AfxSig_cmdui, \
    (AFX_PMSG)(void (AFX_MSG_CALL CCmdTarget::*)(CCmdUI*))&memberFxn },

#define ON_NOTIFY(wNotifyCode, id, memberFxn) \
  { WM_NOTIFY, (WORD)(int)wNotifyCode, (WORD)id, (WORD)id, AfxSig_vNMHDRpl, \
    (AFX_PMSG)(void (AFX_MSG_CALL CCmdTarget::*)(NMHDR*, LRESULT*))&memberFxn },

#define ON_NOTIFY_RANGE(wNotifyCode, id, idLast, memberFxn) \
  { WM_NOTIFY, (WORD)(int)wNotifyCode, (WORD)id, (WORD)idLast, AfxSig_vwNMHDRpl, \
    (AFX_PMSG)(void (AFX_MSG_CALL CCmdTarget::*)(UINT, NMHDR*, LRESULT*))&memberFxn },

#define ON_NOTIFY_EX(wNotifyCode, id, memberFxn) \
  { WM_NOTIFY, (WORD)(int)wNotifyCode, (WORD)id, (WORD)id, AfxSig_bwNMHDRpl, \
    (AFX_PMSG)(BOOL (AFX_MSG_CALL CCmdTarget::*)(UINT, NMHDR*, LRESULT*))&memberFxn },

#define ON_NOTIFY_EX_RANGE(wNotifyCode, id, idLast, memberFxn) \
  { WM_NOTIFY, (WORD)(int)wNotifyCode, (WORD)id, (WORD)idLast, AfxSig_bwNMHDRpl, \
    (AFX_PMSG)(BOOL (AFX_MSG_CALL CCmdTarget::*)(UINT, NMHDR*, LRESULT*))&memberFxn },

// for general controls
#define ON_CONTROL(wNotifyCode, id, memberFxn) \
  { WM_COMMAND, (WORD)wNotifyCode, (WORD)id, (WORD)id, AfxSig_vv, \
    (AFX_PMSG)&memberFxn },

#define ON_CONTROL_RANGE(wNotifyCode, id, idLast, memberFxn) \
  { WM_COMMAND, (WORD)wNotifyCode, (WORD)id, (WORD)idLast, AfxSig_vw, \
    (AFX_PMSG)(void (AFX_MSG_CALL CCmdTarget::*)(UINT))&memberFxn },

#define WM_REFLECT_BASE 0xBC00

// for control notification reflection
#define ON_CONTROL_REFLECT(wNotifyCode, memberFxn) \
  { WM_COMMAND+WM_REFLECT_BASE, (WORD)wNotifyCode, 0, 0, AfxSig_vv, \
    (AFX_PMSG)&memberFxn },

#define ON_CONTROL_REFLECT_EX(wNotifyCode, memberFxn) \
  { WM_COMMAND+WM_REFLECT_BASE, (WORD)wNotifyCode, 0, 0, AfxSig_bv, \
    (AFX_PMSG)(BOOL (AFX_MSG_CALL CCmdTarget::*)())&memberFxn },

#define ON_NOTIFY_REFLECT(wNotifyCode, memberFxn) \
  { WM_NOTIFY+WM_REFLECT_BASE, (WORD)(int)wNotifyCode, 0, 0, AfxSig_vNMHDRpl, \
    (AFX_PMSG)(void (AFX_MSG_CALL CCmdTarget::*)(NMHDR*, LRESULT*))&memberFxn },

#define ON_NOTIFY_REFLECT_EX(wNotifyCode, memberFxn) \
  { WM_NOTIFY+WM_REFLECT_BASE, (WORD)(int)wNotifyCode, 0, 0, AfxSig_bNMHDRpl, \
    (AFX_PMSG)(BOOL (AFX_MSG_CALL CCmdTarget::*)(NMHDR*, LRESULT*))&memberFxn },

#define ON_UPDATE_COMMAND_UI_REFLECT(memberFxn) \
  { WM_COMMAND+WM_REFLECT_BASE, (WORD)CN_UPDATE_COMMAND_UI, 0, 0, AfxSig_cmdui, \
    (AFX_PMSG)(void (AFX_MSG_CALL CCmdTarget::*)(CCmdUI*))&memberFxn },

/////////////////////////////////////////////////////////////////////////////
// Message map tables for Windows messages

#define ON_WM_CREATE() \
  { WM_CREATE, 0, 0, 0, AfxSig_is, \
    (AFX_PMSG)(AFX_PMSGW)(int (AFX_MSG_CALL CWnd::*)(LPCREATESTRUCT))&OnCreate },
#define ON_WM_COPYDATA() \
  { WM_COPYDATA, 0, 0, 0, AfxSig_bWCDS, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(CWnd*, COPYDATASTRUCT*))&OnCopyData },
#define ON_WM_DESTROY() \
  { WM_DESTROY, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnDestroy },
#define ON_WM_MOVE() \
  { WM_MOVE, 0, 0, 0, AfxSig_vvii, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(int, int))&OnMove },
#define ON_WM_SIZE() \
  { WM_SIZE, 0, 0, 0, AfxSig_vwii, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, int, int))&OnSize },
#define ON_WM_ACTIVATE() \
  { WM_ACTIVATE, 0, 0, 0, AfxSig_vwWb, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CWnd*, BOOL))&OnActivate },
#define ON_WM_SETFOCUS() \
  { WM_SETFOCUS, 0, 0, 0, AfxSig_vW, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CWnd*))&OnSetFocus },
#define ON_WM_KILLFOCUS() \
  { WM_KILLFOCUS, 0, 0, 0, AfxSig_vW, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CWnd*))&OnKillFocus },
#define ON_WM_ENABLE() \
  { WM_ENABLE, 0, 0, 0, AfxSig_vb, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(BOOL))&OnEnable },
#define ON_WM_HELPINFO() \
  { WM_HELP, 0, 0, 0, AfxSig_bHELPINFO, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(HELPINFO*))&OnHelpInfo },
#define ON_WM_PAINT() \
  { WM_PAINT, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnPaint },
#define ON_WM_CLOSE() \
  { WM_CLOSE, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnClose },
#define ON_WM_QUERYENDSESSION() \
  { WM_QUERYENDSESSION, 0, 0, 0, AfxSig_bv, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(void))&OnQueryEndSession },
#define ON_WM_QUERYOPEN() \
  { WM_QUERYOPEN, 0, 0, 0, AfxSig_bv, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(void))&OnQueryOpen },
#define ON_WM_ERASEBKGND() \
  { WM_ERASEBKGND, 0, 0, 0, AfxSig_bD, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(CDC*))&OnEraseBkgnd },
#define ON_WM_SYSCOLORCHANGE() \
  { WM_SYSCOLORCHANGE, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnSysColorChange },
#define ON_WM_ENDSESSION() \
  { WM_ENDSESSION, 0, 0, 0, AfxSig_vb, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(BOOL))&OnEndSession },
#define ON_WM_SHOWWINDOW() \
  { WM_SHOWWINDOW, 0, 0, 0, AfxSig_vbw, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(BOOL, UINT))&OnShowWindow },
#define ON_WM_CTLCOLOR() \
  { WM_CTLCOLOR, 0, 0, 0, AfxSig_hDWw, \
    (AFX_PMSG)(AFX_PMSGW)(HBRUSH (AFX_MSG_CALL CWnd::*)(CDC*, CWnd*, UINT))&OnCtlColor },
#define ON_WM_CTLCOLOR_REFLECT() \
  { WM_CTLCOLOR+WM_REFLECT_BASE, 0, 0, 0, AfxSig_hDw, \
    (AFX_PMSG)(AFX_PMSGW)(HBRUSH (AFX_MSG_CALL CWnd::*)(CDC*, UINT))CtlColor },
#if (WINVER >= 0x0400)
#define ON_WM_SETTINGCHANGE() \
  { WM_SETTINGCHANGE, 0, 0, 0, AfxSig_vws, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, LPCTSTR))&OnSettingChange },
#endif
#define ON_WM_WININICHANGE() \
  { WM_WININICHANGE, 0, 0, 0, AfxSig_vs, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(LPCTSTR))&OnWinIniChange },
#define ON_WM_DEVMODECHANGE() \
  { WM_DEVMODECHANGE, 0, 0, 0, AfxSig_vs, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(LPTSTR))&OnDevModeChange },
#define ON_WM_ACTIVATEAPP() \
  { WM_ACTIVATEAPP, 0, 0, 0, AfxSig_vbh, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(BOOL, HTASK))&OnActivateApp },
#define ON_WM_FONTCHANGE() \
  { WM_FONTCHANGE, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnFontChange },
#define ON_WM_TIMECHANGE() \
  { WM_TIMECHANGE, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnTimeChange },
#define ON_WM_CANCELMODE() \
  { WM_CANCELMODE, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnCancelMode },
#define ON_WM_SETCURSOR() \
  { WM_SETCURSOR, 0, 0, 0, AfxSig_bWww, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(CWnd*, UINT, UINT))&OnSetCursor },
#define ON_WM_MOUSEACTIVATE() \
  { WM_MOUSEACTIVATE, 0, 0, 0, AfxSig_iWww, \
    (AFX_PMSG)(AFX_PMSGW)(int (AFX_MSG_CALL CWnd::*)(CWnd*, UINT, UINT))&OnMouseActivate },
#define ON_WM_CHILDACTIVATE() \
  { WM_CHILDACTIVATE, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnChildActivate },
#define ON_WM_GETMINMAXINFO() \
  { WM_GETMINMAXINFO, 0, 0, 0, AfxSig_vs, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(MINMAXINFO*))&OnGetMinMaxInfo },
#define ON_WM_ICONERASEBKGND() \
  { WM_ICONERASEBKGND, 0, 0, 0, AfxSig_vD, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CDC*))&OnIconEraseBkgnd },
#define ON_WM_SPOOLERSTATUS() \
  { WM_SPOOLERSTATUS, 0, 0, 0, AfxSig_vww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT))&OnSpoolerStatus },
#define ON_WM_DRAWITEM() \
  { WM_DRAWITEM, 0, 0, 0, AfxSig_vOWNER, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(int, LPDRAWITEMSTRUCT))&OnDrawItem },
#define ON_WM_DRAWITEM_REFLECT() \
  { WM_DRAWITEM+WM_REFLECT_BASE, 0, 0, 0, AfxSig_vs, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(LPDRAWITEMSTRUCT))DrawItem },
#define ON_WM_MEASUREITEM() \
  { WM_MEASUREITEM, 0, 0, 0, AfxSig_vOWNER, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(int, LPMEASUREITEMSTRUCT))&OnMeasureItem },
#define ON_WM_MEASUREITEM_REFLECT() \
  { WM_MEASUREITEM+WM_REFLECT_BASE, 0, 0, 0, AfxSig_vs, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(LPMEASUREITEMSTRUCT))MeasureItem },
#define ON_WM_DELETEITEM() \
  { WM_DELETEITEM, 0, 0, 0, AfxSig_vOWNER, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(int, LPDELETEITEMSTRUCT))&OnDeleteItem },
#define ON_WM_DELETEITEM_REFLECT() \
  { WM_DELETEITEM+WM_REFLECT_BASE, 0, 0, 0, AfxSig_vs, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(LPDELETEITEMSTRUCT))DeleteItem },
#define ON_WM_CHARTOITEM() \
  { WM_CHARTOITEM, 0, 0, 0, AfxSig_iwWw, \
    (AFX_PMSG)(AFX_PMSGW)(int (AFX_MSG_CALL CWnd::*)(UINT, CListBox*, UINT))&OnCharToItem },
#define ON_WM_CHARTOITEM_REFLECT() \
  { WM_CHARTOITEM+WM_REFLECT_BASE, 0, 0, 0, AfxSig_iww, \
    (AFX_PMSG)(AFX_PMSGW)(int (AFX_MSG_CALL CWnd::*)(UINT, UINT))CharToItem },
#define ON_WM_VKEYTOITEM() \
  { WM_VKEYTOITEM, 0, 0, 0, AfxSig_iwWw, \
    (AFX_PMSG)(AFX_PMSGW)(int (AFX_MSG_CALL CWnd::*)(UINT, CListBox*, UINT))&OnVKeyToItem },
#define ON_WM_VKEYTOITEM_REFLECT() \
  { WM_VKEYTOITEM+WM_REFLECT_BASE, 0, 0, 0, AfxSig_iww, \
    (AFX_PMSG)(AFX_PMSGW)(int (AFX_MSG_CALL CWnd::*)(UINT, UINT))VKeyToItem },
#define ON_WM_QUERYDRAGICON() \
  { WM_QUERYDRAGICON, 0, 0, 0, AfxSig_hv, \
    (AFX_PMSG)(AFX_PMSGW)(HCURSOR (AFX_MSG_CALL CWnd::*)())&OnQueryDragIcon },
#define ON_WM_COMPAREITEM() \
  { WM_COMPAREITEM, 0, 0, 0, AfxSig_iis, \
    (AFX_PMSG)(AFX_PMSGW)(int (AFX_MSG_CALL CWnd::*)(int, LPCOMPAREITEMSTRUCT))&OnCompareItem },
#define ON_WM_COMPAREITEM_REFLECT() \
  { WM_COMPAREITEM+WM_REFLECT_BASE, 0, 0, 0, AfxSig_is, \
    (AFX_PMSG)(AFX_PMSGW)(int (AFX_MSG_CALL CWnd::*)(LPCOMPAREITEMSTRUCT))CompareItem },
#define ON_WM_COMPACTING() \
  { WM_COMPACTING, 0, 0, 0, AfxSig_vw, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT))&OnCompacting },
#define ON_WM_NCCREATE() \
  { WM_NCCREATE, 0, 0, 0, AfxSig_is, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(LPCREATESTRUCT))&OnNcCreate },
#define ON_WM_NCDESTROY() \
  { WM_NCDESTROY, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnNcDestroy },
#define ON_WM_NCCALCSIZE() \
  { WM_NCCALCSIZE, 0, 0, 0, AfxSig_vCALC, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(BOOL, NCCALCSIZE_PARAMS*))&OnNcCalcSize },
#define ON_WM_NCHITTEST() \
  { WM_NCHITTEST, 0, 0, 0, AfxSig_wp, \
    (AFX_PMSG)(AFX_PMSGW)(UINT (AFX_MSG_CALL CWnd::*)(CPoint))&OnNcHitTest },
#define ON_WM_NCPAINT() \
  { WM_NCPAINT, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnNcPaint },
#define ON_WM_NCACTIVATE() \
  { WM_NCACTIVATE, 0, 0, 0, AfxSig_bb, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(BOOL))&OnNcActivate },
#define ON_WM_GETDLGCODE() \
  { WM_GETDLGCODE, 0, 0, 0, AfxSig_wv, \
    (AFX_PMSG)(AFX_PMSGW)(UINT (AFX_MSG_CALL CWnd::*)(void))&OnGetDlgCode },
#define ON_WM_NCMOUSEMOVE() \
  { WM_NCMOUSEMOVE, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnNcMouseMove },
#define ON_WM_NCLBUTTONDOWN() \
  { WM_NCLBUTTONDOWN, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnNcLButtonDown },
#define ON_WM_NCLBUTTONUP() \
  { WM_NCLBUTTONUP, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnNcLButtonUp },
#define ON_WM_NCLBUTTONDBLCLK() \
  { WM_NCLBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnNcLButtonDblClk },
#define ON_WM_NCRBUTTONDOWN() \
  { WM_NCRBUTTONDOWN, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnNcRButtonDown },
#define ON_WM_NCRBUTTONUP() \
  { WM_NCRBUTTONUP, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnNcRButtonUp },
#define ON_WM_NCRBUTTONDBLCLK() \
  { WM_NCRBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnNcRButtonDblClk },
#define ON_WM_NCMBUTTONDOWN() \
  { WM_NCMBUTTONDOWN, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnNcMButtonDown },
#define ON_WM_NCMBUTTONUP() \
  { WM_NCMBUTTONUP, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnNcMButtonUp },
#define ON_WM_NCMBUTTONDBLCLK() \
  { WM_NCMBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnNcMButtonDblClk },
#define ON_WM_KEYDOWN() \
  { WM_KEYDOWN, 0, 0, 0, AfxSig_vwww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT))&OnKeyDown },
#define ON_WM_KEYUP() \
  { WM_KEYUP, 0, 0, 0, AfxSig_vwww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT))&OnKeyUp },
#define ON_WM_CHAR() \
  { WM_CHAR, 0, 0, 0, AfxSig_vwww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT))&OnChar },
#define ON_WM_DEADCHAR() \
  { WM_DEADCHAR, 0, 0, 0, AfxSig_vwww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT))&OnDeadChar },
#define ON_WM_SYSKEYDOWN() \
  { WM_SYSKEYDOWN, 0, 0, 0, AfxSig_vwww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT))&OnSysKeyDown },
#define ON_WM_SYSKEYUP() \
  { WM_SYSKEYUP, 0, 0, 0, AfxSig_vwww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT))&OnSysKeyUp },
#define ON_WM_SYSCHAR() \
  { WM_SYSCHAR, 0, 0, 0, AfxSig_vwww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT))&OnSysChar },
#define ON_WM_SYSDEADCHAR() \
  { WM_SYSDEADCHAR, 0, 0, 0, AfxSig_vwww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, UINT))&OnSysDeadChar },
#define ON_WM_SYSCOMMAND() \
  { WM_SYSCOMMAND, 0, 0, 0, AfxSig_vwl, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, LPARAM))&OnSysCommand },
#define ON_WM_TCARD() \
  { WM_TCARD, 0, 0, 0, AfxSig_vwl, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, DWORD))&OnTCard },
#define ON_WM_TIMER() \
  { WM_TIMER, 0, 0, 0, AfxSig_vw, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT))&OnTimer },
#define ON_WM_HSCROLL() \
  { WM_HSCROLL, 0, 0, 0, AfxSig_vwwW, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, CScrollBar*))&OnHScroll },
#define ON_WM_HSCROLL_REFLECT() \
  { WM_HSCROLL+WM_REFLECT_BASE, 0, 0, 0, AfxSig_vwwx, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT))HScroll },
#define ON_WM_VSCROLL() \
  { WM_VSCROLL, 0, 0, 0, AfxSig_vwwW, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, CScrollBar*))&OnVScroll },
#define ON_WM_VSCROLL_REFLECT() \
  { WM_VSCROLL+WM_REFLECT_BASE, 0, 0, 0, AfxSig_vwwx, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT))VScroll },
#define ON_WM_INITMENU() \
  { WM_INITMENU, 0, 0, 0, AfxSig_vM, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CMenu*))&OnInitMenu },
#define ON_WM_INITMENUPOPUP() \
  { WM_INITMENUPOPUP, 0, 0, 0, AfxSig_vMwb, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CMenu*, UINT, BOOL))&OnInitMenuPopup },
#define ON_WM_MENUSELECT() \
  { WM_MENUSELECT, 0, 0, 0, AfxSig_vwwh, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT, HMENU))&OnMenuSelect },
#define ON_WM_MENUCHAR() \
  { WM_MENUCHAR, 0, 0, 0, AfxSig_lwwM, \
    (AFX_PMSG)(AFX_PMSGW)(LRESULT (AFX_MSG_CALL CWnd::*)(UINT, UINT, CMenu*))&OnMenuChar },
#define ON_WM_ENTERIDLE() \
  { WM_ENTERIDLE, 0, 0, 0, AfxSig_vwW, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CWnd*))&OnEnterIdle },
#define ON_WM_MOUSEMOVE() \
  { WM_MOUSEMOVE, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnMouseMove },
#define ON_WM_MOUSEWHEEL() \
  { WM_MOUSEWHEEL, 0, 0, 0, AfxSig_bwsp, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(UINT, short, CPoint))&OnMouseWheel },
#define ON_WM_LBUTTONDOWN() \
  { WM_LBUTTONDOWN, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnLButtonDown },
#define ON_WM_LBUTTONUP() \
  { WM_LBUTTONUP, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnLButtonUp },
#define ON_WM_LBUTTONDBLCLK() \
  { WM_LBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnLButtonDblClk },
#define ON_WM_RBUTTONDOWN() \
  { WM_RBUTTONDOWN, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnRButtonDown },
#define ON_WM_RBUTTONUP() \
  { WM_RBUTTONUP, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnRButtonUp },
#define ON_WM_RBUTTONDBLCLK() \
  { WM_RBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnRButtonDblClk },
#define ON_WM_MBUTTONDOWN() \
  { WM_MBUTTONDOWN, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnMButtonDown },
#define ON_WM_MBUTTONUP() \
  { WM_MBUTTONUP, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnMButtonUp },
#define ON_WM_MBUTTONDBLCLK() \
  { WM_MBUTTONDBLCLK, 0, 0, 0, AfxSig_vwp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, CPoint))&OnMButtonDblClk },
#define ON_WM_PARENTNOTIFY() \
  { WM_PARENTNOTIFY, 0, 0, 0, AfxSig_vwl, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, LPARAM))&OnParentNotify },
#define ON_WM_PARENTNOTIFY_REFLECT() \
  { WM_PARENTNOTIFY+WM_REFLECT_BASE, 0, 0, 0, AfxSig_vwl, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, LPARAM))ParentNotify },
#define ON_WM_MDIACTIVATE() \
  { WM_MDIACTIVATE, 0, 0, 0, AfxSig_vbWW, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(BOOL, CWnd*, CWnd*))&OnMDIActivate },
#define ON_WM_RENDERFORMAT() \
  { WM_RENDERFORMAT, 0, 0, 0, AfxSig_vw, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT))&OnRenderFormat },
#define ON_WM_RENDERALLFORMATS() \
  { WM_RENDERALLFORMATS, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnRenderAllFormats },
#define ON_WM_DESTROYCLIPBOARD() \
  { WM_DESTROYCLIPBOARD, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnDestroyClipboard },
#define ON_WM_DRAWCLIPBOARD() \
  { WM_DRAWCLIPBOARD, 0, 0, 0, AfxSig_vv, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(void))&OnDrawClipboard },
#define ON_WM_PAINTCLIPBOARD() \
  { WM_PAINTCLIPBOARD, 0, 0, 0, AfxSig_vWh, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CWnd*, HGLOBAL))&OnPaintClipboard },
#define ON_WM_VSCROLLCLIPBOARD() \
  { WM_VSCROLLCLIPBOARD, 0, 0, 0, AfxSig_vWww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CWnd*, UINT, UINT))&OnVScrollClipboard },
#define ON_WM_CONTEXTMENU() \
  { WM_CONTEXTMENU, 0, 0, 0, AfxSig_vWp, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CWnd*, CPoint))&OnContextMenu },
#define ON_WM_SIZECLIPBOARD() \
  { WM_SIZECLIPBOARD, 0, 0, 0, AfxSig_vWh, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CWnd*, HGLOBAL))&OnSizeClipboard },
#define ON_WM_ASKCBFORMATNAME() \
  { WM_ASKCBFORMATNAME, 0, 0, 0, AfxSig_vwl, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, LPTSTR))&OnAskCbFormatName },
#define ON_WM_CHANGECBCHAIN() \
  { WM_CHANGECBCHAIN, 0, 0, 0, AfxSig_vhh, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(HWND, HWND))&OnChangeCbChain },
#define ON_WM_HSCROLLCLIPBOARD() \
  { WM_HSCROLLCLIPBOARD, 0, 0, 0, AfxSig_vWww, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CWnd*, UINT, UINT))&OnHScrollClipboard },
#define ON_WM_QUERYNEWPALETTE() \
  { WM_QUERYNEWPALETTE, 0, 0, 0, AfxSig_bv, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(void))&OnQueryNewPalette },
#define ON_WM_PALETTECHANGED() \
  { WM_PALETTECHANGED, 0, 0, 0, AfxSig_vW, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CWnd*))&OnPaletteChanged },

#define ON_WM_PALETTEISCHANGING() \
  { WM_PALETTEISCHANGING, 0, 0, 0, AfxSig_vW, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CWnd*))&OnPaletteIsChanging },
#define ON_WM_DROPFILES() \
  { WM_DROPFILES, 0, 0, 0, AfxSig_vh, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(HDROP))&OnDropFiles },
#define ON_WM_WINDOWPOSCHANGING() \
  { WM_WINDOWPOSCHANGING, 0, 0, 0, AfxSig_vPOS, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(WINDOWPOS*))&OnWindowPosChanging },
#define ON_WM_WINDOWPOSCHANGED() \
  { WM_WINDOWPOSCHANGED, 0, 0, 0, AfxSig_vPOS, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(WINDOWPOS*))&OnWindowPosChanged },

#define ON_WM_EXITMENULOOP() \
  { WM_EXITMENULOOP, 0, 0, 0, AfxSig_vb, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(BOOL))&OnExitMenuLoop },
#define ON_WM_ENTERMENULOOP() \
  { WM_ENTERMENULOOP, 0, 0, 0, AfxSig_vb, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(BOOL))&OnEnterMenuLoop },

#define ON_WM_STYLECHANGED() \
  { WM_STYLECHANGED, 0, 0, 0, AfxSig_viSS, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(int, LPSTYLESTRUCT))&OnStyleChanged },
#define ON_WM_STYLECHANGING() \
  { WM_STYLECHANGING, 0, 0, 0, AfxSig_viSS, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(int, LPSTYLESTRUCT))&OnStyleChanging },
#define ON_WM_SIZING() \
  { WM_SIZING, 0, 0, 0, AfxSig_vwSIZING, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, LPRECT))&OnSizing },
#define ON_WM_MOVING() \
  { WM_MOVING, 0, 0, 0, AfxSig_vwMOVING, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, LPRECT))&OnMoving },
#define ON_WM_CAPTURECHANGED() \
  { WM_CAPTURECHANGED, 0, 0, 0, AfxSig_vW2, \
    (AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(CWnd*))&OnCaptureChanged },
#define ON_WM_DEVICECHANGE() \
  { WM_DEVICECHANGE, 0, 0, 0, AfxSig_bwl, \
    (AFX_PMSG)(AFX_PMSGW)(BOOL (AFX_MSG_CALL CWnd::*)(UINT, DWORD))&OnDeviceChange },

/////////////////////////////////////////////////////////////////////////////
// Message map tables for Control Notification messages

// Static control notification codes
#define ON_STN_CLICKED(id, memberFxn) \
  ON_CONTROL(STN_CLICKED, id, memberFxn)
#define ON_STN_DBLCLK(id, memberFxn) \
  ON_CONTROL(STN_DBLCLK, id, memberFxn)
#define ON_STN_ENABLE(id, memberFxn) \
  ON_CONTROL(STN_ENABLE, id, memberFxn)
#define ON_STN_DISABLE(id, memberFxn) \
  ON_CONTROL(STN_DISABLE, id, memberFxn)


// Edit Control Notification Codes
#define ON_EN_SETFOCUS(id, memberFxn) \
  ON_CONTROL(EN_SETFOCUS, id, memberFxn)
#define ON_EN_KILLFOCUS(id, memberFxn) \
  ON_CONTROL(EN_KILLFOCUS, id, memberFxn)
#define ON_EN_CHANGE(id, memberFxn) \
  ON_CONTROL(EN_CHANGE, id, memberFxn)
#define ON_EN_UPDATE(id, memberFxn) \
  ON_CONTROL(EN_UPDATE, id, memberFxn)
#define ON_EN_ERRSPACE(id, memberFxn) \
  ON_CONTROL(EN_ERRSPACE, id, memberFxn)
#define ON_EN_MAXTEXT(id, memberFxn) \
  ON_CONTROL(EN_MAXTEXT, id, memberFxn)
#define ON_EN_HSCROLL(id, memberFxn) \
  ON_CONTROL(EN_HSCROLL, id, memberFxn)
#define ON_EN_VSCROLL(id, memberFxn) \
  ON_CONTROL(EN_VSCROLL, id, memberFxn)

// User Button Notification Codes
#define ON_BN_CLICKED(id, memberFxn) \
  ON_CONTROL(BN_CLICKED, id, memberFxn)
#define ON_BN_DOUBLECLICKED(id, memberFxn) \
  ON_CONTROL(BN_DOUBLECLICKED, id, memberFxn)
#define ON_BN_SETFOCUS(id, memberFxn) \
  ON_CONTROL(BN_SETFOCUS, id, memberFxn)
#define ON_BN_KILLFOCUS(id, memberFxn) \
  ON_CONTROL(BN_KILLFOCUS, id, memberFxn)

// old BS_USERBUTTON button notifications - obsolete in Win31
#define ON_BN_PAINT(id, memberFxn) \
  ON_CONTROL(BN_PAINT, id, memberFxn)
#define ON_BN_HILITE(id, memberFxn) \
  ON_CONTROL(BN_HILITE, id, memberFxn)
#define ON_BN_UNHILITE(id, memberFxn) \
  ON_CONTROL(BN_UNHILITE, id, memberFxn)
#define ON_BN_DISABLE(id, memberFxn) \
  ON_CONTROL(BN_DISABLE, id, memberFxn)

// Listbox Notification Codes
#define ON_LBN_ERRSPACE(id, memberFxn) \
  ON_CONTROL(LBN_ERRSPACE, id, memberFxn)
#define ON_LBN_SELCHANGE(id, memberFxn) \
  ON_CONTROL(LBN_SELCHANGE, id, memberFxn)
#define ON_LBN_DBLCLK(id, memberFxn) \
  ON_CONTROL(LBN_DBLCLK, id, memberFxn)
#define ON_LBN_SELCANCEL(id, memberFxn) \
  ON_CONTROL(LBN_SELCANCEL, id, memberFxn)
#define ON_LBN_SETFOCUS(id, memberFxn) \
  ON_CONTROL(LBN_SETFOCUS, id, memberFxn)
#define ON_LBN_KILLFOCUS(id, memberFxn) \
  ON_CONTROL(LBN_KILLFOCUS, id, memberFxn)

// Check Listbox Notification codes
#define CLBN_CHKCHANGE (40)
#define ON_CLBN_CHKCHANGE(id, memberFxn) \
  ON_CONTROL(CLBN_CHKCHANGE, id, memberFxn)

// Combo Box Notification Codes
#define ON_CBN_ERRSPACE(id, memberFxn) \
  ON_CONTROL(CBN_ERRSPACE, id, memberFxn)
#define ON_CBN_SELCHANGE(id, memberFxn) \
  ON_CONTROL(CBN_SELCHANGE, id, memberFxn)
#define ON_CBN_DBLCLK(id, memberFxn) \
  ON_CONTROL(CBN_DBLCLK, id, memberFxn)
#define ON_CBN_SETFOCUS(id, memberFxn) \
  ON_CONTROL(CBN_SETFOCUS, id, memberFxn)
#define ON_CBN_KILLFOCUS(id, memberFxn) \
  ON_CONTROL(CBN_KILLFOCUS, id, memberFxn)
#define ON_CBN_EDITCHANGE(id, memberFxn) \
  ON_CONTROL(CBN_EDITCHANGE, id, memberFxn)
#define ON_CBN_EDITUPDATE(id, memberFxn) \
  ON_CONTROL(CBN_EDITUPDATE, id, memberFxn)
#define ON_CBN_DROPDOWN(id, memberFxn) \
  ON_CONTROL(CBN_DROPDOWN, id, memberFxn)
#define ON_CBN_CLOSEUP(id, memberFxn)  \
  ON_CONTROL(CBN_CLOSEUP, id, memberFxn)
#define ON_CBN_SELENDOK(id, memberFxn)  \
  ON_CONTROL(CBN_SELENDOK, id, memberFxn)
#define ON_CBN_SELENDCANCEL(id, memberFxn)  \
  ON_CONTROL(CBN_SELENDCANCEL, id, memberFxn)

/////////////////////////////////////////////////////////////////////////////
// User extensions for message map entries

// for Windows messages
#define ON_MESSAGE(message, memberFxn) \
  { message, 0, 0, 0, AfxSig_lwl, \
    (AFX_PMSG)(AFX_PMSGW)(LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))&memberFxn },

// for Registered Windows messages
#define ON_REGISTERED_MESSAGE(nMessageVariable, memberFxn) \
  { 0xC000, 0, 0, 0, (UINT)(UINT*)(&nMessageVariable), \
    /*implied 'AfxSig_lwl'*/ \
    (AFX_PMSG)(AFX_PMSGW)(LRESULT (AFX_MSG_CALL CWnd::*)(WPARAM, LPARAM))&memberFxn },

// for Thread messages
#define ON_THREAD_MESSAGE(message, memberFxn) \
  { message, 0, 0, 0, AfxSig_vwl, \
    (AFX_PMSG)(AFX_PMSGT)(void (AFX_MSG_CALL CWinThread::*)(WPARAM, LPARAM))&memberFxn },

// for Registered Windows messages
#define ON_REGISTERED_THREAD_MESSAGE(nMessageVariable, memberFxn) \
  { 0xC000, 0, 0, 0, (UINT)(UINT*)(&nMessageVariable), \
    /*implied 'AfxSig_vwl'*/ \
    (AFX_PMSG)(AFX_PMSGT)(void (AFX_MSG_CALL CWinThread::*)(WPARAM, LPARAM))&memberFxn },

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Dialog data exchange (DDX_) and validation (DDV_)
// CDataExchange - for data exchange and validation
class CDataExchange
{
  // Attributes
public:
  BOOL m_bSaveAndValidate;   // TRUE => save and validate data
  CWnd* m_pDlgWnd;           // container usually a dialog
  // Operations (for implementors of DDX and DDV procs)
  HWID PrepareCtrl(int nIDC);     // return HWID of control
  HWID PrepareEditCtrl(int nIDC); // return HWID of control
  void Fail();                    // will throw exception
#ifndef _AFX_NO_OCC_SUPPORT
  CWnd* PrepareOleCtrl(int nIDC); // for OLE controls in dialog
#endif
  // Implementation
  CDataExchange(CWnd* pDlgWnd, BOOL bSaveAndValidate);
  HWID m_hWndLastControl;    // last control used (for validation)
  BOOL m_bEditLastControl;   // last control was an edit item
};
// standard DDX_ and DDV_ routines
/////////////////////////////////////////////////////////////////////////////
class COleCurrency;    // forward reference (see afxdisp.h)
class COleDateTime;    // forward reference (see afxdisp.h)

// simple text operations
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, BYTE& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, short& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, int& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, UINT& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, long& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, DWORD& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, CString& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, LPTSTR value, int nMaxLen);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, float& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, double& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, COleCurrency& value);
void AFXAPI DDX_Text(CDataExchange* pDX, int nIDC, COleDateTime& value);

// special control types
void AFXAPI DDX_Check(CDataExchange* pDX, int nIDC, int& value);
void AFXAPI DDX_Radio(CDataExchange* pDX, int nIDC, int& value);
void AFXAPI DDX_LBString(CDataExchange* pDX, int nIDC, CString& value);
void AFXAPI DDX_CBString(CDataExchange* pDX, int nIDC, CString& value);
void AFXAPI DDX_LBIndex(CDataExchange* pDX, int nIDC, int& index);
void AFXAPI DDX_CBIndex(CDataExchange* pDX, int nIDC, int& index);
void AFXAPI DDX_LBStringExact(CDataExchange* pDX, int nIDC, CString& value);
void AFXAPI DDX_CBStringExact(CDataExchange* pDX, int nIDC, CString& value);
void AFXAPI DDX_Scroll(CDataExchange* pDX, int nIDC, int& value);
void AFXAPI DDX_Slider(CDataExchange* pDX, int nIDC, int& value);

void AFXAPI DDX_MonthCalCtrl(CDataExchange* pDX, int nIDC, CTime& value);
void AFXAPI DDX_MonthCalCtrl(CDataExchange* pDX, int nIDC, COleDateTime& value);
void AFXAPI DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, CTime& value);
void AFXAPI DDX_DateTimeCtrl(CDataExchange* pDX, int nIDC, COleDateTime& value);

// for getting access to the actual controls
void AFXAPI DDX_Control(CDataExchange* pDX, int nIDC, CWnd& rControl);

/////////////////////////////////////////////////////////////////////////////
// Standard Dialog Data Validation routines

// range - value must be >= minVal and <= maxVal
// NOTE: you will require casts for 'minVal' and 'maxVal' to use the
//   UINT, DWORD or float types
void AFXAPI DDV_MinMaxByte(CDataExchange* pDX, BYTE value, BYTE minVal, BYTE maxVal);
void AFXAPI DDV_MinMaxShort(CDataExchange* pDX, short value, short minVal, short maxVal);
void AFXAPI DDV_MinMaxInt(CDataExchange* pDX, int value, int minVal, int maxVal);
void AFXAPI DDV_MinMaxLong(CDataExchange* pDX, long value, long minVal, long maxVal);
void AFXAPI DDV_MinMaxUInt(CDataExchange* pDX, UINT value, UINT minVal, UINT maxVal);
void AFXAPI DDV_MinMaxDWord(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
void AFXAPI DDV_MinMaxFloat(CDataExchange* pDX, float const& value, float minVal, float maxVal);
void AFXAPI DDV_MinMaxDouble(CDataExchange* pDX, double const& value, double minVal, double maxVal);

// special control types
void AFXAPI DDV_MinMaxSlider(CDataExchange* pDX, DWORD value, DWORD minVal, DWORD maxVal);
void AFXAPI DDV_MinMaxDateTime(CDataExchange* pDX, CTime& refValue, const CTime* refMinRange, const CTime* refMaxRange);
void AFXAPI DDV_MinMaxDateTime(CDataExchange* pDX, COleDateTime& refValue, const COleDateTime* refMinRange, const COleDateTime* refMaxRange);
void AFXAPI DDV_MinMaxMonth(CDataExchange* pDX, CTime& refValue, const CTime* pMinRange, const CTime* pMaxRange);
void AFXAPI DDV_MinMaxMonth(CDataExchange* pDX, COleDateTime& refValue, const COleDateTime* refMinRange, const COleDateTime* refMaxRange);


// number of characters
void AFXAPI DDV_MaxChars(CDataExchange* pDX, CString const& value, int nChars);

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// OLE types
typedef LONG HRESULT;
struct IUnknown;
typedef IUnknown* LPUNKNOWN;
struct IDispatch;
typedef IDispatch* LPDISPATCH;
struct IConnectionPoint;
typedef IConnectionPoint* LPCONNECTIONPOINT;
struct IEnumOLEVERB;
typedef IEnumOLEVERB* LPENUMOLEVERB;
typedef struct _GUID GUID;
typedef GUID IID;
typedef GUID CLSID;
#ifndef _REFCLSID_DEFINED
#define REFCLSID const CLSID &
#endif
typedef long DISPID;
typedef unsigned short VARTYPE;
typedef long SCODE;
#if defined(WIN32) && !defined(OLE2ANSI)
typedef WCHAR OLECHAR;
#else
typedef char OLECHAR;
#endif
typedef OLECHAR* BSTR;
struct tagDISPPARAMS;
typedef tagDISPPARAMS DISPPARAMS;
struct tagVARIANT;
typedef tagVARIANT VARIANT;
struct ITypeInfo;
typedef ITypeInfo* LPTYPEINFO;
struct ITypeLib;
typedef ITypeLib* LPTYPELIB;
/////////////////////////////////////////////////////////////////////////////
// CCmdTarget
// private structures
struct AFX_CMDHANDLERINFO;  // info about where the command is handled
struct AFX_EVENT;           // info about an event
class CTypeLibCache;        // cache for OLE type libraries
/////////////////////////////////////////////////////////////////////////////
// OLE interface map handling (more in AFXDISP.H)
#ifndef _AFX_NO_OLE_SUPPORT
struct AFX_INTERFACEMAP_ENTRY {
  const void* piid;       // the interface id (IID) (NULL for aggregate)
  size_t nOffset;         // offset of the interface vtable from m_unknown
};
struct AFX_INTERFACEMAP {
#ifdef _AFXDLL
  const AFX_INTERFACEMAP* (PASCAL* pfnGetBaseMap)(); // NULL is root class
#else
  const AFX_INTERFACEMAP* pBaseMap;
#endif
  const AFX_INTERFACEMAP_ENTRY* pEntry; // map for this class
};
#ifdef _AFXDLL
#define DECLARE_INTERFACE_MAP() \
  private: \
  static const AFX_INTERFACEMAP_ENTRY _interfaceEntries[]; \
  protected: \
  static AFX_DATA const AFX_INTERFACEMAP interfaceMap; \
  static const AFX_INTERFACEMAP* PASCAL _GetBaseInterfaceMap(); \
  virtual const AFX_INTERFACEMAP* GetInterfaceMap() const;
#else
#define DECLARE_INTERFACE_MAP() \
  private: \
  static const AFX_INTERFACEMAP_ENTRY _interfaceEntries[]; \
  protected: \
  static AFX_DATA const AFX_INTERFACEMAP interfaceMap; \
  virtual const AFX_INTERFACEMAP* GetInterfaceMap() const;
#endif
#endif //!_AFX_NO_OLE_SUPPORT
/////////////////////////////////////////////////////////////////////////////
// OLE dispatch map handling (more in AFXDISP.H)
#ifndef _AFX_NO_OLE_SUPPORT
struct AFX_DISPMAP_ENTRY;
struct AFX_DISPMAP {
#ifdef _AFXDLL
  const AFX_DISPMAP* (PASCAL* pfnGetBaseMap)();
#else
  const AFX_DISPMAP* pBaseMap;
#endif
  const AFX_DISPMAP_ENTRY* lpEntries;
  UINT* lpEntryCount;
  DWORD* lpStockPropMask;
};
#ifdef _AFXDLL
#define DECLARE_DISPATCH_MAP() \
  private: \
  static const AFX_DISPMAP_ENTRY _dispatchEntries[]; \
  static UINT _dispatchEntryCount; \
  static DWORD _dwStockPropMask; \
  protected: \
  static AFX_DATA const AFX_DISPMAP dispatchMap; \
  static const AFX_DISPMAP* PASCAL _GetBaseDispatchMap(); \
  virtual const AFX_DISPMAP* GetDispatchMap() const;
#else
#define DECLARE_DISPATCH_MAP() \
  private: \
  static const AFX_DISPMAP_ENTRY _dispatchEntries[]; \
  static UINT _dispatchEntryCount; \
  static DWORD _dwStockPropMask; \
  protected: \
  static AFX_DATA const AFX_DISPMAP dispatchMap; \
  virtual const AFX_DISPMAP* GetDispatchMap() const;
#endif
#endif //!_AFX_NO_OLE_SUPPORT
/////////////////////////////////////////////////////////////////////////////
// OLE Document Object command target handling
#ifndef _AFX_NO_DOCOBJECT_SUPPORT
struct AFX_OLECMDMAP_ENTRY {
  const GUID* pguid;   // id of the command group
  ULONG       cmdID;   // OLECMD ID
  UINT        nID;     // corresponding WM_COMMAND message ID
};
struct AFX_OLECMDMAP {
#ifdef _AFXDLL
  const AFX_OLECMDMAP* (PASCAL* pfnGetBaseMap)();
#else
  const AFX_OLECMDMAP* pBaseMap;
#endif
  const AFX_OLECMDMAP_ENTRY* lpEntries;
};
#ifdef _AFXDLL
#define DECLARE_OLECMD_MAP() \
  private: \
  static const AFX_OLECMDMAP_ENTRY _commandEntries[]; \
  protected: \
  static AFX_DATA const AFX_OLECMDMAP commandMap; \
  static const AFX_OLECMDMAP* PASCAL _GetBaseCommandMap(); \
  virtual const AFX_OLECMDMAP* GetCommandMap() const;
#else
#define DECLARE_OLECMD_MAP() \
  private: \
  static const AFX_OLECMDMAP_ENTRY _commandEntries[]; \
  protected: \
  static AFX_DATA const AFX_OLECMDMAP commandMap; \
  virtual const AFX_OLECMDMAP* GetCommandMap() const;
#endif
#ifdef _AFXDLL
#define BEGIN_OLECMD_MAP(theClass, baseClass) \
  const AFX_OLECMDMAP* PASCAL theClass::_GetBaseCommandMap() \
  { return &baseClass::commandMap; } \
  const AFX_OLECMDMAP* theClass::GetCommandMap() const \
  { return &theClass::commandMap; } \
  AFX_COMDAT AFX_DATADEF const AFX_OLECMDMAP theClass::commandMap = \
      { &theClass::_GetBaseCommandMap, &theClass::_commandEntries[0] }; \
  AFX_COMDAT const AFX_OLECMDMAP_ENTRY theClass::_commandEntries[] = {
#else
#define BEGIN_OLECMD_MAP(theClass, baseClass) \
  const AFX_OLECMDMAP* theClass::GetCommandMap() const \
  { return &theClass::commandMap; } \
  AFX_COMDAT AFX_DATADEF const AFX_OLECMDMAP theClass::commandMap = \
      { &baseClass::commandMap, &theClass::_commandEntries[0] }; \
  AFX_COMDAT const AFX_OLECMDMAP_ENTRY theClass::_commandEntries[] = {
#endif
#define END_OLECMD_MAP()  {NULL, 0, 0}  };
class COleCmdUI;
#endif //!_AFX_NO_DOCOBJECT_SUPPORT
/////////////////////////////////////////////////////////////////////////////
// OLE event sink map handling (more in AFXDISP.H)
#ifndef _AFX_NO_OCC_SUPPORT
struct AFX_EVENTSINKMAP_ENTRY;
struct AFX_EVENTSINKMAP {
#ifdef _AFXDLL
  const AFX_EVENTSINKMAP* (PASCAL* pfnGetBaseMap)();
#else
  const AFX_EVENTSINKMAP* pBaseMap;
#endif
  const AFX_EVENTSINKMAP_ENTRY* lpEntries;
  UINT* lpEntryCount;
};
#ifdef _AFXDLL
#define DECLARE_EVENTSINK_MAP() \
  private: \
  static const AFX_EVENTSINKMAP_ENTRY _eventsinkEntries[]; \
  static UINT _eventsinkEntryCount; \
  protected: \
  static AFX_DATA const AFX_EVENTSINKMAP eventsinkMap; \
  static const AFX_EVENTSINKMAP* PASCAL _GetBaseEventSinkMap(); \
  virtual const AFX_EVENTSINKMAP* GetEventSinkMap() const;
#else
#define DECLARE_EVENTSINK_MAP() \
  private: \
  static const AFX_EVENTSINKMAP_ENTRY _eventsinkEntries[]; \
  static UINT _eventsinkEntryCount; \
  protected: \
  static AFX_DATA const AFX_EVENTSINKMAP eventsinkMap; \
  virtual const AFX_EVENTSINKMAP* GetEventSinkMap() const;
#endif
#endif //!_AFX_NO_OCC_SUPPORT
/////////////////////////////////////////////////////////////////////////////
// OLE connection map handling (more in AFXDISP.H)
#ifndef _AFX_NO_OLE_SUPPORT
struct AFX_CONNECTIONMAP_ENTRY {
  const void* piid;   // the interface id (IID)
  size_t nOffset;         // offset of the interface vtable from m_unknown
};
struct AFX_CONNECTIONMAP {
#ifdef _AFXDLL
  const AFX_CONNECTIONMAP* (PASCAL* pfnGetBaseMap)(); // NULL is root class
#else
  const AFX_CONNECTIONMAP* pBaseMap;
#endif
  const AFX_CONNECTIONMAP_ENTRY* pEntry; // map for this class
};
#ifdef _AFXDLL
#define DECLARE_CONNECTION_MAP() \
  private: \
  static const AFX_CONNECTIONMAP_ENTRY _connectionEntries[]; \
  protected: \
  static AFX_DATA const AFX_CONNECTIONMAP connectionMap; \
  static const AFX_CONNECTIONMAP* PASCAL _GetBaseConnectionMap(); \
  virtual const AFX_CONNECTIONMAP* GetConnectionMap() const;
#else
#define DECLARE_CONNECTION_MAP() \
  private: \
  static const AFX_CONNECTIONMAP_ENTRY _connectionEntries[]; \
  protected: \
  static AFX_DATA const AFX_CONNECTIONMAP connectionMap; \
  virtual const AFX_CONNECTIONMAP* GetConnectionMap() const;
#endif
#endif //!_AFX_NO_OLE_SUPPORT
/////////////////////////////////////////////////////////////////////////////
// CCmdTarget proper
#ifndef _AFX_NO_OCC_SUPPORT
class COccManager;      // forward reference (see ..\src\occimpl.h)
#endif
#ifdef _AFXDLL
class CCmdTarget : public CObject
#else
class AFX_NOVTABLE CCmdTarget : public CObject
#endif
{
  DECLARE_DYNAMIC(CCmdTarget)
protected:
public:
  // Constructors
  CCmdTarget();
  // Attributes
  LPDISPATCH GetIDispatch(BOOL bAddRef);
  // retrieve IDispatch part of CCmdTarget
  static CCmdTarget* PASCAL FromIDispatch(LPDISPATCH lpDispatch);
  // map LPDISPATCH back to CCmdTarget* (inverse of GetIDispatch)
  BOOL IsResultExpected();
  // returns TRUE if automation function should return a value
  // Operations
  void EnableAutomation();
  // call in constructor to wire up IDispatch
  void EnableConnections();
  // call in constructor to wire up IConnectionPointContainer
  void BeginWaitCursor();
  void EndWaitCursor();
  void RestoreWaitCursor();       // call after messagebox
#ifndef _AFX_NO_OLE_SUPPORT
  // dispatch OLE verbs through the message map
  BOOL EnumOleVerbs(LPENUMOLEVERB* ppenumOleVerb);
  BOOL DoOleVerb(LONG iVerb, LPMSG lpMsg, HWID hWndParent, LPCRECT lpRect);
#endif
  // Overridables
  // route and dispatch standard command message types
  //   (more sophisticated than OnCommand)
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
      AFX_CMDHANDLERINFO* pHandlerInfo);
#ifndef _AFX_NO_OLE_SUPPORT
  // called when last OLE reference is released
  virtual void OnFinalRelease();
#endif
#ifndef _AFX_NO_OLE_SUPPORT
  // called before dispatching to an automation handler function
  virtual BOOL IsInvokeAllowed(DISPID dispid);
#endif
#ifndef _AFX_NO_OLE_SUPPORT
  // support for OLE type libraries
  void EnableTypeLib();
  HRESULT GetTypeInfoOfGuid(LCID lcid, const GUID& guid,
      LPTYPEINFO* ppTypeInfo);
  virtual BOOL GetDispatchIID(IID* pIID);
  virtual UINT GetTypeInfoCount();
  virtual CTypeLibCache* GetTypeLibCache();
  virtual HRESULT GetTypeLib(LCID lcid, LPTYPELIB* ppTypeLib);
#endif
  // Implementation
public:
  virtual ~CCmdTarget();
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
  virtual void AssertValid() const;
#endif
#ifndef _AFX_NO_OLE_SUPPORT
  void GetNotSupported();
  void SetNotSupported();
#endif
protected:
  friend class CView;
  CView* GetRoutingView();
  CFrameWnd* GetRoutingFrame();
  static CView* PASCAL GetRoutingView_();
  static CFrameWnd* PASCAL GetRoutingFrame_();
  DECLARE_MESSAGE_MAP()       // base class - no {{ }} macros
#ifndef _AFX_NO_DOCOBJECT_SUPPORT
  DECLARE_OLECMD_MAP()
  friend class COleCmdUI;
#endif
#ifndef _AFX_NO_OLE_SUPPORT
  DECLARE_DISPATCH_MAP()
  DECLARE_CONNECTION_MAP()
  DECLARE_INTERFACE_MAP()
#ifndef _AFX_NO_OCC_SUPPORT
  DECLARE_EVENTSINK_MAP()
#endif // !_AFX_NO_OCC_SUPPORT
  // OLE interface map implementation
public:
  // data used when CCmdTarget is made OLE aware
  long m_dwRef;
  LPUNKNOWN m_pOuterUnknown;  // external controlling unknown if != NULL
  DWORD m_xInnerUnknown;  // place-holder for inner controlling unknown
public:
  // advanced operations
  void EnableAggregation();       // call to enable aggregation
  void ExternalDisconnect();      // forcibly disconnect
  LPUNKNOWN GetControllingUnknown();
  // get controlling IUnknown for aggregate creation
  // these versions do not delegate to m_pOuterUnknown
  DWORD InternalQueryInterface(const void*, LPVOID* ppvObj);
  DWORD InternalAddRef();
  DWORD InternalRelease();
  // these versions delegate to m_pOuterUnknown
  DWORD ExternalQueryInterface(const void*, LPVOID* ppvObj);
  DWORD ExternalAddRef();
  DWORD ExternalRelease();
  // implementation helpers
  LPUNKNOWN GetInterface(const void*);
  LPUNKNOWN QueryAggregates(const void*);
  // advanced overrideables for implementation
  virtual BOOL OnCreateAggregates();
  virtual LPUNKNOWN GetInterfaceHook(const void*);
  // OLE automation implementation
protected:
  struct XDispatch {
    DWORD m_vtbl;   // place-holder for IDispatch vtable
#ifndef _AFX_NO_NESTED_DERIVATION
    size_t m_nOffset;
#endif
  } m_xDispatch;
  BOOL m_bResultExpected;
  // member variable-based properties
  void GetStandardProp(const AFX_DISPMAP_ENTRY* pEntry,
      VARIANT* pvarResult, UINT* puArgErr);
  SCODE SetStandardProp(const AFX_DISPMAP_ENTRY* pEntry,
      DISPPARAMS* pDispParams, UINT* puArgErr);
  // DISPID to dispatch map lookup
  static UINT PASCAL GetEntryCount(const AFX_DISPMAP* pDispMap);
  const AFX_DISPMAP_ENTRY* PASCAL GetDispEntry(LONG memid);
  static LONG PASCAL MemberIDFromName(const AFX_DISPMAP* pDispMap, LPCTSTR lpszName);
  // helpers for member function calling implementation
  static UINT PASCAL GetStackSize(const BYTE* pbParams, VARTYPE vtResult);
#ifdef _PPC_
  SCODE PushStackArgs(BYTE* pStack, const BYTE* pbParams,
      void* pResult, VARTYPE vtResult, DISPPARAMS* pDispParams,
      UINT* puArgErr, VARIANT* rgTempVars, UINT nSizeArgs);
#else
  SCODE PushStackArgs(BYTE* pStack, const BYTE* pbParams,
      void* pResult, VARTYPE vtResult, DISPPARAMS* pDispParams,
      UINT* puArgErr, VARIANT* rgTempVars);
#endif
  SCODE CallMemberFunc(const AFX_DISPMAP_ENTRY* pEntry, WORD wFlags,
      VARIANT* pvarResult, DISPPARAMS* pDispParams, UINT* puArgErr);
  friend class COleDispatchImpl;
#ifndef _AFX_NO_OCC_SUPPORT
public:
  // OLE event sink implementation
  BOOL OnEvent(UINT idCtrl, AFX_EVENT* pEvent,
      AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
  const AFX_EVENTSINKMAP_ENTRY* PASCAL GetEventSinkEntry(UINT idCtrl,
      AFX_EVENT* pEvent);
#endif // !_AFX_NO_OCC_SUPPORT
  // OLE connection implementation
  struct XConnPtContainer {
    DWORD m_vtbl;   // place-holder for IConnectionPointContainer vtable
#ifndef _AFX_NO_NESTED_DERIVATION
    size_t m_nOffset;
#endif
  } m_xConnPtContainer;
#ifdef _AFXDLL
  AFX_MODULE_STATE* m_pModuleState;
  friend class CInnerUnknown;
  friend UINT APIENTRY _AfxThreadEntry(void* pParam);
#endif
  virtual BOOL GetExtraConnectionPoints(CPtrArray* pConnPoints);
  virtual LPCONNECTIONPOINT GetConnectionHook(const IID& iid);
  friend class COleConnPtContainer;
#endif //!_AFX_NO_OLE_SUPPORT
};
class CCmdUI        // simple helper class
{
public:
  // Attributes
  UINT m_nID;
  UINT m_nIndex;          // menu item or other index
  // if a menu item
  CMenu* m_pMenu;         // NULL if not a menu
  CMenu* m_pSubMenu;      // sub containing menu item
  // if a popup sub menu - ID is for first in popup
  // if from some other window
  CWnd* m_pOther;         // NULL if a menu or not a CWnd
  // Operations to do in ON_UPDATE_COMMAND_UI
  virtual void Enable(BOOL bOn = TRUE);
  virtual void SetCheck(int nCheck = 1);   // 0, 1 or 2 (indeterminate)
  virtual void SetRadio(BOOL bOn = TRUE);
  virtual void SetText(LPCTSTR lpszText);
  // Advanced operation
  void ContinueRouting();
  // Implementation
  CCmdUI();
  BOOL m_bEnableChanged;
  BOOL m_bContinueRouting;
  UINT m_nIndexMax;       // last + 1 for iterating m_nIndex
  CMenu* m_pParentMenu;   // NULL if parent menu not easily determined
  //  (probably a secondary popup menu)
  BOOL DoUpdate(CCmdTarget* pTarget, BOOL bDisableIfNoHndler);
};
// special CCmdUI derived classes are used for other UI paradigms
//  like toolbar buttons and status indicators
// pointer to afx_msg member function
#ifndef AFX_MSG_CALL
#define AFX_MSG_CALL
#endif
typedef void (AFX_MSG_CALL CCmdTarget::*AFX_PMSG)(void);
enum AFX_DISPMAP_FLAGS {
  afxDispCustom = 0,
  afxDispStock = 1
};
struct AFX_DISPMAP_ENTRY {
  LPCTSTR lpszName;       // member/property name
  long lDispID;           // DISPID (may be DISPID_UNKNOWN)
  LPCSTR lpszParams;      // member parameter description
  WORD vt;                // return value type / or type of property
  AFX_PMSG pfn;           // normal member On<membercall> or, OnGet<property>
  AFX_PMSG pfnSet;        // special member for OnSet<property>
  size_t nPropOffset;     // property offset
  AFX_DISPMAP_FLAGS flags;// flags (e.g. stock/custom)
};
struct AFX_EVENTSINKMAP_ENTRY {
  AFX_DISPMAP_ENTRY dispEntry;
  UINT nCtrlIDFirst;
  UINT nCtrlIDLast;
};
// DSC Sink state/reason codes passed to MFC user event handlers
enum DSCSTATE {
  dscNoState = 0,
  dscOKToDo,
  dscCancelled,
  dscSyncBefore,
  dscAboutToDo,
  dscFailedToDo,
  dscSyncAfter,
  dscDidEvent
};
enum DSCREASON {
  dscNoReason = 0,
  dscClose,
  dscCommit,
  dscDelete,
  dscEdit,
  dscInsert,
  dscModify,
  dscMove
};
/////////////////////////////////////////////////////////////////////////////
// CWnd implementation
// structures (see afxext.h)
struct CCreateContext;      // context for creating things
struct CPrintInfo;          // print preview customization info
struct AFX_MSGMAP_ENTRY {
  UINT nMessage;   // windows message
  UINT nCode;      // control code or WM_NOTIFY code
  UINT nID;        // control ID (or 0 for windows messages)
  UINT nLastID;    // used for entries specifying a range of control id's
  UINT nSig;       // signature type (action) or pointer to message #
  AFX_PMSG pfn;    // routine to call (or special value)
};
class CWinThread;       // forward reference for friend declaration
class CHandleMap
{
private:    // implementation
  CMapPtrToPtr m_permanentMap;
  CMapPtrToPtr m_temporaryMap;
  CRuntimeClass*  m_pClass;
  size_t m_nOffset;       // offset of handles in the object
  int m_nHandles;         // 1 or 2 (for CDC)
  // Constructor/Destructor
public:
  CHandleMap(CRuntimeClass* pClass, size_t nOffset, int nHandles = 1);
#ifdef _AFXDLL
  ~CHandleMap()
#else
  virtual ~CHandleMap()
#endif
  {
    DeleteTemp();
  }
  // Operations
public:
  CObject* FromHandle(HANDLE h);
  void DeleteTemp();
  void SetPermanent(HANDLE h, CObject* permOb);
  void RemoveHandle(HANDLE h);
  CObject* LookupPermanent(HANDLE h);
  CObject* LookupTemporary(HANDLE h);
  friend class CWinThread;
};
// Note: out-of-line _DEBUG version is in winhand.cpp
#ifndef _DEBUG
inline void CHandleMap::SetPermanent(HANDLE h, CObject* permOb)
{
  m_permanentMap[(LPVOID)h] = permOb;
}
inline void CHandleMap::RemoveHandle(HANDLE h)
{
  // remove only from permanent map -- temporary objects are removed
  //  at idle in CHandleMap::DeleteTemp, always!
  m_permanentMap.RemoveKey((LPVOID)h);
}
#endif
inline CObject* CHandleMap::LookupPermanent(HANDLE h)
{
  return (CObject*)m_permanentMap.GetValueAt((LPVOID)h);
}
inline CObject* CHandleMap::LookupTemporary(HANDLE h)
{
  return (CObject*)m_temporaryMap.GetValueAt((LPVOID)h);
}
/////////////////////////////////////////////////////////////////////////////
// CWnd - a Microsoft Windows application window
class COleDropTarget;   // for more information see AFXOLE.H
class COleControlContainer;
class COleControlSite;
// CWnd::m_nFlags (generic to CWnd)
#define WF_TOOLTIPS         0x0001  // window is enabled for tooltips
#define WF_TEMPHIDE         0x0002  // window is temporarily hidden
#define WF_STAYDISABLED     0x0004  // window should stay disabled
#define WF_MODALLOOP        0x0008  // currently in modal loop
#define WF_CONTINUEMODAL    0x0010  // modal loop should continue running
#define WF_OLECTLCONTAINER  0x0100  // some descendant is an OLE control
#define WF_TRACKINGTOOLTIPS 0x0400  // window is enabled for tracking tooltips
// CWnd::m_nFlags (specific to CFrameWnd)
#define WF_STAYACTIVE       0x0020  // look active even though not active
#define WF_NOPOPMSG         0x0040  // ignore WM_POPMESSAGESTRING calls
#define WF_MODALDISABLE     0x0080  // window is disabled
#define WF_KEEPMINIACTIVE   0x0200  // stay activate even though you are deactivated
// flags for CWnd::RunModalLoop
#define MLF_NOIDLEMSG       0x0001  // don't send WM_ENTERIDLE messages
#define MLF_NOKICKIDLE      0x0002  // don't send WM_KICKIDLE messages
#define MLF_SHOWONIDLE      0x0004  // show window if not visible at idle time
// extra MFC defined TTF_ flags for TOOLINFO::uFlags
#define TTF_NOTBUTTON       0x80000000L // no status help on buttondown
#define TTF_ALWAYSTIP       0x40000000L // always show the tip even if not active
class CWnd : public CCmdTarget
{
  DECLARE_DYNCREATE(CWnd)
protected:
  static const MSG* PASCAL GetCurrentMessage();
  // Attributes
public:
  HWID m_hWnd;            // must be first data member
  operator HWID() const;
  BOOL operator==(const CWnd& wnd) const;
  BOOL operator!=(const CWnd& wnd) const;
  HWID GetSafeHwnd() const;
  DWORD GetStyle() const;
  DWORD GetExStyle() const;
  BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
  BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0);
  CWnd* GetOwner() const;
  void SetOwner(CWnd* pOwnerWnd);
  // Constructors and other creation
  CWnd();
  static CWnd* PASCAL FromHandle(HWID hWnd);
  static CWnd* PASCAL FromHandlePermanent(HWID hWnd);
  static void PASCAL DeleteTempMap();
  BOOL Attach(HWID hWndNew);
  HWID Detach();
  // subclassing/unsubclassing functions
  virtual void PreSubclassWindow();
  BOOL SubclassWindow(HWID hWnd);
  BOOL SubclassDlgItem(UINT nID, CWnd* pParent);
  HWID UnsubclassWindow();
  // handling of RT_DLGINIT resource (extension to RT_DIALOG)
  BOOL ExecuteDlgInit(LPCTSTR lpszResourceName);
  BOOL ExecuteDlgInit(LPVOID lpResource);
public:
  // for child windows, views, panes etc
  virtual BOOL Create(LPCTSTR lpszClassName,
      LPCTSTR lpszWindowName, DWORD dwStyle,
      const RECT& rect,
      CWnd* pParentWnd, UINT nID,
      CCreateContext* pContext = NULL);
  // advanced creation (allows access to extended styles)
  BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName,
      LPCTSTR lpszWindowName, DWORD dwStyle,
      int x, int y, int nWidth, int nHeight,
      HWID hWndParent, HMENU nIDorHMenu, LPVOID lpParam = NULL);
  BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName,
      LPCTSTR lpszWindowName, DWORD dwStyle,
      const RECT& rect,
      CWnd* pParentWnd, UINT nID,
      LPVOID lpParam = NULL);
#ifndef _AFX_NO_OCC_SUPPORT
  // for wrapping OLE controls
  BOOL CreateControl(REFCLSID clsid, LPCTSTR pszWindowName, DWORD dwStyle,
      const RECT& rect, CWnd* pParentWnd, UINT nID, CFile* pPersist = NULL,
      BOOL bStorage = FALSE, BSTR bstrLicKey = NULL);
  BOOL CreateControl(LPCTSTR pszClass, LPCTSTR pszWindowName, DWORD dwStyle,
      const RECT& rect, CWnd* pParentWnd, UINT nID, CFile* pPersist = NULL,
      BOOL bStorage = FALSE, BSTR bstrLicKey = NULL);
  // Another overload for creating controls that use default extents.
  BOOL CreateControl(REFCLSID clsid, LPCTSTR pszWindowName, DWORD dwStyle,
      const POINT* ppt, const SIZE* psize, CWnd* pParentWnd, UINT nID,
      CFile* pPersist = NULL, BOOL bStorage = FALSE, BSTR bstrLicKey = NULL);
  LPUNKNOWN GetControlUnknown();
#endif
  virtual BOOL DestroyWindow();
  // special pre-creation and window rect adjustment hooks
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  // Advanced: virtual AdjustWindowRect
  enum AdjustType { adjustBorder = 0, adjustOutside = 1 };
  virtual void CalcWindowRect(LPRECT lpClientRect,
      UINT nAdjustType = adjustBorder);
  // Window tree access
  int GetDlgCtrlID() const;
  int SetDlgCtrlID(int nID);
  // get and set window ID, for child windows only
  CWnd* GetDlgItem(int nID) const;
  // get immediate child with given ID
  void GetDlgItem(int nID, HWID* phWnd) const;
  // as above, but returns HWID
  CWnd* GetDescendantWindow(int nID, BOOL bOnlyPerm = FALSE) const;
  // like GetDlgItem but recursive
  void SendMessageToDescendants(UINT message, WPARAM wParam = 0,
      LPARAM lParam = 0, BOOL bDeep = TRUE, BOOL bOnlyPerm = FALSE);
  CFrameWnd* GetParentFrame() const;
  CWnd* GetTopLevelParent() const;
  CWnd* GetTopLevelOwner() const;
  CWnd* GetParentOwner() const;
  CFrameWnd* GetTopLevelFrame() const;
  static CWnd* PASCAL GetSafeOwner(CWnd* pParent = NULL, HWID* pWndTop = NULL);
  // Message Functions
  LRESULT SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
  BOOL PostMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
  BOOL SendNotifyMessage(UINT message, WPARAM wParam, LPARAM lParam);
  BOOL SendChildNotifyLastMsg(LRESULT* pResult = NULL);
  // Message processing for modeless dialog-like windows
  BOOL IsDialogMessage(LPMSG lpMsg);
  // Window Text Functions
  void SetWindowText(LPCTSTR lpszString);
  int GetWindowText(LPTSTR lpszStringBuf, int nMaxCount) const;
  void GetWindowText(CString& rString) const;
  int GetWindowTextLength() const;
  void SetFont(CFont* pFont, BOOL bRedraw = TRUE);
  CFont* GetFont() const;
  // CMenu Functions - non-Child windows only
  CMenu* GetMenu() const;
  BOOL SetMenu(CMenu* pMenu);
  void DrawMenuBar();
  CMenu* GetSystemMenu(BOOL bRevert) const;
  BOOL HiliteMenuItem(CMenu* pMenu, UINT nIDHiliteItem, UINT nHilite);
  // Window Size and Position Functions
  BOOL IsIconic() const;
  BOOL IsZoomed() const;
  void MoveWindow(int x, int y, int nWidth, int nHeight,
      BOOL bRepaint = TRUE);
  void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE);
  int SetWindowRgn(HRGN hRgn, BOOL bRedraw);
  int GetWindowRgn(HRGN hRgn) const;
  static AFX_DATA const CWnd wndTop; // SetWindowPos's pWndInsertAfter
  static AFX_DATA const CWnd wndBottom; // SetWindowPos's pWndInsertAfter
  static AFX_DATA const CWnd wndTopMost; // SetWindowPos pWndInsertAfter
  static AFX_DATA const CWnd wndNoTopMost; // SetWindowPos pWndInsertAfter
  BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y,
      int cx, int cy, UINT nFlags);
  UINT ArrangeIconicWindows();
  void BringWindowToTop();
  void GetWindowRect(LPRECT lpRect) const;
  void GetClientRect(LPRECT lpRect) const;
  BOOL GetWindowPlacement(WINDOWPLACEMENT* lpwndpl) const;
  BOOL SetWindowPlacement(const WINDOWPLACEMENT* lpwndpl);
  // Coordinate Mapping Functions
  void ClientToScreen(LPPOINT lpPoint) const;
  void ClientToScreen(LPRECT lpRect) const;
  void ScreenToClient(LPPOINT lpPoint) const;
  void ScreenToClient(LPRECT lpRect) const;
  void MapWindowPoints(CWnd* pwndTo, LPPOINT lpPoint, UINT nCount) const;
  void MapWindowPoints(CWnd* pwndTo, LPRECT lpRect) const;
  // Update/Painting Functions
  CDC* BeginPaint(LPPAINTSTRUCT lpPaint);
  void EndPaint(LPPAINTSTRUCT lpPaint);
  CDC* GetDC();
  CDC* GetWindowDC();
  int ReleaseDC(CDC* pDC);
  void Print(CDC* pDC, DWORD dwFlags) const;
  void PrintClient(CDC* pDC, DWORD dwFlags) const;
  void UpdateWindow();
  void SetRedraw(BOOL bRedraw = TRUE);
  BOOL GetUpdateRect(LPRECT lpRect, BOOL bErase = FALSE);
  int GetUpdateRgn(CRgn* pRgn, BOOL bErase = FALSE);
  void Invalidate(BOOL bErase = TRUE);
  void InvalidateRect(LPCRECT lpRect, BOOL bErase = TRUE);
  void InvalidateRgn(CRgn* pRgn, BOOL bErase = TRUE);
  void ValidateRect(LPCRECT lpRect);
  void ValidateRgn(CRgn* pRgn);
  BOOL ShowWindow(int nCmdShow);
  BOOL IsWindowVisible() const;
  void ShowOwnedPopups(BOOL bShow = TRUE);
  CDC* GetDCEx(CRgn* prgnClip, DWORD flags);
  BOOL LockWindowUpdate();    // for backward compatibility
  void UnlockWindowUpdate();
  BOOL RedrawWindow(LPCRECT lpRectUpdate = NULL,
      CRgn* prgnUpdate = NULL,
      UINT flags = RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
  BOOL EnableScrollBar(int nSBFlags, UINT nArrowFlags = ESB_ENABLE_BOTH);
  // Timer Functions
  UINT SetTimer(UINT nIDEvent, UINT nElapse,
      void (CALLBACK* lpfnTimer)(HWID, UINT, UINT, DWORD));
  BOOL KillTimer(int nIDEvent);
  // ToolTip Functions
  BOOL EnableToolTips(BOOL bEnable = TRUE);
  BOOL EnableTrackingToolTips(BOOL bEnable = TRUE);
  static void PASCAL CancelToolTips(BOOL bKeys = FALSE);
  void FilterToolTipMessage(MSG* pMsg);
  // for command hit testing (used for automatic tooltips)
  virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
  // Window State Functions
  BOOL IsWindowEnabled() const;
  BOOL EnableWindow(BOOL bEnable = TRUE);
  // the active window applies only to top-level (frame windows)
  static CWnd* PASCAL GetActiveWindow();
  CWnd* SetActiveWindow();
  // the foreground window applies only to top-level windows (frame windows)
  BOOL SetForegroundWindow();
  static CWnd* PASCAL GetForegroundWindow();
  // capture and focus apply to all windows
  static CWnd* PASCAL GetCapture();
  CWnd* SetCapture();
  static CWnd* PASCAL GetFocus();
  CWnd* SetFocus();
  static CWnd* PASCAL GetDesktopWindow();
  // Obsolete and non-portable APIs - not recommended for new code
  void CloseWindow();
  BOOL OpenIcon();
  // Dialog-Box Item Functions
  // (NOTE: Dialog-Box Items/Controls are not necessarily in dialog boxes!)
  void CheckDlgButton(int nIDButton, UINT nCheck);
  void CheckRadioButton(int nIDFirstButton, int nIDLastButton,
      int nIDCheckButton);
  int GetCheckedRadioButton(int nIDFirstButton, int nIDLastButton);
  int DlgDirList(LPTSTR lpPathSpec, int nIDListBox,
      int nIDStaticPath, UINT nFileType);
  int DlgDirListComboBox(LPTSTR lpPathSpec, int nIDComboBox,
      int nIDStaticPath, UINT nFileType);
  BOOL DlgDirSelect(LPTSTR lpString, int nIDListBox);
  BOOL DlgDirSelectComboBox(LPTSTR lpString, int nIDComboBox);
  UINT GetDlgItemInt(int nID, BOOL* lpTrans = NULL,
      BOOL bSigned = TRUE) const;
  int GetDlgItemText(int nID, LPTSTR lpStr, int nMaxCount) const;
  int GetDlgItemText(int nID, CString& rString) const;
  CWnd* GetNextDlgGroupItem(CWnd* pWndCtl, BOOL bPrevious = FALSE) const;
  CWnd* GetNextDlgTabItem(CWnd* pWndCtl, BOOL bPrevious = FALSE) const;
  UINT IsDlgButtonChecked(int nIDButton) const;
  LRESULT SendDlgItemMessage(int nID, UINT message,
      WPARAM wParam = 0, LPARAM lParam = 0);
  void SetDlgItemInt(int nID, UINT nValue, BOOL bSigned = TRUE);
  void SetDlgItemText(int nID, LPCTSTR lpszString);
  // Scrolling Functions
  int GetScrollPos(int nBar) const;
  void GetScrollRange(int nBar, LPINT lpMinPos, LPINT lpMaxPos) const;
  void ScrollWindow(int xAmount, int yAmount,
      LPCRECT lpRect = NULL,
      LPCRECT lpClipRect = NULL);
  int SetScrollPos(int nBar, int nPos, BOOL bRedraw = TRUE);
  void SetScrollRange(int nBar, int nMinPos, int nMaxPos,
      BOOL bRedraw = TRUE);
  void ShowScrollBar(UINT nBar, BOOL bShow = TRUE);
  void EnableScrollBarCtrl(int nBar, BOOL bEnable = TRUE);
  virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
  // return sibling scrollbar control (or NULL if none)
  int ScrollWindowEx(int dx, int dy,
      LPCRECT lpRectScroll, LPCRECT lpRectClip,
      CRgn* prgnUpdate, LPRECT lpRectUpdate, UINT flags);
  BOOL SetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo,
      BOOL bRedraw = TRUE);
  BOOL GetScrollInfo(int nBar, LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
  int GetScrollLimit(int nBar);
  // Window Access Functions
  CWnd* ChildWindowFromPoint(POINT point) const;
  CWnd* ChildWindowFromPoint(POINT point, UINT nFlags) const;
  static CWnd* PASCAL FindWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName);
#undef GetNextWindow
  CWnd* GetNextWindow(UINT nFlag = GW_HWNDNEXT) const;
  CWnd* GetTopWindow() const;
  CWnd* GetWindow(UINT nCmd) const;
  CWnd* GetLastActivePopup() const;
  BOOL IsChild(const CWnd* pWnd) const;
  CWnd* GetParent() const;
  CWnd* SetParent(CWnd* pWndNewParent);
  static CWnd* PASCAL WindowFromPoint(POINT point);
  // Alert Functions
  BOOL FlashWindow(BOOL bInvert);
  int MessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL,
      UINT nType = MB_OK);
  // Clipboard Functions
  BOOL ChangeClipboardChain(HWID hWndNext);
  HWID SetClipboardViewer();
  BOOL OpenClipboard();
  static CWnd* PASCAL GetClipboardOwner();
  static CWnd* PASCAL GetClipboardViewer();
  static CWnd* PASCAL GetOpenClipboardWindow();
  // Caret Functions
  void CreateCaret(CBitmap* pBitmap);
  void CreateSolidCaret(int nWidth, int nHeight);
  void CreateGrayCaret(int nWidth, int nHeight);
  static CPoint PASCAL GetCaretPos();
  static void PASCAL SetCaretPos(POINT point);
  void HideCaret();
  void ShowCaret();
  // Shell Interaction Functions
  void DragAcceptFiles(BOOL bAccept = TRUE);
  // Icon Functions
  HICON SetIcon(HICON hIcon, BOOL bBigIcon);
  HICON GetIcon(BOOL bBigIcon) const;
  // Context Help Functions
  BOOL SetWindowContextHelpId(DWORD dwContextHelpId);
  DWORD GetWindowContextHelpId() const;
  // Dialog Data support
public:
  BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
  // data wnd must be same type as this
  // Help Command Handlers
  afx_msg void OnHelp();          // F1 (uses current context)
  afx_msg void OnHelpIndex();     // ID_HELP_INDEX
  afx_msg void OnHelpFinder();    // ID_HELP_FINDER, ID_DEFAULT_HELP
  afx_msg void OnHelpUsing();     // ID_HELP_USING
  virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
  // Layout and other functions
public:
  enum RepositionFlags
  { reposDefault = 0, reposQuery = 1, reposExtra = 2 };
  void RepositionBars(UINT nIDFirst, UINT nIDLast, UINT nIDLeftOver,
      UINT nFlag = reposDefault, LPRECT lpRectParam = NULL,
      LPCRECT lpRectClient = NULL, BOOL bStretch = TRUE);
  // dialog support
  void UpdateDialogControls(CCmdTarget* pTarget, BOOL bDisableIfNoHndler);
  void CenterWindow(CWnd* pAlternateOwner = NULL);
  int RunModalLoop(DWORD dwFlags = 0);
  virtual BOOL ContinueModal();
  virtual void EndModalLoop(int nResult);
#ifndef _AFX_NO_OCC_SUPPORT
  // OLE control wrapper functions
  void AFX_CDECL InvokeHelper(DISPID dwDispID, WORD wFlags,
      VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, ...);
  void AFX_CDECL SetProperty(DISPID dwDispID, VARTYPE vtProp, ...);
  void GetProperty(DISPID dwDispID, VARTYPE vtProp, void* pvProp) const;
  IUnknown* GetDSCCursor();
  void BindDefaultProperty(DISPID dwDispID, VARTYPE vtProp, LPCTSTR szFieldName, CWnd* pDSCWnd);
  void BindProperty(DISPID dwDispId, CWnd* pWndDSC);
#endif
  // Window-Management message handler member functions
protected:
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
  afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
  afx_msg LRESULT OnActivateTopLevel(WPARAM, LPARAM);
  afx_msg void OnCancelMode();
  afx_msg void OnChildActivate();
  afx_msg void OnClose();
  afx_msg void OnContextMenu(CWnd* pWnd, CPoint pos);
  afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnDestroy();
  afx_msg void OnEnable(BOOL bEnable);
  afx_msg void OnEndSession(BOOL bEnding);
  afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
  afx_msg BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
  afx_msg void OnIconEraseBkgnd(CDC* pDC);
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
  afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
  afx_msg void OnMove(int x, int y);
  afx_msg void OnPaint();
  afx_msg void OnParentNotify(UINT message, LPARAM lParam);
  afx_msg HCURSOR OnQueryDragIcon();
  afx_msg BOOL OnQueryEndSession();
  afx_msg BOOL OnQueryNewPalette();
  afx_msg BOOL OnQueryOpen();
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnTCard(UINT idAction, DWORD dwActionData);
  afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
  afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
  // Nonclient-Area message handler member functions
  afx_msg BOOL OnNcActivate(BOOL bActive);
  afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
  afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnNcDestroy();
  afx_msg UINT OnNcHitTest(CPoint point);
  afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
  afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
  afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
  afx_msg void OnNcMButtonDblClk(UINT nHitTest, CPoint point);
  afx_msg void OnNcMButtonDown(UINT nHitTest, CPoint point);
  afx_msg void OnNcMButtonUp(UINT nHitTest, CPoint point);
  afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
  afx_msg void OnNcPaint();
  afx_msg void OnNcRButtonDblClk(UINT nHitTest, CPoint point);
  afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
  afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
  // System message handler member functions
  afx_msg void OnDropFiles(HDROP hDropInfo);
  afx_msg void OnPaletteIsChanging(CWnd* pRealizeWnd);
  afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnSysDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnCompacting(UINT nCpuTime);
  afx_msg void OnDevModeChange(LPTSTR lpDeviceName);
  afx_msg void OnFontChange();
  afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
  afx_msg void OnSpoolerStatus(UINT nStatus, UINT nJobs);
  afx_msg void OnSysColorChange();
  afx_msg void OnTimeChange();
  afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
  afx_msg void OnWinIniChange(LPCTSTR lpszSection);
  // Input message handler member functions
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnDeadChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
  afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg LRESULT OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam);
  afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg void OnTimer(UINT nIDEvent);
  // Initialization message handler member functions
  afx_msg void OnInitMenu(CMenu* pMenu);
  afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
  // Clipboard message handler member functions
  afx_msg void OnAskCbFormatName(UINT nMaxCount, LPTSTR lpszString);
  afx_msg void OnChangeCbChain(HWID hWndRemove, HWID hWndAfter);
  afx_msg void OnDestroyClipboard();
  afx_msg void OnDrawClipboard();
  afx_msg void OnHScrollClipboard(CWnd* pClipAppWnd, UINT nSBCode, UINT nPos);
  afx_msg void OnPaintClipboard(CWnd* pClipAppWnd, HGLOBAL hPaintStruct);
  afx_msg void OnRenderAllFormats();
  afx_msg void OnRenderFormat(UINT nFormat);
  afx_msg void OnSizeClipboard(CWnd* pClipAppWnd, HGLOBAL hRect);
  afx_msg void OnVScrollClipboard(CWnd* pClipAppWnd, UINT nSBCode, UINT nPos);
  // Control message handler member functions
  afx_msg int OnCompareItem(int nIDCtl, LPCOMPAREITEMSTRUCT lpCompareItemStruct);
  afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
  afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
  afx_msg UINT OnGetDlgCode();
  afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
  afx_msg int OnCharToItem(UINT nChar, CListBox* pListBox, UINT nIndex);
  afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
  // MDI message handler member functions
  afx_msg void OnMDIActivate(BOOL bActivate,
      CWnd* pActivateWnd, CWnd* pDeactivateWnd);
  // Menu loop notification messages
  afx_msg void OnEnterMenuLoop(BOOL bIsTrackPopupMenu);
  afx_msg void OnExitMenuLoop(BOOL bIsTrackPopupMenu);
  // Win4 messages
  afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
  afx_msg void OnStyleChanging(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
  afx_msg void OnSizing(UINT nSide, LPRECT lpRect);
  afx_msg void OnMoving(UINT nSide, LPRECT lpRect);
  afx_msg void OnCaptureChanged(CWnd* pWnd);
  afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
  // Overridables and other helpers (for implementation of derived classes)
protected:
  // for deriving from a standard control
  virtual WNDPROC* GetSuperWndProcAddr();
  // for dialog data exchange and validation
  virtual void DoDataExchange(CDataExchange* pDX);
public:
  // for modality
  virtual void BeginModalState();
  virtual void EndModalState();
  // for translating Windows messages in main message pump
  virtual BOOL PreTranslateMessage(MSG* pMsg);
#ifndef _AFX_NO_OCC_SUPPORT
  // for ambient properties exposed to contained OLE controls
  virtual BOOL OnAmbientProperty(COleControlSite* pSite, DISPID dispid,
      VARIANT* pvar);
#endif
protected:
  // for processing Windows messages
  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
  // for handling default processing
  LRESULT Default();
  virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  // for custom cleanup after WM_NCDESTROY
  virtual void PostNcDestroy();
  // for notifications from parent
  virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
  // return TRUE if parent should not process this message
  BOOL ReflectChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
  static BOOL PASCAL ReflectLastMsg(HWID hWndChild, LRESULT* pResult = NULL);
  // Implementation
public:
  virtual ~CWnd();
  virtual BOOL CheckAutoCenter();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
#ifndef _AFX_NO_CTL3D_SUPPORT
  // 3D support (these APIs will be obsolete with next version of Windows)
  BOOL SubclassCtl3d(int nControlType = -1);
  // see CTL3D.H for list of control types
  BOOL SubclassDlg3d(DWORD dwMask = 0xFFFF /*CTL3D_ALL*/);
  // see CTL3D.H for list of mask values
#endif
  static BOOL PASCAL GrayCtlColor(HDC hDC, HWID hWnd, UINT nCtlColor,
      HBRUSH hbrGray, COLORREF clrText);
#ifndef _AFX_NO_GRAYDLG_SUPPORT
  HBRUSH OnGrayCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
#endif
  // helper routines for implementation
  BOOL HandleFloatingSysCommand(UINT nID, LPARAM lParam);
  BOOL IsTopParentActive() const;
  void ActivateTopParent();
  static BOOL PASCAL WalkPreTranslateTree(HWID hWndStop, MSG* pMsg);
  static CWnd* PASCAL GetDescendantWindow(HWID hWnd, int nID,
      BOOL bOnlyPerm);
  static void PASCAL SendMessageToDescendants(HWID hWnd, UINT message,
      WPARAM wParam, LPARAM lParam, BOOL bDeep, BOOL bOnlyPerm);
  virtual BOOL IsFrameWnd() const; // IsKindOf(RUNTIME_CLASS(CFrameWnd)))
  virtual void OnFinalRelease();
  BOOL PreTranslateInput(LPMSG lpMsg);
  static BOOL PASCAL ModifyStyle(HWID hWnd, DWORD dwRemove, DWORD dwAdd,
      UINT nFlags);
  static BOOL PASCAL ModifyStyleEx(HWID hWnd, DWORD dwRemove, DWORD dwAdd,
      UINT nFlags);
  static void PASCAL _FilterToolTipMessage(MSG* pMsg, CWnd* pWnd);
  BOOL _EnableToolTips(BOOL bEnable, UINT nFlag);
  static HWID PASCAL GetSafeOwner_(HWID hWnd, HWID* pWndTop);
public:
  HWID m_hWndOwner;   // implementation of SetOwner and GetOwner
  UINT m_nFlags;      // see WF_ flags above
protected:
  WNDPROC m_pfnSuper; // for subclassing of controls
  static const UINT m_nMsgDragList;
  int m_nModalResult; // for return values from CWnd::RunModalLoop
  COleDropTarget* m_pDropTarget;  // for automatic cleanup of drop target
  friend class COleDropTarget;
  friend class CFrameWnd;
  // for creating dialogs and dialog-like windows
  BOOL CreateDlg(LPCTSTR lpszTemplateName, CWnd* pParentWnd);
  BOOL CreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd);
  BOOL CreateDlgIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd,
      HINSTANCE hInst);
#ifndef _AFX_NO_OCC_SUPPORT
  COleControlContainer* m_pCtrlCont;  // for containing OLE controls
  COleControlSite* m_pCtrlSite;       // for wrapping an OLE control
  friend class COccManager;
  friend class COleControlSite;
  friend class COleControlContainer;
  BOOL InitControlContainer();
  virtual BOOL SetOccDialogInfo(struct _AFX_OCC_DIALOG_INFO* pOccDialogInfo);
  void AttachControlSite(CHandleMap* pMap);
public:
  void AttachControlSite(CWnd* pWndParent);
#endif
protected:
  // implementation of message dispatch/hooking
  friend LRESULT CALLBACK _AfxSendMsgHook(int, WPARAM, LPARAM);
  friend void AFXAPI _AfxStandardSubclass(HWID);
  friend LRESULT CALLBACK _AfxCbtFilterHook(int, WPARAM, LPARAM);
  friend LRESULT AFXAPI AfxCallWndProc(CWnd*, HWID, UINT, WPARAM, LPARAM);
  // standard message implementation
  afx_msg LRESULT OnNTCtlColor(WPARAM wParam, LPARAM lParam);
#ifndef _AFX_NO_CTL3D_SUPPORT
  afx_msg LRESULT OnQuery3dControls(WPARAM, LPARAM);
#endif
  afx_msg LRESULT OnDisplayChange(WPARAM, LPARAM);
  afx_msg LRESULT OnDragList(WPARAM, LPARAM);
  //{{AFX_MSG(CWnd)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
  CWnd(HWID hWnd);    // just for special initialization
};
// helpers for registering your own WNDCLASSes
LPCTSTR AFXAPI AfxRegisterWndClass(UINT nClassStyle,
    HCURSOR hCursor = 0, HBRUSH hbrBackground = 0, HICON hIcon = 0);
BOOL AFXAPI AfxRegisterClass(WNDCLASS* lpWndClass);
// helper to initialize rich edit control
BOOL AFXAPI AfxInitRichEdit();
// Implementation
LRESULT CALLBACK AfxWndProc(HWID, UINT, WPARAM, LPARAM);
WNDPROC AFXAPI AfxGetAfxWndProc();
#define AfxWndProc (*AfxGetAfxWndProc())
typedef void (AFX_MSG_CALL CWnd::*AFX_PMSGW)(void);
// like 'AFX_PMSG' but for CWnd derived classes only
typedef void (AFX_MSG_CALL CWinThread::*AFX_PMSGT)(void);
// like 'AFX_PMSG' but for CWinThread-derived classes only
/////////////////////////////////////////////////////////////////////////////
// CDialog - a modal or modeless dialog
class CDialog : public CWnd
{
  DECLARE_DYNAMIC(CDialog)
  // Modeless construct
public:
  CDialog();
  BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
  BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
  BOOL CreateIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd = NULL,
      void* lpDialogInit = NULL);
  BOOL CreateIndirect(HGLOBAL hDialogTemplate, CWnd* pParentWnd = NULL);
  // Modal construct
public:
  CDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
  CDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
  BOOL InitModalIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd = NULL,
      void* lpDialogInit = NULL);
  BOOL InitModalIndirect(HGLOBAL hDialogTemplate, CWnd* pParentWnd = NULL);
  // Attributes
public:
  void MapDialogRect(LPRECT lpRect) const;
  void SetHelpID(UINT nIDR);
  // Operations
public:
  // modal processing
  virtual int DoModal();
  // support for passing on tab control - use 'PostMessage' if needed
  void NextDlgCtrl() const;
  void PrevDlgCtrl() const;
  void GotoDlgCtrl(CWnd* pWndCtrl);
  // default button access
  void SetDefID(UINT nID);
  DWORD GetDefID() const;
  // termination
  void EndDialog(int nResult);
  // Overridables (special message map entries)
  virtual BOOL OnInitDialog();
  virtual void OnSetFont(CFont* pFont);
protected:
  virtual void OnOK();
  virtual void OnCancel();
  // Implementation
public:
  virtual ~CDialog();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
      AFX_CMDHANDLERINFO* pHandlerInfo);
  virtual BOOL CheckAutoCenter();
protected:
  UINT m_nIDHelp;                 // Help ID (0 for none, see HID_BASE_RESOURCE)
  // parameters for 'DoModal'
  LPCTSTR m_lpszTemplateName;     // name or MAKEINTRESOURCE
  HGLOBAL m_hDialogTemplate;      // indirect (m_lpDialogTemplate == NULL)
  LPCDLGTEMPLATE m_lpDialogTemplate;  // indirect if (m_lpszTemplateName == NULL)
  void* m_lpDialogInit;           // DLGINIT resource data
  CWnd* m_pParentWnd;             // parent/owner window
  HWID m_hWndTop;                 // top level parent window (may be disabled)
#ifndef _AFX_NO_OCC_SUPPORT
  _AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
  virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);
#endif
  virtual void PreInitDialog();
  // implementation helpers
  HWID PreModal();
  void PostModal();
  BOOL CreateIndirect(LPCDLGTEMPLATE lpDialogTemplate, CWnd* pParentWnd,
      void* lpDialogInit, HINSTANCE hInst);
  BOOL CreateIndirect(HGLOBAL hDialogTemplate, CWnd* pParentWnd,
      HINSTANCE hInst);
protected:
  //{{AFX_MSG(CDialog)
  afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
  afx_msg LRESULT HandleSetFont(WPARAM, LPARAM);
  //}}AFX_MSG
#ifndef _AFX_NO_GRAYDLG_SUPPORT
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
#endif
  DECLARE_MESSAGE_MAP()
};
// all CModalDialog functionality is now in CDialog
#define CModalDialog    CDialog
/////////////////////////////////////////////////////////////////////////////
// Standard Windows controls
class CStatic : public CWnd
{
  DECLARE_DYNAMIC(CStatic)
  // Constructors
public:
  CStatic();
  BOOL Create(LPCTSTR lpszText, DWORD dwStyle,
      const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
  // Operations
  HICON SetIcon(HICON hIcon);
  HICON GetIcon() const;
#if (WINVER >= 0x400)
  HENHMETAFILE SetEnhMetaFile(HENHMETAFILE hMetaFile);
  HENHMETAFILE GetEnhMetaFile() const;
  HBITMAP SetBitmap(HBITMAP hBitmap);
  HBITMAP GetBitmap() const;
  HCURSOR SetCursor(HCURSOR hCursor);
  HCURSOR GetCursor();
#endif
  // Implementation
public:
  virtual ~CStatic();
};
class CButton : public CWnd
{
  DECLARE_DYNAMIC(CButton)
  // Constructors
public:
  CButton();
  BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle,
      const RECT& rect, CWnd* pParentWnd, UINT nID);
  // Attributes
  UINT GetState() const;
  void SetState(BOOL bHighlight);
  int GetCheck() const;
  void SetCheck(int nCheck);
  UINT GetButtonStyle() const;
  void SetButtonStyle(UINT nStyle, BOOL bRedraw = TRUE);
#if (WINVER >= 0x400)
  HICON SetIcon(HICON hIcon);
  HICON GetIcon() const;
  HBITMAP SetBitmap(HBITMAP hBitmap);
  HBITMAP GetBitmap() const;
  HCURSOR SetCursor(HCURSOR hCursor);
  HCURSOR GetCursor();
#endif
  // Overridables (for owner draw only)
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  // Implementation
public:
  virtual ~CButton();
protected:
  virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
};
class CListBox : public CWnd
{
  DECLARE_DYNAMIC(CListBox)
  // Constructors
public:
  CListBox();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
  // Attributes
  // for entire listbox
  int GetCount() const;
  int GetHorizontalExtent() const;
  void SetHorizontalExtent(int cxExtent);
  int GetTopIndex() const;
  int SetTopIndex(int nIndex);
  LCID GetLocale() const;
  LCID SetLocale(LCID nNewLocale);
#if (WINVER >= 0x400)
  int InitStorage(int nItems, UINT nBytes);
  UINT ItemFromPoint(CPoint pt, BOOL& bOutside) const;
#endif
  // for single-selection listboxes
  int GetCurSel() const;
  int SetCurSel(int nSelect);
  // for multiple-selection listboxes
  int GetSel(int nIndex) const;           // also works for single-selection
  int SetSel(int nIndex, BOOL bSelect = TRUE);
  int GetSelCount() const;
  int GetSelItems(int nMaxItems, LPINT rgIndex) const;
  void SetAnchorIndex(int nIndex);
  int GetAnchorIndex() const;
  // for listbox items
  DWORD GetItemData(int nIndex) const;
  int SetItemData(int nIndex, DWORD dwItemData);
  void* GetItemDataPtr(int nIndex) const;
  int SetItemDataPtr(int nIndex, void* pData);
  int GetItemRect(int nIndex, LPRECT lpRect) const;
  int GetText(int nIndex, LPTSTR lpszBuffer) const;
  void GetText(int nIndex, CString& rString) const;
  int GetTextLen(int nIndex) const;
  // Settable only attributes
  void SetColumnWidth(int cxWidth);
  BOOL SetTabStops(int nTabStops, LPINT rgTabStops);
  void SetTabStops();
  BOOL SetTabStops(const int& cxEachStop);    // takes an 'int'
  int SetItemHeight(int nIndex, UINT cyItemHeight);
  int GetItemHeight(int nIndex) const;
  int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;
  int GetCaretIndex() const;
  int SetCaretIndex(int nIndex, BOOL bScroll = TRUE);
  // Operations
  // manipulating listbox items
  int AddString(LPCTSTR lpszItem);
  int DeleteString(UINT nIndex);
  int InsertString(int nIndex, LPCTSTR lpszItem);
  void ResetContent();
  int Dir(UINT attr, LPCTSTR lpszWildCard);
  // selection helpers
  int FindString(int nStartAfter, LPCTSTR lpszItem) const;
  int SelectString(int nStartAfter, LPCTSTR lpszItem);
  int SelItemRange(BOOL bSelect, int nFirstItem, int nLastItem);
  // Overridables (must override draw, measure and compare for owner draw)
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
  virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
  virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
  virtual int VKeyToItem(UINT nKey, UINT nIndex);
  virtual int CharToItem(UINT nKey, UINT nIndex);
  // Implementation
public:
  virtual ~CListBox();
protected:
  virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
};
class CCheckListBox : public CListBox
{
  DECLARE_DYNAMIC(CCheckListBox)
  // Constructors
public:
  CCheckListBox();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
  // Attributes
  void SetCheckStyle(UINT nStyle);
  UINT GetCheckStyle();
  void SetCheck(int nIndex, int nCheck);
  int GetCheck(int nIndex);
  void Enable(int nIndex, BOOL bEnabled = TRUE);
  BOOL IsEnabled(int nIndex);
  virtual CRect OnGetCheckPosition(CRect rectItem, CRect rectCheckBox);
  // Overridables (must override draw, measure and compare for owner draw)
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
  // Implementation
protected:
  void PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  void PreMeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
  int PreCompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
  void PreDeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
  virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
  void SetSelectionCheck(int nCheck);
#ifdef _DEBUG
  virtual void PreSubclassWindow();
#endif
  int CalcMinimumItemHeight();
  void InvalidateCheck(int nIndex);
  void InvalidateItem(int nIndex);
  int CheckFromPoint(CPoint point, BOOL& bInCheck);
  int m_cyText;
  UINT m_nStyle;
  // Message map functions
protected:
  //{{AFX_MSG(CCheckListBox)
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnLBAddString(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnLBFindString(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnLBFindStringExact(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnLBGetItemData(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnLBGetText(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnLBInsertString(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnLBSelectString(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnLBSetItemData(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnLBSetItemHeight(WPARAM wParam, LPARAM lParam);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
class CComboBox : public CWnd
{
  DECLARE_DYNAMIC(CComboBox)
  // Constructors
public:
  CComboBox();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
  // Attributes
  // for entire combo box
  int GetCount() const;
  int GetCurSel() const;
  int SetCurSel(int nSelect);
  LCID GetLocale() const;
  LCID SetLocale(LCID nNewLocale);
  // Win4
  int GetTopIndex() const;
  int SetTopIndex(int nIndex);
  int InitStorage(int nItems, UINT nBytes);
  void SetHorizontalExtent(UINT nExtent);
  UINT GetHorizontalExtent() const;
  int SetDroppedWidth(UINT nWidth);
  int GetDroppedWidth() const;
  // for edit control
  DWORD GetEditSel() const;
  BOOL LimitText(int nMaxChars);
  BOOL SetEditSel(int nStartChar, int nEndChar);
  // for combobox item
  DWORD GetItemData(int nIndex) const;
  int SetItemData(int nIndex, DWORD dwItemData);
  void* GetItemDataPtr(int nIndex) const;
  int SetItemDataPtr(int nIndex, void* pData);
  int GetLBText(int nIndex, LPTSTR lpszText) const;
  void GetLBText(int nIndex, CString& rString) const;
  int GetLBTextLen(int nIndex) const;
  int SetItemHeight(int nIndex, UINT cyItemHeight);
  int GetItemHeight(int nIndex) const;
  int FindStringExact(int nIndexStart, LPCTSTR lpszFind) const;
  int SetExtendedUI(BOOL bExtended = TRUE);
  BOOL GetExtendedUI() const;
  void GetDroppedControlRect(LPRECT lprect) const;
  BOOL GetDroppedState() const;
  // Operations
  // for drop-down combo boxes
  void ShowDropDown(BOOL bShowIt = TRUE);
  // manipulating listbox items
  int AddString(LPCTSTR lpszString);
  int DeleteString(UINT nIndex);
  int InsertString(int nIndex, LPCTSTR lpszString);
  void ResetContent();
  int Dir(UINT attr, LPCTSTR lpszWildCard);
  // selection helpers
  int FindString(int nStartAfter, LPCTSTR lpszString) const;
  int SelectString(int nStartAfter, LPCTSTR lpszString);
  // Clipboard operations
  void Clear();
  void Copy();
  void Cut();
  void Paste();
  // Overridables (must override draw, measure and compare for owner draw)
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
  virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
  virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
  virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
  // Implementation
public:
  virtual ~CComboBox();
protected:
  virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
};
class CEdit : public CWnd
{
  DECLARE_DYNAMIC(CEdit)
  // Constructors
public:
  CEdit();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
  // Attributes
  BOOL CanUndo() const;
  int GetLineCount() const;
  BOOL GetModify() const;
  void SetModify(BOOL bModified = TRUE);
  void GetRect(LPRECT lpRect) const;
  DWORD GetSel() const;
  void GetSel(int& nStartChar, int& nEndChar) const;
  HLOCAL GetHandle() const;
  void SetHandle(HLOCAL hBuffer);
#if (WINVER >= 0x400)
  void SetMargins(UINT nLeft, UINT nRight);
  DWORD GetMargins() const;
  void SetLimitText(UINT nMax);
  UINT GetLimitText() const;
  CPoint PosFromChar(UINT nChar) const;
  int CharFromPos(CPoint pt) const;
#endif
  // NOTE: first word in lpszBuffer must contain the size of the buffer!
  int GetLine(int nIndex, LPTSTR lpszBuffer) const;
  int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const;
  // Operations
  void EmptyUndoBuffer();
  BOOL FmtLines(BOOL bAddEOL);
  void LimitText(int nChars = 0);
  int LineFromChar(int nIndex = -1) const;
  int LineIndex(int nLine = -1) const;
  int LineLength(int nLine = -1) const;
  void LineScroll(int nLines, int nChars = 0);
  void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);
  void SetPasswordChar(TCHAR ch);
  void SetRect(LPCRECT lpRect);
  void SetRectNP(LPCRECT lpRect);
  void SetSel(DWORD dwSelection, BOOL bNoScroll = FALSE);
  void SetSel(int nStartChar, int nEndChar, BOOL bNoScroll = FALSE);
  BOOL SetTabStops(int nTabStops, LPINT rgTabStops);
  void SetTabStops();
  BOOL SetTabStops(const int& cxEachStop);    // takes an 'int'
  // Clipboard operations
  BOOL Undo();
  void Clear();
  void Copy();
  void Cut();
  void Paste();
  BOOL SetReadOnly(BOOL bReadOnly = TRUE);
  int GetFirstVisibleLine() const;
  TCHAR GetPasswordChar() const;
  // Implementation
public:
  virtual ~CEdit();
};
class CScrollBar : public CWnd
{
  DECLARE_DYNAMIC(CScrollBar)
  // Constructors
public:
  CScrollBar();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
  // Attributes
  int GetScrollPos() const;
  int SetScrollPos(int nPos, BOOL bRedraw = TRUE);
  void GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos) const;
  void SetScrollRange(int nMinPos, int nMaxPos, BOOL bRedraw = TRUE);
  void ShowScrollBar(BOOL bShow = TRUE);
  BOOL EnableScrollBar(UINT nArrowFlags = ESB_ENABLE_BOTH);
  BOOL SetScrollInfo(LPSCROLLINFO lpScrollInfo, BOOL bRedraw = TRUE);
  BOOL GetScrollInfo(LPSCROLLINFO lpScrollInfo, UINT nMask = SIF_ALL);
  int GetScrollLimit();
  // Implementation
public:
  virtual ~CScrollBar();
};
/////////////////////////////////////////////////////////////////////////////
// CFrameWnd - base class for SDI and other frame windows
// Frame window styles
#define FWS_ADDTOTITLE  0x00008000L // modify title based on content
#define FWS_PREFIXTITLE 0x00004000L // show document name before app name
#define FWS_SNAPTOBARS  0x00002000L // snap size to size of contained bars
struct CPrintPreviewState;  // forward reference (see afxext.h)
class CControlBar;          // forward reference (see afxext.h)
class CReBar;               // forward reference (see afxext.h)
class CDockBar;             // forward reference (see afxpriv.h)
class CMiniDockFrameWnd;    // forward reference (see afxpriv.h)
class CDockState;           // forward reference (see afxpriv.h)
class COleFrameHook;        // forward reference (see ..\src\oleimpl2.h)
class CFrameWnd : public CWnd
{
  DECLARE_DYNCREATE(CFrameWnd)
  // Constructors
public:
  static AFX_DATA const CRect rectDefault;
  CFrameWnd();
  BOOL LoadAccelTable(LPCTSTR lpszResourceName);
  BOOL Create(LPCTSTR lpszClassName,
      LPCTSTR lpszWindowName,
      DWORD dwStyle = WS_OVERLAPPEDWINDOW,
      const RECT& rect = rectDefault,
      CWnd* pParentWnd = NULL,        // != NULL for popups
      LPCTSTR lpszMenuName = NULL,
      DWORD dwExStyle = 0,
      CCreateContext* pContext = NULL);
  // dynamic creation - load frame and associated resources
  virtual BOOL LoadFrame(UINT nIDResource,
      DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
      CWnd* pParentWnd = NULL,
      CCreateContext* pContext = NULL);
  // special helper for view creation
  CWnd* CreateView(CCreateContext* pContext, UINT nID = AFX_IDW_PANE_FIRST);
  // Attributes
  virtual CDocument* GetActiveDocument();
  // Active child view maintenance
  CView* GetActiveView() const;           // active view or NULL
  void SetActiveView(CView* pViewNew, BOOL bNotify = TRUE);
  // active view or NULL, bNotify == FALSE if focus should not be set
  // Active frame (for frames within frames -- MDI)
  virtual CFrameWnd* GetActiveFrame();
  // For customizing the default messages on the status bar
  virtual void GetMessageString(UINT nID, CString& rMessage) const;
  BOOL m_bAutoMenuEnable;
  // TRUE => menu items without handlers will be disabled
  BOOL IsTracking() const;
  // Operations
  virtual void RecalcLayout(BOOL bNotify = TRUE);
  virtual void ActivateFrame(int nCmdShow = -1);
  void InitialUpdateFrame(CDocument* pDoc, BOOL bMakeVisible);
  void SetTitle(LPCTSTR lpszTitle);
  CString GetTitle() const;
  // to set text of standard status bar
  void SetMessageText(LPCTSTR lpszText);
  void SetMessageText(UINT nID);
  // control bar docking
  void EnableDocking(DWORD dwDockStyle);
  void DockControlBar(CControlBar* pBar, UINT nDockBarID = 0,
      LPCRECT lpRect = NULL);
  void FloatControlBar(CControlBar* pBar, CPoint point,
      DWORD dwStyle = CBRS_ALIGN_TOP);
  CControlBar* GetControlBar(UINT nID);
  // frame window based modality
  virtual void BeginModalState();
  virtual void EndModalState();
  BOOL InModalState() const;
  void ShowOwnedWindows(BOOL bShow);
  // saving and loading control bar state
  void LoadBarState(LPCTSTR lpszProfileName);
  void SaveBarState(LPCTSTR lpszProfileName) const;
  void ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay);
  void SetDockState(const CDockState& state);
  void GetDockState(CDockState& state) const;
  // Overridables
  virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
  virtual CWnd* GetMessageBar();
  // border space negotiation
  enum BorderCmd
  { borderGet = 1, borderRequest = 2, borderSet = 3 };
  virtual BOOL NegotiateBorderSpace(UINT nBorderCmd, LPRECT lpRectBorder);
protected:
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
  // Command Handlers
public:
  afx_msg void OnContextHelp();   // for Shift+F1 help
  afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
  afx_msg BOOL OnBarCheck(UINT nID);
  // Implementation
public:
  virtual ~CFrameWnd();
  int m_nWindow;  // general purpose window number - display as ":n"
  // -1 => unknown, 0 => only window viewing document
  // 1 => first of many windows viewing document, 2=> second
  HMENU m_hMenuDefault;       // default menu resource for this frame
  HACCEL m_hAccelTable;       // accelerator table
  DWORD m_dwPromptContext;    // current help prompt context for message box
  BOOL m_bHelpMode;           // if TRUE, then Shift+F1 help mode is active
  CFrameWnd* m_pNextFrameWnd; // next CFrameWnd in app global list
  CRect m_rectBorder;         // for OLE border space negotiation
  COleFrameHook* m_pNotifyHook;
  CPtrList m_listControlBars; // array of all control bars that have this
  // window as their dock site
  int m_nShowDelay;           // SW_ command for delay show/hide
  CMiniDockFrameWnd* CreateFloatingFrame(DWORD dwStyle);
  DWORD CanDock(CRect rect, DWORD dwDockStyle,
      CDockBar** ppDockBar = NULL); // called by CDockContext
  void AddControlBar(CControlBar* pBar);
  void RemoveControlBar(CControlBar* pBar);
  void DockControlBar(CControlBar* pBar, CDockBar* pDockBar,
      LPCRECT lpRect = NULL);
  void ReDockControlBar(CControlBar* pBar, CDockBar* pDockBar,
      LPCRECT lpRect = NULL);
  void NotifyFloatingWindows(DWORD dwFlags);
  void DestroyDockBars();
protected:
  UINT m_nIDHelp;             // Help ID (0 for none, see HID_BASE_RESOURCE)
  UINT m_nIDTracking;         // tracking command ID or string IDS
  UINT m_nIDLastMessage;      // last displayed message string IDS
  CView* m_pViewActive;       // current active view
  BOOL (CALLBACK* m_lpfnCloseProc)(CFrameWnd* pFrameWnd);
  UINT m_cModalStack;         // BeginModalState depth
  HWID* m_phWndDisable;       // windows disabled because of BeginModalState
  HMENU m_hMenuAlt;           // menu to update to (NULL means default)
  CString m_strTitle;         // default title (original)
  BOOL m_bInRecalcLayout;     // avoid recursion in RecalcLayout
  CRuntimeClass* m_pFloatingFrameClass;
  static const DWORD dwDockBarMap[4][2];
public:
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  virtual BOOL IsFrameWnd() const;
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
      AFX_CMDHANDLERINFO* pHandlerInfo);
  virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
  virtual void OnUpdateFrameMenu(HMENU hMenuAlt);
  virtual HACCEL GetDefaultAccelerator();
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  // idle update of frame user interface
  enum IdleFlags
  { idleMenu = 1, idleTitle = 2, idleNotify = 4, idleLayout = 8 };
  UINT m_nIdleFlags;          // set of bit flags for idle processing
  virtual void DelayUpdateFrameMenu(HMENU hMenuAlt);
  void DelayUpdateFrameTitle();
  void DelayRecalcLayout(BOOL bNotify = TRUE);
  // for Shift+F1 help support
  BOOL CanEnterHelpMode();
  virtual void ExitHelpMode();
  // implementation helpers
public:
  void UpdateFrameTitleForDocument(LPCTSTR lpszDocName);
protected:
  LPCTSTR GetIconWndClass(DWORD dwDefaultStyle, UINT nIDResource);
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  virtual void PostNcDestroy();   // default to delete this.
  int OnCreateHelper(LPCREATESTRUCT lpcs, CCreateContext* pContext);
  void BringToTop(int nCmdShow);
  // bring window to top for SW_ commands which affect z-order
  // implementation helpers for Shift+F1 help mode
  BOOL ProcessHelpMsg(MSG& msg, DWORD* pContext);
  HWID SetHelpCapture(POINT point, BOOL* pbDescendant);
  // CFrameWnd list management
  void AddFrameWnd();
  void RemoveFrameWnd();
  friend class CWnd;  // for access to m_bModalDisable
  friend class CReBar; // for access to m_bInRecalcLayout
  //{{AFX_MSG(CFrameWnd)
  // Windows messages
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnDestroy();
  afx_msg void OnClose();
  afx_msg void OnInitMenu(CMenu*);
  afx_msg void OnInitMenuPopup(CMenu*, UINT, BOOL);
  afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
  afx_msg LRESULT OnPopMessageString(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnHelpPromptAddr(WPARAM wParam, LPARAM lParam);
  afx_msg void OnIdleUpdateCmdUI();
  afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
  afx_msg BOOL OnNcActivate(BOOL bActive);
  afx_msg void OnSysCommand(UINT nID, LONG lParam);
  afx_msg BOOL OnQueryEndSession();
  afx_msg void OnEndSession(BOOL bEnding);
  afx_msg void OnDropFiles(HDROP hDropInfo);
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnActivateTopLevel(WPARAM wParam, LPARAM lParam);
  afx_msg void OnEnable(BOOL bEnable);
  afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
  afx_msg BOOL OnQueryNewPalette();
  // standard commands
  afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnUpdateKeyIndicator(CCmdUI* pCmdUI);
  afx_msg void OnHelp();
  afx_msg void OnUpdateContextHelp(CCmdUI* pCmdUI);
  //}}AFX_MSG
protected:
  afx_msg LRESULT OnDDEInitiate(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnDDEExecute(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnDDETerminate(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnRegisteredMouseWheel(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()
  friend class CWinApp;
};
/////////////////////////////////////////////////////////////////////////////
// MDI Support
class CMDIFrameWnd : public CFrameWnd
{
  DECLARE_DYNCREATE(CMDIFrameWnd)
public:
  // Constructors
  CMDIFrameWnd();
  // Operations
  void MDIActivate(CWnd* pWndActivate);
  CMDIChildWnd* MDIGetActive(BOOL* pbMaximized = NULL) const;
  void MDIIconArrange();
  void MDIMaximize(CWnd* pWnd);
  void MDINext();
  void MDIRestore(CWnd* pWnd);
  CMenu* MDISetMenu(CMenu* pFrameMenu, CMenu* pWindowMenu);
  void MDITile();
  void MDICascade();
  void MDITile(int nType);
  void MDICascade(int nType);
  CMDIChildWnd* CreateNewChild(CRuntimeClass* pClass, UINT nResource,
      HMENU hMenu = NULL, HACCEL hAccel = NULL);
  // Overridables
  // MFC 1.0 backward compatible CreateClient hook (called by OnCreateClient)
  virtual BOOL CreateClient(LPCREATESTRUCT lpCreateStruct, CMenu* pWindowMenu);
  // customize if using an 'Window' menu with non-standard IDs
  virtual HMENU GetWindowMenuPopup(HMENU hMenuBar);
  // Implementation
public:
  HWID m_hWndMDIClient;       // MDI Client window handle
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL LoadFrame(UINT nIDResource,
      DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE,
      CWnd* pParentWnd = NULL,
      CCreateContext* pContext = NULL);
  virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
      AFX_CMDHANDLERINFO* pHandlerInfo);
  virtual void OnUpdateFrameMenu(HMENU hMenuAlt);
  virtual void DelayUpdateFrameMenu(HMENU hMenuAlt);
  virtual CFrameWnd* GetActiveFrame();
protected:
  virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  //{{AFX_MSG(CMDIFrameWnd)
  afx_msg void OnDestroy();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnUpdateMDIWindowCmd(CCmdUI* pCmdUI);
  afx_msg BOOL OnMDIWindowCmd(UINT nID);
  afx_msg void OnWindowNew();
  afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
  afx_msg void OnIdleUpdateCmdUI();
  afx_msg LRESULT OnMenuChar(UINT nChar, UINT, CMenu*);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
class CMDIChildWnd : public CFrameWnd
{
  DECLARE_DYNCREATE(CMDIChildWnd)
  // Constructors
public:
  CMDIChildWnd();
  virtual BOOL Create(LPCTSTR lpszClassName,
      LPCTSTR lpszWindowName,
      DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
      const RECT& rect = rectDefault,
      CMDIFrameWnd* pParentWnd = NULL,
      CCreateContext* pContext = NULL);
  // Attributes
  CMDIFrameWnd* GetMDIFrame();
  // Operations
  void MDIDestroy();
  void MDIActivate();
  void MDIMaximize();
  void MDIRestore();
  void SetHandles(HMENU hMenu, HACCEL hAccel);
  // Implementation
protected:
  HMENU m_hMenuShared;        // menu when we are active
public:
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle,
      CWnd* pParentWnd, CCreateContext* pContext = NULL);
  // 'pParentWnd' parameter is required for MDI Child
  virtual BOOL DestroyWindow();
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual void ActivateFrame(int nCmdShow = -1);
  virtual void OnUpdateFrameMenu(BOOL bActive, CWnd* pActivateWnd,
      HMENU hMenuAlt);
  BOOL m_bPseudoInactive;     // TRUE if window is MDI active according to
  //  windows, but not according to MFC...
protected:
  virtual CWnd* GetMessageBar();
  virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
  virtual LRESULT DefWindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
  BOOL UpdateClientEdge(LPRECT lpRect = NULL);
  //{{AFX_MSG(CMDIChildWnd)
  afx_msg void OnMDIActivate(BOOL bActivate, CWnd*, CWnd*);
  afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnWindowPosChanging(LPWINDOWPOS lpWndPos);
  afx_msg BOOL OnNcActivate(BOOL bActive);
  afx_msg void OnDestroy();
  afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CMiniFrameWnd
// MiniFrame window styles
#define MFS_SYNCACTIVE      0x00000100L // syncronize activation w/ parent
#define MFS_4THICKFRAME     0x00000200L // thick frame all around (no tiles)
#define MFS_THICKFRAME      0x00000400L // use instead of WS_THICKFRAME
#define MFS_MOVEFRAME       0x00000800L // no sizing, just moving
#define MFS_BLOCKSYSMENU    0x00001000L // block hit testing on system menu
class CMiniFrameWnd : public CFrameWnd
{
  DECLARE_DYNCREATE(CMiniFrameWnd)
  // Constructors
public:
  CMiniFrameWnd();
  BOOL Create(LPCTSTR lpClassName, LPCTSTR lpWindowName,
      DWORD dwStyle, const RECT& rect,
      CWnd* pParentWnd = NULL, UINT nID = 0);
  BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpClassName, LPCTSTR lpWindowName,
      DWORD dwStyle, const RECT& rect,
      CWnd* pParentWnd = NULL, UINT nID = 0);
  // Implementation
public:
  ~CMiniFrameWnd();
  static void AFX_CDECL Initialize();
  //{{AFX_MSG(CMiniFrameWnd)
  afx_msg BOOL OnNcActivate(BOOL bActive);
  afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpParams);
  afx_msg UINT OnNcHitTest(CPoint point);
  afx_msg void OnNcPaint();
  afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint pt);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
  afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnGetMinMaxInfo(MINMAXINFO* pMMI);
  afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnFloatStatus(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT OnQueryCenterWnd(WPARAM wParam, LPARAM lParam);
  afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpcs);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  virtual void CalcWindowRect(LPRECT lpClientRect,
      UINT nAdjustType = adjustBorder);
  static void PASCAL CalcBorders(LPRECT lpClientRect,
      DWORD dwStyle = WS_THICKFRAME | WS_CAPTION, DWORD dwExStyle = 0);
protected:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
  BOOL m_bSysTracking;
  BOOL m_bInSys;
  BOOL m_bActive;
  CString m_strCaption;
  void InvertSysMenu();
};
/////////////////////////////////////////////////////////////////////////////
// class CView is the client area UI for a document
class CPrintDialog;     // forward reference (see afxdlgs.h)
class CPreviewView;     // forward reference (see afxpriv.h)
class CSplitterWnd;     // forward reference (see afxext.h)
class COleServerDoc;    // forward reference (see afxole.h)
typedef DWORD DROPEFFECT;
class COleDataObject;   // forward reference (see afxole.h)
#ifdef _AFXDLL
class CView : public CWnd
#else
class AFX_NOVTABLE CView : public CWnd
#endif
{
  DECLARE_DYNAMIC(CView)
  // Constructors
protected:
  CView();
  // Attributes
public:
  CDocument* GetDocument() const;
  // Operations
public:
  // for standard printing setup (override OnPreparePrinting)
  BOOL DoPreparePrinting(CPrintInfo* pInfo);
  // Overridables
public:
  virtual BOOL IsSelected(const CObject* pDocItem) const; // support for OLE
  // OLE scrolling support (used for drag/drop as well)
  virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
  virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
  // OLE drag/drop support
  virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject,
      DWORD dwKeyState, CPoint point);
  virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject,
      DWORD dwKeyState, CPoint point);
  virtual void OnDragLeave();
  virtual BOOL OnDrop(COleDataObject* pDataObject,
      DROPEFFECT dropEffect, CPoint point);
  virtual DROPEFFECT OnDropEx(COleDataObject* pDataObject,
      DROPEFFECT dropDefault, DROPEFFECT dropList, CPoint point);
  virtual DROPEFFECT OnDragScroll(DWORD dwKeyState, CPoint point);
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  virtual void OnInitialUpdate(); // called first time after construct
protected:
  // Activation
  virtual void OnActivateView(BOOL bActivate, CView* pActivateView,
      CView* pDeactiveView);
  virtual void OnActivateFrame(UINT nState, CFrameWnd* pFrameWnd);
  // General drawing/updating
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  virtual void OnDraw(CDC* pDC) = 0;
  // Printing support
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  // must override to enable printing and print preview
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  // Advanced: end print preview mode, move to point
  virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point,
      CPreviewView* pView);
  // Implementation
public:
  virtual ~CView();
#ifdef _DEBUG
  virtual void Dump(CDumpContext&) const;
  virtual void AssertValid() const;
#endif //_DEBUG
  // Advanced: for implementing custom print preview
  BOOL DoPrintPreview(UINT nIDResource, CView* pPrintView,
      CRuntimeClass* pPreviewViewClass, CPrintPreviewState* pState);
  virtual void CalcWindowRect(LPRECT lpClientRect,
      UINT nAdjustType = adjustBorder);
  virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
  static CSplitterWnd* PASCAL GetParentSplitter(
      const CWnd* pWnd, BOOL bAnyState);
protected:
  CDocument* m_pDocument;
public:
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
      AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual void PostNcDestroy();
  // friend classes that call protected CView overridables
  friend class CDocument;
  friend class CDocTemplate;
  friend class CPreviewView;
  friend class CFrameWnd;
  friend class CMDIFrameWnd;
  friend class CMDIChildWnd;
  friend class CSplitterWnd;
  friend class COleServerDoc;
  friend class CDocObjectServer;
  //{{AFX_MSG(CView)
  afx_msg int OnCreate(LPCREATESTRUCT lpcs);
  afx_msg void OnDestroy();
  afx_msg void OnPaint();
  afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
  // commands
  afx_msg void OnUpdateSplitCmd(CCmdUI* pCmdUI);
  afx_msg BOOL OnSplitCmd(UINT nID);
  afx_msg void OnUpdateNextPaneMenu(CCmdUI* pCmdUI);
  afx_msg BOOL OnNextPaneCmd(UINT nID);
  // not mapped commands - must be mapped in derived class
  afx_msg void OnFilePrint();
  afx_msg void OnFilePrintPreview();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// class CCtrlView allows almost any control to be a view
#ifdef _AFXDLL
class CCtrlView : public CView
#else
class AFX_NOVTABLE CCtrlView : public CView
#endif
{
  DECLARE_DYNCREATE(CCtrlView)
public:
  CCtrlView(LPCTSTR lpszClass, DWORD dwStyle);
  // Attributes
protected:
  CString m_strClass;
  DWORD m_dwDefaultStyle;
  // Overrides
  virtual void OnDraw(CDC*);
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  // Implementation
public:
#ifdef _DEBUG
  virtual void Dump(CDumpContext&) const;
  virtual void AssertValid() const;
#endif //_DEBUG
protected:
  afx_msg void OnPaint();
  DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// class CScrollView supports simple scrolling and scaling
class CScrollView : public CView
{
  DECLARE_DYNAMIC(CScrollView)
  // Constructors
protected:
  CScrollView();
public:
  static AFX_DATA const SIZE sizeDefault;
  // used to specify default calculated page and line sizes
  // in logical units - call one of the following Set routines
  void SetScaleToFitSize(SIZE sizeTotal);
  void SetScrollSizes(int nMapMode, SIZE sizeTotal,
      const SIZE& sizePage = sizeDefault,
      const SIZE& sizeLine = sizeDefault);
  // Attributes
public:
  CPoint GetScrollPosition() const;       // upper corner of scrolling
  CSize GetTotalSize() const;             // logical size
  // for device units
  CPoint GetDeviceScrollPosition() const;
  void GetDeviceScrollSizes(int& nMapMode, SIZE& sizeTotal,
      SIZE& sizePage, SIZE& sizeLine) const;
  // Operations
public:
  void ScrollToPosition(POINT pt);    // set upper left position
  void FillOutsideRect(CDC* pDC, CBrush* pBrush);
  void ResizeParentToFit(BOOL bShrinkOnly = TRUE);
  BOOL DoMouseWheel(UINT fFlags, short zDelta, CPoint point);
  // Implementation
protected:
  int m_nMapMode;
  CSize m_totalLog;           // total size in logical units (no rounding)
  CSize m_totalDev;           // total size in device units
  CSize m_pageDev;            // per page scroll size in device units
  CSize m_lineDev;            // per line scroll size in device units
  BOOL m_bCenter;             // Center output if larger than total size
  BOOL m_bInsideUpdate;       // internal state for OnSize callback
  void CenterOnPoint(CPoint ptCenter);
  void ScrollToDevicePosition(POINT ptDev); // explicit scrolling no checking
protected:
  virtual void OnDraw(CDC* pDC) = 0;      // pass on pure virtual
  void UpdateBars();          // adjust scrollbars etc
  BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
  // size with no bars
  void GetScrollBarSizes(CSize& sizeSb);
  void GetScrollBarState(CSize sizeClient, CSize& needSb,
      CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);
public:
  virtual ~CScrollView();
#ifdef _DEBUG
  virtual void Dump(CDumpContext&) const;
  virtual void AssertValid() const;
#endif //_DEBUG
  virtual void CalcWindowRect(LPRECT lpClientRect,
      UINT nAdjustType = adjustBorder);
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  // scrolling implementation support for OLE
  virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
  virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
  //{{AFX_MSG(CScrollView)
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CWinThread
typedef UINT(AFX_CDECL* AFX_THREADPROC)(LPVOID);
class COleMessageFilter;        // forward reference (see afxole.h)
class CWinThread : public CCmdTarget
{
  DECLARE_DYNAMIC(CWinThread)
public:
  // Constructors
  CWinThread();
  BOOL CreateThread(DWORD dwCreateFlags = 0, UINT nStackSize = 0,
      LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);
  // Attributes
  CWnd* m_pMainWnd;       // main window (usually same AfxGetApp()->m_pMainWnd)
  CWnd* m_pActiveWnd;     // active main window (may not be m_pMainWnd)
  BOOL m_bAutoDelete;     // enables 'delete this' after thread termination
  // only valid while running
  HANDLE m_hThread;       // this thread's HANDLE
  operator HANDLE() const;
  DWORD m_nThreadID;      // this thread's ID
  int GetThreadPriority();
  BOOL SetThreadPriority(int nPriority);
  // Operations
  DWORD SuspendThread();
  DWORD ResumeThread();
  BOOL PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam);
  // Overridables
  // thread initialization
  virtual BOOL InitInstance();
  // running and idle processing
  virtual int Run();
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  virtual BOOL PumpMessage();     // low level message pump
  virtual BOOL OnIdle(LONG lCount); // return TRUE if more idle processing
  virtual BOOL IsIdleMessage(MSG* pMsg);  // checks for special messages
  // thread termination
  virtual int ExitInstance(); // default will 'delete this'
  // Advanced: exception handling
  virtual LRESULT ProcessWndProcException(CException* e, const MSG* pMsg);
  // Advanced: handling messages sent to message filter hook
  virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
  // Advanced: virtual access to m_pMainWnd
  virtual CWnd* GetMainWnd();
  // Implementation
public:
  virtual ~CWinThread();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
  int m_nDisablePumpCount; // Diagnostic trap to detect illegal re-entrancy
#endif
  void CommonConstruct();
  virtual void Delete();
  // 'delete this' only if m_bAutoDelete == TRUE
  // message pump for Run
  MSG m_msgCur;                   // current message
public:
  // constructor used by implementation of AfxBeginThread
  CWinThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam);
  // valid after construction
  LPVOID m_pThreadParams; // generic parameters passed to starting function
  AFX_THREADPROC m_pfnThreadProc;
  // set after OLE is initialized
  void (AFXAPI* m_lpfnOleTermOrFreeLib)(BOOL, BOOL);
  COleMessageFilter* m_pMessageFilter;
protected:
  CPoint m_ptCursorLast;      // last mouse position
  UINT m_nMsgLast;            // last mouse message
  BOOL DispatchThreadMessageEx(MSG* msg);  // helper
  void DispatchThreadMessage(MSG* msg);  // obsolete
};
// global helpers for threads
CWinThread* AFXAPI AfxBeginThread(AFX_THREADPROC pfnThreadProc, LPVOID pParam,
    int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,
    DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);
CWinThread* AFXAPI AfxBeginThread(CRuntimeClass* pThreadClass,
    int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0,
    DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);
CWinThread* AFXAPI AfxGetThread();
void AFXAPI AfxEndThread(UINT nExitCode, BOOL bDelete = TRUE);
void AFXAPI AfxInitThread();
void AFXAPI AfxTermThread(HINSTANCE hInstTerm = NULL);
/////////////////////////////////////////////////////////////////////////////
// Global functions for access to the one and only CWinApp
#define afxCurrentWinApp    AfxGetModuleState()->m_pCurrentWinApp
#define afxCurrentInstanceHandle    AfxGetModuleState()->m_hCurrentInstanceHandle
#define afxCurrentResourceHandle    AfxGetModuleState()->m_hCurrentResourceHandle
#define afxCurrentAppName   AfxGetModuleState()->m_lpszCurrentAppName
#define afxContextIsDLL     AfxGetModuleState()->m_bDLL
#define afxRegisteredClasses    AfxGetModuleState()->m_fRegisteredClasses
#ifndef _AFX_NO_OCC_SUPPORT
#define afxOccManager   AfxGetModuleState()->m_pOccManager
#endif
// Advanced initialization: for overriding default WinMain
BOOL AFXAPI AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine, int nCmdShow);
void AFXAPI AfxWinTerm();
// Global Windows state data helper functions (inlines)
CWinApp* AFXAPI AfxGetApp();
CWnd* AFXAPI AfxGetMainWnd();
HINSTANCE AFXAPI AfxGetInstanceHandle();
HINSTANCE AFXAPI AfxGetResourceHandle();
void AFXAPI AfxSetResourceHandle(HINSTANCE hInstResource);
LPCTSTR AFXAPI AfxGetAppName();
// Use instead of PostQuitMessage in OLE server applications
void AFXAPI AfxPostQuitMessage(int nExitCode);
// Use AfxFindResourceHandle to find resource in chain of extension DLLs
#ifndef _AFXDLL
#define AfxFindResourceHandle(lpszResource, lpszType) AfxGetResourceHandle()
#else
HINSTANCE AFXAPI AfxFindResourceHandle(LPCTSTR lpszName, LPCTSTR lpszType);
#endif
LONG AFXAPI AfxDelRegTreeHelper(HKEY hParentKey, const CString& strKeyName);
class CRecentFileList;          // forward reference (see afxpriv.h)
// access to message filter in CWinApp
COleMessageFilter* AFXAPI AfxOleGetMessageFilter();
/////////////////////////////////////////////////////////////////////////////
// CCommandLineInfo
class CCommandLineInfo : public CObject
{
public:
  // Sets default values
  CCommandLineInfo();
  //plain char* version on UNICODE for source-code backwards compatibility
  virtual void ParseParam(const TCHAR* pszParam, BOOL bFlag, BOOL bLast);
#ifdef _UNICODE
  virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast);
#endif
  BOOL m_bShowSplash;
  BOOL m_bRunEmbedded;
  BOOL m_bRunAutomated;
  enum { FileNew, FileOpen, FilePrint, FilePrintTo, FileDDE,
      AppUnregister, FileNothing = -1
       } m_nShellCommand;
  // not valid for FileNew
  CString m_strFileName;
  // valid only for FilePrintTo
  CString m_strPrinterName;
  CString m_strDriverName;
  CString m_strPortName;
  ~CCommandLineInfo();
  // Implementation
protected:
  void ParseParamFlag(const char* pszParam);
  void ParseParamNotFlag(const TCHAR* pszParam);
#ifdef _UNICODE
  void ParseParamNotFlag(const char* pszParam);
#endif
  void ParseLast(BOOL bLast);
};
/////////////////////////////////////////////////////////////////////////////
// CDocManager
class CDocManager : public CObject
{
  DECLARE_DYNAMIC(CDocManager)
public:
  // Constructor
  CDocManager();
  //Document functions
  virtual void AddDocTemplate(CDocTemplate* pTemplate);
  virtual POSITION GetFirstDocTemplatePosition() const;
  virtual CDocTemplate* GetNextDocTemplate(POSITION& pos) const;
  virtual void RegisterShellFileTypes(BOOL bCompat);
  void UnregisterShellFileTypes();
  virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName); // open named file
  virtual BOOL SaveAllModified(); // save before exit
  virtual void CloseAllDocuments(BOOL bEndSession); // close documents before exiting
  virtual int GetOpenDocumentCount();
  // helper for standard commdlg dialogs
  virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
      DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
  //Commands
  // Advanced: process async DDE request
  virtual BOOL OnDDECommand(LPTSTR lpszCommand);
  virtual void OnFileNew();
  virtual void OnFileOpen();
  // Implementation
protected:
  CPtrList m_templateList;
  int GetDocumentCount(); // helper to count number of total documents
public:
  static CPtrList* pStaticList;       // for static CDocTemplate objects
  static BOOL bStaticInit;            // TRUE during static initialization
  static CDocManager* pStaticDocManager;  // for static CDocTemplate objects
public:
  virtual ~CDocManager();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
};
/////////////////////////////////////////////////////////////////////////////
// CWinApp - the root of all Windows applications
#define _AFX_MRU_COUNT   4      // default support for 4 entries in file MRU
#define _AFX_MRU_MAX_COUNT 16   // currently allocated id range supports 16
class CWinApp : public CWinThread
{
  DECLARE_DYNAMIC(CWinApp)
public:
  // Constructor
  CWinApp(LPCTSTR lpszAppName = NULL);     // app name defaults to EXE name
  // Attributes
  // Startup args (do not change)
  HINSTANCE m_hInstance;
  HINSTANCE m_hPrevInstance;
  LPTSTR m_lpCmdLine;
  int m_nCmdShow;
  // Running args (can be changed in InitInstance)
  LPCTSTR m_pszAppName;  // human readable name
  //  (from constructor or AFX_IDS_APP_TITLE)
  LPCTSTR m_pszRegistryKey;   // used for registry entries
  CDocManager* m_pDocManager;
  // Support for Shift+F1 help mode.
  BOOL m_bHelpMode;           // are we in Shift+F1 mode?
public:  // set in constructor to override default
  LPCTSTR m_pszExeName;       // executable name (no spaces)
  LPCTSTR m_pszHelpFilePath;  // default based on module path
  LPCTSTR m_pszProfileName;   // default based on app name
  // Initialization Operations - should be done in InitInstance
protected:
  void LoadStdProfileSettings(UINT nMaxMRU = _AFX_MRU_COUNT); // load MRU file list and last preview state
  void EnableShellOpen();
#ifndef _AFX_NO_GRAYDLG_SUPPORT
  void SetDialogBkColor(COLORREF clrCtlBk = RGB(192, 192, 192),
      COLORREF clrCtlText = RGB(0, 0, 0));
  // set dialog box and message box background color
#endif
  void SetRegistryKey(LPCTSTR lpszRegistryKey);
  void SetRegistryKey(UINT nIDRegistryKey);
  // enables app settings in registry instead of INI files
  //  (registry key is usually a "company name")
#ifndef _AFX_NO_CTL3D_SUPPORT
  BOOL Enable3dControls(); // use CTL3D32.DLL for 3D controls in dialogs
#ifndef _AFXDLL
  BOOL Enable3dControlsStatic();  // statically link CTL3D.LIB instead
#endif
#endif
  void RegisterShellFileTypes(BOOL bCompat = FALSE);
  // call after all doc templates are registered
  void RegisterShellFileTypesCompat();
  // for backwards compatibility
  void UnregisterShellFileTypes();
  // Helper Operations - usually done in InitInstance
public:
  // Cursors
  HCURSOR LoadCursor(LPCTSTR lpszResourceName) const;
  HCURSOR LoadCursor(UINT nIDResource) const;
  HCURSOR LoadStandardCursor(LPCTSTR lpszCursorName) const; // for IDC_ values
  HCURSOR LoadOEMCursor(UINT nIDCursor) const;             // for OCR_ values
  // Icons
  HICON LoadIcon(LPCTSTR lpszResourceName) const;
  HICON LoadIcon(UINT nIDResource) const;
  HICON LoadStandardIcon(LPCTSTR lpszIconName) const;       // for IDI_ values
  HICON LoadOEMIcon(UINT nIDIcon) const;                   // for OIC_ values
  // Profile settings (to the app specific .INI file, or registry)
  UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);
  BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);
  CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
      LPCTSTR lpszDefault = NULL);
  BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry,
      LPCTSTR lpszValue);
  BOOL GetProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry,
      LPBYTE* ppData, UINT* pBytes);
  BOOL WriteProfileBinary(LPCTSTR lpszSection, LPCTSTR lpszEntry,
      LPBYTE pData, UINT nBytes);
  BOOL Unregister();
  LONG DelRegTree(HKEY hParentKey, const CString& strKeyName);
  // Running Operations - to be done on a running application
  // Dealing with document templates
  void AddDocTemplate(CDocTemplate* pTemplate);
  POSITION GetFirstDocTemplatePosition() const;
  CDocTemplate* GetNextDocTemplate(POSITION& pos) const;
  // Dealing with files
  virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName); // open named file
  virtual void AddToRecentFileList(LPCTSTR lpszPathName);  // add to MRU
  // Printer DC Setup routine, 'struct tagPD' is a PRINTDLG structure
  void SelectPrinter(HANDLE hDevNames, HANDLE hDevMode,
      BOOL bFreeOld = TRUE);
  BOOL CreatePrinterDC(CDC& dc);
#ifndef _UNICODE
  BOOL GetPrinterDeviceDefaults(struct tagPDA* pPrintDlg);
#else
  BOOL GetPrinterDeviceDefaults(struct tagPDW* pPrintDlg);
#endif
  // Command line parsing
  BOOL RunEmbedded();
  BOOL RunAutomated();
  void ParseCommandLine(CCommandLineInfo& rCmdInfo);
  BOOL ProcessShellCommand(CCommandLineInfo& rCmdInfo);
  // Overridables
  // hooks for your initialization code
  virtual BOOL InitApplication();
  // exiting
  virtual BOOL SaveAllModified(); // save before exit
  void HideApplication();
  void CloseAllDocuments(BOOL bEndSession); // close documents before exiting
  // Advanced: to override message boxes and other hooks
  virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
  virtual void DoWaitCursor(int nCode); // 0 => restore, 1=> begin, -1=> end
  // Advanced: process async DDE request
  virtual BOOL OnDDECommand(LPTSTR lpszCommand);
  // Advanced: Help support
  virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
  // Command Handlers
protected:
  // map to the following for file new/open
  afx_msg void OnFileNew();
  afx_msg void OnFileOpen();
  // map to the following to enable print setup
  afx_msg void OnFilePrintSetup();
  // map to the following to enable help
  afx_msg void OnContextHelp();   // shift-F1
  afx_msg void OnHelp();          // F1 (uses current context)
  afx_msg void OnHelpIndex();     // ID_HELP_INDEX
  afx_msg void OnHelpFinder();    // ID_HELP_FINDER, ID_DEFAULT_HELP
  afx_msg void OnHelpUsing();     // ID_HELP_USING
  // Implementation
protected:
  HGLOBAL m_hDevMode;             // printer Dev Mode
  HGLOBAL m_hDevNames;            // printer Device Names
  DWORD m_dwPromptContext;        // help context override for message box
  int m_nWaitCursorCount;         // for wait cursor (>0 => waiting)
  HCURSOR m_hcurWaitCursorRestore; // old cursor to restore after wait cursor
  CRecentFileList* m_pRecentFileList;
  void UpdatePrinterSelection(BOOL bForceDefaults);
  void SaveStdProfileSettings();  // save options to .INI file
public: // public for implementation access
  CCommandLineInfo* m_pCmdInfo;
  ATOM m_atomApp, m_atomSystemTopic;   // for DDE open
  UINT m_nNumPreviewPages;        // number of default printed pages
  size_t  m_nSafetyPoolSize;      // ideal size
  void (AFXAPI* m_lpfnDaoTerm)();
  void DevModeChange(LPTSTR lpDeviceName);
  void SetCurrentHandles();
  int GetOpenDocumentCount();
  // helpers for standard commdlg dialogs
  BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle,
      DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);
  int DoPrintDialog(CPrintDialog* pPD);
  void EnableModeless(BOOL bEnable); // to disable OLE in-place dialogs
  // overrides for implementation
  virtual BOOL InitInstance();
  virtual int ExitInstance(); // return app exit code
  virtual int Run();
  virtual BOOL OnIdle(LONG lCount); // return TRUE if more idle processing
  virtual LRESULT ProcessWndProcException(CException* e, const MSG* pMsg);
public:
  virtual ~CWinApp();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  // helpers for registration
  HKEY GetSectionKey(LPCTSTR lpszSection);
  HKEY GetAppRegistryKey();
protected:
  //{{AFX_MSG(CWinApp)
  afx_msg void OnAppExit();
  afx_msg void OnUpdateRecentFileMenu(CCmdUI* pCmdUI);
  afx_msg BOOL OnOpenRecentFile(UINT nID);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// class CWaitCursor
class CWaitCursor
{
  // Construction/Destruction
public:
  CWaitCursor();
  ~CWaitCursor();
  // Operations
public:
  void Restore();
};
/////////////////////////////////////////////////////////////////////////////
// class CDocTemplate creates documents
#ifdef _AFXDLL
class CDocTemplate : public CCmdTarget
#else
class AFX_NOVTABLE CDocTemplate : public CCmdTarget
#endif
{
  DECLARE_DYNAMIC(CDocTemplate)
  // Constructors
protected:
  CDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
      CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
public:
  virtual void LoadTemplate();
  // Attributes
public:
  // setup for OLE containers
  void SetContainerInfo(UINT nIDOleInPlaceContainer);
  // setup for OLE servers
  void SetServerInfo(UINT nIDOleEmbedding, UINT nIDOleInPlaceServer = 0,
      CRuntimeClass* pOleFrameClass = NULL, CRuntimeClass* pOleViewClass = NULL);
  // iterating over open documents
  virtual POSITION GetFirstDocPosition() const = 0;
  virtual CDocument* GetNextDoc(POSITION& rPos) const = 0;
  // Operations
public:
  virtual void AddDocument(CDocument* pDoc);      // must override
  virtual void RemoveDocument(CDocument* pDoc);   // must override
  enum DocStringIndex {
    windowTitle,        // default window title
    docName,            // user visible name for default document
    fileNewName,        // user visible name for FileNew
    // for file based documents:
    filterName,         // user visible name for FileOpen
    filterExt,          // user visible extension for FileOpen
    // for file based documents with Shell open support:
    regFileTypeId,      // REGEDIT visible registered file type identifier
    regFileTypeName,    // Shell visible registered file type name
  };
  virtual BOOL GetDocString(CString& rString,
      enum DocStringIndex index) const; // get one of the info strings
  CFrameWnd* CreateOleFrame(CWnd* pParentWnd, CDocument* pDoc,
      BOOL bCreateView);
  // Overridables
public:
  enum Confidence {
    noAttempt,
    maybeAttemptForeign,
    maybeAttemptNative,
    yesAttemptForeign,
    yesAttemptNative,
    yesAlreadyOpen
  };
  virtual Confidence MatchDocType(LPCTSTR lpszPathName,
      CDocument*& rpDocMatch);
  virtual CDocument* CreateNewDocument();
  virtual CFrameWnd* CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther);
  virtual void InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc,
      BOOL bMakeVisible = TRUE);
  virtual BOOL SaveAllModified();     // for all documents
  virtual void CloseAllDocuments(BOOL bEndSession);
  virtual CDocument* OpenDocumentFile(
      LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE) = 0;
  // open named file
  // if lpszPathName == NULL => create new file with this type
  virtual void SetDefaultTitle(CDocument* pDocument) = 0;
  // Implementation
public:
  BOOL m_bAutoDelete;
  virtual ~CDocTemplate();
  // back pointer to OLE or other server (NULL if none or disabled)
  CObject* m_pAttachedFactory;
  // menu & accelerator resources for in-place container
  HMENU m_hMenuInPlace;
  HACCEL m_hAccelInPlace;
  // menu & accelerator resource for server editing embedding
  HMENU m_hMenuEmbedding;
  HACCEL m_hAccelEmbedding;
  // menu & accelerator resource for server editing in-place
  HMENU m_hMenuInPlaceServer;
  HACCEL m_hAccelInPlaceServer;
#ifdef _DEBUG
  virtual void Dump(CDumpContext&) const;
  virtual void AssertValid() const;
#endif
  virtual void OnIdle();             // for all documents
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
      AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
  UINT m_nIDResource;                 // IDR_ for frame/menu/accel as well
  UINT m_nIDServerResource;           // IDR_ for OLE inplace frame/menu/accel
  UINT m_nIDEmbeddingResource;        // IDR_ for OLE open frame/menu/accel
  UINT m_nIDContainerResource;        // IDR_ for container frame/menu/accel
  CRuntimeClass* m_pDocClass;         // class for creating new documents
  CRuntimeClass* m_pFrameClass;       // class for creating new frames
  CRuntimeClass* m_pViewClass;        // class for creating new views
  CRuntimeClass* m_pOleFrameClass;    // class for creating in-place frame
  CRuntimeClass* m_pOleViewClass;     // class for creating in-place view
  CString m_strDocStrings;    // '\n' separated names
  // The document names sub-strings are represented as _one_ string:
  // windowTitle\ndocName\n ... (see DocStringIndex enum)
};
// SDI support (1 document only)
class CSingleDocTemplate : public CDocTemplate
{
  DECLARE_DYNAMIC(CSingleDocTemplate)
  // Constructors
public:
  CSingleDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
      CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
  // Implementation
public:
  virtual ~CSingleDocTemplate();
  virtual void AddDocument(CDocument* pDoc);
  virtual void RemoveDocument(CDocument* pDoc);
  virtual POSITION GetFirstDocPosition() const;
  virtual CDocument* GetNextDoc(POSITION& rPos) const;
  virtual CDocument* OpenDocumentFile(
      LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
  virtual void SetDefaultTitle(CDocument* pDocument);
#ifdef _DEBUG
  virtual void Dump(CDumpContext&) const;
  virtual void AssertValid() const;
#endif //_DEBUG
protected:  // standard implementation
  CDocument* m_pOnlyDoc;
};
// MDI support (zero or more documents)
class CMultiDocTemplate : public CDocTemplate
{
  DECLARE_DYNAMIC(CMultiDocTemplate)
  // Constructors
public:
  CMultiDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
      CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
  // Implementation
public:
  // Menu and accel table for MDI Child windows of this type
  HMENU m_hMenuShared;
  HACCEL m_hAccelTable;
  virtual ~CMultiDocTemplate();
  virtual void LoadTemplate();
  virtual void AddDocument(CDocument* pDoc);
  virtual void RemoveDocument(CDocument* pDoc);
  virtual POSITION GetFirstDocPosition() const;
  virtual CDocument* GetNextDoc(POSITION& rPos) const;
  virtual CDocument* OpenDocumentFile(
      LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);
  virtual void SetDefaultTitle(CDocument* pDocument);
#ifdef _DEBUG
  virtual void Dump(CDumpContext&) const;
  virtual void AssertValid() const;
#endif //_DEBUG
protected:  // standard implementation
  CPtrList m_docList;          // open documents of this type
  UINT m_nUntitledCount;   // start at 0, for "Document1" title
};
/////////////////////////////////////////////////////////////////////////////
// class CDocument is the main document data abstraction
#ifdef _AFXDLL
class CDocument : public CCmdTarget
#else
class AFX_NOVTABLE CDocument : public CCmdTarget
#endif
{
  DECLARE_DYNAMIC(CDocument)
public:
  // Constructors
  CDocument();
  // Attributes
public:
  const CString& GetTitle() const;
  virtual void SetTitle(LPCTSTR lpszTitle);
  const CString& GetPathName() const;
  virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
  CDocTemplate* GetDocTemplate() const;
  virtual BOOL IsModified();
  virtual void SetModifiedFlag(BOOL bModified = TRUE);
  // Operations
  void AddView(CView* pView);
  void RemoveView(CView* pView);
  virtual POSITION GetFirstViewPosition() const;
  virtual CView* GetNextView(POSITION& rPosition) const;
  // Update Views (simple update - DAG only)
  void UpdateAllViews(CView* pSender, LPARAM lHint = 0L,
      CObject* pHint = NULL);
  // Overridables
  // Special notifications
  virtual void OnChangedViewList(); // after Add or Remove view
  virtual void DeleteContents(); // delete doc items etc
  // File helpers
  virtual BOOL OnNewDocument();
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
  virtual void OnCloseDocument();
  virtual void ReportSaveLoadException(LPCTSTR lpszPathName,
      CException* e, BOOL bSaving, UINT nIDPDefault);
  virtual CFile* GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,
      CFileException* pError);
  virtual void ReleaseFile(CFile* pFile, BOOL bAbort);
  // advanced overridables, closing down frame/doc, etc.
  virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
  virtual BOOL SaveModified(); // return TRUE if ok to continue
  virtual void PreCloseFrame(CFrameWnd* pFrame);
  // Implementation
protected:
  // default implementation
  CString m_strTitle;
  CString m_strPathName;
  CDocTemplate* m_pDocTemplate;
  CPtrList m_viewList;                // list of views
  BOOL m_bModified;                   // changed since last saved
public:
  BOOL m_bAutoDelete;     // TRUE => delete document when no more views
  BOOL m_bEmbedded;       // TRUE => document is being created by OLE
#ifdef _DEBUG
  virtual void Dump(CDumpContext&) const;
  virtual void AssertValid() const;
#endif //_DEBUG
  virtual ~CDocument();
  // implementation helpers
  virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
  virtual BOOL DoFileSave();
  virtual void UpdateFrameCounts();
  void DisconnectViews();
  void SendInitialUpdate();
  // overridables for implementation
  virtual HMENU GetDefaultMenu(); // get menu depending on state
  virtual HACCEL GetDefaultAccelerator();
  virtual void OnIdle();
  virtual void OnFinalRelease();
  virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
      AFX_CMDHANDLERINFO* pHandlerInfo);
  friend class CDocTemplate;
protected:
  // file menu commands
  //{{AFX_MSG(CDocument)
  afx_msg void OnFileClose();
  afx_msg void OnFileSave();
  afx_msg void OnFileSaveAs();
  //}}AFX_MSG
  // mail enabling
  afx_msg void OnFileSendMail();
  afx_msg void OnUpdateFileSendMail(CCmdUI* pCmdUI);
  DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// Extra diagnostic tracing options
#ifdef _DEBUG
extern AFX_DATA UINT afxTraceFlags;
enum AfxTraceFlags {
  traceMultiApp = 1,      // multi-app debugging
  traceAppMsg = 2,        // main message pump trace (includes DDE)
  traceWinMsg = 4,        // Windows message tracing
  traceCmdRouting = 8,    // Windows command routing trace (set 4+8 for control notifications)
  traceOle = 16,          // special OLE callback trace
  traceDatabase = 32,     // special database trace
  traceInternet = 64      // special Internet client trace
};
#endif // _DEBUG
//////////////////////////////////////////////////////////////////////////////
// MessageBox helpers
void AFXAPI AfxFormatString1(CString& rString, UINT nIDS, LPCTSTR lpsz1);
void AFXAPI AfxFormatString2(CString& rString, UINT nIDS,
    LPCTSTR lpsz1, LPCTSTR lpsz2);
int AFXAPI AfxMessageBox(LPCTSTR lpszText, UINT nType = MB_OK,
    UINT nIDHelp = 0);
int AFXAPI AfxMessageBox(UINT nIDPrompt, UINT nType = MB_OK,
    UINT nIDHelp = (UINT) - 1);
// Implementation string helpers
void AFXAPI AfxFormatStrings(CString& rString, UINT nIDS,
    LPCTSTR const* rglpsz, int nString);
void AFXAPI AfxFormatStrings(CString& rString, LPCTSTR lpszFormat,
    LPCTSTR const* rglpsz, int nString);
BOOL AFXAPI AfxExtractSubString(CString& rString, LPCTSTR lpszFullString,
    int iSubString, TCHAR chSep = '\n');
/////////////////////////////////////////////////////////////////////////////
// Special target variant APIs
#ifdef _AFXDLL
#include <afxdll_.h>
#endif
// Windows Version compatibility (obsolete)
#define AfxEnableWin30Compatibility()
#define AfxEnableWin31Compatibility()
#define AfxEnableWin40Compatibility()
// Temporary map management (locks temp map on current thread)
void AFXAPI AfxLockTempMaps();
BOOL AFXAPI AfxUnlockTempMaps(BOOL bDeleteTemps = TRUE);
/////////////////////////////////////////////////////////////////////////////
// Special OLE related functions (see OLELOCK.CPP)
void AFXAPI AfxOleOnReleaseAllObjects();
BOOL AFXAPI AfxOleCanExitApp();
void AFXAPI AfxOleLockApp();
void AFXAPI AfxOleUnlockApp();
void AFXAPI AfxOleSetUserCtrl(BOOL bUserCtrl);
BOOL AFXAPI AfxOleGetUserCtrl();
#ifndef _AFX_NO_OCC_SUPPORT
BOOL AFXAPI AfxOleLockControl(REFCLSID clsid);
BOOL AFXAPI AfxOleUnlockControl(REFCLSID clsid);
BOOL AFXAPI AfxOleLockControl(LPCTSTR lpszProgID);
BOOL AFXAPI AfxOleUnlockControl(LPCTSTR lpszProgID);
void AFXAPI AfxOleUnlockAllControls();
#endif
/////////////////////////////////////////////////////////////////////////////
// Use version 1.0 of the RichEdit control
#define _RICHEDIT_VER 0x0100

/////////////////////////////////////////////////////////////////////////////
// static exceptions


/////////////////////////////////////////////////////////////////////////////
// Window class names and other window creation support

// from wincore.cpp
extern const TCHAR _afxWnd[];           // simple child windows/controls
extern const TCHAR _afxWndControlBar[]; // controls with gray backgrounds
extern const TCHAR _afxWndMDIFrame[];
extern const TCHAR _afxWndFrameOrView[];
extern const TCHAR _afxWndOleControl[];

#define AFX_WND_REG                     0x00001
#define AFX_WNDCONTROLBAR_REG           0x00002
#define AFX_WNDMDIFRAME_REG             0x00004
#define AFX_WNDFRAMEORVIEW_REG          0x00008
#define AFX_WNDCOMMCTLS_REG             0x00010 // means all original Win95
#define AFX_WNDOLECONTROL_REG           0x00020
#define AFX_WNDCOMMCTL_UPDOWN_REG       0x00040 // these are original Win95
#define AFX_WNDCOMMCTL_TREEVIEW_REG     0x00080
#define AFX_WNDCOMMCTL_TAB_REG          0x00100
#define AFX_WNDCOMMCTL_PROGRESS_REG     0x00200
#define AFX_WNDCOMMCTL_LISTVIEW_REG     0x00400
#define AFX_WNDCOMMCTL_HOTKEY_REG       0x00800
#define AFX_WNDCOMMCTL_BAR_REG          0x01000
#define AFX_WNDCOMMCTL_ANIMATE_REG      0x02000
#define AFX_WNDCOMMCTL_INTERNET_REG     0x04000 // these are new in IE4
#define AFX_WNDCOMMCTL_COOL_REG         0x08000
#define AFX_WNDCOMMCTL_USEREX_REG       0x10000
#define AFX_WNDCOMMCTL_DATE_REG         0x20000

#define AFX_WIN95CTLS_MASK              0x03FC0 // UPDOWN -> ANIMATE
#define AFX_WNDCOMMCTLSALL_REG          0x3C010 // COMMCTLS|INTERNET|COOL|USEREX|DATE
#define AFX_WNDCOMMCTLSNEW_REG          0x3C000 // INTERNET|COOL|USEREX|DATE

#define AfxDeferRegisterClass(fClass) AfxEndDeferRegisterClass(fClass)

BOOL AFXAPI AfxEndDeferRegisterClass(LONG fToRegister);

// MFC has its own version of the TOOLINFO structure containing the
// the Win95 base version of the structure. Since MFC targets Win95 base,
// we need this structure so calls into that old library don't fail.

typedef struct tagAFX_OLDTOOLINFO {
  UINT cbSize;
  UINT uFlags;
  HWND hwnd;
  UINT uId;
  RECT rect;
  HINSTANCE hinst;
  LPTSTR lpszText;
} AFX_OLDTOOLINFO;

// special AFX window class name mangling

#ifndef _UNICODE
#define _UNICODE_SUFFIX
#else
#define _UNICODE_SUFFIX _T("u")
#endif

#ifndef _DEBUG
#define _DEBUG_SUFFIX
#else
#define _DEBUG_SUFFIX _T("d")
#endif

#ifdef _AFXDLL
#define _STATIC_SUFFIX
#else
#define _STATIC_SUFFIX _T("s")
#endif

#define AFX_WNDCLASS(s) \
  _T("Afx") _T(s) _T("42") _STATIC_SUFFIX _UNICODE_SUFFIX _DEBUG_SUFFIX

#define AFX_WND             AFX_WNDCLASS("Wnd")
#define AFX_WNDCONTROLBAR   AFX_WNDCLASS("ControlBar")
#define AFX_WNDMDIFRAME     AFX_WNDCLASS("MDIFrame")
#define AFX_WNDFRAMEORVIEW  AFX_WNDCLASS("FrameOrView")
#define AFX_WNDOLECONTROL   AFX_WNDCLASS("OleControl")

// dialog/commdlg hook procs
BOOL CALLBACK AfxDlgProc(HWND, UINT, WPARAM, LPARAM);
UINT CALLBACK _AfxCommDlgProc(HWND hWnd, UINT, WPARAM, LPARAM);

// support for standard dialogs
extern UINT _afxMsgSETRGB;
typedef UINT(CALLBACK* COMMDLGPROC)(HWND, UINT, UINT, LONG);

/////////////////////////////////////////////////////////////////////////////
// Extended dialog templates (new in Win95)

#pragma pack(push, 1)

typedef struct {
  WORD dlgVer;
  WORD signature;
  DWORD helpID;
  DWORD exStyle;
  DWORD style;
  WORD cDlgItems;
  short x;
  short y;
  short cx;
  short cy;
} DLGTEMPLATEEX;

typedef struct {
  DWORD helpID;
  DWORD exStyle;
  DWORD style;
  short x;
  short y;
  short cx;
  short cy;
  DWORD id;
} DLGITEMTEMPLATEEX;

#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// Special helpers

void AFXAPI AfxCancelModes(HWND hWndRcvr);
HWND AFXAPI AfxGetParentOwner(HWND hWnd);
BOOL AFXAPI AfxIsDescendant(HWND hWndParent, HWND hWndChild);
BOOL AFXAPI AfxHelpEnabled();  // determine if ID_HELP handler exists
void AFXAPI AfxDeleteObject(HGDIOBJ* pObject);
BOOL AFXAPI AfxCustomLogFont(UINT nIDS, LOGFONT* pLogFont);
BOOL AFXAPI AfxGetPropSheetFont(CString& strFace, WORD& wSize, BOOL bWizard);

BOOL AFXAPI _AfxIsComboBoxControl(HWND hWnd, UINT nStyle);
BOOL AFXAPI _AfxCheckCenterDialog(LPCTSTR lpszResource);
BOOL AFXAPI _AfxCompareClassName(HWND hWnd, LPCTSTR lpszClassName);
HWND AFXAPI _AfxChildWindowFromPoint(HWND, POINT);

// for determining version of COMCTL32.DLL
#define VERSION_WIN4    MAKELONG(0, 4)
#define VERSION_IE3     MAKELONG(70, 4)
#define VERSION_IE4     MAKELONG(71, 4)
#define VERSION_IE401   MAKELONG(72, 4)
extern int _afxComCtlVersion;
DWORD AFXAPI _AfxGetComCtlVersion();


// UNICODE/MBCS abstractions
#ifdef _MBCS
extern AFX_DATA const BOOL _afxDBCS;
#else
#define _afxDBCS FALSE
#endif


// determine number of elements in an array (not bytes)
#define _countof(array) (sizeof(array)/sizeof(array[0]))

#ifndef _AFX_PORTABLE
int AFX_CDECL AfxCriticalNewHandler(size_t nSize);
#endif

void AFXAPI AfxGlobalFree(HGLOBAL hGlobal);

/////////////////////////////////////////////////////////////////////////////
// useful message ranges

#define WM_SYSKEYFIRST  WM_SYSKEYDOWN
#define WM_SYSKEYLAST   WM_SYSDEADCHAR

#define WM_NCMOUSEFIRST WM_NCMOUSEMOVE
#define WM_NCMOUSELAST  WM_NCMBUTTONDBLCLK


/////////////////////////////////////////////////////////////////////////////
// AFX_CRITICAL_SECTION

/////////////////////////////////////////////////////////////////////////////
// Auxiliary System/Screen metrics

struct AUX_DATA {
  // system metrics
  int cxVScroll, cyHScroll;
  int cxIcon, cyIcon;

  int cxBorder2, cyBorder2;

  // device metrics for screen
  int cxPixelsPerInch, cyPixelsPerInch;

  // convenient system color
  HBRUSH hbrWindowFrame;
  HBRUSH hbrBtnFace;

  // color values of system colors used for CToolBar
  COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
  COLORREF clrBtnText, clrWindowFrame;

  // standard cursors
  HCURSOR hcurWait;
  HCURSOR hcurArrow;
  HCURSOR hcurHelp;       // cursor used in Shift+F1 help

  // special GDI objects allocated on demand
  HFONT   hStatusFont;
  HFONT   hToolTipsFont;
  HBITMAP hbmMenuDot;

  // other system information
  UINT    nWinVer;        // Major.Minor version numbers
  BOOL    bWin95;         // TRUE if Windows 95 (not NT)
  BOOL    bWin4;          // TRUE if Windows 4.0
  BOOL    bNotWin4;       // TRUE if not Windows 4.0
  BOOL    bSmCaption;     // TRUE if WS_EX_SMCAPTION is supported
  BOOL    bMarked4;       // TRUE if marked as 4.0

  // Implementation
  AUX_DATA();
  ~AUX_DATA();
  void UpdateSysColors();
  void UpdateSysMetrics();
};


extern AFX_DATA AUX_DATA afxData;

// Note: afxData.cxBorder and afxData.cyBorder aren't used anymore
#define CX_BORDER   1
#define CY_BORDER   1

// states for Shift+F1 hep mode
#define HELP_INACTIVE   0   // not in Shift+F1 help mode (must be 0)
#define HELP_ACTIVE     1   // in Shift+F1 help mode (non-zero)
#define HELP_ENTERING   2   // entering Shift+F1 help mode (non-zero)


// these globals are protected by the same critical section
#define CRIT_DYNLINKLIST    0
#define CRIT_RUNTIMECLASSLIST   0
#define CRIT_OBJECTFACTORYLIST  0
#define CRIT_LOCKSHARED 0
// these globals are not protected by independent critical sections
#define CRIT_REGCLASSLIST   1
#define CRIT_WAITCURSOR     2
#define CRIT_DROPSOURCE     3
#define CRIT_DROPTARGET     4
#define CRIT_RECTTRACKER    5
#define CRIT_EDITVIEW       6
#define CRIT_WINMSGCACHE    7
#define CRIT_HALFTONEBRUSH  8
#define CRIT_SPLITTERWND    9
#define CRIT_MINIFRAMEWND   10
#define CRIT_CTLLOCKLIST    11
#define CRIT_DYNDLLLOAD     12
#define CRIT_TYPELIBCACHE   13
#define CRIT_STOCKMASK      14
#define CRIT_ODBC           15
#define CRIT_PROCESSLOCAL   16
#define CRIT_MAX    17  // Note: above plus one!

#ifdef _MT
void AFXAPI AfxLockGlobals(int nLockType);
void AFXAPI AfxUnlockGlobals(int nLockType);
BOOL AFXAPI AfxCriticalInit();
void AFXAPI AfxCriticalTerm();
#else
#define AfxLockGlobals(nLockType)
#define AfxUnlockGlobals(nLockType)
#define AfxCriticalInit() (TRUE)
#define AfxCriticalTerm()
#endif

/////////////////////////////////////////////////////////////////////////////
// Portability abstractions

#define _AfxSetDlgCtrlID(hWnd, nID)     SetWindowLong(hWnd, GWL_ID, nID)
#define _AfxGetDlgCtrlID(hWnd)          ((UINT)(WORD)::GetDlgCtrlID(hWnd))

// misc helpers
BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);
BOOL AFXAPI AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2);

UINT AFXAPI AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);
UINT AFXAPI AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);
void AFX_CDECL AfxTimeToFileTime(const CTime& time, LPFILETIME pFileTime);
void AFXAPI AfxGetRoot(LPCTSTR lpszPath, CString& strRoot);

#ifndef _AFX_NO_OLE_SUPPORT
class AFX_COM
{
public:
  HRESULT CreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter,
      REFIID riid, LPVOID* ppv);
  HRESULT GetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
};

CString AFXAPI AfxStringFromCLSID(REFCLSID rclsid);
BOOL AFXAPI AfxGetInProcServer(LPCTSTR lpszCLSID, CString& str);
BOOL AFXAPI AfxResolveShortcut(CWnd* pWnd, LPCTSTR pszShortcutFile,
    LPTSTR pszPath, int cchPath);
#endif // _AFX_NO_OLE_SUPPORT

#define NULL_TLS ((DWORD)-1)

/////////////////////////////////////////////////////////////////////////////
// Message map and message dispatch

const AFX_MSGMAP_ENTRY* AFXAPI
AfxFindMessageEntry(const AFX_MSGMAP_ENTRY* lpEntry,
    UINT nMsg, UINT nCode, UINT nID);

union MessageMapFunctions {
  AFX_PMSG pfn;   // generic member function pointer

  // specific type safe variants for WM_COMMAND and WM_NOTIFY messages
  void (AFX_MSG_CALL CCmdTarget::*pfn_COMMAND)();
  BOOL (AFX_MSG_CALL CCmdTarget::*pfn_bCOMMAND)();
  void (AFX_MSG_CALL CCmdTarget::*pfn_COMMAND_RANGE)(UINT);
  BOOL (AFX_MSG_CALL CCmdTarget::*pfn_COMMAND_EX)(UINT);

  void (AFX_MSG_CALL CCmdTarget::*pfn_UPDATE_COMMAND_UI)(CCmdUI*);
  void (AFX_MSG_CALL CCmdTarget::*pfn_UPDATE_COMMAND_UI_RANGE)(CCmdUI*, UINT);
  void (AFX_MSG_CALL CCmdTarget::*pfn_OTHER)(void*);
  BOOL (AFX_MSG_CALL CCmdTarget::*pfn_OTHER_EX)(void*);

  void (AFX_MSG_CALL CCmdTarget::*pfn_NOTIFY)(NMHDR*, LRESULT*);
  BOOL (AFX_MSG_CALL CCmdTarget::*pfn_bNOTIFY)(NMHDR*, LRESULT*);
  void (AFX_MSG_CALL CCmdTarget::*pfn_NOTIFY_RANGE)(UINT, NMHDR*, LRESULT*);
  BOOL (AFX_MSG_CALL CCmdTarget::*pfn_NOTIFY_EX)(UINT, NMHDR*, LRESULT*);

  // type safe variant for thread messages

  void (AFX_MSG_CALL CWinThread::*pfn_THREAD)(WPARAM, LPARAM);

  // specific type safe variants for WM-style messages
  BOOL (AFX_MSG_CALL CWnd::*pfn_bD)(CDC*);
  BOOL (AFX_MSG_CALL CWnd::*pfn_bb)(BOOL);
  BOOL (AFX_MSG_CALL CWnd::*pfn_bWww)(CWnd*, UINT, UINT);
  BOOL (AFX_MSG_CALL CWnd::*pfn_bHELPINFO)(HELPINFO*);
  BOOL (AFX_MSG_CALL CWnd::*pfn_bWCDS)(CWnd*, COPYDATASTRUCT*);
  HBRUSH(AFX_MSG_CALL CWnd::*pfn_hDWw)(CDC*, CWnd*, UINT);
  HBRUSH(AFX_MSG_CALL CWnd::*pfn_hDw)(CDC*, UINT);
  int (AFX_MSG_CALL CWnd::*pfn_iwWw)(UINT, CWnd*, UINT);
  int (AFX_MSG_CALL CWnd::*pfn_iww)(UINT, UINT);
  int (AFX_MSG_CALL CWnd::*pfn_iWww)(CWnd*, UINT, UINT);
  int (AFX_MSG_CALL CWnd::*pfn_is)(LPTSTR);
  LRESULT(AFX_MSG_CALL CWnd::*pfn_lwl)(WPARAM, LPARAM);
  LRESULT(AFX_MSG_CALL CWnd::*pfn_lwwM)(UINT, UINT, CMenu*);
  void (AFX_MSG_CALL CWnd::*pfn_vv)(void);

  void (AFX_MSG_CALL CWnd::*pfn_vw)(UINT);
  void (AFX_MSG_CALL CWnd::*pfn_vww)(UINT, UINT);
  void (AFX_MSG_CALL CWnd::*pfn_vvii)(int, int);
  void (AFX_MSG_CALL CWnd::*pfn_vwww)(UINT, UINT, UINT);
  void (AFX_MSG_CALL CWnd::*pfn_vwii)(UINT, int, int);
  void (AFX_MSG_CALL CWnd::*pfn_vwl)(WPARAM, LPARAM);
  void (AFX_MSG_CALL CWnd::*pfn_vbWW)(BOOL, CWnd*, CWnd*);
  void (AFX_MSG_CALL CWnd::*pfn_vD)(CDC*);
  void (AFX_MSG_CALL CWnd::*pfn_vM)(CMenu*);
  void (AFX_MSG_CALL CWnd::*pfn_vMwb)(CMenu*, UINT, BOOL);

  void (AFX_MSG_CALL CWnd::*pfn_vW)(CWnd*);
  void (AFX_MSG_CALL CWnd::*pfn_vWww)(CWnd*, UINT, UINT);
  void (AFX_MSG_CALL CWnd::*pfn_vWp)(CWnd*, CPoint);
  void (AFX_MSG_CALL CWnd::*pfn_vWh)(CWnd*, HANDLE);
  void (AFX_MSG_CALL CWnd::*pfn_vwW)(UINT, CWnd*);
  void (AFX_MSG_CALL CWnd::*pfn_vwWb)(UINT, CWnd*, BOOL);
  void (AFX_MSG_CALL CWnd::*pfn_vwwW)(UINT, UINT, CWnd*);
  void (AFX_MSG_CALL CWnd::*pfn_vwwx)(UINT, UINT);
  void (AFX_MSG_CALL CWnd::*pfn_vs)(LPTSTR);
  void (AFX_MSG_CALL CWnd::*pfn_vOWNER)(int, LPTSTR);      // force return TRUE
  int (AFX_MSG_CALL CWnd::*pfn_iis)(int, LPTSTR);
  UINT(AFX_MSG_CALL CWnd::*pfn_wp)(CPoint);
  UINT(AFX_MSG_CALL CWnd::*pfn_wv)(void);
  void (AFX_MSG_CALL CWnd::*pfn_vPOS)(WINDOWPOS*);
  void (AFX_MSG_CALL CWnd::*pfn_vCALC)(BOOL, NCCALCSIZE_PARAMS*);
  void (AFX_MSG_CALL CWnd::*pfn_vwp)(UINT, CPoint);
  void (AFX_MSG_CALL CWnd::*pfn_vwwh)(UINT, UINT, HANDLE);
  BOOL (AFX_MSG_CALL CWnd::*pfn_bwsp)(UINT, short, CPoint);
  void (AFX_MSG_CALL CWnd::*pfn_vws)(UINT, LPCTSTR);
};

CHandleMap* PASCAL afxMapHWND(BOOL bCreate = FALSE);
CHandleMap* PASCAL afxMapHIMAGELIST(BOOL bCreate = FALSE);
CHandleMap* PASCAL afxMapHDC(BOOL bCreate = FALSE);
CHandleMap* PASCAL afxMapHGDIOBJ(BOOL bCreate = FALSE);
CHandleMap* PASCAL afxMapHMENU(BOOL bCreate = FALSE);

/////////////////////////////////////////////////////////////////////////////
// Debugging/Tracing helpers

#ifdef _DEBUG
void AFXAPI _AfxTraceMsg(LPCTSTR lpszPrefix, const MSG* pMsg);
BOOL AFXAPI _AfxCheckDialogTemplate(LPCTSTR lpszResource,
    BOOL bInvisibleChild);
#endif

/////////////////////////////////////////////////////////////////////////////
// byte-swapping helpers

#ifdef _AFX_BYTESWAP

struct _AFXWORD {
  BYTE WordBits[sizeof(WORD)];
};
struct _AFXDWORD {
  BYTE DwordBits[sizeof(DWORD)];
};

struct _AFXFLOAT {
  BYTE FloatBits[sizeof(float)];
};
struct _AFXDOUBLE {
  BYTE DoubleBits[sizeof(double)];
};

inline void _AfxByteSwap(WORD w, BYTE* pb)
{
  _AFXWORD wAfx;
  *(WORD*)&wAfx = w;

  ASSERT(sizeof(WORD) == 2);

  *pb++ = wAfx.WordBits[1];
  *pb = wAfx.WordBits[0];
}

inline void _AfxByteSwap(DWORD dw, BYTE* pb)
{
  _AFXDWORD dwAfx;
  *(DWORD*)&dwAfx = dw;

  ASSERT(sizeof(DWORD) == 4);

  *pb++ = dwAfx.DwordBits[3];
  *pb++ = dwAfx.DwordBits[2];
  *pb++ = dwAfx.DwordBits[1];
  *pb = dwAfx.DwordBits[0];
}

inline void _AfxByteSwap(float f, BYTE* pb)
{
  _AFXFLOAT fAfx;
  *(float*)&fAfx = f;

  ASSERT(sizeof(float) == 4);

  *pb++ = fAfx.FloatBits[3];
  *pb++ = fAfx.FloatBits[2];
  *pb++ = fAfx.FloatBits[1];
  *pb = fAfx.FloatBits[0];
}

inline void _AfxByteSwap(double d, BYTE* pb)
{
  _AFXDOUBLE dAfx;
  *(double*)&dAfx = d;

  ASSERT(sizeof(double) == 8);

  *pb++ = dAfx.DoubleBits[7];
  *pb++ = dAfx.DoubleBits[6];
  *pb++ = dAfx.DoubleBits[5];
  *pb++ = dAfx.DoubleBits[4];
  *pb++ = dAfx.DoubleBits[3];
  *pb++ = dAfx.DoubleBits[2];
  *pb++ = dAfx.DoubleBits[1];
  *pb = dAfx.DoubleBits[0];
}
#endif //_AFX_BYTESWAP

extern CNotSupportedException _simpleNotSupportedException;
extern CMemoryException _simpleMemoryException;
extern CUserException _simpleUserException;
extern CResourceException _simpleResourceException;



#ifdef _UNICODE
class CToolInfo : public tagTOOLINFOW
#else
class CToolInfo : public tagTOOLINFOA
#endif
{
public:
  TCHAR szText[256];
};

/////////////////////////////////////////////////////////////////////////////
// CDragListBox

class CDragListBox : public CListBox
{
  DECLARE_DYNAMIC(CDragListBox)

  // Constructors
public:
  CDragListBox();

  // Attributes
  int ItemFromPt(CPoint pt, BOOL bAutoScroll = TRUE) const;

  // Operations
  virtual void DrawInsert(int nItem);

  // Overridables
  virtual BOOL BeginDrag(CPoint pt);
  virtual void CancelDrag(CPoint pt);
  virtual UINT Dragging(CPoint pt);
  virtual void Dropped(int nSrcIndex, CPoint pt);

  // Implementation
public:
  int m_nLast;
  void DrawSingle(int nIndex);
  virtual void PreSubclassWindow();
  virtual ~CDragListBox();
protected:
  virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
};

/////////////////////////////////////////////////////////////////////////////
// CStatusBarCtrl

class CStatusBarCtrl : public CWnd
{
  DECLARE_DYNAMIC(CStatusBarCtrl)

  // Constructors
public:
  CStatusBarCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  BOOL SetText(LPCTSTR lpszText, int nPane, int nType);
  CString GetText(int nPane, int* pType = NULL) const;
  int GetText(LPCTSTR lpszText, int nPane, int* pType = NULL) const;
  int GetTextLength(int nPane, int* pType = NULL) const;
  BOOL SetParts(int nParts, int* pWidths);
  int GetParts(int nParts, int* pParts) const;
  BOOL GetBorders(int* pBorders) const;
  BOOL GetBorders(int& nHorz, int& nVert, int& nSpacing) const;
  void SetMinHeight(int nMin);
  BOOL SetSimple(BOOL bSimple = TRUE);
  BOOL GetRect(int nPane, LPRECT lpRect) const;
  BOOL IsSimple() const;
  CString GetTipText(int nPane) const;
  void SetTipText(int nPane, LPCTSTR pszTipText);
  COLORREF SetBkColor(COLORREF cr);
  BOOL SetIcon(int nPane, HICON hIcon);

  // Overridables
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

  // Implementation
public:
  virtual ~CStatusBarCtrl();
protected:
  virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
};

/////////////////////////////////////////////////////////////////////////////
// CListCtrl

class CListCtrl : public CWnd
{
  DECLARE_DYNAMIC(CListCtrl)

  // Constructors
public:
  CListCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  COLORREF GetBkColor() const;
  BOOL SetBkColor(COLORREF cr);
  CImageList* GetImageList(int nImageList) const;
  CImageList* SetImageList(CImageList* pImageList, int nImageListType);
  int GetItemCount() const;
  BOOL GetItem(LVITEM* pItem) const;
  BOOL SetItem(const LVITEM* pItem);
  BOOL SetItem(int nItem, int nSubItem, UINT nMask, LPCTSTR lpszItem,
      int nImage, UINT nState, UINT nStateMask, LPARAM lParam);
  UINT GetCallbackMask() const;
  BOOL SetCallbackMask(UINT nMask);
  int GetNextItem(int nItem, int nFlags) const;
  POSITION GetFirstSelectedItemPosition() const;
  int GetNextSelectedItem(POSITION& pos) const;
  BOOL GetItemRect(int nItem, LPRECT lpRect, UINT nCode) const;
  BOOL SetItemPosition(int nItem, POINT pt);
  BOOL GetItemPosition(int nItem, LPPOINT lpPoint) const;
  int GetStringWidth(LPCTSTR lpsz) const;
  CEdit* GetEditControl() const;
  BOOL GetColumn(int nCol, LVCOLUMN* pColumn) const;
  BOOL SetColumn(int nCol, const LVCOLUMN* pColumn);
  int GetColumnWidth(int nCol) const;
  BOOL SetColumnWidth(int nCol, int cx);
  BOOL GetViewRect(LPRECT lpRect) const;
  COLORREF GetTextColor() const;
  BOOL SetTextColor(COLORREF cr);
  COLORREF GetTextBkColor() const;
  BOOL SetTextBkColor(COLORREF cr);
  int GetTopIndex() const;
  int GetCountPerPage() const;
  BOOL GetOrigin(LPPOINT lpPoint) const;
  BOOL SetItemState(int nItem, LVITEM* pItem);
  BOOL SetItemState(int nItem, UINT nState, UINT nMask);
  UINT GetItemState(int nItem, UINT nMask) const;
  CString GetItemText(int nItem, int nSubItem) const;
  int GetItemText(int nItem, int nSubItem, LPTSTR lpszText, int nLen) const;
  BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
  void SetItemCount(int nItems);
  BOOL SetItemData(int nItem, DWORD dwData);
  DWORD GetItemData(int nItem) const;
  UINT GetSelectedCount() const;
  BOOL SetColumnOrderArray(int iCount, LPINT piArray);
  BOOL GetColumnOrderArray(LPINT piArray, int iCount = -1);
  CSize SetIconSpacing(CSize size);
  CSize SetIconSpacing(int cx, int cy);
  CHeaderCtrl* GetHeaderCtrl();
  HCURSOR GetHotCursor();
  HCURSOR SetHotCursor(HCURSOR hc);
  BOOL GetSubItemRect(int iItem, int iSubItem, int nArea, CRect& ref);
  int GetHotItem();
  int SetHotItem(int iIndex);
  int GetSelectionMark();
  int SetSelectionMark(int iIndex);
  DWORD GetExtendedStyle();
  DWORD SetExtendedStyle(DWORD dwNewStyle);
  int SubItemHitTest(LPLVHITTESTINFO pInfo);
  void SetWorkAreas(int nWorkAreas, LPRECT lpRect);
  BOOL SetItemCountEx(int iCount, DWORD dwFlags = LVSICF_NOINVALIDATEALL);
  CSize ApproximateViewRect(CSize sz = CSize(-1, -1), int iCount = -1) const;
  BOOL GetBkImage(LVBKIMAGE* plvbkImage) const;
  DWORD GetHoverTime() const;
  void GetWorkAreas(int nWorkAreas, LPRECT prc) const;
  BOOL SetBkImage(HBITMAP hbm, BOOL fTile = TRUE, int xOffsetPercent = 0, int yOffsetPercent = 0);
  BOOL SetBkImage(LPTSTR pszUrl, BOOL fTile = TRUE, int xOffsetPercent = 0, int yOffsetPercent = 0);
  BOOL SetBkImage(LVBKIMAGE* plvbkImage);
  DWORD SetHoverTime(DWORD dwHoverTime = (DWORD) - 1);
  UINT GetNumberOfWorkAreas() const;
  BOOL GetCheck(int nItem) const;
  BOOL SetCheck(int nItem, BOOL fCheck = TRUE);

  // Operations
  int InsertItem(const LVITEM* pItem);
  int InsertItem(int nItem, LPCTSTR lpszItem);
  int InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
  BOOL DeleteItem(int nItem);
  BOOL DeleteAllItems();
  int FindItem(LVFINDINFO* pFindInfo, int nStart = -1) const;
  int HitTest(LVHITTESTINFO* pHitTestInfo) const;
  int HitTest(CPoint pt, UINT* pFlags = NULL) const;
  BOOL EnsureVisible(int nItem, BOOL bPartialOK);
  BOOL Scroll(CSize size);
  BOOL RedrawItems(int nFirst, int nLast);
  BOOL Arrange(UINT nCode);
  CEdit* EditLabel(int nItem);
  int InsertColumn(int nCol, const LVCOLUMN* pColumn);
  int InsertColumn(int nCol, LPCTSTR lpszColumnHeading,
      int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
  BOOL DeleteColumn(int nCol);
  CImageList* CreateDragImage(int nItem, LPPOINT lpPoint);
  BOOL Update(int nItem);
  BOOL SortItems(PFNLVCOMPARE pfnCompare, DWORD dwData);

  // Overridables
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

  // Implementation
public:
  int InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem, UINT nState,
      UINT nStateMask, int nImage, LPARAM lParam);
  virtual ~CListCtrl();
protected:
  void RemoveImageList(int nImageList);
  virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
protected:
  //{{AFX_MSG(CListCtrl)
  afx_msg void OnNcDestroy();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrl

class CTreeCtrl : public CWnd
{
  DECLARE_DYNAMIC(CTreeCtrl)

  // Constructors
public:
  CTreeCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  BOOL GetItemRect(HTREEITEM hItem, LPRECT lpRect, BOOL bTextOnly) const;
  UINT GetCount() const;
  UINT GetIndent() const;
  void SetIndent(UINT nIndent);
  CImageList* GetImageList(UINT nImageList) const;
  CImageList* SetImageList(CImageList* pImageList, int nImageListType);
  HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode) const;
  HTREEITEM GetChildItem(HTREEITEM hItem) const;
  HTREEITEM GetNextSiblingItem(HTREEITEM hItem) const;
  HTREEITEM GetPrevSiblingItem(HTREEITEM hItem) const;
  HTREEITEM GetParentItem(HTREEITEM hItem) const;
  HTREEITEM GetFirstVisibleItem() const;
  HTREEITEM GetNextVisibleItem(HTREEITEM hItem) const;
  HTREEITEM GetPrevVisibleItem(HTREEITEM hItem) const;
  HTREEITEM GetSelectedItem() const;
  HTREEITEM GetDropHilightItem() const;
  HTREEITEM GetRootItem() const;
  BOOL GetItem(TVITEM* pItem) const;
  CString GetItemText(HTREEITEM hItem) const;
  BOOL GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage) const;
  UINT GetItemState(HTREEITEM hItem, UINT nStateMask) const;
  DWORD GetItemData(HTREEITEM hItem) const;
  BOOL SetItem(TVITEM* pItem);
  BOOL SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR lpszItem, int nImage,
      int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam);
  BOOL SetItemText(HTREEITEM hItem, LPCTSTR lpszItem);
  BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage);
  BOOL SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask);
  BOOL SetItemData(HTREEITEM hItem, DWORD dwData);
  BOOL ItemHasChildren(HTREEITEM hItem) const;
  CEdit* GetEditControl() const;
  UINT GetVisibleCount() const;
  CToolTipCtrl* GetToolTips() const;
  CToolTipCtrl* SetToolTips(CToolTipCtrl* pWndTip);
  COLORREF GetBkColor() const;
  COLORREF SetBkColor(COLORREF clr);
  SHORT GetItemHeight() const;
  SHORT SetItemHeight(SHORT cyHeight);
  COLORREF GetTextColor() const;
  COLORREF SetTextColor(COLORREF clr);
  BOOL SetInsertMark(HTREEITEM hItem, BOOL fAfter = TRUE);
  BOOL GetCheck(HTREEITEM hItem) const;
  BOOL SetCheck(HTREEITEM hItem, BOOL fCheck = TRUE);
  COLORREF GetInsertMarkColor() const;
  COLORREF SetInsertMarkColor(COLORREF clrNew);

  // Operations
  HTREEITEM InsertItem(LPTVINSERTSTRUCT lpInsertStruct);
  HTREEITEM InsertItem(UINT nMask, LPCTSTR lpszItem, int nImage,
      int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
      HTREEITEM hParent, HTREEITEM hInsertAfter);
  HTREEITEM InsertItem(LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT,
      HTREEITEM hInsertAfter = TVI_LAST);
  HTREEITEM InsertItem(LPCTSTR lpszItem, int nImage, int nSelectedImage,
      HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
  BOOL DeleteItem(HTREEITEM hItem);
  BOOL DeleteAllItems();
  BOOL Expand(HTREEITEM hItem, UINT nCode);
  BOOL Select(HTREEITEM hItem, UINT nCode);
  BOOL SelectItem(HTREEITEM hItem);
  BOOL SelectDropTarget(HTREEITEM hItem);
  BOOL SelectSetFirstVisible(HTREEITEM hItem);
  CEdit* EditLabel(HTREEITEM hItem);
  HTREEITEM HitTest(CPoint pt, UINT* pFlags = NULL) const;
  HTREEITEM HitTest(TVHITTESTINFO* pHitTestInfo) const;
  CImageList* CreateDragImage(HTREEITEM hItem);
  BOOL SortChildren(HTREEITEM hItem);
  BOOL EnsureVisible(HTREEITEM hItem);
  BOOL SortChildrenCB(LPTVSORTCB pSort);

  // Implementation
protected:
  void RemoveImageList(int nImageList);
public:
  virtual ~CTreeCtrl();
  //{{AFX_MSG(CTreeCtrl)
  afx_msg void OnDestroy();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSpinButtonCtrl

class CSpinButtonCtrl : public CWnd
{
  DECLARE_DYNAMIC(CSpinButtonCtrl)

  // Constructors
public:
  CSpinButtonCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  BOOL SetAccel(int nAccel, UDACCEL* pAccel);
  UINT GetAccel(int nAccel, UDACCEL* pAccel) const;
  int SetBase(int nBase);
  UINT GetBase() const;
  CWnd* SetBuddy(CWnd* pWndBuddy);
  CWnd* GetBuddy() const;
  int SetPos(int nPos);
  int GetPos() const;
  void SetRange(int nLower, int nUpper);
  void SetRange32(int nLower, int nUpper);
  DWORD GetRange() const;
  void GetRange(int& lower, int& upper) const;
  void GetRange32(int& lower, int& upper) const;

  // Implementation
public:
  virtual ~CSpinButtonCtrl();
};

/////////////////////////////////////////////////////////////////////////////
// CSliderCtrl

class CSliderCtrl : public CWnd
{
  DECLARE_DYNAMIC(CSliderCtrl)

  // Constructors
public:
  CSliderCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  int GetLineSize() const;
  int SetLineSize(int nSize);
  int GetPageSize() const;
  int SetPageSize(int nSize);
  int GetRangeMax() const;
  int GetRangeMin() const;
  void GetRange(int& nMin, int& nMax) const;
  void SetRangeMin(int nMin, BOOL bRedraw = FALSE);
  void SetRangeMax(int nMax, BOOL bRedraw = FALSE);
  void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);
  void GetSelection(int& nMin, int& nMax) const;
  void SetSelection(int nMin, int nMax);
  void GetChannelRect(LPRECT lprc) const;
  void GetThumbRect(LPRECT lprc) const;
  int GetPos() const;
  void SetPos(int nPos);
  UINT GetNumTics() const;
  DWORD* GetTicArray() const;
  int GetTic(int nTic) const;
  int GetTicPos(int nTic) const;
  BOOL SetTic(int nTic);
  void SetTicFreq(int nFreq);
  CWnd* GetBuddy(BOOL fLocation = TRUE) const;
  CWnd* SetBuddy(CWnd* pWndBuddy, BOOL fLocation = TRUE);
  CToolTipCtrl* GetToolTips() const;
  void SetToolTips(CToolTipCtrl* pWndTip);
  int SetTipSide(int nLocation);

  // Operations
  void ClearSel(BOOL bRedraw = FALSE);
  void VerifyPos();
  void ClearTics(BOOL bRedraw = FALSE);

  // Implementation
public:
  virtual ~CSliderCtrl();
};

/////////////////////////////////////////////////////////////////////////////
// CProgressCtrl

class CProgressCtrl : public CWnd
{
  DECLARE_DYNAMIC(CProgressCtrl)

  // Constructors
public:
  CProgressCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  void SetRange(short nLower, short nUpper);
  void SetRange32(int nLower, int nUpper);
  void GetRange(int& nLower, int& nUpper);
  int GetPos();
  int SetPos(int nPos);
  int OffsetPos(int nPos);
  int SetStep(int nStep);

  // Operations
  int StepIt();

  // Implementation
public:
  virtual ~CProgressCtrl();
};

/////////////////////////////////////////////////////////////////////////////
// CComboBoxEx

class CComboBoxEx : public CComboBox
{
  DECLARE_DYNAMIC(CComboBoxEx)

  // Constructors
public:
  CComboBoxEx();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Operations
  int DeleteItem(int iIndex);
  BOOL GetItem(COMBOBOXEXITEM* pCBItem);
  int InsertItem(const COMBOBOXEXITEM* pCBItem);
  BOOL SetItem(const COMBOBOXEXITEM* pCBItem);

  // Attributes
  BOOL HasEditChanged();
  DWORD GetExtendedStyle() const;
  DWORD SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles);
  CEdit* GetEditCtrl();
  CComboBox* GetComboBoxCtrl();
  CImageList* GetImageList() const;
  CImageList* SetImageList(CImageList* pImageList);

  // These functions are supported by the Windows ComboBox control,
  // but not supported by the Windows ComboBoxEx control.

  int Dir(UINT attr, LPCTSTR lpszWildCard);
  int FindString(int nIndexStart, LPCTSTR lpszFind) const;
  int AddString(LPCTSTR lpszString);
  BOOL SetEditSel(int nStartChar, int nEndChar);
  int InsertString(int nIndex, LPCTSTR lpszString);

  // Implementation
public:
  virtual ~CComboBoxEx();
};

/////////////////////////////////////////////////////////////////////////////
// CHeaderCtrl

class CHeaderCtrl : public CWnd
{
  DECLARE_DYNAMIC(CHeaderCtrl)

  // Constructors
public:
  CHeaderCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  int GetItemCount() const;
  BOOL GetItem(int nPos, HDITEM* pHeaderItem) const;
  BOOL SetItem(int nPos, HDITEM* pHeaderItem);
  CImageList* GetImageList() const;
  CImageList* SetImageList(CImageList* pImageList);
  BOOL GetItemRect(int nIndex, LPRECT lpRect) const;
  BOOL GetOrderArray(LPINT piArray, int iCount = -1);
  BOOL SetOrderArray(int iCount, LPINT piArray);
  int OrderToIndex(int nOrder) const;

  // Operations
  int InsertItem(int nPos, HDITEM* phdi);
  BOOL DeleteItem(int nPos);
  BOOL Layout(HDLAYOUT* pHeaderLayout);
  CImageList* CreateDragImage(int nIndex);
  int SetHotDivider(CPoint pt);
  int SetHotDivider(int nIndex);

  // Overridables
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

  // Implementation
public:
  virtual ~CHeaderCtrl();
protected:
  virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);

};

/////////////////////////////////////////////////////////////////////////////
// CHotKeyCtrl

class CHotKeyCtrl : public CWnd
{
  DECLARE_DYNAMIC(CHotKeyCtrl)

  // Constructors
public:
  CHotKeyCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers);
  DWORD GetHotKey() const;
  void GetHotKey(WORD& wVirtualKeyCode, WORD& wModifiers) const;

  // Operations
  void SetRules(WORD wInvalidComb, WORD wModifiers);

  // Implementation
public:
  virtual ~CHotKeyCtrl();
};

/////////////////////////////////////////////////////////////////////////////
// CToolTipCtrl

class CToolTipCtrl : public CWnd
{
  DECLARE_DYNAMIC(CToolTipCtrl)

  // Constructors
public:
  CToolTipCtrl();
  BOOL Create(CWnd* pParentWnd, DWORD dwStyle = 0);

  // Attributes
  void GetText(CString& str, CWnd* pWnd, UINT nIDTool = 0) const;
  BOOL GetToolInfo(CToolInfo& ToolInfo, CWnd* pWnd, UINT nIDTool = 0) const;
  void SetToolInfo(LPTOOLINFO lpToolInfo);
  void SetToolRect(CWnd* pWnd, UINT nIDTool, LPCRECT lpRect);
  int GetToolCount() const;
  int GetDelayTime(DWORD dwDuration) const;
  void SetDelayTime(DWORD dwDuration, int iTime);
  void GetMargin(LPRECT lprc) const;
  void SetMargin(LPRECT lprc);
  int GetMaxTipWidth() const;
  int SetMaxTipWidth(int iWidth);
  COLORREF GetTipBkColor() const;
  void SetTipBkColor(COLORREF clr);
  COLORREF GetTipTextColor() const;
  void SetTipTextColor(COLORREF clr);

  // Operations
  void Activate(BOOL bActivate);

  BOOL AddTool(CWnd* pWnd, UINT nIDText, LPCRECT lpRectTool = NULL,
      UINT nIDTool = 0);
  BOOL AddTool(CWnd* pWnd, LPCTSTR lpszText = LPSTR_TEXTCALLBACK,
      LPCRECT lpRectTool = NULL, UINT nIDTool = 0);

  void DelTool(CWnd* pWnd, UINT nIDTool = 0);

  BOOL HitTest(CWnd* pWnd, CPoint pt, LPTOOLINFO lpToolInfo) const;
  void RelayEvent(LPMSG lpMsg);
  void SetDelayTime(UINT nDelay);
  void UpdateTipText(LPCTSTR lpszText, CWnd* pWnd, UINT nIDTool = 0);
  void UpdateTipText(UINT nIDText, CWnd* pWnd, UINT nIDTool = 0);
  void Update();

  void Pop();

  // Implementation
public:
  void FillInToolInfo(TOOLINFO& ti, CWnd* pWnd, UINT nIDTool) const;
  virtual ~CToolTipCtrl();
#ifndef _AFXDLL
  virtual BOOL DestroyToolTipCtrl();
#else
  BOOL DestroyToolTipCtrl();
#endif

protected:
  //{{AFX_MSG(CToolTipCtrl)
  afx_msg LRESULT OnDisableModal(WPARAM, LPARAM);
  afx_msg LRESULT OnWindowFromPoint(WPARAM, LPARAM);
  afx_msg LRESULT OnAddTool(WPARAM, LPARAM);
  afx_msg void OnEnable(BOOL bEnable);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  CMapStringToPtr m_mapString;

  friend class CWnd;
  friend class CToolBar;
};

/////////////////////////////////////////////////////////////////////////////
// CTabCtrl

class CTabCtrl : public CWnd
{
  DECLARE_DYNAMIC(CTabCtrl)

  // Constructors
public:
  CTabCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  CImageList* GetImageList() const;
  CImageList* SetImageList(CImageList* pImageList);
  int GetItemCount() const;
  BOOL GetItem(int nItem, TCITEM* pTabCtrlItem) const;
  BOOL SetItem(int nItem, TCITEM* pTabCtrlItem);
  BOOL SetItemExtra(int nBytes);
  BOOL GetItemRect(int nItem, LPRECT lpRect) const;
  int GetCurSel() const;
  int SetCurSel(int nItem);
  void SetCurFocus(int nItem);
  CSize SetItemSize(CSize size);
  void SetPadding(CSize size);
  int GetRowCount() const;
  CToolTipCtrl* GetToolTips() const;
  void SetToolTips(CToolTipCtrl* pWndTip);
  int GetCurFocus() const;
  int SetMinTabWidth(int cx);
  DWORD GetExtendedStyle();
  DWORD SetExtendedStyle(DWORD dwNewStyle, DWORD dwExMask = 0);
  DWORD GetItemState(int nItem, DWORD dwMask) const;
  BOOL SetItemState(int nItem, DWORD dwMask, DWORD dwState);

  // Operations
  BOOL InsertItem(int nItem, TCITEM* pTabCtrlItem);
  BOOL InsertItem(int nItem, LPCTSTR lpszItem);
  BOOL InsertItem(int nItem, LPCTSTR lpszItem, int nImage);
  BOOL InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem,
      int nImage, LPARAM lParam);
  BOOL InsertItem(UINT nMask, int nItem, LPCTSTR lpszItem,
      int nImage, LPARAM lParam, DWORD dwState, DWORD dwStateMask);
  BOOL DeleteItem(int nItem);
  BOOL DeleteAllItems();
  void AdjustRect(BOOL bLarger, LPRECT lpRect);
  void RemoveImage(int nImage);
  int HitTest(TCHITTESTINFO* pHitTestInfo) const;
  void DeselectAll(BOOL fExcludeFocus);
  BOOL HighlightItem(int idItem, BOOL fHighlight = TRUE);

  // Overridables
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

  // Implementation
public:
  virtual ~CTabCtrl();
protected:
  virtual BOOL OnChildNotify(UINT, WPARAM, LPARAM, LRESULT*);
  //{{AFX_MSG(CTabCtrl)
  afx_msg void OnDestroy();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAnimateCtrl

class CAnimateCtrl : public CWnd
{
  DECLARE_DYNAMIC(CAnimateCtrl)

  // Constructors
public:
  CAnimateCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Operations
  BOOL Open(LPCTSTR lpszFileName);
  BOOL Open(UINT nID);
  BOOL Play(UINT nFrom, UINT nTo, UINT nRep);
  BOOL Stop();
  BOOL Close();
  BOOL Seek(UINT nTo);

  // Implementation
public:
  virtual ~CAnimateCtrl();
};

/////////////////////////////////////////////////////////////////////////////
// CImageList

class CImageList : public CObject
{
  DECLARE_DYNCREATE(CImageList)

  // Constructors
public:
  CImageList();
  BOOL Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow);
  BOOL Create(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
  BOOL Create(LPCTSTR lpszBitmapID, int cx, int nGrow, COLORREF crMask);
  BOOL Create(CImageList& imagelist1, int nImage1, CImageList& imagelist2,
      int nImage2, int dx, int dy);
  BOOL Create(CImageList* pImageList);

  // Attributes
  HIMAGELIST m_hImageList;            // must be first data member
  operator HIMAGELIST() const;
  HIMAGELIST GetSafeHandle() const;

  static CImageList* PASCAL FromHandle(HIMAGELIST hImageList);
  static CImageList* PASCAL FromHandlePermanent(HIMAGELIST hImageList);
  static void PASCAL DeleteTempMap();
  BOOL Attach(HIMAGELIST hImageList);
  HIMAGELIST Detach();

  int GetImageCount() const;
  COLORREF SetBkColor(COLORREF cr);
  COLORREF GetBkColor() const;
  BOOL GetImageInfo(int nImage, IMAGEINFO* pImageInfo) const;

  // Operations
  BOOL DeleteImageList();
  BOOL SetImageCount(UINT uNewCount);

  int Add(CBitmap* pbmImage, CBitmap* pbmMask);
  int Add(CBitmap* pbmImage, COLORREF crMask);
  BOOL Remove(int nImage);
  BOOL Replace(int nImage, CBitmap* pbmImage, CBitmap* pbmMask);
  int Add(HICON hIcon);
  int Replace(int nImage, HICON hIcon);
  HICON ExtractIcon(int nImage);
  BOOL Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle);
  BOOL SetOverlayImage(int nImage, int nOverlay);
  BOOL Copy(int iDst, int iSrc, UINT uFlags = ILCF_MOVE);
  BOOL Copy(int iDst, CImageList* pSrc, int iSrc, UINT uFlags = ILCF_MOVE);
  BOOL DrawIndirect(IMAGELISTDRAWPARAMS* pimldp);
  BOOL DrawIndirect(CDC* pDC, int nImage, POINT pt, SIZE sz, POINT ptOrigin,
      UINT fStyle = ILD_NORMAL, DWORD dwRop = SRCCOPY,
      COLORREF rgbBack = CLR_DEFAULT, COLORREF rgbFore = CLR_DEFAULT);

#ifndef _AFX_NO_OLE_SUPPORT
  BOOL Read(CArchive* pArchive);
  BOOL Write(CArchive* pArchive);
#endif

  // Drag APIs
  BOOL BeginDrag(int nImage, CPoint ptHotSpot);
  static void PASCAL EndDrag();
  static BOOL PASCAL DragMove(CPoint pt);
  BOOL SetDragCursorImage(int nDrag, CPoint ptHotSpot);
  static BOOL PASCAL DragShowNolock(BOOL bShow);
  static CImageList* PASCAL GetDragImage(LPPOINT lpPoint, LPPOINT lpPointHotSpot);
  static BOOL PASCAL DragEnter(CWnd* pWndLock, CPoint point);
  static BOOL PASCAL DragLeave(CWnd* pWndLock);

  // Implementation
public:
  virtual ~CImageList();
#ifdef _DEBUG
  virtual void Dump(CDumpContext& dc) const;
  virtual void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CToolBarCtrl

struct IDropTarget; // forward declaration

class CToolBarCtrl : public CWnd
{
  DECLARE_DYNAMIC(CToolBarCtrl)
  // Construction
public:
  CToolBarCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
public:
  BOOL IsButtonEnabled(int nID) const;
  BOOL IsButtonChecked(int nID) const;
  BOOL IsButtonPressed(int nID) const;
  BOOL IsButtonHidden(int nID) const;
  BOOL IsButtonIndeterminate(int nID) const;
  BOOL SetState(int nID, UINT nState);
  int GetState(int nID) const;
  BOOL GetButton(int nIndex, LPTBBUTTON lpButton) const;
  int GetButtonCount() const;
  BOOL GetItemRect(int nIndex, LPRECT lpRect) const;
  BOOL GetRect(int nID, LPRECT lpRect) const;
  void SetButtonStructSize(int nSize);
  DWORD GetButtonSize() const;
  BOOL SetButtonSize(CSize size);
  BOOL SetBitmapSize(CSize size);
  CToolTipCtrl* GetToolTips() const;
  void SetToolTips(CToolTipCtrl* pTip);
  void SetOwner(CWnd* pWnd);
  void SetRows(int nRows, BOOL bLarger, LPRECT lpRect);
  int GetRows() const;
  BOOL SetCmdID(int nIndex, UINT nID);
  UINT GetBitmapFlags() const;
  CImageList* GetDisabledImageList() const;
  CImageList* GetHotImageList() const;
  CImageList* GetImageList() const;
  DWORD GetStyle() const;
  int GetMaxTextRows() const;
  BOOL IsButtonHighlighted(int nID) const;
  BOOL SetButtonWidth(int cxMin, int cxMax);
  CImageList* SetDisabledImageList(CImageList* pImageList);
  CImageList* SetHotImageList(CImageList* pImageList);
  CImageList* SetImageList(CImageList* pImageList);
  HRESULT GetDropTarget(IDropTarget** ppDropTarget) const;
  BOOL SetIndent(int iIndent);
  BOOL SetMaxTextRows(int iMaxRows);
  void SetStyle(DWORD dwStyle);
  BOOL GetButtonInfo(int nID, TBBUTTONINFO* ptbbi) const;
  BOOL SetButtonInfo(int nID, TBBUTTONINFO* ptbbi);
  DWORD SetDrawTextFlags(DWORD dwMask, DWORD dwDTFlags);
  BOOL GetAnchorHighlight() const;
  BOOL SetAnchorHighlight(BOOL fAnchor = TRUE);
  int GetHotItem() const;
  int SetHotItem(int nHot);
  void GetInsertMark(TBINSERTMARK* ptbim) const;
  void SetInsertMark(TBINSERTMARK* ptbim);
  BOOL GetMaxSize(LPSIZE pSize) const;
  BOOL InsertMarkHitTest(LPPOINT ppt, LPTBINSERTMARK ptbim) const;
  DWORD GetExtendedStyle() const;
  DWORD SetExtendedStyle(DWORD dwExStyle);
  COLORREF GetInsertMarkColor() const;
  COLORREF SetInsertMarkColor(COLORREF clrNew);

  // Operations
public:
  BOOL EnableButton(int nID, BOOL bEnable = TRUE);
  BOOL CheckButton(int nID, BOOL bCheck = TRUE);
  BOOL PressButton(int nID, BOOL bPress = TRUE);
  BOOL HideButton(int nID, BOOL bHide = TRUE);
  BOOL Indeterminate(int nID, BOOL bIndeterminate = TRUE);
  int AddBitmap(int nNumButtons, UINT nBitmapID);
  int AddBitmap(int nNumButtons, CBitmap* pBitmap);
  BOOL AddButtons(int nNumButtons, LPTBBUTTON lpButtons);
  BOOL InsertButton(int nIndex, LPTBBUTTON lpButton);
  BOOL DeleteButton(int nIndex);
  UINT CommandToIndex(UINT nID) const;
  void SaveState(HKEY hKeyRoot, LPCTSTR lpszSubKey,
      LPCTSTR lpszValueName);
  void RestoreState(HKEY hKeyRoot, LPCTSTR lpszSubKey,
      LPCTSTR lpszValueName);
  void LoadImages(int iBitmapID, HINSTANCE hinst);
  BOOL MapAccelerator(TCHAR chAccel, UINT* pIDBtn);
  BOOL MarkButton(int nID, BOOL fHighlight = TRUE);
  BOOL MoveButton(UINT nOldPos, UINT nNewPos);
  int HitTest(LPPOINT ppt) const;

  void Customize();
  int AddString(UINT nStringID);
  int AddStrings(LPCTSTR lpszStrings);
  void AutoSize();

  // Implementation
public:
  virtual ~CToolBarCtrl();

protected:
  //{{AFX_MSG(CToolBarCtrl)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

class CReBarCtrl : public CWnd
{
  DECLARE_DYNAMIC(CReBarCtrl)
  // Construction
public:
  CReBarCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
public:
  UINT GetBandCount() const;
  BOOL GetBandInfo(UINT uBand, REBARBANDINFO* prbbi) const;
  UINT GetBarHeight() const;
  BOOL GetBarInfo(REBARINFO* prbi) const;
  COLORREF GetBkColor() const;
  IDropTarget* GetDropTarget() const;
  BOOL GetRect(UINT uBand, LPRECT prc) const;
  UINT GetRowCount() const;
  UINT GetRowHeight(UINT uRow) const;
  COLORREF GetTextColor() const;
  CToolTipCtrl* GetToolTips() const;
  int IDToIndex(UINT uBandID) const;
  BOOL SetBandInfo(UINT uBand, REBARBANDINFO* prbbi);
  BOOL SetBarInfo(REBARINFO* prbi);
  COLORREF SetBkColor(COLORREF clr);
  CWnd* SetOwner(CWnd* pWnd);
  COLORREF SetTextColor(COLORREF clr);
  void SetToolTips(CToolTipCtrl* pToolTip);
  CImageList* GetImageList() const;
  BOOL SetImageList(CImageList* pImageList);
  void GetBandBorders(UINT uBand, LPRECT prc) const;
  CPalette* GetPalette() const;
  CPalette* SetPalette(HPALETTE hPal);
  BOOL GetColorScheme(COLORSCHEME* lpcs);
  void SetColorScheme(const COLORSCHEME* lpcs);

  // Operations
public:
  void BeginDrag(UINT uBand, DWORD dwPos = (DWORD) - 1);
  BOOL DeleteBand(UINT uBand);
  void DragMove(DWORD dwPos = (DWORD) - 1);
  void EndDrag();
  int HitTest(RBHITTESTINFO* prbht);
  BOOL InsertBand(UINT uIndex, REBARBANDINFO* prbbi);
  void MaximizeBand(UINT uBand);
  void MinimizeBand(UINT uBand);
  BOOL ShowBand(UINT uBand, BOOL fShow = TRUE);
  BOOL SizeToRect(CRect& rect);
  BOOL MoveBand(UINT uFrom, UINT uTo);
};

#ifndef _AFX_NO_RICHEDIT_SUPPORT
/////////////////////////////////////////////////////////////////////////////
// CRichEditCtrl

class CRichEditCtrl : public CWnd
{
  DECLARE_DYNAMIC(CRichEditCtrl)

  // Constructors
public:
  CRichEditCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  BOOL CanUndo() const;
  int GetLineCount() const;
  BOOL GetModify() const;
  void SetModify(BOOL bModified = TRUE);
  void GetRect(LPRECT lpRect) const;
  CPoint GetCharPos(long lChar) const;
  void SetOptions(WORD wOp, DWORD dwFlags);

  // NOTE: first word in lpszBuffer must contain the size of the buffer!
  int GetLine(int nIndex, LPTSTR lpszBuffer) const;
  int GetLine(int nIndex, LPTSTR lpszBuffer, int nMaxLength) const;

  BOOL CanPaste(UINT nFormat = 0) const;
  void GetSel(long& nStartChar, long& nEndChar) const;
  void GetSel(CHARRANGE& cr) const;
  void LimitText(long nChars = 0);
  long LineFromChar(long nIndex) const;
  void SetSel(long nStartChar, long nEndChar);
  void SetSel(CHARRANGE& cr);
  DWORD GetDefaultCharFormat(CHARFORMAT& cf) const;
  DWORD GetSelectionCharFormat(CHARFORMAT& cf) const;
  long GetEventMask() const;
  long GetLimitText() const;
  DWORD GetParaFormat(PARAFORMAT& pf) const;
  // richedit EM_GETSELTEXT is ANSI
  long GetSelText(LPSTR lpBuf) const;
  CString GetSelText() const;
  WORD GetSelectionType() const;
  COLORREF SetBackgroundColor(BOOL bSysColor, COLORREF cr);
  BOOL SetDefaultCharFormat(CHARFORMAT& cf);
  BOOL SetSelectionCharFormat(CHARFORMAT& cf);
  BOOL SetWordCharFormat(CHARFORMAT& cf);
  DWORD SetEventMask(DWORD dwEventMask);
  BOOL SetParaFormat(PARAFORMAT& pf);
  BOOL SetTargetDevice(HDC hDC, long lLineWidth);
  BOOL SetTargetDevice(CDC& dc, long lLineWidth);
  long GetTextLength() const;
  BOOL SetReadOnly(BOOL bReadOnly = TRUE);
  int GetFirstVisibleLine() const;

  // Operations
  void EmptyUndoBuffer();

  int LineIndex(int nLine = -1) const;
  int LineLength(int nLine = -1) const;
  void LineScroll(int nLines, int nChars = 0);
  void ReplaceSel(LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);
  void SetRect(LPCRECT lpRect);

  BOOL DisplayBand(LPRECT pDisplayRect);
  long FindText(DWORD dwFlags, FINDTEXTEX* pFindText) const;
  long FormatRange(FORMATRANGE* pfr, BOOL bDisplay = TRUE);
  void HideSelection(BOOL bHide, BOOL bPerm);
  void PasteSpecial(UINT nClipFormat, DWORD dvAspect = 0, HMETAFILE hMF = 0);
  void RequestResize();
  long StreamIn(int nFormat, EDITSTREAM& es);
  long StreamOut(int nFormat, EDITSTREAM& es);

  // Clipboard operations
  BOOL Undo();
  void Clear();
  void Copy();
  void Cut();
  void Paste();

  // OLE support
  IRichEditOle* GetIRichEditOle() const;
  BOOL SetOLECallback(IRichEditOleCallback* pCallback);

  // Implementation
public:
  virtual ~CRichEditCtrl();
};
#endif //!_AFX_NO_RICHEDIT_SUPPORT


/////////////////////////////////////////////////////////////////////////////
// CIPAddressCtrl

class CIPAddressCtrl : public CWnd
{
  DECLARE_DYNAMIC(CIPAddressCtrl)

  // Constructors
  CIPAddressCtrl();
  BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

  // Attributes
  BOOL IsBlank() const;
  void ClearAddress();
  int GetAddress(BYTE& nField0, BYTE& nField1, BYTE& nField2, BYTE& nField3);
  int GetAddress(DWORD& dwAddress);
  void SetAddress(DWORD dwAddress); //TODO: return value?
  void SetAddress(BYTE nField0, BYTE nField1, BYTE nField2, BYTE nField3); //TODO: return value?
  void SetFieldFocus(WORD nField); //TODO: return value?
  void SetFieldRange(int nField, BYTE nLower, BYTE nUpper); //TODO: return value?

  // Implementation
public:
  virtual ~CIPAddressCtrl();
};

// special struct for WM_SIZEPARENT
struct AFX_SIZEPARENTPARAMS
{
	HDWP hDWP;       // handle for DeferWindowPos
	RECT rect;       // parent client rectangle (trim as appropriate)
	SIZE sizeTotal;  // total size on each side as layout proceeds
	BOOL bStretch;   // should stretch to fill all space
};

// flags for wParam in the WM_FLOATSTATUS message
enum {  FS_SHOW = 0x01, FS_HIDE = 0x02,
		FS_ACTIVATE = 0x04, FS_DEACTIVATE = 0x08,
		FS_ENABLE = 0x10, FS_DISABLE = 0x20,
		FS_SYNCACTIVE = 0x40 };

void AFXAPI AfxRepositionWindow(AFX_SIZEPARENTPARAMS* lpLayout,
	HWND hWnd, LPCRECT lpRect);

/////////////////////////////////////////////////////////////////////////////
// Implementation of command routing

struct AFX_CMDHANDLERINFO
{
	CCmdTarget* pTarget;
	void (AFX_MSG_CALL CCmdTarget::*pmf)(void);
};

/////////////////////////////////////////////////////////////////////////////
// Robust file save support
// opens a temp file if modeCreate specified and enough free space
// renaming, etc occurs automatically if everything succeeds

class CMirrorFile : public CFile
{
// Implementation
public:
	virtual void Abort();
	virtual void Close();
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags,
		CFileException* pError = NULL);
	static CString GetTempName(LPCTSTR pstrOriginalFile, BOOL bCreate);

protected:
	CString m_strMirrorName;
};

/////////////////////////////////////////////////////////////////////////////
// Implementation of PrintPreview

class CPreviewDC : public CDC
{
	DECLARE_DYNAMIC(CPreviewDC)

public:
	virtual void SetAttribDC(HDC hDC);  // Set the Attribute DC
	virtual void SetOutputDC(HDC hDC);

	virtual void ReleaseOutputDC();

// Constructors
	CPreviewDC();

// Implementation
public:
	virtual ~CPreviewDC();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void SetScaleRatio(int nNumerator, int nDenominator);
	void SetTopLeftOffset(CSize TopLeft);
	void ClipToPage();

	// These conversion functions can be used without an output DC

	void PrinterDPtoScreenDP(LPPOINT lpPoint) const;

// Device-Context Functions
	virtual int SaveDC();
	virtual BOOL RestoreDC(int nSavedDC);

public:
	virtual CGdiObject* SelectStockObject(int nIndex);
	virtual CFont* SelectObject(CFont* pFont);

// Drawing-Attribute Functions
	virtual COLORREF SetBkColor(COLORREF crColor);
	virtual COLORREF SetTextColor(COLORREF crColor);

// Mapping Functions
	virtual int SetMapMode(int nMapMode);
	virtual CPoint SetViewportOrg(int x, int y);
	virtual CPoint OffsetViewportOrg(int nWidth, int nHeight);
	virtual CSize SetViewportExt(int x, int y);
	virtual CSize ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom);
	virtual CSize SetWindowExt(int x, int y);
	virtual CSize ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom);

// Text Functions
	virtual BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount);
	virtual BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
				LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths);
	virtual CSize TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount,
				int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
	virtual int DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect,
				UINT nFormat);
	virtual BOOL GrayString(CBrush* pBrush,
				BOOL (CALLBACK* lpfnOutput)(HDC, LPARAM, int),
					LPARAM lpData, int nCount,
					int x, int y, int nWidth, int nHeight);

// Printer Escape Functions
	virtual int Escape(int nEscape, int nCount, LPCSTR lpszInData, LPVOID lpOutData);

// Implementation
protected:
	void MirrorMappingMode(BOOL bCompute);
	void MirrorViewportOrg();
	void MirrorFont();
	void MirrorAttributes();

	CSize ComputeDeltas(int& x, LPCTSTR lpszString, UINT& nCount, BOOL bTabbed,
					UINT nTabStops, LPINT lpnTabStops, int nTabOrigin,
					LPTSTR lpszOutputString, int* pnDxWidths, int& nRightFixup);

protected:
	int m_nScaleNum;    // Scale ratio Numerator
	int m_nScaleDen;    // Scale ratio Denominator
	int m_nSaveDCIndex; // DC Save index when Screen DC Attached
	int m_nSaveDCDelta; // delta between Attrib and output restore indices
	CSize m_sizeTopLeft;// Offset for top left corner of page
	HFONT m_hFont;      // Font selected into the screen DC (NULL if none)
	HFONT m_hPrinterFont; // Font selected into the print DC

	CSize m_sizeWinExt; // cached window extents computed for screen
	CSize m_sizeVpExt;  // cached viewport extents computed for screen
};

/////////////////////////////////////////////////////////////////////////////
// CPreviewView

class CDialogBar;

class CPreviewView : public CScrollView
{
	DECLARE_DYNCREATE(CPreviewView)

// Constructors
public:
	CPreviewView();
	BOOL SetPrintView(CView* pPrintView);

// Attributes
protected:
	CView* m_pOrigView;
	CView* m_pPrintView;
	CPreviewDC* m_pPreviewDC;  // Output and attrib DCs Set, not created
	CDC m_dcPrint;             // Actual printer DC

// Operations
	void SetZoomState(UINT nNewState, UINT nPage, CPoint point);
	void SetCurrentPage(UINT nPage, BOOL bClearRatios);

	// Returns TRUE if in a page rect. Returns the page index
	// in nPage and the point converted to 1:1 screen device coordinates
	BOOL FindPageRect(CPoint& point, UINT& nPage);


// Overridables
	virtual void OnActivateView(BOOL bActivate,
			CView* pActivateView, CView* pDeactiveView);

	// Returns .cx/.cy as the numerator/denominator pair for the ratio
	// using CSize for convenience
	virtual CSize CalcScaleRatio(CSize windowSize, CSize actualSize);

	virtual void PositionPage(UINT nPage);
	virtual void OnDisplayPageNumber(UINT nPage, UINT nPagesDisplayed);

// Implementation
public:
	virtual ~CPreviewView();
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
#ifdef _DEBUG
	void AssertValid() const;
	void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CPreviewView)
	afx_msg void OnPreviewClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDraw(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNumPageChange();
	afx_msg void OnNextPage();
	afx_msg void OnPrevPage();
	afx_msg void OnPreviewPrint();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnUpdateNumPageChange(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNextPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePrevPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	void DoZoom(UINT nPage, CPoint point);
	void SetScaledSize(UINT nPage);
	CSize CalcPageDisplaySize();

	CPrintPreviewState* m_pPreviewState; // State to restore
	CDialogBar* m_pToolBar; // Toolbar for preview

	struct PAGE_INFO
	{
		PAGE_INFO();
		CRect rectScreen; // screen rect (screen device units)
		CSize sizeUnscaled; // unscaled screen rect (screen device units)
		CSize sizeScaleRatio; // scale ratio (cx/cy)
		CSize sizeZoomOutRatio; // scale ratio when zoomed out (cx/cy)
	};

	PAGE_INFO* m_pPageInfo; // Array of page info structures
	PAGE_INFO m_pageInfoArray[2]; // Embedded array for the default implementation

	BOOL m_bPageNumDisplayed;// Flags whether or not page number has yet
								// been displayed on status line
	UINT m_nZoomOutPages; // number of pages when zoomed out
	UINT m_nZoomState;
	UINT m_nMaxPages; // for sanity checks
	UINT m_nCurrentPage;
	UINT m_nPages;
	int m_nSecondPageOffset; // used to shift second page position

	HCURSOR m_hMagnifyCursor;

	CSize m_sizePrinterPPI; // printer pixels per inch
	CPoint m_ptCenterPoint;
	CPrintInfo* m_pPreviewInfo;

	DECLARE_MESSAGE_MAP()

	friend class CView;
	friend BOOL CALLBACK _AfxPreviewCloseProc(CFrameWnd* pFrameWnd);
};

// Zoom States
#define ZOOM_OUT    0
#define ZOOM_MIDDLE 1
#define ZOOM_IN     2

/////////////////////////////////////////////////////////////////////////////
// mirroring support

//WINBUG: some mirroring stuff will be in wingdi.h someday
#ifndef LAYOUT_RTL
#define LAYOUT_RTL                         0x00000001
#endif
#ifndef NOMIRRORBITMAP
#define NOMIRRORBITMAP                     0x80000000
#endif
#ifndef LAYOUT_LTR
#define LAYOUT_LTR                         0x00000000
#endif

//WINBUG: some mirroring stuff will be in winuser.h someday
#ifndef WS_EX_LAYOUTRTL
#define WS_EX_LAYOUTRTL                    0x00400000L
#endif


/////////////////////////////////////////////////////////////////////////////
// toolbar docking support

class CDockContext
{
public:
// Construction
	CDockContext(CControlBar* pBar);

// Attributes
	CPoint m_ptLast;            // last mouse position during drag
	CRect m_rectLast;
	CSize m_sizeLast;
	BOOL m_bDitherLast;

	// Rectangles used during dragging or resizing
	CRect m_rectDragHorz;
	CRect m_rectDragVert;
	CRect m_rectFrameDragHorz;
	CRect m_rectFrameDragVert;

	CControlBar* m_pBar;        // the toolbar that created this context
	CFrameWnd* m_pDockSite;     // the controlling frame of the CControlBar
	DWORD m_dwDockStyle;        // allowable dock styles for bar
	DWORD m_dwOverDockStyle;    // style of dock that rect is over
	DWORD m_dwStyle;            // style of control bar
	BOOL m_bFlip;               // if shift key is down
	BOOL m_bForceFrame;         // if ctrl key is down

	CDC* m_pDC;                 // where to draw during drag
	BOOL m_bDragging;
	int m_nHitTest;

	UINT m_uMRUDockID;
	CRect m_rectMRUDockPos;

	DWORD m_dwMRUFloatStyle;
	CPoint m_ptMRUFloatPos;

// Drag Operations
	virtual void StartDrag(CPoint pt);
	void Move(CPoint pt);       // called when mouse has moved
	void EndDrag();             // drop
	void OnKey(int nChar, BOOL bDown);

// Resize Operations
	virtual void StartResize(int nHitTest, CPoint pt);
	void Stretch(CPoint pt);
	void EndResize();

// Double Click Operations
	virtual void ToggleDocking();

// Operations
	void InitLoop();
	void CancelLoop();

// Implementation
public:
	~CDockContext();
	BOOL Track();
	void DrawFocusRect(BOOL bRemoveRect = FALSE);
		// draws the correct outline
	void UpdateState(BOOL* pFlag, BOOL bNewValue);
	DWORD CanDock();
	CDockBar* GetDockBar(DWORD dwOverDockStyle);
};

/////////////////////////////////////////////////////////////////////////////
// CControlBarInfo - used for docking serialization

class CControlBarInfo
{
public:
// Implementation
	CControlBarInfo();

// Attributes
	UINT m_nBarID;      // ID of this bar
	BOOL m_bVisible;    // visibility of this bar
	BOOL m_bFloating;   // whether floating or not
	BOOL m_bHorz;       // orientation of floating dockbar
	BOOL m_bDockBar;    // TRUE if a dockbar
	CPoint m_pointPos;  // topleft point of window

	UINT m_nMRUWidth;   // MRUWidth for Dynamic Toolbars
	BOOL m_bDocking;    // TRUE if this bar has a DockContext
	UINT m_uMRUDockID;  // most recent docked dockbar
	CRect m_rectMRUDockPos; // most recent docked position
	DWORD m_dwMRUFloatStyle; // most recent floating orientation
	CPoint m_ptMRUFloatPos; // most recent floating position

	CPtrArray m_arrBarID;   // bar IDs for bars contained within this one
	CControlBar* m_pBar;    // bar which this refers to (transient)

	void Serialize(CArchive& ar, CDockState* pDockState);
	BOOL LoadState(LPCTSTR lpszProfileName, int nIndex, CDockState* pDockState);
	BOOL SaveState(LPCTSTR lpszProfileName, int nIndex);
};


/////////////////////////////////////////////////////////////////////////////
// AFXEXT - MFC Advanced Extensions and Advanced Customizable classes

// Classes declared in this file

//CObject
	//CCmdTarget;
		//CWnd
			//CButton
				class CBitmapButton;    // Bitmap button (self-draw)

			class CControlBar;          // control bar
				class CStatusBar;       // status bar
				class CToolBar;         // toolbar
				class CDialogBar;       // dialog as control bar
				class CReBar;           // ie40 dock bar

			class CSplitterWnd;         // splitter manager

			//CView
				//CScrollView
				class CFormView;        // view with a dialog template
				class CEditView;        // simple text editor view

	//CDC
		class CMetaFileDC;              // a metafile with proxy

class CRectTracker;                     // tracker for rectangle objects

// information structures
struct CPrintInfo;          // Printing context
struct CPrintPreviewState;  // Print Preview context/state
struct CCreateContext;      // Creation context

#undef AFX_DATA
#define AFX_DATA AFX_CORE_DATA

/////////////////////////////////////////////////////////////////////////////
// Simple bitmap button

// CBitmapButton - push-button with 1->4 bitmap images
class CBitmapButton : public CButton
{
	DECLARE_DYNAMIC(CBitmapButton)
public:
// Construction
	CBitmapButton();

	BOOL LoadBitmaps(LPCTSTR lpszBitmapResource,
			LPCTSTR lpszBitmapResourceSel = NULL,
			LPCTSTR lpszBitmapResourceFocus = NULL,
			LPCTSTR lpszBitmapResourceDisabled = NULL);
	BOOL LoadBitmaps(UINT nIDBitmapResource,
			UINT nIDBitmapResourceSel = 0,
			UINT nIDBitmapResourceFocus = 0,
			UINT nIDBitmapResourceDisabled = 0);
	BOOL AutoLoad(UINT nID, CWnd* pParent);

// Operations
	void SizeToContent();

// Implementation:
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	// all bitmaps must be the same size
	CBitmap m_bitmap;           // normal image (REQUIRED)
	CBitmap m_bitmapSel;        // selected image (OPTIONAL)
	CBitmap m_bitmapFocus;      // focused but not selected (OPTIONAL)
	CBitmap m_bitmapDisabled;   // disabled bitmap (OPTIONAL)

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};

/////////////////////////////////////////////////////////////////////////////
// Control Bars

// forward declarations (private to implementation)
class CDockBar;
class CDockContext;
class CControlBarInfo;
struct AFX_SIZEPARENTPARAMS;

// Layout Modes for CalcDynamicLayout
#define LM_STRETCH  0x01    // same meaning as bStretch in CalcFixedLayout.  If set, ignores nLength
							// and returns dimensions based on LM_HORZ state, otherwise LM_HORZ is used
							// to determine if nLength is the desired horizontal or vertical length
							// and dimensions are returned based on nLength
#define LM_HORZ     0x02    // same as bHorz in CalcFixedLayout
#define LM_MRUWIDTH 0x04    // Most Recently Used Dynamic Width
#define LM_HORZDOCK 0x08    // Horizontal Docked Dimensions
#define LM_VERTDOCK 0x10    // Vertical Docked Dimensions
#define LM_LENGTHY  0x20    // Set if nLength is a Height instead of a Width
#define LM_COMMIT   0x40    // Remember MRUWidth

#ifdef _AFXDLL
class CControlBar : public CWnd
#else
class AFX_NOVTABLE CControlBar : public CWnd
#endif
{
	DECLARE_DYNAMIC(CControlBar)
// Construction
protected:
	CControlBar();

// Attributes
public:
	int GetCount() const;

	// for styles specific to CControlBar
	DWORD GetBarStyle();
	void SetBarStyle(DWORD dwStyle);

	BOOL m_bAutoDelete;

	// getting and setting border space
	void SetBorders(LPCRECT lpRect);
	void SetBorders(int cxLeft = 0, int cyTop = 0, int cxRight = 0, int cyBottom = 0);
	CRect GetBorders() const;

	CFrameWnd* GetDockingFrame() const;
	BOOL IsFloating() const;
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);

// Operations
	void EnableDocking(DWORD dwDockStyle);

// Overridables
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) = 0;

// Implementation
public:
	virtual ~CControlBar();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void DelayShow(BOOL bShow);
	virtual BOOL IsVisible() const;
	virtual DWORD RecalcDelayShow(AFX_SIZEPARENTPARAMS* lpLayout);

	virtual BOOL IsDockBar() const;
	virtual BOOL DestroyWindow();
	virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);

	// info about bar (for status bar and toolbar)
	int m_cxLeftBorder, m_cxRightBorder;
	int m_cyTopBorder, m_cyBottomBorder;
	int m_cxDefaultGap;         // default gap value
	UINT m_nMRUWidth;   // For dynamic resizing.

	// array of elements
	int m_nCount;
	void* m_pData;        // m_nCount elements - type depends on derived class

	// support for delayed hide/show
	enum StateFlags
		{ delayHide = 1, delayShow = 2, tempHide = 4, statusSet = 8 };
	UINT m_nStateFlags;

	// support for docking
	DWORD m_dwStyle;    // creation style (used for layout)
	DWORD m_dwDockStyle;// indicates how bar can be docked
	CFrameWnd* m_pDockSite; // current dock site, if dockable
	CDockBar* m_pDockBar;   // current dock bar, if dockable
	CDockContext* m_pDockContext;   // used during dragging

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();

	virtual void DoPaint(CDC* pDC);
	void DrawBorders(CDC* pDC, CRect& rect);
	void DrawGripper(CDC* pDC, const CRect& rect);

	// implementation helpers
	virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
	void CalcInsideRect(CRect& rect, BOOL bHorz) const; // adjusts borders etc
	BOOL AllocElements(int nElements, int cbElement);
	virtual BOOL SetStatusText(int nHit);
	void ResetTimer(UINT nEvent, UINT nTime);
	void EraseNonClient();

	void GetBarInfo(CControlBarInfo* pInfo);
	void SetBarInfo(CControlBarInfo* pInfo, CFrameWnd* pFrameWnd);

	//{{AFX_MSG(CControlBar)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnWindowPosChanging(LPWINDOWPOS lpWndPos);
	afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
	afx_msg void OnInitialUpdate();
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint pt);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT nMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CFrameWnd;
	friend class CDockBar;
};

/////////////////////////////////////////////////////////////////////////////
// CStatusBar control

class CStatusBarCtrl;   // forward reference (see afxcmn.h for definition)
struct AFX_STATUSPANE;  // private to implementation

class CStatusBar : public CControlBar
{
	DECLARE_DYNAMIC(CStatusBar)

// Construction
public:
	CStatusBar();
	BOOL Create(CWnd* pParentWnd,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
		UINT nID = AFX_IDW_STATUS_BAR);
	BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = 0,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
		UINT nID = AFX_IDW_STATUS_BAR);
	BOOL SetIndicators(const UINT* lpIDArray, int nIDCount);

// Attributes
public:
	void SetBorders(LPCRECT lpRect);
	void SetBorders(int cxLeft = 0, int cyTop = 0, int cxRight = 0, int cyBottom = 0);

	// standard control bar things
	int CommandToIndex(UINT nIDFind) const;
	UINT GetItemID(int nIndex) const;
	void GetItemRect(int nIndex, LPRECT lpRect) const;

	// specific to CStatusBar
	void GetPaneText(int nIndex, CString& rString) const;
	CString GetPaneText(int nIndex) const;
	BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate = TRUE);
	void GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const;
	void SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth);
	UINT GetPaneStyle(int nIndex) const;
	void SetPaneStyle(int nIndex, UINT nStyle);

	// for direct access to the underlying common control
	CStatusBarCtrl& GetStatusBarCtrl() const;

// Overridables
	virtual void DrawItem(LPDRAWITEMSTRUCT);

// Implementation
public:
	virtual ~CStatusBar();
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL AllocElements(int nElements, int cbElement);
	void CalcInsideRect(CRect& rect, BOOL bHorz) const;
	virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	void EnableDocking(DWORD dwDockStyle);
#endif
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	int m_nMinHeight;

	AFX_STATUSPANE* _GetPanePtr(int nIndex) const;
	void UpdateAllPanes(BOOL bUpdateRects, BOOL bUpdateText);
	virtual BOOL OnChildNotify(UINT message, WPARAM, LPARAM, LRESULT*);

	//{{AFX_MSG(CStatusBar)
	afx_msg UINT OnNcHitTest(CPoint);
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanging(LPWINDOWPOS);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetMinHeight(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// Styles for status bar panes
#define SBPS_NORMAL     0x0000
#define SBPS_NOBORDERS  SBT_NOBORDERS
#define SBPS_POPOUT     SBT_POPOUT
#define SBPS_OWNERDRAW  SBT_OWNERDRAW
#define SBPS_DISABLED   0x04000000
#define SBPS_STRETCH    0x08000000  // stretch to fill status bar

/////////////////////////////////////////////////////////////////////////////
// CToolBar control

HBITMAP AFXAPI AfxLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc, BOOL bMono = FALSE);

class CToolBarCtrl; // forward reference (see afxcmn.h for definition)

class CToolBar : public CControlBar
{
	DECLARE_DYNAMIC(CToolBar)

// Construction
public:
	CToolBar();
	BOOL Create(CWnd* pParentWnd,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
		UINT nID = AFX_IDW_TOOLBAR);
	BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = TBSTYLE_FLAT,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
		CRect rcBorders = CRect(0, 0, 0, 0),
		UINT nID = AFX_IDW_TOOLBAR);

	void SetSizes(SIZE sizeButton, SIZE sizeImage);
		// button size should be bigger than image
	void SetHeight(int cyHeight);
		// call after SetSizes, height overrides bitmap size
	BOOL LoadToolBar(LPCTSTR lpszResourceName);
	BOOL LoadToolBar(UINT nIDResource);
	BOOL LoadBitmap(LPCTSTR lpszResourceName);
	BOOL LoadBitmap(UINT nIDResource);
	BOOL SetBitmap(HBITMAP hbmImageWell);
	BOOL SetButtons(const UINT* lpIDArray, int nIDCount);
		// lpIDArray can be NULL to allocate empty buttons

// Attributes
public:
	// standard control bar things
	int CommandToIndex(UINT nIDFind) const;
	UINT GetItemID(int nIndex) const;
	virtual void GetItemRect(int nIndex, LPRECT lpRect) const;
	UINT GetButtonStyle(int nIndex) const;
	void SetButtonStyle(int nIndex, UINT nStyle);

	// for changing button info
	void GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const;
	void SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage);
	BOOL SetButtonText(int nIndex, LPCTSTR lpszText);
	CString GetButtonText(int nIndex) const;
	void GetButtonText(int nIndex, CString& rString) const;

	// for direct access to the underlying common control
	CToolBarCtrl& GetToolBarCtrl() const;

// Implementation
public:
	virtual ~CToolBar();
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	void SetOwner(CWnd* pOwnerWnd);
	BOOL AddReplaceBitmap(HBITMAP hbmImageWell);
	virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HRSRC m_hRsrcImageWell; // handle to loaded resource for image well
	HINSTANCE m_hInstImageWell; // instance handle to load image well from
	HBITMAP m_hbmImageWell; // contains color mapped button images
	BOOL m_bDelayedButtonLayout; // used to manage when button layout should be done

	CSize m_sizeImage;  // current image size
	CSize m_sizeButton; // current button size

	CMapStringToPtr* m_pStringMap;  // used as CMapStringToUInt

	// implementation helpers
	void _GetButton(int nIndex, TBBUTTON* pButton) const;
	void _SetButton(int nIndex, TBBUTTON* pButton);
	CSize CalcLayout(DWORD nMode, int nLength = -1);
	CSize CalcSize(TBBUTTON* pData, int nCount);
	int WrapToolBar(TBBUTTON* pData, int nCount, int nWidth);
	void SizeToolBar(TBBUTTON* pData, int nCount, int nLength, BOOL bVert = FALSE);
	void Layout(); // called for for delayed button layout

	//{{AFX_MSG(CToolBar)
	afx_msg UINT OnNcHitTest(CPoint);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg void OnWindowPosChanging(LPWINDOWPOS);
	afx_msg void OnSysColorChange();
	afx_msg LRESULT OnSetButtonSize(WPARAM, LPARAM);
	afx_msg LRESULT OnSetBitmapSize(WPARAM, LPARAM);
	afx_msg LRESULT OnPreserveZeroBorderHelper(WPARAM, LPARAM);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT OnSetSizeHelper(CSize& size, LPARAM lParam);
};

// Styles for toolbar buttons
#define TBBS_BUTTON     MAKELONG(TBSTYLE_BUTTON, 0) // this entry is button
#define TBBS_SEPARATOR  MAKELONG(TBSTYLE_SEP, 0)    // this entry is a separator
#define TBBS_CHECKBOX   MAKELONG(TBSTYLE_CHECK, 0)  // this is an auto check button
#define TBBS_GROUP      MAKELONG(TBSTYLE_GROUP, 0)  // marks the start of a group
#define TBBS_CHECKGROUP (TBBS_GROUP|TBBS_CHECKBOX)  // normal use of TBBS_GROUP
#define TBBS_DROPDOWN   MAKELONG(TBSTYLE_DROPDOWN, 0) // drop down style
#define TBBS_AUTOSIZE   MAKELONG(TBSTYLE_AUTOSIZE, 0) // autocalc button width
#define TBBS_NOPREFIX   MAKELONG(TBSTYLE_NOPREFIX, 0) // no accel prefix for this button

// styles for display states
#define TBBS_CHECKED    MAKELONG(0, TBSTATE_CHECKED)    // button is checked/down
#define TBBS_PRESSED    MAKELONG(0, TBSTATE_PRESSED)    // button is being depressed
#define TBBS_DISABLED   MAKELONG(0, TBSTATE_ENABLED)    // button is disabled
#define TBBS_INDETERMINATE  MAKELONG(0, TBSTATE_INDETERMINATE)  // third state
#define TBBS_HIDDEN     MAKELONG(0, TBSTATE_HIDDEN) // button is hidden
#define TBBS_WRAPPED    MAKELONG(0, TBSTATE_WRAP)   // button is wrapped at this point
#define TBBS_ELLIPSES   MAKELONG(0, TBSTATE_ELIPSES)
#define TBBS_MARKED     MAKELONG(0, TBSTATE_MARKED)

////////////////////////////////////////////
// CDialogBar control
// This is a control bar built from a dialog template. It is a modeless
// dialog that delegates all control notifications to the parent window
// of the control bar [the grandparent of the control]

class CDialogBar : public CControlBar
{
	DECLARE_DYNAMIC(CDialogBar)

// Construction
public:
	CDialogBar();
	BOOL Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName,
			UINT nStyle, UINT nID);
	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate,
			UINT nStyle, UINT nID);

// Implementation
public:
	virtual ~CDialogBar();
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	CSize m_sizeDefault;
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
#ifndef _AFX_NO_OCC_SUPPORT
	// data and functions necessary for OLE control containment
	_AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
	LPCTSTR m_lpszTemplateName;
	virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);

	//{{AFX_MSG(CDialogBar)
	DECLARE_MESSAGE_MAP()
	//}}AFX_MSG
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
#endif
};

////////////////////////////////////////////
// CReBar control

class CReBarCtrl;

class CReBar : public CControlBar
{
	DECLARE_DYNAMIC(CReBar)

// Construction
public:
	CReBar();
	BOOL Create(CWnd* pParentWnd, DWORD dwCtrlStyle = RBS_BANDBORDERS,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP,
		UINT nID = AFX_IDW_REBAR);

// Attributes
public:
	// for direct access to the underlying common control
	CReBarCtrl& GetReBarCtrl() const;

// Operations
public:
	BOOL AddBar(CWnd* pBar, LPCTSTR pszText = NULL, CBitmap* pbmp = NULL,
		DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);
	BOOL AddBar(CWnd* pBar, COLORREF clrFore, COLORREF clrBack,
		LPCTSTR pszText = NULL, DWORD dwStyle = RBBS_GRIPPERALWAYS);

// Implementation
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
#ifdef _DEBUG
	void EnableDocking(DWORD dwDockStyle);
#endif

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	BOOL _AddBar(CWnd* pBar, REBARBANDINFO* pRBBI);

	//{{AFX_MSG(CReBar)
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnHeightChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnShowBand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRecalcParent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// Splitter Window

#define SPLS_DYNAMIC_SPLIT  0x0001
#define SPLS_INVERT_TRACKER 0x0002  // obsolete (now ignored)

class CSplitterWnd : public CWnd
{
	DECLARE_DYNAMIC(CSplitterWnd)

// Construction
public:
	CSplitterWnd();
	// Create a single view type splitter with multiple splits
	BOOL Create(CWnd* pParentWnd,
				int nMaxRows, int nMaxCols, SIZE sizeMin,
				CCreateContext* pContext,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE |
					WS_HSCROLL | WS_VSCROLL | SPLS_DYNAMIC_SPLIT,
				UINT nID = AFX_IDW_PANE_FIRST);

	// Create a multiple view type splitter with static layout
	BOOL CreateStatic(CWnd* pParentWnd,
				int nRows, int nCols,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE,
				UINT nID = AFX_IDW_PANE_FIRST);

	virtual BOOL CreateView(int row, int col, CRuntimeClass* pViewClass,
			SIZE sizeInit, CCreateContext* pContext);

// Attributes
public:
	int GetRowCount() const;
	int GetColumnCount() const;

	// information about a specific row or column
	void GetRowInfo(int row, int& cyCur, int& cyMin) const;
	void SetRowInfo(int row, int cyIdeal, int cyMin);
	void GetColumnInfo(int col, int& cxCur, int& cxMin) const;
	void SetColumnInfo(int col, int cxIdeal, int cxMin);

	// for setting and getting shared scroll bar style
	DWORD GetScrollStyle() const;
	void SetScrollStyle(DWORD dwStyle);

	// views inside the splitter
	CWnd* GetPane(int row, int col) const;
	BOOL IsChildPane(CWnd* pWnd, int* pRow, int* pCol);
	BOOL IsChildPane(CWnd* pWnd, int& row, int& col); // obsolete
	int IdFromRowCol(int row, int col) const;

	BOOL IsTracking();  // TRUE during split operation

// Operations
public:
	virtual void RecalcLayout();    // call after changing sizes

// Overridables
protected:
	// to customize the drawing
	enum ESplitType { splitBox, splitBar, splitIntersection, splitBorder };
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	virtual void OnInvertTracker(const CRect& rect);

public:
	// for customizing scrollbar regions
	virtual BOOL CreateScrollBarCtrl(DWORD dwStyle, UINT nID);

	// for customizing DYNAMIC_SPLIT behavior
	virtual void DeleteView(int row, int col);
	virtual BOOL SplitRow(int cyBefore);
	virtual BOOL SplitColumn(int cxBefore);
	virtual void DeleteRow(int rowDelete);
	virtual void DeleteColumn(int colDelete);

	// determining active pane from focus or active view in frame
	virtual CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
	virtual void SetActivePane(int row, int col, CWnd* pWnd = NULL);
protected:
	CWnd* GetActivePane(int& row, int& col); // obsolete

public:
	// high level command operations - called by default view implementation
	virtual BOOL CanActivateNext(BOOL bPrev = FALSE);
	virtual void ActivateNext(BOOL bPrev = FALSE);
	virtual BOOL DoKeyboardSplit();

	// synchronized scrolling
	virtual BOOL DoScroll(CView* pViewFrom, UINT nScrollCode,
		BOOL bDoScroll = TRUE);
	virtual BOOL DoScrollBy(CView* pViewFrom, CSize sizeScroll,
		BOOL bDoScroll = TRUE);

// Implementation
public:
	virtual ~CSplitterWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// implementation structure
	struct CRowColInfo
	{
		int nMinSize;       // below that try not to show
		int nIdealSize;     // user set size
		// variable depending on the available size layout
		int nCurSize;       // 0 => invisible, -1 => nonexistant
	};

protected:
	// customizable implementation attributes (set by constructor or Create)
	CRuntimeClass* m_pDynamicViewClass;
	int m_nMaxRows, m_nMaxCols;

	// implementation attributes which control layout of the splitter
	int m_cxSplitter, m_cySplitter;         // size of splitter bar
	int m_cxBorderShare, m_cyBorderShare;   // space on either side of splitter
	int m_cxSplitterGap, m_cySplitterGap;   // amount of space between panes
	int m_cxBorder, m_cyBorder;             // borders in client area

	// current state information
	int m_nRows, m_nCols;
	BOOL m_bHasHScroll, m_bHasVScroll;
	CRowColInfo* m_pColInfo;
	CRowColInfo* m_pRowInfo;

	// Tracking info - only valid when 'm_bTracking' is set
	BOOL m_bTracking, m_bTracking2;
	CPoint m_ptTrackOffset;
	CRect m_rectLimit;
	CRect m_rectTracker, m_rectTracker2;
	int m_htTrack;

	// implementation routines
	BOOL CreateCommon(CWnd* pParentWnd, SIZE sizeMin, DWORD dwStyle, UINT nID);
	virtual int HitTest(CPoint pt) const;
	virtual void GetInsideRect(CRect& rect) const;
	virtual void GetHitRect(int ht, CRect& rect);
	virtual void TrackRowSize(int y, int row);
	virtual void TrackColumnSize(int x, int col);
	virtual void DrawAllSplitBars(CDC* pDC, int cxInside, int cyInside);
	virtual void SetSplitCursor(int ht);
	CWnd* GetSizingParent();

	// starting and stopping tracking
	virtual void StartTracking(int ht);
	virtual void StopTracking(BOOL bAccept);

	// special command routing to frame
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//{{AFX_MSG(CSplitterWnd)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnCancelMode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDisplayChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFormView - generic view constructed from a dialog template

class CFormView : public CScrollView
{
	DECLARE_DYNAMIC(CFormView)

// Construction
protected:      // must derive your own class
	CFormView(LPCTSTR lpszTemplateName);
	CFormView(UINT nIDTemplate);

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void OnInitialUpdate();

protected:
	LPCTSTR m_lpszTemplateName;
	CCreateContext* m_pCreateContext;
	HWND m_hWndFocus;   // last window to have focus

	virtual void OnDraw(CDC* pDC);      // default does nothing
	// special case override of child window creation
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnActivateView(BOOL, CView*, CView*);
	virtual void OnActivateFrame(UINT, CFrameWnd*);
	BOOL SaveFocusControl();    // updates m_hWndFocus

#ifndef _AFX_NO_OCC_SUPPORT
	// data and functions necessary for OLE control containment
	_AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
	virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
#endif

	//{{AFX_MSG(CFormView)
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CEditView - simple text editor view

class CEditView : public CCtrlView
{
	DECLARE_DYNCREATE(CEditView)

// Construction
public:
	CEditView();

// Attributes
public:
	static AFX_DATA const DWORD dwStyleDefault;
	// CEdit control access
	CEdit& GetEditCtrl() const;

	// presentation attributes
	CFont* GetPrinterFont() const;
	void SetPrinterFont(CFont* pFont);
	void SetTabStops(int nTabStops);

	// other attributes
	void GetSelectedText(CString& strResult) const;

	// buffer access
	LPCTSTR LockBuffer() const;
	void UnlockBuffer() const;
	UINT GetBufferLength() const;

// Operations
public:
	BOOL FindText(LPCTSTR lpszFind, BOOL bNext = TRUE, BOOL bCase = TRUE);
	void SerializeRaw(CArchive& ar);
	UINT PrintInsideRect(CDC* pDC, RECT& rectLayout, UINT nIndexStart,
		UINT nIndexStop);

// Overrideables
protected:
	virtual void OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase);
	virtual void OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase,
		LPCTSTR lpszReplace);
	virtual void OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace,
		BOOL bCase);
	virtual void OnTextNotFound(LPCTSTR lpszFind);

// Implementation
public:
	virtual ~CEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	void ReadFromArchive(CArchive& ar, UINT nLen);
	void WriteToArchive(CArchive& ar);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);

	static AFX_DATA const UINT nMaxSize;
		// maximum number of characters supported

protected:
	int m_nTabStops;            // tab stops in dialog units
	LPTSTR m_pShadowBuffer;     // special shadow buffer only used in Win32s
	UINT m_nShadowSize;

	CUIntArray m_aPageStart;    // array of starting pages
	HFONT m_hPrinterFont;       // if NULL, mirror display font
	HFONT m_hMirrorFont;        // font object used when mirroring

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	// printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo = NULL);
	BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);

	// find & replace support
	void OnEditFindReplace(BOOL bFindOnly);
	BOOL InitializeReplace();
	BOOL SameAsSelected(LPCTSTR lpszCompare, BOOL bCase);

	// special overrides for implementation
	virtual void CalcWindowRect(LPRECT lpClientRect,
		UINT nAdjustType = adjustBorder);

	//{{AFX_MSG(CEditView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedClip(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedText(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg BOOL OnEditChange();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnEditUndo();
	afx_msg void OnEditSelectAll();
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
	afx_msg void OnEditRepeat();
	afx_msg LRESULT OnFindReplaceCmd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CMetaFileDC

class CMetaFileDC : public CDC
{
	DECLARE_DYNAMIC(CMetaFileDC)

// Constructors
public:
	CMetaFileDC();
	BOOL Create(LPCTSTR lpszFilename = NULL);
	BOOL CreateEnhanced(CDC* pDCRef, LPCTSTR lpszFileName,
		LPCRECT lpBounds, LPCTSTR lpszDescription);

// Operations
	HMETAFILE Close();
	HENHMETAFILE CloseEnhanced();

// Implementation
public:
	virtual void SetAttribDC(HDC hDC);  // Set the Attribute DC

protected:
	virtual void SetOutputDC(HDC hDC);  // Set the Output DC -- Not allowed
	virtual void ReleaseOutputDC();     // Release the Output DC -- Not allowed

public:
	virtual ~CMetaFileDC();

// Clipping Functions (use the Attribute DC's clip region)
	virtual int GetClipBox(LPRECT lpRect) const;
	virtual BOOL PtVisible(int x, int y) const;
			BOOL PtVisible(POINT point) const;
	virtual BOOL RectVisible(LPCRECT lpRect) const;

// Text Functions
	virtual BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount);
			BOOL TextOut(int x, int y, const CString& str);
	virtual BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
				LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths);
			BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
				const CString& str, LPINT lpDxWidths);
	virtual CSize TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount,
				int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
			CSize TabbedTextOut(int x, int y, const CString& str,
				int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
	virtual int DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect,
				UINT nFormat);
			int DrawText(const CString& str, LPRECT lpRect, UINT nFormat);

// Printer Escape Functions
	virtual int Escape(int nEscape, int nCount, LPCSTR lpszInData, LPVOID lpOutData);

// Viewport Functions
	virtual CPoint SetViewportOrg(int x, int y);
			CPoint SetViewportOrg(POINT point);
	virtual CPoint OffsetViewportOrg(int nWidth, int nHeight);
	virtual CSize SetViewportExt(int x, int y);
			CSize SetViewportExt(SIZE size);
	virtual CSize ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom);

protected:
	void AdjustCP(int cx);
};

/////////////////////////////////////////////////////////////////////////////
// CRectTracker - simple rectangular tracking rectangle w/resize handles

class CRectTracker
{
public:
// Constructors
	CRectTracker();
	CRectTracker(LPCRECT lpSrcRect, UINT nStyle);

// Style Flags
	enum StyleFlags
	{
		solidLine = 1, dottedLine = 2, hatchedBorder = 4,
		resizeInside = 8, resizeOutside = 16, hatchInside = 32,
	};

// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
		hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
	};

// Attributes
	UINT m_nStyle;      // current state
	CRect m_rect;       // current position (always in pixels)
	CSize m_sizeMin;    // minimum X and Y size during track operation
	int m_nHandleSize;  // size of resize handles (default from WIN.INI)

// Operations
	void Draw(CDC* pDC) const;
	void GetTrueRect(LPRECT lpTrueRect) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert = FALSE,
		CWnd* pWndClipTo = NULL);
	BOOL TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert = TRUE);
	int HitTest(CPoint point) const;
	int NormalizeHit(int nHandle) const;

// Overridables
	virtual void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo,
		CDC* pDC, CWnd* pWnd);
	virtual void AdjustRect(int nHandle, LPRECT lpRect);
	virtual void OnChangedRect(const CRect& rectOld);
	virtual UINT GetHandleMask() const;

// Implementation
public:
	virtual ~CRectTracker();

protected:
	BOOL m_bAllowInvert;    // flag passed to Track or TrackRubberBand
	CRect m_rectLast;
	CSize m_sizeLast;
	BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase

	// implementation helpers
	int HitTestHandles(CPoint point) const;
	void GetHandleRect(int nHandle, CRect* pHandleRect) const;
	void GetModifyPointers(int nHandle, int**ppx, int**ppy, int* px, int*py);
	virtual int GetHandleSize(LPCRECT lpRect = NULL) const;
	BOOL TrackHandle(int nHandle, CWnd* pWnd, CPoint point, CWnd* pWndClipTo);
	void Construct();
};

/////////////////////////////////////////////////////////////////////////////
// Informational data structures

struct CPrintInfo // Printing information structure
{
	CPrintInfo();
	~CPrintInfo();

	CPrintDialog* m_pPD;     // pointer to print dialog

	BOOL m_bDocObject;       // TRUE if printing by IPrint interface
	BOOL m_bPreview;         // TRUE if in preview mode
	BOOL m_bDirect;          // TRUE if bypassing Print Dialog
	BOOL m_bContinuePrinting;// set to FALSE to prematurely end printing
	UINT m_nCurPage;         // Current page
	UINT m_nNumPreviewPages; // Desired number of preview pages
	CString m_strPageDesc;   // Format string for page number display
	LPVOID m_lpUserData;     // pointer to user created struct
	CRect m_rectDraw;        // rectangle defining current usable page area

	// these only valid if m_bDocObject
	UINT m_nOffsetPage;      // offset of first page in combined IPrint job
	DWORD m_dwFlags;         // flags passed to IPrint::Print

	void SetMinPage(UINT nMinPage);
	void SetMaxPage(UINT nMaxPage);
	UINT GetMinPage() const;
	UINT GetMaxPage() const;
	UINT GetFromPage() const;
	UINT GetToPage() const;
	UINT GetOffsetPage() const;
};

struct CPrintPreviewState   // Print Preview context/state
{
	UINT nIDMainPane;          // main pane ID to hide
	HMENU hMenu;               // saved hMenu
	DWORD dwStates;            // Control Bar Visible states (bit map)
	CView* pViewActiveOld;     // save old active view during preview
	BOOL (CALLBACK* lpfnCloseProc)(CFrameWnd* pFrameWnd);
	HACCEL hAccelTable;       // saved accelerator table

// Implementation
	CPrintPreviewState();
};

struct CCreateContext   // Creation information structure
	// All fields are optional and may be NULL
{
	// for creating new views
	CRuntimeClass* m_pNewViewClass; // runtime class of view to create or NULL
	CDocument* m_pCurrentDoc;

	// for creating MDI children (CMDIChildWnd::LoadFrame)
	CDocTemplate* m_pNewDocTemplate;

	// for sharing view/frame state from the original view/frame
	CView* m_pLastView;
	CFrameWnd* m_pCurrentFrame;

// Implementation
	CCreateContext();
};

/////////////////////////////////////////////////////////////////////////////
// CDockBar - used for docking

class CDockBar : public CControlBar
{
	DECLARE_DYNAMIC(CDockBar)

// Construction
public:
	CDockBar(BOOL bFloating = FALSE);   // TRUE if attached to CMiniDockFrameWnd
	BOOL Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID);

// Attributes
	BOOL m_bFloating;

	virtual BOOL IsDockBar() const;
	int GetDockedCount() const;
	virtual int GetDockedVisibleCount() const;

// Operations
	void DockControlBar(CControlBar* pBar, LPCRECT lpRect = NULL);
	void ReDockControlBar(CControlBar* pBar, LPCRECT lpRect = NULL);
	BOOL RemoveControlBar(CControlBar*, int nPosExclude = -1, int nAddPlaceHolder = 0);
	void RemovePlaceHolder(CControlBar* pBar);

// Implementation
public:
	virtual ~CDockBar();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual void DoPaint(CDC* pDC);

	// public implementation helpers
	void GetBarInfo(CControlBarInfo* pInfo);
	void SetBarInfo(CControlBarInfo* pInfo, CFrameWnd* pFrameWnd);
	int FindBar(CControlBar* pBar, int nPosExclude = -1);
	void ShowAll(BOOL bShow);

	CPtrArray m_arrBars;    // each element is a CControlBar
protected:
	BOOL m_bLayoutQuery;
	CRect m_rectLayout;

	CControlBar* GetDockedControlBar(int nPos) const;

	// implementation helpers
	int Insert(CControlBar* pBar, CRect rect, CPoint ptMid);
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

	//{{AFX_MSG(CDockBar)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnWindowPosChanging(LPWINDOWPOS lpWndPos);
	afx_msg void OnPaint();
	afx_msg LRESULT OnSizeParent(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CMiniDockFrameWnd;
};

class CMiniDockFrameWnd : public CMiniFrameWnd
{
	DECLARE_DYNCREATE(CMiniDockFrameWnd)

public:
// Construction
	CMiniDockFrameWnd();
	virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);

// Operations
	virtual void RecalcLayout(BOOL bNotify = TRUE);

// Implementation
public:
	CDockBar m_wndDockBar;

	//{{AFX_MSG(CMiniFrameWnd)
	afx_msg void OnClose();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// COleCntrFrameWnd

class COleIPFrameWnd;

class COleCntrFrameWnd : public CFrameWnd
{
// Constructor
public:
	COleCntrFrameWnd(COleIPFrameWnd* pInPlaceFrame);

// Implementation
protected:
	COleIPFrameWnd* m_pInPlaceFrame;
	virtual void PostNcDestroy();

public:
	virtual ~COleCntrFrameWnd();
	virtual void RecalcLayout(BOOL bNotify = TRUE);
	void OnIdleUpdateCmdUI();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
#ifdef _DEBUG
	void AssertValid() const;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CDialogTemplate

class CDialogTemplate
{
// Constructors
public:
	CDialogTemplate(const DLGTEMPLATE* pTemplate = NULL);
	CDialogTemplate(HGLOBAL hGlobal);

// Attributes
	BOOL HasFont() const;
	BOOL SetFont(LPCTSTR lpFaceName, WORD nFontSize);
	BOOL SetSystemFont(WORD nFontSize = 0);
	BOOL GetFont(CString& strFaceName, WORD& nFontSize) const;
	void GetSizeInDialogUnits(SIZE* pSize) const;
	void GetSizeInPixels(SIZE* pSize) const;

	static BOOL AFX_CDECL GetFont(const DLGTEMPLATE* pTemplate,
		CString& strFaceName, WORD& nFontSize);

// Operations
	BOOL Load(LPCTSTR lpDialogTemplateID);
	HGLOBAL Detach();

// Implementation
public:
	~CDialogTemplate();

	HGLOBAL m_hTemplate;
	DWORD m_dwTemplateSize;
	BOOL m_bSystemFont;

protected:
	static BYTE* AFX_CDECL GetFontSizeField(const DLGTEMPLATE* pTemplate);
	static UINT AFX_CDECL GetTemplateSize(const DLGTEMPLATE* pTemplate);
	BOOL SetTemplate(const DLGTEMPLATE* pTemplate, UINT cb);
};

/////////////////////////////////////////////////////////////////////////////
// WM_NOTIFY support

struct AFX_NOTIFY
{
	LRESULT* pResult;
	NMHDR* pNMHDR;
};

/////////////////////////////////////////////////////////////////////////////
// Global implementation helpers

// window creation hooking
void AFXAPI AfxHookWindowCreate(CWnd* pWnd);
BOOL AFXAPI AfxUnhookWindowCreate();
void AFXAPI AfxResetMsgCache();

// for backward compatibility to previous versions
#define _AfxHookWindowCreate    AfxHookWindowCreate
#define _AfxUnhookWindowCreate  AfxUnhookWindowCreate

// string helpers
void AFXAPI AfxSetWindowText(HWND hWndCtrl, LPCTSTR lpszNew);
int AFXAPI AfxLoadString(UINT nIDS, LPTSTR lpszBuf, UINT nMaxBuf = 256);

HDC AFXAPI AfxCreateDC(HGLOBAL hDevNames, HGLOBAL hDevMode);

void AFXAPI AfxGetModuleShortFileName(HINSTANCE hInst, CString& strShortName);

// Failure dialog helpers
void AFXAPI AfxFailMaxChars(CDataExchange* pDX, int nChars);
void AFXAPI AfxFailRadio(CDataExchange* pDX);

// DLL load helpers
HINSTANCE AFXAPI AfxLoadDll(HINSTANCE* volatile hInst, LPCSTR lpszDLL);
HINSTANCE AFXAPI AfxLoadDll(HINSTANCE* volatile hInst, LPCSTR lpszDLL,
	FARPROC* pProcPtrs, LPCSTR lpszProcName);

#endif // _AFXWIN_H_

