
#include "mem.h"
#include "draw\draw.inl"
typedef struct _sRGBA {
  //! current red value
  double r;
  //! current green value
  double s;
  //! current blue value
  double b;
  //! current alpha value
  double a;
} SRGBA;

typedef struct _sRectF {
  double left;
  double top;
  double width;
  double height;
} SRectF;

typedef struct _sRectI {
  int left;
  int top;
  int width;
  int height;
} SRectI;

typedef struct _sPointF {
  double x;
  double y;
} SPointF;

//! \enum Text alignment
typedef enum _ETextAlignment {
  TextAlignmentLeft = 0,
  TextAlignmentRight = 1,
  TextAlignmentCenter = 2,
} ETextAlignment;

typedef struct _SGDIpState {
  int m_gState;
  DWORD m_curColor;
  DWORD m_fillColor;
  LOGFONT m_font;
  double m_lineWidth;
} SGDIpState;

typedef struct gfx_t {
  //! main bounding box llx lly width height
  SRectI m_rMainBoundingBox;
  //! current bonding box llx lly width height
  SRectI m_rBoundingBox;
  //! llx lly width height
  SRectF m_rWorldBoundingBox;
  //! Current clip coordinates
  SRectF  m_rCurClip;
  // Stack of clip coordinates
  //SRectF* m_lClips;
  SGDIpState m_curState;
  SGDIpState* m_lStates;
  int m_lStateLen;
  DDC m_pGraphics[1];
  double* m_pDash;
  int m_dashlen;
  SRectF* m_lClips;
  int m_lClipLen;
} gfx_t;


double GetMainBBLeft(gfx_t* s)
{
  return s->m_rMainBoundingBox.left;
}
double GetMainBBBottom(gfx_t* s)
{
  return s->m_rMainBoundingBox.top - s->m_rMainBoundingBox.height;
}
double GetMainBBRight(gfx_t* s)
{
  return s->m_rMainBoundingBox.left + s->m_rMainBoundingBox.width;
}
double GetMainBBTop(gfx_t* s)
{
  return s->m_rMainBoundingBox.top;
}
double GetMainBBWidth(gfx_t* s)
{
  return s->m_rMainBoundingBox.width;
}
double GetMainBBHeight(gfx_t* s)
{
  return s->m_rMainBoundingBox.height;
}
double GetBBLeft(gfx_t* s)
{
  return s->m_rBoundingBox.left;
}
double GetBBBottom(gfx_t* s)
{
  return s->m_rBoundingBox.top - s->m_rBoundingBox.height;
}
double GetBBRight(gfx_t* s)
{
  return s->m_rBoundingBox.left + s->m_rBoundingBox.width;
}
double GetBBTop(gfx_t* s)
{
  return s->m_rBoundingBox.top;
}
double GetBBWidth(gfx_t* s)
{
  return s->m_rBoundingBox.width;
};
double GetBBHeight(gfx_t* s)
{
  return s->m_rBoundingBox.height;
}
double GetWorldLeft(gfx_t* s)
{
  return s->m_rWorldBoundingBox.left;
}
double GetWorldBottom(gfx_t* s)
{
  return s->m_rWorldBoundingBox.top - s->m_rWorldBoundingBox.height;
}
double GetWorldRight(gfx_t* s)
{
  return s->m_rWorldBoundingBox.left + s->m_rWorldBoundingBox.width;
}
double GetWorldTop(gfx_t* s)
{
  return s->m_rWorldBoundingBox.top;
}
double GetWorldWidth(gfx_t* s)
{
  return s->m_rWorldBoundingBox.width;
}
double GetWorldHeight(gfx_t* s)
{
  return s->m_rWorldBoundingBox.height;
}

double BBToWorldx(gfx_t* s, double x)
{
  return GetWorldLeft(s) + (x - (double)GetBBLeft(s)) / (double)(GetBBWidth(s))
      * (double)(GetWorldWidth(s));
};
double BBToWorldy(gfx_t* s, double y)
{
  return GetWorldTop(s) + (y - (double)GetBBTop(s)) / (double)(GetBBHeight(s))
      * (double)(GetWorldHeight(s));
};
double BBToWorldAbsx(gfx_t* s, double x)
{
  return x / (double)(GetBBWidth(s)) * (double)(GetWorldWidth(s));
};
double BBToWorldAbsy(gfx_t* s, double y)
{
  return y / (double)(GetBBHeight(s)) * (double)(GetWorldHeight(s));
};

