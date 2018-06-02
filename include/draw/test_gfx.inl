
#include "draw\gfx.inl"

void TestArrow(gfx_t* s)
{
  double llx = 0;
  double lly = 0;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;

  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);

  // Testing simple arrow
  DrawArrow(s, llx + w * 0.5, lly + h * 0.5, w * 0.4, h * 0.4, 1, 6, 7, true);
  DrawArrow(s, llx + w * 0.5, lly + h * 0.4, -w * 0.2, -h * 0.1, 0, 10, 15, false);
  PopClipWorld(s);
  PopState(s);
  DrawRectf(s, llx, lly, urx, ury, false);
}

void TestLine(gfx_t* s)
{
  double llx = 0.25;
  double lly = 0;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;
  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);
  // Testing simple line
  // adding a line.
  DrawLine(s, llx + w * 0.1, lly + h * 0.1, llx + w * 0.9, lly + h * 0.9);
  PopClipWorld(s);
  PopState(s);
  DrawRectf(s, llx, lly, urx, ury, false);
}

void TestLineStrip(gfx_t* s)
{
  double llx = 0.5;
  double lly = 0;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;

  // testing line strip. (open)
  int i;
  enum { n = 15 };
  double pX[n];
  double pY[n];

  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);

  for (i = 0; i < n; i++) {
    pX[i] = llx + i * w / (n - 1) / 2;
    pY[i] = (ury + lly) / 2.0 * ((1 + 2 / 3.0 * sin(i / (double)(n - 1) * 3.14)));
  }

  DrawLineStrip(s, n, pX, pY, true, false);
  DrawTextCenter(s, pX[n / 2], pY[n / 2], "Line strip (open)", 0);

  // testing line strip. (closed)
  for (i = 0; i < n; i++) {
    pX[i] = llx + w * 0.3 + w / 8.0 * cos(i / (double)(n - 1) * 3.14);
    pY[i] = (ury + lly) / 4.0 * (1 + 3 / 5.0 * pow(sin(i / (double)(n - 1) * 3.14), 2));
  }

  DrawLineStrip(s, n, pX, pY, false, false);
  DrawTextCenter(s, pX[n / 2], pY[n / 2], "Line strip (closed)", 0);

  for (i = 0; i < n; i++) {
    pX[i] = llx + w * 0.7 + w / 8.0 * cos(i / (double)(n - 1) * 3.14);
    pY[i] = lly + h * 0.5 * sin(i / (double)(n - 1) * 3.14);
  }

  SetFillColor(s, 0.5, 0.5, 1, 0.8);
  DrawLineStrip(s, n, pX, pY, false, true);
  DrawTextCenter(s, pX[n / 2], pY[n / 2], "Line strip (closed and filled)", 0);
  PopClipWorld(s);
  PopState(s);
  DrawRectf(s, llx, lly, urx, ury, false);
}

void TestStepStrip(gfx_t* s)
{
  double llx = 0.75;
  double lly = 0;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;


  // testing line strip. (open)
  int i;
  enum { n = 15 };
  double pX[n];
  double pY[n];

  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);

  for (i = 0; i < n; i++) {
    pX[i] = llx + i * w / (n - 1) / 2;
    pY[i] = (ury + lly) / 2.0 * ((1 + 2 / 3.0 * sin(i / (double)(n - 1) * 3.14)));
  }

  DrawStepStrip(s, n, pX, pY, true, false);
  DrawTextCenter(s, pX[n / 2], pY[n / 2], "Step strip (open)", 0);

  // testing line strip. (closed)
  for (i = 0; i < n; i++) {
    pX[i] = llx + w * 0.3 + w / 8.0 * cos(i / (double)(n - 1) * 3.14);
    pY[i] = (ury + lly) / 4.0 * (1 + 3 / 5.0 * pow(sin(i / (double)(n - 1) * 3.14), 2));
  }

  DrawStepStrip(s, n, pX, pY, false, false);
  DrawTextCenter(s, pX[n / 2], pY[n / 2], "Step strip (closed)", 0);

  for (i = 0; i < n; i++) {
    pX[i] = llx + w * 0.7 + w / 8.0 * cos(i / (double)(n - 1) * 3.14);
    pY[i] = lly + h * 0.1 + h * 0.5 * sin(i / (double)(n - 1) * 3.14);
  }

  SetFillColor(s, 0.5, 0.5, 1, 0.8);
  DrawStepStrip(s, n, pX, pY, false, true);
  DrawTextCenter(s, pX[n / 2], pY[n / 2], "Step strip (closed and filled)", 0);
  PopClipWorld(s);
  PopState(s);
  DrawRectf(s, llx, lly, urx, ury, false);
}


