
// CBCGPColorPickerCtrl window

#include <math.h>


enum COLORTYPE { CURRENT, LUMINANCE, PICKER, HEX, HEX_GREYSCALE, PICKERH };

#define DEFAULT_WIDTH_OF_LUMINANCE_BAR  20
#define DEFAULT_OFFSET_OF_LUMINANCE_BAR 5
#define DEFAULT_LUMINANCE       0.50f
#define PICKER_CURSOR_SIZE        19
#define PICKER_CURSOR_SIZEH       9
#define LUM_CURSOR_SIZE           9

// Hex
#define NUM_LEVELS      7
#define CELL_EDGES      6
#define GRAY_CELLS_NUM    15  // + 2 (Black and white)
#define TAN30       0.57735026918962F
#define YOFFSET       (1.5F * TAN30)
//#define PI          3.14159265358979

static const float cfxOffset[] = { -0.5, -1.0, -0.5, 0.5, 1.0, 0.5 };
static const float cfyOffset[] = { YOFFSET, 0.0, -YOFFSET, -YOFFSET, 0.0, YOFFSET };

static const COLORREF colorWhite = _RGB(255, 255, 255);
static const COLORREF colorBlack = _RGB(0, 0, 0);

static int AlignColor(int lPart, int lDelta)
{
  if (lDelta == 0) {
    return lPart;
  }

  if (lPart < lDelta) {
    return 0;
  }

  if (lPart > 255 - lDelta) {
    return 255;
  }

  if (abs(lPart - 128) < lDelta) {
    return 128;
  }

  if (abs(lPart - 192) < lDelta) {
    return 192;
  }

  return lPart;
}
typedef struct cellobj {
  COLORREF m_CellColor;
  double x, y, w;
} cellobj;

int cellobj_getpoints(double x, double y, double nCellWidth, DPOINT* pptArray)
{
  // side length = half the height * sin(60)
  double w = nCellWidth - 2;
  double nTemp0, nTemp1, nTemp2;
  nTemp2 = w * TAN30;
  nTemp0 = nTemp2 / 2.;
  nTemp1 = w / 2.;

  pptArray[0].x = x - nTemp1;
  pptArray[0].y = y - nTemp0;

  pptArray[1].x = x;
  pptArray[1].y = y - nTemp2;

  pptArray[2].x = x + nTemp1;
  pptArray[2].y = y - nTemp0;

  pptArray[3].x = x + nTemp1;
  pptArray[3].y = y + nTemp0;

  pptArray[4].x = x;
  pptArray[4].y = y + nTemp2;

  pptArray[5].x = x - nTemp1;
  pptArray[5].y = y + nTemp0;
  return 0;
}

int cellobj_set(cellobj* s, COLORREF color, double x, double y, double nCellWidth, long lDelta)
{
  int lRed, lGreen, lBlue;
  s->x = x, s->y = y;
  s->w = nCellWidth;

  // Approximate color to one of "standard" colors:
  lRed = AlignColor(GetRV(color), lDelta);
  lGreen = AlignColor(GetGV(color), lDelta);
  lBlue = AlignColor(GetBV(color), lDelta);

  s->m_CellColor = _RGB(lRed, lGreen, lBlue);
  return 0;
}

BOOL cellobj_hittest(cellobj* s, POINT pt)
{
  DPOINT   pts[CELL_EDGES];
  cellobj_getpoints(s->x, s->y, s->w, pts);
  return point_in_poly(pts, CELL_EDGES, pt.x, pt.y);
}

int draw_cellobj(const cellobj* s, HDDC  pDC)
{
  DPOINT pts[CELL_EDGES];
  cellobj_getpoints(s->x, s->y, s->w, pts);
  drawaa_poly(pDC, pts, CELL_EDGES, s->m_CellColor, 0, 0);
  return 0;
}