//! Transform world x coordinate to file coordinate x
double WorldToBBx(gfx_t* s, double x)
{
  return GetBBLeft(s) + (x - (double)GetWorldLeft(s)) / (double)(GetWorldWidth(s))
      * (double)(GetBBWidth(s));
};
//! Transform world y coordinate to file coordinate y
double WorldToBBy(gfx_t* s, double y)
{
  return GetMainBBBottom(s) + GetMainBBHeight(s)
      - ((y - (double)GetWorldBottom(s)) / (double)(GetWorldHeight(s))
          * (double)(GetBBHeight(s)) + GetBBBottom(s));
};
//! Transform world x coordinate to file coordinate x
double WorldToBBAbsx(gfx_t* s, double x)
{
  return x / (double)(GetWorldWidth(s)) * (double)(GetBBWidth(s));
};
//! Transform absolute world y coordinate to file coordinate y
double WorldToBBAbsy(gfx_t* s, double y)
{
  return y / (double)(GetWorldHeight(s)) * (double)(GetBBHeight(s));
};

double GetWorldClipBottom(gfx_t* s)
{
  return BBToWorldy(s, s->m_rCurClip.top);
};
double GetWorldClipTop(gfx_t* s)
{
  return BBToWorldy(s, s->m_rCurClip.top + s->m_rCurClip.height);
};

double GetBBClipBottom(gfx_t* s)
{
  return s->m_rCurClip.top + s->m_rCurClip.height;
};
double GetBBClipTop(gfx_t* s)
{
  return s->m_rCurClip.top;
};



void gfx_set(gfx_t* s)
{
  s->m_rCurClip.left = s->m_rCurClip.top = s->m_rCurClip.width = s->m_rCurClip.height = 0;
}

void gfx_free(gfx_t* s)
{
  SAFEFREE(s->m_lClips);
  SAFEFREE(s->m_pDash);
}

bool SetMainBoundingBoxRC(gfx_t* s, const RECT rect)
{
  s->m_rMainBoundingBox.left = rect.left;
  s->m_rMainBoundingBox.top = rect.bottom;
  s->m_rMainBoundingBox.width = RCW(&rect);
  s->m_rMainBoundingBox.height = RCH(&rect);
  return true;
}

bool SetBoundingBox(gfx_t* s, int llx, int lly, int urx, int ury)
{
  if ((llx >= urx) || (lly >= ury)) {
    return false;
  }

  s->m_rBoundingBox.left = llx;
  s->m_rBoundingBox.top = ury;
  s->m_rBoundingBox.width = urx - llx;
  s->m_rBoundingBox.height = ury - lly;
  ASSERT(s->m_rBoundingBox.width >= 0);
  ASSERT(s->m_rBoundingBox.height >= 0);

  return true;
}

bool SetMainBoundingBox(gfx_t* s, int llx, int lly, int urx, int ury)
{
  if ((llx >= urx) || (lly >= ury)) {
    return false;
  }

  s->m_rMainBoundingBox.left = llx;
  s->m_rMainBoundingBox.top = ury;
  s->m_rMainBoundingBox.width = urx - llx;
  s->m_rMainBoundingBox.height = ury - lly;
  ASSERT(s->m_rMainBoundingBox.width >= 0);
  ASSERT(s->m_rMainBoundingBox.height >= 0);

  return SetBoundingBox(s, llx, lly, urx, ury);
}

bool SetWorldBoundingBox(gfx_t* s, double llx, double lly, double urx, double ury)
{
  if ((llx >= urx) || (lly >= ury)) {
    return false;
  }

  s->m_rWorldBoundingBox.left = llx;
  s->m_rWorldBoundingBox.top = ury;
  s->m_rWorldBoundingBox.width = urx - llx;
  s->m_rWorldBoundingBox.height = ury - lly;

  return true;
}