void TestGraphicState(gfx_t* s)
{
  double llx = 0;
  double lly = 0.25;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;
  double x;

  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);
  // setting font for text
  SetFont(s, 10);

  // Testing linewidth
  x = llx + w * 0.05;
  DrawTextLeft(s, x, lly + w * 0.6, "Line width", 0);
  SetLineWidth(s, 1);
  DrawLine(s, x, lly, x, lly + w * 0.5);
  x += w * 0.1;
  SetLineWidth(s, 2);
  DrawLine(s, x, lly, x, lly + w * 0.5);
  x += w * 0.1;
  SetLineWidth(s, 4);
  DrawLine(s, x, lly, x, lly + w * 0.5);

  // Testing color
  x += w * 0.15;
  SetColor(s, 1, 0, 0, 1);
  DrawTextLeft(s, x, lly + w * 0.6, "Color", 0);
  SetLineWidth(s, 2);
  SetColor(s, 1, 0, 0, 1);
  DrawLine(s, x, lly, x, lly + w * 0.5);
  x += w * 0.1;
  SetColor(s, 0, 0, 1, 1);
  DrawLine(s, x, lly, x, lly + w * 0.5);
  x += w * 0.1;
  SetColor(s, 0.8, 0.8, 0, 1);
  DrawLine(s, x, lly, x, lly + w * 0.5);

  // Testing dash
  x += w * 0.15;
  SetColor(s, 0, 0, 0, 1);
  DrawTextLeft(s, x, lly + w * 0.6, "Dash", 0);
  SetDash(s, "4 1");
  DrawLine(s, x, lly, x, lly + w * 0.5);
  x += w * 0.1;
  SetDash(s, "4 4 1");
  DrawLine(s, x, lly, x, lly + w * 0.5);
  x += w * 0.1;
  SetDash(s, "2 2");
  DrawLine(s, x, lly, x, lly + w * 0.5);
  PopClipWorld(s);
  PopState(s);
  DrawRectf(s, llx, lly, urx, ury, false);
}


void TestGrid(gfx_t* s)
{
  double llx = 0.25;
  double lly = 0.25;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;
  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);

  SetDash(s, "1 3");
  SetLineWidth(s, 1.5);
  DrawGridHor(s, llx, urx, lly, (ury + lly) / 2, (ury - lly) / 15);
  SetDash(s, "3 1 3");
  SetLineWidth(s, 2);
  DrawGridVer(s, llx, urx, (ury + lly) / 2, ury, (urx - llx) / 20);
  PopClipWorld(s);
  PopState(s);
  DrawRectf(s, llx, lly, urx, ury, false);
}

void TestShapes(gfx_t* s)
{
  double x;
  double llx = 0.5;
  double lly = 0.25;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;
  int i;
  enum { n = 20};
  double pX[n];
  double pY[n];

  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);

  // Testing box, triangles and others
  x = llx + w * 0.2;
  DrawTextCenter(s, x, (ury - lly) / 2 + h * 0.05, "Single shapes", 30);
  SetFillColor(s, 0.5, 0.0, 0.5, 0.5);
  DrawBox(s, x, (ury - lly) / 2, 5, 10, false);
  DrawBox(s, x, (ury - lly) / 2 - h * .2, 30, 15, true);
  x += w * 0.05;
  SetFillColor(s, 0.5, 0.5, 0.5, 0.5);
  DrawCircle(s, x, (ury - lly) / 2, 7, false);
  DrawCircle(s, x, (ury - lly) / 2 - h * 0.2, 15, true);
  x += w * 0.05;
  SetFillColor(s, 0.0, 0.5, 0.5, 0.5);
  DrawTriangle(s, x, (ury - lly) / 2, 8, false);
  DrawTriangle(s, x, (ury - lly) / 2 - h * 0.2, 15, true);
  x += w * 0.05;
  SetFillColor(s, 0.5, 0.5, 0.0, 0.5);
  DrawSquare(s, x, (ury - lly) / 2, 10, false);
  DrawSquare(s, x, (ury - lly) / 2 - h * 0.2, 15, true);

  // testing line strip. (open)
  for (i = 0; i < n; i++) {
    pX[i] = llx + w / (n) * (i + 1) * 0.9;
    pY[i] = lly + h * 0.5 + h / 3.0 * sin(2 * i / (double)(n - 1) * 3.14);
  }

  DrawTextCenter(s, pX[n / 2] + w * 0.1, pY[n / 2] + h * 0.1, "Shape strip", 0);

  SetFillColor(s, 0, 0, 0.8, 0.5);
  DrawSquareStrip(s, n, pX, pY, 5, true);
  DrawCircleStrip(s, n, pX, pY, 10, false);
  SetColor(s, 0.8, 0, 1, 1);
  DrawTriangleStrip(s, n, pX, pY, 10, false);
  SetColor(s, 1, 0, 0, 1);
  DrawLineStrip(s, n, pX, pY, true, false);
  PopClipWorld(s);
  PopState(s);
  DrawRectf(s, llx, lly, urx, ury, false);
}

