
#ifndef _DRAW_H_
#define _DRAW_H_

enum SIDE { SIDE_LEFT, SIDE_TOP, SIDE_RIGHT, SIDE_BOTTOM};


static int imdraw_DrawFrame(img_t* im, const IRECT* pclip, IPOINT const* pPoints, int iCount, COLOR clrBorder, COLOR clrBack)
{
  //imdrawaa_poly(im, pPoints, &iCount, 1, 1, clrBack, clrBack, 1);
  imdraw_poly(im, pclip, pPoints, iCount, 1, clrBack, clrBorder, 1);
  //imdrawaa_poly_solid_i(im, pclip, pPoints, &iCount, 1, clrBack, 0);
  return 0;
}
static int imdraw_DrawHalfRoundFrame(img_t* im, const IRECT* pclip, IRECT const* pRect, int side, int radius, COLOR clrBorder, COLOR clrBack)
{
  IPOINT pts[6];

  switch (side) {
  case SIDE_LEFT:
    pts[0].x = pRect->r - 1;
    pts[0].y = pRect->t;
    pts[1].x = pRect->l + radius;
    pts[1].y = pRect->t;
    pts[2].x = pRect->l;
    pts[2].y = pRect->t + radius;
    pts[3].x = pRect->l;
    pts[3].y = pRect->b - 1 - radius;
    pts[4].x = pRect->l + radius;
    pts[4].y = pRect->b - 1;
    pts[5].x = pRect->r - 1;
    pts[5].y = pRect->b - 1;
    break;

  case SIDE_TOP:
    pts[0].x = pRect->l;
    pts[0].y = pRect->b - 1;
    pts[1].x = pRect->l;
    pts[1].y = pRect->t + radius;
    pts[2].x = pRect->l + radius;
    pts[2].y = pRect->t;
    pts[3].x = pRect->r - 1 - radius;
    pts[3].y = pRect->t;
    pts[4].x = pRect->r - 1;
    pts[4].y = pRect->t + radius;
    pts[5].x = pRect->r - 1;
    pts[5].y = pRect->b - 1;
    break;

  case SIDE_RIGHT:
    pts[0].x = pRect->l;
    pts[0].y = pRect->t;
    pts[1].x = pRect->r - 1 - radius;
    pts[1].y = pRect->t;
    pts[2].x = pRect->r - 1;
    pts[2].y = pRect->t + radius;
    pts[3].x = pRect->r - 1;
    pts[3].y = pRect->b - 1 - radius;
    pts[4].x = pRect->r - 1 - radius;
    pts[4].y = pRect->b - 1;
    pts[5].x = pRect->l;
    pts[5].y = pRect->b - 1;
    break;

  case SIDE_BOTTOM:
    pts[0].x = pRect->l;
    pts[0].y = pRect->t;
    pts[1].x = pRect->l;
    pts[1].y = pRect->b - 1 - radius;
    pts[2].x = pRect->l + radius;
    pts[2].y = pRect->b - 1;
    pts[3].x = pRect->r - 1 - radius;
    pts[3].y = pRect->b - 1;
    pts[4].x = pRect->r - 1;
    pts[4].y = pRect->b - 1 - radius;
    pts[5].x = pRect->r - 1;
    pts[5].y = pRect->t;
    break;
  }

  imdraw_DrawFrame(im, pclip, pts, sizeof(pts) / sizeof(IPOINT), clrBorder, clrBack);
  return 0;
}
static int imdraw_DrawGradient(img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL horz, COLOR clrTopLeft, COLOR clrBottomRight)
{
  imdraw_rect_gradient(im, pclip, *pRect, clrTopLeft, clrBottomRight, horz);
  return 0;
}
static int imdraw_DrawLine(img_t* im, const IRECT* pclip, int x1, int y1, int x2, int y2, COLOR clrLine)
{
  imdraw_line(im, pclip, iPOINT(x1, y1), iPOINT(x2, y2), clrLine, 1);
  return 0;
}
static int imdraw_DrawLineA(img_t* im, const IRECT* pclip, double x1, double y1, double x2, double y2, COLOR clrLine)
{
  imdrawaa_line(im, pclip, x1, y1, x2, y2, clrLine, 1);
  return 0;
}
static int imdraw_DrawRect(img_t* im, const IRECT* pclip, int x1, int y1, int x2, int y2, COLOR clrFill, COLOR clrLine)
{
  imdraw_rect(im, pclip, iRECT(x1, y1, x2, y2), clrFill, clrLine, 1, BDM_ALL);
  return 0;
}
static int imdraw_DrawRectR(img_t* im, const IRECT* pclip, IRECT rc, COLOR clrFill, COLOR clrLine)
{
  imdraw_rect(im, pclip, rc, clrFill, clrLine, 1, BDM_ALL);
  return 0;
}
static COLOR imdraw_GetPixel(img_t* im, const IRECT* pclip, int x, int y)
{
  COLOR clr = 0xffffffff;
  IRECT rc1 = iRectClip(pclip, 0, 0, im->w, im->h);

  if (iPtInRect(&rc1, x, y)) {
    memcpy(&clr, im->tt.data + y * im->s + x * im->c, im->c);
  }

  return clr;
}
static int imdraw_SetPixel(img_t* im, const IRECT* pclip, int x, int y, COLOR clr)
{
  int _a = GetAV(clr);
  IRECT rc1 = iRectClip(pclip, 0, 0, im->w, im->h);

  if (iPtInRect(&rc1, x, y)) {
    if (255 != _a) {
      COLOR clrbk = 0xffffffff;
      memcpy(&clrbk, im->tt.data + y * im->s + x * im->c, im->c);
      clr = RGBBLEN(clrbk, clr, _a);
    }

    memcpy(im->tt.data + y * im->s + x * im->c, &clr, im->c);
  }

  return 0;
}
static int imdraw_DrawBeveledRect(img_t* im, const IRECT* pclip, IRECT const* pRect, int bevel, COLOR clrBorder)
{
  IPOINT pt[] = {pRect->l, pRect->t + bevel,
      pRect->l, pRect->b - bevel - 1,
      pRect->l + bevel, pRect->b - 1,
      pRect->r - bevel - 1, pRect->b - 1,
      pRect->r - 1, pRect->b - bevel - 1,
      pRect->r - 1, pRect->t + bevel,
      pRect->r - bevel - 1, pRect->t,
      pRect->l + bevel, pRect->t,
      pRect->l, pRect->t + bevel
                };
  return imdraw_poly(im, pclip, pt, countof(pt), 1, 0, clrBorder, 1);
}
static COLOR PixelAlpha_100(COLOR src, COLOR dst, int percent)
{
  int ipercent = 100 - percent;
  return _RGB(
      (GetRV(src) * percent + GetRV(dst) * ipercent) / 100,
      (GetGV(src) * percent + GetGV(dst) * ipercent) / 100,
      (GetBV(src) * percent + GetBV(dst) * ipercent) / 100);
}
static int imdraw_Ellipse(img_t* im, const IRECT* pclip, int x1, int y1, int x2, int y2, COLOR crFill, COLOR crLine, double wline)
{
  return imdrawaa_ellipse(im, pclip, (x1 + x2) * .5, (y1 + y2) * .5, (x2 - x1) * .5, (y2 - y1) * .5, crFill, crLine, wline);
}
static int imdraw_EllipseR(img_t* im, const IRECT* pclip, IRECT rc, COLOR crFill, COLOR crLine, double wline)
{
  return imdrawaa_ellipse(im, pclip, (rc.l + rc.r) * .5, (rc.t + rc.b) * .5, (rc.r - rc.l) * .5, (rc.b - rc.t) * .5, crFill, crLine, wline);
}
static int imdraw_FillSolidRect(img_t* im, const IRECT* pclip, IRECT const* rc, COLOR clrFill)
{
  return imdraw_rect(im, pclip, *rc, clrFill, 0, 0, 0);
}
static int imdraw_DrawText(img_t* im, const IRECT* pclip, const char* text, IRECT rc, int fmt, const font_t* fo, COLOR crFg)
{
  return imdraw_text(im, pclip, rc, text, -1, fo, crFg, 0, fmt);
}
static int imdraw_DrawMarker1(img_t* im, const IRECT* pclip, IRECT const* pRect, const img_t* img1, COLOR color)
{
  return imdraw_bit(im, pclip, *pRect, 0, img1->w, img1->h, img1->tt.data, img1->s, 0, 0, 8, color);
}
static int imdraw_DrawMarker(img_t* im, const IRECT* pclip, IRECT const* pRect, const img_t* im1, int iFrame, COLOR color)
{
  uchar* data = im1->tt.data + iFrame * im1->h * im1->s;
  return imdraw_bit(im, pclip, *pRect, 0, im1->w, im1->h, data, im1->s, 0, 0, 8, color);
}
static int font_text_extent_point(font_t* fo, const char* str, int len, int nMaxExtent, int* lpnFit, int* alpDx, ISIZE* lpSize)
{
  int i = 0, i0, ww = 0;
  int font_h = fo ? fo->h : 0;
  int font_avg_width = fo ? fo->h / 2 : 0;

  for (; i < len;) {
    ushort temp = chr_next(str, i, &i0);
    int font_w = temp <= 128 ? font_avg_width : font_h;

    if (ww + font_w > nMaxExtent) {
      *lpnFit = i;
      break;
    }

    i = i0;
  }

  return 1;
}
static BOOL CorrectFitSpaceString(font_t* fo, const IRECT* pclip, const char* strSrc, int maxLength, char* strDst/*out*/)
{
  int strSrcLength = strlen(strSrc);
  int count;
  ISIZE sz;

  if (font_text_extent_point(fo, strSrc, strSrcLength, maxLength, &count, NULL, &sz) != 0) {
    if (count < strSrcLength) {
      if (count > 0) {
        int pointsWidth = font_text_size(fo, ("..."), -1, 0).w;

        if (font_text_extent_point(fo, strSrc, strSrcLength, max(0, maxLength - pointsWidth), &count, NULL, &sz) == 0) {
          return FALSE;
        }

        strncpy(strDst, strSrc, count);
        strcat(strDst, "...");
      }
      else {
        strcat(strDst, "...");
      }

      return TRUE;
    }
  }

  return FALSE;
}

