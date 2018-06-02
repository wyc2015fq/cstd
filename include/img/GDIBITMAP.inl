//GDI操作BITMAP的基本编程方法
#ifndef _GDIBITMAP_INL_
#define _GDIBITMAP_INL_
#include <windows.h>
#include <stdio.h>

int SaveImage(const char* bmp_file, int* rgb_data, int rgb_len, BITMAPINFOHEADER* bi) // 保存BITMAP为文件
{
  BITMAPFILEHEADER hdr;
  FILE* fsave;
  memset(&hdr, 0, sizeof(hdr));
  hdr.bfType = ((WORD)('M' << 8) | 'B');
  hdr.bfSize = sizeof(hdr) + sizeof(BITMAPINFOHEADER) + rgb_len;
  hdr.bfOffBits = (DWORD)(sizeof(hdr) + sizeof(BITMAPINFOHEADER));

  fsave = fopen(bmp_file, "wb");
  fwrite(&hdr, sizeof(hdr), 1, fsave);
  fwrite(bi, sizeof(BITMAPINFOHEADER), 1, fsave);
  fwrite(rgb_data, rgb_len, 1, fsave);
  fflush(fsave);
  fclose(fsave);
  return 0;
}


int FillBitmapInfoHdr(BITMAPINFOHEADER* bi, int width, int height, int deep) // 填充BITMAP头信息
{
  memset(bi, 0, sizeof(BITMAPINFOHEADER));
  bi->biSize = sizeof(BITMAPINFOHEADER);
  bi->biWidth = width;
  bi->biHeight = height;
  bi->biPlanes = 1;
  bi->biBitCount = deep;//bm.bmPlanes * bm.bmBitsPixel;
  bi->biCompression = BI_RGB;
  bi->biSizeImage = 0;
  bi->biXPelsPerMeter = 0;
  bi->biYPelsPerMeter = 0;
  bi->biClrUsed = 0;
  bi->biClrImportant = 0;
  return 0;
}

bool GetImageData2(HWND target, RECT rect, int* data, int* len)  // 获取HWND中的BITMAP信息
{
  HDC entireDC;
  RECT rc;
  HBITMAP bitmap;
  HDC CompDC;
  BITMAPINFO bmi;
  BITMAPINFOHEADER* bi;
  int rgb_len;
  int lines;
  char* rgb_buff;

  entireDC = GetDC(target);
  //GetWindowRect(target,&rc);
  rc = rect;

  if (rc.right - rc.left <= 0 || rc.bottom - rc.top <= 0) {
    return false;
  }

  bitmap = CreateCompatibleBitmap(entireDC, rc.right - rc.left, rc.bottom - rc.top);

  if (bitmap == NULL) {
    ReleaseDC(target, entireDC);
    return false;
  }

  CompDC = CreateCompatibleDC(entireDC);
  SelectObject(CompDC, bitmap);
  BitBlt(CompDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, entireDC, rect.left, rect.top, SRCCOPY);

  bi = &bmi.bmiHeader;
  FillBitmapInfoHdr(bi, rc.right - rc.left, rc.bottom - rc.top, 24);

  rgb_len = bi->biWidth * bi->biHeight * 3;

  rgb_buff = (char*) data; // new char[rgb_len];

  if (rgb_buff == NULL) {
    return false;
  }

  lines = GetDIBits(CompDC, bitmap, 0, rc.bottom - rc.top, rgb_buff,
      (LPBITMAPINFO)bi, DIB_RGB_COLORS);
  DeleteObject(bitmap);
  ReleaseDC(target, CompDC);
  ReleaseDC(target, entireDC);
  DeleteDC(CompDC);

  if (lines == 0) {
    // pfree()rgb_buff;
    return false;
  }

  // *data = rgb_buff;
  *len = rgb_len;
  return true;
}


bool GetImageData(HWND target, RECT rect, int** data, int* len)  // 获取HWND中的BITMAP信息
{
  HDC entireDC;
  RECT rc;
  HBITMAP bitmap;
  HDC CompDC;
  BITMAPINFO bmi;
  BITMAPINFOHEADER* bi;
  int rgb_len;
  int lines;
  char* rgb_buff;

  entireDC = GetDC(target);
  //GetWindowRect(target,&rc);
  rc = rect;
  bitmap = CreateCompatibleBitmap(entireDC, rc.right - rc.left, rc.bottom - rc.top);
  CompDC = CreateCompatibleDC(entireDC);
  SelectObject(CompDC, bitmap);
  BitBlt(CompDC, 0, 0, rc.right - rc.left, rc.bottom - rc.top, entireDC, 0, 0, SRCCOPY);

  bi = &bmi.bmiHeader;
  FillBitmapInfoHdr(bi, rc.right - rc.left, rc.bottom - rc.top, 24);

  rgb_len = bi->biWidth * bi->biHeight * 3;
  rgb_buff = NEW(char, rgb_len);

  if (rgb_buff == NULL) {
    return false;
  }

  lines = GetDIBits(CompDC, bitmap, 0, rc.bottom - rc.top, rgb_buff,
      (LPBITMAPINFO)bi, DIB_RGB_COLORS);
  DeleteObject(bitmap);
  ReleaseDC(target, CompDC);
  ReleaseDC(target, entireDC);
  DeleteDC(CompDC);

  if (lines == 0) {
    pfree(rgb_buff);
    return false;
  }

  *data = (int*)rgb_buff;
  *len = rgb_len;
  return true;
}

