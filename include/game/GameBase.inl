typedef struct _PUSHBOXGAME {
  Image* im;
  int level; //游戏关数
  int mapslen;
  Image* maps[256];
  int rows, cols, step;
  char* map; //地图信息
  POINT flagpt[10]; //旗帜点
  POINT boxpoint[10]; //箱子数组
  POINT babypoint; //人物坐标
  POINT mousepoint; //鼠标按下坐标
  int babyn; //人物状态
  int flagn; //旗帜数量
  int boxn; //箱子个数
  int box; //记录箱子状态
  BOOL Lkeydown; //按键确认控制参数
  BOOL GameOver; //游戏结束参数
  HDC hdc;
  Image* allim;
} PUSHBOXGAME;
static HDRAWDIB m_hDD = 0;
void myDrawDibClose(void)
{
  DrawDibClose(m_hDD);
}
SIZE GetClientSize(HWND hWnd)
{
  RECT rcCli;
  SIZE sz;
  GetClientRect(hWnd, &rcCli);
  sz.w = RCWIDTH(&rcCli), sz.h = RCHEIGHT(&rcCli);
  return sz;
}
int SetClientSize(HWND hWnd, int w, int h)
{
  RECT rcWin, rcCli;
  int wx, hx;
  GetWindowRect(hWnd, &rcWin);
  GetClientRect(hWnd, &rcCli);
  hx = RCHEIGHT(&rcWin) - RCHEIGHT(&rcCli);
  wx = RCWIDTH(&rcWin) - RCWIDTH(&rcCli);
  if (h != RCHEIGHT(&rcCli) && w != RCWIDTH(&rcCli)) {
    MoveWindow(hWnd, rcWin.left, rcWin.top, w + wx, h + hx, 1);
    return 1;
  }
  return 0;
}
int DrawFrameToHDC(HWND hWnd, int w, int h, LPVOID lpBits, BOOL bResizeWin)
{
  BITMAPINFOHEADER m_bih;
  HDC hDC;
  DWORD m_dwSize;
  int bytes_per_line = w * 4;
  uchar* m_lpData2;
  if (0 == m_hDD) {
    m_hDD = DrawDibOpen();
    atexit(myDrawDibClose);
  }
  //LPBYTE m_lpZBuffer;
  if (bResizeWin) {
    SetClientSize(hWnd, w, h);
  }
  m_dwSize = h * w * 4;
  m_lpData2 = (LPBYTE)malloc(m_dwSize * sizeof(BYTE));
  hDC = GetDC(hWnd);
  memset(&m_bih, 0, sizeof(BITMAPINFOHEADER));
  m_bih.biSize = sizeof(BITMAPINFOHEADER);
  m_bih.biWidth = w;
  m_bih.biHeight = h;
  m_bih.biPlanes = 1;
  m_bih.biBitCount = 32;
  m_bih.biCompression = BI_RGB;
  m_bih.biSizeImage = m_dwSize;
  memcpy2d(m_lpData2 + (h - 1)*bytes_per_line,
      -bytes_per_line, lpBits, bytes_per_line, h, bytes_per_line);
  DrawDibDraw(m_hDD, hDC, 0, 0, w, h, &m_bih, m_lpData2, 0, 0, w, -h, 0);
  free(m_lpData2);
  ReleaseDC(hWnd, hDC);
  // printf("%d %d\n", h, w);
  return 0;
}

