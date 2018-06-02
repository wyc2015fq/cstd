
// BCGToolBarImages.h: interface for the CBCGPToolBarImages class.
//
//////////////////////////////////////////////////////////////////////

// for custom drawing
typedef struct tagDrawState {
  CBitmap* hbmMono;
  CBitmap* hbmMonoOld;
  CBitmap* hbmOldGlyphs;
}
CBCGPDrawState;

typedef BOOL (__stdcall* ALPHABLEND)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION);
typedef BOOL (__stdcall* TRANSPARENTBLT)(HDC hdcDest, int xDest, int yDest, int nDestWidth, int nDestHeight,
    HDC hdcSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
    UINT clrTransparent);

class BCGCBPRODLLEXPORT CBCGPToolBarImages
{
  friend class CBCGPDropDownFrame;
  friend class CBCGPImageEditDlg;
  friend class CBCGPMenuImages;
  friend class CBCGPDrawManager;
  friend struct BCGPGLOBAL_DATA;

public:
  CBCGPToolBarImages();
  virtual ~CBCGPToolBarImages();

  // Operations:
public:
  static BOOL Is32BitTransparencySupported();
  static BOOL PreMultiplyAlpha(CBitmap* hbmp, BOOL bAutoCheckPremlt);

  BOOL IsValid() const {
    return (m_hbmImageWell != NULL);
  }

  BOOL IsReadOnly() const {
    return m_bReadOnly;
  }

  CBitmap* GetImageWell() const {
    return m_hbmImageWell;
  }

  CBitmap* GetImageWellLight() const {
    return m_hbmImageLight;
  }

  BOOL IsUserImagesList() const {
    return m_bUserImagesList;
  }

  void SetImageSize(ISIZE sizeImage, BOOL bUpdateCount = FALSE) {
    m_sizeImage = sizeImage;

    if (bUpdateCount) {
      UpdateCount();
    }
  }

  ISIZE GetImageSize(BOOL bDest = FALSE) const {
    return bDest ? m_sizeImageDest : m_sizeImage;
  }

  int GetCount() const {
    return m_iCount;
  }

  int GetResourceOffset(UINT uiResId) const {
    int iOffset = -1;

    if (m_mapOrigResOffsets.Lookup(uiResId, iOffset)) {
      return iOffset;
    }

    return -1;
  }

  COLORREF SetTransparentColor(COLORREF clrTransparent) {
    COLORREF clrOld = m_clrTransparent;

    if (clrTransparent != clrOld) {
      m_clrTransparent = clrTransparent;
      UpdateInternalImage(0);
      UpdateInternalImage(1);
    }

    return clrOld;
  }

  COLORREF GetTransparentColor() const {
    return m_clrTransparent;
  }

  BOOL Load(UINT uiResID, HINSTANCE hinstRes = NULL, BOOL bAdd = FALSE);
  BOOL LoadStr(LPCTSTR lpszResourceName, HINSTANCE hinstRes = NULL, BOOL bAdd = FALSE);

  BOOL Load(LPCTSTR lpszBmpFileName);
  BOOL Save(LPCTSTR lpszBmpFileName = NULL);

  void Clear();

  BOOL PrepareDrawImage(CSize sizeImageDest = CSize(0, 0), BOOL bFadeInactive = FALSE);

  BOOL Draw(CDC* pDC,
      int x, int y,
      int iImageIndex,
      BOOL bHilite = FALSE,
      BOOL bDisabled = FALSE,
      BOOL bIndeterminate = FALSE,
      BOOL bShadow = FALSE,
      BOOL bInactive = FALSE,
      BYTE alphaSrc = 255);

  BOOL DrawEx(CDC* pDC,
      CRect rect,
      int iImageIndex,
      ImageAlignHorz horzAlign = ImageAlignHorzLeft,
      ImageAlignVert vertAlign = ImageAlignVertTop,
      CRect rectSrc = CRect(0, 0, 0, 0),
      BYTE alphaSrc = 255);

  void EndDrawImage(CBCGPDrawState& ds);

  int AddImage(CBitmap* hbmp, BOOL bSetBitPerPixel = FALSE);
  int AddImage(const CBCGPToolBarImages& imageList, int nIndex);
  int AddIcon(HICON hIcon, BOOL bAlphaBlend = FALSE);
  BOOL UpdateImage(int iImage, CBitmap* hbmp);
  BOOL DeleteImage(int iImage);

  int GetBitsPerPixel() const {
    return m_nBitsPerPixel;
  }

  HICON ExtractIcon(int nIndex);

  BOOL CreateFromImageList(const CImageList& imageList);

  BOOL CopyTo(CBCGPToolBarImages& imageList);
  BOOL CopyImageToClipboard(int iImage);

  BOOL GrayImages(int nGrayPercentage);

  int GetMask(int iImage, CBitmap* bitmapMask);

  void OnSysColorChange();

  static void SetDisabledImageAlpha(BYTE nValue) {
    ASSERT(nValue > 0);
    ASSERT(nValue <= 255);

    m_nDisabledImageAlpha = nValue;
  }

  static BYTE GetDisabledImageAlpha() {
    return m_nDisabledImageAlpha;
  }

  static void SetFadedImageAlpha(BYTE nValue) {
    ASSERT(nValue > 0);
    ASSERT(nValue <= 255);

    m_nFadedImageAlpha = nValue;
  }

  static BYTE GetFadedImageAlpha() {
    return m_nFadedImageAlpha;
  }

  void SetLightPercentage(int nValue) {
    m_nLightPercentage = nValue;
  }

  int GetLightPercentage() const {
    return m_nLightPercentage;
  }

  void SetPreMultiplyAutoCheck(BOOL bAuto = TRUE) {
    m_bAutoCheckPremlt = bAuto;
  }

  BOOL IsPreMultiplyAutoCheck() const {
    return m_bAutoCheckPremlt;
  }

  // RTL support:
  static void EnableRTL(BOOL bIsRTL = TRUE);
  static BOOL IsRTL() {
    return m_bIsRTL;
  }

  BOOL Mirror();
  static BOOL MirrorBitmap(CBitmap*& hbmp, int cxImage);

  BOOL MirrorVert();
  static BOOL MirrorBitmapVert(CBitmap*& hbmp, int cyImage);

  static HRGN CreateRegionFromImage(CBitmap* bmp, COLORREF clrTransparent);

  void AddaptColors(COLORREF clrBase, COLORREF clrTone);
  void AddaptColors(COLORREF clrBase, COLORREF clrTone, double dOpacity);  // dOpacity [0.0 ... 1.0]

  void SetSingleImage();

  BOOL GetMapTo3DColors() const {
    return m_bMapTo3DColors;
  }

  void SetMapTo3DColors(BOOL bMapTo3DColors) {
    m_bMapTo3DColors = bMapTo3DColors;
  }

  BOOL GetAlwaysLight() const {
    return m_bAlwaysLight;
  }

  void SetAlwaysLight(BOOL bAlwaysLight = TRUE) {
    m_bAlwaysLight = bAlwaysLight;
  }

  CRect GetLastImageRect() const {
    return m_rectLastDraw;
  }

  static CBitmap* Copy(CBitmap* hbmpSrc);

  BOOL SmoothResize(double dblImageScale);

  double GetScale() const {
    return m_dblScale;
  }

  BOOL IsScaled() const {
    return GetScale() != 1.0;
  }

  BOOL ConvertTo32Bits(COLORREF clrTransparent = (COLORREF) - 1);

protected:
  static void TransparentBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth,
      int nHeight, CDC* pDcSrc, int nXSrc, int nYSrc,
      COLORREF colorTransparent,
      int nWidthDest = -1, int nHeightDest = -1);

  void CreateMask(int iImage,
      BOOL bHilite,
      BOOL bHiliteShadow);
  static CBitmap* CreateDitherBitmap();
  void UpdateCount();
  BOOL UpdateInternalImage(int nIndex);

  BOOL PreMultiplyAlpha(CBitmap* hbmp);
  void CopyTemp(CBCGPToolBarImages& imagesDest);

  static BOOL MapBmpTo3dColors(CBitmap*& hBmp, BOOL bUseRGBQUAD = TRUE,
      COLORREF clrSrc = (COLORREF) - 1,
      COLORREF clrDest = (COLORREF) - 1);

  // Attributes:
public:
  static BOOL     m_bDisableTrueColorAlpha;
  static BOOL     m_bMultiThreaded; // Set to TRUE if images are used in different threads
  static BOOL     m_bIsDrawOnGlass; // Draw image on Vista Glass area
  static double   m_dblColorTolerance;// Used by 'AddaptColors' method

protected:
  CSize       m_sizeImage;        // size of glyph
  CSize       m_sizeImageOriginal;
  CSize       m_sizeImageDest;  // destination size glyph
  CBitmap*        m_hbmImageWell;   // glyphs only
  CBitmap*        m_hbmImageLight;  // "faded" version (hi-color only)
  CBitmap*        m_hbmImageShadow; // "shadow" version
  BOOL        m_bUserImagesList;  // is user-defined images list?
  CString       m_strUDLPath;   // user-defined images path
  BOOL        m_bModified;    // is image modified?
  int         m_iCount;     // image counter
  CList<UINT> m_lstOrigResIds;  // original resource ids
  CList<HINSTANCE>  m_lstOrigResInstances;  // original resource instances
  CMap<UINT, int> m_mapOrigResOffsets;// original resource offsets
  BOOL        m_bStretch;     // stretch images
  CBitmap*        m_bmpMem;     // bitmap for the drawing in memory
  CBitmap*      m_pBmpOriginal;
  COLORREF      m_clrTransparent; // Transparent color
  COLORREF      m_clrTransparentOriginal;
  BOOL        m_bReadOnly;    // Loaded from read-only file
  COLORREF      m_clrImageShadow; // Color of the shadow
  BOOL        m_bFadeInactive;  // Inactive image will be drawn with the "fading" effect
  int         m_nBitsPerPixel;  // Bitmap color depth
  static BYTE     m_nDisabledImageAlpha;  // Used in the alpha-blending only
  static BYTE     m_nFadedImageAlpha; // Used in the alpha-blending only
  BOOL        m_bIsGray;      // Image is grayed
  int         m_nGrayPercentage;  // Grayed image brightness
  static BOOL     m_bIsRTL;     // RTL languages support
  int         m_nLightPercentage; // Light image percentage value
  BOOL        m_bMapTo3DColors; // Map image colors to system
  BOOL        m_bAlwaysLight;   // Always use light image in all color resolutions
  CRect       m_rectLastDraw;   // Last drawn image location
  CRect       m_rectSubImage;   // The part of drawn image
  BOOL        m_bAutoCheckPremlt; // Auto-check for 32 bpp images
  BOOL        m_bCreateMonoDC;  // Create mono DC in CBCGPDrawState
  double        m_dblScale;
  static CCriticalSection g_cs;     // For multi-theard applications
};

class BCGCBPRODLLEXPORT CBCGPDrawOnGlass
{
public:
  CBCGPDrawOnGlass(BOOL bSet = TRUE) {
    m_bIsDrawOnGlassSaved = CBCGPToolBarImages::m_bIsDrawOnGlass;
    CBCGPToolBarImages::m_bIsDrawOnGlass = bSet;
  }

  virtual ~CBCGPDrawOnGlass() {
    Restore();
  }

  void Restore() {
    CBCGPToolBarImages::m_bIsDrawOnGlass = m_bIsDrawOnGlassSaved;
  }

protected:
  BOOL  m_bIsDrawOnGlassSaved;
};

static BOOL WriteDIB(LPCTSTR szFile, HANDLE hDIB);
static HANDLE DDBToDIB(CBitmap* bitmap, DWORD dwCompression);

BOOL CBCGPToolBarImages::m_bDisableTrueColorAlpha = TRUE;
CCriticalSection CBCGPToolBarImages::g_cs;
BOOL CBCGPToolBarImages::m_bMultiThreaded = FALSE;
BOOL CBCGPToolBarImages::m_bIsDrawOnGlass = FALSE;
BYTE CBCGPToolBarImages::m_nDisabledImageAlpha = 127;
BYTE CBCGPToolBarImages::m_nFadedImageAlpha = 150;
BOOL CBCGPToolBarImages::m_bIsRTL = FALSE;
double CBCGPToolBarImages::m_dblColorTolerance = 0.2;

// globals for fast drawing (shared globals)

/*
  DIBs use RGBQUAD format:
    0xbb 0xgg 0xrr 0x00

  Reasonably efficient code to convert a COLORREF into an
  RGBQUAD is byte-order-dependent, so we need different
  code depending on the byte order we're targeting.
*/

class CBCGPZoomKernel
{
public:
  typedef double XFilterProc(double dValue);
  typedef XFilterProc* XLPFilterProc;

  struct XKernel {
    long   pixel;
    double weight;
  };

  struct XKernelList {
    DWORD         count;
    XKernel* stat;
  };

  enum XZoomType {
    e_ZoomTypeFirst     = 0,
    e_ZoomTypeStretch   = e_ZoomTypeFirst,
    e_ZoomTypeFitImage  = 1,
    e_ZoomTypeFitWidth  = 2,
    e_ZoomTypeFitHeight = 3,
    e_ZoomTypeLast      = e_ZoomTypeFitHeight
  };

  enum XFilterType {
    e_FilterTypeFirst    = 0,
    e_FilterTypeBox      = e_FilterTypeFirst,
    e_FilterTypeBilinear = 1,
    e_FilterTypeBicubic  = 2,
    e_FilterTypeBell     = 3,
    e_FilterTypeBSpline  = 4,
    e_FilterTypeLanczos3 = 5,
    e_FilterTypeMitchell = 6,
    e_FilterTypeLast     = e_FilterTypeMitchell
  };

public:
  CBCGPZoomKernel();
  virtual                 ~CBCGPZoomKernel();

  void            Create
  (
      long sizeSrc,
      long sizeDst,
      long originSrc,
      long widthSrc,
      XFilterType ft
  );
  void            Create
  (
      long sizeSrc,
      long sizeDst,
      XFilterType ft
  );
  void            Empty();

  inline  XKernelList&
  operator [](long index);
  inline  const XKernelList&
  operator [](long index) const;

  static  double          FilterWidth(XFilterType ft);
  static  XLPFilterProc   FilterProc(XFilterType ft);
  static  double          Filter(XFilterType ft, double value);

  static  void      CorrectZoomSize(const CSize& sizeSrc, CSize& sizeDst,
      XZoomType zt);

private:
  DWORD     m_Size;
  XKernelList*
  m_List;
};

void CBCGPZoomKernel::CorrectZoomSize(const CSize& sizeSrc, CSize& sizeDst, XZoomType zt)
{
  double ZoomX = (double)sizeDst.w / (double)sizeSrc.w;
  double ZoomY = (double)sizeDst.h / (double)sizeSrc.h;

  if (zt != e_ZoomTypeStretch) {
    switch (zt) {
    case e_ZoomTypeFitWidth:
      ZoomY = ZoomX;
      break;

    case e_ZoomTypeFitHeight:
      ZoomX = ZoomY;
      break;

    case e_ZoomTypeFitImage:
      ZoomX = min(ZoomX, ZoomY);
      ZoomY = ZoomX;
      break;
    }

    sizeDst.w = (long)(sizeSrc.w * ZoomX);
    sizeDst.h = (long)(sizeSrc.h * ZoomY);
  }
}

inline
double SinC(double dValue)
{
  if (dValue != 0.0) {
    dValue *= M_PI;
    return sin(dValue) / dValue;
  }

  return 1.0;
}

inline
double
Filter_Box(double dValue)
{
  if ((dValue > -0.5) && (dValue <= 0.5)) {
    return 1.0;
  }

  return 0.0;
}

// Bilinear filter
// a.k.a. "Linear" or "Triangle" filter
inline
double
Filter_Bilinear(double dValue)
{
  if (dValue < 0.0) {
    dValue = -dValue;
  }

  if (dValue < 1.0) {
    return 1.0 - dValue;
  }

  return 0.0;
}

inline
double
Filter_Bicubic(double dValue)
{
  // f(t) = 2|t|^3 - 3|t|^2 + 1, -1 <= t <= 1
  if (dValue < 0.0) {
    dValue = -dValue;
  }

  if (dValue < 1.0) {
    return (2.0 * dValue - 3.0) * dValue * dValue + 1.0;
  }

  return 0.0;
}

