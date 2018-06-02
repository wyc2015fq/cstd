// 中点法画圆
int draw_circle1(HDDC hDC, int xc, int yc, int r, COLORREF clr, int wline)
{
  int i;
  int x = 0, y = r, d = 3 - 2 * r;

  while (x <= y) {
    _SetPixel(hDC, xc + x, yc + y, clr);
    _SetPixel(hDC, xc + x, yc - y, clr);
    _SetPixel(hDC, xc - x, yc + y, clr);
    _SetPixel(hDC, xc - x, yc - y, clr);
    _SetPixel(hDC, xc + y, yc + x, clr);
    _SetPixel(hDC, xc + y, yc - x, clr);
    _SetPixel(hDC, xc - y, yc + x, clr);
    _SetPixel(hDC, xc - y, yc - x, clr);

    if (!wline) {
      for (i = xc + x; i >= xc - x; i--) {
        _SetPixel(hDC, i, yc + y, clr);
        _SetPixel(hDC, i, yc - y, clr);
      }

      for (i = xc + y; i >= xc - y; i--) {
        _SetPixel(hDC, i, yc + x, clr);
        _SetPixel(hDC, i, yc - x, clr);
      }
    }

    if (d < 0) {
      d += 4 * x + 6;
    }
    else {
      d += 4 * (x - y) + 10;
      y--;
    }

    x++;
  }

  return 0;
}
int draw_circle_map(int r, int* mapx)
{
  int x = 0, y = r, d = 3 - 2 * r;

  while (x <= y) {
    mapx[x] = y;
    mapx[y] = x;

    if (d < 0) {
      d += 4 * x + 6;
    }
    else {
      d += 4 * (x - y) + 10;
      y--;
    }

    x++;
  }

  return 0;
}
// wline 线宽 0 表示填充
int draw_circle(HDDC hDC, int xc, int yc, int r, COLORREF clr, int wline)
{
  int buf[4096] = {0};
  int* map1 = buf;
  int* map2 = buf + r + wline + 1;
  int i, j, r1 = r + wline / 2, r2 = r1 - wline;
  ASSERT(r + r < 4096);
  draw_circle_map(r1, map1);
  draw_circle_map(r2, map2);

  for (i = 0; i <= r1; ++i) {
    for (j = map2[i]; j <= map1[i]; ++j) {
      _SetPixel(hDC, xc + j, yc - i, clr);
      _SetPixel(hDC, xc + j, yc + i, clr);
      _SetPixel(hDC, xc - j, yc - i, clr);
      _SetPixel(hDC, xc - j, yc + i, clr);
    }
  }

  return 0;
}
#define setdot() \
  _SetPixel(hDC, xc2 + x, yc2 + y, clr); _SetPixel(hDC, xc2 + x, yc1 - y, clr); \
  _SetPixel(hDC, xc1 - x, yc2 + y, clr); _SetPixel(hDC, xc1 - x, yc1 - y, clr); \
  if (isfill) { for (i = xc2 + x; i >= xc1 - x; i--) { \
      _SetPixel(hDC, i, yc2 + y, clr); _SetPixel(hDC, i, yc1 - y, clr); } }
