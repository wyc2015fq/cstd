//#define FONT_BOLD (1<<28)
//#define FONT_ITALIC (1<<29)
//#define FONT_ANTIALIAS (1<<30)
// 中文字体绘制

// 根据坐标获取字符索引
int FontGetCharIndexFromPosX(TCHAR* s, int sLen, int pixel_x)
{
  int x = 0, y = 0, w;
  TCHAR* e = s + sLen;
  TCHAR* s2 = s;
  TCHAR* s0 = s;
  HDC hMemDC = GetMemDC();
  MAT2 mat2 = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
  GLYPHMETRICS gm;

  if (pixel_x < 0) {
    return 0;
  }

  for (; s < e; s = s2) {
    uint nChar = nextChar(s, &s2);
    GetGlyphOutline(hMemDC, nChar, 0, &gm, 0, NULL, &mat2);
    w = gm.gmCellIncX;

    if (pixel_x <= x + w / 2) {
      return (s - s0);
    }

    x += gm.gmCellIncX;
  }

  return sLen;
}
#define TTextOut(hDC, x, y, text) draw_text_impl(hDC, (text).s, (text).l, x, y, _clr, 0, 0, 0)
#define TTextOut2(hDC, x, y, text) draw_text_impl(hDC, (text), strlen(text), x, y, _clr, 0, 0, 0)
#define TDrawText(hDC, lpString, lpRect, uFormat) drawaa_text(hDC, (lpRect)->left, (lpRect)->top, RCWIDTH(lpRect), RCHEIGHT(lpRect), lpString, strlen(lpString), uFormat, _RGB(0,0,0))
#define TDrawText2(hDC, lpString, nLen, lpRect, uFormat, clr) DrawText11(hDC, (lpRect)->left, (lpRect)->top, RCWIDTH(lpRect), RCHEIGHT(lpRect), lpString, nLen, uFormat|FONT_ANTIALIAS, clr)

typedef struct draw_text_t {
  HDDC hDC;
  const char* s;
  int slen;
  POINT pt0;
  POINT pthit;
  DWORD clr, crbk; // 文字颜色 背景颜色
  DWORD* clrs;
  DWORD* crbks;
  SIZE sz;
  int max_cx;
  int charhit;// 输出 pthit 在哪个字符上
  int fmt;
  //int angle; // 0 向右 90 向上 180 向左 270 向下
} draw_text_t;