// 获取HWND的BITMAP为文件
int CaptureImage(HWND target, const char* destfile)
{
  int* data;
  int len;
  RECT rc;
  BITMAPINFO bmi;
  BITMAPINFOHEADER* bi;
  GetWindowRect(target, &rc);
  GetImageData(target, rc, &data, &len);

  bi = &bmi.bmiHeader;
  FillBitmapInfoHdr(bi, rc.right - rc.left, rc.bottom - rc.top, 24);

  SaveImage(destfile, data, len, bi);

  pfree(data);
  return 0;
}

int DrawBitmap2(HWND hwnd, RECT rect, char* data, int deep) // 在HWND中绘制BITMAP
{
  BITMAPINFOHEADER* bi;
  BITMAPINFO bitmap;
  HDC hdc;

  bi = &bitmap.bmiHeader;
  memset(&bitmap, 0, sizeof(bitmap));
  bi->biSize = sizeof(BITMAPINFOHEADER);
  bi->biWidth = rect.right - rect.left;
  bi->biHeight = rect.bottom - rect.top;
  bi->biPlanes = 1;
  bi->biBitCount = deep;//bm.bmPlanes * bm.bmBitsPixel;
  bi->biCompression = BI_RGB;
  bi->biSizeImage = bi->biWidth * bi->biHeight * deep / 3;
  bi->biXPelsPerMeter = 0;
  bi->biYPelsPerMeter = 0;
  bi->biClrUsed = 0;
  bi->biClrImportant = 0;

  hdc = GetDC(hwnd);

  StretchDIBits(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, rect.right - rect.left, rect.bottom - rect.top, (CONST int*)data, &bitmap, DIB_RGB_COLORS, SRCCOPY);

  //ReleaseDC(hwnd,hdc);
  ReleaseDC(hwnd, hdc);
  return 0;
}

int StretchDrawBitmap(HWND hwnd, int width, int height, char* data, int deep) // 在HWND中绘制BITMAP
{
  BITMAPINFOHEADER* bi;
  BITMAPINFO bitmap;
  RECT rc;
  HDC hdc;

  bi = &bitmap.bmiHeader;
  memset(&bitmap, 0, sizeof(bitmap));
  bi->biSize = sizeof(BITMAPINFOHEADER);
  bi->biWidth = width;
  bi->biHeight = height;
  bi->biPlanes = 1;
  bi->biBitCount = deep;//bm.bmPlanes * bm.bmBitsPixel;
  bi->biCompression = BI_RGB;
  bi->biSizeImage = bi->biWidth * bi->biHeight * deep / 3;
  bi->biXPelsPerMeter = 0;
  bi->biYPelsPerMeter = 0;
  bi->biClrUsed = 0;
  bi->biClrImportant = 0;
  hdc = GetDC(hwnd);
  GetWindowRect(hwnd, &rc);
  StretchDIBits(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0, 0, width, height, (CONST int*)data, &bitmap, DIB_RGB_COLORS, SRCCOPY);

  //ReleaseDC(hwnd,hdc);
  ReleaseDC(hwnd, hdc);
  return 0;
}

int DrawBitmap(HWND hwnd, int width, int height, char* data, int deep) // 在HWND中绘制BITMAP
{
  BITMAPINFOHEADER* bi;
  BITMAPINFO bitmap;
  HDC hdc;

  bi = &bitmap.bmiHeader;
  memset(&bitmap, 0, sizeof(bitmap));
  bi->biSize = sizeof(BITMAPINFOHEADER);
  bi->biWidth = width;
  bi->biHeight = height;
  bi->biPlanes = 1;
  bi->biBitCount = deep;//bm.bmPlanes * bm.bmBitsPixel;
  bi->biCompression = BI_RGB;
  bi->biSizeImage = width * height * deep;
  bi->biXPelsPerMeter = 0;
  bi->biYPelsPerMeter = 0;
  bi->biClrUsed = 0;
  bi->biClrImportant = 0;

  hdc = GetDC(hwnd);
  StretchDIBits(hdc, 0, 0, width - 1, height - 1, 0, 0, width, height, (CONST int*)data, &bitmap, DIB_RGB_COLORS, SRCCOPY);
  //ReleaseDC(hwnd,hdc);
  ReleaseDC(hwnd, hdc);
  return 0;
}

#endif _GDIBITMAP_INL_