//中点法画椭圆
//#define draw_ellipse(hDC, xc1, yc1, rx, ry, clr, isfill) draw_roundrect(hDC, xc1, yc1, xc1, yc1, rx, ry, clr, isfill)
#define draw_roundrectR(hDC, prc, cx, cy, clr, isfill) draw_roundrect(hDC, (prc)->left, (prc)->top, (prc)->right, (prc)->bottom, cx, cy, clr, isfill)
#define draw_fillellipse(hDC, x1, y1, x2, y2, clr) draw_roundrect(hDC, (x1+x2)/2, yc1, xc1, yc1, rx, ry, clr, isfill)
//#define draw_ellipseR(hDC, prc, clr, isfill)
// 圆角矩形
// rx, ry 外扩边界
int draw_roundrect(HDDC hDC, int xc1, int yc1, int xc2, int yc2, int rx, int ry, COLORREF clr, int isfill)
{
  int rx2, ry2, rx22, ry22;
  int dx, dy, d;
  int i, x, y;
  --xc2, --yc2;
  rx2 = rx * rx, ry2 = ry * ry, rx22 = 2 * rx2, ry22 = 2 * ry2;
  dx = 0, dy = rx22 * ry;
  x = 0, y = ry;
  isfill = !isfill;
  //upper
  setdot();
  isfill = !isfill;
  d = roundl(ry2 - rx2 * ry + (rx2 >> 2));

  while (dx < dy) {
    x++;
    dx += ry22;

    if (d < 0) {
      //d<0在圆内
      d += ry2 + dx;
    }
    else {
      y--;
      dy -= rx22;
      d += ry2 + dx - dy;
    }

    setdot();
  }

  //lower
  d = roundl(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2);

  while (y > 0) {
    y--;
    dy -= rx22;

    if (d < 0) {
      x++;
      dx += ry22;
      d += rx2 + dx - dy;
    }
    else {
      d += rx2 - dy;
    }

    setdot();
  }

  for (x = rx, y = (yc1 - yc2) / 2; y < 0; ++y) {
    setdot();
  }

  return 0;
}
#undef setdot
#define GRADIENTRGBF(_I1, _V1, _V2) _RGB((UCHAR)(_I1 * GetRV(_V1) + (1.0f - _I1) * GetRV(_V2)), (UCHAR)(_I1 * GetGV(_V1) + (1.0f - _I1) * GetGV(_V2)), (UCHAR)(_I1 * GetBV(_V1) + (1.0f - _I1) * GetBV(_V2)))
#define TEllipse(hDC, rc, clr) TDrawEllipse(hDC, rc, clr, 0 )
int draw_ellipse_aa(HDDC hDC, double cx, double cy, double rx, double ry, COLORREF clr)
{
  double rel = sqrt(rx * rx + ry * ry) / ry;
  COLORREF bg;
  int xs, ys;
  double xt, yt;
  double distance;
  rx = ABS(rx);
  ry = ABS(ry);

  for (xs = 0; xs <= rx; xs++) {
    yt = sqrt(rx * rx - xs * xs) * ry / rx;
    distance = yt - (int)yt;
    //y = (int)(yt*(1<<16)); iy = y>>16; bg = _GetPixel(hDC, cx + xs, cy + (int)yt);
    bg = _GetPixel(hDC, cx + xs, cy + (int)yt), _SetPixel(hDC, cx + xs, cy + (int)yt, GRADIENTRGBF(distance, bg, clr));
    bg = _GetPixel(hDC, cx + xs, cy + (int)yt + 1), _SetPixel(hDC, cx + xs, cy + (int)yt + 1, GRADIENTRGBF(distance, clr, bg));
    bg = _GetPixel(hDC, cx - xs, cy - (int)yt), _SetPixel(hDC, cx - xs, cy - (int)yt, GRADIENTRGBF(distance, bg, clr));
    bg = _GetPixel(hDC, cx - xs, cy - (int)yt - 1), _SetPixel(hDC, cx - xs, cy - (int)yt - 1, GRADIENTRGBF(distance, clr, bg));
    bg = _GetPixel(hDC, cx - xs, cy + (int)yt), _SetPixel(hDC, cx - xs, cy + (int)yt, GRADIENTRGBF(distance, bg, clr));
    bg = _GetPixel(hDC, cx - xs, cy + (int)yt + 1), _SetPixel(hDC, cx - xs, cy + (int)yt + 1, GRADIENTRGBF(distance, clr, bg));
    bg = _GetPixel(hDC, cx + xs, cy - (int)yt), _SetPixel(hDC, cx + xs, cy - (int)yt, GRADIENTRGBF(distance, bg, clr));
    bg = _GetPixel(hDC, cx + xs, cy - (int)yt - 1), _SetPixel(hDC, cx + xs, cy - (int)yt - 1, GRADIENTRGBF(distance, clr, bg));
  }

  for (ys = - (int)((float)ry / rel); ys <= (int)((float)ry / rel); ys++) {
    xt = sqrt(ry * ry - ys * ys) * rx / ry;
    distance = xt - (int)xt;
    bg = _GetPixel(hDC, cx + (int)xt, cy + ys), _SetPixel(hDC, cx + (int)xt, cy + ys, GRADIENTRGBF(distance, bg, clr));
    bg = _GetPixel(hDC, cx + (int)xt + 1, cy + ys), _SetPixel(hDC, cx + (int)xt + 1, cy + ys, GRADIENTRGBF(distance, clr, bg));
    bg = _GetPixel(hDC, cx - (int)xt, cy - ys), _SetPixel(hDC, cx - (int)xt, cy - ys, GRADIENTRGBF(distance, bg, clr));
    bg = _GetPixel(hDC, cx - (int)xt - 1, cy - ys), _SetPixel(hDC, cx - (int)xt - 1, cy - ys, GRADIENTRGBF(distance, clr, bg));
    bg = _GetPixel(hDC, cx + (int)xt, cy - ys), _SetPixel(hDC, cx + (int)xt, cy - ys, GRADIENTRGBF(distance, bg, clr));
    bg = _GetPixel(hDC, cx + (int)xt + 1, cy - ys), _SetPixel(hDC, cx + (int)xt + 1, cy - ys, GRADIENTRGBF(distance, clr, bg));
    bg = _GetPixel(hDC, cx - (int)xt, cy + ys), _SetPixel(hDC, cx - (int)xt, cy + ys, GRADIENTRGBF(distance, bg, clr));
    bg = _GetPixel(hDC, cx - (int)xt - 1, cy + ys), _SetPixel(hDC, cx - (int)xt - 1, cy + ys, GRADIENTRGBF(distance, clr, bg));
  }

  return 0;
}
int draw_arc(HDDC hDC, int centerX, int centerY, int radiusX, int radiusY, double startAngle, double endAngle, COLORREF clr)
{
  COLORREF bgColor;
  double distance;
  UCHAR r, g, b;
  double angle = 0.0f;
  radiusX = (ABS(radiusX));
  radiusY = (ABS(radiusY));

  while ((angle + 90.0f) < startAngle) {
    angle += 90.0f;
  }

  while (angle < endAngle) {
    if (startAngle >= angle) {
      double eAngle, sx, sy;
      int startY, endY, deltaY;
      int startX, endX, deltaX;

      if (endAngle <= angle + 90.0f) {
        eAngle = endAngle;
      }
      else {
        eAngle = angle + 90.0f;
      }

      sx = (double)(radiusX * cos((startAngle / 180.0f) * PI));

      if (sx > 0.0f) {
        sx += 0.5f;
      }
      else {
        sx -= 0.5f;
      }

      startX = (int)sx;
      endX = (int)(radiusX * cos((eAngle / 180.0f) * PI));

      if (endX - startX != 0) {
        deltaX = (int)((endX - startX) / fabs(endX - startX));
      }
      else {
        deltaX = 0;
      }

      sy = (double)(radiusY * sin((startAngle / 180.0f) * PI));

      if (sy > 0.0f) {
        sy += 0.5f;
      }
      else {
        sy -= 0.5f;
      }

      startY = (int)sy;
      endY = (int)(radiusY * sin((eAngle / 180.0f) * PI));

      if (endY - startY != 0) {
        deltaY = (int)((endY - startY) / fabs(endY - startY));
      }
      else {
        deltaY = 0;
      }

      if (deltaX != 0) {
        int oldY = startY;
        int xs = startX;
        double yt;

        while (xs != endX) {
          yt = (double)(radiusY * sqrt(1.0f - (double)(xs * xs) / (double)(radiusX * radiusX)));
          distance = (double)(yt - (int)yt);

          if (abs(oldY - (int)yt) < 2) {
            if (deltaX < 0) {
              if (deltaY > 0) {
                bgColor = _GetPixel(hDC, centerX + xs, centerY + (int)yt);
              }
              else {
                bgColor = _GetPixel(hDC, centerX + xs, centerY + (int)yt);
              }
            }
            else {
              if (deltaY < 0) {
                bgColor = _GetPixel(hDC, centerX + xs, centerY - (int)yt);
              }
              else {
                bgColor = _GetPixel(hDC, centerX + xs, centerY - (int)yt);
              }
            }

            r = (UCHAR)(distance * GetRV(bgColor)) + (UCHAR)((1.0f - distance) * GetRV(clr));
            g = (UCHAR)(distance * GetGV(bgColor)) + (UCHAR)((1.0f - distance) * GetGV(clr));
            b = (UCHAR)(distance * GetBV(bgColor)) + (UCHAR)((1.0f - distance) * GetBV(clr));

            if (deltaX < 0) {
              if (deltaY > 0) {
                _SetPixel(hDC, centerX + xs, centerY + (int)yt, _RGB(r, g, b));
              }
              else {
                _SetPixel(hDC, centerX + xs, centerY + (int)yt, _RGB(r, g, b));
              }
            }
            else {
              if (deltaY < 0) {
                _SetPixel(hDC, centerX + xs, centerY - (int)yt, _RGB(r, g, b));
              }
              else {
                _SetPixel(hDC, centerX + xs, centerY - (int)yt, _RGB(r, g, b));
              }
            }

            if (deltaX < 0) {
              if (deltaY > 0) {
                bgColor = _GetPixel(hDC, centerX + xs, centerY + (int)yt + 1);
              }
              else {
                bgColor = _GetPixel(hDC, centerX + xs, centerY + (int)yt + 1);
              }
            }
            else {
              if (deltaY < 0) {
                bgColor = _GetPixel(hDC, centerX + xs, centerY - (int)yt - 1);
              }
              else {
                bgColor = _GetPixel(hDC, centerX + xs, centerY - (int)yt - 1);
              }
            }

            r = (UCHAR)((1.0f - distance) * GetRV(bgColor)) + (UCHAR)(distance * GetRV(clr));
            g = (UCHAR)((1.0f - distance) * GetGV(bgColor)) + (UCHAR)(distance * GetGV(clr));
            b = (UCHAR)((1.0f - distance) * GetBV(bgColor)) + (UCHAR)(distance * GetBV(clr));

            if (deltaX < 0) {
              if (deltaY > 0) {
                _SetPixel(hDC, centerX + xs, centerY + (int)yt + 1, _RGB(r, g, b));
              }
              else {
                _SetPixel(hDC, centerX + xs, centerY + (int)yt + 1, _RGB(r, g, b));
              }
            }
            else {
              if (deltaY < 0) {
                _SetPixel(hDC, centerX + xs, centerY - (int)yt - 1, _RGB(r, g, b));
              }
              else {
                _SetPixel(hDC, centerX + xs, centerY - (int)yt - 1, _RGB(r, g, b));
              }
            }
          }

          oldY = (int)yt;
          xs += deltaX;
        }
      }

      if (deltaY != 0) {
        int oldX = startX;
        int ys = startY;
        double xt;

        while (ys != endY) {
          xt = (double)(radiusX * sqrt(1.0f - (double)(ys * ys) / (double)(radiusY * radiusY)));
          distance = (double)(xt - (int)xt);

          if (abs(oldX - (int)xt) < 2) {
            if (deltaX < 0) {
              if (deltaY > 0) {
                bgColor = _GetPixel(hDC, centerX + (int)xt, centerY + ys);
              }
              else {
                bgColor = _GetPixel(hDC, centerX - (int)xt, centerY + ys);
              }
            }
            else {
              if (deltaY < 0) {
                bgColor = _GetPixel(hDC, centerX - (int)xt, centerY + ys);
              }
              else {
                bgColor = _GetPixel(hDC, centerX + (int)xt, centerY + ys);
              }
            }

            r = (UCHAR)(distance * GetRV(bgColor)) + (UCHAR)((1.0f - distance) * GetRV(clr));
            g = (UCHAR)(distance * GetGV(bgColor)) + (UCHAR)((1.0f - distance) * GetGV(clr));
            b = (UCHAR)(distance * GetBV(bgColor)) + (UCHAR)((1.0f - distance) * GetBV(clr));

            if (deltaX < 0) {
              if (deltaY > 0) {
                _SetPixel(hDC, centerX + (int)xt, centerY + ys, _RGB(r, g, b));
              }
              else {
                _SetPixel(hDC, centerX - (int)xt, centerY + ys, _RGB(r, g, b));
              }
            }
            else {
              if (deltaY < 0) {
                _SetPixel(hDC, centerX - (int)xt, centerY + ys, _RGB(r, g, b));
              }
              else {
                _SetPixel(hDC, centerX + (int)xt, centerY + ys, _RGB(r, g, b));
              }
            }

            if (deltaX < 0) {
              if (deltaY > 0) {
                bgColor = _GetPixel(hDC, centerX + (int)xt + 1, centerY + ys);
              }
              else {
                bgColor = _GetPixel(hDC, centerX - (int)xt - 1, centerY + ys);
              }
            }
            else {
              if (deltaY < 0) {
                bgColor = _GetPixel(hDC, centerX - (int)xt - 1, centerY + ys);
              }
              else {
                bgColor = _GetPixel(hDC, centerX + (int)xt + 1, centerY + ys);
              }
            }

            r = (UCHAR)((1.0f - distance) * GetRV(bgColor)) + (UCHAR)(distance * GetRV(clr));
            g = (UCHAR)((1.0f - distance) * GetGV(bgColor)) + (UCHAR)(distance * GetGV(clr));
            b = (UCHAR)((1.0f - distance) * GetBV(bgColor)) + (UCHAR)(distance * GetBV(clr));

            if (deltaX < 0) {
              if (deltaY > 0) {
                _SetPixel(hDC, centerX + (int)xt + 1, centerY + ys, _RGB(r, g, b));
              }
              else {
                _SetPixel(hDC, centerX - (int)xt - 1, centerY + ys, _RGB(r, g, b));
              }
            }
            else {
              if (deltaY < 0) {
                _SetPixel(hDC, centerX - (int)xt - 1, centerY + ys, _RGB(r, g, b));
              }
              else {
                _SetPixel(hDC, centerX + (int)xt + 1, centerY + ys, _RGB(r, g, b));
              }
            }
          }

          oldX = (int)xt;
          ys += deltaY;
        }
      }
    }

    angle += 90.0f;
    startAngle = angle;
  }

  return 0;
}
#define draw_roundrect_aaR(hDC, prc, cx, cy, clr) draw_roundrect_aa(hDC, (prc)->left, (prc)->top, (prc)->right, (prc)->bottom, cx, cy, clr)
int draw_roundrect_aa(HDDC hDC, int x1, int y1, int x2, int y2, int cx, int cy, COLORREF clr)
{
  int dx = (x2 - x1);
  int dy = (y2 - y1);

  if ((cx > abs(dx)) || (cy > abs(dy))) {
    draw_ellipse_aa(hDC, (x2 + x1) / 2, (y2 + y1) / 2, dx / 2, dy / 2, clr);
  }
  else {
    TDrawLine(hDC, x1 + cx, y1, x2 - cx, y1, clr);
    TDrawLine(hDC, x2, y1 + cx, x2, y2 - cy, clr);
    TDrawLine(hDC, x2 - cx, y2, x1 + cx, y2, clr);
    TDrawLine(hDC, x1, y2 - cy, x1, y1 + cy, clr);
    draw_arc(hDC, x2 - cx, y1 + cy, cx, cy, 270.0f, 360.0f, clr);
    draw_arc(hDC, x2 - cx, y2 - cy, cx, cy, 0.0f, 90.0f, clr);
    draw_arc(hDC, x1 + cx, y2 - cy, cx, cy, 90.0f, 180.0f, clr);
    draw_arc(hDC, x1 + cx, y1 + cy, cx, cy, 180.0f, 270.0f, clr);
  }

  return 0;
}
int draw_pie(HDDC hDC, int centerX, int centerY, int radiusX, int radiusY, double startAngle, double endAngle, COLORREF clr)
{
  int startX = (int)(radiusX * cos((startAngle / 180.0f) * PI));
  int startY = (int)(radiusY * sin((startAngle / 180.0f) * PI));
  int endX = (int)(radiusX * cos((endAngle / 180.0f) * PI));
  int endY = (int)(radiusY * sin((endAngle / 180.0f) * PI));
  radiusX = (ABS(radiusX));
  radiusY = (ABS(radiusY));

  if (endX < 0) {
    if (endY > 0) {
      endY++;
    }
    else {
      endX--;
    }
  }
  else {
    if (endY < 0) {
      endY--;
    }
    else {
      endX++;
    }
  }

  draw_arc(hDC, centerX, centerY, radiusX, radiusY, startAngle, endAngle, clr);
  draw_line(hDC, centerX, centerY, centerX + startX, centerY + startY, clr);
  draw_line(hDC, centerX, centerY, centerX + endX, centerY + endY, clr);
  return 0;
}
int draw_chord(HDDC hDC, int centerX, int centerY, int radiusX, int radiusY, double startAngle, double endAngle, COLORREF clr)
{
  int startX = (int)(radiusX * cos((startAngle / 180.0f) * PI));
  int startY = (int)(radiusY * sin((startAngle / 180.0f) * PI));
  int endX = (int)(radiusX * cos((endAngle / 180.0f) * PI));
  int endY = (int)(radiusY * sin((endAngle / 180.0f) * PI));

  if (endX < 0) {
    if (endY > 0) {
      endY++;
    }
    else {
      endX--;
    }
  }
  else {
    if (endY < 0) {
      endY--;
    }
    else {
      endX++;
    }
  }

  draw_arc(hDC, centerX, centerY, radiusX, radiusY, startAngle, endAngle, clr);
  TDrawLine(hDC, centerX + startX, centerY + startY, centerX + endX, centerY + endY, clr);
  return 0;
}
int draw_bezier(HDDC hDC, DPOINT* points, int numPoints, COLORREF clr)
{
  int i, numCurves = (numPoints - 1) / 3;
  int numReqPoints = numCurves * 3 + 1;

  if (numPoints >= numReqPoints) {
    DPOINT startPoint, endPoint, controlPoint1, controlPoint2;
    double distance1, distance2, distance3;
    double ax, bx, cx, ay, by, cy, t, step;
    double xt, yt, k, k1;
    int oldX, oldY, oldX1, oldY1;

    for (i = 0; i < numCurves; i++) {
      startPoint = points[i * 3];
      controlPoint1 = points[i * 3 + 1];
      controlPoint2 = points[i * 3 + 2];
      endPoint = points[i * 3 + 3];

      if (controlPoint1.y == controlPoint2.y) {
        TDrawLine(hDC, startPoint.x, startPoint.y, endPoint.x, endPoint.y, clr);
      }
      else {
        distance1 = (double)sqrt(pow(controlPoint1.x - startPoint.x, 2) + pow(controlPoint1.y - startPoint.y, 2));
        distance2 = (double)sqrt(pow(controlPoint2.x - controlPoint1.x, 2) + pow(controlPoint2.y - controlPoint1.y, 2));
        distance3 = (double)sqrt(pow(endPoint.x - controlPoint2.x, 2) + pow(endPoint.y - controlPoint2.y, 2));
        step = 1.0f / (distance1 + distance2 + distance3);
        cx = 3.0f * (controlPoint1.x - startPoint.x);
        bx = 3.0f * (controlPoint2.x - controlPoint1.x) - cx;
        ax = endPoint.x - startPoint.x - bx - cx;
        cy = 3.0f * (controlPoint1.y - startPoint.y);
        by = 3.0f * (controlPoint2.y - controlPoint1.y) - cy;
        ay = endPoint.y - startPoint.y - by - cy;
        oldX = (int)startPoint.x;
        oldY = (int)startPoint.y;
        k1 = 0.0f;

        for (t = 0.0f; t <= 1.0f; t += step) {
          xt = ax * (t * t * t) + bx * (t * t) + cx * t + startPoint.x;
          yt = ay * (t * t * t) + by * (t * t) + cy * t + startPoint.y;

          if (((int)xt != oldX) && ((int)yt != oldY)) {
            oldX1 = oldX;
            oldY1 = oldY;
            k = (double)((int)yt - oldY) / (double)((int)xt - oldX);

            if (k != k1) {
              TDrawLine(hDC, oldX, oldY, (int)xt, (int)yt, clr);
              k1 = k;
              oldX = (int)xt;
              oldY = (int)yt;
            }
          }
        }

        if (((int)xt != oldX1) || ((int)yt != oldY1)) {
          int dx = ((int)xt - oldX1);
          int dy = ((int)yt - oldY1);

          if (abs(dx) > abs(dy)) {
            if (dy > 0) {
              TDrawLine(hDC, oldX, oldY, (int)xt, (int)yt + 1, clr);
            }
            else {
              TDrawLine(hDC, oldX, oldY, (int)xt, (int)yt - 1, clr);
            }
          }
          else {
            if (dx > 0) {
              TDrawLine(hDC, oldX, oldY, (int)xt + 1, (int)yt, clr);
            }
            else {
              TDrawLine(hDC, oldX, oldY, (int)xt - 1, (int)yt, clr);
            }
          }
        }
      }
    }
  }

  return 0;
}
double B(int x, double t)
{
  switch (x) {
  case 0:
    return (- t * t * t + 3 * t * t - 3 * t + 1) / 6.0;
    // return (1-t*t*t)/6.0;

  case 1:
    return (3 * t * t * t - 6 * t * t + 4) / 6.0;

  case 2:
    return (- 3 * t * t * t + 3 * t * t + 3 * t + 1) / 6.0;

  case 3:
    return t * t * t / 6.0;
  }

  return 0.0;
}
int B3(HDDC hDC, POINT p[], int len)
{
  int k, m;
  double dt = 0.001;
  double t = 0;
  double x, y;

  for (m = 0; m <= len - 3; m++) {
    t = 0;

    while (t <= 1) {
      x = 0;
      y = 0;

      for (k = 0; k <= 3; k++) {
        x = x + p[k + m].x * B(k, t);
        y = y + p[k + m].y * B(k, t);
      }

      t = t + dt;
      _SetPixel(hDC, (int)x, (int)y, _RGB(255, 0, 0));
    }
  } //for

  return 0;
}
void draw_bresline(HDDC hDC, int x1, int y1, int xx, int yy, COLORREF clr)
{
  POINT start, end;
  int x, y, dx, dy;
  int dx2, dy2, s1, s2, temp, changed = 0, i, f;
  start.x = x1;
  start.y = y1;
  end.x = xx;
  end.y = yy;
  x = start.x, y = start.y;
  dx = abs(end.x - start.x), dy = abs(end.y - start.y);
  s1 = end.x - start.x > 0 ? 1 : - 1;
  s2 = end.y - start.y > 0 ? 1 : - 1;

  if (dy > dx) {
    temp = dx;
    dx = dy;
    dy = temp;
    changed = 1;
  }

  dx2 = 2 * dx;
  dy2 = 2 * dy;
  f = dy2 - dx;

  for (i = 0; i <= dx; i++) {
    _SetPixel(hDC, x, y, clr);

    if (f >= 0) {
      if (changed) {
        x += s1;
      }
      else {
        y += s2;
      }

      f -= dx2;
    }

    if (changed) {
      y += s2;
    }
    else {
      x += s1;
    }

    f += dy2;
  }
}
int draw_bezier2(HDDC hDC, POINT* p, int len, COLORREF clr)
{
  POINT pp[100];
  POINT pa[100];
  int r, k, i;
  double t = 0.5;

  if (1) {
    DPOINT pd[10];
    int i;

    for (i = 0; i < len; ++i) {
      pd[i].x = p[i].x;
      pd[i].y = p[i].y;
    }

    draw_bezier(hDC, pd, len, clr);
  }

  for (i = 0; i < len; i++) {
    pp[i].x = p[i].x;
    pp[i].y = p[i].y;
  }

  if (abs(pp[0].x - pp[1].x) < 2 && abs(pp[0].y - pp[1].y) < 2) {
    // pDC->MoveTo(pp[i].x,pp[i].y);
    // pDC->LineTo(pp[i+1].x,pp[i+1].y);
    for (i = 0; i < len - 1; i++) {
      draw_bresline(hDC, pp[i].x, pp[i].y, pp[i + 1].x, pp[i + 1].y, clr);
    }

    return 0;
  }

  k = 1;
  pa[0] = p[0];

  for (r = 1; r <= len; r++) {
    for (i = 0; i < len - r; i++) {
      pp[i].x = (long)((1 - t) * pp[i].x + t * pp[i + 1].x);
      pp[i].y = (long)((1 - t) * pp[i].y + t * pp[i + 1].y);
    } //for

    pa[k] = pp[0];
    k++;
  } //for

  draw_bezier2(hDC, pp, len, clr);
  draw_bezier2(hDC, pa, len, clr);
  return 0;
}
#if 1
#define M_SetPixel _SetPixel
#else
#define zzz(x) 10*((int)(x))
#define M_SetPixel(hDC, x, y, clr) draw_fillrect(hDC, zzz(x), zzz(y), zzz(x+1), zzz(y+1), clr); \
  draw_3drect_in(hDC, zzz(x), zzz(y), zzz(x+1), zzz(y+1), rgb(0,0,0), rgb(0,0,0))