void GetDefaultLOGFONT(gfx_t* s, LOGFONT* lf)
{
  lf->lfHeight = 8;
  lf->lfWidth = 0;
  lf->lfEscapement = 0;
  lf->lfOrientation = 0;
  lf->lfWeight = FW_NORMAL;
  lf->lfItalic = FALSE;
  lf->lfUnderline = FALSE;
  lf->lfStrikeOut = FALSE;
  lf->lfCharSet = ANSI_CHARSET;
  lf->lfOutPrecision = OUT_TT_PRECIS;
  lf->lfClipPrecision = CLIP_DEFAULT_PRECIS;
  lf->lfQuality = ANTIALIASED_QUALITY;
  lf->lfPitchAndFamily = FF_DONTCARE | DEFAULT_PITCH;
}

void gfx_BeginPaint(gfx_t* s)
{
  s->m_rBoundingBox = s->m_rMainBoundingBox;
  s->m_rCurClip = s->m_rWorldBoundingBox;

  s->m_curState.m_curColor = rgb(0, 0, 0);
  s->m_curState.m_fillColor = rgb(1, 1, 1);
  s->m_curState.m_lineWidth = 1;
  GetDefaultLOGFONT(s, &s->m_curState.m_font);
  //s->m_curState.m_dashlen = 0;

  //s->m_pGraphics->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
  //s->m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
  //s->m_pGraphics->SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
  //
}

void gfx_EndPaint(gfx_t* s)
{
  s->m_rBoundingBox = s->m_rMainBoundingBox;
}

void PushState(gfx_t* s)
{
  SGDIpState oldstate;
  ASSERT(s->m_pGraphics);
  // saving state
  oldstate = s->m_curState;

  MEMPUSH(SGDIpState, s->m_lStates, s->m_lStateLen);
  s->m_lStates[s->m_lStateLen++] = oldstate;
}

void PopState(gfx_t* s)
{
  ASSERT(s->m_pGraphics);
  ASSERT(s->m_lStateLen > 0);
  // restoring state
  s->m_curState = s->m_lStates[--s->m_lStateLen];

  // delete dash if needed
  if (s->m_pDash) {
    SAFEFREE(s->m_pDash);
  }

}


void gfx_DrawArrow(gfx_t* s, double tailx, double taily, double tipx, double tipy, double tailthickness, double  headthickness, double headlength, bool filled)
{
}

void gfx_DrawArrowAbs(gfx_t* s, double tailx, double taily, double dtipx, double dtipy, double tailthickness, double  headthickness, double headlength, bool filled)
{
}

void gfx_DrawLine(gfx_t* s, double x1, double y1, double x2, double y2)
{
  ASSERT(s->m_pGraphics);
  drawaa_strokeline(s->m_pGraphics, x1, y1, x2, y2, s->m_curState.m_curColor, s->m_curState.m_lineWidth);
}

void gfx_DrawLineStrip(gfx_t* s, int n, double* pX, double* pY, bool open, bool filled)
{
  int i;
  DPOINT* pPoints;
  ASSERT(s->m_pGraphics);


  if (n == 0) {
    return;
  }

  if (!filled) {
    pPoints = MALLOC(DPOINT, n);
  }
  else {
    // adding bottom if filled
    pPoints = MALLOC(DPOINT, n + 2);
  }

  for (i = 0; i < n; i++) {
    pPoints[i].x = WorldToBBx(s, pX[i]);
    pPoints[i].y = WorldToBBy(s, pY[i]);
  }

  if (filled) {
    pPoints[n].x = WorldToBBx(s, pX[n - 1]);
    pPoints[n].y = GetBBClipBottom(s);
    pPoints[n + 1].x = WorldToBBx(s, pX[0]);
    pPoints[n + 1].y = GetBBClipBottom(s);
  }

  drawaa_poly(s->m_pGraphics, pPoints, (int)n, !open,
      filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);

  // cleaning
  SAFEFREE(pPoints);
}