// Bell filter
inline
double
Filter_Bell(double dValue)
{
  if (dValue < 0.0) {
    dValue = -dValue;
  }

  if (dValue < 0.5) {
    return 0.75 - dValue * dValue;
  }
  else {
    if (dValue < 1.5) {
      dValue = dValue - 1.5;
      return 0.5 * dValue * dValue;
    }
  }

  return 0.0;
}

inline
double
Filter_BSpline(double dValue)
{
  if (dValue < 0.0) {
    dValue = -dValue;
  }

  if (dValue < 1.0) {
    double tt = dValue * dValue;
    return 0.5 * tt * dValue - tt + 2.0 / 3.0;
  }
  else {
    if (dValue < 2.0) {
      dValue = 2.0 - dValue;
      return dValue * dValue * dValue / 6.0;
    }
  }

  return 0.0;
}

inline
double
Filter_Lanczos3(double dValue)
{
  if (dValue < 0.0) {
    dValue = -dValue;
  }

  if (dValue < 3.0) {
    return SinC(dValue) * SinC(dValue / 3.0);
  }

  return 0.0;
}

inline
double
Filter_Mitchell(double dValue)
{
  static double B = 1.0 / 3.0;
  static double C = B;

  if (dValue < 0.0) {
    dValue = -dValue;
  }

  const double tt = dValue * dValue;

  if (dValue < 1.0) {
    return ((12.0 - 9.0 * B - 6.0 * C) * (dValue * tt) +
        (-18.0 + 12.0 * B + 6.0 * C) * tt +
        (6.0 - 2.0 * B)) / 6.0;
  }
  else {
    if (dValue < 2.0) {
      return ((-1.0 * B - 6.0 * C) * (dValue * tt) +
          (6.0 * B + 30.0 * C) * tt +
          (-12.0 * B - 48.0 * C) * dValue +
          (8.0 * B + 24.0 * C)) / 6.0;
    }
  }

  return 0.0;
}


struct CBCGPImageResizeFilter {
  CBCGPZoomKernel::XLPFilterProc Proc;
  double        Width;
};

static CBCGPImageResizeFilter Filters[7] = {
  { &Filter_Box     , 0.5},
  { &Filter_Bilinear, 1.0},
  { &Filter_Bicubic , 1.0},
  { &Filter_Bell    , 1.5},
  { &Filter_BSpline , 2.0},
  { &Filter_Lanczos3, 3.0},
  { &Filter_Mitchell, 2.0}
};

CBCGPZoomKernel::XLPFilterProc
CBCGPZoomKernel::FilterProc(CBCGPZoomKernel::XFilterType ft)
{
  return Filters[ft].Proc;
}

double CBCGPZoomKernel::FilterWidth(CBCGPZoomKernel::XFilterType ft)
{
  return Filters[ft].Width;
}


double CBCGPZoomKernel::Filter(CBCGPZoomKernel::XFilterType ft, double value)
{
  return Filters[ft].Proc(value);
}


CBCGPZoomKernel::CBCGPZoomKernel()
  : m_Size(0)
  , m_List(NULL)
{
}


CBCGPZoomKernel::~CBCGPZoomKernel()
{
  Empty();
}


void CBCGPZoomKernel::Create
(
    long sizeSrc,
    long sizeDst,
    long originSrc,
    long widthSrc,
    XFilterType ft
)
{
  if (sizeSrc <= 0) {
    ASSERT(FALSE);
    return;
  }

  if (sizeDst <= 0) {
    ASSERT(FALSE);
    return;
  }

  Empty();

  m_Size = sizeDst;
  const double dScale = (double)(m_Size) / (double)(sizeSrc);

  const XLPFilterProc lpFilterProc = Filters[ft].Proc;
  const double dFilterWidth        = Filters[ft].Width;

  m_List = new XKernelList[m_Size];

  double width = dFilterWidth;
  double scale = 1.0;
  double correction = -0.25;

  if (dScale < 1.0) {
    width /= dScale;
    scale = dScale;
    correction = -correction;
  }

  for (DWORD i = 0; i < m_Size; i++) {
    double center = i / dScale;

    long left  = (long)floor(center - width);
    long right = (long)ceil(center + width);

    const long c_Count = right - left + 1;

    m_List[i].count = 0;

    if (c_Count == 0) {
      continue;
    }

    m_List[i].stat  = new XKernel[c_Count];

    bool bCross = false;
    DWORD index = 0;
    double weightSum = 0.0;

    XKernel* pStat = m_List[i].stat;

    bool bFirst = true;

    for (long j = left; j <= right; j++) {
      double weight = lpFilterProc((center - (double)j + correction) * scale) * scale;

      if (weight == 0.0) {
        if (!bFirst) {
          break;
        }

        continue;
      }

      bFirst = false;

      long pixel = j + originSrc;

      if (pixel < 0) {
        pixel = -pixel;
        bCross = true;
      }
      else if (pixel >= widthSrc) {
        pixel = 2 * widthSrc - pixel - 1;
        bCross = true;
      }

      bool bFound = false;

      if (bCross) {
        for (DWORD k = 0; k < index; k++) {
          if (pStat[k].pixel == pixel) {
            pStat[k].weight += weight;
            bFound = true;
            break;
          }
        }
      }

      if (!bFound) {
        pStat[index].pixel  = pixel;
        pStat[index].weight = weight;
        index++;
        m_List[i].count = index;
      }

      weightSum += weight;
    }

    if (weightSum != 0.0) {
      for (DWORD j = 0; j <= m_List[i].count; j++) {
        m_List[i].stat[j].weight /= weightSum;
      }
    }
  }
}


void
CBCGPZoomKernel::Create
(
    long sizeSrc,
    long sizeDst,
    XFilterType ft
)
{
  Create(sizeSrc, sizeDst, 0, sizeSrc, ft);
}


void
CBCGPZoomKernel::Empty()
{
  if (m_List != NULL) {
    for (DWORD i = 0; i < m_Size; i++) {
      if (m_List[i].count > 0) {
        delete [] m_List[i].stat;
      }
    }

    delete [] m_List;

    m_List = NULL;
    m_Size = 0;
  }
}


CBCGPZoomKernel::XKernelList&
CBCGPZoomKernel::operator [](long index)
{
  return m_List[index];
}


const CBCGPZoomKernel::XKernelList&
CBCGPZoomKernel::operator [](long index) const
{
  return m_List[index];
}

class CBCGPScanliner
{
public:
  CBCGPScanliner() {
    empty();
  }

  CBCGPScanliner
  (
      LPBYTE data,
      const CSize& size,
      size_t height = 0,
      size_t pitch  = 0,
      BYTE channels = 4,
      BOOL invert   = FALSE
  ) {
    attach(data, size, height, pitch, channels, invert);
  }

  CBCGPScanliner
  (
      LPBYTE data,
      const CRect& rect,
      size_t height = 0,
      size_t pitch  = 0,
      BYTE channels = 4,
      BOOL invert   = FALSE
  ) {
    attach(data, rect, height, pitch, channels, invert);
  }

  void            attach
  (
      LPBYTE data,
      const CSize& size,
      size_t height = 0,
      size_t pitch  = 0,
      BYTE channels = 4,
      BOOL invert   = FALSE
  ) {
    attach(data, CRect(CPoint(0, 0), size), height, pitch, channels, invert);
  }

  void            attach
  (
      LPBYTE data,
      const CRect& rect,
      size_t height = 0,
      size_t pitch  = 0,
      BYTE channels = 4,
      BOOL invert   = FALSE
  ) {
    empty();

    ASSERT(data != NULL);

    CPoint point(rect.TopLeft());
    CSize size(rect.Size());

    if (pitch == 0) {
      pitch = size.w;
    }

    if (height == 0) {
      height = point.y + size.h;
    }

    ASSERT((size_t)size.w <= pitch);
    ASSERT((size_t)(point.y + size.h) <= height);

    m_rows     = size.h;
    m_cols     = size.w * channels;
    m_pitch    = (DWORD)pitch;
    m_offset   = (long)m_pitch;

    if (invert) {
      m_offset = -m_offset;
    }

    m_channels = channels;
    m_height   = (DWORD)height;

    m_start_row = point.y;
    m_start_col = point.x;

    m_line_begin = _begin(data);
    m_line_end   = _end(data);
    m_line       = m_line_begin;
  }

  inline  LPBYTE          begin() {
    m_line = m_line_begin;
    return m_line;
  }
  inline  LPBYTE          end() {
    m_line = m_line_end;
    return m_line;
  }

  inline  LPBYTE          operator[](long index) {
    return m_line_begin + m_offset * index;
  }
  inline  const LPBYTE    operator[](long index) const {
    return m_line_begin + m_offset * index;
  }

  inline  LPBYTE          get() {
    return m_line;
  }
  inline  const LPBYTE    get() const {
    return m_line;
  }

  inline  DWORD           pitch() const {
    return m_pitch;
  }
  inline  DWORD           rows() const {
    return m_rows;
  }
  inline  DWORD           cols() const {
    return m_cols;
  }
  inline  BYTE      channels() const {
    return m_channels;
  }

  ~CBCGPScanliner() {
    empty();
  }

  inline  const CBCGPScanliner&
  operator += (DWORD line) {
    m_line += m_offset * line;
    return *this;
  }

  inline  const CBCGPScanliner&
  operator -= (DWORD line) {
    m_line -= m_offset * line;
    return *this;
  }

  inline  const CBCGPScanliner&
  operator ++ () {
    m_line += m_offset;
    return *this;
  }

  inline  const CBCGPScanliner&
  operator ++ (int) {
    m_line += m_offset;
    return *this;
  }

  inline  const CBCGPScanliner&
  operator -- () {
    m_line -= m_offset;
    return *this;
  }

  inline  const CBCGPScanliner&
  operator -- (int) {
    m_line += m_offset;
    return *this;
  }

protected:
  void            empty() {
    m_line      = NULL;
    m_pitch     = 0;
    m_start_row = 0;
    m_start_col = 0;
    m_rows      = 0;
    m_cols      = 0;
    m_offset    = 0;
    m_height    = 0;

    m_line_begin = NULL;
    m_line_end   = NULL;
  }

  inline  LPBYTE          _begin(LPBYTE data) const {
    LPBYTE line = data;

    if (m_offset > 0) {
      line += m_start_row * m_pitch;
    }
    else {
      line += (m_height - m_start_row - 1) * m_pitch;
    }

    if (m_start_col != 0) {
      line += m_start_col * m_channels;
    }

    return line;
  }

  inline  LPBYTE          _end(LPBYTE data) const {
    LPBYTE line = data;

    if (m_offset > 0) {
      line += (m_start_row + m_rows - 1) * m_pitch;
    }
    else {
      line += (m_height - m_start_row - m_rows) * m_pitch;
    }

    if (m_start_col != 0) {
      line += m_start_col * m_channels;
    }

    return line;
  }

private:
  LPBYTE          m_line;
  LPBYTE          m_line_begin;
  LPBYTE          m_line_end;
  DWORD           m_pitch;
  DWORD           m_start_row;
  DWORD           m_start_col;
  DWORD           m_rows;
  DWORD           m_cols;
  long            m_offset;
  BYTE      m_channels;
  DWORD           m_height;
};

class CBCGPScanlinerBitmap: public CBCGPScanliner
{
public:
  CBCGPScanlinerBitmap() {
    empty();
  }

  void      attach(CBitmap* bitmap, const CPoint& ptBegin = CPoint(0, 0)) {
    if (bitmap == NULL) {
      ASSERT(FALSE);
      return;
    }

    BITMAP bmp;

    if (::GetObject(bitmap, sizeof(BITMAP), &bmp) == 0 ||
        bmp.bmBits == 0 || bmp.bmBitsPixel < 24) {
      ASSERT(FALSE);
      return;
    }

    CSize size(bmp.bmWidth, abs(bmp.bmHeight));
    CRect rect(CPoint(0, 0), size);
    rect.IntersectRect(CRect(ptBegin, size), rect);

    int channels = bmp.bmBitsPixel / 8;
    int pitch = channels * size.w;

    if (pitch % 4) {
      pitch += 4 - (pitch % 4);
    }

    CBCGPScanliner::attach((LPBYTE) bmp.bmBits,
        size, size.h, pitch, (BYTE) channels, FALSE);
  }
};

/////////////////////////////////////////////////////////////////////////////
// Init / Term

// a special struct that will cleanup automatically
struct _AFX_TOOLBAR_TERM {
};

static const _AFX_TOOLBAR_TERM toolbarTerm;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPToolBarImages::CBCGPToolBarImages()
{
  m_bModified = FALSE;
  m_bReadOnly = FALSE;
  m_iCount = 0;
  m_bIsGray = FALSE;
  m_nGrayPercentage = 0;

  m_bUserImagesList = FALSE;

  // initialize the toolbar drawing engine

  m_clrTransparentOriginal = m_clrTransparent = (COLORREF) - 1;

  // UISG standard sizes
  m_sizeImage = CSize(16, 15);
  m_sizeImageOriginal = CSize(0, 0);
  m_sizeImageDest = CSize(0, 0);
  m_rectLastDraw = CRect(0, 0, 0, 0);
  m_rectSubImage = CRect(0, 0, 0, 0);
  m_bStretch = FALSE;
  m_pBmpOriginal = NULL;

  m_bFadeInactive = FALSE;
  m_nBitsPerPixel = 0;

  m_nLightPercentage = 130;
  m_bAlwaysLight = FALSE;

  m_bMapTo3DColors = TRUE;
  m_bAutoCheckPremlt = FALSE;
  m_bCreateMonoDC = TRUE;

  m_dblScale = 1.0;

  OnSysColorChange();
}
//*******************************************************************************
CBCGPToolBarImages::~CBCGPToolBarImages()
{
  ASSERT(m_pBmpOriginal == NULL);
}
//*******************************************************************************
BOOL CBCGPToolBarImages::Load(UINT uiResID, HINSTANCE hinstRes, BOOL bAdd)
{
  return LoadStr(MAKEINTRESOURCE(uiResID), hinstRes, bAdd);
}
//*******************************************************************************
BOOL CBCGPToolBarImages::LoadStr(LPCTSTR lpszResourceName, HINSTANCE hinstRes, BOOL bAdd)
{
#if 0

  if (m_bIsTemporary) {
    ASSERT(FALSE);
    return FALSE;
  }

  if (lpszResourceName == NULL) {
    ASSERT(FALSE);
    return FALSE;
  }

  UINT uiResID = IS_INTRESOURCE(lpszResourceName) ? (UINT)((UINT_PTR)(lpszResourceName)) : 0;

  if (!bAdd) {
    AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);     // get rid of old one
    m_lstOrigResIds.RemoveAll();
    m_lstOrigResInstances.RemoveAll();
    m_mapOrigResOffsets.RemoveAll();
  }
  else if (uiResID != 0 &&
      m_lstOrigResIds.Find(uiResID) != NULL) { // Already loaded, do nothing
    return TRUE;
  }

  CBitmap* hbmp = NULL;

  {
    if (hinstRes == NULL) {
      hinstRes = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
    }

    UINT uiLoadImageFlags = LR_CREATEDIBSECTION;

    if (m_bMapTo3DColors && !globalData.m_bIsBlackHighContrast) {
      uiLoadImageFlags |= LR_LOADMAP3DCOLORS;
    }

    hbmp = (CBitmap*) ::LoadImage(
        hinstRes,
        lpszResourceName,
        IMAGE_BITMAP,
        0, 0,
        uiLoadImageFlags);
  }

  if (hbmp == NULL) {
    if (uiResID != 0) {
      TRACE(_T("Can't load image: %x. GetLastError() = %x\n"),
          uiResID,
          GetLastError());
    }
    else {
      TRACE(_T("Can't load bitmap: %s. GetLastError() = %x\n"),
          lpszResourceName,
          GetLastError());
    }

    return FALSE;
  }

  BITMAP bmp;

  if (::GetObject(hbmp, sizeof(BITMAP), &bmp) == 0) {
    ASSERT(FALSE);
    ::DeleteObject(hbmp);
    return FALSE;
  }

  if (bmp.bmBitsPixel >= 32) {
    PreMultiplyAlpha(hbmp);
  }
  else if ((bmp.bmBitsPixel > 8 && m_bMapTo3DColors) || globalData.m_bIsBlackHighContrast) {
    //------------------------------------------------
    // LR_LOADMAP3DCOLORS don't support > 8bpp images,
    // we should convert it now:
    //------------------------------------------------
    MapBmpTo3dColors(hbmp, FALSE);
  }

  m_nBitsPerPixel = max(m_nBitsPerPixel, bmp.bmBitsPixel);

  if (bAdd) {
    if (uiResID != 0) {
      m_mapOrigResOffsets.SetAt(uiResID, m_iCount);
    }

    AddImage(hbmp);

    if (uiResID != 0) {
      m_lstOrigResIds.AddTail(uiResID);
      m_lstOrigResInstances.AddTail(hinstRes);
    }

    ::DeleteObject(hbmp);
  }
  else {
    m_hbmImageWell = hbmp;
  }

  UpdateCount();

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;