#endif
#define EllipsePlot(x, y) M_SetPixel(hDC, x, y, clr), M_SetPixel(hDC, x, (ay-y), clr), \
  M_SetPixel(hDC, (ax-x), y, clr), M_SetPixel(hDC, (ax-x), (ay-y), clr); \
  if (isfill) { int i; for (i = MIN(x, (ax-x)); i <= MAX(x, (ax-x)); ++i) { M_SetPixel(hDC, i, y, clr); M_SetPixel(hDC, i, (ay-y), clr); } }
#define draw_ellipseR(hDC, prc, clr, isfill) draw_ellipse(hDC, (prc)->left, (prc)->top, (prc)->right, (prc)->bottom, clr, isfill)
int draw_ellipse(HDDC hDC, int x1, int y1, int x2, int y2, COLORREF clr, BOOL isfill)
{
  int x, y;
  int a, b, cx, cy, dx, dy, ax = x1 + x2 - 1, ay = y1 + y2 - 1;
  int a2, b2;
  a = (x2 - x1), b = (y2 - y1);

  if (0 == a || 0 == b) {
    return 0;
  }

  cx = (x2 + x1), cy = (y2 + y1);
  a2 = a * a, b2 = b * b;
#if 1
  x = (x2 + x1 - 1) / 2, y = y1;

  // 在圆外则y+1
  for (; b2 * (dx = (cx - (2 * x + 1))) < a2 * (dy = (cy - (2 * y + 1))); --x) {
    y += b2 * dx * dx + a2 * (dy * dy - b2) > 0;
    EllipsePlot(x, y);
  }

  x = x1, y = (y2 + y1 - 1) / 2;

  for (; b2 * (dx = (cx - (2 * x + 1))) > a2 * (dy = (cy - (2 * y + 1))); --y) {
    x += b2 * (dx * dx - a2) + a2 * dy * dy > 0;
    EllipsePlot(x, y);
  }

#else
  x = (x2 + x1 - 1) / 2, y = y1;

  // 在圆外则y+1
  for (; y <= (y2 + y1 - 1) / 2;) {
    int t = b2 * (dx = (cx - (2 * x + 1))) < a2 * (dy = (cy - (2 * y + 1)));

    if (t) {
      y += b2 * dx * dx + a2 * (dy * dy - b2) > 0;
      EllipsePlot(x, (y - 1));
      --x;
    }
    else {
      x -= b2 * (dx * dx - a2) + a2 * dy * dy < 0;
      EllipsePlot((x + 1), y);
      ++y;
    }
  }

#endif
  return 0;
}