int draw_cellobj_selected(const cellobj* s, HDDC pDC)
{
  COLORREF clrWhite = _RGB(255, 255, 255);
  COLORREF clrBlack = _RGB(0, 0, 0);
  DPOINT pts[CELL_EDGES];

  cellobj_getpoints(s->x, s->y, s->w + 2, pts);
  drawaa_poly(pDC, pts, CELL_EDGES, 0, clrWhite, 2);

  cellobj_getpoints(s->x, s->y, s->w + 3, pts);
  drawaa_poly(pDC, pts, CELL_EDGES, 0, clrBlack, 1);

  cellobj_getpoints(s->x, s->y, s->w - 2, pts);
  drawaa_poly(pDC, pts, CELL_EDGES, 0, clrBlack, 1);
  return 0;
}


typedef struct colorpicker_ctrl {
  uictrl base;
  int m_COLORTYPE;
  COLORREF  m_color;
  int     m_nLumBarWidth;
  int m_foc;
  cellobj  m_arCells[256]; // Array of cellobj's
  int m_arCellsSize;
  RECT m_rcCursorRect;
} colorpicker_ctrl;

// CBCGPColorPickerCtrl struct

int colorpicker_ctrl_set(colorpicker_ctrl* s)
{
  s->m_nLumBarWidth = DEFAULT_WIDTH_OF_LUMINANCE_BAR;
  s->m_COLORTYPE = PICKER;
  return 0;
}

// CBCGPColorPickerCtrl message handlers
double GetAngleFromPoint(double nX, double nY)
{
  double dAngle = atan2((double) nY, (double)nX);
  return (dAngle * 180.0 / PI);
}

int CreateHexGreyScaleBar(colorpicker_ctrl* s, RECT rectClient)
{
  int cellSize, nCellLargeSize, yCenter, nSideLength, y1, y2;
  int nRowNum, nRGBOffset, nStartOffset;
  RECT area = rectClient;

  if (s->m_arCellsSize != 0) {
    // Already created
    return 0;
  }

  cellSize = MIN(RCH(&area) / 2 - 2,
      RCW(&area) / (GRAY_CELLS_NUM / 2 + 6));

  if ((cellSize % 2) != 0) {
    cellSize ++;
  }

  nCellLargeSize = cellSize * 2;

  yCenter = (area.top + area.bottom) / 2;
  nSideLength = (int)(cellSize * TAN30 * 1.5);

  y1 = yCenter - nSideLength / 2;
  y2 = y1 + nSideLength;

  nRGBOffset = 255 / (GRAY_CELLS_NUM + 2);

  nStartOffset = area.left;

  for (nRowNum = 0; nRowNum < 2; nRowNum++) {
    int i, x, nCurry, nRGB;

    if (nRowNum == 1) {
      // Draw large white cell:
      int x1 = nStartOffset + (nCellLargeSize / 2);
      cellobj_set(s->m_arCells + s->m_arCellsSize, colorWhite, x1, yCenter, nCellLargeSize, 0);
      s->m_arCellsSize++;
    }

    x = nCellLargeSize + cellSize + nStartOffset;
    nCurry = y1;
    nRGB = 255 - nRGBOffset;

    for (i = 0; i < GRAY_CELLS_NUM; i++) {
      COLORREF color = _RGB(nRGB, nRGB, nRGB);

      if (nRowNum == 1) {
        cellobj_set(s->m_arCells + s->m_arCellsSize, color, x, nCurry, cellSize, 7);
        s->m_arCellsSize++;
      }

      x += (cellSize / 2);

      nCurry = (nCurry == y1) ? y2 : y1; // Toggle Y
      nRGB -= nRGBOffset;
    }

    if (nRowNum == 1) {
      // Draw large black cell:
      int x1 = (x + cellSize + (cellSize / 2)) - 1;

      cellobj_set(s->m_arCells + s->m_arCellsSize, colorBlack, x1, yCenter, nCellLargeSize, 0);
      s->m_arCellsSize++;
    }

    x += nCellLargeSize + (cellSize / 2);

    if (nRowNum == 0) {
      nStartOffset = (area.right - x) / 2;
    }
  }

  return 0;
}

int SetColor(colorpicker_ctrl* s, COLORREF Color)
{
  s->m_color = Color;
  return 0;
}