#endif
  return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::Load(LPCTSTR lpszBmpFileName)
{
#if 0
  ASSERT(lpszBmpFileName != NULL);

  CString strPath = lpszBmpFileName;

  //-----------------------------------------------------------------------
  // If the image path is not defined, try to open it in the EXE directory:
  //-----------------------------------------------------------------------
  if (strPath.Find(_T("\\")) == -1 &&
      strPath.Find(_T("/")) == -1 &&
      strPath.Find(_T(":")) == -1) {
    TCHAR lpszFilePath [_MAX_PATH];

    if (::GetModuleFileName(NULL, lpszFilePath, _MAX_PATH) > 0) {
      TCHAR path_buffer[_MAX_PATH];
      TCHAR drive[_MAX_DRIVE];
      TCHAR dir[_MAX_DIR];
      TCHAR fname[_MAX_FNAME];
      TCHAR ext[_MAX_EXT];

#if _MSC_VER < 1400
      _tsplitpath(lpszFilePath, drive, dir, NULL, NULL);
      _tsplitpath(lpszBmpFileName, NULL, NULL, fname, ext);

      _tmakepath(path_buffer, drive, dir, fname, ext);
#else
      _tsplitpath_s(lpszFilePath, drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
      _tsplitpath_s(lpszBmpFileName, NULL, 0, NULL, 0, fname, _MAX_FNAME, ext, _MAX_EXT);

      _tmakepath_s(path_buffer, drive, dir, fname, ext);
#endif

      strPath = path_buffer;
    }
  }

  //--------------------------------
  // Load images from the disk file:
  //--------------------------------
  {
    UINT uiLoadImageFlags = LR_LOADFROMFILE | LR_CREATEDIBSECTION;

    if (m_bMapTo3DColors) {
      uiLoadImageFlags |= LR_LOADMAP3DCOLORS;
    }

    m_hbmImageWell = (CBitmap*) ::LoadImage(
        AfxGetInstanceHandle(),
        strPath,
        IMAGE_BITMAP,
        0, 0,
        uiLoadImageFlags);
  }

  if (m_hbmImageWell == NULL) {
    TRACE(_T("Can't load bitmap: %s. GetLastError() = %x\r\n"),
        lpszBmpFileName,
        GetLastError());
    return FALSE;
  }

  BITMAP bmp;

  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    ASSERT(FALSE);
    ::DeleteObject(m_hbmImageWell);
    m_hbmImageWell = NULL;
    return FALSE;
  }

  m_bUserImagesList = TRUE;
  m_strUDLPath = strPath;

  if (::GetFileAttributes(strPath) & FILE_ATTRIBUTE_READONLY) {
    m_bReadOnly = TRUE;
  }

  m_nBitsPerPixel = bmp.bmBitsPixel;

  if (m_nBitsPerPixel > 8 && m_nBitsPerPixel < 32) {
    //------------------------------------------------
    // LR_LOADMAP3DCOLORS don't support > 8bpp images,
    // we should convert it now:
    //------------------------------------------------
    MapTo3dColors(FALSE);
  }

  if (bmp.bmBitsPixel >= 32) {
    PreMultiplyAlpha(m_hbmImageWell);
  }

  UpdateCount();

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;

#endif
  return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::PrepareDrawImage(CSize sizeImageDest, BOOL bFadeInactive)
{
  if (m_bMultiThreaded) {
    g_cs.Lock();
  }

  if (bFadeInactive && m_nBitsPerPixel < 32) {
    UpdateInternalImage(BCGImage_Light);
  }

  if (m_nBitsPerPixel < 32 &&
      //CBCGPVisualManager::GetInstance ()->IsShadowHighlightedImage () &&
      !globalData.IsHighContastMode()) {
    UpdateInternalImage(BCGImage_Shadow);
  }

  m_bStretch = FALSE;

  if (
      (m_nBitsPerPixel > 4 && !m_bAlwaysLight) || m_nBitsPerPixel == 0) {
    // Down't fade 256+ or unknown bitmaps
    bFadeInactive = FALSE;
  }

  m_bFadeInactive = bFadeInactive;

  ASSERT(m_pBmpOriginal == NULL);

  // We need to kick-start the bitmap selection process.
  CBitmap* bmp = bFadeInactive && m_nBitsPerPixel < 32 ? m_hbmImageLight : m_hbmImageWell;

  if (sizeImageDest.w <= 0 || sizeImageDest.h <= 0) {
    m_sizeImageDest = m_sizeImage;
  }
  else {
    m_sizeImageDest = sizeImageDest;
  }

  COLORREF clrTransparent = m_nBitsPerPixel == 32 ? (COLORREF) - 1 : m_clrTransparent;

  return TRUE;
}
//*******************************************************************************
void CBCGPToolBarImages::EndDrawImage(CBCGPDrawState& ds)
{
}
//*******************************************************************************
void CBCGPToolBarImages::CreateMask(int iImage, BOOL bHilite, BOOL bHiliteShadow)
{
#if 0
  // initalize whole area with 0's
  PatBlt(hDCMono, 0, 0, m_sizeImage.w + 2, m_sizeImage.h + 2, WHITENESS);

  COLORREF clrTransparent = m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL ?
      (COLORREF) - 1 : m_clrTransparent;

  // create mask based on color bitmap
  // convert this to 1's
  SetBkColor(hDCGlyphs,
      clrTransparent != -1 ? clrTransparent : globalData.clrBtnFace);

  ::BitBlt(hDCMono, 0, 0, m_sizeImage.w, m_sizeImage.h,
      hDCGlyphs, iImage * m_sizeImage.w, 0, SRCCOPY);

  if (bHilite) {
    // convert this to 1's
    SetBkColor(hDCGlyphs, globalData.clrBtnHilite);

    // OR in the new 1's
    ::BitBlt(hDCMono, 0, 0, m_sizeImage.w, m_sizeImage.h,
        hDCGlyphs, iImage * m_sizeImage.w, 0, SRCPAINT);

    if (bHiliteShadow) {
      ::BitBlt(hDCMono, 1, 1, m_sizeImage.w + 1, m_sizeImage.h + 1,
          hDCMono, 0, 0, SRCAND);
    }
  }

#endif
}
//********************************************************************************
int CBCGPToolBarImages::GetMask(int iImage, CBitmap* bitmapMask)
{
  PrepareDrawImage(FALSE);
  CreateMask(iImage, FALSE, FALSE);

  if (bitmapMask->CreateBitmap(m_sizeImage.w, m_sizeImage.h, 1, 1, NULL)) {
    //memDCDest.BitBlt (0, 0, m_sizeImage.w, m_sizeImage.h, pDCMono, 0, 0, SRCCOPY);
  }

  return 0;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::Draw(CDC* pDCDest,
    int xDest, int yDest,
    int iImage,
    BOOL bHilite,
    BOOL bDisabled,
    BOOL bIndeterminate,
    BOOL bShadow,
    BOOL bInactive,
    BYTE alphaSrc/* = 255*/)
{
#if 0

  if (iImage < 0 || iImage >= m_iCount) {
    return FALSE;
  }

  if (bShadow && globalData.m_nBitsPerPixel <= 8) {
    return TRUE;
  }

  m_rectLastDraw = CRect(CPoint(xDest, yDest), m_sizeImageDest);

  if (m_bStretch) {
    bHilite = FALSE;
    bIndeterminate = FALSE;
  }

  CBitmap* hBmpOriginal = NULL;

  if ((!bInactive || bDisabled) && m_bFadeInactive && m_nBitsPerPixel < 32) {
    hBmpOriginal = (CBitmap*) SelectObject(hDCGlyphs, m_hbmImageWell);
  }

  BOOL bStretchOld = m_bStretch;
  BOOL bAlphaStretch =
      (m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL && m_bStretch);

  if (bAlphaStretch) {
    m_bStretch = FALSE;
  }

  COLORREF clrTransparent = (m_nBitsPerPixel == 32 || m_bIsDrawOnGlass) &&
      m_pfAlphaBlend != NULL
      ? (COLORREF) - 1
      : m_clrTransparent;

  BOOL bIsTransparent = (clrTransparent != (COLORREF) - 1);

  COLORREF clrTransparentDisabled = clrTransparent;

  CDC* pDC = m_bStretch || bIsTransparent ? &m_dcMem : pDCDest;
  ASSERT_VALID(pDC);

  int x = m_bStretch || bIsTransparent ? 0 : xDest;
  int y = m_bStretch || bIsTransparent ? 0 : yDest;

  const int xOffset = m_rectSubImage.l;
  const int yOffset = m_rectSubImage.top;

  const int nWidth = m_rectSubImage.IsRectEmpty() ? m_sizeImage.w : m_rectSubImage.Width();
  const int nHeight = m_rectSubImage.IsRectEmpty() ? m_sizeImage.h : m_rectSubImage.Height();

  if (m_bStretch || bIsTransparent) {
    CRect rectImage(CPoint(0, 0), m_sizeImage);

    if (bIsTransparent && clrTransparent != globalData.clrBtnFace) {
      CBrush brBackgr(clrTransparent);
      pDC->FillRect(rectImage, &brBackgr);
    }
    else {
      pDC->FillRect(rectImage, &globalData.brBtnFace);
    }

    if (bDisabled && globalData.m_nBitsPerPixel == 16) {
      clrTransparentDisabled = pDC->GetPixel(rectImage.TopLeft());
    }
  }

  BOOL bDisabledTrueColor = FALSE;

  if (bDisabled && m_nBitsPerPixel >= 24) {
    bDisabled = FALSE;
    bDisabledTrueColor = TRUE;
  }

  BOOL bShadowTrueColor = FALSE;

  if (bShadow && m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL) {
    bShadow = FALSE;
    bShadowTrueColor = TRUE;
  }

  if (!bHilite && !bDisabled && !bShadow) {
    BOOL bIsReady = FALSE;

    if ((m_nBitsPerPixel == 32 || m_bIsDrawOnGlass) && m_pfAlphaBlend != NULL) {
      BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0,
                    alphaSrc, 1 /*AC_SRC_ALPHA*/
                                 };

      if (bDisabledTrueColor) {
        pixelblend.SourceConstantAlpha = m_nDisabledImageAlpha;
      }

      if (bInactive && m_bFadeInactive) {
        pixelblend.SourceConstantAlpha = m_nFadedImageAlpha;
      }

      ASSERT(m_pfAlphaBlend != NULL);

      if (globalData.bIsWindows9x) {
        BITMAPINFO bi;

        // Fill in the BITMAPINFOHEADER
        bi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth         = nWidth;
        bi.bmiHeader.biHeight        = nHeight;
        bi.bmiHeader.biPlanes        = 1;
        bi.bmiHeader.biBitCount      = 32;
        bi.bmiHeader.biCompression   = BI_RGB;
        bi.bmiHeader.biSizeImage     = nWidth * nHeight;
        bi.bmiHeader.biXPelsPerMeter = 0;
        bi.bmiHeader.biYPelsPerMeter = 0;
        bi.bmiHeader.biClrUsed       = 0;
        bi.bmiHeader.biClrImportant  = 0;

        CDC dcMem;
        dcMem.CreateCompatibleDC(NULL);

        COLORREF* pBits = NULL;
        CBitmap* hbmp = CreateDIBSection(
            dcMem.m_hDC, &bi, DIB_RGB_COLORS, (void**)&pBits,
            NULL, NULL);

        if (hbmp == NULL) {
          ASSERT(FALSE);
          return FALSE;
        }

        CBitmap* bmpMem;
        bmpMem.Attach(hbmp);

        CBitmap* pBmpOriginal = dcMem.SelectObject(&bmpMem);

        dcMem.FillRect(CRect(0, 0, nWidth, nHeight), &globalData.brBtnFace);

        ::BitBlt(dcMem.m_hDC, 0, 0,
            nWidth, nHeight,
            hDCGlyphs, iImage * m_sizeImage.w + xOffset, yOffset, SRCCOPY);

        bIsReady = (*m_pfAlphaBlend)(pDC->m_hDC, x, y,
            m_sizeImage.w, m_sizeImage.h,
            dcMem.m_hDC, 0, 0,
            nWidth, nHeight, pixelblend);

        dcMem.SelectObject(pBmpOriginal);
      }
      else {
        const CSize sizeDest = bAlphaStretch ? m_sizeImageDest : m_sizeImage;

        if (m_nBitsPerPixel != 32) {
          BITMAPINFO bi;

          // Fill in the BITMAPINFOHEADER
          bi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
          bi.bmiHeader.biWidth         = nWidth;
          bi.bmiHeader.biHeight        = nHeight;
          bi.bmiHeader.biPlanes        = 1;
          bi.bmiHeader.biBitCount      = 32;
          bi.bmiHeader.biCompression   = BI_RGB;
          bi.bmiHeader.biSizeImage     = nWidth * nHeight;
          bi.bmiHeader.biXPelsPerMeter = 0;
          bi.bmiHeader.biYPelsPerMeter = 0;
          bi.bmiHeader.biClrUsed       = 0;
          bi.bmiHeader.biClrImportant  = 0;

          COLORREF* pBits = NULL;
          CBitmap* hbmp = CreateDIBSection(NULL, &bi, DIB_RGB_COLORS, (LPVOID*)&pBits, NULL, 0);

          if (hbmp == NULL) {
            ASSERT(FALSE);
            return FALSE;
          }

          CBitmap* bmpMem;
          bmpMem.Attach(hbmp);

          CDC dcMem;
          dcMem.CreateCompatibleDC(NULL);
          CBitmap* pBmpOld = dcMem.SelectObject(&bmpMem);

          ::BitBlt(dcMem,
              0, 0, nWidth, nHeight,
              hDCGlyphs, iImage * m_sizeImage.w + xOffset, yOffset, SRCCOPY);

          int nSizeImage = nHeight * nWidth;

          if (m_clrTransparent == -1) {
            for (int i = 0; i < nSizeImage; i++) {
              *pBits |= 0xFF000000;
              pBits++;
            }
          }
          else {
            COLORREF clrTrans = RGB(GetBValue(m_clrTransparent),
                GetGValue(m_clrTransparent),
                GetRValue(m_clrTransparent));

            for (int i = 0; i < nSizeImage; i++) {
              if (*pBits != clrTrans) {
                *pBits |= 0xFF000000;
              }
              else {
                *pBits = (COLORREF)0;
              }

              pBits++;
            }
          }

          bIsReady = (*m_pfAlphaBlend)(pDC->m_hDC, x, y,
              sizeDest.w, sizeDest.h,
              dcMem, 0, 0,
              nWidth, nHeight, pixelblend);

          dcMem.SelectObject(pBmpOld);
        }
        else {
          bIsReady = (*m_pfAlphaBlend)(pDC->m_hDC, x, y,
              sizeDest.w, sizeDest.h,
              hDCGlyphs, iImage * m_sizeImage.w + xOffset, yOffset,
              nWidth, nHeight, pixelblend);
        }
      }
    }

    if (!bIsReady) {
      //----------------------
      // normal image version:
      //----------------------
      ::BitBlt(pDC->m_hDC, x, y,
          nWidth, nHeight,
          hDCGlyphs, iImage * m_sizeImage.w + xOffset, yOffset, SRCCOPY);

      if (bDisabledTrueColor) {
        CBCGPDrawManager dm(*pDC);
        dm.GrayRect(CRect(x, y, x + nWidth + 2, y + nHeight + 2),
            -1, clrTransparentDisabled == -1 ? globalData.clrBtnFace : clrTransparentDisabled,
            CBCGPVisualManager::GetInstance()->GetToolbarDisabledColor());
      }
    }
  }
  else if (bShadow && m_hbmImageShadow != NULL) {
    CBitmap* hbmpCurr =
        (CBitmap*) SelectObject(hDCGlyphs, m_hbmImageShadow);

    ::BitBlt(pDC->m_hDC, x, y,
        nWidth, nHeight,
        hDCGlyphs, iImage * m_sizeImage.w + xOffset, yOffset, SRCCOPY);

    SelectObject(hDCGlyphs, hbmpCurr);
  }
  else {
    if (bDisabled || bIndeterminate || bShadow) {
      // disabled or indeterminate version
      CreateMask(iImage, TRUE, FALSE);

      pDC->SetTextColor(bShadow ? m_clrImageShadow : 0L); // 0's in mono -> 0 (for ROP)
      pDC->SetBkColor((COLORREF)0x00FFFFFFL); // 1's in mono -> 1

      if (bDisabled && CBCGPVisualManager::GetInstance()->IsEmbossDisabledImage()) {
        // disabled - draw the hilighted shadow
        HGDIOBJ hbrOld = pDC->SelectObject(globalData.hbrBtnHilite);

        if (hbrOld != NULL) {
          // draw hilight color where we have 0's in the mask
          ::BitBlt(pDC->m_hDC, x + 1, y + 1,
              nWidth + 2, nHeight + 2,
              hDCMono, 0, 0, ROP_PSDPxax);

          pDC->SelectObject(hbrOld);
        }
      }

      //BLOCK: always draw the shadow
      {
        HGDIOBJ hbrOld = pDC->SelectObject(globalData.hbrBtnShadow);

        if (hbrOld != NULL) {
          // draw the shadow color where we have 0's in the mask
          ::BitBlt(pDC->m_hDC,
              x, y,
              nWidth + 2, nHeight + 2,
              hDCMono, 0, 0, ROP_PSDPxax);

          pDC->SelectObject(hbrOld);
        }
      }
    }

    // if it is checked do the dither brush avoiding the glyph
    if (bHilite || bIndeterminate) {
      CBrush* pBrOld = pDC->SelectObject(&globalData.brLight);

      if (pBrOld != NULL) {
        CreateMask(iImage, !bIndeterminate, bDisabled);

        pDC->SetTextColor(0L);              // 0 -> 0
        pDC->SetBkColor((COLORREF)0x00FFFFFFL); // 1 -> 1

        // only draw the dither brush where the mask is 1's
        ::BitBlt(pDC->m_hDC, x, y,
            nWidth, nHeight,
            hDCMono, 0, 0, ROP_DSPDxax);

        pDC->SelectObject(pBrOld);
      }
    }
  }

  if (m_bStretch) {
    TransparentBlt(pDCDest->GetSafeHdc(), xDest, yDest,
        nWidth, nHeight,
        pDC, 0, 0,
        bIsTransparent ? clrTransparent : globalData.clrBtnFace,
        m_sizeImageDest.w, m_sizeImageDest.h);
  }
  else if (bIsTransparent) {
    TransparentBlt(pDCDest->GetSafeHdc(), xDest, yDest,
        nWidth, nHeight,
        pDC, 0, 0, clrTransparent);
  }

  if (hBmpOriginal != NULL) {
    SelectObject(hDCGlyphs, hBmpOriginal);
  }

  m_bStretch = bStretchOld;
#endif
  return TRUE;
}
//********************************************************************************
BOOL CBCGPToolBarImages::DrawEx(CDC* pDC,
    CRect rect,
    int iImageIndex,
    ImageAlignHorz horzAlign/* = ImageAlignHorzLeft*/,
    ImageAlignVert vertAlign/* = ImageAlignVertTop*/,
    CRect rectSrc/* = CRect (0, 0, 0, 0)*/,
    BYTE alphaSrc/* = 255*/)
{
#if 0

  ASSERT_VALID(pDC);

  if (rectSrc.IsRectEmpty()) {
    rectSrc = CRect(CPoint(0, 0), m_sizeImage);
  }

  if (rectSrc.IsRectEmpty()) {
    return FALSE;
  }

  CRect rectDst(rect);

  if (horzAlign != ImageAlignHorzStretch) {
    BOOL bUpdate = TRUE;

    if (horzAlign == ImageAlignHorzLeft) {
      rectDst.r = rectDst.l + rectSrc.Width();
    }
    else if (horzAlign == ImageAlignHorzRight) {
      rectDst.l = rectDst.r - rectSrc.Width();
    }
    else if (horzAlign == ImageAlignHorzCenter) {
      rectDst.l += (rectDst.Width() - rectSrc.Width()) / 2;
      rectDst.r = rectDst.l + rectSrc.Width();
    }
    else {
      bUpdate = FALSE;
    }

    if (bUpdate) {
      CRect rt(rectDst);
      rectDst.IntersectRect(rectDst, rect);

      if (0 < rectDst.Width() && rectDst.Width() !=  rectSrc.Width()) {
        rectSrc.l += rectDst.l - rt.l;
        rectSrc.r = rectSrc.l + min(rectDst.Width(), rectSrc.Width());
      }
    }
  }

  if (vertAlign != ImageAlignVertStretch) {
    BOOL bUpdate = TRUE;

    if (vertAlign == ImageAlignVertTop) {
      rectDst.bottom = rectDst.top + rectSrc.Height();
    }
    else if (vertAlign == ImageAlignVertBottom) {
      rectDst.top = rectDst.bottom - rectSrc.Height();
    }
    else if (vertAlign == ImageAlignVertCenter) {
      rectDst.top += (rectDst.Height() - rectSrc.Height()) / 2;
      rectDst.bottom = rectDst.top + rectSrc.Height();
    }
    else {
      bUpdate = FALSE;
    }

    if (bUpdate) {
      CRect rt(rectDst);
      rectDst.IntersectRect(rectDst, rect);

      if (0 < rectDst.Height() && rectDst.Height() !=  rectSrc.Height()) {
        rectSrc.top += rectDst.top - rt.top;
        rectSrc.bottom = rectSrc.top + min(rectDst.Height(), rectSrc.Height());
      }
    }
  }

  if (rectSrc.IsRectEmpty() || rectDst.IsRectEmpty()) {
    return FALSE;
  }

  if (!globalData.bIsWindows9x) {
    CBitmap* hbmOldGlyphs = (CBitmap*)SelectObject(hDCGlyphs, m_hbmImageWell);

    const int xOffset = rectSrc.l;
    const int yOffset = rectSrc.top;

    const int nWidth = rectSrc.IsRectEmpty() ? m_sizeImage.w : rectSrc.Width();
    const int nHeight = rectSrc.IsRectEmpty() ? m_sizeImage.h : rectSrc.Height();

    BOOL bRes = FALSE;

    if (m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL) {
      BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0,
                    alphaSrc, 1 /*AC_SRC_ALPHA*/
                                 };

      bRes = (*m_pfAlphaBlend)(pDC->m_hDC, rectDst.l, rectDst.top,
          rectDst.Width(), rectDst.Height(),
          hDCGlyphs, iImageIndex * m_sizeImage.w + xOffset, yOffset,
          nWidth, nHeight, pixelblend);
    }
    else if (m_clrTransparent == -1) {
      bRes = ::StretchBlt(pDC->m_hDC, rectDst.l, rectDst.top,
          rectDst.Width(), rectDst.Height(),
          hDCGlyphs, iImageIndex * m_sizeImage.w + xOffset, yOffset,
          nWidth, nHeight, SRCCOPY);
    }

    SelectObject(hDCGlyphs, hbmOldGlyphs);

    if (bRes) {
      return TRUE;
    }
  }

  BOOL bCreateMonoDC = m_bCreateMonoDC;
  m_bCreateMonoDC = FALSE;

  CBCGPDrawState ds;

  if (!PrepareDrawImage(ds, rectDst.Size())) {
    m_bCreateMonoDC = bCreateMonoDC;
    return FALSE;
  }

  m_rectSubImage = rectSrc;

  BOOL bRes = Draw(pDC, rectDst.l, rectDst.top, iImageIndex,
      FALSE, FALSE, FALSE, FALSE, FALSE, alphaSrc);

  m_rectSubImage.SetRectEmpty();

  EndDrawImage(ds);
  m_bCreateMonoDC = bCreateMonoDC;
#endif
  return 0;
}
//********************************************************************************

