
#define DEFAULT(x)

int DrawSimpleRoundRectBorder(HDDC hDC, RECT rect, COLORREF clrLine, BOOL bForClipping DEFAULT(FALSE))
{
  const int nOffset = 2;
  int _x0, _y0;
  COLORREF _clr = (clrLine);

  int nExtraOffset;

  if (bForClipping) {
    nExtraOffset = 1;
    ++rect.right;
    ++rect.bottom;
  }
  else {
    nExtraOffset = 0;
  }

  // top border
  TMoveTo(hDC, rect.left + nOffset, rect.top);
  _LineTo(hDC, rect.right - nOffset - 1, rect.top);

  // top-right corner
  _LineTo(hDC, rect.right - 1, rect.top + nOffset);

  // right border
  _LineTo(hDC, rect.right - 1, rect.bottom - 1 - nOffset - nExtraOffset);

  // bottom-right corner
  _LineTo(hDC, rect.right - nOffset - 1, rect.bottom - 1 - nExtraOffset);

  // bottom border
  _LineTo(hDC, rect.left + nOffset + nExtraOffset, rect.bottom - 1);

  // bottom-left corner
  _LineTo(hDC, rect.left, rect.bottom - 1 - nOffset - nExtraOffset);

  // left border
  _LineTo(hDC, rect.left, rect.top + nOffset);

  // top-left corner
  _LineTo(hDC, rect.left + nOffset, rect.top);
  return 0;
}

// GradientDrawParams


// Reference: <Windows Graphics Programming Win32 GDI and DirectDraw> by FengYuan
// Chapter 9.7 GRADIENT FILLS