int imdraw_FillSolidRect4(img_t* im, const IRECT* pclip, int x, int y, int cx, int cy, COLOR clr)
{
  IRECT rc = iRECT2(x, y, cx, cy);
  imdraw_FillSolidRect(im, pclip, &rc, clr);
  return 0;
}
int imdraw_Draw3dRect(img_t* im, const IRECT* pclip, const IRECT* lpRect, COLOR clrTopLeft, COLOR clrBottomRight, COLOR clrFace)
{
  COLOR crLT = clrTopLeft;
  COLOR crRB = clrBottomRight;
  imdraw_border(im, pclip, *lpRect, clrFace, 1, &crLT, 1, &crLT, 1, &crRB, 1, &crRB);
  return 0;
}
int imdraw_Draw3dRect4(img_t* im, const IRECT* pclip, int x, int y, int cx, int cy, COLOR clrTopLeft, COLOR clrBottomRight, COLOR clrFace)
{
  IRECT rc = iRECT2(x, y, cx, cy);
  imdraw_Draw3dRect(im, pclip, &rc, clrTopLeft, clrBottomRight, clrFace);
  return 0;
}
int imdraw_FillGradient(img_t* im, const IRECT* pclip, IRECT rect, COLOR colorStart, COLOR colorFinish,
    BOOL bHorz , int nStartFlatPercentage , int nEndFlatPercentage)
{
  imdraw_rect_gradient2(im, pclip, rect, colorStart, colorFinish, bHorz, nStartFlatPercentage, nEndFlatPercentage);
  return 0;
}
BOOL imdraw_Image(img_t* pDCDest, const IRECT* pclip, img_t* im, int xDest, int yDest, int iImage,
    BOOL bHilite, BOOL bDisabled, BOOL bIndeterminate, BOOL bShadow, BOOL bInactive, BYTE alphaSrc)
{
  int m_iCount = im->f;
  //ISIZE m_sizeImageDest = iSIZE(im->w, im->h);
  //IRECT m_rectLastDraw;
  BOOL m_bStretch = TRUE;
  BOOL m_bFadeInactive = TRUE;
  int m_nBitsPerPixel = im->c << 3;
  BOOL m_bIsDrawOnGlass = TRUE;
  COLOR m_clrTransparent = _RGB(255, 0, 255);
#if 0

  if (iImage < 0 || iImage >= m_iCount) {
    return FALSE;
  }

  if (bShadow && globalData.m_nBitsPerPixel <= 8) {
    return TRUE;
  }

  m_rectLastDraw = IRECT(CPoint(xDest, yDest), m_sizeImageDest);

  if (m_bStretch) {
    bHilite = FALSE;
    bIndeterminate = FALSE;
  }

  CBitmap* hBmpOriginal = NULL;

  if ((!bInactive || bDisabled) && m_bFadeInactive && m_nBitsPerPixel < 32) {
    //hBmpOriginal = (CBitmap*) hDCGlyphs->SelectObject (m_hbmImageWell);
  }

  BOOL bStretchOld = m_bStretch;
  BOOL bAlphaStretch = (m_nBitsPerPixel == 32 && m_bStretch);

  if (bAlphaStretch) {
    m_bStretch = FALSE;
  }

  COLOR clrTransparent = (m_nBitsPerPixel == 32 || m_bIsDrawOnGlass) && 1
      ? (COLOR) - 1
      : m_clrTransparent;

  BOOL bIsTransparent = (clrTransparent != (COLOR) - 1);

  COLOR clrTransparentDisabled = clrTransparent;

  img_t* im = pDCDest;
  ASSERT_VALID(im);

  int x = m_bStretch || bIsTransparent ? 0 : xDest;
  int y = m_bStretch || bIsTransparent ? 0 : yDest;

  IRECT m_rectSubImage;
  ISIZE m_sizeImage = imsize(im);
  const int xOffset = m_rectSubImage.l;
  const int yOffset = m_rectSubImage.t;

  const int nWidth = m_rectSubImage.iRectIsEmpty() ? m_sizeImage.w : m_rectSubImage RCW(&);
  const int nHeight = m_rectSubImage.iRectIsEmpty() ? m_sizeImage.h : m_rectSubImage.Height();

  if (m_bStretch || bIsTransparent) {
    IRECT rectImage(CPoint(0, 0), m_sizeImage);

    if (bIsTransparent && clrTransparent != globalData.clrBtnFace) {
      CBrush brBackgr(clrTransparent);
      im->FillRect(rectImage, &brBackgr);
    }
    else {
      im->FillRect(rectImage, &globalData.brBtnFace);
    }

    if (bDisabled && globalData.m_nBitsPerPixel == 16) {
      clrTransparentDisabled = im->GetPixel(rectImage.TopLeft());
    }
  }

  BOOL bDisabledTrueColor = FALSE;

  if (bDisabled && m_nBitsPerPixel >= 24) {
    bDisabled = FALSE;
    bDisabledTrueColor = TRUE;
  }

  BOOL bShadowTrueColor = FALSE;

  if (bShadow && m_nBitsPerPixel == 32 && 1) {
    bShadow = FALSE;
    bShadowTrueColor = TRUE;
  }

  if (!bHilite && !bDisabled && !bShadow) {
    BOOL bIsReady = FALSE;

    if ((m_nBitsPerPixel == 32 || m_bIsDrawOnGlass) && 1) {
      BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0, alphaSrc, 1 /*AC_SRC_ALPHA*/ };

      if (bDisabledTrueColor) {
        //pixelblend.SourceConstantAlpha = m_nDisabledImageAlpha;
      }

      if (bInactive && m_bFadeInactive) {
        //pixelblend.SourceConstantAlpha = m_nFadedImageAlpha;
      }

      imdraw_image_corner(this->im, 0, nWidth, nHeight, hDCGlyphs, iImage * m_sizeImage.w + xOffset, yOffset, SRCCOPY);

    }

    if (!bIsReady) {
      //----------------------
      // normal image version:
      //----------------------
      ::BitBlt(im->m_hDC, x, y,
          nWidth, nHeight,
          hDCGlyphs, iImage * m_sizeImage.w + xOffset, yOffset, SRCCOPY);

      if (bDisabledTrueColor) {

        dm.GrayRect(IRECT(x, y, x + nWidth + 2, y + nHeight + 2),
            -1, clrTransparentDisabled == -1 ? globalData.clrBtnFace : clrTransparentDisabled,
            CBCGPVisualManager::GetInstance()->GetToolbarDisabledColor());
      }
    }
  }
  else if (bShadow && m_hbmImageShadow != NULL) {
    CBitmap* hbmpCurr =
        (CBitmap*) SelectObject(hDCGlyphs, m_hbmImageShadow);

    ::BitBlt(im->m_hDC, x, y,
        nWidth, nHeight,
        hDCGlyphs, iImage * m_sizeImage.w + xOffset, yOffset, SRCCOPY);

    SelectObject(hDCGlyphs, hbmpCurr);
  }
  else {
    if (bDisabled || bIndeterminate || bShadow) {
      // disabled or indeterminate version
      CreateMask(iImage, TRUE, FALSE);

      im->SetTextColor(bShadow ? m_clrImageShadow : 0L); // 0's in mono -> 0 (for ROP)
      im->SetBkColor((COLOR)0x00FFFFFFL); // 1's in mono -> 1

      if (bDisabled && CBCGPVisualManager::GetInstance()->IsEmbossDisabledImage()) {
        // disabled - draw the hilighted shadow
        HGDIOBJ hbrOld = im->SelectObject(globalData.hbrBtnHilite);

        if (hbrOld != NULL) {
          // draw hilight color where we have 0's in the mask
          ::BitBlt(im->m_hDC, x + 1, y + 1,
              nWidth + 2, nHeight + 2,
              hDCMono, 0, 0, ROP_PSDPxax);

          im->SelectObject(hbrOld);
        }
      }

      //BLOCK: always draw the shadow
      {
        HGDIOBJ hbrOld = im->SelectObject(globalData.hbrBtnShadow);

        if (hbrOld != NULL) {
          // draw the shadow color where we have 0's in the mask
          ::BitBlt(im->m_hDC,
              x, y,
              nWidth + 2, nHeight + 2,
              hDCMono, 0, 0, ROP_PSDPxax);

          im->SelectObject(hbrOld);
        }
      }
    }

    // if it is checked do the dither brush avoiding the glyph
    if (bHilite || bIndeterminate) {
      CBrush* pBrOld = im->SelectObject(&globalData.brLight);

      if (pBrOld != NULL) {
        CreateMask(iImage, !bIndeterminate, bDisabled);

        im->SetTextColor(0L);              // 0 -> 0
        im->SetBkColor((COLOR)0x00FFFFFFL); // 1 -> 1

        // only draw the dither brush where the mask is 1's
        ::BitBlt(im->m_hDC, x, y,
            nWidth, nHeight,
            hDCMono, 0, 0, ROP_DSPDxax);

        im->SelectObject(pBrOld);
      }
    }
  }

  m_bStretch = bStretchOld;
