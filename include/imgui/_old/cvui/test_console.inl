#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
HWND WINAPI GetConsoleWindow();
int test_console(int argc, char* argv[])
{
  HWND hwnd;
  HDC hdc;
  HPEN hpen;
  hwnd = GetConsoleWindow();
  hdc = GetDC(hwnd);
  system("color F0");
  system("cls");
  hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  SelectObject(hdc, hpen);
  MoveToEx(hdc, 20, 20, NULL);
  LineTo(hdc, 200, 300);
  DeleteObject(hpen);
  ReleaseDC(hwnd, hdc);
  getchar();
  return 0;
}