int draw_text_impl1(draw_text_t* d)
{
  HDDC hDC = d->hDC;
  int slen = d->slen;
  const char* s = d->s;
  const char* s0 = d->s;
  const char* e = s + slen;
  char* s2;
  RECT clip = hDC ? (hDC->clip ? *hDC->clip : iRECT(0, 0, hDC->w, hDC->h)) : iRECT(0, 0, 0, 0);
  int x0 = d->pt0.x, y0 = d->pt0.y;
  int x00 = x0, y00 = y0, max_x0 = x0;
  font_t* font;
  const uchar* mapA = 0;
  int depth;
  // 65级灰度表
  const uchar g_byAlphaLevel8[65] = { 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96, 100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144, 148, 152, 156, 160, 164, 168, 172, 176, 180, 184, 188, 192, 196, 200, 204, 208, 212, 216, 220, 224, 228, 232, 236, 240, 244, 248, 252, 255 };
  const uchar g_byAlphaLevel1[2] = {0, 255};
  uint nChar;
  int ww;
  int bSingleline = d->fmt & DT_SINGLELINE;
  font = GetCurFont();

  if (slen < 0) {
    slen = strlen(s0);
  }

  if (GGO_BITMAP == font->uFormat) {
    depth = 1;
    mapA = g_byAlphaLevel1;
  }
  else {
    depth = 8;
    mapA = g_byAlphaLevel8;
  }

  if (hDC && !bSingleline) {
    const char* e0 = e;
    int y1;

    for (; s < e0 && y0 + font->tm.tmHeight < clip.top;) {
      for (; s < e0; ++s) {
        if ('\n' == *s) {
          y0 += font->tm.tmHeight;
          ++s;
          break;
        }
      }
    }

    y1 = y0;
    e = s;

    for (; e < e0 && y0 - font->tm.tmHeight < clip.bottom;) {
      for (; e < e0; ++e) {
        if ('\n' == *e) {
          y0 += font->tm.tmHeight;
          ++e;
          break;
        }
      }
    }

    y0 = y1;
  }

  for (; s < e; s = s2) {
    nChar = nextChar(s, &s2);
    ww = font->tm.tmAveCharWidth * (s2 - s) / 2;

    if (' ' == nChar) {
      if (hDC) {
        int i = s - s0;
        DWORD crbk = d->crbks ? d->crbks[i] : d->crbk;
        DWORD clr = d->clrs ? d->clrs[i] : d->clr;

        if (GetAV(crbk)) {
          draw_fillrect(hDC, x0, y0, x0 + font->tm.tmAveCharWidth, y0 + font->tm.tmHeight, crbk);
        }
      }

      x0 += font->tm.tmAveCharWidth;
    }
    else if ('\n' == nChar && !bSingleline) {
      y0 += font->tm.tmHeight;
      x0 = x00;

      if (y0 < d->pthit.y) {
        d->charhit = s2 - s0;
      }

      continue;
    }
    else if (nChar != 13) {
      glyph_info* gi = FontGlyphOutline(nChar);

      if (NULL == gi) {
        x0 += font->tm.tmAveCharWidth;
        max_x0 = MAX(max_x0, x0);
        continue;
      }

      if (hDC) {
#define BITALIGN(x, n) (((x)+n-1)&(~(n-1)))
        int y2 = y0 + font->tm.tmAscent - gi->gm.gmptGlyphOrigin.y, x2 = x0 + gi->gm.gmptGlyphOrigin.x;
        int pitch = BITALIGN(gi->gm.gmBlackBoxX * depth, 32) >> 3;
        int i = s - s0;
        DWORD crbk = d->crbks ? d->crbks[i] : d->crbk;
        DWORD clr = d->clrs ? d->clrs[i] : d->clr;

        if (GetAV(crbk)) {
          draw_fillrect(hDC, x0, y0, x0 + gi->gm.gmCellIncX, y0 + font->tm.tmHeight, crbk);
        }

        draw_bit(hDC, x2, y2, gi->gm.gmBlackBoxX, gi->gm.gmBlackBoxY, gi->bit, pitch, depth, clr, mapA, 0);
      }

      x0 += gi->gm.gmCellIncX;
    }

    if (y0 < d->pthit.y && x0 - ww < d->pthit.x) {
      d->charhit = s2 - s0 + (s2 - s) * ((x0 + ww) < d->pthit.x);
    }

    max_x0 = MAX(max_x0, x0);
  }

  y0 += font->tm.tmHeight;

  d->sz.w = x0 - x00;
  d->sz.h = y0 - y00;
  d->max_cx = max_x0 - x00;

  if (d->pthit.x > 0) {
    d->charhit = text_hit(font, d->s, d->slen, d->pthit);
  }

  return 0;
}
int draw_text_impl(HDDC hDC, const char* s, int slen, int x0, int y0, DWORD clr, SIZE* outsz, int ptx, int pty)
{
  draw_text_t d[1] = {0};
  d->hDC = hDC, d->s = s, d->slen = slen, d->pt0.x = x0, d->pt0.y = y0, d->clr = clr, d->pthit.x = ptx, d->pthit.y = pty;
  draw_text_impl1(d);

  if (outsz) {
    *outsz = d->sz;
  }

  return d->charhit;
}
#define FontGetTextExtent(s) FontGetTextSize(s, strlen(s))
#define PosFromChar(S, SLEN) FontGetTextSize(S, SLEN).w
#define CharFromPos(S, SLEN, ptx, pty) draw_text_impl(0, S, SLEN, 0, 0, 0, 0, ptx, pty)
#define FontHeight()     FontGetTextSize("H", 1).h
// 获得字符串尺寸
SIZE FontGetTextSize(const char* s, int slen)
{
  SIZE sz = {0, 0};
  draw_text_impl(0, s, slen, 0, 0, 0, &sz, 0, 0);
  return sz;
}
// typedef struct _GLYPHMETRICS {
// uint gmBlackBoxX; //指定完全包围字体结构的最小矩阵的宽度
// uint gmBlackBoxY; //指定完全包围字体结构的最小矩阵的高度
// POINT gmptGlyphOrigin; //指定完全包围字体结构的最小矩阵左上角的点坐标
// short gmCellIncX; //指定当前的起点到下一个字符的起点的水平距离
// short gmCellIncY; //...垂直距离
// } GLYPHMETRICS, *LPGLYPHMETRICS;
#define draw_textPt(hDC, s, slen, x, y, uF, clr) draw_text(hDC, s, slen, (int)x, (int)y, (int)x+1300, (int)y+300, uF, clr)
#define draw_textR(hDC, s, slen, prc, uF, clr) draw_text(hDC, s, slen, (prc)->left, (prc)->top, (prc)->right, (prc)->bottom, uF, clr)
#define draw_textR2(hDC, s, rc, uF) draw_textR(hDC, s, -1, &(rc), uF, _clr)
int draw_text(HDDC hDC, const char* s, int slen, int x0, int y0, int x1, int y1, uint uFormat, DWORD clr)
{
  RECT rc1, rc = {x0, y0, x1, y1};
  SIZE sz;
  slen = (slen < 0) ? strlen(s) : slen;
  sz = FontGetTextSize(s, slen);
  RectAlign(rc, sz.w, sz.h, uFormat, &rc1);
  HDDC_BEGIN(hDC, &rc);
  draw_text_impl(hDC, s, slen, rc1.left, rc1.top, clr, 0, 0, 0);
  HDDC_END(hDC);
  return 0;
}
//自己绘制True type font字体
// int value approximating the FIXED value.
int PASCAL NEAR IntFromFixed1(FIXED f)
{
  if (f.fract >= 0x8000) {
    return (f.value + 1);
  }
  else {
    return (f.value);
  }
}
#define IntFromFixed fx_to_dbl
// (val1 + val2)/2 for FIXED values
FIXED PASCAL NEAR fxDiv2(FIXED fxVal1, FIXED fxVal2)
{
  long l;
  l = (*((long far*) & (fxVal1)) + * ((long far*) & (fxVal2))) / 2;
  return (*(FIXED*) &l);
}
// Converts a line define by two points to a four point Bezier spline representation of the line in pPts.
// number of Bezier points placed into the pPts DPOINT array.
uint MakeBezierFromLine(DPOINT* pPts, DPOINT startpt, DPOINT endpt)
{
  uint cTotal = 0; // starting point of Bezier
  pPts[cTotal] = startpt;
  cTotal++; // 1rst Control, pt == endpoint makes Bezier a line
  //pPts[cTotal].x = endpt.x;
  //pPts[cTotal].y = endpt.y;
  pPts[cTotal] = endpt;
  cTotal++; // 2nd Control, pt == startpoint makes Bezier a line
  //pPts[cTotal].x = startpt.x;
  //pPts[cTotal].y = startpt.y;
  pPts[cTotal] = startpt;
  cTotal++; // ending point of Bezier
  pPts[cTotal] = endpt;
  cTotal++;
  return cTotal;
}
// Converts a quadratic spline in pSline to a four point Bezier spline in pPts.
// number of Bezier points placed into the pPts DPOINT array.
uint MakeBezierFromQBSpline(DPOINT* pPts, POINTFX* pSpline)
{
  DPOINT P0, // Quadratic on curve start point
          P1, // Quadratic control point
          P2; // Quadratic on curve end point
  uint cTotal = 0; // Convert the Quadratic points to integer
  P0.x = IntFromFixed(pSpline[0].x);
  P0.y = IntFromFixed(pSpline[0].y);
  P1.x = IntFromFixed(pSpline[1].x);
  P1.y = IntFromFixed(pSpline[1].y);
  P2.x = IntFromFixed(pSpline[2].x);
  P2.y = IntFromFixed(pSpline[2].y);
  // conversion of a quadratic to a cubic // Cubic P0 is the on curve start point
  pPts[cTotal] = P0;
  cTotal++;
  // Cubic P1 in terms of Quadratic P0 and P1
  pPts[cTotal].x = P0.x + 2 * (P1.x - P0.x) / 3;
  pPts[cTotal].y = P0.y + 2 * (P1.y - P0.y) / 3;
  cTotal++; // Cubic P2 in terms of Qudartic P1 and P2
  pPts[cTotal].x = P1.x + 1 * (P2.x - P1.x) / 3;
  pPts[cTotal].y = P1.y + 1 * (P2.y - P1.y) / 3;
  cTotal++; // Cubic P3 is the on curve end point
  pPts[cTotal] = P2;
  cTotal++;
  return cTotal;
}
// FUNCTION : AppendPolyLineToBezier
// PURPOSE : Converts line segments into their Bezier point
// representation and appends them to a list of Bezier points.
// WARNING - The array must have at least one valid
// start point prior to the address of the element passed.
// RETURNS : number of Bezier points added to the DPOINT array.
uint AppendPolyLineToBezier(DPOINT* pt, POINTFX start, LPTTPOLYCURVE lpCurve)
{
  int i;
  uint cTotal = 0;
  DPOINT endpt;
  DPOINT startpt;
  DPOINT bezier[4];
  endpt.x = IntFromFixed(start.x);
  endpt.y = IntFromFixed(start.y);

  for (i = 0; i < lpCurve->cpfx; i++) {
    // define the line segment
    startpt = endpt;
    endpt.x = IntFromFixed(lpCurve->apfx[i].x);
    endpt.y = IntFromFixed(lpCurve->apfx[i].y); // convert a line to a bezier representation
    MakeBezierFromLine(bezier, startpt, endpt); // append the Bezier to the existing ones
    // Point 0 is Point 3 of previous.
    pt[cTotal++] = bezier[1]; // Point 1
    pt[cTotal++] = bezier[2]; // Point 2
    pt[cTotal++] = bezier[3]; // Point 3
  }

  return cTotal;
}
// FUNCTION : AppendQuadBSplineToBezier
// PURPOSE : Converts Quadratic spline segments into their Bezier point
// representation and appends them to a list of Bezier points.
// WARNING - The array must have at least one valid
// start point prior to the address of the element passed.
// RETURNS : number of Bezier points added to the DPOINT array.
uint AppendQuadBSplineToBezier(DPOINT* pt, POINTFX start, LPTTPOLYCURVE lpCurve)
{
  WORD i;
  uint cTotal = 0;
  POINTFX spline[3]; // a Quadratic is defined by 3 points
  DPOINT bezier[4]; // a Cubic by 4 // The initial A point is on the curve.
  spline[0] = start;

  for (i = 0; i < lpCurve->cpfx;) {
    // The B point.
    spline[1] = lpCurve->apfx[i++]; // Calculate the C point.

    if (i == (lpCurve->cpfx - 1)) {
      // The last C point is described explicitly
      // i.e. it is on the curve.
      spline[2] = lpCurve->apfx[i++];
    }
    else {
      // C is midpoint between B and next B point
      // because that is the on curve point of
      // a Quadratic B-Spline.
      spline[2].x = fxDiv2(lpCurve->apfx[i - 1].x, lpCurve->apfx[i].x);
      spline[2].y = fxDiv2(lpCurve->apfx[i - 1].y, lpCurve->apfx[i].y);
    } // convert the Q Spline to a Bezier

    MakeBezierFromQBSpline(bezier, spline);
    // append the Bezier to the existing ones
    // Point 0 is Point 3 of previous.
    pt[cTotal++] = bezier[1]; // Point 1
    pt[cTotal++] = bezier[2]; // Point 2
    pt[cTotal++] = bezier[3]; // Point 3 // New A point for next slice of spline is the
    // on curve C point of this B-Spline
    spline[0] = spline[2];
  }

  return cTotal;
}
//FUNCTION : CloseContour
//PURPOSE : Adds a bezier line to close the circuit defined in pt.
//RETURNS : number of points aded to the pt DPOINT array.
uint CloseContour(DPOINT* pt, uint cTotal)
{
  DPOINT endpt, startpt; // definition of a line
  DPOINT bezier[4]; // connect the first and last points by a line segment
  startpt = pt[cTotal - 1];
  endpt = pt[0]; // convert a line to a bezier representation
  MakeBezierFromLine(bezier, startpt, endpt); // append the Bezier to the existing ones
  // Point 0 is Point 3 of previous.
  pt[cTotal++] = bezier[1]; // Point 1
  pt[cTotal++] = bezier[2]; // Point 2
  pt[cTotal++] = bezier[3]; // Point 3
  return 3;
}
int GetOutline(DPOINT* pt, int* lens, const uint pChar)
{
  GLYPHMETRICS gmm;
  MAT2 mat;
  unsigned short nChar;
  DWORD dwSize;
  HDC hDC = GetMemDC();
  char pHeader[1024];
  int n = 0;

  if (pChar < 0) {
    nChar = *(int*)pChar;
    nChar = (nChar >> 8) | ((nChar & 0x00ff) << 8);
  }
  else {
    nChar = pChar;
  }

  mat.eM11.value = 1, mat.eM11.fract = 0;
  mat.eM12.value = 0, mat.eM12.fract = 0;
  mat.eM21.value = 0, mat.eM21.fract = 0;
  mat.eM22.value = 1, mat.eM22.fract = 0;
  dwSize = GetGlyphOutline(hDC, nChar, GGO_NATIVE, &gmm, countof(pHeader), pHeader, &mat);

  if (dwSize == GDI_ERROR) {
    return 0;
  }

  {
    LPTTPOLYGONHEADER lpHeader = (LPTTPOLYGONHEADER)pHeader;
    WORD i;
    uint cTotal = 0; // Total points in a contour.
    LPTTPOLYGONHEADER lpStart; // the start of the buffer
    LPTTPOLYCURVE lpCurve; // the current curve of a contour
    DPOINT* pt0 = pt; // the bezier buffer
    POINTFX ptStart; // The starting point of a curve
    int size = dwSize;
    DWORD dwMaxPts = size / sizeof(POINTFX); // max possible pts.
    DWORD dwBuffSize;
    dwBuffSize = dwMaxPts* // Maximum possible # of contour points.
        sizeof(DPOINT) * // sizeof buffer element
        3; // Worst case multiplier of one additional point
    // of line expanding to three points of a bezier
    lpStart = lpHeader;

    // Loop until we have processed the entire buffer of contours.
    // The buffer may contain one or more contours that begin with
    // a TTPOLYGONHEADER. We have them all when we the end of the buffer.
    //pO->nMaxPts = dwMaxPts*3;
    while ((DWORD)lpHeader < (DWORD)(((LPSTR)lpStart) + size) && pt != NULL) {
      if (lpHeader->dwType == TT_POLYGON_TYPE) {
        // Draw each coutour, currently this is the only valid
        // type of contour.
        // Convert the starting point. It is an on curve point.
        // All other points are continuous from the "last"
        // point of the contour. Thus the start point the next
        // bezier is always pt[cTotal-1] - the last point of the
        // previous bezier. See PolyBezier.
        // Get to first curve of contour - it starts at the next byte beyond header
        cTotal = 1;
        pt[0].x = IntFromFixed(lpHeader->pfxStart.x);
        pt[0].y = IntFromFixed(lpHeader->pfxStart.y);
        lpCurve = (LPTTPOLYCURVE)(lpHeader + 1); // Walk this contour and process each curve( or line ) segment

        // and add it to the Beziers
        while ((DWORD)lpCurve < (DWORD)(((LPSTR)lpHeader) + lpHeader->cb)) {
          // Format assumption:
          // The bytes immediately preceding a POLYCURVE
          // structure contain a valid POINTFX.
          //
          // If this is first curve, this points to the
          // pfxStart of the POLYGONHEADER.
          // Otherwise, this points to the last point of
          // the previous POLYCURVE.
          //
          // In either case, this is representative of the
          // previous curve"s last point.
          ptStart = *(LPPOINTFX)((LPSTR)lpCurve - sizeof(POINTFX));

          if (lpCurve->wType == TT_PRIM_LINE) {
            // convert the line segments to Bezier segments
            cTotal += AppendPolyLineToBezier(&pt[cTotal], ptStart, lpCurve);
            i = lpCurve->cpfx;
          }
          else if (lpCurve->wType == TT_PRIM_QSPLINE) {
            // Decode each Quadratic B-Spline segment, convert to bezier,
            // and append to the Bezier segments
            cTotal += AppendQuadBSplineToBezier(&pt[cTotal], ptStart, lpCurve);
            i = lpCurve->cpfx;
          }
          else
            // Oops! A POLYCURVE format we don"t understand.
            ;

          // error, error, error // Move on to next curve in the contour.
          lpCurve = (LPTTPOLYCURVE) & (lpCurve->apfx[i]);
        } // Add points to close the contour.

        // All contours are implied closed by TrueType definition.
        // Depending on the specific font and glyph being used, these
        // may not always be needed.
        if (pt[cTotal - 1].x != pt[0].x || pt[cTotal - 1].y != pt[0].y) {
          cTotal += CloseContour(pt, cTotal);
        } // flip coordinates to get glyph right side up (Windows coordinates)

        // TT native coordiantes are zero originate at lower-left.
        // Windows MM_TEXT are zero originate at upper-left.
        for (i = 0; i < cTotal; i++) {
          pt[i].y = 0 - pt[i].y; // Draw the contour
        }

        // RealRender added
        lens[n++] = cTotal;
        pt += cTotal;
      }
      else {
        // Bad, bail, must have a bogus buffer.
        break; // error, error, error // Move on to next Contour.
      }

      // Its header starts immediate after this contour
      lpHeader = (LPTTPOLYGONHEADER)(((LPSTR)lpHeader) + lpHeader->cb);
    }
  }

  return n;
}
#define ClipDC1(hDst, hSrc, lRc) ClipDC(hDst, hSrc, (lRc)->left, (lRc)->top, (lRc)->right, (lRc)->bottom)
int ClipDC(HDDC hDst, HDDC hSrc, int l, int t, int r, int b)
{
  int tmp;

  if (l > r) {
    SWAP(l, r, tmp);
  }

  if (t > b) {
    SWAP(t, b, tmp);
  }

  hDst->data = &_Pixel(hSrc, l, t);
  hDst->bw = hSrc->bw, hDst->h = b - t, hDst->w = r - l;
  return 0;
}
//在指定位置把ttf的轮廓画出来
//不仅自己绘制ttf字体的轮廓，而且，略加修改，可以生成全三维的ttf mesh，用d3d渲染
int DrawPoly_tt(HDDC hDC, DPOINT* pt, int* lens, int n, DWORD clr)
{
  int i, j, k;

  for (j = 0; j < n; j++) {
    //if (j!=1) continue;
    for (i = 0; i < lens[j]; i++) {
      k = (i + 1) % lens[j];
      //TDrawLine(hDC, pt[i].x, pt[i].y, pt[k].x, pt[k].y, clr);
    }

    pt += lens[j];
  }

  return 0;
}
int drawaa_text(HDDC hDC, const char* s, int slen, double x, double y, DWORD clr)
{
  GLYPHMETRICS gm;
  TEXTMETRIC tm;
  DPOINT pt[1 << 10];
  int n, lens[1 << 5];
  const char* e = s + slen;
  char* s2;
  HDC hMemDC = GetMemDC();
  GetTextMetrics(hMemDC, &tm);

  for (; s < e; s = s2) {
    uint thisChar = nextChar(s, &s2);
    n = GetCharGlyph(thisChar, x, y + tm.tmAscent, &gm, pt, lens);

    if (x + gm.gmCellIncX > hDC->w) {
      y += tm.tmHeight, x = 0;
      n = GetCharGlyph(thisChar, x, y + tm.tmAscent, &gm, pt, lens);
    }

    drawaa_fillpoly_solid(hDC, pt, lens, n, clr);
    x += gm.gmCellIncX;
  }

  return 0;
}
#define DrawTextOutline(hDC, _x, _y, _h, _s, clr) DrawTextOutlineA(hDC, _x, _y, _h, _s, strlen(_s), clr)
int test_truetypefont(HDDC hDC, int ch)
{
  //DPOINT pt[10000];
  //int lens[100];
  //int n;
  static double w = 200;
  char* str = "龘龖靐嚼CDSa";
  int slen = strlen(str);

  if ('1' == ch) {
    w *= 1.1;
    w = MIN(w, 800);
  }

  if ('2' == ch) {
    w /= 1.1;
    w = MAX(w, 8);
  }

  //CTTFOutline outline[1];
  SelectFontToMemDC("黑体", (int)w, 0);
  //CreateOutline(outline, "哪");
  //DrawTTFOutline(hDC, outline, 160, 160, _RGB(255, 0, 0), 1);
  //通过这段代码，就创建了一个outline
  //有了ttf的字体轮廓，我想主要应用在3d渲染上比较有用，比如一些专门的三维字体制作工具，或者用在游戏的特殊效果中，
  //比如说旋转的"临兵斗者皆阵列在前"。
  //SelectFontToMemDC("宋体", 72, 0, 0);
  //DrawTextOutline(hDC, 100, 100, 160, "字体轮廓", _RGB(255, 0, 0));
  //n = GetTextGlyph(str, slen, 100, 100, pt, lens);
  //ASSERT(lens[]<countof(pt));
  //FillPolyCell2(hDC, pt, lens, n, 100, 100, slen*w/2, 100+w, _RGB(255, 0, 0), _RGB(0, 0, 255));
  drawaa_text(hDC, str, slen, 100, 100, _RGB(255, 0, 0));
  return 0;
}

