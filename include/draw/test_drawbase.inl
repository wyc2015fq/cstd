int test_drawArrows(HDDC hDC)
{
  DPOINT ptd[][7] = {
    {
      1330.599999999999909, 1282.399999999999864, 1377.400000000000091, 1282.399999999999864, 1361.799999999999955, 1298.000000000000000, 1393.000000000000000, 1313.599999999999909, 1361.799999999999955, 1344.799999999999955, 1346.200000000000045, 1313.599999999999909, 1330.599999999999909, 1329.200000000000045,
    }
    , {
      1330.599999999999909, 1266.799999999999955, 1377.400000000000091, 1266.799999999999955, 1361.799999999999955, 1251.200000000000045, 1393.000000000000000, 1235.599999999999909, 1361.799999999999955, 1204.399999999999864, 1346.200000000000045, 1235.599999999999909, 1330.599999999999909, 1220.000000000000000,
    }
    , {
      1315.000000000000000, 1282.399999999999864, 1315.000000000000000, 1329.200000000000045, 1299.400000000000091, 1313.599999999999909, 1283.799999999999955, 1344.799999999999955, 1252.599999999999909, 1313.599999999999909, 1283.799999999999955, 1298.000000000000000, 1268.200000000000045, 1282.399999999999864,
    }
    , {
      1268.200000000000045, 1266.799999999999955, 1315.000000000000000, 1266.799999999999955, 1315.000000000000000, 1220.000000000000000, 1299.400000000000091, 1235.599999999999909, 1283.799999999999955, 1204.399999999999864, 1252.599999999999909, 1235.599999999999909, 1283.799999999999955, 1251.200000000000045,
    }
  };
  int i, n = countof(ptd);
  point2type centerPt = {
    (hDC->w) / 2, (hDC->h) / 2
  };

  for (i = 0; i < n; ++i) {
    DPOINT cPloy = PtCenter(n, ptd[i]);
    PtRotate(7, ptd[i], ptd[i], 0, cPloy.x, cPloy.y, 1., 1., centerPt.x - cPloy.x + i * 100 - 200, centerPt.y - cPloy.y);
    DrawPoly(hDC, ptd[i], 7, _RGB(0, 255, 0), 1);
  }

  return 0;
}
int test_drawbase(HDDC hDC, int ch)
{
  static point2type bezierPts[4] = {
    - 100, 0, - 70, 0, 70, 0, 100, 0
  };
  static point2type polylineOld[5] = {
    - 100, - 100, - 50, 100, 0, - 100, 50, 100, 100, - 100
  };
  static point2type polyline[5] = {
    - 100, - 100, - 50, 100, 0, - 100, 50, 100, 100, - 100
  };
  static point2type m_Line[2] = {
    - 100, - 100, 100, 100
  };
  static float m_StartAngle = 0.0f;
  static float m_Angle = 0.0f;
  static RECT _Rect = {
    - 100, - 100, 100, 100
  };
  static BOOL m_bFlag = FALSE;
  static int m_Size = 0;
  static int do_rotate = 1;
  static int do_draw = 1;
  point2type pt[100];
  DPOINT ptd[100];
  //#define RANDCOL() _RGB(rand()%255, rand()%255, rand()%255)
#define RANDCOL() _RGB(255, 0, 0)
  int i;
  DWORD color = (RANDCOL());
  //float angle = (((int)clock()/10) / 180.0f) * 3.1415f;
  float angle = (m_Angle / 180.0f) * 3.1415f;
  point2type centerPt = {
    (hDC->w) / 2, (hDC->h) / 2
  };

  if (' ' == ch) {
    do_rotate = !do_rotate;
  }

  if ('d' == ch) {
    do_draw = !do_draw;
  }

  if (!do_draw) {
    return 0;
  }

  test_drawArrows(hDC);

  if (1) {
    PtRotate(1, pt, m_Line, angle, 0, 0, 1, 1, centerPt.x, centerPt.y);
    G_text(hDC, (int)pt[0].x, (int)pt[0].y, "abcdAAdfe", RANDCOL());
  }

  if (0) {
    DPOINT ptPloy[] = {
      100, 60, 369, 170, 143, 310
    };
    int n = countof(ptPloy);
    DPOINT cPloy = PtCenter(n, ptPloy);
    PtRotate(n, ptd, ptPloy, angle, cPloy.x, cPloy.y, 1, 1, centerPt.x - cPloy.x, centerPt.y - cPloy.y);
    ScanFill(hDC, ptd, n, _RGB(80, 30, 20));
  }

  if (1) {
    point2type pts1[5];
    memcpy(pts1, polyline, 5 * sizeof(point2type));

    for (i = 0; i < 5; i++) {
      pts1[i].x += centerPt.x;
      pts1[i].y += centerPt.y;
    }

    DrawPoly(hDC, pts1, 5, RANDCOL(), 1);
    //TextOut(_T("The AA polyline"));
  }

  if (1) {
    static int r = 1;
    r = 70;
    cg_antialiased_circle(hDC, centerPt.x, centerPt.y, 70, RANDCOL());
    Drawcircle(hDC, centerPt.x, centerPt.y, 60, RANDCOL());
    Midpointcircle(hDC, centerPt.x, centerPt.y, 50, RANDCOL(), 0);
    Bresenhamcircle(hDC, centerPt.x, centerPt.y, 30, RANDCOL(), 0);
    r = (r + 1) % 100;
  }

  if (1) {
    TDrawEllipse(hDC, centerPt.x, centerPt.y, 100, 50, RANDCOL());
    //Drawellipse(hDC, centerPt.x, centerPt.y, 95, 45, RANDCOL());
    //MidpointEllispe(hDC, centerPt.x, centerPt.y, 90, 40, RANDCOL(), 0);
    FillEllipse(hDC, centerPt.x, centerPt.y, 80, 35, RANDCOL(), 0);
    FillEllipse(hDC, centerPt.x, centerPt.y, 35, 80, RANDCOL(), 0);
    //Fillellipse(hDC, rect.left, rect.top, rect.right, rect.bottom, RANDCOL());
    //TextOut(_T("The AA ellipse"));
  }

  if (1) {
    TDrawArc(hDC, centerPt.x, centerPt.y, 50, 100, m_StartAngle, m_StartAngle + m_Angle, RANDCOL());
    //TextOut(_T("The AA arc"));
  }

  if (1) {
    TDrawPie(hDC, centerPt.x, centerPt.y, 50, 100, m_StartAngle, m_StartAngle + m_Angle, RANDCOL());
    //TextOut(_T("The AA pie"));
  }

  if (1) {
    TDrawChord(hDC, centerPt.x, centerPt.y, 50, 100, m_StartAngle, m_StartAngle + m_Angle, RANDCOL());
    //TextOut(_T("The AA chord"));
  }

  if (1) {
    TDrawRoundRect(hDC, - 50 + centerPt.x, - 100 + centerPt.y, 50 + centerPt.x, 100 + centerPt.y, m_Size, m_Size, RANDCOL());
  }

  if (1) {
    point2type pts1[4];
    memcpy(pts1, bezierPts, 4 * sizeof(point2type));

    for (i = 0; i < 4; i++) {
      pts1[i].x += centerPt.x;
      pts1[i].y += centerPt.y;
    }

    TDrawBezier(hDC, pts1, 4, RANDCOL());
    //TextOut(_T("The AA Bezier"));
  }

  if (do_rotate) {
    m_Angle += 2.0f;

    if (m_Angle >= 360.0f) {
      m_StartAngle += 10.0f;

      if (m_StartAngle >= 360.0f) {
        m_StartAngle = 0.0f;
      }

      m_Angle = 0.0f;
    }

    if (!m_bFlag) {
      if (_Rect.top >= _Rect.bottom) {
        _Rect.top = - 100;
        _Rect.bottom = 100;
        m_bFlag = TRUE;
      }

      bezierPts[1].y++;
      bezierPts[2].y--;
      m_Size++;

      if (m_Size > 100) {
        m_Size = 100;
      }
    }
    else {
      InflateRect(&_Rect, - 1, 0);

      if (_Rect.left >= _Rect.right) {
        _Rect.left = - 100;
        _Rect.right = 100;
        m_bFlag = FALSE;
      }

      bezierPts[1].y--;
      bezierPts[2].y++;
      m_Size--;

      if (m_Size < 0) {
        m_Size = 0;
      }
    }

    {
      float angle = (m_Angle / 180.0f) * 3.1415f;
      int i, x, y;

      for (i = 0; i < 5; i++) {
        x = (int)(polylineOld[i].x * cos(angle) - polylineOld[i].y * sin(angle));
        y = (int)(polylineOld[i].x * sin(angle) + polylineOld[i].y * cos(angle));
        polyline[i].x = x;
        polyline[i].y = y;
      }
    }
  }

  return 0;
}