//********************************************************************************
void CBCGPToolBarImages::OnSysColorChange()
{
#if 0

  if (m_bIsTemporary) {
    return;
  }

  if (m_dblScale != 1.0) {
    m_dblScale = 1.0;
    m_nBitsPerPixel = 0;

    if (m_clrTransparentOriginal != (COLORREF) - 1) {
      m_clrTransparent = m_clrTransparentOriginal;
      m_clrTransparentOriginal = (COLORREF) - 1;
    }

    m_sizeImage = m_sizeImageOriginal;
    m_sizeImageOriginal = CSize(0, 0);
    m_sizeImageDest = CSize(0, 0);
    m_rectLastDraw = CRect(0, 0, 0, 0);
    m_rectSubImage = CRect(0, 0, 0, 0);
  }

  // re-color bitmap for toolbar
  if (m_hbmImageWell != NULL) {
    if (m_bUserImagesList) {
      Load(m_strUDLPath);
    }
    else {
      // Image was buit from the resources...
      if (m_lstOrigResIds.IsEmpty()) {
        return;
      }

      ASSERT(m_lstOrigResInstances.GetCount() == m_lstOrigResIds.GetCount());

      AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);     // get rid of old one

      POSITION posInst = m_lstOrigResInstances.GetHeadPosition();

      for (POSITION pos = m_lstOrigResIds.GetHeadPosition();
          pos != NULL;) {
        ASSERT(posInst != NULL);

        UINT uiResId = m_lstOrigResIds.GetNext(pos);
        ASSERT(uiResId > 0);

        HINSTANCE hInst = m_lstOrigResInstances.GetNext(posInst);
        ASSERT(hInst != NULL);

        CBitmap* hbmp = NULL;

        if (hbmp == NULL) {
          UINT uiLoadImageFlags = LR_CREATEDIBSECTION;

          if (m_bMapTo3DColors && !globalData.IsHighContastMode()) {
            uiLoadImageFlags |= LR_LOADMAP3DCOLORS;
          }

          hbmp = (CBitmap*) ::LoadImage(
              hInst,
              MAKEINTRESOURCE(uiResId),
              IMAGE_BITMAP,
              0, 0,
              uiLoadImageFlags);
        }

        BITMAP bmp;

        if (::GetObject(hbmp, sizeof(BITMAP), &bmp) == 0) {
          ASSERT(FALSE);
        }

        m_nBitsPerPixel = bmp.bmBitsPixel;

        if (bmp.bmBitsPixel >= 32) {
          PreMultiplyAlpha(hbmp);
        }
        else if ((bmp.bmBitsPixel > 8 && m_bMapTo3DColors) || globalData.m_bIsBlackHighContrast) {
          //------------------------------------------------
          // LR_LOADMAP3DCOLORS don't support > 8bpp images,
          // we should convert it now:
          //------------------------------------------------
          MapBmpTo3dColors(hbmp, FALSE);
        }

        AddImage(hbmp);

        ::DeleteObject(hbmp);
      }
    }
  }

  UpdateCount();

  if (m_bIsRTL) {
    MirrorBitmap(m_hbmImageWell, m_sizeImage.w);
  }

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;

  if (m_bIsGray) {
    GrayImages(m_nGrayPercentage);
  }

  m_clrImageShadow = globalData.clrBtnShadow;
#endif
}
#if 0
//********************************************************************************
void CBCGPToolBarImages::UpdateCount()
{
  if (m_hbmImageWell == NULL) {
    m_iCount = 0;
    return;
  }

  BITMAP bmp;

  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    m_iCount = 0;
    return;
  }

  int iWidth = bmp.bmWidth;
  m_iCount = iWidth / m_sizeImage.w;
}

//////////////////////////////////////////
// Image editing methods:
//////////////////////////////////////////

int CBCGPToolBarImages::AddImage(CBitmap* hbmp, BOOL bSetBitPerPixel/* = FALSE*/)
{
#if 0

  if (m_bIsTemporary) {
    ASSERT(FALSE);
    return -1;
  }

  BOOL bIsMirror = FALSE;

  if (m_bIsRTL) {
    bIsMirror = TRUE;

    hbmp = (CBitmap*) ::CopyImage(hbmp, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    MirrorBitmap(hbmp, m_sizeImage.w);
  }

  if (IsScaled()) {
    BITMAP bmpScale;

    if (::GetObject(hbmp, sizeof(BITMAP), &bmpScale) == 0) {
      return -1;
    }

    if (bmpScale.bmHeight != m_sizeImage.h) {
      CBCGPToolBarImages imageForScale;
      imageForScale.m_hbmImageWell = hbmp;

      imageForScale.m_nBitsPerPixel = bmpScale.bmBitsPixel;

      imageForScale.SetImageSize(m_sizeImageOriginal);
      imageForScale.m_iCount = bmpScale.bmWidth / m_sizeImageOriginal.w;
      imageForScale.SmoothResize(m_dblScale);
      imageForScale.m_bIsTemporary = TRUE;

      ::DeleteObject(hbmp);
      hbmp = imageForScale.GetImageWell();
    }
  }

  //-------------------------------------------------------
  // Create memory source DC and select an original bitmap:
  //-------------------------------------------------------
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);

  CBitmap* hOldBitmapSrc = NULL;

  int iBitmapWidth;
  int iBitmapHeight;
  int iNewBitmapWidth;

  BITMAP bmp;

  if (::GetObject(hbmp, sizeof(BITMAP), &bmp) == 0) {
    return -1;
  }

  if (bSetBitPerPixel) {
    m_nBitsPerPixel = bmp.bmBitsPixel;
  }

  iNewBitmapWidth = bmp.bmWidth;
  iBitmapHeight = bmp.bmHeight;

  if (m_hbmImageWell != NULL) {
    //-------------------------------
    // Get original bitmap attributes:
    //-------------------------------
    if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
      return -1;
    }

    hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(m_hbmImageWell);

    if (hOldBitmapSrc == NULL) {
      return -1;
    }

    iBitmapWidth = bmp.bmWidth;
    iBitmapHeight = bmp.bmHeight;
  }
  else {
    iBitmapWidth = 0;

    hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(hbmp);

    if (hOldBitmapSrc == NULL) {
      return -1;
    }
  }

  //----------------------------------------------------------
  // Create a new bitmap compatible with the source memory DC
  // (original bitmap SHOULD BE ALREADY SELECTED!):
  //----------------------------------------------------------
  CBitmap* hNewBitmap = (CBitmap*) ::CreateCompatibleBitmap(memDCSrc,
      iBitmapWidth + iNewBitmapWidth,
      iBitmapHeight);

  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return -1;
  }

  //------------------------------------------------------
  // Create memory destination DC and select a new bitmap:
  //------------------------------------------------------
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);

  CBitmap* hOldBitmapDst = (CBitmap*) memDCDst.SelectObject(hNewBitmap);

  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    return -1;
  }

  if (m_hbmImageWell != NULL) {
    //-----------------------------
    // Copy original bitmap to new:
    //-----------------------------
    memDCDst.BitBlt(0, 0, iBitmapWidth, iBitmapHeight,
        &memDCSrc, 0, 0, SRCCOPY);
  }

  //--------------------------------
  // Select a new image and copy it:
  //--------------------------------
  if (memDCSrc.SelectObject(hbmp) == NULL) {
    memDCDst.SelectObject(hOldBitmapDst);
    memDCSrc.SelectObject(hOldBitmapSrc);

    ::DeleteObject(hNewBitmap);
    return -1;
  }

  memDCDst.BitBlt(iBitmapWidth, 0, iNewBitmapWidth, iBitmapHeight,
      &memDCSrc, 0, 0, SRCCOPY);

  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);

  if (m_hbmImageWell != NULL) {
    ::DeleteObject(m_hbmImageWell);
  }

  m_hbmImageWell = hNewBitmap;
  m_bModified = TRUE;

  UpdateCount();

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;

  if (bIsMirror) {
    ::DeleteObject(hbmp);
  }

  return m_iCount - 1;