void TestSurf(gfx_t* s)
{
  double llx = 0.75;
  double lly = 0.25;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;

  // testing surf...
  enum {    nx = 10,    ny = 15,  };
  int i, j, k;
  double dx = w / (nx - 1);
  double dy = h / (ny - 1);
  double pColor[3 * nx * ny];

  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);

  for (i = 0; i < ny; i++) {
    for (j = 0; j < nx; j++) {
      k = i * nx + j;
      pColor[k * 3 + 0] = (j * j * dx * dx) / (w * w);
      pColor[k * 3 + 1] = (j * dx + i * dy) / (w + h);
      pColor[k * 3 + 2] = (i * dx * j * dy) / (w * h) - 0.1;
    }
  }

  DrawSurf(s, nx, ny, llx, lly, dx, dy, pColor);
  PopClipWorld(s);
  PopState(s);
  DrawRectf(s, llx, lly, urx, ury, false);
}

void TestVectorField(gfx_t* s)
{
  double llx = 0;
  double lly = 0.5;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;

  // testing surf...
  enum {    nx = 10,    ny = 15,  };
  int i, j, k;
  double dx = w / (nx - 1);
  double dy = h / (ny - 1);
  // testing vector field
  double pV[2 * nx * ny];
  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);

  for (i = 0; i < ny; i++) {
    for (j = 0; j < nx; j++) {
      k = i * nx + j;
      pV[k * 2] = 0.2 * cos(j * dx / w * 3.14);
      pV[k * 2 + 1] = 0.2 * sin(2 * i * dy / w * 3.14);
    }
  }

  DrawVectorField(s, nx, ny, llx, lly, dx, dy, pV, 1.5);
  PopClipWorld(s);
  PopState(s);
  DrawRectf(s, llx, lly, urx, ury, false);
}

void TestRect(gfx_t* s)
{
  int i;
  double llx = 0.25;
  double lly = 0.5;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;
  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);

  // Testing simple line
  // adding a line.
  for (i = 0; i < 11; i++) {
    SetColor(s, i / (float)10, 0, (9 - i) / (float)10, 1);
    DrawRectf(s, llx + w * 0.5 + w * 0.2 * sin(i * 3.14 / 10), lly + h * 0.4 + h * 0.2 * cos(i * 3.14 / 10), llx + w * 0.6 + w * 0.2 * sin(i * 3.14 / 10), lly + h * 0.5 + h * 0.2 * cos(i * 3.14 / 10), false);
  }

  for (i = 0; i > -11; i--) {
    SetFillColor(s, -i / (float)10, 0, (9 + i) / (float)10, (9 + i) / (float)15 + 0.4);
    SetColor(s, 0, 0, 0, 1 - ((9 + i) / (float)15 + 0.4));
    DrawRectf(s, llx + w * 0.4 + w * 0.2 * sin(i * 3.14 / 10), lly + h * 0.4 + h * 0.2 * cos(i * 3.14 / 10), llx + w * 0.5 + w * 0.2 * sin(i * 3.14 / 10), lly + h * 0.5 + h * 0.2 * cos(i * 3.14 / 10), true);
  }

  PopClipWorld(s);
  PopState(s);
  DrawRectf(s, llx, lly, urx, ury, false);
}

