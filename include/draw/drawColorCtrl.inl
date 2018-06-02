#include "paintmanager.h"
#include "ExtPaintManager.inl"
#define NUM_LEVELS 7
#define TAN30 0.57735026918962F
#define YOFFSET (1.5F * TAN30)
//#define PI 3.14159265358979
#define NUM_HEX_FULL_CELLS 144
#define NUM_HEX_GRAY_CELLS 17
#define HEX_DIST_GAP 10
#define NUM_RGB_SINGLE_CELLS 256
#define NUM_RGB_FULL_CELLS (NUM_RGB_SINGLE_CELLS*3)
static const float cfxFOOffset[] = {
  - 0.5, - 1.0, - 0.5, 0.5, 1.0, 0.5
};
static const float cfyFOOffset[] = {
  YOFFSET, 0.0, - YOFFSET, - YOFFSET, 0.0, YOFFSET
};
static int GetAngleFromPoint(int nX, int nY)
{
  double dAngle = atan2((double)nY, (double)nX);
  return (int)(dAngle * 180.0 / PI);
}
typedef struct __PROF_UIS_API _cell_t {
  COLORREF m_clr;
  RECT m_rcPosition;
  int m_nBorderSize;
  POINT m_vPoints[6];
  BOOL m_bAtFirstType;
} cell_t;
static int MoreNear(float fNear)
{
  int nRound = (int)fabs(fNear);
  nRound = (fabs(fNear) - (float)nRound >= 0.5) ? nRound + 1 : nRound;
  return (fNear < 0) ? - nRound : nRound;
};
void AdjustCellPosition(cell_t* c, int x, int y, int nWidth)
{
  float nSideLength = (float)((float)nWidth * 0.57735026918962F);
  c->m_vPoints[0] = iPOINT(MoreNear(x - (float)(nWidth / 2)), MoreNear(y - (float)(nSideLength / 2)));
  c->m_vPoints[1] = iPOINT(MoreNear((float)x), MoreNear(y - (float)(nWidth / 2)));
  c->m_vPoints[2] = iPOINT(MoreNear(x + (float)(nWidth / 2)), MoreNear(y - (float)(nSideLength / 2)));
  c->m_vPoints[3] = iPOINT(MoreNear(x + (float)(nWidth / 2)), MoreNear(y + (float)(nSideLength / 2)) + 1);
  c->m_vPoints[4] = iPOINT(MoreNear((float)x), MoreNear(y + (float)(nWidth / 2)) + 1);
  c->m_vPoints[5] = iPOINT(MoreNear(x - (float)(nWidth / 2)), MoreNear(y + (float)(nSideLength / 2)) + 1);
};
int GenerateCell(cell_t* m_vCells, int cx, int cy)
{
  SIZE m_sizeTotal = {cx, cy};
  int nCellSizeByX = m_sizeTotal.w / (2 * (NUM_LEVELS + 1)) + 2;
  int nCellSizeByY = (m_sizeTotal.h - HEX_DIST_GAP) / (2 * (NUM_LEVELS + 1)) + 2;
  int nLevel, nCellSize = MIN(nCellSizeByX, nCellSizeByY);
  int x = m_sizeTotal.w / 2;
  int y = (m_sizeTotal.h - nCellSize * 2 - HEX_DIST_GAP) / 2;
  int nNumber = 1, nSide, nCell;
  // white hexagon at center
  m_vCells[0].m_clr = _RGB(255, 255, 255);
  m_vCells[0].m_bAtFirstType = TRUE;
  AdjustCellPosition(m_vCells, x, y, nCellSize);

  for (nLevel = 1; nLevel < NUM_LEVELS; nLevel++) {
    // walk through all levels
    // level start position
    int nPosX = x + (nCellSize * nLevel);
    int nPosY = y;

    for (nSide = 0; nSide < NUM_LEVELS - 1; nSide++) {
      // walk through all sided
      // set the deltas for the side
      int nDx = (int)((float)(nCellSize) * cfxFOOffset[nSide]);
      int nDy = (int)((float)(nCellSize) * cfyFOOffset[nSide]);

      for (nCell = 0; nCell < nLevel; nCell++) {
        // walk through all cells at one side
        int nAngle = GetAngleFromPoint(nPosX - x, nPosY - y);
        double L = 1. *(NUM_LEVELS - nLevel) / NUM_LEVELS + .1;
        m_vCells[nNumber].m_clr = (stat_GetRGBFromHLSExtend((float)nAngle, L, 1.0F));
        m_vCells[nNumber].m_bAtFirstType = TRUE;
        AdjustCellPosition(&m_vCells[nNumber], nPosX, nPosY, nCellSize);
        nNumber++;
        // offset the position
        nPosX += nDx;
        nPosY += nDy;
      } // // walk through all cells at one side
    } // walk through all sided
  } // walk through all levels

  {
    int xGrayPos = (m_sizeTotal.w - (nCellSize * 3 + nCellSize / 2 + (nCellSize * (NUM_HEX_GRAY_CELLS - 2)) / 2)) / 2;
    int yGrayCenter = m_sizeTotal.h - nCellSize * 1 - HEX_DIST_GAP / 2;
    int nTmp = (int)((float)(nCellSize) / 2.3);
    int yUpper = yGrayCenter - nTmp;
    int yLower = yGrayCenter + nTmp;
    int nGrayColorPos = 0;
    COLORREF clrGray;

    for (nCell = 0; nCell < NUM_HEX_GRAY_CELLS; nCell++, nNumber++, nGrayColorPos += 255 / NUM_HEX_GRAY_CELLS) {
      // walk through all gray-scale positions
      if (nCell == 0) {
        // if first - white cell
        m_vCells[nNumber].m_clr = _RGB(255, 255, 255);
        m_vCells[nNumber].m_bAtFirstType = TRUE;
        AdjustCellPosition(&m_vCells[nNumber], xGrayPos, yGrayCenter, nCellSize * 2);
        xGrayPos += nCellSize * 2;
        continue;
      } // if first - white cell

      if (nCell == NUM_HEX_GRAY_CELLS - 1) {
        // if last - black cell
        xGrayPos += (nCellSize * 3) / 2 - 1;
        m_vCells[nNumber].m_clr = _RGB(0, 0, 0);
        m_vCells[nNumber].m_bAtFirstType = TRUE;
        AdjustCellPosition(&m_vCells[nNumber], xGrayPos, yGrayCenter, nCellSize * 2);
        break;
      } // if last - black cell

      clrGray = _RGB(255 - nGrayColorPos, 255 - nGrayColorPos, 255 - nGrayColorPos);
      m_vCells[nNumber].m_clr = clrGray;
      m_vCells[nNumber].m_bAtFirstType = TRUE;
      AdjustCellPosition(&m_vCells[nNumber], xGrayPos, ((nCell & 1) != 0) ? yUpper : yLower, nCellSize);
      xGrayPos += nCellSize / 2;
    } // walk through all gray-scale positions
  }
  return nNumber;
}
// NUM_RGB_SINGLE_CELLS
int grayscale_mixer_data(cell_t* m_vCells, int cx, int cy)
{
  SIZE m_sizeTotal = {cx, cy};
  // m_vCells.SetSize(NUM_RGB_SINGLE_CELLS);
  int nCellWidth = m_sizeTotal.w / 3 - 20;
  int nCellCenter = m_sizeTotal.w / 2;
  int nWorkHeight = (m_sizeTotal.h - 20);
  int nWorkY1 = 10;
  int i, nCellHeight = nWorkHeight / NUM_RGB_SINGLE_CELLS;
  RECT rc;

  if (nCellHeight < 2) {
    nCellHeight = 2;
  }

  for (i = 0; i < NUM_RGB_SINGLE_CELLS; i++) {
    m_vCells[i].m_clr = _RGB(i, i, i);
    m_vCells[i].m_bAtFirstType = FALSE;
    m_vCells[i].m_nBorderSize = 0;
    rc.top = nWorkY1 + (nWorkHeight * i) / NUM_RGB_SINGLE_CELLS;
    rc.bottom = rc.top + nCellHeight;
    rc.left = nCellCenter - nCellWidth / 2;
    rc.right = rc.left + nCellWidth;
    m_vCells[i].m_rcPosition = rc;

    if (i > 0 && m_vCells[i].m_rcPosition.top > m_vCells[i - 1].m_rcPosition.bottom) {
      int asdf = 0;
    }
  }

  return NUM_RGB_SINGLE_CELLS;
}
int rgb_mixer_data(cell_t* m_vCells, int cx, int cy)
{
  SIZE m_sizeTotal = {cx, cy};
  //m_vCells.SetSize(NUM_RGB_FULL_CELLS);
  // }
  int nCellWidth = m_sizeTotal.w / 3 - 20;
  int nCellCenter = m_sizeTotal.w / 2;
  int nWorkHeight = (m_sizeTotal.h - 20);
  int nWorkY1 = 10; // + nWorkHeight/2;
  int nCellHeight = nWorkHeight / NUM_RGB_SINGLE_CELLS;
  RECT rcG, rcR, rcB;
  int i;

  if (nCellHeight < 2) {
    nCellHeight = 2;
  }

  for (i = 0; i < NUM_RGB_SINGLE_CELLS; i++) {
    m_vCells[i].m_clr = _RGB(i, 0, 0);
    m_vCells[i].m_bAtFirstType = FALSE;
    m_vCells[i].m_nBorderSize = 0;
    m_vCells[i + NUM_RGB_SINGLE_CELLS].m_clr = _RGB(0, i, 0);
    m_vCells[i + NUM_RGB_SINGLE_CELLS].m_bAtFirstType = FALSE;
    m_vCells[i + NUM_RGB_SINGLE_CELLS].m_nBorderSize = 0;
    m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_clr = _RGB(0, 0, i);
    m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_bAtFirstType = FALSE;
    m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_nBorderSize = 0;
    rcG.top = nWorkY1 + (nWorkHeight * i) / NUM_RGB_SINGLE_CELLS;
    rcG.bottom = rcG.top + nCellHeight;
    rcG.left = nCellCenter - nCellWidth / 2;
    rcG.right = rcG.left + nCellWidth;
    rcR = (rcG), rcB = (rcG);
    OffsetRect(&rcR, - nCellWidth - 20, 0);
    OffsetRect(&rcB, nCellWidth + 20, 0);
    m_vCells[i].m_rcPosition = rcR;
    m_vCells[i + NUM_RGB_SINGLE_CELLS].m_rcPosition = rcG;
    m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_rcPosition = rcB;
  }

  return NUM_RGB_FULL_CELLS;
}
#define DEFAULT_RGB_CUBE_GAP 5
#define DEF_RGB_CUBE_HL 0
#define DEF_RGB_CUBE_TRANSPARENT _RGB(255,255,255)
int rgb_cube_data(HDDC dc0, int cx, int cy)
{
  TDrawDC dc[1];
  SIZE sizeCube, m_sizeTotal = {cx, cy};
  // if needs re-create anything
  RECT m_rcCube = {0, 0, m_sizeTotal.w, m_sizeTotal.h};
  int nMetric, m_nSideGap = DEFAULT_RGB_CUBE_GAP;
  POINT m_ptCubeR, m_ptCubeG, m_ptCubeB, m_ptCubeC, m_ptClrR, m_ptClrG, m_ptClrB, m_ptClrCurr, m_ptPlainRG, m_ptPlainRB, m_ptPlainGB;
  DeflateRect(&m_rcCube, m_nSideGap, m_nSideGap);
  sizeCube = iSIZE(RCWIDTH(&m_rcCube), RCHEIGHT(&m_rcCube));

  if (sizeCube.w > sizeCube.h) {
    DeflateRect(&m_rcCube, (sizeCube.w - sizeCube.h) / 2, 0);
  }

  if (sizeCube.h > sizeCube.w) {
    DeflateRect(&m_rcCube, 0, (sizeCube.h - sizeCube.w) / 2);
  }

  sizeCube = iSIZE(RCWIDTH(&m_rcCube), RCHEIGHT(&m_rcCube));
  ClipDC1(dc, dc0, &m_rcCube);

  if ((sizeCube.w & 0x3) != 0) {
    m_rcCube.right--;
  }

  if ((sizeCube.w & 0x3) != 0) {
    m_rcCube.right--;
  }

  if ((sizeCube.w & 0x3) != 0) {
    m_rcCube.right--;
  }

  if ((sizeCube.h & 0x3) != 0) {
    m_rcCube.bottom--;
  }

  if ((sizeCube.h & 0x3) != 0) {
    m_rcCube.bottom--;
  }

  if ((sizeCube.h & 0x3) != 0) {
    m_rcCube.bottom--;
  }

  sizeCube = iSIZE(RCWIDTH(&m_rcCube), RCHEIGHT(&m_rcCube));
  nMetric = sizeCube.h / 4;
  //sizeCube = m_rcCube.Size();
  sizeCube = iSIZE(RCWIDTH(&m_rcCube), RCHEIGHT(&m_rcCube));
  m_ptCubeC = iPOINT(RCCENTERX(&m_rcCube), RCCENTERY(&m_rcCube));
  m_ptCubeR.x = m_ptCubeC.x;
  m_ptCubeR.y = m_rcCube.top;
  m_ptCubeG.x = m_rcCube.left;
  m_ptCubeG.y = m_rcCube.bottom - nMetric;
  m_ptCubeB.x = m_rcCube.right;
  m_ptCubeB.y = m_rcCube.bottom - nMetric;
  {
    int nBPP = stat_GetBPP();
    int nStep = (nBPP <= 8) ? 4 : 1;
    POINT ptStart = iPOINT(sizeCube.w / 2, sizeCube.h / 2);
    int nHalf = sizeCube.h / 2;
    int nR, nG, nB;
    COLORREF clr;

    //TFillRect4(dc, 0, 0, sizeCube.w, sizeCube.h, DEF_RGB_CUBE_TRANSPARENT);
    for (nR = 0; nR <= nHalf; nR++) {
      for (nG = 0; nG <= nHalf; nG++) {
        POINT pt = (ptStart);
        pt.x -= nG + 1;
        pt.y += nG / 2;
        pt.y -= nR;
        clr = _RGB((nR * 255) / nHalf, (nG * 255) / nHalf, DEF_RGB_CUBE_HL);

        if (nBPP <= 8) {
          pt.x -= nStep / 2, pt.y -= nStep / 2;
          TFillRect4(dc, pt.x, pt.y, pt.x + nStep, pt.y + nStep, clr);
        }
        else {
          _SetPixel(dc, pt.x, pt.y, clr);
        }
      } // for( nG = 0; nG < nHalf; nG++ )
    } // for( nR = 0; nR < nHalf; nR++ )

    for (nR = 0; nR <= nHalf; nR++) {
      for (nB = 0; nB <= nHalf; nB++) {
        POINT pt = (ptStart);
        pt.x += nB;
        pt.y += nB / 2;
        pt.y -= nR;
        clr = _RGB((nR * 255) / nHalf, DEF_RGB_CUBE_HL, (nB * 255) / nHalf);

        if (nBPP <= 8) {
          pt.x -= nStep / 2, pt.y -= nStep / 2;
          TFillRect4(dc, pt.x, pt.y, pt.x + nStep, pt.y + nStep, clr);
        }
        else {
          _SetPixel(dc, pt.x, pt.y, clr);
        }
      } // for( nB = 0; nB <= nHalf; nB++ )
    } // for( nR = 0; nR < nHalf; nR++ )

    for (nB = 0; nB <= nHalf; nB++) {
      for (nG = 0; nG <= nHalf; nG++) {
        POINT pt = (ptStart);
        pt.x -= nG + 1;
        pt.y += nG / 2;
        pt.x += nB + 1;
        pt.y += nB / 2;
        clr = _RGB(DEF_RGB_CUBE_HL, (nG * 255) / nHalf, (nB * 255) / nHalf);

        if (nBPP <= 8) {
          pt.x -= nStep / 2, pt.y -= nStep / 2;
          TFillRect4(dc, pt.x, pt.y, pt.x + nStep, pt.y + nStep, clr);
        }
        else {
          _SetPixel(dc, pt.x, pt.y, clr);
          _SetPixel(dc, pt.x - 1, pt.y, clr);
        }
      } // for( nG = 0; nG <= nHalf; nG++ )
    } // for( nB = 0; nB <= nHalf; nB++ )

    //RecalcLayout();
    {
      int nHalf;
      DWORD m_clr = 0;
      sizeCube = iSIZE(RCWIDTH(&m_rcCube), RCHEIGHT(&m_rcCube));

      if (sizeCube.w < 1 || sizeCube.h < 1) {
        return 0;
      }

      m_ptClrR = m_ptCubeC;
      m_ptClrG = m_ptCubeC;
      m_ptClrB = m_ptCubeC;
      m_ptClrCurr = m_ptCubeC;
      nHalf = sizeCube.h / 2;
      m_ptClrR.y -= (GetRV(m_clr) * nHalf) / 255;
      m_ptClrG.x -= (GetGV(m_clr) * nHalf) / 255;
      m_ptClrG.y += (GetGV(m_clr) * nHalf) / 255 / 2;
      m_ptClrB.x += (GetBV(m_clr) * nHalf) / 255;
      m_ptClrB.y += (GetBV(m_clr) * nHalf) / 255 / 2;
      m_ptPlainRG.x = m_ptClrG.x;
      m_ptPlainRG.y = m_ptClrR.y - (m_ptCubeC.y - m_ptClrG.y);
      m_ptPlainRB.x = m_ptClrB.x;
      m_ptPlainRB.y = m_ptClrR.y - (m_ptCubeC.y - m_ptClrB.y);
      m_ptPlainGB.x = m_ptClrG.x - (m_ptCubeC.x - m_ptClrB.x);
      m_ptPlainGB.y = m_ptClrG.y - (m_ptCubeC.y - m_ptClrB.y);
      m_ptClrCurr = m_ptPlainGB;
      m_ptClrCurr.y -= m_ptCubeC.y - m_ptClrR.y;
    }
    {
      int _x0, _y0;
      DWORD _clr;
      RECT rcDrag;
      SIZE m_sizeHalfSelBox;
      *dc = *dc0;
      m_sizeHalfSelBox.w = m_sizeHalfSelBox.h = 4;
      sizeCube = iSIZE(RCWIDTH(&m_rcCube), RCHEIGHT(&m_rcCube));
#define TMoveToPt(_DC, _PT) TMoveTo(_DC, (_PT).x, (_PT).y)
#define TLineToPt(_DC, _PT) TLineTo(_DC, (_PT).x, (_PT).y, _clr)
      _clr = _RGB(0, 0, 0);
      // draw axes
      TMoveToPt(dc, m_ptCubeR);
      TLineToPt(dc, m_ptCubeC);
      TLineToPt(dc, m_ptCubeB);
      TMoveToPt(dc, m_ptCubeG);
      TLineToPt(dc, m_ptCubeC);
      // draw value lines
      _clr = _RGB(255, 255, 255);
      TMoveToPt(dc, m_ptPlainRG);
      TLineToPt(dc, m_ptClrG);
      TLineToPt(dc, m_ptPlainGB);
      TLineToPt(dc, m_ptClrB);
      TLineToPt(dc, m_ptPlainRB);
      TLineToPt(dc, m_ptClrR);
      TLineToPt(dc, m_ptPlainRG);
      TLineToPt(dc, m_ptClrCurr);
      TLineToPt(dc, m_ptPlainRB);
      TMoveToPt(dc, m_ptClrCurr);
      TLineToPt(dc, m_ptPlainGB);
      // draw value boxes
      SetRect(&rcDrag, m_ptClrR.x, m_ptClrR.y, m_ptClrR.x, m_ptClrR.y);
      InflateRect4(&rcDrag, m_sizeHalfSelBox.w, m_sizeHalfSelBox.h, m_sizeHalfSelBox.w + 1, m_sizeHalfSelBox.h + 1);
      // TDraw3DRect(dc, &rcDrag, _RGB(255,255,255), _RGB(255,255,255) );
      // rcDrag.InflateRect(1,1);
      // TDraw3DRect(dc, &rcDrag, _RGB(0,0,0), _RGB(0,0,0) );
      // rcDrag.DeflateRect(2,2);
      // TDraw3DRect(dc, &rcDrag, _RGB(0,0,0), _RGB(0,0,0) );
      TDraw3DRect(dc, &rcDrag, _RGB(0, 0, 0), _RGB(0, 0, 0));
      DeflateRect(&rcDrag, 1, 1);
      TDraw3DRect(dc, &rcDrag, _RGB(255, 255, 255), _RGB(255, 255, 255));
      SetRect(&rcDrag, m_ptClrG.x, m_ptClrG.y, m_ptClrG.x, m_ptClrG.y);
      InflateRect4(&rcDrag, m_sizeHalfSelBox.w, m_sizeHalfSelBox.h, m_sizeHalfSelBox.w, m_sizeHalfSelBox.h);
      // TDraw3DRect(dc, &rcDrag, _RGB(255,255,255), _RGB(255,255,255) );
      // rcDrag.InflateRect(1,1);
      // TDraw3DRect(dc, &rcDrag, _RGB(0,0,0), _RGB(0,0,0) );
      // rcDrag.DeflateRect(2,2);
      // TDraw3DRect(dc, &rcDrag, _RGB(0,0,0), _RGB(0,0,0) );
      TDraw3DRect(dc, &rcDrag, _RGB(0, 0, 0), _RGB(0, 0, 0));
      DeflateRect(&rcDrag, 1, 1);
      TDraw3DRect(dc, &rcDrag, _RGB(255, 255, 255), _RGB(255, 255, 255));
      SetRect(&rcDrag, m_ptClrB.x, m_ptClrB.y, m_ptClrB.x, m_ptClrB.y);
      InflateRect4(&rcDrag, m_sizeHalfSelBox.w, m_sizeHalfSelBox.h, m_sizeHalfSelBox.w, m_sizeHalfSelBox.h);
      // TDraw3DRect(dc, &rcDrag, _RGB(255,255,255), _RGB(255,255,255) );
      // rcDrag.InflateRect(1,1);
      // TDraw3DRect(dc, &rcDrag, _RGB(0,0,0), _RGB(0,0,0) );
      // rcDrag.DeflateRect(2,2);
      // TDraw3DRect(dc, &rcDrag, _RGB(0,0,0), _RGB(0,0,0) );
      TDraw3DRect(dc, &rcDrag, _RGB(0, 0, 0), _RGB(0, 0, 0));
      DeflateRect(&rcDrag, 1, 1);
      TDraw3DRect(dc, &rcDrag, _RGB(255, 255, 255), _RGB(255, 255, 255));
    }
  }
  return 0;
}
int cmy_mixer_data(cell_t* m_vCells, int cx, int cy)
{
  SIZE m_sizeTotal = {cx, cy};
  // if( !bOnlySizeChanged )
  // {
  // ASSERT( m_vCells.empty() );
  //m_vCells.SetSize(NUM_RGB_FULL_CELLS);
  // }
  int nCellWidth = m_sizeTotal.w / 3 - 20;
  int nCellCenter = m_sizeTotal.w / 2;
  int nWorkHeight = (m_sizeTotal.h - 20);
  int nWorkY1 = 10; // + nWorkHeight/2;
  int nCellHeight = nWorkHeight / NUM_RGB_SINGLE_CELLS;
  RECT rcM, rcC, rcY;
  int i;

  if (nCellHeight < 2) {
    nCellHeight = 2;
  }

  for (i = 0; i < NUM_RGB_SINGLE_CELLS; i++) {
    m_vCells[i].m_clr = stat_CMYtoRGB(_RGB(i, 0, 0));
    m_vCells[i].m_bAtFirstType = FALSE;
    m_vCells[i].m_nBorderSize = 0;
    m_vCells[i + NUM_RGB_SINGLE_CELLS].m_clr = stat_CMYtoRGB(_RGB(0, i, 0));
    m_vCells[i + NUM_RGB_SINGLE_CELLS].m_bAtFirstType = FALSE;
    m_vCells[i + NUM_RGB_SINGLE_CELLS].m_nBorderSize = 0;
    m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_clr = stat_CMYtoRGB(_RGB(0, 0, i));
    m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_bAtFirstType = FALSE;
    m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_nBorderSize = 0;
    rcM.top = nWorkY1 + (nWorkHeight * i) / NUM_RGB_SINGLE_CELLS;
    rcM.bottom = rcM.top + nCellHeight;
    rcM.left = nCellCenter - nCellWidth / 2;
    rcM.right = rcM.left + nCellWidth;
    rcC = (rcM), rcY = (rcM);
    OffsetRect(&rcC, - nCellWidth - 20, 0);
    OffsetRect(&rcY, nCellWidth + 20, 0);
    m_vCells[i].m_rcPosition = rcC;
    m_vCells[i + NUM_RGB_SINGLE_CELLS].m_rcPosition = rcM;
    m_vCells[i + NUM_RGB_SINGLE_CELLS * 2].m_rcPosition = rcY;
  }

  return NUM_RGB_FULL_CELLS;
}
void DrawCell(HDDC dc, cell_t* c, BOOL bFocused)
{
  if (!c->m_bAtFirstType) {
    // draw rectangular
    if (bFocused) {
      RECT rcPos = (c->m_rcPosition);
      InflateRect(&rcPos, 3, 3);
      TFillRect(dc, &rcPos, _RGB(0, 0, 0));
      DeflateRect(&rcPos, 1, 1);
      TFillRect(dc, &rcPos, _RGB(255, 255, 255));
      DeflateRect(&rcPos, 1, 1);
      TFillRect(dc, &rcPos, _RGB(0, 0, 0));
      TFillRect(dc, &c->m_rcPosition, c->m_clr);
    } // if( bFocused )
    else {
      TFillRect(dc, &c->m_rcPosition, c->m_clr);
    } // else from if( bFocused )
  } // draw rectangular
  else {
    // draw hexagonal
    if (bFocused) {
      int i;
      POINT vPoints[6];

      for (i = 0; i < 6; i++) {
        vPoints[i] = c->m_vPoints[i];
      }

      vPoints[0].x -= 2;
      vPoints[0].y -= 1;
      vPoints[5].x -= 2;
      vPoints[5].y += 1;
      vPoints[2].x += 2;
      vPoints[2].y -= 1;
      vPoints[3].x += 2;
      vPoints[3].y += 1;
      vPoints[1].y -= 2;
      vPoints[4].y += 2;
      DrawPoly(dc, vPoints, 6, 0, 1);
      DrawPoly(dc, c->m_vPoints, 6, 0, 1);
    } // if( bFocused )
    else {
      ScanFill(dc, c->m_vPoints, 6, c->m_clr);
    }
  } // draw hexagonal

  // resrore DC
}
#define DEFAULT_LUMINANCE 0.50f
#define DEFAULT_LUM_BAR_HEIGHT 20
#define DEFAULT_HSL_AREA_GAP 4
#define DEF_HSL_TRANSPARENT _RGB(255,255,255)
#define __SEL_BOX_DX 6
#define __SEL_BOX_DY 6
void DrawCrossAt(HDDC dc, int x, int y, int cx, int cy)
{
  RECT rc = iRECT(x, y, x + cx, y + cy);
  OffsetRect(&rc, -cx / 2, -cy / 2);
  TDraw3DRect(dc, &rc, _RGB(255, 255, 255), _RGB(255, 255, 255));
  InflateRect(&rc, 1, 1);
  TDraw3DRect(dc, &rc, _RGB(0, 0, 0), _RGB(0, 0, 0));
}
int hsl_panel_data(HDDC dc, int cx, int cy)
{
  SIZE m_sizeTotal = {cx, cy};
  BOOL bRegen = FALSE;
  POINT ptDrawCalc, m_ptMousePos = {100, 100}; // position of the picker
  SIZE m_sizeColorPicker, m_sizeColorSlider, sz = { __SEL_BOX_DX, __SEL_BOX_DY };
  int m_nLumBarDy = DEFAULT_LUM_BAR_HEIGHT;
  int m_nAreaGap = DEFAULT_HSL_AREA_GAP;
  DWORD m_clr = _RGB(255, 255, 255);
  double m_fCurrLuminance, m_fCurrSaturation, m_fCurrHue;
  int i, j, nBPP = stat_GetBPP();
  int nStep = (nBPP <= 8) ? 4 : 1;
  COLORREF clr;
  int maxi = m_sizeTotal.w - m_nAreaGap * 2;
  int maxj = m_sizeTotal.h - m_nAreaGap * 3;
  m_sizeColorPicker.w = maxi;
  m_sizeColorPicker.h = maxj - m_nLumBarDy;
  m_sizeColorSlider.w = maxi;
  m_sizeColorSlider.h = m_nLumBarDy;
  m_sizeTotal.w = maxi;
  m_sizeTotal.h = maxj;
  stat_RGBtoHSL(m_clr, &m_fCurrHue, &m_fCurrSaturation, &m_fCurrLuminance);

  //void hsl_panel_data_GeneratePicker(HDDC dc, CPalette *pPalette)
  if (0) {
    for (j = 0; j < m_sizeColorPicker.h; j++) {
      for (i = 0; i < m_sizeColorPicker.w; i++) {
        clr = stat_HLStoRGB((double)(i) / (double)(m_sizeColorPicker.w), DEFAULT_LUMINANCE, (double)(m_sizeColorPicker.h - j - 1) / (double)(m_sizeColorPicker.h));

        if (nBPP <= 8) {
          int x = i - nStep / 2, y = j - nStep / 2;
          TFillRect4(dc, x, y, x + nStep, y + nStep, clr);
        }
        else {
          _SetPixel(dc, i, j, clr);
        }
      }
    }

    for (j = m_sizeColorSlider.w; j > 0; j -= nStep) {
      double fLuminance = (double)(j - 1) / (double)(m_sizeColorSlider.w);
      clr = stat_HLStoRGB(m_fCurrHue, fLuminance, m_fCurrSaturation);

      if (nBPP <= 8) {
        int x = j - nStep, y = 0;
        TFillRect4(dc, x, y, x + nStep, y + m_sizeColorSlider.h, clr);
      }
      else {
        TFillRect4(dc, j - 1, 0, 1, m_sizeColorSlider.h, clr);
      }
    }

    ptDrawCalc = m_ptMousePos;
    PTOFFSET(ptDrawCalc, m_nAreaGap, m_nAreaGap);
    DrawCrossAt(dc, ptDrawCalc.x, ptDrawCalc.y, sz.w, sz.h); // draw a rectangle on picker
    sz.w = __SEL_BOX_DX;
    sz.h = m_sizeColorSlider.h;
    ptDrawCalc = iPOINT((int)(m_fCurrLuminance * m_sizeColorSlider.w) + m_nAreaGap, m_sizeColorPicker.h + m_sizeColorSlider.h / 2 + m_nAreaGap * 2);
    DrawCrossAt(dc, ptDrawCalc.x, ptDrawCalc.y, sz.w, sz.h); //draw rectangle on luminance bar
  }
  else {
    for (i = 0; i < m_sizeColorPicker.w; i += nStep) {
      for (j = 0; j < m_sizeColorPicker.h; j += nStep) {
        clr = stat_HLStoRGB(m_fCurrHue, (double)(m_sizeColorPicker.h - j - 1) / (double)(m_sizeColorPicker.h), (double)(i) / (double)(m_sizeColorPicker.w));

        if (nBPP <= 8) {
          int x = i - nStep / 2, y = j - nStep / 2;
          TFillRect4(dc, x, y, x + nStep, y + nStep, clr);
        }
        else {
          _SetPixel(dc, i, j, clr);
        }
      }
    }

    for (j = 0; j < m_sizeColorSlider.w; j += nStep) {
      double fHue = (double)(j) / (double)(m_sizeColorSlider.w);
      COLORREF clr = stat_HLStoRGB(fHue, DEFAULT_LUMINANCE, 1.0);

      if (nBPP <= 8) {
        int x = j, y = 0;
        TFillRect4(dc, x, y, x + nStep, y + m_sizeColorSlider.h, clr);
      }
      else {
        TFillRect4(dc, j, 0, 1, m_sizeColorSlider.h, clr);
      }
    }

    ptDrawCalc.x = (int)(m_fCurrSaturation * (double)(m_sizeColorPicker.w)) + m_nAreaGap;
    ptDrawCalc.y = m_sizeColorPicker.h - (int)(m_fCurrLuminance * (double)(m_sizeColorPicker.h)) + m_nAreaGap;
    // draw a rectangle on picker
    DrawCrossAt(dc, ptDrawCalc.x, ptDrawCalc.y, sz.w, sz.h);
    sz.w = __SEL_BOX_DX;
    sz.h = m_sizeColorSlider.h;
    ptDrawCalc = iPOINT((int)(m_fCurrHue * m_sizeColorSlider.w) + m_nAreaGap, m_sizeColorPicker.h + m_sizeColorSlider.h / 2 + m_nAreaGap * 2);
    //draw rectangle on luminance bar
    DrawCrossAt(dc, ptDrawCalc.x, ptDrawCalc.y, sz.w, sz.h);
  }

  return 0;
}
int DrawColorCtrl(HDDC dc)
{
  cell_t m_vCells[NUM_RGB_FULL_CELLS];
  int m_vCellsLen, it = 0, itFocus0;
  m_vCellsLen = grayscale_mixer_data(m_vCells, dc->w, dc->h);
  m_vCellsLen = rgb_mixer_data(m_vCells, dc->w, dc->h);
  m_vCellsLen = GenerateCell(m_vCells, dc->w, dc->h);
  m_vCellsLen = cmy_mixer_data(m_vCells, dc->w, dc->h);
  itFocus0 = m_vCellsLen;

  for (; it < m_vCellsLen; ++it) {
    cell_t* _c = &m_vCells[it];
    DrawCell(dc, _c, 0);

    if (itFocus0 == m_vCellsLen) {
      itFocus0 = it;
      continue;
    }
  }

  if (itFocus0 != m_vCellsLen) {
    DrawCell(dc, &m_vCells[itFocus0], TRUE);
  }

  //rgb_cube_data(dc, dc->w, dc->h);
  hsl_panel_data(dc, dc->w, dc->h);
  return 0;
}