int CreateHexagon(colorpicker_ctrl* s, RECT rectClient)
{
  double cellSize, x, y;
  int nLevel;
  s->m_arCellsSize = 0;

  // Normalize to squere:
  if (RCH(&rectClient) < RCW(&rectClient)) {
    DeflateRect(&rectClient, (RCW(&rectClient) - RCH(&rectClient)) / 2, 0);
  }
  else {
    DeflateRect(&rectClient, 0, (RCH(&rectClient) - RCW(&rectClient)) / 2);
  }

  ASSERT(abs(RCH(&rectClient) - RCW(&rectClient)) <= 1);

  cellSize = RCH(&rectClient) / (2. * NUM_LEVELS - 1);

  x = (rectClient.left + rectClient.right) / 2;
  y = (rectClient.top + rectClient.bottom) / 2;

  // Add center cell
  cellobj_set(s->m_arCells + s->m_arCellsSize, colorWhite, x, y, cellSize, 0);
  s->m_arCellsSize++;

  // for each level
  for (nLevel = 1; nLevel < NUM_LEVELS; nLevel++) {
    int nCell, nSide;
    double nPosX, nPosY;
    // store the level start position
    nPosX = x + (cellSize * nLevel);
    nPosY = y;

    // for each side
    for (nSide = 0; nSide < NUM_LEVELS - 1; nSide++) {
      // set the deltas for the side
      double nDx = (cellSize * cfxOffset[nSide]);
      double nDy = (cellSize * cfyOffset[nSide]);

      // for each cell per side
      for (nCell = 0; nCell < nLevel; nCell++) {
        double nAngle = GetAngleFromPoint(nPosX - x, nPosY - y);

        // TODO: Set the luminance and saturation properly
        double L = 1. *(NUM_LEVELS - nLevel) / NUM_LEVELS + .1;

        cellobj_set(s->m_arCells + s->m_arCellsSize, HLStoRGB_TWO(nAngle, L, 1.0), nPosX, nPosY, cellSize, 16);
        s->m_arCellsSize++;

        // offset the position
        nPosX += nDx;
        nPosY += nDy;
      }
    }
  }

  return 0;
}

int DrawHex(const colorpicker_ctrl* s, HDDC hDC)
{
  int i;
  const cellobj* pSelCell = NULL;

  for (i = 0; i < s->m_arCellsSize; i ++) {
    const cellobj* pCell = s->m_arCells + i;

    draw_cellobj(pCell, hDC);

    if (pCell->m_CellColor == s->m_color) {
      pSelCell = pCell;
    }
  }

  if (pSelCell != NULL) {
    draw_cellobj_selected(pSelCell, hDC);
  }

  return 0;
}

int DrawPicker(const colorpicker_ctrl* s, HDDC hDC, RECT rectClient)
{
  int i, j;
  SIZE sz = RCSZ(&rectClient);
  int nStep = 1;

  for (i = 0; i < sz.h; i += nStep) {
    for (j = 0; j < sz.w; j += nStep) {
      POINT pt = iPOINT(j, sz.h - i - nStep);
      COLORREF color = HLStoRGB_ONE((double)j / sz.w, DEFAULT_LUMINANCE, (double)i / sz.h);
      // Draw exact color:
      _SetPixel(hDC, pt.x, pt.y, color);
    }
  }

  return 0;
}

