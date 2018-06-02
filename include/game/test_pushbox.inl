// 推箱子游戏 为研究A*寻路算法的实现
#include "test_maze.inl"
#include "Astar.inl"
#include "PushBoxGame.inl"
#include "win.inl"
#include <mmsystem.h>
//自定义函数
//void InitGame(int level); //游戏初始化
//BOOL ProLG(); //游戏逻辑
//void FlagShow(); //显示旗帜
//BOOL IfPass(); //判断是否过关
int test_nor()
{
  char buf[256];
  //char buf2[256];
  Image* im;
  Image* im2;
  Image* im3;
  int h = 30, w = 40, allh = 0, n;
  FILE* pf;
  DWORD* pix;
  _chdir("D:\\home\\Downloads\\PushBox\\");
  pf = fopen("list.txt", "rb");
  im3 = newImage(w, h * 20, 4);
  if (NULL == pf) {
    return 0;
  }
  for (; fgets(buf, 256, pf) > 0;) {
    strtrim(buf, "\r\n");
    im = loadbmpfile(buf, 32, 1);
    if (im->h == h && im->w == w * 2) {
      im2 = newImage(im->w / 2, im->h, 4);
      pix = (DWORD*)(im + 1);
      imand_32u(im->h, im->w / 2, pix, im->w, pix + im->w / 2, im->w, (DWORD*)(im2 + 1), im->w / 2, 0xffffffff);
      copy2d(im2->h, im2->bw, im2 + 1, im2->bw, ((uchar*)(im3 + 1)) + (allh * im2->bw), im3->bw);
      //_snprintf(buf2, 256, "%s.txt", buf);
      //data2hex(im2->h, im2->bw, (im2+1), im2->bw, buf2);
      //_snprintf(buf2, 256, "%s.bmp", buf);
      //savebmpfile(buf2, im2, 32, 32, 1);
      allh += h;
      free(im2);
    }
    if (im->h == h && im->w == w) {
      uchar* p = (uchar*)(im + 1);
      int i, n = im->h * im->bw;
      for (i = 0; i < n; i += 4) {
        p[i + 3] = 0;
      }
      copy2d(im->h, im->bw, im + 1, im->bw, ((uchar*)(im3 + 1)) + (allh * im->bw), im->bw);
      allh += h;
    }
    free(im);
  }
  im3->h = allh;
  savebmpfile("all.bmp", im3, 32, 8, 1);
  n = im3->h * im3->bw + sizeof(*im3);
  savedata2txt(n, 1, im3, 1, "allim.txt", "0x", "%02x", ",\n");
  fclose(pf);
  free(im3);
  return 0;
}
PushBoxGame mydraw; //定义全局绘图对像
LRESULT test_pushbox_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, WINDEF* wd)
{
  PAINTSTRUCT ps;
  HDC hdc;
  switch (message) {
  case WM_COMMAND:
#if 0
    switch (LOWORD(wParam)) {
    case IDR_EXIT:
      PostQuitMessage(0);
      break;
    case IDR_START:
      InitGame(level);
      break;
    case IDR_SLECTPASS:
      DialogBox(hInst, (LPCTSTR)IDD_DIALOG1, hWnd, (DLGPROC)SelectPass);
      break;
    }
#endif
    break;
  case WM_KEYDOWN: {
    if (wParam == VK_ESCAPE) {
      // Destroy main window
      DestroyWindow(hWnd);
    }
    else if ((_TCHAR)wParam == '0') {
    }
    else if ((_TCHAR)wParam == '1') {
    }
    else if ((_TCHAR)wParam == '2') {
    }
    else if ((_TCHAR)wParam == '3') {
    }
    else if ((_TCHAR)wParam == '4') {
    }
    else if ((_TCHAR)wParam == '5') {
    }
    else if ((_TCHAR)wParam == '6') {
    }
    else if ((_TCHAR)wParam == 'B') {
    }
    else if ((_TCHAR)wParam == 'P') {
    }
    else if ((_TCHAR)wParam == ' ') {
      Image* immaze = mydraw.maps[mydraw.mapslen - 1];
      makeMaze(immaze->h, immaze->w, (char*)(immaze + 1), immaze->bw);
      mydraw.level = -1;
      InitGame(&mydraw, hWnd);
    }
    else if ((_TCHAR)wParam == VK_LEFT) {
      mydraw.level = (mydraw.level - 1);
      InitGame(&mydraw, hWnd);
    }
    else if ((_TCHAR)wParam == VK_RIGHT) {
      mydraw.level = (mydraw.level + 1);
      InitGame(&mydraw, hWnd);
    }
    else if ((_TCHAR)wParam == VK_UP) {
    }
    else if ((_TCHAR)wParam == VK_DOWN) {
    }
    else if ((_TCHAR)wParam == 'R') {
    }
  }
  break;
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    DrawAll(&mydraw, hdc);
    EndPaint(hWnd, &ps);
    break;
  case WM_LBUTTONDOWN:
    LMouseDown(&mydraw, LOWORD(lParam), HIWORD(lParam));
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}
int test_pushbox()
{
  //游戏循环时间控制参数
  WINDEF wd = {test_pushbox_WndProc};
  int i = initPushBoxGame(&mydraw);
  HWND hWnd = newwin(&wd);
  Image* immaze = newImage(31, 21, 1);
  makeMaze(immaze->h, immaze->w, (char*)(immaze + 1), immaze->bw);
  AddMap(&mydraw, immaze);
  mydraw.level = -1;
  InitGame(&mydraw, hWnd);
  ShowWindow(hWnd, 1);
  UpdateWindow(hWnd);
  MSGLOOP_BEGIN(30);
  ProLG(&mydraw, hWnd);
  MSGLOOP_END();
  delPushBoxGame(&mydraw);
  return 0;
}