void TestText(gfx_t* s)
{
  double llx = 0.5;
  double lly = 0.5;
  double w = 0.25;
  double h = 0.25;
  double urx = llx + w;
  double ury = lly + h;
  PushState(s);
  PushClipWorld(s, llx, lly, urx, ury);
  SetColor(s, 0, 0, 0, 1);
  SetFont(s, 6);

  // Testing simple line
  // adding a line.
  DrawTextLeft(s, llx + w * 0.3, lly + h * 0.1, "Left text", 0);
  DrawTextRight(s, llx + w * 0.3, lly + h * 0.2, "Right text", 0);
  DrawTextCenter(s, llx + w * 0.3, lly + h * 0.3, "Centered text", 0);

  SetColor(s, 0, 0, 1, 1);
  DrawTextLeft(s, llx + w * 0.7, lly + h * 0.5, "Left text, vertical", 90);
  DrawTextRight(s, llx + w * 0.8, lly + h * 0.5, "right text, vertical", 90);
  DrawTextCenter(s, llx + w * 0.9, lly + h * 0.5, "Centered text, vertical", 90);

  SetColor(s, 1, 0, 0, 1);
  DrawTextLeft(s, llx + w * 0.4, lly + h * 0.8, "Left text, 390deg", 390);
  DrawTextRight(s, llx + w * 0.5, lly + h * 0.8, "right text, -330deg", -330);
  DrawTextCenter(s, llx + w * 0.6, lly + h * 0.8, "Centered text, 30deg", 30);

#if 0
  std::vector<std::string> vString(5);
  std::vector<gfxinterface::SPointF> vPoints(5);
  vString[0] = "This";
  vString[1] = "is";
  vString[2] = "a";
  vString[3] = "text";
  vString[4] = "strip";
  vPoints[0].x = llx + w * 0.1;
  vPoints[0].y = lly + h * 0.9;
  vPoints[1].x = llx + w * 0.15;
  vPoints[1].y = lly + h * 0.9;
  vPoints[2].x = llx + w * 0.2;
  vPoints[2].y = lly + h * 0.9;
  vPoints[3].x = llx + w * 0.25;
  vPoints[3].y = lly + h * 0.9;
  vPoints[4].x = llx + w * 0.3;
  vPoints[4].y = lly + h * 0.9;
  SetColor(s, 0, 0.8, 0);
  s.DrawTextStrip(vPoints, vString, TextAlignmentLeft, 25);

  PopClipWorld(s);
  PopState(s);
  DrawRect(s, llx, lly, urx, ury);
#endif
}


/////////////////////////////////////////////////////////////////////////////
// CIGfxTestView drawing
int test_gfx(HDDC pDC)
{
  RECT rect = {0, 0, pDC->w, pDC->h};
  gfx_t s[1] = {0};
  int nWidth = rect.right - rect.left + 1;
  int nHeight = rect.bottom - rect.top + 1;

  double llx = 0;
  double urx = 1;
  double lly = 0;
  double ury = 1;
  double w = urx - llx;
  double h = ury - lly;
  s->m_pGraphics[0] = *pDC;
  gfx_set(s);
  SetMainBoundingBox(s, 0, 0, nWidth, nHeight);
  SetWorldBoundingBox(s, llx, lly, urx, ury);

  gfx_BeginPaint(s);
  PushState(s);
  TestArrow(s);
  PopState(s);

  PushState(s);
  TestGraphicState(s);
  PopState(s);

  PushState(s);
  TestGrid(s);
  PopState(s);

  PushState(s);
  TestLine(s);
  PopState(s);

  PushState(s);
  TestLineStrip(s);
  PopState(s);

  PushState(s);
  TestRect(s);
  PopState(s);

  PushState(s);
  TestShapes(s);
  PopState(s);

  PushState(s);
  TestStepStrip(s);
  PopState(s);

  PushState(s);
  TestSurf(s);
  PopState(s);

  PushState(s);
  TestVectorField(s);
  PopState(s);

  PushState(s);
  TestText(s);
  PopState(s);

  gfx_EndPaint(s);
  gfx_free(s);
  return 0;
}