int DrawCursor(const colorpicker_ctrl* s, HDDC hDC, RECT rect)
{
  int nHalfSize = RCW(&rect) / 2; // Assume square

  if (s->m_COLORTYPE == PICKER) {
    COLORREF colorFocus = (s->m_foc) ? colorBlack : colorWhite;
    draw_fillrect(hDC, (rect.left + nHalfSize) - 1, rect.top, 3, 5, colorFocus); // Top
    draw_fillrect(hDC, (rect.left + nHalfSize) - 1, rect.bottom - 5, 3, 5, colorFocus); // Bottom
    draw_fillrect(hDC, rect.left, (rect.top + nHalfSize) - 1, 5, 3, colorFocus); // Left
    draw_fillrect(hDC, rect.right - 5, (rect.top + nHalfSize) - 1, 5, 3, colorFocus); // Right
  }
  else if (s->m_COLORTYPE == PICKERH) {
    DPOINT pts[3];
    COLORREF brArrow, penArrow;
    RECT rectArrow = rect;
    int iXMiddle = rectArrow.left + RCW(&rectArrow) / 2;
    int iHalfWidth;

    DeflateRect(&rectArrow, 0, RCH(&rectArrow) / 3);
    rectArrow.left = iXMiddle - RCH(&rectArrow) - 1;
    rectArrow.right = iXMiddle + RCH(&rectArrow) + 1;

    iHalfWidth = (RCW(&rectArrow) % 2 != 0) ? (RCW(&rectArrow) - 1) / 2 : RCW(&rectArrow) / 2;

    pts[0].x = rectArrow.left;
    pts[0].y = rectArrow.top;
    pts[1].x = rectArrow.right;
    pts[1].y = rectArrow.top;
    pts[2].x = rectArrow.left + iHalfWidth;
    pts[2].y = rectArrow.bottom + 1;

    brArrow = (colorWhite);
    penArrow = (colorBlack);

    //hDC->SetPolyFillMode(WINDING);
    drawaa_poly(hDC, pts, 3, brArrow, penArrow, 1);
  }
  else if (s->m_COLORTYPE == LUMINANCE) {
    COLORREF pen, br;
    DPOINT pt[3];
    pt[0].x = rect.left;
    pt[0].y = rect.top + nHalfSize;

    pt[1].x = rect.right - 1;
    pt[1].y = rect.top;

    pt[2].x = rect.right - 1;
    pt[2].y = rect.bottom - 1;

    pen = (globalData.clrBtnText);

    br = (s->m_foc ? globalData.clrBtnText : globalData.clrBtnShadow);

    drawaa_poly(hDC, pt, 3, br, pen, 1);
  }

  return 0;
}

double LumFromPoint(const colorpicker_ctrl* s, int nY)
{
  RECT rectClient;
  ASSERT(s->m_COLORTYPE == LUMINANCE);

  rectClient = s->base.rc;

  DeflateRect(&rectClient, 0, DEFAULT_OFFSET_OF_LUMINANCE_BAR);

  nY = MIN(MAX(rectClient.top, nY), rectClient.bottom);
  return((double) rectClient.bottom - nY) / RCH(&rectClient);
}

// this function must be call before the first paint message
// on the picker control
int SetLuminanceBarWidth(colorpicker_ctrl* s, int w)
{
  RECT rectClient;
  rectClient = s->base.rc;

  w = MIN(w, RCW(&rectClient) * 3 / 4);

  s->m_nLumBarWidth = w;
  return 0;
}

int OnKeyDown(colorpicker_ctrl* s, UINT nChar, UINT nRepCnt, UINT nFlags)
{
  const double dblDelta = .05;
  double    dblLum, dblSat, dblHue;
  RGBtoHSL(s->m_color, &dblHue, &dblSat, &dblLum);

  switch (s->m_COLORTYPE) {
  case PICKER: {
    //RECT rectCursorOld = s->m_rcCursorRect;
    //InflateRect(&rectCursorOld, 1, 1);

    switch (nChar) {
    case VK_UP:
      dblSat += dblDelta;
      break;

    case VK_DOWN:
      dblSat -= dblDelta;
      break;

    case VK_LEFT:
      dblHue -= dblDelta;
      break;

    case VK_RIGHT:
      dblHue += dblDelta;
      break;
    }

    dblSat = MIN(1., MAX(0., dblSat));
    dblHue = MIN(1., MAX(0., dblHue));

    if (dblHue != dblHue || dblSat != dblSat) {
      s->m_color = HLStoRGB_ONE(dblHue, dblLum, dblSat);

    }
  }
  break;

  case PICKERH: {
    switch (nChar) {
    case VK_LEFT:
      dblHue -= dblDelta;
      break;

    case VK_RIGHT:
      dblHue += dblDelta;
      break;
    }

    dblHue = MIN(1., MAX(0., dblHue));

    s->m_color = HLStoRGB_ONE(dblHue, dblLum, dblSat);
  }
  break;

  case LUMINANCE: {
    switch (nChar) {
    case VK_UP:
      dblLum += dblDelta;
      break;

    case VK_DOWN:
      dblLum -= dblDelta;
      break;
    }

    dblLum = MIN(1., MAX(0., dblLum));

    s->m_color = HLStoRGB_ONE(dblHue, dblLum, dblSat);
  }
  break;

  case HEX:
  case HEX_GREYSCALE:
  default:
    break;
    // TBD
  }

  return 0;
}