void gfx_DrawMultipleLineStrip(gfx_t* s, int n, int ns, double* pX, double* pY, bool open, bool filled)
{
  int i;
  int nscur, index;
  DPOINT* pPoints;
  ASSERT(s->m_pGraphics);

  //allocating memory
  if (!filled) {
    pPoints = MALLOC(DPOINT, ns);
  }
  else {
    // adding bottom if filled
    pPoints = MALLOC(DPOINT, ns + 2);
  }

  index = 0;
  nscur = 0;

  do {
    nscur = __min(ns, n - index);

    for (i = index; i < index + nscur; i++) {
      pPoints[i - index].x = WorldToBBx(s, pX[i]);
      pPoints[i - index].y = WorldToBBy(s, pY[i]);
    }

    if (filled) {
      pPoints[nscur].x = WorldToBBx(s, pX[index + nscur - 1]);
      pPoints[nscur].y = GetBBClipBottom(s);
      pPoints[nscur + 1].x = WorldToBBx(s, pX[index]);
      pPoints[nscur + 1].y = GetBBClipBottom(s);
    }

    drawaa_poly(s->m_pGraphics, pPoints, (int)nscur, !open,
        filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);

    // udpating index...
    index += ns;
  }
  while (index < n);

  // cleaning
  SAFEFREE(pPoints);

}


void gfx_DrawStepStrip(gfx_t* s, int n, double* pX, double* pY, bool open, bool filled)
{
  int i;
  DPOINT* pPoints;
  ASSERT(s->m_pGraphics);

  //allocating memory
  //allocating memory

  if (!filled) {
    pPoints = MALLOC(DPOINT, n * 2 - 2);
  }
  else {
    // adding bottom if filled
    pPoints = MALLOC(DPOINT, 2 * n);
  }


  for (i = 0; i < n - 1; i++) {
    pPoints[2 * i].x = WorldToBBx(s, pX[i]);
    pPoints[2 * i].y = WorldToBBy(s, pY[i]);

    pPoints[2 * i + 1].x = WorldToBBx(s, pX[i + 1]);
    pPoints[2 * i + 1].y = WorldToBBy(s, pY[i]);
  }

  if (filled) {
    pPoints[2 * n - 2].x = WorldToBBx(s, pX[n - 1]);
    pPoints[2 * n - 2].y = GetBBClipBottom(s);
    pPoints[2 * n - 1].x = WorldToBBx(s, pX[0]);
    pPoints[2 * n - 1].y = GetBBClipBottom(s);
  }

  drawaa_poly(s->m_pGraphics, pPoints, (int)(n * 2 - 2), !open,
      filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);

  // cleaning
  SAFEFREE(pPoints);

}

void gfx_DrawGridHor(gfx_t* s, double llx, double urx, double lly, double ury, double dy)
{
  int i, n;
  ASSERT(s->m_pGraphics);

  n = (int)floor((ury - lly) / dy) + 1;
  n = __min(n, 100);

  for (i = 0; i < n; i++) {
    drawaa_strokeline(s->m_pGraphics, WorldToBBx(s, llx), WorldToBBy(s, lly + i * dy),
        WorldToBBx(s, urx), WorldToBBy(s, lly + i * dy), s->m_curState.m_curColor, s->m_curState.m_lineWidth);
  }

}

void gfx_DrawGridVer(gfx_t* s, double llx, double urx, double lly, double ury, double dx)
{
  int i, n;
  ASSERT(s->m_pGraphics);
  n = (uint)floor((urx - llx) / dx) + 1;

  n = __min(n, 100);

  for (i = 0; i < n; i++) {
    drawaa_strokeline(s->m_pGraphics, WorldToBBx(s, llx + i * dx), WorldToBBy(s, lly),
        WorldToBBx(s, llx + i * dx), WorldToBBy(s, ury), s->m_curState.m_curColor, s->m_curState.m_lineWidth);
  }

}

void gfx_DrawRectf(gfx_t* s, double llx, double lly, double urx, double ury, bool filled)
{
  ASSERT(s->m_pGraphics);

  drawaa_rect(s->m_pGraphics, WorldToBBx(s, llx), WorldToBBy(s, lly), WorldToBBx(s, urx), WorldToBBy(s, ury),
      filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);
}

void gfx_DrawBox(gfx_t* s, double llx, double lly, double dx, double dy, bool filled)
{
  drawaa_rect(s->m_pGraphics, WorldToBBx(s, llx), WorldToBBy(s, lly), WorldToBBx(s, llx) + dx, WorldToBBy(s, lly) + dy,
      filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);

}

