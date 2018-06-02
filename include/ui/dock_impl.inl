static unsigned char DragRecType = 0;
static const WORD DotPatternBmp1[] = {
  0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055, 0x00aa, 0x0055
  //0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555, 0xaaaa, 0x5555
};
static const WORD DotPatternBmp2[] = {
  0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
};
static void drawDragFrame(RECT rc)
{
  HDC hdc;
  HBITMAP hbm;
  HBRUSH hbrush;
  HANDLE hbrushOrig;
  int border;
  RECT DragRecPlacement = rc;
  hdc = GetDC(0);
  {
    const WORD* bitmap;
    // Determine whether to draw a solid drag rectangle or checkered
    if (DragRecType) {
      border = 1;
      bitmap = &DotPatternBmp2[0];
    }
    else {
      border = 3;
      bitmap = &DotPatternBmp1[0];
    }
    // Create a brush with the appropriate bitmap pattern to draw our drag rectangle
    hbm = CreateBitmap(8, 8, 1, 1, bitmap);
    hbrush = CreatePatternBrush(hbm);
  }
  SetBrushOrgEx(hdc, DragRecPlacement.left, DragRecPlacement.top, 0);
  hbrushOrig = SelectObject(hdc, hbrush);
  // Draw the drag rectangle
  {
    int width, height;
    width = DragRecPlacement.right - DragRecPlacement.left;
    height = DragRecPlacement.bottom - DragRecPlacement.top;
    PatBlt(hdc, DragRecPlacement.left + border, DragRecPlacement.top, width - border, border, PATINVERT);
    PatBlt(hdc, DragRecPlacement.left + width - border, DragRecPlacement.top + border, border, height - border, PATINVERT);
    PatBlt(hdc, DragRecPlacement.left, DragRecPlacement.top + height - border, width - border, border, PATINVERT);
    PatBlt(hdc, DragRecPlacement.left, DragRecPlacement.top, border, height - border, PATINVERT);
  }
  SelectObject(hdc, hbrushOrig);
  DeleteObject(hbrush);
  DeleteObject(hbm);
  ReleaseDC(0, hdc);
}