int DrawLuminanceBar(const colorpicker_ctrl* s, HDDC hDC, RECT rectClient)
{
  int y;
  double    dblLum, dblSat, dblHue;
  RGBtoHSL(s->m_color, &dblHue, &dblSat, &dblLum);
  DeflateRect(&rectClient, 0, DEFAULT_OFFSET_OF_LUMINANCE_BAR);

  for (y = rectClient.top; y <= rectClient.bottom; y ++) {
    COLORREF col = HLStoRGB_ONE(dblHue, LumFromPoint(s, y), dblSat);
    DrawRect(hDC, iRECT(0, y, s->m_nLumBarWidth, y + 1), col, 0);
  }

  return 0;
}

int draw_colorpicker_ctrl(const colorpicker_ctrl* s, HDDC hDC, RECT rectClient)
{
  switch (s->m_COLORTYPE) {
  case HEX:
    DrawHex(s, hDC);
    break;

  case HEX_GREYSCALE:
    DrawHex(s, hDC);
    break;

  case CURRENT: {
    COLORREF clrText = _RGB(0, 0, 0);

    int nHalf = RCH(&rectClient) / 2;
    draw_fillrect(hDC, 0, 0, RCW(&rectClient), nHalf, s->m_color);

    // Draw frame
    Draw3dRect(hDC, &rectClient, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);
  }
  break;

  case PICKER:
  case PICKERH:
    DrawPicker(s, hDC, rectClient);
    DrawCursor(s, hDC, s->m_rcCursorRect);
    Draw3dRect(hDC, &rectClient, globalData.clrBtnDkShadow, globalData.clrBtnHilite);
    break;

  case LUMINANCE:
    DrawLuminanceBar(s, hDC, rectClient);

    // Draw marker:
    DrawCursor(s, hDC, s->m_rcCursorRect);
    break;
  }

  return 0;
}


int PointFromLum(colorpicker_ctrl* s, double dblLum)
{
  RECT rectClient;
  ASSERT(s->m_COLORTYPE == LUMINANCE);

  rectClient = s->base.rc;

  DeflateRect(&rectClient, 0, DEFAULT_OFFSET_OF_LUMINANCE_BAR);
  return rectClient.top + (int)((1. - dblLum) * RCH(&rectClient));
}

POINT GetCursorPos1(colorpicker_ctrl* s)
{
  POINT point = iPOINT(0, 0);
  RECT rectClient;
  double    dblLum, dblSat, dblHue;
  RGBtoHSL(s->m_color, &dblHue, &dblSat, &dblLum);
  rectClient = s->base.rc;

  switch (s->m_COLORTYPE) {
  case LUMINANCE:
    point = iPOINT(rectClient.left + s->m_nLumBarWidth + 6, PointFromLum(s, dblLum));
    break;

  case PICKER:
    point = iPOINT((long)((double) RCW(&rectClient) * dblHue), (long)((1. - dblSat) * RCH(&rectClient)));
    break;

  case PICKERH:
    point = iPOINT((long)((double) RCW(&rectClient) * dblHue), CenterPoint(&rectClient).y);
    break;

  case HEX:
  case HEX_GREYSCALE:
  default:
    ASSERT(FALSE);
  }

  return point;
}