void gfx_DrawCircle(gfx_t* s, double cx, double cy, double radius, bool filled)
{
  ASSERT(s->m_pGraphics);

  drawaa_ellipse(s->m_pGraphics, cx, cy, radius, radius,
      filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);

}

void gfx_DrawTriangle(gfx_t* s, double cx, double cy, double radius, bool filled)
{
  DPOINT tri[3];
  double sr3 = radius / (sqrt(3));
  ASSERT(s->m_pGraphics);

  tri[0].x = WorldToBBx(s, cx) - sqrt(2) * sr3;
  tri[0].y = WorldToBBy(s, cy) + sr3;
  tri[1].x = WorldToBBx(s, cx) + sqrt(2) * sr3;
  tri[1].y = WorldToBBy(s, cy) + sr3;
  tri[2].x = WorldToBBx(s, cx);
  tri[2].y = WorldToBBy(s, cy) - radius * sqrt(2) / 2;

  drawaa_poly(s->m_pGraphics, tri, 3, 1,
      filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);
}

void gfx_DrawSquare(gfx_t* s, double cx, double cy, double radius, bool filled)
{
  ASSERT(s->m_pGraphics);

  drawaa_rect(s->m_pGraphics, WorldToBBx(s, cx) - radius, WorldToBBy(s, cy) - radius, WorldToBBx(s, cx) + radius, WorldToBBy(s, cy) + radius,
      filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);

}

void gfx_DrawBoxStrip(gfx_t* s, int n, double* pLlx, double* pLly, double dx, double dy, bool filled)
{
  int i;

  for (i = 0; i < n; i++) {
    drawaa_rect(s->m_pGraphics, WorldToBBx(s, pLlx[i]) - dx / 2, WorldToBBy(s, pLly[i]) - dy / 2,
        WorldToBBx(s, pLlx[i]) + dx / 2, WorldToBBy(s, pLly[i]) + dy / 2,
        filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);
  }
}

void gfx_DrawCircleStrip(gfx_t* s, int n, double* pCx, double* pCy, double radius, bool filled)
{
  int i;
  ASSERT(s->m_pGraphics);

  for (i = 0; i < n; i++) {
    drawaa_ellipse(s->m_pGraphics, WorldToBBx(s, pCx[i]), WorldToBBy(s, pCy[i]), radius, radius,
        filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);
  }

}

void gfx_DrawTriangleStrip(gfx_t* s, int n, double* pCx, double* pCy, double radius, bool filled)
{
  int i;
  DPOINT tri[3];
  double sr3 = radius / (sqrt(3));
  ASSERT(s->m_pGraphics);

  for (i = 0; i < n; i++) {
    tri[0].x = WorldToBBx(s, pCx[i]) - sqrt(2) * sr3;
    tri[0].y = WorldToBBy(s, pCy[i]) + sr3;
    tri[1].x = WorldToBBx(s, pCx[i]) + sqrt(2) * sr3;
    tri[1].y = WorldToBBy(s, pCy[i]) + sr3;
    tri[2].x = WorldToBBx(s, pCx[i]);
    tri[2].y = WorldToBBy(s, pCy[i]) - radius * sqrt(2) / 2;
    drawaa_poly(s->m_pGraphics, tri, 3, 1,
        filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);
  }
}

void gfx_DrawSquareStrip(gfx_t* s, int n, double* pCx, double* pCy, double radius, bool filled)
{
  int i;
  ASSERT(s->m_pGraphics);

  for (i = 0; i < n; i++) {
    drawaa_rect(s->m_pGraphics, WorldToBBx(s, pCx[i]) - radius, WorldToBBy(s, pCy[i]) - radius, WorldToBBx(s, pCx[i]) + radius, WorldToBBy(s, pCy[i]) + radius,
        filled ? s->m_curState.m_fillColor : 0, filled ? 0 : s->m_curState.m_curColor, s->m_curState.m_lineWidth);
  }
}