#endif
}
//********************************************************************************
int CBCGPToolBarImages::AddImage(const CBCGPToolBarImages& imageList, int nIndex)
{
  if (nIndex < 0 || nIndex >= imageList.GetCount()) {
    ASSERT(FALSE);
    return -1;
  }

  CWindowDC dc(NULL);

  if (!IsScaled()) {
    m_sizeImage = imageList.m_sizeImage;
    m_sizeImageDest = imageList.m_sizeImageDest;
    m_clrTransparent = imageList.m_clrTransparent;
    m_clrImageShadow = imageList.m_clrImageShadow;
    m_bFadeInactive = imageList.m_bFadeInactive;
    m_nBitsPerPixel = imageList.m_nBitsPerPixel;
  }

  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);

  CBitmap* hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(imageList.m_hbmImageWell);

  CDC memDCDest;
  memDCDest.CreateCompatibleDC(NULL);

  CBitmap* bitmap;
  DIBSECTION ds = {0};

  if (imageList.m_nBitsPerPixel >= 24 &&
      ::GetObject(m_hbmImageWell, sizeof(DIBSECTION), &ds) != 0) {
    BITMAPINFO bi = {0};
    bi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth       = imageList.m_sizeImage.w;
    bi.bmiHeader.biHeight      = imageList.m_sizeImage.h;
    bi.bmiHeader.biPlanes      = ds.dsBmih.biPlanes;
    bi.bmiHeader.biBitCount    = ds.dsBmih.biBitCount;
    bi.bmiHeader.biCompression = BI_RGB;

    COLORREF* pBits = NULL;
    CBitmap* hNewBitmap = ::CreateDIBSection(
        dc, &bi, DIB_RGB_COLORS, (void**)&pBits,
        NULL, NULL);
    bitmap.Attach(hNewBitmap);
  }
  else {
    bitmap.CreateCompatibleBitmap(&dc, imageList.m_sizeImage.w, imageList.m_sizeImage.h);
  }

  CBitmap* pOldBitmapDest = memDCDest.SelectObject(&bitmap);

  memDCDest.BitBlt(0, 0, imageList.m_sizeImage.w, imageList.m_sizeImage.h,
      &memDCSrc, nIndex * imageList.m_sizeImage.w, 0, SRCCOPY);

  memDCDest.SelectObject(pOldBitmapDest);
  memDCSrc.SelectObject(hOldBitmapSrc);

  return AddImage(bitmap);
}
//********************************************************************************
int CBCGPToolBarImages::AddIcon(HICON hIcon, BOOL bAlphaBlend/* = FALSE*/)
{
  CWindowDC dc(NULL);

  if (hIcon == NULL) {
    bAlphaBlend = FALSE;
  }

  CDC dcMem;
  dcMem.CreateCompatibleDC(NULL);

  CBitmap* bmpMem;

  CSize sizeIcon = m_sizeImage;

  if (IsScaled()) {
    sizeIcon = m_sizeImageOriginal;
  }

  if (bAlphaBlend) {
    BITMAPINFO bi;

    // Fill in the BITMAPINFOHEADER
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = sizeIcon.w;
    bi.bmiHeader.biHeight = sizeIcon.h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = sizeIcon.w * sizeIcon.h;
    bi.bmiHeader.biXPelsPerMeter = 0;
    bi.bmiHeader.biYPelsPerMeter = 0;
    bi.bmiHeader.biClrUsed = 0;
    bi.bmiHeader.biClrImportant = 0;

    COLORREF* pBits = NULL;
    CBitmap* hbmp = CreateDIBSection(
        dcMem.m_hDC, &bi, DIB_RGB_COLORS, (void**)&pBits,
        NULL, NULL);

    if (hbmp == NULL) {
      ASSERT(FALSE);
      return -1;
    }

    bmpMem.Attach(hbmp);
  }
  else {
    bmpMem.CreateCompatibleBitmap(&dc, sizeIcon.w, sizeIcon.h);
  }

  CBitmap* pBmpOriginal = dcMem.SelectObject(&bmpMem);

  if (!bAlphaBlend) {
    dcMem.FillRect(CRect(0, 0, sizeIcon.w, sizeIcon.h), &globalData.brBtnFace);
  }

  if (hIcon != NULL) {
    dcMem.DrawState(CPoint(0, 0), sizeIcon, hIcon, DSS_NORMAL, (CBrush*) NULL);
  }

  dcMem.SelectObject(pBmpOriginal);

  if (bAlphaBlend) {
    m_nBitsPerPixel = 32;
    PreMultiplyAlpha(bmpMem, TRUE);
  }

  return AddImage(bmpMem);
}
//*******************************************************************************
BOOL CBCGPToolBarImages::UpdateImage(int iImage, CBitmap* hbmp)
{
  if (m_bIsTemporary) {
    ASSERT(FALSE);
    return FALSE;
  }

  if (!m_bUserImagesList) { // Only user images can be edited!
    return FALSE;
  }

  CWindowDC dc(NULL);
  CBitmap*  bitmap;
  CDC     memDCSrc;
  CDC     memDCDst;

  memDCSrc.CreateCompatibleDC(&dc);
  memDCDst.CreateCompatibleDC(&dc);

  CBitmap* hOldBitmapDst = (CBitmap*) memDCDst.SelectObject(m_hbmImageWell);
  CBitmap* hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(hbmp);

  memDCDst.BitBlt(m_sizeImage.w * iImage, 0, m_sizeImage.w, m_sizeImage.h,
      &memDCSrc, 0, 0, SRCCOPY);

  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);

  m_bModified = TRUE;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;

  return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::DeleteImage(int iImage)
{
  if (m_bIsTemporary) {
    ASSERT(FALSE);
    return FALSE;
  }

  if (!m_bUserImagesList) { // Only user images can be edited!
    return FALSE;
  }

  if (iImage < 0 || iImage >= GetCount()) { // Wrong index
    return FALSE;
  }

  //-------------------------------
  // Get original bitmap attrbutes:
  //-------------------------------
  BITMAP bmp;

  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    return FALSE;
  }

  //-------------------------------------------------------
  // Create memory source DC and select an original bitmap:
  //-------------------------------------------------------
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);

  CBitmap* hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(m_hbmImageWell);

  if (hOldBitmapSrc == NULL) {
    return FALSE;
  }

  //----------------------------------------------------------
  // Create a new bitmap compatibel with the source memory DC
  // (original bitmap SHOULD BE ALREADY SELECTED!):
  //----------------------------------------------------------
  CBitmap* hNewBitmap = (CBitmap*) ::CreateCompatibleBitmap(memDCSrc,
      bmp.bmWidth - m_sizeImage.w,
      bmp.bmHeight);

  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return FALSE;
  }

  //------------------------------------------------------
  // Create memory destination DC and select a new bitmap:
  //------------------------------------------------------
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);

  CBitmap* hOldBitmapDst = (CBitmap*) memDCDst.SelectObject(hNewBitmap);

  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    return FALSE;
  }

  //-----------------------------
  // Copy original bitmap to new:
  //-----------------------------

  if (iImage != 0) {
    memDCDst.BitBlt(0, 0,
        m_sizeImage.w * iImage, bmp.bmHeight,
        &memDCSrc, 0, 0, SRCCOPY);
  }

  if (iImage != m_iCount - 1) {
    memDCDst.BitBlt(m_sizeImage.w * iImage, 0,
        (m_iCount - iImage - 1) * m_sizeImage.w, bmp.bmHeight,
        &memDCSrc,
        m_sizeImage.w * (iImage + 1), 0, SRCCOPY);
  }

  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);

  ::DeleteObject(m_hbmImageWell);

  m_hbmImageWell = hNewBitmap;
  m_bModified = TRUE;

  UpdateCount();
  UpdateInternalImage(BCGImage_Light);
  UpdateInternalImage(BCGImage_Shadow);

  return TRUE;
}
//*******************************************************************************
HICON CBCGPToolBarImages::ExtractIcon(int nIndex)
{
  if (nIndex < 0 || nIndex >= GetCount()) { // Wrong index
    return NULL;
  }

  UINT nFlags = (m_nBitsPerPixel == 32) ? 0 : ILC_MASK;

  switch (m_nBitsPerPixel) {
  case 4:
  default:
    nFlags |= ILC_COLOR4;
    break;

  case 8:
    nFlags |= ILC_COLOR8;
    break;

  case 16:
    nFlags |= ILC_COLOR16;
    break;

  case 24:
    nFlags |= ILC_COLOR24;
    break;

  case 32:
    nFlags |= ILC_COLOR32;
    break;
  }

  CImageList images;
  images.Create(m_sizeImage.w, m_sizeImage.h, nFlags, 0, 0);

  CBitmap* hbmImageWellCopy = Copy(m_hbmImageWell);

  images.Add(CBitmap*::FromHandle(hbmImageWellCopy),
      m_clrTransparent == -1 ? globalData.clrBtnFace : m_clrTransparent);

  AfxDeleteObject((HGDIOBJ*)&hbmImageWellCopy);

  return images.ExtractIcon(nIndex);
}
//************************************************************************************
BOOL CBCGPToolBarImages::Save(LPCTSTR lpszBmpFileName)
{
  if (!m_bUserImagesList || // Only user-defined bitmaps can be saved!
      m_hbmImageWell == NULL) { // Not loaded yet!
    return FALSE;
  }

  if (m_bReadOnly) {
    return FALSE;
  }

  CString strFile;

  if (lpszBmpFileName == NULL) {
    strFile = m_strUDLPath;
  }
  else {
    strFile = lpszBmpFileName;
  }

  if (!m_bModified && strFile == m_strUDLPath) {
    return TRUE;
  }

  HANDLE hDib = DDBToDIB(m_hbmImageWell, 0);

  if (hDib == NULL) {
    TRACE(_T("CBCGPToolBarImages::Save Can't convert DDB to DIB\n"));
    return FALSE;
  }

  BOOL bSuccess = WriteDIB(strFile, hDib);
  ::GlobalFree(hDib);

  if (!bSuccess) {
    return FALSE;
  }

  m_bModified = FALSE;
  return TRUE;
}
//************************************************************************************
static BOOL WriteDIB(LPCTSTR szFile, HANDLE hDIB)
{
  BITMAPFILEHEADER  hdr;
  LPBITMAPINFOHEADER  lpbi;

  if (!hDIB) {
    return FALSE;
  }

  CFile file;

  if (!file.Open(szFile, CFile::modeWrite | CFile::modeCreate)) {
    return FALSE;
  }

  lpbi = (LPBITMAPINFOHEADER) hDIB;

  int nColors = 1 << lpbi->biBitCount;

  if (nColors > 256 || lpbi->biBitCount == 32) {
    nColors = 0;
  }

  // Fill in the fields of the file header
  hdr.bfType    = ((WORD)('M' << 8) | 'B');   // is always "BM"
  hdr.bfSize    = (DWORD)(GlobalSize(hDIB) + sizeof(BITMAPFILEHEADER));
  hdr.bfReserved1   = 0;
  hdr.bfReserved2   = 0;
  hdr.bfOffBits   = (DWORD)(sizeof(hdr) + lpbi->biSize +
      nColors * sizeof(RGBQUAD));

  // Write the file header
  file.Write(&hdr, sizeof(hdr));

  // Write the DIB header and the bits
  file.Write(lpbi, (UINT) GlobalSize(hDIB));

  return TRUE;
}
//********************************************************************************
static HANDLE DDBToDIB(CBitmap* bitmap, DWORD dwCompression)
{
  BITMAP        bm;
  BITMAPINFOHEADER  bi;
  LPBITMAPINFOHEADER  lpbi;
  DWORD       dwLen;
  HANDLE        hDIB;
  HANDLE        handle;
  HDC         hDC;
  HPALETTE      hPal;

  // The function has no arg for bitfields
  if (dwCompression == BI_BITFIELDS) {
    return NULL;
  }

  hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

  // Get bitmap information
  ::GetObject(bitmap, sizeof(bm), (LPSTR)&bm);

  // Initialize the bitmapinfoheader
  bi.biSize     = sizeof(BITMAPINFOHEADER);
  bi.biWidth      = bm.bmWidth;
  bi.biHeight     = bm.bmHeight;
  bi.biPlanes     = 1;
  bi.biBitCount   = (WORD)(bm.bmPlanes * bm.bmBitsPixel);
  bi.biCompression  = dwCompression;
  bi.biSizeImage    = 0;
  bi.biXPelsPerMeter  = 0;
  bi.biYPelsPerMeter  = 0;
  bi.biClrUsed    = 0;
  bi.biClrImportant = 0;

  // Compute the size of the  infoheader and the color table
  int nColors = (1 << bi.biBitCount);

  if (nColors > 256 || bi.biBitCount == 32) {
    nColors = 0;
  }

  dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

  // We need a device context to get the DIB from
  hDC = ::CreateCompatibleDC(NULL);

  if (hDC == NULL) {
    return FALSE;
  }

  CBitmap* bmp = ::CreateBitmap(1, 1, 1, bi.biBitCount, NULL);

  if (bmp == NULL) {
    ::DeleteDC(hDC);
    return NULL;
  }

  CBitmap* hOldBitmap = (CBitmap*)::SelectObject(hDC, bmp);

  hPal = SelectPalette(hDC, hPal, FALSE);
  RealizePalette(hDC);

  // Allocate enough memory to hold bitmapinfoheader and color table
  hDIB = GlobalAlloc(GMEM_FIXED, dwLen);

  if (!hDIB) {
    ::SelectPalette(hDC, hPal, FALSE);

    if (hOldBitmap != NULL) {
      ::SelectObject(hDC, hOldBitmap);
    }

    ::DeleteObject(bmp);
    ::DeleteDC(hDC);
    return NULL;
  }

  lpbi = (LPBITMAPINFOHEADER)hDIB;

  *lpbi = bi;

  // Call GetDIBits with a NULL lpBits param, so the device driver
  // will calculate the biSizeImage field
  GetDIBits(hDC, bitmap, 0L, (DWORD)bi.biHeight,
      (LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

  bi = *lpbi;

  // If the driver did not fill in the biSizeImage field, then compute it
  // Each scan line of the image is aligned on a DWORD (32bit) boundary
  if (bi.biSizeImage == 0) {
    bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
        * bi.biHeight;

    // If a compression scheme is used the result may infact be larger
    // Increase the size to account for this.
    if (dwCompression != BI_RGB) {
      bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }
  }

  // Realloc the buffer so that it can hold all the bits
  dwLen += bi.biSizeImage;
  handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE);

  if (handle != NULL) {
    hDIB = handle;
  }
  else {
    GlobalFree(hDIB);

    // Reselect the original palette
    SelectPalette(hDC, hPal, FALSE);

    if (hOldBitmap != NULL) {
      ::SelectObject(hDC, hOldBitmap);
    }

    ::DeleteObject(bmp);
    ::DeleteDC(hDC);
    return NULL;
  }

  // Get the bitmap bits
  lpbi = (LPBITMAPINFOHEADER)hDIB;

  // FINALLY get the DIB
  BOOL bGotBits = GetDIBits(hDC, bitmap,
      0L,       // Start scan line
      (DWORD)bi.biHeight,   // # of scan lines
      (LPBYTE)lpbi      // address for bitmap bits
      + (bi.biSize + nColors * sizeof(RGBQUAD)),
      (LPBITMAPINFO)lpbi,   // address of bitmapinfo
      (DWORD)DIB_RGB_COLORS);   // Use RGB for color table

  if (!bGotBits) {
    GlobalFree(hDIB);

    SelectPalette(hDC, hPal, FALSE);

    if (hOldBitmap != NULL) {
      ::SelectObject(hDC, hOldBitmap);
    }

    ::DeleteObject(bmp);
    ::DeleteDC(hDC);
    return NULL;
  }

  // Convert color table to the standard 3-d colors:
  DWORD* pColorTable = (DWORD*)(((LPBYTE)lpbi) + (UINT) lpbi->biSize);

  for (int iColor = 0; iColor < nColors; iColor ++) {
    pColorTable[iColor] = CBCGPToolBarImages::MapFromSysColor(pColorTable[iColor]);
  }

  SelectPalette(hDC, hPal, FALSE);

  if (hOldBitmap != NULL) {
    ::SelectObject(hDC, hOldBitmap);
  }

  ::DeleteObject(bmp);
  ::DeleteDC(hDC);

  return hDIB;
}


/////////////////////////////////////////////////////////////////////////////
// CBCGPToolBarImages diagnostics

BOOL CBCGPToolBarImages::CopyImageToClipboard(int iImage)
{
  CBCGPLocalResource locaRes;

  try {
    CWindowDC dc(NULL);

    //----------------------
    // Create a bitmap copy:
    //----------------------
    CDC memDCDest;
    memDCDest.CreateCompatibleDC(NULL);

    CBitmap* bitmapCopy;

    if (!bitmapCopy.CreateCompatibleBitmap(&dc, m_sizeImage.w, m_sizeImage.h)) {
      AfxMessageBox(IDP_BCGBARRES_CANT_COPY_BITMAP);
      return FALSE;
    }

    CBitmap* pOldBitmapDest = memDCDest.SelectObject(&bitmapCopy);

    memDCDest.FillRect(CRect(0, 0, m_sizeImage.w, m_sizeImage.h),
        &globalData.brBtnFace);

    CBCGPDrawState ds;
    PrepareDrawImage(ds, FALSE);

    Draw(&memDCDest, 0, 0, iImage);

    memDCDest.SelectObject(pOldBitmapDest);

    if (!AfxGetMainWnd()->OpenClipboard()) {
      AfxMessageBox(IDP_BCGBARRES_CANT_COPY_BITMAP);
      return FALSE;
    }

    if (!::EmptyClipboard()) {
      AfxMessageBox(IDP_BCGBARRES_CANT_COPY_BITMAP);
      ::CloseClipboard();
      return FALSE;
    }


    HANDLE hclipData = ::SetClipboardData(CF_BITMAP, bitmapCopy.Detach());

    if (hclipData == NULL) {
      AfxMessageBox(IDP_BCGBARRES_CANT_COPY_BITMAP);
      TRACE(_T("CBCGToolBar::OnBcgbarresCopyImage error. Error code = %x\n"), GetLastError());
    }

    ::CloseClipboard();
    return TRUE;
  }
  catch (...) {
    AfxMessageBox(IDP_BCGBARRES_INTERLAL_ERROR);
  }

  return FALSE;
}
//****************************************************************************************
BOOL CBCGPToolBarImages::CopyTo(CBCGPToolBarImages& dest)
{
  if (dest.m_bIsTemporary) {
    ASSERT(FALSE);
    return FALSE;
  }

  if (dest.IsValid()) {
    dest.Clear();
  }

  if (globalData.bIsWindowsVista) {
    BITMAP bmp;

    if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == sizeof(BITMAP)) {
      CSize sizeImage(bmp.bmWidth, bmp.bmHeight);

      //-------------------------------------------------------
      // Create memory source DC and select an original bitmap:
      //-------------------------------------------------------
      CDC memDCSrc;
      memDCSrc.CreateCompatibleDC(NULL);

      CBitmap* hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(m_hbmImageWell);

      if (hOldBitmapSrc != NULL) {

        //----------------------------------------------------------
        // Create a new bitmap compatible with the source memory DC
        // (original bitmap SHOULD BE ALREADY SELECTED!):
        //----------------------------------------------------------

        CBitmap* hNewBitmap = NULL;

        DIBSECTION ds = {0};

        if (bmp.bmBitsPixel >= 24 &&
            ::GetObject(m_hbmImageWell, sizeof(DIBSECTION), &ds) != 0) {
          BITMAPINFO bi = {0};
          bi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
          bi.bmiHeader.biWidth       = sizeImage.w;
          bi.bmiHeader.biHeight      = sizeImage.h;
          bi.bmiHeader.biPlanes      = bmp.bmPlanes;
          bi.bmiHeader.biBitCount    = bmp.bmBitsPixel;
          bi.bmiHeader.biCompression = BI_RGB;

          COLORREF* pBits = NULL;
          hNewBitmap = ::CreateDIBSection(
              memDCSrc, &bi, DIB_RGB_COLORS, (void**)&pBits,
              NULL, NULL);
        }
        else {
          hNewBitmap = (CBitmap*) ::CreateCompatibleBitmap(memDCSrc,
              sizeImage.w, sizeImage.h);
        }

        if (hNewBitmap != NULL) {
          //------------------------------------------------------
          // Create memory destination DC and select a new bitmap:
          //------------------------------------------------------
          CDC memDCDst;
          memDCDst.CreateCompatibleDC(&memDCSrc);

          CBitmap* hOldBitmapDst = (CBitmap*) memDCDst.SelectObject(hNewBitmap);

          if (hOldBitmapDst != NULL) {
            //-----------------------------
            // Copy original bitmap to new:
            //-----------------------------
            memDCDst.BitBlt(0, 0, sizeImage.w, sizeImage.h,
                &memDCSrc, 0, 0, SRCCOPY);

            memDCDst.SelectObject(hOldBitmapDst);

            dest.m_hbmImageWell = hNewBitmap;
          }
          else {
            ::DeleteObject(hNewBitmap);
          }
        }

        memDCSrc.SelectObject(hOldBitmapSrc);
      }
    }
  }
  else {
    dest.m_hbmImageWell = (CBitmap*) ::CopyImage(m_hbmImageWell, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    if (m_hbmImageLight != NULL) {
      dest.m_hbmImageLight = (CBitmap*) ::CopyImage(m_hbmImageLight, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    }

    if (m_hbmImageShadow != NULL) {
      dest.m_hbmImageShadow = (CBitmap*) ::CopyImage(m_hbmImageShadow, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    }
  }

  dest.m_sizeImage = m_sizeImage;
  dest.m_sizeImageDest = m_sizeImageDest;
  dest.m_bUserImagesList = m_bUserImagesList;
  dest.m_strUDLPath = m_strUDLPath;
  dest.m_bModified = m_bModified;
  dest.m_iCount = m_iCount;
  dest.m_clrTransparent = m_clrTransparent;
  dest.m_bReadOnly = m_bReadOnly;
  dest.m_clrImageShadow = m_clrImageShadow;
  dest.m_bFadeInactive = m_bFadeInactive;
  dest.m_nBitsPerPixel = m_nBitsPerPixel;
  dest.m_dblScale = m_dblScale;

  for (POSITION pos = m_lstOrigResIds.GetHeadPosition(); pos != NULL;) {
    UINT uiResId = m_lstOrigResIds.GetNext(pos);

    dest.m_lstOrigResIds.AddTail(uiResId);

    int iOffset = -1;

    if (m_mapOrigResOffsets.Lookup(uiResId, iOffset)) {
      dest.m_mapOrigResOffsets.SetAt(uiResId, iOffset);
    }
  }

  for (POSITION posInst = m_lstOrigResInstances.GetHeadPosition(); posInst != NULL;) {
    HINSTANCE hInst = m_lstOrigResInstances.GetNext(posInst);
    dest.m_lstOrigResInstances.AddTail(hInst);
  }

  return TRUE;
}
//****************************************************************************************
void CBCGPToolBarImages::Clear()
{
  if (m_bIsTemporary) {
    ASSERT(FALSE);
    return;
  }

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);
  m_hbmImageWell = NULL;
  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;
  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;

  m_lstOrigResIds.RemoveAll();
  m_mapOrigResOffsets.RemoveAll();
  m_lstOrigResInstances.RemoveAll();
  m_strUDLPath.Empty();

  m_bUserImagesList = FALSE;
  m_iCount = 0;
  m_bModified = FALSE;
  m_bIsGray = FALSE;
  m_nGrayPercentage = 0;
  m_nBitsPerPixel = 0;

  if (m_dblScale != 1.0) {
    m_sizeImage = m_sizeImageOriginal;
    m_sizeImageOriginal = CSize(0, 0);
    m_dblScale = 1.0;
  }
}
//*************************************************************************************
void CBCGPToolBarImages::TransparentBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth,
    int nHeight, CDC* pDcSrc, int nXSrc, int nYSrc,
    COLORREF colorTransparent,
    int nWidthDest/* = -1*/, int nHeightDest/* = -1*/)
{
  int cx = nWidthDest == -1 ? nWidth : nWidthDest;
  int cy = nHeightDest == -1 ? nHeight : nHeightDest;

  if (m_pfTransparentBlt != NULL && !m_bIsRTL) {
    if ((*m_pfTransparentBlt)(hdcDest, nXDest, nYDest, cx, cy,
        pDcSrc->GetSafeHdc(), nXSrc, nYSrc,
        nWidth, nHeight, colorTransparent)) {
      return;
    }
  }

  CDC dc, memDC, maskDC;
  dc.Attach(hdcDest);
  maskDC.CreateCompatibleDC(&dc);
  CBitmap* maskBitmap;

  //add these to store return of SelectObject() calls
  CBitmap* pOldMemBmp = NULL;
  CBitmap* pOldMaskBmp = NULL;

  memDC.CreateCompatibleDC(&dc);
  CBitmap* bmpImage;
  bmpImage.CreateCompatibleBitmap(&dc, cx, cy);
  pOldMemBmp = memDC.SelectObject(&bmpImage);

  if (nWidthDest == -1 || (nWidthDest == nWidth && nHeightDest == nHeight)) {
    memDC.BitBlt(0, 0, nWidth, nHeight, pDcSrc, nXSrc, nYSrc, SRCCOPY);
  }
  else {
    memDC.StretchBlt(0, 0, nWidthDest, nHeightDest, pDcSrc,
        nXSrc, nYSrc, nWidth, nHeight, SRCCOPY);
  }

  // Create monochrome bitmap for the mask
  maskBitmap.CreateBitmap(cx, cy, 1, 1, NULL);
  pOldMaskBmp = maskDC.SelectObject(&maskBitmap);
  memDC.SetBkColor(colorTransparent);

  // Create the mask from the memory DC
  maskDC.BitBlt(0, 0, cx, cy, &memDC, 0, 0, SRCCOPY);

  // Set the background in memDC to black. Using SRCPAINT with black
  // and any other color results in the other color, thus making
  // black the transparent color
  memDC.SetBkColor(RGB(0, 0, 0));
  memDC.SetTextColor(RGB(255, 255, 255));
  memDC.BitBlt(0, 0, cx, cy, &maskDC, 0, 0, SRCAND);

  // Set the foreground to black. See comment above.
  dc.SetBkColor(RGB(255, 255, 255));
  dc.SetTextColor(RGB(0, 0, 0));

  dc.BitBlt(nXDest, nYDest, cx, cy, &maskDC, 0, 0, SRCAND);

  // Combine the foreground with the background
  dc.BitBlt(nXDest, nYDest, cx, cy, &memDC,
      0, 0, SRCPAINT);

  if (pOldMaskBmp) {
    maskDC.SelectObject(pOldMaskBmp);
  }

  if (pOldMemBmp) {
    memDC.SelectObject(pOldMemBmp);
  }

  dc.Detach();
}


//**********************************************************************************
BOOL CBCGPToolBarImages::MapTo3dColors(BOOL bUseRGBQUAD/* = TRUE*/,
    COLORREF clrSrc/* = (COLORREF)-1*/,
    COLORREF clrDest/* = (COLORREF)-1*/)
{
  return MapBmpTo3dColors(m_hbmImageWell, bUseRGBQUAD, clrSrc, clrDest);
}
//*******************************************************************************
void CBCGPToolBarImages::CopyTemp(CBCGPToolBarImages& imagesDest)
{
  imagesDest.Clear();
  imagesDest.m_bIsTemporary = TRUE;

  imagesDest.m_sizeImage = m_sizeImage;
  imagesDest.m_sizeImageDest = m_sizeImageDest;
  imagesDest.m_hbmImageWell = m_hbmImageWell;
  imagesDest.m_bUserImagesList = m_bUserImagesList;
  imagesDest.m_iCount = m_iCount;
  imagesDest.m_bReadOnly = TRUE;
  imagesDest.m_nBitsPerPixel = m_nBitsPerPixel;
}
//*******************************************************************************
#endif
BOOL CBCGPToolBarImages::UpdateInternalImage(int nIndex)
{
#if 0
  CBitmap*& hbmpInternal = (nIndex == BCGImage_Light) ?
      m_hbmImageLight : m_hbmImageShadow;

  if (nIndex == BCGImage_Light) {
    if ((m_nBitsPerPixel > 4 && !m_bAlwaysLight) || m_nBitsPerPixel == 0) {
      // Down't fade 256+ or unknown bitmaps
      return FALSE;
    }
  }

  AfxDeleteObject((HGDIOBJ*)&hbmpInternal);
  hbmpInternal = NULL;

  if (m_hbmImageWell == NULL) {
    return TRUE;
  }

  OSVERSIONINFO osvi;
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  ::GetVersionEx(&osvi);

  if (globalData.m_nBitsPerPixel <= 8 ||
      osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) {
    return TRUE;
  }

  //-------------------------------------------------------
  // Create memory source DC and select an original bitmap:
  //-------------------------------------------------------
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);

  BITMAP bmp;

  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    return FALSE;
  }

  int iBitmapWidth = bmp.bmWidth;
  int iBitmapHeight = bmp.bmHeight;

  CBitmap* hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(m_hbmImageWell);

  if (hOldBitmapSrc == NULL) {
    return FALSE;
  }

  //------------------------------------------------------
  // Create memory destination DC and select a new bitmap:
  //------------------------------------------------------
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);

  BITMAPINFO bi;

  // Fill in the BITMAPINFOHEADER
  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bi.bmiHeader.biWidth = iBitmapWidth;
  bi.bmiHeader.biHeight = iBitmapHeight;
  bi.bmiHeader.biPlanes = 1;
  bi.bmiHeader.biBitCount = 32;
  bi.bmiHeader.biCompression = BI_RGB;
  bi.bmiHeader.biSizeImage = iBitmapWidth * iBitmapHeight;
  bi.bmiHeader.biXPelsPerMeter = 0;
  bi.bmiHeader.biYPelsPerMeter = 0;
  bi.bmiHeader.biClrUsed = 0;
  bi.bmiHeader.biClrImportant = 0;

  COLORREF* pBits = NULL;
  hbmpInternal = CreateDIBSection(
      memDCDst.m_hDC, &bi, DIB_RGB_COLORS, (void**)&pBits,
      NULL, NULL);

  if (hbmpInternal == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return FALSE;
  }

  CBitmap* hOldBitmapDst = (CBitmap*) memDCDst.SelectObject(hbmpInternal);

  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hbmpInternal);
    hbmpInternal = NULL;
    return FALSE;
  }

  //-----------------------------
  // Copy original bitmap to new:
  //-----------------------------
  COLORREF clrTransparent = m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL ?
      (COLORREF) - 1 : m_clrTransparent;

  memDCDst.BitBlt(0, 0, iBitmapWidth, iBitmapHeight,
      &memDCSrc, 0, 0, SRCCOPY);

  if (nIndex == BCGImage_Light) {
    CBCGPDrawManager dm(memDCDst);

    dm.HighlightRect(CRect(0, 0, iBitmapWidth, iBitmapHeight),
        m_nLightPercentage,
        clrTransparent == -1 ? globalData.clrBtnFace : clrTransparent);
  }
  else {
    COLORREF clrTr =
        clrTransparent == -1 ? globalData.clrBtnFace : clrTransparent;

    COLORREF clrHL = CBCGPVisualManager::GetInstance()->GetToolbarHighlightColor();
    COLORREF clrShadow =
        globalData.m_nBitsPerPixel <= 8 ?
        globalData.clrBtnShadow :
        CBCGPDrawManager::PixelAlpha(clrHL, 67);

    for (int x = 0; x < iBitmapWidth; x++) {
      for (int y = 0; y < iBitmapHeight; y++) {
        COLORREF clr = memDCDst.GetPixel(x, y);

        if (clr != clrTr) {
          memDCDst.SetPixel(x, y, clrShadow);
        }
      }
    }
  }

  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);