#endif
  return TRUE;
}

enum ImageAlignHorz {
  ImageAlignHorzLeft,
  ImageAlignHorzCenter,
  ImageAlignHorzRight,
  ImageAlignHorzStretch
};
enum ImageAlignVert {
  ImageAlignVertTop,
  ImageAlignVertCenter,
  ImageAlignVertBottom,
  ImageAlignVertStretch
};

#define ASSERT_VALID(x)
//****
BOOL imdraw_ImageEx(img_t* im, const IRECT* pclip, IRECT rect, int iImageIndex, int horzAlign DEFAULT(ImageAlignHorzLeft), int vertAlign DEFAULT(ImageAlignVertTop), IRECT rectSrc DEFAULT(iRECT(0, 0, 0, 0)), BYTE alphaSrc DEFAULT(255))
{
  IRECT rectDst = (rect);
  ASSERT_VALID(im);

  if (iRectIsEmpty(&rectSrc)) {
    return FALSE;
  }


  if (horzAlign != ImageAlignHorzStretch) {
    BOOL bUpdate = TRUE;

    if (horzAlign == ImageAlignHorzLeft) {
      rectDst.r = rectDst.l + RCW(&rectSrc);
    }
    else if (horzAlign == ImageAlignHorzRight) {
      rectDst.l = rectDst.r -  RCW(&rectSrc);
    }
    else if (horzAlign == ImageAlignHorzCenter) {
      rectDst.l += (RCW(&rectDst) -  RCW(&rectSrc)) / 2;
      rectDst.r = rectDst.l +  RCW(&rectSrc);
    }
    else {
      bUpdate = FALSE;
    }

    if (bUpdate) {
      IRECT rt = (rectDst);
      iRectIntersect(&rectDst, rectDst, rect);

      if (0 <  RCW(&rectDst) &&  RCW(&rectDst) !=  RCW(&rectSrc)) {
        rectSrc.l += rectDst.l - rt.l;
        rectSrc.r = rectSrc.l + min(RCW(&rectDst), RCW(&rectSrc));
      }
    }
  }

  if (vertAlign != ImageAlignVertStretch) {
    BOOL bUpdate = TRUE;

    if (vertAlign == ImageAlignVertTop) {
      rectDst.b = rectDst.t + RCH(&rectSrc);
    }
    else if (vertAlign == ImageAlignVertBottom) {
      rectDst.t = rectDst.b - RCH(&rectSrc);
    }
    else if (vertAlign == ImageAlignVertCenter) {
      rectDst.t += (RCH(&rectDst) - RCH(&rectSrc)) / 2;
      rectDst.b = rectDst.t + RCH(&rectSrc);
    }
    else {
      bUpdate = FALSE;
    }

    if (bUpdate) {
      IRECT rt = (rectDst);
      iRectIntersect(&rectDst, rectDst, rect);

      if (0 < RCH(&rectDst) && RCH(&rectDst) !=  RCH(&rectSrc)) {
        rectSrc.t += rectDst.t - rt.t;
        rectSrc.b = rectSrc.t + min(RCH(&rectDst), RCH(&rectSrc));
      }
    }
  }

  if (iRectIsEmpty(&rectSrc) || iRectIsEmpty(&rectDst)) {
    return FALSE;
  }

#if 0

  if (!globalData.bIsWindows9x) {
    CBitmap* hbmOldGlyphs = (CBitmap*)SelectObject(hDCGlyphs, m_hbmImageWell);

    const int xOffset = rectSrc.l;
    const int yOffset = rectSrc.t;

    const int nWidth = iRectIsEmpty(&rectSrc) ? m_sizeImage.w : RCW(&rectSrc);
    const int nHeight = iRectIsEmpty(&rectSrc) ? m_sizeImage.h : RCH(&rectSrc);

    BOOL bRes = FALSE;

    if (m_nBitsPerPixel == 32 && 1) {
      BLENDFUNCTION pixelblend = { AC_SRC_OVER, 0,
                    alphaSrc, 1 /*AC_SRC_ALPHA*/
                                 };

      bRes = (*m_pfAlphaBlend)(im->m_hDC, rectDst.l, rectDst.t, RCW(&rectDst), RCH(&rectDst), hDCGlyphs, iImageIndex * m_sizeImage.w + xOffset, yOffset,
          nWidth, nHeight, pixelblend);
    }
    else if (m_clrTransparent == -1) {
      bRes = ::StretchBlt(im->m_hDC, rectDst.l, rectDst.t,
          RCW(&rectDst), RCH(&rectDst),
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

  BOOL bRes = Draw(im, rectDst.l, rectDst.t, iImageIndex,
      FALSE, FALSE, FALSE, FALSE, FALSE, alphaSrc);

  m_rectSubImage.SetRectEmpty();

  EndDrawImage(ds);
  m_bCreateMonoDC = bCreateMonoDC;
#endif
  return 0;
}

BOOL imdraw_ImageList(img_t* im, const IRECT* pclip, const img_t* im1, int nImage, IPOINT pt, UINT nStyle)
{
  img_t im2[1] = {0};
  IRECT rc = iRECT2(pt.x, pt.y, im->w, im->h);
  IMINIT(im2, im1->h, im1->w, im1->tt.data + nImage * im1->h * im1->s, im1->s, im1->c, 1);
  imdraw_image_corner(im, pclip, rc, im2, 0, 0, 0);
  return TRUE;
}

void imdraw_FillRect(img_t* im, const IRECT* pclip, const IRECT* lpRect, COLOR crFill)
{
  imdraw_FillSolidRect(im, pclip, lpRect, crFill);
}
BOOL HighlightRect(img_t* im, const IRECT* pclip, IRECT rect, int nPercentage DEFULT(-1), COLOR clrTransparent DEFULT(0),
    int nTolerance DEFULT(0), COLOR clrBlend DEFULT(0))
{
  int x, y;
  IRECT rc = rect;
  IRECT rc1 = iRectInter(rc, iRectClip(pclip, 0, 0, im->w, im->h));
  uchar* ptran = (uchar*)&clrTransparent;
  uchar* pBlend = (uchar*)&clrBlend;
  rc = rc1;


  if (nPercentage == 100) {
    // Nothing to do
    return TRUE;
  }

  if (clrBlend != 0 && nPercentage > 100) {
    return FALSE;
  }

  for (y = rc.t; y < rc.b; ++y) {
    uchar* pix = im->tt.data + y * im->s + rc.l * im->c;

    for (x = rc.l; x < rc.r; ++x, pix += im->c) {
      BOOL bIgnore = FALSE;

      if (nTolerance > 0) {
        bIgnore = (abs(pix[0] - ptran[0]) < nTolerance && abs(pix[1] - ptran[1]) < nTolerance && abs(pix[2] - ptran[2]) < nTolerance);
      }
      else {
        bIgnore = pix[0] == ptran[0] && pix[1] == ptran[1] && pix[2] == ptran[2];
      }

      if (!bIgnore) {
        if (nPercentage == -1) {
          pix[0] = MIN(255, (2 * pix[0] + 255) / 3);
          pix[1] = MIN(255, (2 * pix[1] + 255) / 3);
          pix[2] = MIN(255, (2 * pix[2] + 255) / 3);
        }
        else {
          if (clrBlend == 0) {
            pix[0] = MIN(255, (nPercentage * pix[0]) / 100);
            pix[1] = MIN(255, (nPercentage * pix[1]) / 100);
            pix[2] = MIN(255, (nPercentage * pix[2]) / 100);
          }
          else {
            pix[0] = MIN(255, pix[0] + (pBlend[0] - pix[0]) * nPercentage / 100);
            pix[1] = MIN(255, pix[1] + (pBlend[1] - pix[1]) * nPercentage / 100);
            pix[2] = MIN(255, pix[2] + (pBlend[2] - pix[2]) * nPercentage / 100);
          }
        }
      }
    }
  }

  return TRUE;
}
#endif // _DRAW_H_