void gfx_DrawSurf(gfx_t* s, int nx, int ny, double llx, double lly, double dx, double dy, const double* pColor)
{
  int i, j, k;
  int index[4];
  double r, g, b;
  DPOINT psF[4];
  DWORD colors[4];
  DWORD center;
  //Gdiplus::RectF rect;
  ASSERT(s->m_pGraphics);

  for (i = 0; i < ny - 1; i++) {
    for (j = 0; j < nx - 1; j++) {
      index[0]  = i * nx + j;
      index[1]  = (i + 1) * nx + j;
      index[2]  = (i + 1) * nx + j + 1;
      index[3]  = i * nx + j + 1;

      r = g = b = 0;

      for (k = 0; k < 4; k++) {
        const double* clr = pColor + index[k] * 3;
        colors[k] = rgb(clr[0], clr[1], clr[2]);
        r += clr[0];
        g += clr[1];
        b += clr[2];
      }

      center = rgb((int)floor(r / 4 * 255.0), (int)floor(g / 4 * 255.0), (int)floor(b / 4 * 255.0));

      psF[0] = dPOINT(WorldToBBx(s, llx + (j + 0) * dx) - 1, WorldToBBy(s, lly + (ny - (i + 1) - 0) * dy) - 1);
      psF[1] = dPOINT(WorldToBBx(s, llx + (j + 0) * dx) - 1, WorldToBBy(s, lly + (ny - (i + 1) - 1) * dy) + 1);
      psF[2] = dPOINT(WorldToBBx(s, llx + (j + 1) * dx) + 1, WorldToBBy(s, lly + (ny - (i + 1) - 1) * dy) + 1);
      psF[3] = dPOINT(WorldToBBx(s, llx + (j + 1) * dx) + 1, WorldToBBy(s, lly + (ny - (i + 1) - 0) * dy) - 1);

#if 0
      Gdiplus::PathGradientBrush brush(psF, 4);
      int count = 4;
      brush.SetSurroundColors(colors, &count);
      brush.SetCenterColor(center);

      s->m_pGraphics->FillRectangle(&brush,
          Gdiplus::RectF(WorldToBBx(llx + j * dx) - 1, WorldToBBy(lly + (ny - (i + 1))*dy) - 1, WorldToBBAbsx(dx) + 2, WorldToBBAbsy(dy) + 2));
#endif

    }
  }
}
double NormalizeAngle(double angle)
{
  if (angle >= 0) {
    return angle - 360 * ((int)floor(angle) / 360);
  }
  else {
    return angle + 360 * ((int)floor(angle) / 360 + 1);
  }
};

void gfx_DrawTextLeft(gfx_t* s, double x, double y, const char* str, double angle)
{

  DPOINT pointF = dPOINT(WorldToBBx(s, x), WorldToBBy(s, y));
  int nchar = strlen(str);

  ASSERT(s->m_pGraphics);
  SelectFontToMemDC("Verdana", abs(s->m_curState.m_font.lfHeight), 0);
  //Gdiplus::SolidBrush  solidBrush(s->m_curState.m_curColor);

  angle = NormalizeAngle(angle);

  if (nchar) {
    pointF.y -= s->m_curState.m_font.lfHeight;

    if (angle == 0) {
      draw_textPt(s->m_pGraphics, str, nchar, pointF.x, pointF.y, 0, s->m_curState.m_curColor);
    }
    else {
#if 0
      s->m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
      s->m_pGraphics->TranslateTransform(pointF.X, pointF.Y);
      s->m_pGraphics->RotateTransform(-angle);
      s->m_pGraphics->TranslateTransform(-pointF.X, -pointF.Y);
      s->m_pGraphics->gfx_DrawString(m_wBuffer, -1, &font, pointF, &sf, &solidBrush);
#endif
    }
  }

}

void gfx_DrawTextRight(gfx_t* s, double x, double y, const char* str, double angle)
{
  DPOINT pointF = dPOINT(WorldToBBx(s, x), WorldToBBy(s, y));
  int nchar = strlen(str);
  ASSERT(s->m_pGraphics);
  SelectFontToMemDC("Verdana", abs(s->m_curState.m_font.lfHeight), 0);

  angle = NormalizeAngle(angle);

  if (nchar) {
    pointF.y -= s->m_curState.m_font.lfHeight;

    if (angle == 0) {
      draw_textPt(s->m_pGraphics, str, nchar, pointF.x, pointF.y, 0, s->m_curState.m_curColor);
    }
    else {
#if 0
      graphicsContainer = s->m_pGraphics->BeginContainer();
      s->m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
      s->m_pGraphics->TranslateTransform(pointF.X, pointF.Y);
      s->m_pGraphics->RotateTransform(-angle);
      s->m_pGraphics->TranslateTransform(-pointF.X, -pointF.Y);
      s->m_pGraphics->gfx_DrawString(m_wBuffer, -1, &font, pointF, &sf, &solidBrush);
      s->m_pGraphics->EndContainer(graphicsContainer);
#endif
    }
  }

}