void DrawEllipseR(HDDC hDC, RECT rc, COLORREF clrFill, COLORREF clrLine)
{
  if (clrFill) {
    draw_ellipseR(hDC, &rc, clrFill, 1);
  }

  if (clrLine) {
    draw_ellipseR(hDC, &rc, clrLine, 0);
  }
}

int TDrawEllipse(HDDC hDC, RECT rect, COLORREF clrFill, COLORREF clrLine)
{
  RECT rt;
  SIZE size;
  BOOL bFill;
  int brdR;
  int brdG;
  int brdB;

  int filR;
  int filG;
  int filB;


  int R, G, B, A;
  COLORREF clrN, clrI;

  BOOL exch;
  double a, b;
  double cx, cy;
  double y, t;
  int ix, iy, i, i1, i2, al = (hDC->bw >> 2);
  DWORD* pBits;

  NormalizeRect(&rect);
  ClipRect(hDC, &rect);

  if (clrFill == 0 && clrLine == 0) {
    ASSERT(FALSE);
    return 0;
  }

  rt = (rect);
  NormalizeRect(&rt);

  size = RCSZ(&rt);

  if (size.w == 0 || size.h == 0) {
    return 0;
  }

  if (clrLine == 0) {
    clrLine = clrFill;
  }

  bFill = clrFill != 0;

  brdR = GetRV(clrLine);
  brdG = GetGV(clrLine);
  brdB = GetBV(clrLine);

  filR = GetRV(clrFill);
  filG = GetGV(clrFill);
  filB = GetBV(clrFill);


  exch = FALSE;

  {
    double x1 = 0;
    double x2 = size.w - 1;
    double y1 = 0;
    double y2 = size.h - 1;

    if (x2 < x1) {
      t  = x1;
      x1 = x2;
      x2 = t;
    }

    if (y2 < y1) {
      double t  = y1;
      y1 = y2;
      y2 = t;
    }

    if (y2 - y1 <= x2 - x1) {
      t = x1;
      exch = TRUE;
      x1 = y1;
      y1 = t;

      t = x2;
      x2 = y2;
      y2 = t;
    }

    a  = (x2 - x1) / 2.0;
    b  = (y2 - y1) / 2.0;
    cx = (x1 + x2) / 2.0;
    cy = (y1 + y2) / 2.0;
  }

  pBits = &_Pixel(hDC, rect.left, rect.top);

  if (bFill) {
    i1 = (int)ceil(cx - a);
    i2 = (int)floor(cx + a);

    for (ix = i1; ix <= i2; ix++) {
      double dist = 1.0 - sqr((ix - cx) / a);

      if (dist < 0) {
        continue;
      }

      y = b * sqrt(dist);

      if (!exch) {
        int y1 = (int)ceil(cy - y);
        int y2 = (int)floor(cy + y);
        COLORREF* pRow = pBits + y1 * al + ix;

        for (i = y1; i <= y2; i++) {
          *pRow = clrFill;
          pRow += al;
        }
      }
      else {
        int x1 = (int)ceil(cy - y);
        int x2 = (int)floor(cy + y);
        COLORREF* pRow = pBits + ix * al + x1;

        for (i = x1; i <= x2; i++) {
          *pRow = clrFill;
          pRow++;
        }
      }
    }
  }


  t  = a * a / sqrt(a * a + b * b);
  i1 = (int)floor(cx - t);
  i2 = (int)ceil(cx + t);

  for (ix = i1; ix <= i2; ix++) {
    double y, f, dist = 1.0 - sqr((ix - cx) / a);

    if (dist < 0) {
      continue;
    }

    y  = b * sqrt(dist);
    iy = (int)ceil(cy + y);
    f  = iy - cy - y;

    B = (int)(brdB * f);
    G = (int)(brdG * f);
    R = (int)(brdR * f);
    A = (int)(255  * f);

    if (bFill) {
      double fi = 1.0 - f;
      clrN = _RGB(filR * fi + R, filG * fi + G, filB * fi + B);
    }
    else {
      clrN = _RGBA(R, G, B, A);
    }

    clrI = _RGBA((brdR - R), (brdG - G), (brdB - B), (255 - A));

    if (!exch) {
      RGBBLENSET(*(pBits + iy * al + ix), clrI);
      RGBBLENSET(*(pBits + (iy - 1) * al + ix), clrN);
    }
    else {
      RGBBLENSET(*(pBits + ix * al + iy), clrI);
      RGBBLENSET(*(pBits + ix * al + iy - 1), clrN);
    }

    iy = (int)floor(cy - y);
    f  = cy - y - iy;

    B = (int)(brdB * f);
    G = (int)(brdG * f);
    R = (int)(brdR * f);
    A = (int)(255  * f);

    if (bFill) {
      double fi = 1.0 - f;
      clrN = _RGB(filR * fi + R, filG * fi + G, filB * fi + B);
    }
    else {
      clrN = _RGBA(R, G, B, A);
    }

    clrI = _RGBA((brdR - R), (brdG - G), (brdB - B), (255 - A));

    if (!exch) {
      RGBBLENSET(*(pBits + iy * al + ix), clrI);
      RGBBLENSET(*(pBits + (iy + 1) * al + ix), clrN);
    }
    else {
      RGBBLENSET(*(pBits + ix * al + iy), clrI);
      RGBBLENSET(*(pBits + ix * al + iy + 1), clrN);
    }
  }

  t  = b * b / sqrt(a * a + b * b);
  i1 = (int)ceil(cy - t);
  i2 = (int)floor(cy + t);

  for (iy = i1; iy <= i2; iy++) {
    double x, f, dist = 1.0 - sqr((iy - cy) / b);

    if (dist < 0) {
      continue;
    }

    x  = a * sqrt(dist);
    ix = (int)floor(cx - x);
    f  = cx - x - ix;

    B = (int)(brdB * f);
    G = (int)(brdG * f);
    R = (int)(brdR * f);
    A = (int)(255  * f);

    if (bFill) {
      double fi = 1.0 - f;
      clrN = _RGB(filR * fi + R, filG * fi + G, filB * fi + B);
    }
    else {
      clrN = _RGBA(R, G, B, A);
    }

    clrI = _RGBA((brdR - R), (brdG - G), (brdB - B), (255 - A));

    if (!exch) {
      RGBBLENSET(*(pBits + iy * al + ix), clrI);
      RGBBLENSET(*(pBits + iy * al + ix + 1), clrN);
    }
    else {
      RGBBLENSET(*(pBits + ix * al + iy), clrI);
      RGBBLENSET(*(pBits + (ix + 1) * al + iy), clrN);
    }

    ix = (int)ceil(cx + x);
    f  = ix - cx - x;

    B = (int)(brdB * f);
    G = (int)(brdG * f);
    R = (int)(brdR * f);
    A = (int)(255  * f);

    if (bFill) {
      double fi = 1.0 - f;
      clrN = _RGB(filR * fi + R, filG * fi + G, filB * fi + B);
    }
    else {
      clrN = _RGBA(R, G, B, A);
    }

    clrI = _RGBA((brdR - R), (brdG - G), (brdB - B), (255 - A));

    if (!exch) {
      RGBBLENSET(*(pBits + iy * al + ix), clrI);
      RGBBLENSET(*(pBits + iy * al + ix - 1), clrN);
    }
    else {
      RGBBLENSET(*(pBits + ix * al + iy), clrI);
      RGBBLENSET(*(pBits + (ix - 1) * al + iy), clrN);
    }
  }

  return 0;
}

//#define RCARG(rc) (rc)->left,(rc)->top,(rc)->right,(rc)->bottom
#define drawaa_ellipseR(hDC, rc, clr, wline) drawaa_ellipse(hDC, RCX(rc)*0.5,RCY(rc)*0.5,RCW(rc)*0.5,RCH(rc)*0.5, clr, wline)
#define drawaa_fillellipseR(hDC, rc, clr) drawaa_ellipseR(hDC, rc, clr, -1)
