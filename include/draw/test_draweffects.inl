
const int nHeaderSize = 30;
const int nBlockSize = 150;
const int nMarginSize = 10;

// CDrawEffectsView drawing

// CDrawEffectsView message handlers
int Shadow(HDDC hDC, RECT rect)
{
  int nTextHeight;
  RECT rect2;
  TFillSolidRect(hDC, &rect, _RGB(255, 255, 255));
  Draw3dRect(hDC, &rect, _GetSysColor(COLOR_3DSHADOW), _GetSysColor(COLOR_3DLIGHT));
  DeflateRect(&rect, 10, 10);

  // Draw caption:
  nTextHeight = 20;
  draw_textR(hDC, _T("Shadow Effect:"), -1, &rect, 0, _RGB(0, 0, 0));
  rect.top += nTextHeight + 10;

  // Fill area:
  draw_fill_hatch_rectR(hDC, &rect, _RGB(120, 204, 202), _RGB(255, 255, 255), HS_FDIAGONAL);

  DeflateRect(&rect, 15, 15);
  rect2 = rect;

  rect.bottom -=  RCH(&rect) / 3;

  // Draw solid rectangle
  TFillSolidRect(hDC, &rect, _RGB(238, 238, 238));

  // Drop shadow:
  DrawShadow(hDC, rect, 10, 100, 50, 0, TRUE);

  rect2.left += 30;
  rect2.right -= 10;
  rect2.top += 15;

  // Draw solid rectangle
  TFillSolidRect(hDC, &rect2, _RGB(141, 207, 244));

  // Drop shadow:
  DrawShadow(hDC, rect2, 5, 100, 50, 0, TRUE);
  return 0;
}

int Gradient(HDDC hDC, RECT rect)
{
  int nTextHeight = 20;
  RECT rect1, rect2, rect3, rect4;
  DWORD clrText = _RGB(0, 0, 0);
  TFillSolidRect(hDC, &rect, _RGB(255, 255, 255));
  Draw3dRect(hDC, &rect, _GetSysColor(COLOR_3DSHADOW), _GetSysColor(COLOR_3DLIGHT));
  DeflateRect(&rect, 10, 10);

  // Draw caption:
  draw_textR(hDC, _T("Gradient Effect:"), -1, &rect, 0, clrText);
  rect.top += nTextHeight + 10;

  rect1 = rect;
  rect1.bottom = rect1.top + 20;

  DrawRect(hDC, rect1, 0, clrText);
  DeflateRect(&rect1, 1, 1);
  FillGradient(hDC, rect1, _RGB(0, 159, 98), _RGB(255, 255, 255), FALSE, 0);

  rect2 = rect;
  rect2.top = rect1.bottom + 10;
  rect2.right = rect2.left + 30;

  DrawRect(hDC, rect2, 0, clrText);
  DeflateRect(&rect2, 1, 1);
  FillGradient(hDC, rect2, _RGB(245, 183, 87), _RGB(255, 255, 255), TRUE, 0);

  rect3 = rect;
  rect3.left = rect2.right + 10;
  rect3.top = rect2.top;
  rect3.bottom = rect3.top + 20;

  DrawRect(hDC, rect3, 0, clrText);
  DeflateRect(&rect3, 1, 1);

  rect3.right = rect3.left + RCW(&rect3) / 3;
  FillGradient(hDC, rect3, _RGB(153, 153, 153), _RGB(255, 255, 255), FALSE, 0);

  rect3.left = rect3.right;
  rect3.right = rect.right - 1;
  FillGradient(hDC, rect3, _RGB(255, 255, 255), _RGB(153, 153, 153), FALSE, 0);

  rect4 = rect;
  rect4.left = rect2.right + 30;
  rect4.top = rect3.bottom + 10;

  DrawRect(hDC, rect4, 0, clrText);
  DeflateRect(&rect4, 1, 1);

  FillGradientPt2(hDC, rect4, _RGB(0, 255, 255), _RGB(0, 0, 255), RCLT(&rect4), RCRB(&rect4));
  //FillGradient2(hDC, rect4, _RGB(0, 255, 255), _RGB(0, 0, 255), 45);
  return 0;
}

int Highlight(HDDC hDC, RECT rect)
{
  int nTextHeight = 20, x, y;
  RECT rectText;
  img_t m_Bitmap[1] = {0};
  DWORD clrText = _RGB(0, 0, 0);
  GetImage(m_Bitmap, "D:\\code\\c\\GUI\\bcgsoft\\BCG\\Samples\\DrawEffects\\res\\bitmap1.bmp", 0);
  TFillSolidRect(hDC, &rect, _RGB(255, 255, 255));
  Draw3dRect(hDC, &rect, _GetSysColor(COLOR_3DSHADOW), _GetSysColor(COLOR_3DLIGHT));
  DeflateRect(&rect, 10, 10);

  // Draw caption:
  draw_textR(hDC, _T("Highlight Effect:"), -1, &rect, 0, clrText);
  rect.top += nTextHeight + 10;

  // Draw original image:
  x = rect.left + 80;
  y = rect.top;

  rectText = rect;
  rectText.left += 10;
  rectText.right = x;
  rectText.top += 10;
  rectText.bottom = rectText.top +  RCH(&rect) / 3;

  draw_textR(hDC, _T("Original:"), -1, &rectText, 0, clrText);
  draw_image(hDC, x, y, x + m_Bitmap->width, y + m_Bitmap->height, m_Bitmap, 0, 0, 0, 0);

  // Draw 20% lighter:
  y +=  RCH(&rect) / 3;
  OffsetRect(&rectText, 0, RCH(&rect) / 3);

  draw_textR(hDC, _T("Light:"), -1, &rectText, 0, clrText);
  draw_image(hDC, x, y, x + m_Bitmap->width, y + m_Bitmap->height, m_Bitmap, 0, 0, 0, 0);

  HighlightRect(hDC, iRECT3(cPOINT(x, y), iSIZE(32, 32)), 150, 0, 0, 0);

  // Draw 50% darker:
  y +=  RCH(&rect) / 3;
  OffsetRect(&rectText, 0, RCH(&rect) / 3);

  draw_textR(hDC, _T("Dark:"), -1, &rectText, 0, clrText);
  draw_image(hDC, x, y, x + m_Bitmap->width, y + m_Bitmap->height, m_Bitmap, 0, 0, 0, 0);

  HighlightRect(hDC, iRECT3(cPOINT(x, y), iSIZE(32, 32)), 80, 150, 0, 0);

  imfree(m_Bitmap);
  return 0;
}