#endif
  return TRUE;
}

#if 0
//*******************************************************************************
BOOL CBCGPToolBarImages::PreMultiplyAlpha(CBitmap* hbmp, BOOL bAutoCheckPremlt)
{
  DIBSECTION ds;

  if (::GetObject(hbmp, sizeof(DIBSECTION), &ds) == 0) {
    ASSERT(FALSE);
    return FALSE;
  }

  if (ds.dsBm.bmBitsPixel != 32) {
    return FALSE;
  }

  int i = 0;

  RGBQUAD* pBits = (RGBQUAD*) ds.dsBm.bmBits;
  const int length = ds.dsBm.bmWidth * ds.dsBm.bmHeight;

  if (bAutoCheckPremlt) {
    BOOL bPremultiply = FALSE;

    RGBQUAD* pBit = pBits;

    for (i = 0; i < length; i++) {
      if (pBit->rgbRed   > pBit->rgbReserved ||
          pBit->rgbGreen > pBit->rgbReserved ||
          pBit->rgbBlue  > pBit->rgbReserved) {
        bPremultiply = TRUE;
        break;
      }

      pBit++;
    }

    if (!bPremultiply) {
      return TRUE;
    }
  }

  //----------------------------------------------------------------
  // Premultiply the R,G and B values with the Alpha channel values:
  //----------------------------------------------------------------
  RGBQUAD* pBit = pBits;

  for (i = 0; i < length; i++) {
    pBit->rgbRed   = (BYTE)(pBit->rgbRed   * pBit->rgbReserved / 255);
    pBit->rgbGreen = (BYTE)(pBit->rgbGreen * pBit->rgbReserved / 255);
    pBit->rgbBlue  = (BYTE)(pBit->rgbBlue  * pBit->rgbReserved / 255);
    pBit++;
  }

  return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::PreMultiplyAlpha(CBitmap* hbmp)
{
  return PreMultiplyAlpha(hbmp, m_bAutoCheckPremlt);
}
//*******************************************************************************
BOOL CBCGPToolBarImages::CreateFromImageList(const CImageList& imageList)
{
  ASSERT(imageList.GetSafeHandle() != NULL);
  ASSERT(imageList.GetImageCount() > 0);

  Clear();

  IMAGEINFO info;
  imageList.GetImageInfo(0, &info);

  CRect rectImage = info.rcImage;
  m_sizeImage = rectImage.Size();

  for (int i = 0; i < imageList.GetImageCount(); i++) {
    HICON hIcon = ((CImageList&) imageList).ExtractIcon(i);
    ASSERT(hIcon != NULL);

    AddIcon(hIcon);

    ::DestroyIcon(hIcon);
  }

  return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::Is32BitTransparencySupported()
{
  return globalData.bIsOSAlphaBlendingSupport;
}
//********************************************************************************
BOOL CBCGPToolBarImages::GrayImages(int nGrayPercentage)
{
  m_bIsGray = TRUE;
  m_nGrayPercentage = nGrayPercentage;

  if (m_hbmImageWell == NULL) {
    return TRUE;
  }

  OSVERSIONINFO osvi;
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  ::GetVersionEx(&osvi);

  if (globalData.m_nBitsPerPixel <= 8 ||
      osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) {
    return TRUE;
  }

  //-------------------------------------------------------
  // Create memory source DC and select an original bitmap:
  //-------------------------------------------------------
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);

  BITMAP bmp;

  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    return FALSE;
  }

  int iBitmapWidth = bmp.bmWidth;
  int iBitmapHeight = bmp.bmHeight;

  CBitmap* hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(m_hbmImageWell);

  if (hOldBitmapSrc == NULL) {
    return FALSE;
  }

  //------------------------------------------------------
  // Create memory destination DC and select a new bitmap:
  //------------------------------------------------------
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);

  BITMAPINFO bi;

  // Fill in the BITMAPINFOHEADER
  bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bi.bmiHeader.biWidth = iBitmapWidth;
  bi.bmiHeader.biHeight = iBitmapHeight;
  bi.bmiHeader.biPlanes = 1;
  bi.bmiHeader.biBitCount = 32;
  bi.bmiHeader.biCompression = BI_RGB;
  bi.bmiHeader.biSizeImage = iBitmapWidth * iBitmapHeight;
  bi.bmiHeader.biXPelsPerMeter = 0;
  bi.bmiHeader.biYPelsPerMeter = 0;
  bi.bmiHeader.biClrUsed = 0;
  bi.bmiHeader.biClrImportant = 0;

  COLORREF* pBits = NULL;
  CBitmap* hNewBitmap = CreateDIBSection(
      memDCDst.m_hDC, &bi, DIB_RGB_COLORS, (void**)&pBits,
      NULL, NULL);

  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return FALSE;
  }

  CBitmap* hOldBitmapDst = (CBitmap*) memDCDst.SelectObject(hNewBitmap);

  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    hNewBitmap = NULL;
    return FALSE;
  }

  //-----------------------------
  // Copy original bitmap to new:
  //-----------------------------
  memDCDst.BitBlt(0, 0, iBitmapWidth, iBitmapHeight,
      &memDCSrc, 0, 0, SRCCOPY);

  int nPercentage = m_nGrayPercentage <= 0 ? 130 : m_nGrayPercentage;

  if (m_nBitsPerPixel == 32 && m_pfAlphaBlend != NULL) {
    DIBSECTION ds;

    if (::GetObject(hNewBitmap, sizeof(DIBSECTION), &ds) == 0) {
      ASSERT(FALSE);
      return FALSE;
    }

    if (ds.dsBm.bmBitsPixel != 32) {
      ASSERT(FALSE);
      return FALSE;
    }

    RGBQUAD* pBits32 = (RGBQUAD*) ds.dsBm.bmBits;

    //----------------------------------------------------------------
    // Premultiply the R,G and B values with the Alpha channel values:
    //----------------------------------------------------------------
    for (int i = 0; i < ds.dsBm.bmWidth * ds.dsBm.bmHeight; i++) {
      RGBQUAD* pBit = pBits32 + i;

      double H, S, L;
      CBCGPDrawManager::RGBtoHSL(RGB(pBit->rgbRed, pBit->rgbGreen, pBit->rgbBlue), &H, &S, &L);
      COLORREF color = CBCGPDrawManager::PixelAlpha(
          CBCGPDrawManager::HLStoRGB_ONE(H, L, 0),
          .01 * nPercentage, .01 * nPercentage, .01 * nPercentage);

      pBit->rgbRed = (BYTE)(GetRValue(color) * pBit->rgbReserved / 255);
      pBit->rgbGreen = (BYTE)(GetGValue(color) * pBit->rgbReserved / 255);
      pBit->rgbBlue = (BYTE)(GetBValue(color) * pBit->rgbReserved / 255);
    }
  }
  else {
    CBCGPDrawManager dm(memDCDst);

    dm.GrayRect(CRect(0, 0, iBitmapWidth, iBitmapHeight),
        nPercentage,
        m_clrTransparent == -1 ? globalData.clrBtnFace : m_clrTransparent);
  }

  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);

  ::DeleteObject(m_hbmImageWell);
  m_hbmImageWell = hNewBitmap;

  return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::MirrorBitmap(CBitmap*& hbmp, int cxImage)
{
  if (hbmp == NULL) {
    return TRUE;
  }

  BITMAP bmp;

  if (::GetObject(hbmp, sizeof(BITMAP), &bmp) == 0) {
    return FALSE;
  }

  const int cx = bmp.bmWidth;
  const int cy = bmp.bmHeight;
  const int iCount = cx / cxImage;

  if (bmp.bmBitsPixel == 32) {
    DIBSECTION ds;

    if (::GetObject(hbmp, sizeof(DIBSECTION), &ds) == 0) {
      ASSERT(FALSE);
      return FALSE;
    }

    if (ds.dsBm.bmBitsPixel != 32) {
      ASSERT(FALSE);
      return FALSE;
    }

    DWORD* pBits = (DWORD*) ds.dsBm.bmBits;

    for (int iImage = 0; iImage < iCount; iImage++) {
      for (int y = 0; y < cy; y++) {
        DWORD* pRow1 = pBits + cx * y + iImage * cxImage;
        DWORD* pRow2 = pRow1 + cxImage - 1;

        for (int x = 0; x < cxImage / 2; x++) {
          DWORD color = *pRow1;

          *pRow1 = *pRow2;
          *pRow2 = color;

          pRow1++;
          pRow2--;
        }
      }
    }

    return TRUE;
  }

  CDC memDC;
  memDC.CreateCompatibleDC(NULL);

  CBitmap* hOldBitmap = (CBitmap*) memDC.SelectObject(hbmp);

  if (hOldBitmap == NULL) {
    return FALSE;
  }

  for (int iImage = 0; iImage < iCount; iImage++) {
    for (int y = 0; y < cy; y++) {
      int x1 = iImage * cxImage;
      int x2 = x1 + cxImage - 1;

      for (int x = 0; x < cxImage / 2; x++) {
        COLORREF color = memDC.GetPixel(x1, y);

        memDC.SetPixel(x1, y, memDC.GetPixel(x2, y));
        memDC.SetPixel(x2, y, color);

        x1++;
        x2--;
      }
    }
  }

  memDC.SelectObject(hOldBitmap);

  return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::Mirror()
{
  if (!MirrorBitmap(m_hbmImageWell, m_sizeImage.w)) {
    return FALSE;
  }

  if (m_hbmImageLight != NULL) {
    MirrorBitmap(m_hbmImageLight, m_sizeImage.w);
  }

  if (m_hbmImageShadow != NULL) {
    MirrorBitmap(m_hbmImageShadow, m_sizeImage.w);
  }

  return TRUE;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::MirrorVert()
{
  if (!MirrorBitmapVert(m_hbmImageWell, m_sizeImage.h)) {
    return FALSE;
  }

  if (m_hbmImageLight != NULL) {
    MirrorBitmapVert(m_hbmImageLight, m_sizeImage.h);
  }

  if (m_hbmImageShadow != NULL) {
    MirrorBitmapVert(m_hbmImageShadow, m_sizeImage.h);
  }

  return TRUE;
}
//****************************************************************************
void CBCGPToolBarImages::EnableRTL(BOOL bIsRTL/* = TRUE*/)
{
  m_bIsRTL = bIsRTL;
}
//****************************************************************************
void CBCGPToolBarImages::AddaptColors(COLORREF clrBase, COLORREF clrTone)
{
  double dSrcH, dSrcS, dSrcL;
  CBCGPDrawManager::RGBtoHSL(clrBase, &dSrcH, &dSrcS, &dSrcL);

  double dDestH, dDestS, dDestL;
  CBCGPDrawManager::RGBtoHSL(clrTone, &dDestH, &dDestS, &dDestL);

  double DH = dDestH - dSrcH;
  double DL = dDestL - dSrcL;
  double DS = dDestS - dSrcS;

  if (m_nBitsPerPixel >= 24) {
    DIBSECTION ds;

    if (::GetObject(m_hbmImageWell, sizeof(DIBSECTION), &ds) == 0) {
      ASSERT(FALSE);
      return;
    }

    if (ds.dsBm.bmBitsPixel < 24) {
      ASSERT(FALSE);
      return;
    }

    if (ds.dsBm.bmBitsPixel == 24) {
      COLORREF clrTransparent =
          m_clrTransparent == (COLORREF) - 1 ? globalData.clrBtnFace : m_clrTransparent;

      DWORD dwPitch = ((ds.dsBm.bmWidth * 24 + 31) & ~31) / 8;
      LPBYTE pBits = (LPBYTE)ds.dsBm.bmBits;

      for (int y = 0; y < ds.dsBm.bmHeight; y++) {
        LPBYTE pBitsRow = pBits;

        for (int x = 0; x < ds.dsBm.bmWidth; x++) {
          COLORREF clrOrig = RGB(pBitsRow[2], pBitsRow[1], pBitsRow[0]);

          if (clrOrig == clrTransparent) {
            pBitsRow += 3;
            continue;
          }

          double H, S, L;
          CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);

          if (fabs(dSrcH - H) < m_dblColorTolerance) {
            double HNew = max(0., min(3.59, H + DH));
            double SNew = max(0., min(1.00, S + DS));
            double LNew = max(0., min(1.00, L + DL));

            COLORREF color = CBCGPDrawManager::HLStoRGB_ONE(
                HNew, dDestH > 0.5 ? L : LNew, SNew);

            pBitsRow[2] = (BYTE)(GetRValue(color));
            pBitsRow[1] = (BYTE)(GetGValue(color));
            pBitsRow[0] = (BYTE)(GetBValue(color));
          }

          pBitsRow += 3;
        }

        pBits += dwPitch;
      }
    }
    else {
      RGBQUAD* pBits = (RGBQUAD*) ds.dsBm.bmBits;

      for (int i = 0; i < ds.dsBm.bmWidth * ds.dsBm.bmHeight; i++) {
        RGBQUAD* pBit = pBits + i;

        if (pBit->rgbReserved == 0) {
          continue;
        }

        double a  = (double)pBit->rgbReserved / 255.0;
        double aR = 255.0 / (double)pBit->rgbReserved;
        COLORREF clrOrig = RGB(pBit->rgbRed * aR, pBit->rgbGreen * aR, pBit->rgbBlue * aR);

        double H, S, L;
        CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);

        if (fabs(dSrcH - H) < m_dblColorTolerance) {
          double HNew = max(0., min(3.59, H + DH));
          double SNew = max(0., min(1.00, S + DS));
          double LNew = max(0., min(1.00, L + DL));

          COLORREF color = CBCGPDrawManager::HLStoRGB_ONE(
              HNew, dDestH > 0.5 ? L : LNew, SNew);

          pBit->rgbRed   = (BYTE)(GetRValue(color) * a);
          pBit->rgbGreen = (BYTE)(GetGValue(color) * a);
          pBit->rgbBlue  = (BYTE)(GetBValue(color) * a);
        }
      }
    }

    return;
  }

  //-------------------------------------------------------
  // Create memory source DC and select an original bitmap:
  //-------------------------------------------------------
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);

  CBitmap* hOldBitmapSrc = NULL;

  int iBitmapWidth;
  int iBitmapHeight;

  //-------------------------------
  // Get original bitmap attrbutes:
  //-------------------------------
  BITMAP bmp;

  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    return;
  }

  hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(m_hbmImageWell);

  if (hOldBitmapSrc == NULL) {
    return;
  }

  iBitmapWidth = bmp.bmWidth;
  iBitmapHeight = bmp.bmHeight;

  //----------------------------------------------------------
  // Create a new bitmap compatibel with the source memory DC:
  // (original bitmap SHOULD BE ALREADY SELECTED!):
  //----------------------------------------------------------
  CBitmap* hNewBitmap = (CBitmap*) ::CreateCompatibleBitmap(memDCSrc,
      iBitmapWidth,
      iBitmapHeight);

  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return;
  }

  //------------------------------
  // Create memory destination DC:
  //------------------------------
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);

  CBitmap* hOldBitmapDst = (CBitmap*) memDCDst.SelectObject(hNewBitmap);

  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    return;
  }

  //-----------------------------
  // Copy original bitmap to new:
  //-----------------------------
  memDCDst.BitBlt(0, 0, iBitmapWidth, iBitmapHeight,
      &memDCSrc, 0, 0, SRCCOPY);

  COLORREF clrTransparent =
      m_clrTransparent == (COLORREF) - 1 ? globalData.clrBtnFace : m_clrTransparent;

  for (int x = 0; x < iBitmapWidth; x ++) {
    for (int y = 0; y < iBitmapHeight; y ++) {
      COLORREF clrOrig = ::GetPixel(memDCDst, x, y);

      if (clrOrig == clrTransparent) {
        continue;
      }

      double H, L, S;
      CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);

      if (fabs(dSrcH - H) < m_dblColorTolerance) {
        double HNew = max(0., min(3.59, H + DH));
        double SNew = max(0., min(1.00, S + DS));
        double LNew = max(0., min(1.00, L + DL));

        COLORREF clrNew = CBCGPDrawManager::HLStoRGB_ONE(
            HNew, dDestH > 0.5 ? L : LNew, SNew);

        if (clrOrig != clrNew) {
          ::SetPixel(memDCDst, x, y, clrNew);
        }
      }
    }
  }

  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);

  ::DeleteObject(m_hbmImageWell);
  m_hbmImageWell = hNewBitmap;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;
}

