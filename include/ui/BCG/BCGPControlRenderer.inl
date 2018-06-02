//
// BCGPControlRenderer.cpp: implementation of the CBCGPControlRenderer class.
//
//////////////////////////////////////////////////////////////////////


CBCGPControlRendererParams::CBCGPControlRendererParams()
{
  CommonInit();
}

CBCGPControlRendererParams::~CBCGPControlRendererParams()
{
}

CBCGPControlRendererParams::CBCGPControlRendererParams(UINT uiBmpResID,
    const CRect& rtImage,
    const CRect& rtCorners,
    const CRect& rtSides/* = CRect (0, 0, 0, 0)*/,
    const CRect& rtInner/* = CRect (0, 0, 0, 0)*/)
{
  CommonInit();

  SetResourceID(MAKEINTRESOURCE(uiBmpResID));

  m_rectImage      = rtImage;
  m_rectCorners  = rtCorners;
  m_rectSides    = rtSides;
  m_rectInter      = rtInner;
}

CBCGPControlRendererParams::CBCGPControlRendererParams(LPCTSTR lpszBmpResID,
    const CRect& rtImage,
    const CRect& rtCorners,
    const CRect& rtSides/* = CRect (0, 0, 0, 0)*/,
    const CRect& rtInner/* = CRect (0, 0, 0, 0)*/)
{
  CommonInit();

  SetResourceID(lpszBmpResID);

  m_rectImage      = rtImage;
  m_rectCorners  = rtCorners;
  m_rectSides    = rtSides;
  m_rectInter      = rtInner;
}

CBCGPControlRendererParams::CBCGPControlRendererParams(UINT uiBmpResID,
    COLORREF clrTransparent,
    const CRect& rtImage,
    const CRect& rtCorners,
    const CRect& rtSides/* = CRect (0, 0, 0, 0)*/,
    const CRect& rtInner/* = CRect (0, 0, 0, 0)*/,
    BOOL bPreMultiplyCheck/* = TRUE*/)
{
  CommonInit();

  SetResourceID(MAKEINTRESOURCE(uiBmpResID));

  m_rectImage         = rtImage;
  m_rectCorners     = rtCorners;
  m_rectSides       = rtSides;
  m_rectInter         = rtInner;
  m_clrTransparent    = clrTransparent;
  m_bPreMultiplyCheck = bPreMultiplyCheck;
}

CBCGPControlRendererParams::CBCGPControlRendererParams(LPCTSTR lpszBmpResID,
    COLORREF clrTransparent,
    const CRect& rtImage,
    const CRect& rtCorners,
    const CRect& rtSides/* = CRect (0, 0, 0, 0)*/,
    const CRect& rtInner/* = CRect (0, 0, 0, 0)*/,
    BOOL bPreMultiplyCheck/* = TRUE*/)
{
  CommonInit();

  SetResourceID(lpszBmpResID);

  m_rectImage         = rtImage;
  m_rectCorners     = rtCorners;
  m_rectSides       = rtSides;
  m_rectInter         = rtInner;
  m_clrTransparent    = clrTransparent;
  m_bPreMultiplyCheck = bPreMultiplyCheck;
}

CBCGPControlRendererParams::CBCGPControlRendererParams(const CBCGPControlRendererParams& rSrc)
{
  CommonInit();

  (*this) = rSrc;
}

void CBCGPControlRendererParams::CommonInit()
{
  m_uiBmpResID   = 0;
  m_strBmpResID.Empty();
  m_rectImage.SetRectEmpty();
  m_rectCorners.SetRectEmpty();
  m_rectSides.SetRectEmpty();
  m_rectInter.SetRectEmpty();
  m_clrTransparent    = CLR_DEFAULT;
  m_bPreMultiplyCheck = TRUE;
  m_clrBase = 0;
  m_clrTarget = 0;
}

LPCTSTR CBCGPControlRendererParams::GetResourceID() const
{
  if (m_strBmpResID.IsEmpty()) {
    return MAKEINTRESOURCE(m_uiBmpResID);
  }

  return m_strBmpResID;
}

void CBCGPControlRendererParams::SetResourceID(LPCTSTR lpszBmpResID)
{
  if (IS_INTRESOURCE(lpszBmpResID)) {
    m_uiBmpResID = (UINT)((UINT_PTR)(lpszBmpResID));
  }
  else {
    m_strBmpResID = lpszBmpResID;
  }
}