RECT GetCursorRect(colorpicker_ctrl* s, POINT point)
{
  RECT rect;

  switch (s->m_COLORTYPE) {
  case PICKER:
    rect = iRECT3(point, iSIZE(PICKER_CURSOR_SIZE, PICKER_CURSOR_SIZE));
    break;

  case PICKERH:
    rect = s->base.rc;
    rect.left = point.x - PICKER_CURSOR_SIZEH / 2;
    rect.right = rect.left + PICKER_CURSOR_SIZEH;
    return rect;

  case LUMINANCE:
    rect = iRECT3(point, iSIZE(LUM_CURSOR_SIZE, LUM_CURSOR_SIZE));
    break;

  case HEX:
  case HEX_GREYSCALE:
  default:
    ASSERT(FALSE);
    SetRectEmpty(&rect);
  }

  OffsetRect(&rect, - RCW(&rect) / 2, - RCH(&rect) / 2);
  s->m_rcCursorRect = rect;
  return rect;
}

int colorpicker_ctrl_event(colorpicker_ctrl* s, const event* e)
{
  UIEVT(e);
  POINT point = e->pt;
  UINT nFlags = e->wp;

  switch (e->msg) {
  case EVENT_INIT:
    if (1) {
    }

    break;

  case WM_MOUSEMOVE:
    if (PtInRect(&s->base.rc, e->pt)) {
      RECT rectClient;
      double    dblLum, dblSat, dblHue;
      RGBtoHSL(s->m_color, &dblHue, &dblSat, &dblLum);
      rectClient = s->base.rc;

      point.x = MIN(MAX(rectClient.left, point.x), rectClient.right);
      point.y = MIN(MAX(rectClient.top, point.y), rectClient.bottom);

      switch (s->m_COLORTYPE) {
      case LUMINANCE: {
        RECT rectCursorOld = s->m_rcCursorRect;
        InflateRect(&rectCursorOld, 1, 1);

        dblLum = LumFromPoint(s, point.y);
        s->m_color = HLStoRGB_ONE(dblHue, dblLum, dblSat);

      }
      break;

      case PICKER: {
        RECT rectCursorOld = s->m_rcCursorRect;
        InflateRect(&rectCursorOld, 1, 1);

        if (nFlags & MK_CONTROL) {
          point.x = point.x;
        }

        if (nFlags & MK_SHIFT) {
          point.y = point.y;
        }

        dblHue = (double) point.x / (double) RCW(&rectClient);
        dblSat = 1. - (double) point.y / RCH(&rectClient);
        s->m_color = HLStoRGB_ONE(dblHue, dblLum, dblSat);

      }
      break;

      case PICKERH: {
        RECT rectCursorOld = s->m_rcCursorRect;
        InflateRect(&rectCursorOld, RCW(&rectCursorOld), RCH(&rectCursorOld));

        if (nFlags & MK_CONTROL) {
          point.x = point.x;
        }

        dblHue = (double) point.x / (double) RCW(&rectClient);
        s->m_color = HLStoRGB_ONE(dblHue, dblLum, dblSat);

      }
      break;

      case HEX:
      case HEX_GREYSCALE:
        if (1) {
          int i;
          COLORREF clr;

          for (i = 0; i < s->m_arCellsSize; i ++) {
            if (cellobj_hittest(s->m_arCells + i, e->pt)) {
              clr = s->m_arCells[i].m_CellColor;

              if (s->m_color != clr) {
                s->m_color = clr;
                force_redraw(e);
              }
            }
          }
        }

        break;
      }
    }

    break;

  case WM_SIZE:
    if (1) {
      switch (s->m_COLORTYPE) {
      case HEX:
        CreateHexagon(s, s->base.rc);
        break;

      case HEX_GREYSCALE:
        CreateHexGreyScaleBar(s, s->base.rc);
        break;
      }
    }

    break;

  case EVENT_EXIT:
    if (e->sender == s) {
      printf("EVENT_EXIT %x\n", e->sender);
      PostQuitMessage(0);
    }

    break;

  }

  return 0;
}