//*****************************************************************************
#define RGB_MID(r, g, b) (BYTE)((r + g + b) / 3)
#define RGB_MID_COLOR(clr) RGB_MID(GetRValue (clr), GetGValue (clr), GetBValue (clr))
#define RGB_LUM(r, g, b) (BYTE)(((DWORD)r * 299 + (DWORD)g * 587 + (DWORD)b * 114) / 1000)
#define RGB_LUM_COLOR(clr) RGB_LUM(GetRValue (clr), GetGValue (clr), GetBValue (clr))

void CBCGPToolBarImages::AddaptColors(COLORREF clrBase, COLORREF clrTone, double dOpacity/* = 1.0*/)
{
  double dH, dS, dL;
  CBCGPDrawManager::RGBtoHSL(clrTone, &dH, &dS, &dL);

  BOOL bUseBase = clrBase != COLORREF(-1) && clrBase != CLR_DEFAULT;
  double dBaseH = 0.0, dBaseS, dBaseL;

  if (bUseBase) {
    CBCGPDrawManager::RGBtoHSL(clrBase, &dBaseH, &dBaseS, &dBaseL);
  }

  DWORD dwLum = RGB_LUM_COLOR(clrTone);

  if (m_nBitsPerPixel >= 24) {
    DIBSECTION ds;

    if (::GetObject(m_hbmImageWell, sizeof(DIBSECTION), &ds) == 0) {
      ASSERT(FALSE);
      return;
    }

    if (ds.dsBm.bmBitsPixel < 24) {
      ASSERT(FALSE);
      return;
    }

    if (ds.dsBm.bmBitsPixel == 24) {
      COLORREF clrTransparent =
          m_clrTransparent == (COLORREF) - 1 ? globalData.clrBtnFace : m_clrTransparent;

      DWORD dwPitch = ((ds.dsBm.bmWidth * 24 + 31) & ~31) / 8;
      LPBYTE pBits = (LPBYTE)ds.dsBm.bmBits;

      for (int y = 0; y < ds.dsBm.bmHeight; y++) {
        LPBYTE pBitsRow = pBits;

        for (int x = 0; x < ds.dsBm.bmWidth; x++) {
          COLORREF clrOrig = RGB(pBitsRow[2], pBitsRow[1], pBitsRow[0]);

          if (clrOrig == clrTransparent) {
            pBitsRow += 3;
            continue;
          }

          if (bUseBase) {
            double H, L, S;
            CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);

            if (fabs(dBaseH - H) > m_dblColorTolerance) {
              pBitsRow += 3;
              continue;
            }
          }

          DWORD dwLumPix = RGB_LUM(pBitsRow[2], pBitsRow[1], pBitsRow[0]);

          if (dwLumPix <= 128) {
            dwLumPix = dwLum * dwLumPix / 128;
          }
          else {
            dwLumPix = (255 - dwLum) * (dwLumPix - 128) / 128 + dwLum;
          }

          COLORREF color = CBCGPDrawManager::HLStoRGB_ONE(
              dH, dwLumPix / 255.0, dS * dOpacity);

          pBitsRow[2] = (BYTE)(GetRValue(color));
          pBitsRow[1] = (BYTE)(GetGValue(color));
          pBitsRow[0] = (BYTE)(GetBValue(color));

          pBitsRow += 3;
        }

        pBits += dwPitch;
      }
    }
    else {
      RGBQUAD* pBits = (RGBQUAD*) ds.dsBm.bmBits;

      for (int i = 0; i < ds.dsBm.bmWidth * ds.dsBm.bmHeight; i++) {
        RGBQUAD* pBit = pBits + i;

        if (pBit->rgbReserved == 0) {
          continue;
        }

        if (bUseBase) {
          COLORREF clrOrig = RGB(pBit->rgbRed, pBit->rgbGreen, pBit->rgbBlue);

          double H, L, S;
          CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);

          if (fabs(dBaseH - H) > m_dblColorTolerance) {
            continue;
          }
        }

        double a  = (double)pBit->rgbReserved / 255.0;
        double aR = 255.0 / (double)pBit->rgbReserved;

        DWORD dwLumPix = RGB_LUM(pBit->rgbRed * aR, pBit->rgbGreen * aR, pBit->rgbBlue * aR);

        if (dwLumPix <= 128) {
          dwLumPix = dwLum * dwLumPix / 128;
        }
        else {
          dwLumPix = (255 - dwLum) * (dwLumPix - 128) / 128 + dwLum;
        }

        COLORREF color = CBCGPDrawManager::HLStoRGB_ONE(
            dH, dwLumPix / 255.0, dS * dOpacity);

        pBit->rgbRed   = (BYTE)(GetRValue(color) * a);
        pBit->rgbGreen = (BYTE)(GetGValue(color) * a);
        pBit->rgbBlue  = (BYTE)(GetBValue(color) * a);
      }
    }

    return;
  }

  //-------------------------------------------------------
  // Create memory source DC and select an original bitmap:
  //-------------------------------------------------------
  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);

  CBitmap* hOldBitmapSrc = NULL;

  int iBitmapWidth;
  int iBitmapHeight;

  //-------------------------------
  // Get original bitmap attrbutes:
  //-------------------------------
  BITMAP bmp;

  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    return;
  }

  hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(m_hbmImageWell);

  if (hOldBitmapSrc == NULL) {
    return;
  }

  iBitmapWidth = bmp.bmWidth;
  iBitmapHeight = bmp.bmHeight;

  //----------------------------------------------------------
  // Create a new bitmap compatibel with the source memory DC:
  // (original bitmap SHOULD BE ALREADY SELECTED!):
  //----------------------------------------------------------
  CBitmap* hNewBitmap = (CBitmap*) ::CreateCompatibleBitmap(memDCSrc,
      iBitmapWidth,
      iBitmapHeight);

  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return;
  }

  //------------------------------
  // Create memory destination DC:
  //------------------------------
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);

  CBitmap* hOldBitmapDst = (CBitmap*) memDCDst.SelectObject(hNewBitmap);

  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    return;
  }

  //-----------------------------
  // Copy original bitmap to new:
  //-----------------------------
  memDCDst.BitBlt(0, 0, iBitmapWidth, iBitmapHeight,
      &memDCSrc, 0, 0, SRCCOPY);

  COLORREF clrTransparent =
      m_clrTransparent == (COLORREF) - 1 ? globalData.clrBtnFace : m_clrTransparent;

  for (int x = 0; x < iBitmapWidth; x ++) {
    for (int y = 0; y < iBitmapHeight; y ++) {
      COLORREF clrOrig = ::GetPixel(memDCDst, x, y);

      if (clrOrig == clrTransparent) {
        continue;
      }

      if (bUseBase) {
        double H, L, S;
        CBCGPDrawManager::RGBtoHSL(clrOrig, &H, &S, &L);

        if (fabs(dBaseH - H) > m_dblColorTolerance) {
          continue;
        }
      }

      DWORD dwLumPix = RGB_LUM_COLOR(clrOrig);

      if (dwLumPix <= 128) {
        dwLumPix = dwLum * dwLumPix / 128;
      }
      else {
        dwLumPix = (255 - dwLum) * (dwLumPix - 128) / 128 + dwLum;
      }

      COLORREF color = CBCGPDrawManager::HLStoRGB_ONE(
          dH, dwLumPix / 255.0, dS * dOpacity);

      if (clrOrig != color) {
        ::SetPixel(memDCDst, x, y, color);
      }
    }
  }

  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);

  ::DeleteObject(m_hbmImageWell);
  m_hbmImageWell = hNewBitmap;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;
}
//*****************************************************************************
HRGN CBCGPToolBarImages::CreateRegionFromImage(CBitmap* hbmp, COLORREF clrTransparent)
{
  if (hbmp == NULL) {
    ASSERT(FALSE);
    return NULL;
  }

  BITMAP bmp;

  if (::GetObject(hbmp, sizeof(BITMAP), &bmp) == 0) {
    ASSERT(FALSE);
    return NULL;
  }

  CDC dc;
  dc.CreateCompatibleDC(NULL);

  CBitmap* hbmpOld = (CBitmap*) dc.SelectObject(hbmp);

  int iBitmapWidth = bmp.bmWidth;
  int iBitmapHeight = bmp.bmHeight;

  CRgn rgnAll;
  rgnAll.CreateRectRgn(0, 0, iBitmapWidth, iBitmapHeight);

  for (int y = 0; y < iBitmapHeight; y++) {
    for (int x = 0; x < iBitmapWidth; x++) {
      COLORREF color = dc.GetPixel(x, y);

      if (color == clrTransparent) {
        CRgn rgnPoint;
        rgnPoint.CreateRectRgn(x, y, x + 1, y + 1);

        rgnAll.CombineRgn(&rgnAll, &rgnPoint, RGN_DIFF);
      }
    }
  }

  dc.SelectObject(hbmpOld);

  return (HRGN) rgnAll.Detach();
}
//*******************************************************************************
void CBCGPToolBarImages::SetSingleImage()
{
  if (m_hbmImageWell == NULL) {
    return;
  }

  BITMAP bmp;

  if (::GetObject(m_hbmImageWell, sizeof(BITMAP), &bmp) == 0) {
    ASSERT(FALSE);
    return;
  }

  m_sizeImage.w = bmp.bmWidth;
  m_sizeImage.h = bmp.bmHeight;

  m_iCount = 1;

  UpdateInternalImage(BCGImage_Light);
  UpdateInternalImage(BCGImage_Shadow);
}
//*******************************************************************************
CBitmap* CBCGPToolBarImages::Copy(CBitmap* hbmpSrc)
{
  if (hbmpSrc == NULL) {
    ASSERT(FALSE);
    return NULL;
  }

  if (!globalData.bIsWindowsVista) {
    return (CBitmap*) ::CopyImage(hbmpSrc, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
  }

  //-------------------------------------------------------
  // Windows Vista has some problems in ::CopyImage method,
  // copy bitmap not using this method:
  //-------------------------------------------------------

  CDC memDCSrc;
  memDCSrc.CreateCompatibleDC(NULL);

  CBitmap* hOldBitmapSrc = (CBitmap*) memDCSrc.SelectObject(hbmpSrc);

  if (hOldBitmapSrc == NULL) {
    return NULL;
  }

  BITMAP bmp;
  ::GetObject(hbmpSrc, sizeof(BITMAP), &bmp);

  //----------------------------------------------------------
  // Create a new bitmap compatibel with the source memory DC:
  //----------------------------------------------------------
  CBitmap* hNewBitmap = (CBitmap*) ::CreateCompatibleBitmap(memDCSrc,
      bmp.bmWidth,
      bmp.bmHeight);

  if (hNewBitmap == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    return NULL;
  }

  //------------------------------------------------------
  // Create memory destination DC and select a new bitmap:
  //------------------------------------------------------
  CDC memDCDst;
  memDCDst.CreateCompatibleDC(&memDCSrc);

  CBitmap* hOldBitmapDst = (CBitmap*) memDCDst.SelectObject(hNewBitmap);

  if (hOldBitmapDst == NULL) {
    memDCSrc.SelectObject(hOldBitmapSrc);
    ::DeleteObject(hNewBitmap);
    return NULL;
  }

  //-----------------------------
  // Copy original bitmap to new:
  //-----------------------------
  memDCDst.BitBlt(0, 0, bmp.bmWidth, bmp.bmHeight, &memDCSrc, 0, 0, SRCCOPY);

  memDCDst.SelectObject(hOldBitmapDst);
  memDCSrc.SelectObject(hOldBitmapSrc);

  return hNewBitmap;
}
//*******************************************************************************
BOOL CBCGPToolBarImages::SmoothResize(double dblImageScale)
{
  if (m_hbmImageWell == NULL) {
    return FALSE;
  }

  if (m_nBitsPerPixel < 24) {
    return FALSE;
  }

  if (dblImageScale == 0.0) {
    ASSERT(FALSE);
    return FALSE;
  }

  if (dblImageScale == 1.0) {
    return TRUE;
  }

  CSize sizeNew((int)(.5 + m_sizeImage.w * dblImageScale), (int)(.5 + m_sizeImage.h * dblImageScale));

  if (sizeNew == m_sizeImage ||
      m_sizeImage.w <= 0 || m_sizeImage.h <= 0 ||
      sizeNew.w <= 0 || sizeNew.h <= 0) {
    return TRUE;
  }

  int nImageCount = GetCount();

  if (nImageCount == 0) {
    return TRUE;
  }

  CSize  sizeIW(0, 0);
  {
    BITMAP bmp;

    if (::GetObject(GetImageWell(), sizeof(BITMAP), &bmp) == 0) {
      ASSERT(FALSE);
      return FALSE;
    }

    sizeIW.w = bmp.bmWidth;
    sizeIW.h = bmp.bmHeight;
  }

  m_dblScale *= dblImageScale;

  CPoint offSrc(m_sizeImage.w, 0);
  CPoint offDst(0, 0);

  if (nImageCount == 1) {
    if (sizeIW.h > m_sizeImage.h) {
      nImageCount = sizeIW.h / m_sizeImage.h;
      offSrc = CPoint(0, m_sizeImage.h);
    }
  }


  CBitmap* hBmpSrc = CBCGPDrawManager::CreateBitmap_32(m_hbmImageWell, m_clrTransparent);

  if (hBmpSrc == NULL) {
    ASSERT(FALSE);
    return FALSE;
  }

  CSize sizeNewIW(sizeNew);

  if (offSrc.x > 0) {
    sizeNewIW.w *= nImageCount;
    offDst.x = sizeNew.w;
  }
  else {
    sizeNewIW.h *= nImageCount;
    offDst.y = sizeNew.h;
  }

  CBitmap* hBmpDst = CBCGPDrawManager::CreateBitmap_32(sizeNewIW, NULL);

  if (hBmpDst == NULL) {
    ASSERT(FALSE);
    ::DeleteObject(hBmpSrc);
    return FALSE;
  }

  CBCGPZoomKernel::XFilterType ft = CBCGPZoomKernel::e_FilterTypeMitchell;

  CBCGPScanlinerBitmap ms;
  ms.attach(hBmpSrc);
  CBCGPScanlinerBitmap md;
  md.attach(hBmpDst);

  DWORD channel = ms.channels();

  CBCGPZoomKernel KernelX;
  KernelX.Create(m_sizeImage.w, sizeNew.w, 0, m_sizeImage.w, ft);

  CBCGPZoomKernel KernelY;
  KernelY.Create(m_sizeImage.h, sizeNew.h, 0, m_sizeImage.h, ft);

  double* values  = new double[channel];
  double* values2 = new double[channel];

  CPoint offSrcSum(0, 0);
  CPoint offDstSum(0, 0);

  for (int index = 0; index < nImageCount; index++) {
    const DWORD val_size   = sizeof(double) * channel;
    const DWORD offsetDstX = offDstSum.x * channel;

    for (DWORD dy = 0; dy < (DWORD)sizeNew.h; dy++) {
      const CBCGPZoomKernel::XKernelList& listY = KernelY[dy];

      LPBYTE pRowDst = md[dy + offDstSum.y] + offsetDstX;

      for (DWORD dx = 0; dx < (DWORD)sizeNew.w; dx++) {
        const CBCGPZoomKernel::XKernelList& listX = KernelX[dx];

        memset(values, 0, val_size);

        for (DWORD sy = 0; sy < listY.count; sy++) {
          const CBCGPZoomKernel::XKernel& statY = listY.stat[sy];

          const LPBYTE pRowSrc = ms[statY.pixel + offSrcSum.y];
          double weight    = statY.weight;

          memset(values2, 0, val_size);

          for (DWORD sx = 0; sx < listX.count; sx++) {
            const CBCGPZoomKernel::XKernel& statX = listX.stat[sx];

            LPBYTE pRowSrc2 = pRowSrc + (statX.pixel + offSrcSum.x) * channel;
            double weight2    = statX.weight;

            for (DWORD c = 0; c < channel; c++) {
              values2[c] += (double)(*pRowSrc2) * weight2;
              pRowSrc2++;
            }
          }

          for (DWORD c = 0; c < channel; c++) {
            values[c] += values2[c] * weight;
          }
        }

        if (channel == 4) {
          values[0] = min(values[0], values[3]);
          values[1] = min(values[1], values[3]);
          values[2] = min(values[2], values[3]);
        }

        for (DWORD c = 0; c < channel; c++) {
          *pRowDst = (BYTE)clamp_to_byte(values[c]);
          pRowDst++;
        }
      }
    }

    offSrcSum.x += offSrc.x;
    offSrcSum.y += offSrc.y;
    offDstSum.x += offDst.x;
    offDstSum.y += offDst.y;
  }

  delete [] values;
  delete [] values2;

  ::DeleteObject(hBmpSrc);

  int nOldCount = m_iCount;

  if (m_sizeImageOriginal == CSize(0, 0)) {
    m_sizeImageOriginal = m_sizeImage;
  }

  SetImageSize(sizeNew);
  m_clrTransparentOriginal = m_clrTransparent;
  m_clrTransparent = (COLORREF) - 1;
  m_hbmImageWell = hBmpDst;
  m_iCount = nOldCount;
  m_nBitsPerPixel = 32;
  UpdateInternalImage(BCGImage_Light);
  UpdateInternalImage(BCGImage_Shadow);

  return IsValid();
}

BOOL CBCGPToolBarImages::ConvertTo32Bits(COLORREF clrTransparent)
{
  if (!IsValid()) {
    ASSERT(FALSE);
    return FALSE;
  }

  if (m_nBitsPerPixel == 32) {
    return TRUE;
  }

  CBitmap* hbmpNew = CBCGPDrawManager::CreateBitmap_32(m_hbmImageWell, clrTransparent == (COLORREF) - 1 ? m_clrTransparent : clrTransparent);

  if (hbmpNew == NULL) {
    return FALSE;
  }

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);

  m_hbmImageWell = hbmpNew;
  m_clrTransparent = (COLORREF) - 1;
  m_nBitsPerPixel = 32;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageLight);
  m_hbmImageLight = NULL;

  AfxDeleteObject((HGDIOBJ*)&m_hbmImageShadow);
  m_hbmImageShadow = NULL;

  return TRUE;
}
#endif