#define IMPLEMENT_COLORREF_TO_16(cName) \
  inline COLOR16 Get##cName##16Value(COLORREF cc)\
  {\
    return MAKEWORD(0x00, Get##cName##Value(cc));\
  }\
  \
  inline COLOR16 Get##cName##16Value(COLORREF c1, COLORREF c2)\
  {\
    return MAKEWORD(0x00, (Get##cName##Value(c1) + Get##cName##Value(c2))/2 );\
  }

IMPLEMENT_COLORREF_TO_16(R)
IMPLEMENT_COLORREF_TO_16(G)
IMPLEMENT_COLORREF_TO_16(B)

#define DECLARE_COLOR16(cName, cRef) \
  const COLOR16 cName##_R = GetR16Value(cRef);\
  const COLOR16 cName##_G = GetG16Value(cRef);\
  const COLOR16 cName##_B = GetB16Value(cRef);

#define DECLARE_COLOR16_MID(cName, cRef1, cRef2) \
  const COLOR16 cName##_R = GetR16Value(cRef1, cRef2);\
  const COLOR16 cName##_G = GetG16Value(cRef1, cRef2);\
  const COLOR16 cName##_B = GetB16Value(cRef1, cRef2);

static BOOL _gradient_fill_rect_single_direction(HDDC hDC, RECT pRect, COLORREF c0, COLORREF c1, int nGradientType)
{
  BOOL bRet = FALSE;
  DECLARE_COLOR16(c0, c0)
  DECLARE_COLOR16(c1, c1)
  DECLARE_COLOR16_MID(cMid, c0, c1)
  TRIVERTEX vtx[4] = {
    {pRect.left, pRect.top, c0_R, c0_G, c0_B, 0},
    {pRect.right, pRect.bottom, c1_R, c1_G, c1_B, 0},
    {pRect.left, pRect.bottom, cMid_R, cMid_G, cMid_B, 0},
    {pRect.right, pRect.top, cMid_R, cMid_G, cMid_B, 0}
  };
  ULONG meshIndices[] = {0, 1, 2, 0, 1, 3};

  switch (nGradientType) {
  case GFT_TOP_BOTTOM:
    bRet = TGradientFill(hDC, vtx, 2, meshIndices, 1, GRADIENT_FILL_RECT_V);
    break;

  case GFT_LEFT_RIGHT:
    bRet = TGradientFill(hDC, vtx, 2, meshIndices, 1, GRADIENT_FILL_RECT_H);
    break;

  case GFT_DIAG_TL_BR:
    bRet = TGradientFill(hDC, vtx, 4, meshIndices, 2, GRADIENT_FILL_TRIANGLE);
    break;

  case GFT_DIAG_TR_BL:
    vtx[0].x = pRect.right;
    vtx[3].x = pRect.left;
    vtx[1].x = pRect.left;
    vtx[2].x = pRect.right;
    bRet = TGradientFill(hDC, vtx, 4, meshIndices, 2, GRADIENT_FILL_TRIANGLE);
    break;

  default:
    ASSERT(FALSE);
    break;
  }

  return bRet;
}

static BOOL _gradient_fill_rect_from_center(HDDC hDC, RECT pRect, COLORREF c0, COLORREF c1, int nGradientType)
{
  const int nMidX = pRect.left + (pRect.right - pRect.left) / 2;
  const int nMidY = pRect.top + (pRect.bottom - pRect.top) / 2;
  DECLARE_COLOR16(c0, c0)
  DECLARE_COLOR16(c1, c1)
  TRIVERTEX vtx[5] = {
    {pRect.left, pRect.top, c1_R, c1_G, c1_B, 0},
    {pRect.right, pRect.top, c1_R, c1_G, c1_B, 0},
    {pRect.right, pRect.bottom, c1_R, c1_G, c1_B, 0},
    {pRect.left, pRect.bottom, c1_R, c1_G, c1_B, 0},
    {nMidX, nMidY, c0_R, c0_G, c0_B, 0},
  };

  ULONG meshIndices[] = {0, 1, 4, 1, 2, 4, 2, 3, 4, 3, 0, 4};
  return TGradientFill(hDC, vtx, 5, meshIndices, 4, GRADIENT_FILL_TRIANGLE);
}

static BOOL _gradient_fill_rect_from_corner(HDDC hDC, RECT pRect, COLORREF c0, COLORREF c1, int nGradientType)
{
  DECLARE_COLOR16(c0, c0)
  DECLARE_COLOR16(c1, c1)
  TRIVERTEX vtx[4] = {
    {pRect.left, pRect.top, c1_R, c1_G, c1_B, 0},
    {pRect.right, pRect.top, c1_R, c1_G, c1_B, 0},
    {pRect.right, pRect.bottom, c1_R, c1_G, c1_B, 0},
    {pRect.left, pRect.bottom, c1_R, c1_G, c1_B, 0}
  };

  int nCorner = GET_GFT_CORNER_INDEX(nGradientType);
  vtx[nCorner].Red = c0_R;
  vtx[nCorner].Green = c0_G;
  vtx[nCorner].Blue = c0_B;

  ULONG meshIndices[] = {nCorner, (nCorner + 1) % 4, (nCorner + 2) % 4, nCorner, (nCorner + 3) % 4, (nCorner + 2) % 4};
  return TGradientFill(hDC, vtx, 4, meshIndices, 2, GRADIENT_FILL_TRIANGLE);
}

static BOOL _gradient_fill_rect_center_out(HDDC hDC, RECT pRect, COLORREF c0, COLORREF c1, int nGradientType)
{
  BOOL bRet = FALSE;
  RECT rect1 = pRect;
  RECT rect2 = pRect;

  switch (nGradientType) {
  case GFT_VERTICAL_CENTER_OUT:
  case GFT_HORIZONTAL_CENTER_OUT: {
    const BOOL bHorizontal = GFT_VERTICAL_CENTER_OUT == nGradientType;
    nGradientType = bHorizontal ? GFT_TOP_BOTTOM : GFT_LEFT_RIGHT;

    if (bHorizontal) {
      rect1.bottom = rect2.top = pRect.top + (pRect.bottom - pRect.top) / 2;
    }
    else {
      rect1.right = rect2.left = pRect.left + (pRect.right - pRect.left) / 2;
    }

    // Fill the two parts
    bRet = _gradient_fill_rect_single_direction(hDC, rect1, c1, c0, nGradientType);
    bRet &= _gradient_fill_rect_single_direction(hDC, rect2, c0, c1, nGradientType);
  }
  break;

  case GFT_DIAG_UP_CENTER_OUT:
  case GFT_DIAG_DOWN_CENTER_OUT: {
    const BOOL bDiagonalUp = GFT_DIAG_UP_CENTER_OUT == nGradientType;
    DECLARE_COLOR16(c0, c0)
    DECLARE_COLOR16(c1, c1)
    TRIVERTEX vtx[4] = {
      {pRect.left, pRect.top, c1_R, c1_G, c1_B, 0},
      {pRect.right, pRect.bottom, c1_R, c1_G, c1_B, 0},
      {pRect.left, pRect.bottom, c1_R, c1_G, c1_B, 0},
      {pRect.right, pRect.top, c1_R, c1_G, c1_B, 0}
    };
    const int nC1IndexBegin = bDiagonalUp ? 2 : 0; // the index of vertices that should be filled in color c1

    for (int ii = nC1IndexBegin; ii < nC1IndexBegin + 2; ++ii) {
      vtx[ii].Red = c0_R;
      vtx[ii].Green = c0_G;
      vtx[ii].Blue = c0_B;
    }

    ULONG meshIndices[] = {0, 2, 3, 2, 1, 3};

    if (!bDiagonalUp) {
      meshIndices[2] = 1; // change 3 to 1
      meshIndices[3] = 0; // change 2 to 0
    }

    bRet = TGradientFill(hDC, vtx, 4, meshIndices, 2, GRADIENT_FILL_TRIANGLE);
  }
  break;

  default:
    ASSERT(FALSE);
    break;
  }

  return bRet;
}

#define CHECK_CORRECT_GRADIENT_COLOR(_nGrad1, _nGrad2, _nGradientType, _c0, _c1) \
  {\
    if ( _nGrad1 == _nGradientType || _nGrad2 == _nGradientType )\
    {\
      --_nGradientType;\
      COLORREF ctmp = _c0;\
      _c0 = _c1;\
      _c1 = ctmp;\
    }\
  }

#ifndef AC_SRC_ALPHA
#define AC_SRC_ALPHA 0x01
#endif

BOOL FillGradient(HDDC hDC, RECT pRect, COLORREF c1, COLORREF c2, int nGradientType /*= GFT_TOP_BOTTOM*/, BYTE byAlpha /*= 255*/)
{
  if (!IS_VALID_GRADIENT_TYPE(nGradientType)) {
    ASSERT(FALSE);
    return FALSE;
  }

  if (0 == byAlpha) {
    return TRUE;
  }

  BOOL bRet = FALSE;
  RECT rectDraw = (pRect);

  int cx = RCW(&rectDraw);
  int cy = RCH(&rectDraw);

  RECT rectGradient(pRect);

  if (IS_GFT_FROM_CENTER(nGradientType)) {
    bRet = _gradient_fill_rect_from_center(hDC, rectGradient, c1, c2, nGradientType);
  }
  else if (IS_GFT_FROM_CORNER(nGradientType)) {
    bRet = _gradient_fill_rect_from_corner(hDC, rectGradient, c1, c2, nGradientType);
  }
  else if (IS_GFT_CENTER_OUT_IN(nGradientType)) {
    CHECK_CORRECT_GRADIENT_COLOR(GFT_VERTICAL_CENTER_IN, GFT_HORIZONTAL_CENTER_IN, nGradientType, c1, c2);
    bRet = _gradient_fill_rect_center_out(hDC, rectGradient, c1, c2, nGradientType);
  }
  else {
    // must be single direction now.
    ASSERT(IS_GFT_SINGLE_DIRECTION(nGradientType));
    CHECK_CORRECT_GRADIENT_COLOR(GFT_BOTTOM_TOP, GFT_RIGHT_LEFT, nGradientType, c1, c2);
    bRet = _gradient_fill_rect_single_direction(hDC, rectGradient, c1, c2, nGradientType);
  }

  return bRet;
}

BOOL SimpleGradientRoundRect(HDDC hDC,
    RECT pRect,
    COLORREF c1,
    COLORREF c2,
    COLORREF clrBorder /*= CLR_INVALID*/,
    int nGradientType /*= GFT_TOP_BOTTOM*/,
    BYTE byAlpha /*= 255*/
                            )
{
  if (!hDC || !IS_VALID_GRADIENT_TYPE(nGradientType)) {
    ASSERT(FALSE);
    return FALSE;
  }

  DrawSimpleRoundRectBorder(hDC, pRect, clrBorder, TRUE);

  BOOL bRet = FillGradient(hDC, pRect, c1, c2, nGradientType, byAlpha);

  if (clrBorder != CLR_INVALID) {
    DrawSimpleRoundRectBorder(hDC, pRect, clrBorder, FALSE);
  }

  return bRet;
}

int DrawRoundRectItem(HDDC hDC, RECT rect,
    COLORREF clrFillColor1, COLORREF clrFillColor2,
    COLORREF clrInnerBorder, COLORREF clrOuterBorder,
    BYTE byAlpha /*= 255*/
                     )
{
  RECT rectInner(rect);
  DeflateRect(&rectInner, 1, 1);
  SimpleGradientRoundRect(hDC, rectInner, clrFillColor1, clrFillColor2, clrInnerBorder, GFT_TOP_BOTTOM, byAlpha);
  DrawSimpleRoundRectBorder(hDC, rect, clrOuterBorder, FALSE);
  return 0;
}

int Fill4ColorsGradient(HDDC hDC, RECT rect,
    COLORREF colorStart1, COLORREF colorFinish1,
    COLORREF colorStart2, COLORREF colorFinish2,
    BOOL bHorz DEFAULT(TRUE),
    int nPercentage DEFAULT(50),
    BYTE byAlpha DEFAULT(255)
                       )
{
  ASSERT(nPercentage >= 0);
  ASSERT(nPercentage <= 100);

  RECT rectFirst = rect;
  RECT rectSecond = rect;

  if (!bHorz) {
    rectFirst.right = rectFirst.left +  RCW(&rectFirst) * nPercentage / 100;
    rectSecond.left = rectFirst.right;
  }
  else {
    rectFirst.bottom = rectFirst.top +  RCH(&rectFirst) * nPercentage / 100;
    rectSecond.top = rectFirst.bottom;
  }

  int nGradientType = bHorz ? GFT_TOP_BOTTOM : GFT_LEFT_RIGHT;
  FillGradient(hDC, rectFirst, colorStart1, colorFinish1, nGradientType, byAlpha);
  FillGradient(hDC, rectSecond, colorStart2, colorFinish2, nGradientType, byAlpha);
  return 0;
}

int Fill4ColorsGradient(HDDC hDC, RECT rect, PCCOLORS4 pColors, BOOL bHorz /*= TRUE*/, int nPercentage /*= 50*/, BYTE byAlpha /*= 255*/)
{
  ASSERT(pColors);
  Fill4ColorsGradient(hDC, rect, pColors->c1, pColors->c2, pColors->c3, pColors->c4, bHorz, nPercentage, byAlpha);
  return 0;
}

const static COLORS4 s_ButtonClrsNormal = {RGB_BUTTON_NORMAL_FILL_COLOR1, RGB_BUTTON_NORMAL_FILL_COLOR2,
                     RGB_BUTTON_NORMAL_FILL_COLOR3, RGB_BUTTON_NORMAL_FILL_COLOR4
                                          };

const static COLORS4 s_ButtonClrsHot = {RGB_BUTTON_HOT_FILL_COLOR1, RGB_BUTTON_HOT_FILL_COLOR2,
                     RGB_BUTTON_HOT_FILL_COLOR3, RGB_BUTTON_HOT_FILL_COLOR4
                                       };

const static COLORS4 s_ButtonClrsPressed = {RGB_BUTTON_PUSHED_FILL_COLOR1, RGB_BUTTON_PUSHED_FILL_COLOR2,
                     RGB_BUTTON_PUSHED_FILL_COLOR3, RGB_BUTTON_PUSHED_FILL_COLOR4
                                           };

const static COLORS4 s_HeaderClrsNormal = {RGB_HEADER_NORMAL_FILL_COLOR1, RGB_HEADER_NORMAL_FILL_COLOR2,
                     RGB_HEADER_NORMAL_FILL_COLOR3, RGB_HEADER_NORMAL_FILL_COLOR4
                                          };

const static COLORS4 s_HeaderClrsHot = {RGB_HEADER_HOT_FILL_COLOR1, RGB_HEADER_HOT_FILL_COLOR2,
                     RGB_HEADER_HOT_FILL_COLOR3, RGB_HEADER_HOT_FILL_COLOR4
                                       };

const static COLORS4 s_HeaderClrsPressed = {RGB_HEADER_PUSHED_FILL_COLOR1, RGB_HEADER_PUSHED_FILL_COLOR2,
                     RGB_HEADER_PUSHED_FILL_COLOR3, RGB_HEADER_PUSHED_FILL_COLOR4
                                           };

typedef struct tagItem3DBackgrnd {
  PCCOLORS4 clrs[CDSTATEID_MAX];
  int nPercentage;
} Item3DBackgrnd, *PItem3DBackgrnd;

const static Item3DBackgrnd s_itemButton = {
  NULL, &s_ButtonClrsHot, &s_ButtonClrsNormal, &s_ButtonClrsPressed, BUTTON_FIRST_HALF_PERCENTAGE
};

const static Item3DBackgrnd s_itemHeader = {
  NULL, &s_HeaderClrsHot, &s_HeaderClrsNormal, &s_HeaderClrsPressed, HEADER_FIRST_HALF_PERCENTAGE
};

const static Item3DBackgrnd s_itemComboBtn = {
  NULL, &s_ButtonClrsHot, NULL, &s_ButtonClrsPressed, BUTTON_FIRST_HALF_PERCENTAGE
};

const static int DrawBorderLines(HDDC hDC, RECT rect, COLORREF clrLine, UINT nOffset)
{
  COLORREF _clr = (clrLine);
  int _x0, _y0;

  // top border
  TMoveTo(hDC, rect.left + nOffset, rect.top);
  _LineTo(hDC, rect.right - nOffset + 1, rect.top);

  // bottom border
  TMoveTo(hDC, rect.left + nOffset, rect.bottom);
  _LineTo(hDC, rect.right - nOffset + 1, rect.bottom);

  // left border
  TMoveTo(hDC, rect.left, rect.top + nOffset);
  _LineTo(hDC, rect.left, rect.bottom - nOffset + 1);

  // right border
  TMoveTo(hDC, rect.right, rect.top + nOffset);
  _LineTo(hDC, rect.right, rect.bottom - nOffset + 1);

  return 0;
}

static int DrawButtonBorder(HDDC hDC, RECT rect, CDSTATEID iStateId,
    BOOL bDrawShadow = TRUE,
    int nOuterBorderOffset = 3,
    int nInnerBorderOffset = 1
                           )
{
  COLORREF clrOuterBorder, clrInnerBorder, clrShadow;

  switch (iStateId) {
  case CDSTATEID_PRESSED:
    clrOuterBorder = _RGB(44, 98, 139);
    clrInnerBorder = _RGB(158, 176, 186);
    clrShadow = _RGB(46, 96, 134);
    break;

  case CDSTATEID_HOT:
    clrOuterBorder = _RGB(60, 127, 177);
    clrInnerBorder = _RGB(250, 253, 254);
    clrShadow = _RGB(66, 131, 179);
    break;

  case CDSTATEID_DISABLED:
    clrOuterBorder = _GetSysColor(COLOR_BTNSHADOW);
    clrInnerBorder = _RGB(250, 253, 254);
    clrShadow = clrOuterBorder;
    break;

  default:
    clrOuterBorder = _RGB(112, 112, 112);
    clrInnerBorder = _RGB(252, 252, 252);
    clrShadow = _RGB(144, 144, 144);
    break;
  }

  DrawBorderLines(hDC, rect, clrOuterBorder, nOuterBorderOffset);

  if (bDrawShadow) {
    // Top-left
    _SetPixel(hDC, rect.left + 1, rect.top, clrShadow);
    _SetPixel(hDC, rect.left + 2, rect.top, clrShadow);
    _SetPixel(hDC, rect.left, rect.top + 1, clrShadow);
    _SetPixel(hDC, rect.left + 1, rect.top + 1, clrShadow);
    _SetPixel(hDC, rect.left, rect.top + 2, clrShadow);

    // Top-right
    _SetPixel(hDC, rect.right - 2, rect.top, clrShadow);
    _SetPixel(hDC, rect.right - 1, rect.top, clrShadow);
    _SetPixel(hDC, rect.right - 1, rect.top + 1, clrShadow);
    _SetPixel(hDC, rect.right, rect.top + 1, clrShadow);
    _SetPixel(hDC, rect.right, rect.top + 2, clrShadow);

    // Bottom-left
    _SetPixel(hDC, rect.left, rect.bottom - 2, clrShadow);
    _SetPixel(hDC, rect.left, rect.bottom - 1, clrShadow);
    _SetPixel(hDC, rect.left + 1, rect.bottom - 1, clrShadow);
    _SetPixel(hDC, rect.left + 1, rect.bottom, clrShadow);
    _SetPixel(hDC, rect.left + 2, rect.bottom, clrShadow);

    // Bottom-right
    _SetPixel(hDC, rect.right, rect.bottom - 2, clrShadow);
    _SetPixel(hDC, rect.right - 1, rect.bottom - 1, clrShadow);
    _SetPixel(hDC, rect.right, rect.bottom - 1, clrShadow);
    _SetPixel(hDC, rect.right - 2, rect.bottom, clrShadow);
    _SetPixel(hDC, rect.right - 1, rect.bottom, clrShadow);
  }

  DeflateRect(&rect, 1, 1);
  DrawBorderLines(hDC, rect, clrInnerBorder, nInnerBorderOffset);
  return 0;
}

int DrawDropDownArrow(HDDC hDC, RECT pRect)
{
  RECT rcFill(pRect);

  // Draw the down arrow
  COLORREF _clr(_RGB(0, 0, 0));
  int _x0, _y0;
  int ii, yy;

  POINT ptTopCenter = cPOINT(CenterPoint(&rcFill).x, (rcFill.top + rcFill.bottom - 1) / 2 - 1);

  for (ii = 0, yy = ptTopCenter.y; ii < 4; ++ii, ++yy) {
    int cx = 7 - ii * 2;
    int xx = ptTopCenter.x - cx / 2;
    TMoveTo(hDC, xx, yy);
    _LineTo(hDC, xx + cx, yy);
  }

  return 0;
}

HRESULT CustomDrawThemeBackground(HDDC hDC, CDTBT type, CDSTATEID state, RECT pRect)
{
  HRESULT hresult = S_OK;

  RECT rcFill(pRect);

  const Item3DBackgrnd* pItem = NULL;

  switch (type) {
  case CDTBT_PUSHBUTTON:
  case CDTBT_COMBOBUTTON:
    pItem = &s_itemButton;
    DeflateRect(&rcFill, 2, 2);
    break;

  case CDTBT_COMBODOWNBUTTON:
    pItem = &s_itemComboBtn;
    DeflateRect(&rcFill, 2, 2);
    break;

  case CDTBT_HEADER:
    pItem = &s_itemHeader;
    break;

  default:
    ASSERT(0);
    return hresult;
    break;
  }

  ASSERT(pItem);
  PCCOLORS4 pFillColors = pItem->clrs[state];

  if (pFillColors) {
    Fill4ColorsGradient(hDC, rcFill, pFillColors, TRUE, pItem->nPercentage, 255);
  }

  BOOL bDrawShadow = TRUE;
  int nOuterBorderOffset = 3;
  int nInnerBorderOffset = 1;
  BOOL bDrawBtnBorder = FALSE;
  BOOL bDisableFill = FALSE;

  if (CDSTATEID_DISABLED == state) {
    if (CDTBT_PUSHBUTTON == type || CDTBT_COMBOBUTTON == type) {
      bDisableFill = TRUE;
    }
  }

  switch (type) {
  case CDTBT_PUSHBUTTON:
  case CDTBT_COMBOBUTTON: {
    bDrawBtnBorder = TRUE;
  }
  break;

  case CDTBT_COMBODOWNBUTTON: {
    if (pFillColors) {
      bDrawBtnBorder = TRUE;
      bDrawShadow = FALSE;
      nOuterBorderOffset = nInnerBorderOffset = 0;
    }

    DrawDropDownArrow(hDC, rcFill);
  }
  break;

  case CDTBT_HEADER:
    break;
  }

  if (bDisableFill) {
    TFillSolidRect(hDC, &rcFill, _GetSysColor(COLOR_3DFACE));
  }

  if (bDrawBtnBorder) {
    RECT rect(pRect);
    --rect.right;
    --rect.bottom;
    DrawButtonBorder(hDC, rect, state, bDrawShadow, nOuterBorderOffset, nInnerBorderOffset);
  }

  return hresult;
}


int DrawSortArrow(HDDC hDC, RECT rectArrow, BOOL bIsUp)
{
  COLORREF clrLight = _RGB(154, 199, 224); //_GetSysColor(COLOR_3DHILIGHT);
  COLORREF clrDark = _RGB(67, 95, 111); //_GetSysColor(COLOR_3DSHADOW);
  COLORREF penLight(clrLight);
  COLORREF penDark(clrDark);

  COLORREF _clr = (penLight);
  int _x0, _y0;

  if (!bIsUp) {
    TMoveTo(hDC, rectArrow.right, rectArrow.top);
    _LineTo(hDC, CenterPoint(&rectArrow).x, rectArrow.bottom);

    _clr = (penDark);
    _LineTo(hDC, rectArrow.left, rectArrow.top);
    _LineTo(hDC, rectArrow.right, rectArrow.top);
  }
  else {
    TMoveTo(hDC, rectArrow.left, rectArrow.bottom);
    _LineTo(hDC, rectArrow.right, rectArrow.bottom);
    _LineTo(hDC, CenterPoint(&rectArrow).x, rectArrow.top);

    _clr = (penDark);
    _LineTo(hDC, rectArrow.left, rectArrow.bottom);
  }

  return 0;
}

// http://www.codeproject.com/KB/list/CGridListCtrlEx.aspx
BOOL CheckOSVersion(WORD requestOS)
{
  static WORD fullver = 0;

  if (fullver == 0) {
    OSVERSIONINFO osver = {0};
    osver.dwOSVersionInfoSize = sizeof(osver);
    GetVersionEx(&osver);
    fullver = MAKEWORD(osver.dwMinorVersion, osver.dwMajorVersion);
  }

  return requestOS <= fullver;
}

int DrawAlphaBlendedSelection(HDDC hDC, RECT rcPaint, RECT rcSelBox, COLORREF clrSelFill, COLORREF clrSelBorder)
{
  RECT rcSelUpdate;
  NormalizeRect(&rcPaint);
  NormalizeRect(&rcSelBox);

  if (IntersectRect(&rcSelUpdate, &rcSelBox, &rcPaint) && !IsRectEmpty(&rcSelUpdate)) {
    TFillSolidRect(hDC, &rcSelUpdate, clrSelFill);
    //TFrameRect(hDC, rcSelBox, clrSelBorder);
    DrawRect(hDC, rcSelBox, 0, clrSelBorder);
  }

  return 0;
}
