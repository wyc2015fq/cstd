typedef struct C3DPoint {
  float x;
  float y;
  float z;
} C3DPoint;
float iSteep;
int iSpeed;
float HighColor;
float LowColor;
float z_Max, z_Min;
float y_Min, y_Max;
float x_Min, x_Max;
int stcWidth, stcHeight;
COLORREF crColor;
int SetSpeed(int Speed)
{
  if (Speed <= 500 && Speed >= 1) {
    iSpeed = 1001 - Speed;
    return 1;
  }
  else {
    iSpeed = 1000;
    return 0;
  }
}
void SetHighLowColor(float Maxx, float Minn)
{
  HighColor = Maxx;
  LowColor = Minn;
}
float MathFunc(float x, float y)
{
  return 2 * (float)sin(x * y / 3);
}
/////////////////////////////////////////////////////////////////////////////
// CAnalogClock
void C3DStatic()
{
  z_Max = 10;
  y_Max = 10;
  x_Max = 10;
  z_Min = - 10;
  y_Min = - 10;
  x_Min = - 10;
  iSteep = 60;
  iSpeed = 500;
  HighColor = 1;
  LowColor = 0;
}
POINT GetNewPixel(float Oldx, float Oldy)
{
  POINT NewPoint;
  NewPoint.x = (int)(stcWidth / (x_Max - x_Min) * Oldx - (stcWidth * x_Min) / (x_Max - x_Min));
  NewPoint.y = (int)(stcHeight / (y_Max - y_Min) * Oldy - (stcHeight * y_Max) / (y_Max - y_Min));
  return NewPoint;
}
COLORREF Set3DColor(float z)
{
  int r, g, b;
  r = (int)(- (255 / (LowColor - HighColor)) * z + (255 * LowColor) / (LowColor - HighColor));
  g = (int)(- z * z * (255 / (HighColor * HighColor)) + 255);
  b = (int)((255 / (LowColor - HighColor)) * z + (- 255 * HighColor) / (LowColor - HighColor));
  crColor = _RGB(r, g, b);
  return crColor;
}
int Set3DPixel(HDDC hDC, C3DPoint pPixel, COLORREF crColor)
{
  // x=l
  // y=A
  // z=B
  // newX=A-+lcos(isteep)
  // newY=newX*tan(isteep)+B-A*tan(isteep)
  // so...
  POINT newP;
  float newX, newY;
  COLORREF pen = (crColor);
  newX = (float)(pPixel.y - pPixel.x * cos(iSteep));
  newY = (float)(newX * tan(iSteep) + (pPixel.z - pPixel.y * tan(iSteep)));

  if ((newX <= pPixel.y && pPixel.x >= 0) || (newX >= pPixel.y && pPixel.x < 0)) {
    newP = GetNewPixel(newX, newY);

    if ((newP.x >= 0 && newP.y <= 0) && (newP.x <= stcWidth && newP.y >= - stcHeight)) {
      _SetPixel(hDC, newP.x, abs(newP.y), crColor);
    }
  }
  else {
    newX = (float)(pPixel.y + pPixel.x * cos(iSteep));
    newY = (float)(newX * tan(iSteep) + (pPixel.z - pPixel.y * tan(iSteep)));

    if ((newX <= pPixel.y && pPixel.x >= 0) || (newX >= pPixel.y && pPixel.x < 0)) {
      newP = GetNewPixel(newX, newY);

      if ((newP.x >= 0 && newP.y <= 0) && (newP.x <= stcWidth && newP.y >= - stcHeight)) {
        _SetPixel(hDC, newP.x, newP.y, crColor);
      }
    }
  }

  return 0;
}
POINT Set3DPixel1(C3DPoint pPixel)
{
  // x=l
  // y=A
  // z=B
  // newX=A-+lcos(isteep)
  // newY=newX*tan(isteep)+B-A*tan(isteep)
  // so...
  POINT newP = {
    0
  };
  float newX, newY;
  newX = (float)(pPixel.y - pPixel.x * cos(iSteep));
  newY = (float)(newX * tan(iSteep) + (pPixel.z - pPixel.y * tan(iSteep)));
  crColor = Set3DColor(pPixel.z);

  if ((newX <= pPixel.y && pPixel.x >= 0) || (newX >= pPixel.y && pPixel.x < 0)) {
    newP = GetNewPixel(newX, newY);
    return newP;
  }
  else {
    newX = (float)(pPixel.y + pPixel.x * cos(iSteep));
    newY = (float)(newX * tan(iSteep) + (pPixel.z - pPixel.y * tan(iSteep)));

    if ((newX <= pPixel.y && pPixel.x >= 0) || (newX >= pPixel.y && pPixel.x < 0)) {
      newP = GetNewPixel(newX, newY);
      return newP;
    }
  }

  return newP;
}
int Set3DLine(HDDC hDC, C3DPoint pPixel, C3DPoint sPixel, COLORREF crColor)
{
  POINT PPixel, SPixel;
  COLORREF pen = crColor;
  PPixel = Set3DPixel1(pPixel);
  SPixel = Set3DPixel1(sPixel);

  if ((PPixel.x >= 0 && PPixel.y <= 0) && (PPixel.x <= stcWidth && PPixel.y >= - stcHeight)) {
    int _x0, _y0;
    TMoveTo(hDC, PPixel.x, PPixel.y);
    TLineTo(hDC, SPixel.x, SPixel.y, crColor);
  }

  return 0;
}
void SetXSteep(HDDC hDC, int degree, RECT rect)
{
  C3DPoint stPoint3, ndPoint3, rdPoint3, refPoint3;
  iSteep = (float)(PI * degree) / 180;
  stcWidth = RCWIDTH(&rect);
  stcHeight = RCHEIGHT(&rect);
  stPoint3.x = x_Max;
  stPoint3.y = 0;
  stPoint3.z = 0;
  ndPoint3.x = 0;
  ndPoint3.y = y_Max;
  ndPoint3.z = 0;
  rdPoint3.x = 0;
  rdPoint3.y = 0;
  rdPoint3.z = z_Max;
  refPoint3.x = 0;
  refPoint3.y = 0;
  refPoint3.z = 0;
  Set3DLine(hDC, refPoint3, stPoint3, _RGB(0, 0, 0));
  Set3DLine(hDC, refPoint3, ndPoint3, _RGB(0, 0, 0));
  Set3DLine(hDC, refPoint3, rdPoint3, _RGB(0, 0, 0));
}
void Draw3DFunction(HDDC hDC, RECT rect)
{
  C3DPoint _3DPixel;
  COLORREF crColor = _RGB(0, 0, 255);
  COLORREF pen = (crColor);
  float i, j, MAXx, MINn;
  stcWidth = RCWIDTH(&rect);
  stcHeight = RCHEIGHT(&rect);
  SetXSteep(hDC, 40, rect);
  MAXx = MINn = 0;

  for (j = y_Min; j <= y_Max; j += (x_Max - x_Min) / iSpeed)
    for (i = x_Min; i <= x_Max; i += (x_Max - x_Min) / iSpeed) {
      _3DPixel.z = MathFunc(j, i);

      if (_3DPixel.z >= MAXx) {
        MAXx = _3DPixel.z;
      }

      if (_3DPixel.z <= MINn) {
        MINn = _3DPixel.z;
      }
    }

  SetHighLowColor(MAXx, MINn);

  for (j = y_Min + .5f; j <= y_Max; j += (y_Max - y_Min) / iSpeed)
    for (i = x_Min + .5f; i <= x_Max; i += (x_Max - x_Min) / iSpeed) {
      _3DPixel.y = i;
      _3DPixel.x = j;
      _3DPixel.z = MathFunc(j, i);
      crColor = Set3DColor(_3DPixel.z);
      Set3DPixel(hDC, _3DPixel, crColor);
    }

  SetXSteep(hDC, 40, rect);
}
void test_3DStatic(HDDC hDC)
{
  int x = 0, y = 0;
  RECT rc = iRECT(x, y, hDC->w, y + hDC->h);
  RECT rc2 = rc;
  RCOFFSET(&rc2, 10, 10, - 10, - 10);
  C3DStatic();
  SetSpeed(1);
  Draw3DFunction(hDC, rc2);
}