void CBCGPControlRendererParams::SetBaseColor(COLORREF clrBase, COLORREF clrTarget)
{
  m_clrBase = clrBase;
  m_clrTarget = clrTarget;
}

CBCGPControlRendererParams& CBCGPControlRendererParams::operator = (const CBCGPControlRendererParams& rSrc)
{
  m_uiBmpResID      = rSrc.m_uiBmpResID;
  m_strBmpResID       = rSrc.m_strBmpResID;
  m_rectImage         = rSrc.m_rectImage;
  m_rectCorners     = rSrc.m_rectCorners;
  m_rectSides       = rSrc.m_rectSides;
  m_rectInter         = rSrc.m_rectInter;
  m_clrTransparent    = rSrc.m_clrTransparent;
  m_bPreMultiplyCheck = rSrc.m_bPreMultiplyCheck;
  m_clrBase     = rSrc.m_clrBase;
  m_clrTarget     = rSrc.m_clrTarget;

  return *this;
}


IMPLEMENT_DYNCREATE(CBCGPControlRenderer, CObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPControlRenderer::CBCGPControlRenderer()
{
  m_bMirror = FALSE;
  m_bIsScaled = FALSE;
}
//****
CBCGPControlRenderer::~CBCGPControlRenderer()
{
  CleanUp();
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

BOOL CBCGPControlRenderer::Create(const CBCGPControlRendererParams& params, BOOL bFlipvert /*= FALSE*/)
{
  CleanUp();

  m_Params = params;
#if 0
  LPCTSTR lpszResID = m_Params.GetResourceID();

  if (lpszResID != NULL) {
    m_Bitmap.SetImageSize(m_Params.m_rectImage.Size());
    m_Bitmap.SetPreMultiplyAutoCheck(m_Params.m_bPreMultiplyCheck);
    m_Bitmap.SetMapTo3DColors(FALSE);
    m_Bitmap.LoadStr(lpszResID);

    if (bFlipvert) {
      m_Bitmap.MirrorVert();
    }

    if (m_Params.m_clrTransparent != CLR_DEFAULT) {
      m_Bitmap.SetTransparentColor(m_Params.m_clrTransparent);
    }

    if (m_Params.m_clrBase != 0 &&
        m_Params.m_clrTarget != 0) {
      m_Bitmap.AddaptColors(m_Params.m_clrBase, m_Params.m_clrTarget);
    }

    if (CBCGPToolBarImages::IsRTL() && m_Bitmap.GetImageWell() != NULL &&
        m_Params.m_clrTransparent == CLR_DEFAULT) {
      BITMAP bmp;

      if (::GetObject(m_Bitmap.GetImageWell(), sizeof(BITMAP), &bmp) != 0) {
        if (bmp.bmBitsPixel == 32) {
          Mirror();
        }
      }
    }

    if (m_Params.m_rectSides.IsRectNull()) {
      m_Params.m_rectSides = m_Params.m_rectCorners;
    }

    if (m_Params.m_rectInter.IsRectNull()) {
      m_Params.m_rectInter = CRect(CPoint(0, 0), m_Params.m_rectImage.Size());
      m_Params.m_rectInter.l   += m_Params.m_rectCorners.l;
      m_Params.m_rectInter.t    += m_Params.m_rectCorners.t;
      m_Params.m_rectInter.r  -= m_Params.m_rectCorners.r;
      m_Params.m_rectInter.b -= m_Params.m_rectCorners.b;
    }

    if (bFlipvert) {
      long temp;
      temp = m_Params.m_rectCorners.t;
      m_Params.m_rectCorners.t = m_Params.m_rectCorners.b;
      m_Params.m_rectCorners.b = temp;

      temp = m_Params.m_rectSides.t;
      m_Params.m_rectSides.t = m_Params.m_rectSides.b;
      m_Params.m_rectSides.b = temp;

      long height = m_Params.m_rectImage.Height();
      temp = m_Params.m_rectInter.t;
      m_Params.m_rectInter.t = height - m_Params.m_rectInter.b;
      m_Params.m_rectInter.b = height - temp;
    }
  }

#endif
  return TRUE;
}
//****
void CBCGPControlRenderer::Mirror()
{
  //if (m_Bitmap.Mirror())
  {
    m_bMirror = !m_bMirror;
  }
}
//****
void CBCGPControlRenderer::CleanUp()
{
  m_Bitmap.Clear();
  m_Bitmap.SetTransparentColor((COLORREF)(-1));

  CBCGPControlRendererParams emptyParams;
  m_Params = emptyParams;
  m_bMirror = FALSE;
}
//****
void CBCGPControlRenderer::Draw(CDC* pDC, CRect rect, UINT index, BYTE alphaSrc/* = 255*/)
{
  CRect rectInter(rect);
  rectInter.l   += m_Params.m_rectSides.l;
  rectInter.t    += m_Params.m_rectSides.t;
  rectInter.r  -= m_Params.m_rectSides.r;
  rectInter.b -= m_Params.m_rectSides.b;

  FillInterior(pDC, rectInter, index, alphaSrc);

  DrawFrame(pDC, rect, index, alphaSrc);
}
//****
void CBCGPControlRenderer::DrawFrame(CDC* pDC, CRect rect, UINT index, BYTE alphaSrc/* = 255*/)
{
  struct XHVTypes {
    ImageAlignHorz horz;
    ImageAlignVert vert;
  };

  XHVTypes corners[4] = {
    {ImageAlignHorzLeft , ImageAlignVertTop},
    {ImageAlignHorzRight, ImageAlignVertTop},
    {ImageAlignHorzLeft , ImageAlignVertBottom},
    {ImageAlignHorzRight, ImageAlignVertBottom}
  };

  XHVTypes sides[4] = {
    {ImageAlignHorzLeft   , ImageAlignVertStretch},
    {ImageAlignHorzRight  , ImageAlignVertStretch},
    {ImageAlignHorzStretch, ImageAlignVertTop},
    {ImageAlignHorzStretch, ImageAlignVertBottom}
  };

  CRect rectImage(m_Params.m_rectImage);
  int ind = index;

  if (m_Bitmap.GetCount() == 1) {
    rectImage.OffsetRect(0, m_Params.m_rectImage.Size().h * ind);
    ind = 0;
  }

  CRect rt(rect);
  CRect rectCorners(m_Params.m_rectCorners);
  CRect rectSides(m_Params.m_rectSides);

  rt.l   += rectCorners.l;
  rt.t    += rectCorners.t;
  rt.r  -= rectCorners.r;
  rt.b -= rectCorners.b;

  if (rt.Width() > 0 || rt.Height() > 0) {
    if (rt.Height() > 0) {
      if (rectSides.l > 0) {
        CRect r(rt);
        r.l  = rect.l;
        r.r = r.l + rectSides.l;

        CRect rectPart;

        if (m_bMirror) {
          rectPart = CRect(rectImage.r - rectSides.l,
              rectImage.t + rectCorners.t, rectImage.r, rectImage.b - rectCorners.b);
        }
        else {
          rectPart = CRect(rectImage.l,
              rectImage.t + rectCorners.t, rectImage.l + rectSides.l, rectImage.b - rectCorners.b);
        }

        m_Bitmap.DrawEx(pDC, r, ind, sides[0].horz, sides[0].vert, rectPart, alphaSrc);
      }

      if (rectSides.r > 0) {
        CRect r(rt);
        r.r = rect.r;
        r.l  = r.r - rectSides.r;

        CRect rectPart;

        if (m_bMirror) {
          rectPart = CRect(rectImage.l,
              rectImage.t + rectCorners.t, rectImage.l + rectSides.r, rectImage.b - rectCorners.b);
        }
        else {
          rectPart = CRect(rectImage.r - rectSides.r,
              rectImage.t + rectCorners.t, rectImage.r, rectImage.b - rectCorners.b);
        }

        m_Bitmap.DrawEx(pDC, r, ind, sides[1].horz, sides[1].vert, rectPart, alphaSrc);
      }
    }

    if (rt.Width() > 0) {
      if (rectSides.t > 0) {
        CRect r(rt);
        r.t    = rect.t;
        r.b = r.t + rectSides.t;

        CRect rectPart;

        if (m_bMirror) {
          rectPart = CRect(rectImage.l + rectCorners.r,
              rectImage.t, rectImage.r - rectCorners.l, rectImage.t + rectSides.t);
        }
        else {
          rectPart = CRect(rectImage.l + rectCorners.l,
              rectImage.t, rectImage.r - rectCorners.r, rectImage.t + rectSides.t);
        }

        m_Bitmap.DrawEx(pDC, r, ind, sides[2].horz, sides[2].vert, rectPart, alphaSrc);
      }

      if (rectSides.b > 0) {
        CRect r(rt);
        r.b = rect.b;
        r.t    = r.b - rectSides.b;

        CRect rectPart;

        if (m_bMirror) {
          rectPart = CRect(rectImage.l + rectCorners.r,
              rectImage.b - rectSides.b, rectImage.r - rectCorners.l, rectImage.b);
        }
        else {
          rectPart = CRect(rectImage.l + rectCorners.l,
              rectImage.b - rectSides.b, rectImage.r - rectCorners.r, rectImage.b);
        }

        m_Bitmap.DrawEx(pDC, r, ind, sides[3].horz, sides[3].vert, rectPart, alphaSrc);
      }
    }

    if (rectCorners.l > 0 && rectCorners.t > 0) {
      CRect rectPart;

      if (m_bMirror) {
        rectPart = CRect(CPoint(rectImage.r - rectCorners.l, rectImage.t),
            CSize(rectCorners.l, rectCorners.t));
      }
      else {
        rectPart = CRect(CPoint(rectImage.l, rectImage.t),
            CSize(rectCorners.l, rectCorners.t));
      }

      m_Bitmap.DrawEx(pDC, rect, ind, corners[0].horz, corners[0].vert, rectPart, alphaSrc);
    }

    if (rectCorners.r > 0 && rectCorners.t > 0) {
      CRect rectPart;

      if (m_bMirror) {
        rectPart = CRect(CPoint(rectImage.l, rectImage.t),
            CSize(rectCorners.r, rectCorners.t));
      }
      else {
        rectPart = CRect(CPoint(rectImage.r - rectCorners.r, rectImage.t),
            CSize(rectCorners.r, rectCorners.t));
      }

      m_Bitmap.DrawEx(pDC, rect, ind, corners[1].horz, corners[1].vert, rectPart, alphaSrc);
    }

    if (rectCorners.l > 0 && rectCorners.b > 0) {
      CRect rectPart;

      if (m_bMirror) {
        rectPart = CRect(CPoint(rectImage.r - rectCorners.l, rectImage.b - rectCorners.b),
            CSize(rectCorners.l, rectCorners.b));
      }
      else {
        rectPart = CRect(CPoint(rectImage.l, rectImage.b - rectCorners.b),
            CSize(rectCorners.l, rectCorners.b));
      }

      m_Bitmap.DrawEx(pDC, rect, ind, corners[2].horz, corners[2].vert, rectPart, alphaSrc);
    }

    if (rectCorners.r > 0 && rectCorners.b > 0) {
      CRect rectPart;

      if (m_bMirror) {
        rectPart = CRect(CPoint(rectImage.l, rectImage.b - rectCorners.b),
            CSize(rectCorners.r, rectCorners.b));
      }
      else {
        rectPart = CRect(CPoint(rectImage.r - rectCorners.r, rectImage.b - rectCorners.b),
            CSize(rectCorners.r, rectCorners.b));
      }

      m_Bitmap.DrawEx(pDC, rect, ind, corners[3].horz, corners[3].vert, rectPart, alphaSrc);
    }
  }
}
//****
void CBCGPControlRenderer::FillInterior(CDC* pDC, CRect rect, ImageAlignHorz horz,
    ImageAlignVert vert, UINT index, BYTE alphaSrc/* = 255*/)
{
  if (m_Params.m_rectInter.IsRectEmpty()) {
    return;
  }

  CRect rectImage(m_Params.m_rectInter);

  if (m_bMirror) {
    rectImage.l  = m_Params.m_rectImage.Size().w - m_Params.m_rectInter.r;
    rectImage.r = rectImage.l + m_Params.m_rectInter.Width();
  }

  rectImage.OffsetRect(m_Params.m_rectImage.TopLeft());

  int ind = index;

  if (m_Bitmap.GetCount() == 1) {
    rectImage.OffsetRect(0, m_Params.m_rectImage.Size().h * ind);
    ind = 0;
  }

  m_Bitmap.DrawEx(pDC, rect, ind, horz, vert, rectImage, alphaSrc);
}
//****
void CBCGPControlRenderer::FillInterior(CDC* pDC, CRect rect, UINT index, BYTE alphaSrc/* = 255*/)
{
  FillInterior(pDC, rect, ImageAlignHorzStretch,
      ImageAlignVertStretch, index, alphaSrc);
}
//****
void CBCGPControlRenderer::OnSysColorChange()
{
  if (m_Bitmap.GetImageWell() != NULL) {
    m_Bitmap.OnSysColorChange();
  }
}
//****
BOOL CBCGPControlRenderer::CopyTo(CBCGPControlRenderer& dest)
{
  if (this == &dest) {
    return FALSE;
  }

  dest.CleanUp();

  if (m_Bitmap.CopyTo(dest.m_Bitmap)) {
    dest.m_Params  = m_Params;
    dest.m_bMirror = m_bMirror;

    return TRUE;
  }

  return FALSE;
}
//****

static void ResizeRect(CRect& rect, double dblScale)
{
  int nWidth = rect.Width();
  int nHeight = rect.Height();

  rect.l = (int)(.5 + dblScale * rect.l);
  rect.t = (int)(.5 + dblScale * rect.t);

  rect.r = rect.l + (int)(.5 + dblScale * nWidth);
  rect.b = rect.t + (int)(.5 + dblScale * nHeight);
}

BOOL CBCGPControlRenderer::SmoothResize(double dblScale)
{
  if (dblScale <= 1. || !m_Bitmap.SmoothResize(dblScale)) {
    return FALSE;
  }

  ResizeRect(m_Params.m_rectImage, dblScale);
  ResizeRect(m_Params.m_rectInter, dblScale);

  m_bIsScaled = TRUE;
  return TRUE;
}


IMPLEMENT_DYNCREATE(CBCGPShadowRenderer, CBCGPControlRenderer)

CBCGPShadowRenderer::CBCGPShadowRenderer()
  : m_nDepth(0)
  , m_clrBase(0)
  , m_iMinBrightness(0)
  , m_iMaxBrightness(100)
{
}
//****
CBCGPShadowRenderer::~CBCGPShadowRenderer()
{
}
//****
BOOL CBCGPShadowRenderer::Create(const CBCGPControlRendererParams& /*params*/, BOOL /*bFlipvert*/ /*= FALSE*/)
{
  return FALSE;
}
//****
BOOL CBCGPShadowRenderer::Create(int nDepth,
    COLORREF clrBase,
    int iMinBrightness/* = 0*/, int iMaxBrightness/* = 100*/,
    BOOL bCanMirror/* = TRUE*/)
{
  if (IsValid() &&
      m_nDepth == nDepth && m_clrBase == clrBase &&
      m_iMinBrightness == iMinBrightness && m_iMaxBrightness == iMaxBrightness) {
    return TRUE;
  }

  CleanUp();

  if (nDepth == 0 || iMaxBrightness == 0) {
    return FALSE;
  }

  m_nDepth         = nDepth;
  m_clrBase        = clrBase;
  m_iMinBrightness = iMinBrightness;
  m_iMaxBrightness = iMaxBrightness;

  if (m_clrBase == 0) {
    clrBase = globalData.clrBarFace;
  }

  CBitmap* hBitmap = CBCGPDrawManager::PrepareShadowMask(nDepth, clrBase, iMinBrightness, iMaxBrightness);

  if (hBitmap == NULL) {
    return FALSE;
  }

  int nSize     = nDepth < 3 ? 3 : nDepth;
  int nDestSize = nSize * 2 + 1;

  m_Params.m_rectImage   = CRect(0, 0, nDestSize, nDestSize);
  m_Params.m_rectCorners = CRect(nSize, nSize, nSize, nSize);
  m_Params.m_rectSides = m_Params.m_rectCorners;

  m_Params.m_rectInter = CRect(CPoint(0, 0), m_Params.m_rectImage.Size());
  m_Params.m_rectInter.l   += m_Params.m_rectCorners.l;
  m_Params.m_rectInter.t    += m_Params.m_rectCorners.t;
  m_Params.m_rectInter.r  -= m_Params.m_rectCorners.r;
  m_Params.m_rectInter.b -= m_Params.m_rectCorners.b;

  m_Bitmap.SetImageSize(m_Params.m_rectImage.Size());
  m_Bitmap.SetPreMultiplyAutoCheck(m_Params.m_bPreMultiplyCheck);
  m_Bitmap.SetMapTo3DColors(FALSE);

  m_Bitmap.AddImage(hBitmap, TRUE);

  ::DeleteObject(hBitmap);

  if (bCanMirror && CBCGPToolBarImages::IsRTL() && m_Bitmap.GetImageWell() != NULL) {
    Mirror();
  }

  return m_Bitmap.GetCount() == 1;
}
//****
void CBCGPShadowRenderer::OnSysColorChange()
{
}
//****
void CBCGPShadowRenderer::Draw(CDC* pDC, CRect rect, UINT index/* = 0*/, BYTE alphaSrc/* = 255*/)
{
  if (128 <= alphaSrc) {
    CRect rectInter(rect);

    if (CBCGPToolBarImages::IsRTL()) {
      rectInter.l   += m_Params.m_rectSides.l;
      rectInter.r   = rectInter.l + m_Params.m_rectSides.l;
    }
    else {
      rectInter.r  -= m_Params.m_rectSides.r;
      rectInter.l    = rectInter.r - m_Params.m_rectSides.r;
    }

    rectInter.b -= m_Params.m_rectSides.b;
    rectInter.t     = rectInter.b - m_Params.m_rectSides.b;

    FillInterior(pDC, rectInter, index, alphaSrc);
  }

  DrawFrame(pDC, rect, index, alphaSrc);
}
//****
void CBCGPShadowRenderer::DrawFrame(CDC* pDC, CRect rect, UINT index/* = 0*/, BYTE alphaSrc/* = 255*/)
{
  struct XHVTypes {
    ImageAlignHorz horz;
    ImageAlignVert vert;
  };

  XHVTypes corners[4] = {
    {CBCGPToolBarImages::ImageAlignHorzLeft , ImageAlignVertTop},
    {CBCGPToolBarImages::ImageAlignHorzRight, ImageAlignVertTop},
    {CBCGPToolBarImages::ImageAlignHorzLeft , ImageAlignVertBottom},
    {CBCGPToolBarImages::ImageAlignHorzRight, ImageAlignVertBottom}
  };

  XHVTypes sides[4] = {
    {CBCGPToolBarImages::ImageAlignHorzLeft   , ImageAlignVertStretch},
    {CBCGPToolBarImages::ImageAlignHorzRight  , ImageAlignVertStretch},
    {CBCGPToolBarImages::ImageAlignHorzStretch, ImageAlignVertTop},
    {CBCGPToolBarImages::ImageAlignHorzStretch, ImageAlignVertBottom}
  };

  CRect rectImage(m_Params.m_rectImage);
  int ind = index;

  if (m_Bitmap.GetCount() == 1) {
    rectImage.OffsetRect(0, m_Params.m_rectImage.Size().h * ind);
    ind = 0;
  }

  CRect rt(rect);
  CRect rectCorners(m_Params.m_rectCorners);
  CRect rectSides(m_Params.m_rectSides);

  rt.l   += rectCorners.l;
  rt.t    += rectCorners.t;
  rt.r  -= rectCorners.r;
  rt.b -= rectCorners.b;

  BOOL bRTL = CBCGPToolBarImages::IsRTL();

  if (rt.Width() > 0 || rt.Height() > 0) {
    if (rt.Height() > 0) {
      if (bRTL && !m_bMirror) {
        if (rectSides.l > 0) {
          CRect r(rt);
          r.l  = rect.l;
          r.r = r.l + rectSides.l;

          CRect rectPart(rectImage.l,
              rectImage.t + rectCorners.t, rectImage.l + rectSides.l, rectImage.b - rectCorners.b);

          m_Bitmap.DrawEx(pDC, r, ind, sides[0].horz, sides[0].vert, rectPart, alphaSrc);
        }
      }
      else {
        if (rectSides.r > 0) {
          CRect r(rt);
          r.r = rect.r;
          r.l  = r.r - rectSides.r;

          CRect rectPart;

          if (m_bMirror) {
            rectPart = CRect(rectImage.l,
                rectImage.t + rectCorners.t, rectImage.l + rectSides.r, rectImage.b - rectCorners.b);
          }
          else {
            rectPart = CRect(rectImage.r - rectSides.r,
                rectImage.t + rectCorners.t, rectImage.r, rectImage.b - rectCorners.b);
          }

          m_Bitmap.DrawEx(pDC, r, ind, sides[1].horz, sides[1].vert, rectPart, alphaSrc);
        }
      }
    }

    if (rt.Width() > 0) {
      if (rectSides.b > 0) {
        CRect r(rt);
        r.b = rect.b;
        r.t    = r.b - rectSides.b;

        CRect rectPart(rectImage.l + rectCorners.l,
            rectImage.b - rectSides.b, rectImage.r - rectCorners.r, rectImage.b);

        m_Bitmap.DrawEx(pDC, r, ind, sides[3].horz, sides[3].vert, rectPart, alphaSrc);
      }
    }

    if (bRTL && !m_bMirror) {
      if (rectCorners.l > 0 && rectCorners.t > 0) {
        CRect rectPart(CPoint(rectImage.l, rectImage.t),
            CSize(rectCorners.l, rectCorners.t));

        m_Bitmap.DrawEx(pDC, rect, ind, corners[0].horz, corners[0].vert, rectPart, alphaSrc);
      }

      if (rectCorners.l > 0 && rectCorners.b > 0) {
        CRect rectPart = CRect(CPoint(rectImage.l, rectImage.b - rectCorners.b),
            CSize(rectCorners.l, rectCorners.b));

        m_Bitmap.DrawEx(pDC, rect, ind, corners[2].horz, corners[2].vert, rectPart, alphaSrc);
      }

      if (rectCorners.r > 0 && rectCorners.b > 0) {
        CRect rectPart(CPoint(rectImage.r - rectCorners.r, rectImage.b - rectCorners.b),
            CSize(rectCorners.r, rectCorners.b));

        m_Bitmap.DrawEx(pDC, rect, ind, corners[3].horz, corners[3].vert, rectPart, alphaSrc);
      }
    }
    else {
      if (rectCorners.r > 0 && rectCorners.t > 0) {
        CRect rectPart;

        if (m_bMirror) {
          rectPart = CRect(CPoint(rectImage.l, rectImage.t),
              CSize(rectCorners.r, rectCorners.t));
        }
        else {
          rectPart = CRect(CPoint(rectImage.r - rectCorners.r, rectImage.t),
              CSize(rectCorners.r, rectCorners.t));
        }

        m_Bitmap.DrawEx(pDC, rect, ind, corners[1].horz, corners[1].vert, rectPart, alphaSrc);
      }

      if (rectCorners.l > 0 && rectCorners.b > 0) {
        CRect rectPart;

        if (m_bMirror) {
          rectPart = CRect(CPoint(rectImage.r - rectCorners.l, rectImage.b - rectCorners.b),
              CSize(rectCorners.l, rectCorners.b));
        }
        else {
          rectPart = CRect(CPoint(rectImage.l, rectImage.b - rectCorners.b),
              CSize(rectCorners.l, rectCorners.b));
        }

        m_Bitmap.DrawEx(pDC, rect, ind, corners[2].horz, corners[2].vert, rectPart, alphaSrc);
      }

      if (rectCorners.r > 0 && rectCorners.b > 0) {
        CRect rectPart;

        if (m_bMirror) {
          rectPart = CRect(CPoint(rectImage.l, rectImage.b - rectCorners.b),
              CSize(rectCorners.r, rectCorners.b));
        }
        else {
          rectPart = CRect(CPoint(rectImage.r - rectCorners.r, rectImage.b - rectCorners.b),
              CSize(rectCorners.r, rectCorners.b));
        }

        m_Bitmap.DrawEx(pDC, rect, ind, corners[3].horz, corners[3].vert, rectPart, alphaSrc);
      }
    }
  }
}
//****
BOOL CBCGPShadowRenderer::CopyTo(CBCGPControlRenderer& dest)
{
  if (this == &dest) {
    return FALSE;
  }

  if (CBCGPControlRenderer::CopyTo(dest)) {
    if (dest.IsKindOf(RUNTIME_CLASS(CBCGPShadowRenderer))) {
      CBCGPShadowRenderer& rDest = (CBCGPShadowRenderer&)dest;
      rDest.m_nDepth = m_nDepth;
      rDest.m_clrBase = m_clrBase;
      rDest.m_iMinBrightness = m_iMinBrightness;
      rDest.m_iMaxBrightness = m_iMaxBrightness;

      return TRUE;
    }
  }

  return FALSE;
}