void gfx_DrawTextCenter(gfx_t* s, double x, double y, const char* str, double angle)
{

  DPOINT pointF = dPOINT(WorldToBBx(s, x), WorldToBBy(s, y));
  int nchar = strlen(str);
  ASSERT(s->m_pGraphics);
  SelectFontToMemDC("Verdana", abs(s->m_curState.m_font.lfHeight), 0);

  angle = NormalizeAngle(angle);

  if (nchar) {
    pointF.y -= s->m_curState.m_font.lfHeight;

    if (angle == 0) {
      draw_textPt(s->m_pGraphics, str, nchar, pointF.x, pointF.y, 0, s->m_curState.m_curColor);
    }
    else {
#if 0
      graphicsContainer = s->m_pGraphics->BeginContainer();
      s->m_pGraphics->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
      s->m_pGraphics->TranslateTransform(pointF.X, pointF.Y);
      s->m_pGraphics->RotateTransform(-angle);
      s->m_pGraphics->TranslateTransform(-pointF.X, -pointF.Y);
      s->m_pGraphics->gfx_DrawString(m_wBuffer, -1, &font, pointF, &sf, &solidBrush);
      s->m_pGraphics->EndContainer(graphicsContainer);
#endif
    }
  }

}


void gfx_DrawTextStrip(gfx_t* s, int n, const DPOINT* vPoints, const char** vStrings, int ta, double angle)
{
}

void SetDash(gfx_t* s, const char* dash)
{
  double length;
  char* str = (char*)dash;

  //s->m_curState.m_dashlen = 0;

  // no dashing
  if (dash == "") {
    if (s->m_pDash) {
      SAFEFREE(s->m_pDash);
      s->m_pDash = NULL;
    }

    return;
  }


  while (*str) {
    length = strtod(str, &str);
    MEMPUSH(double, s->m_pDash, s->m_dashlen);
    s->m_pDash[s->m_dashlen++] = length;
  }

}


int GetEncoderClsid(gfx_t* s, const WCHAR* format, CLSID* pClsid)
{
  return -1;  // Failure
}

Rectd MeasureString(gfx_t* s, const char* str)
{
  Rectd  boundingBox = {0};
  ASSERT(s->m_pGraphics);

  return boundingBox;
}

void PushClipWorld(gfx_t* s, double llx, double lly, double urx, double ury)
{
  ASSERT(llx < urx);
  ASSERT(lly < ury);
  MEMPUSH(SRectF, s->m_lClips, s->m_lClipLen);
  s->m_lClips[s->m_lClipLen++] = s->m_rCurClip;
  s->m_rCurClip.left = WorldToBBx(s, llx), s->m_rCurClip.width = WorldToBBAbsx(s, urx - llx);
  s->m_rCurClip.top = WorldToBBy(s, ury), s->m_rCurClip.height = WorldToBBAbsy(s, ury - lly);
}

void PopClipWorld(gfx_t* s)
{
  ASSERT(s->m_lClipLen > 0);
  s->m_rCurClip = s->m_lClips[--s->m_lClipLen];
}

void SetColor(gfx_t* s, double r, double g, double b, double a)
{
  s->m_curState.m_curColor = rgba(r, g, b, a);
}
void SetFillColor(gfx_t* s, double r, double g, double b, double a)
{
  s->m_curState.m_fillColor = rgba(r, g, b, a);
}
void SetLineWidth(gfx_t* s, double width)
{
  s->m_curState.m_lineWidth = MAX(0, width);
}
void SetFont(gfx_t* s, long scale)
{
  s->m_curState.m_font.lfHeight = scale;
}
void gfx_DrawVectorField(gfx_t* s, int nx, int ny, double llx, double lly, double dx, double dy, double* pV, double lineWidth) {}
