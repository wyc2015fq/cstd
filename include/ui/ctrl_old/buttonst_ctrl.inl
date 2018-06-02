// Uncomment the following line to enable support for sound effects
#define BTNST_USE_SOUND
// Return values
#ifndef BTNST_OK
#define BTNST_OK 0
#endif
#ifndef BTNST_INVALIDRESOURCE
#define BTNST_INVALIDRESOURCE 1
#endif
#ifndef BTNST_FAILEDMASK
#define BTNST_FAILEDMASK 2
#endif
#ifndef BTNST_INVALIDINDEX
#define BTNST_INVALIDINDEX 3
#endif
#ifndef BTNST_INVALIDALIGN
#define BTNST_INVALIDALIGN 4
#endif
#ifndef BTNST_BADPARAM
#define BTNST_BADPARAM 5
#endif
#ifndef BTNST_INVALIDPRESSEDSTYLE
#define BTNST_INVALIDPRESSEDSTYLE 6
#endif
// Dummy identifier for grayscale icon
#ifndef BTNST_AUTO_GRAY
#define BTNST_AUTO_GRAY (0xffffffff - 1L)
#endif
// Dummy identifier for 15% darker icon
#ifndef BTNST_AUTO_DARKER
#define BTNST_AUTO_DARKER (0xffffffff - 2L)
#endif
enum {
  ST_ALIGN_HORIZ = 0, // Icon/bitmap on the left, text on the right
  ST_ALIGN_VERT, // Icon/bitmap on the top, text on the bottom
  ST_ALIGN_HORIZ_RIGHT, // Icon/bitmap on the right, text on the left
  ST_ALIGN_OVERLAP // Icon/bitmap on the same space as text
};
enum {
  BTNST_COLOR_OUT = 0, // Background color when mouse is OUTside
  BTNST_COLOR_IN, // Background color when mouse is INside
  BTNST_COLOR_FOCUS, // Background color when the button is focused
  BTNST_MAX_COLORS
};
enum {
  BTNST_PRESSED_LEFTRIGHT = 0, // Pressed style from left to right (as usual)
  BTNST_PRESSED_TOPBOTTOM // Pressed style from top to bottom
};
typedef struct shade_style {
  BYTE id, granularity, highlight, coloring;
  COLORREF color;
} shade_style;
int shade_style_set(shade_style* s, UINT id, BYTE granularity DEFULT(8),
    BYTE highlight DEFULT(10), BYTE coloring DEFULT(5),
    COLORREF color DEFULT(_RGB(55, 55, 255)))
{
  s->id = id, s->granularity = granularity, s->highlight = highlight, s->coloring = coloring, s->color = color;
  return 0;
}
typedef struct buttonst {
  uictrl base;
  BOOL m_bIsFlat; // Is a flat button?
  BYTE m_byAlign; // Align mode
  BOOL m_bDrawBorder; // Draw border?
  COLORREF m_crColors[BTNST_MAX_COLORS]; // Colors to be used
  img_t m_bmpBk[1];
  UINT m_nBitmapOut;
  img_t m_csBitmaps[1];
  const char* m_sTitle;
  shade_style shade;
  int hit, hot;
} buttonst;
int buttonst_event(buttonst* s, const event* e)
{
  UIEVT(e);

  switch (e->msg) {
  case WM_LBUTTONUP:
    s->hit = 0;

    if (UIIsPress(s)) {
      UISetPress(0);
      UISendEvent(e, s, e->toer, EVENT_LCLICK, 0, 0);
      force_redraw(e);
      return TRUE;
    }

    break;

  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (PtInRect(prc, e->pt)) {
      s->hit = 1;
      UISetPress(s);
      UISetFocus(s);
      force_redraw(e);
      return TRUE;
    }

    break;

  case WM_MOUSEMOVE:
    s->hot = PtInRect(prc, e->pt);

    if (s->hot && !UIIsHot(s)) {
      UISetHot(s);
      force_redraw(e);
    }

    break;
  }

  return FALSE;
}
//#if 0
#ifndef TTM_SETTITLE
#define TTM_SETTITLEA (WM_USER + 32) // wParam = TTI_*, lParam = char* szTitle
#define TTM_SETTITLEW (WM_USER + 33) // wParam = TTI_*, lParam = wchar* szTitle
#ifdef UNICODE
#define TTM_SETTITLE TTM_SETTITLEW
#else
#define TTM_SETTITLE TTM_SETTITLEA
#endif
#endif
#ifndef TTS_BALLOON
#define TTS_BALLOON 0x40
#endif
DWORD buttonst_SetDefaultColors(buttonst* s)
{
  s->m_crColors[BTNST_COLOR_IN] = _GetSysColor(COLOR_BTNFACE);
  s->m_crColors[BTNST_COLOR_OUT] = _GetSysColor(COLOR_BTNFACE);
  s->m_crColors[BTNST_COLOR_FOCUS] = _GetSysColor(COLOR_BTNFACE);
  return BTNST_OK;
}
int BlendPalette(int m_wColors, COLORREF* pPal, COLORREF crColor, DWORD dwPerc)
{
  int i, r, g, b;

  if (dwPerc > 100) {
    dwPerc = 100;
  }

  for (i = 0; i < m_wColors; i++) {
    r = (BYTE)((GetRV(pPal[i]) * (100 - dwPerc) + GetRV(crColor) * dwPerc) / 100);
    g = (BYTE)((GetGV(pPal[i]) * (100 - dwPerc) + GetGV(crColor) * dwPerc) / 100);
    b = (BYTE)((GetBV(pPal[i]) * (100 - dwPerc) + GetBV(crColor) * dwPerc) / 100);
    pPal[i] = _RGB(r, g, b);
  }

  return 0;
}
enum {
  SHS_NOISE = 1,
  SHS_DIAGSHADE,
  SHS_HSHADE,
  SHS_VSHADE,
  SHS_HBUMP,
  SHS_VBUMP,
  SHS_SOFTBUMP,
  SHS_HARDBUMP,
  SHS_METAL
};
int draw_shade(HDDC hDC, RECT rc, const shade_style* s)
{
  long sXSize, sYSize, bytes, j, i, k, h = 0;
  DWORD* iDst , *posDst;
  COLORREF m_dNormal_pal[256];
  COLORREF hicr = _GetSysColor(COLOR_BTNHIGHLIGHT); //get the button base colors
  COLORREF midcr = _GetSysColor(COLOR_BTNFACE);
  COLORREF locr = _GetSysColor(COLOR_BTNSHADOW);
  long r, g, b; //build the shaded palette
  int a, x, y, d, xs, idxmax, idxmin, grainx2;
  sYSize = rc.bottom - rc.top;
  sXSize = rc.right - rc.left;

  for (i = 0; i < 129; i++) {
    r = ((128 - i) * GetRV(locr) + i * GetRV(midcr)) / 128;
    g = ((128 - i) * GetGV(locr) + i * GetGV(midcr)) / 128;
    b = ((128 - i) * GetBV(locr) + i * GetBV(midcr)) / 128;
    m_dNormal_pal[i] = _RGB(r, g, b);
  }

  for (i = 1; i < 129; i++) {
    r = ((128 - i) * GetRV(midcr) + i * GetRV(hicr)) / 128;
    g = ((128 - i) * GetGV(midcr) + i * GetGV(hicr)) / 128;
    b = ((128 - i) * GetBV(midcr) + i * GetBV(hicr)) / 128;
    m_dNormal_pal[(i + 127)] = _RGB(r, g, b);
  }

  BlendPalette(256, m_dNormal_pal, s->color, s->coloring); //color the palette
  bytes = hDC->bw >> 2;
  iDst = &_Pixel(hDC, rc.left, rc.top);
  posDst = iDst;
  grainx2 = RAND_MAX / (MAX(1, 2 * s->granularity));
  idxmax = 255 - s->granularity;
  idxmin = s->granularity;
  HDDC_BEGIN2(hDC, &rc);

  switch (s->id) {
  case SHS_METAL: //SHS_METAL
    // create the strokes
    k = 40; //stroke granularity
    draw_clear(hDC, 0);

    for (a = 0; a < 200; a++) {
      x = sXSize * rand() / RAND_MAX; //stroke postion
      y = sYSize * rand() / RAND_MAX; //stroke position
      xs = MIN(sXSize, sYSize) * rand() / (2 * RAND_MAX); //stroke lenght
      d = rand() / (RAND_MAX / k); //stroke color

      for (i = 0; i < xs; i++) {
        if (((x - i) > 0) && ((y + i) < sYSize)) {
          _SetPixel(hDC, x - i, y + i, (BYTE)d);
        }

        if (((x + i) < sXSize) && ((y - i) > 0)) {
          _SetPixel(hDC, sXSize - x + i, y - i, (BYTE)d);
        }
      }
    }

    //blend strokes with SHS_DIAGONAL
    posDst = iDst;
    a = (idxmax - idxmin - k) / 2;

    for (i = 0; i < sYSize; i++) {
      for (j = 0; j < sXSize; j++) {
        d = posDst[j] + ((a * i) / sYSize + (a * (sXSize - j)) / sXSize);

        if (j > 100) {
          int asdf = 0;
        }

        posDst[j] = (BYTE)d;
        posDst[j] += rand() / grainx2;
      }

      posDst += bytes;
    }

    break;

  case SHS_HARDBUMP: //

    //set horizontal bump
    for (i = 0; i < sYSize; i++) {
      k = (255 * i / sYSize) - 127;
      k = (k * (k * k) / 128) / 128;
      k = (k * (128 - s->granularity * 2)) / 128 + 128;

      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)k;
        posDst[j] += rand() / grainx2 - s->granularity;
      }

      posDst += bytes;
    }

    //set vertical bump
    d = MIN(16, sXSize / 6); //max edge=16
    a = 1 + sYSize * sYSize / 4;
    posDst = iDst;

    for (i = 0; i < sYSize; i++) {
      y = i - sYSize / 2;

      for (j = 0; j < sXSize; j++) {
        x = j - sXSize / 2;
        xs = sXSize / 2 - d + (y * y * d) / a;

        if (x > xs) {
          posDst[j] = idxmin + (BYTE)(((sXSize - j) * 128) / d);
        }

        if ((x + xs) < 0) {
          posDst[j] = idxmax - (BYTE)((j * 128) / d);
        }

        posDst[j] += rand() / grainx2 - s->granularity;
      }

      posDst += bytes;
    }

    break;

  case SHS_SOFTBUMP: //
    for (i = 0; i < sYSize; i++) {
      h = (255 * i / sYSize) - 127;

      for (j = 0; j < sXSize; j++) {
        k = (255 * (sXSize - j) / sXSize) - 127;
        k = (h * (h * h) / 128) / 128 + (k * (k * k) / 128) / 128;
        k = k * (128 - s->granularity) / 128 + 128;

        if (k < idxmin) {
          k = idxmin;
        }

        if (k > idxmax) {
          k = idxmax;
        }

        posDst[j] = (BYTE)k;
        posDst[j] += rand() / grainx2 - s->granularity;
      }

      posDst += bytes;
    }

    break;

  case SHS_VBUMP: //
    for (j = 0; j < sXSize; j++) {
      k = (255 * (sXSize - j) / sXSize) - 127;
      k = (k * (k * k) / 128) / 128;
      k = (k * (128 - s->granularity)) / 128 + 128;

      for (i = 0; i < sYSize; i++) {
        posDst[j + i * bytes] = (BYTE)k;
        posDst[j + i * bytes] += rand() / grainx2 - s->granularity;
      }
    }

    break;

  case SHS_HBUMP: //
    for (i = 0; i < sYSize; i++) {
      k = (255 * i / sYSize) - 127;
      k = (k * (k * k) / 128) / 128;
      k = (k * (128 - s->granularity)) / 128 + 128;

      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)k;
        posDst[j] += rand() / grainx2 - s->granularity;
      }

      posDst += bytes;
    }

    break;

  case SHS_DIAGSHADE: //
    a = (idxmax - idxmin) / 2;

    for (i = 0; i < sYSize; i++) {
      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)(idxmin + a * i / sYSize + a * (sXSize - j) / sXSize);
        posDst[j] += rand() / grainx2 - s->granularity;
      }

      posDst += bytes;
    }

    break;

  case SHS_HSHADE:
    a = idxmax - idxmin;

    for (i = 0; i < sYSize; i++) {
      k = a * i / sYSize + idxmin;

      for (j = 0; j < sXSize; j++) {
        posDst[j] = (BYTE)k;
        posDst[j] += rand() / grainx2 - s->granularity;
      }

      posDst += bytes;
    }

    break;

  case SHS_VSHADE:
    a = idxmax - idxmin;

    for (j = 0; j < sXSize; j++) {
      k = a * (sXSize - j) / sXSize + idxmin;

      for (i = 0; i < sYSize; i++) {
        posDst[j + i * bytes] = (BYTE)k;
        posDst[j + i * bytes] += rand() / grainx2 - s->granularity;
      }
    }

    break;

  case SHS_NOISE:
    for (i = 0; i < sYSize; i++) {
      for (j = 0; j < sXSize; j++) {
        posDst[j] = 128 + rand() / grainx2 - s->granularity;
      }

      posDst += bytes;
    }

    break;

  default: //SHS_NOISE
    break;
  }

  posDst = iDst;

  for (i = 0; i < sYSize; i++) {
    for (j = 0; j < sXSize; j++) {
      DWORD _clr = m_dNormal_pal[(BYTE)posDst[j]];
      posDst[j] = _clr;
    }

    posDst += bytes;
  }

  HDDC_END2(hDC);
  //m_dDisabled.Clone(&m_dNormal); //build the other bitmaps
  //m_dOver.Clone(&m_dNormal);
  //m_dOver.BlendPalette(hicr,highlight);
  //m_dDown.Clone(&m_dOver);
  return 0;
}
int drawaa_buttonst(const buttonst* s, const event* e)
{
  UIHDC(e);
  BOOL bIsDisabled = 0;
  BOOL bIsFocused = s->hit;
  BOOL bIsHot = s->hot;
  BOOL bIsPressed = s->hit;
  RECT rc = s->base.rc;
  BYTE byStyle;
  POINT m_ptPressedOffset;
  byStyle = BTNST_PRESSED_LEFTRIGHT;

  switch (byStyle) {
  case BTNST_PRESSED_LEFTRIGHT:
    m_ptPressedOffset.x = 1;
    m_ptPressedOffset.y = 1;
    break;

  case BTNST_PRESSED_TOPBOTTOM:
    m_ptPressedOffset.x = 0;
    m_ptPressedOffset.y = 2;
    break;
  } // switch

  HDDC_BEGIN(hDC, &rc);
  // Prepare draw... paint button background
  // Draw transparent?
  {
    RECT* pRect = &rc;
    COLORREF crColor;

    if (s->m_bIsFlat == FALSE) {
      if (bIsFocused) {
        COLORREF _clr = (_RGB(0, 0, 0));
        draw_rectR(hDC, pRect, _clr);
        DeflateRect(pRect, 1, 1);
      }
    }

    if (bIsHot || bIsPressed) {
      crColor = s->m_crColors[BTNST_COLOR_IN];
    }
    else {
      if (bIsFocused) {
        crColor = s->m_crColors[BTNST_COLOR_FOCUS];
      }
      else {
        crColor = s->m_crColors[BTNST_COLOR_OUT];
      }
    } // else

    draw_fillrectR(hDC, pRect, crColor);
  }

  if (s->shade.id > 0) {
    draw_shade(hDC, rc, &s->shade);
  }
  else if (s->m_bmpBk->data) {
    draw_imageR(hDC, &rc, s->m_bmpBk, 0, 0, 0);
  }

  // Draw button border
  {
    int _x0, _y0;
    RECT* pRect = &rc;

    // Draw pressed button
    if (bIsPressed) {
      if (s->m_bIsFlat) {
        if (s->m_bDrawBorder) {
          draw_3drect_inR(hDC, pRect, _GetSysColor(COLOR_BTNSHADOW), _GetSysColor(COLOR_BTNHILIGHT));
        }
      }
      else {
        COLORREF brBtnShadow = (_GetSysColor(COLOR_BTNSHADOW));
        draw_rectR(hDC, pRect, brBtnShadow);
      }
    }
    else { // ...else draw non pressed button
      COLORREF penBtnHiLight = (_GetSysColor(COLOR_BTNHILIGHT)); // White
      COLORREF pen3DLight = (_GetSysColor(COLOR_3DLIGHT)); // Light gray
      COLORREF penBtnShadow = (_GetSysColor(COLOR_BTNSHADOW)); // Dark gray
      COLORREF pen3DDKShadow = (_GetSysColor(COLOR_3DDKSHADOW)); // Black

      if (s->m_bIsFlat) {
        if (bIsHot && s->m_bDrawBorder) {
          draw_3drect_inR(hDC, pRect, _GetSysColor(COLOR_BTNHILIGHT), _GetSysColor(COLOR_BTNSHADOW));
        }
      }
      else {
        // Draw top-left borders
        // White line
        COLORREF _clr = (penBtnHiLight);
        TMoveTo(hDC, pRect->left, pRect->bottom - 1);
        _LineTo(hDC, pRect->left, pRect->top);
        _LineTo(hDC, pRect->right, pRect->top);
        // Light gray line
        _clr = (pen3DLight);
        TMoveTo(hDC, pRect->left + 1, pRect->bottom - 1);
        _LineTo(hDC, pRect->left + 1, pRect->top + 1);
        _LineTo(hDC, pRect->right, pRect->top + 1);
        // Draw bottom-right borders
        // Black line
        _clr = (pen3DDKShadow);
        TMoveTo(hDC, pRect->left, pRect->bottom - 1);
        _LineTo(hDC, pRect->right - 1, pRect->bottom - 1);
        _LineTo(hDC, pRect->right - 1, pRect->top - 1);
        // Dark gray line
        _clr = (penBtnShadow);
        TMoveTo(hDC, pRect->left + 1, pRect->bottom - 2);
        _LineTo(hDC, pRect->right - 2, pRect->bottom - 2);
        _LineTo(hDC, pRect->right - 2, pRect->top);
      } // else
    } // else
  }
  // Read the button's title
  {
    const char* sTitle = s->m_sTitle;
    BOOL bHasTitle = sTitle && strlen(sTitle) > 0;
    BOOL ishoriz = s->m_byAlign == ST_ALIGN_HORIZ;
    SIZE sz[2];
    RECT rcs[2];
    fColorMatrix* fcm = 0;
    sz[0] = imsize(s->m_csBitmaps), sz[1] = FontGetTextExtent(sTitle);
    RectImageText(rc, 2, sz, 5, DT_VCENTER | DT_CENTER, ishoriz, rcs);

    if (s->m_csBitmaps->data) {
      if (bIsPressed) {
        OffsetRect(&rcs[0], m_ptPressedOffset.x, m_ptPressedOffset.y);
      }

      if (!bIsHot) {
        if (BTNST_AUTO_GRAY == s->m_nBitmapOut) {
          fColorMatrix gray = {
            0.299, 0.299, 0.299, 0, 0,
            0.587, 0.587, 0.587, 0, 0,
            0.114, 0.114, 0.114, 0, 0,
            0.000, 0.000, 0.000, 1, 0,
            0.000, 0.000, 0.000, 0, 0,
          };
          fcm = &gray;
        }
      }

      draw_imageR(hDC, &rcs[0], s->m_csBitmaps, 0, 0, fcm);
    }

    // Write the button title (if any)
    if (bHasTitle) {
      RECT rTitle = rcs[1];
      COLORREF _clr, _clrbk;
      _clr = rgb(0, 0, 0);

      if (bIsDisabled) {
        OffsetRect(&rTitle, 1, 1);
        _clr = (_GetSysColor(COLOR_3DHILIGHT));
        draw_textR2(hDC, sTitle, rTitle, DT_WORDBREAK | DT_CENTER);
        OffsetRect(&rTitle, -1, -1);
        _clr = (_GetSysColor(COLOR_3DSHADOW));
        draw_textR2(hDC, sTitle, rTitle, DT_WORDBREAK | DT_CENTER);
      }
      else {
        if (bIsHot || bIsPressed) {
          _clrbk = (s->m_crColors[BTNST_COLOR_IN]);
        }
        else {
          if (bIsFocused) {
            _clrbk = (s->m_crColors[BTNST_COLOR_FOCUS]);
          }
          else {
            _clrbk = (s->m_crColors[BTNST_COLOR_OUT]);
          }
        }

        if (bIsPressed) {
          OffsetRect(&rTitle, m_ptPressedOffset.x, m_ptPressedOffset.y);
        }

        draw_textR2(hDC, sTitle, rTitle, DT_WORDBREAK | DT_CENTER);
      }
    }

    // Draw the focus rect
    if (bIsFocused) {
      RECT focusRect = rc;
      DeflateRect(&focusRect, 3, 3);
      TDrawFocusRect(hDC, focusRect);
    }
  }
  HDDC_END(hDC);
  return 0;
}
DWORD buttonst_setcolor(buttonst* s, BYTE byColorIndex, COLORREF crColor)
{
  if (byColorIndex >= BTNST_MAX_COLORS) {
    return BTNST_INVALIDINDEX;
  }

  // Set new color
  s->m_crColors[byColorIndex] = crColor;
  return BTNST_OK;
}
DWORD buttonst_offsetcolor(buttonst* s, BYTE byColorIndex, short shOffset)
{
  BYTE byRed = 0;
  BYTE byGreen = 0;
  BYTE byBlue = 0;
  short shOffsetR = shOffset;
  short shOffsetG = shOffset;
  short shOffsetB = shOffset;

  if (byColorIndex >= BTNST_MAX_COLORS) {
    return BTNST_INVALIDINDEX;
  }

  if (shOffset < -255 || shOffset > 255) {
    return BTNST_BADPARAM;
  }

  // Get _RGB components of specified color
  byRed = GetRV(s->m_crColors[byColorIndex]);
  byGreen = GetGV(s->m_crColors[byColorIndex]);
  byBlue = GetBV(s->m_crColors[byColorIndex]);

  // Calculate max. allowed real offset
  if (shOffset > 0) {
    if (byRed + shOffset > 255) {
      shOffsetR = 255 - byRed;
    }

    if (byGreen + shOffset > 255) {
      shOffsetG = 255 - byGreen;
    }

    if (byBlue + shOffset > 255) {
      shOffsetB = 255 - byBlue;
    }

    shOffset = MIN(MIN(shOffsetR, shOffsetG), shOffsetB);
  }
  else {
    if (byRed + shOffset < 0) {
      shOffsetR = -byRed;
    }

    if (byGreen + shOffset < 0) {
      shOffsetG = -byGreen;
    }

    if (byBlue + shOffset < 0) {
      shOffsetB = -byBlue;
    }

    shOffset = MAX(MAX(shOffsetR, shOffsetG), shOffsetB);
  }

  s->m_crColors[byColorIndex] = _RGB(byRed + shOffset, byGreen + shOffset, byBlue + shOffset);
  return BTNST_OK;
}
int buttonst_set(buttonst* s, const char* szText)
{
  UISETCALL(s, drawaa_buttonst, buttonst_event);
  s->m_sTitle = szText;
  s->m_bIsFlat = TRUE;
  s->m_bDrawBorder = TRUE;
  s->m_byAlign = ST_ALIGN_HORIZ;
  buttonst_SetDefaultColors(s);
  return 0;
}