int GradientRings(HDDC hDC, RECT rect)
{
  RECT rect1, rect2, rect3;
  int nTextHeight = 20;
  DWORD clrText = _RGB(0, 0, 0);
  TFillSolidRect(hDC, &rect, _RGB(255, 255, 255));
  Draw3dRect(hDC, &rect, _GetSysColor(COLOR_3DSHADOW), _GetSysColor(COLOR_3DLIGHT));
  DeflateRect(&rect, 10, 10);

  // Draw caption:
  draw_textR(hDC, _T("Gradient Ring Effect:"), -1, &rect, 0, clrText);
  rect.top += nTextHeight + 10;

  // Draw "ring button"(combined 3 gradient rings):
  rect1 = rect;
  rect1.right = rect.left + 50;
  rect1.bottom = rect1.top + 50;

  DrawGradientRing(hDC, rect1, _RGB(176, 186, 201), _RGB(255, 255, 255), _RGB(255, 255, 255), 145, 10, _RGB(250, 250, 250));

  DeflateRect(&rect1, 10, 10);
  DrawGradientRing(hDC, rect1, _RGB(255, 255, 255), _RGB(176, 186, 201), _RGB(255, 255, 255), 145, 7, _RGB(250, 250, 250));
  DeflateRect(&rect1, 6, 6);

  DrawGradientRing(hDC, rect1, _RGB(0, 153, 0), _RGB(255, 255, 255), _RGB(255, 255, 255), 145, 10, _RGB(250, 250, 250));

  // Draw push button:
  rect2 = rect;
  rect2.top = rect2.bottom - 26;
  rect2.right = rect2.left + 60;

  DrawGradientRing(hDC, rect2, _RGB(176, 186, 201), _RGB(255, 255, 255), _RGB(176, 186, 201), 45, 4, _RGB(230, 240, 255));

  draw_textR(hDC, _T("Button"), -1, &rect2, DT_CENTER | DT_VCENTER | DT_SINGLELINE, clrText);

  // Anothet example:
  rect3 = rect;
  rect3.left += 80;
  rect3.top = CenterPoint(&rect3).y -  RCW(&rect3) / 2;
  rect3.bottom = rect3.top +  RCW(&rect3);

  DrawGradientRing(hDC, rect3, _RGB(192, 192, 192), _RGB(255, 255, 255), (COLORREF) - 1, 225, 4, _RGB(230, 230, 230));
  return 0;
}

int test_draweffects(HDDC hDC, RECT rectClient)
{
  const char* strInfo = "This sample illustrates usage of CBCGPDrawManager struct";
  COLORREF clrText;
  RECT rectFrame;
  RECT rect;

  TFillSolidRect(hDC, &rectClient, _RGB(255, 255, 234));
  clrText = (_GetSysColor(COLOR_INFOTEXT));
  rectFrame = iRECT3(cPOINT(nMarginSize, nMarginSize), iSIZE(nBlockSize * 2, nHeaderSize));
  DeflateRect(&rectFrame, 5, 5);

  draw_textR(hDC, strInfo, -1, &rectFrame, DT_WORDBREAK, clrText);

  clrText = (_RGB(0, 0, 0));

  rect = rectClient;
  rect.top += nHeaderSize;
  DeflateRect(&rect, nMarginSize, nMarginSize);


  // Shadow effect:

  Shadow(hDC, iRECT3(cPOINT(rect.left, rect.top), iSIZE(nBlockSize, nBlockSize)));

  // Gradient effect:

  Gradient(hDC, iRECT3(cPOINT(rect.left + nBlockSize + 2 * nMarginSize, rect.top), iSIZE(nBlockSize, nBlockSize)));

  //--
  // Highlight effect:
  //--
  Highlight(hDC, iRECT3(cPOINT(rect.left, rect.top + nBlockSize + 2 * nMarginSize), iSIZE(nBlockSize, nBlockSize)));

  // Gradient rings effect:
  GradientRings(hDC, iRECT3(cPOINT(rect.left + nBlockSize + 2 * nMarginSize, rect.top + nBlockSize + 2 * nMarginSize), iSIZE(nBlockSize, nBlockSize)));

  return 0;
}
