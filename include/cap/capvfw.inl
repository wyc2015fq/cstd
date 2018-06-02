#ifndef _VFW_INL_
#define _VFW_INL_
#include <vfw.h>
#pragma comment(lib,"vfw32.lib")
#include "cstd.h"
#define MKTAG1(a, b, c, d) (((d)<<24) | ((c)<<16) | ((b)<<8) | (a))
#define MKTAG MAKEFOURCC
typedef struct capvfw {
  HWND m_hWndCap;
  HIC hic;
  DWORD fourcc;
  VIDEOHDR* hdr;
  CAPDRIVERCAPS caps;
} capvfw;
int capvfw_close(capdev* s0)
{
  capvfw* s = (capvfw*)s0->x;
  if (s->m_hWndCap && IsWindow(s->m_hWndCap)) {
    capCaptureAbort(s->m_hWndCap);
    capSetCallbackOnVideoStream(s->m_hWndCap, NULL);
    //断开与驱动程序的连接
    capDriverDisconnect(s->m_hWndCap);
    DestroyWindow(s->m_hWndCap);
    if (s->hic) {
      ICDecompressEnd(s->hic);
      ICClose(s->hic);
      s->hic = 0;
    }
    s->m_hWndCap = NULL;
  }
  return 0;
}
LRESULT PASCAL capvfw_frameCallback(HWND hWnd, VIDEOHDR* hdr)
{
  capvfw* s = 0;
  if (!hWnd) {
    return FALSE;
  }
  s = (capvfw*) capGetUserData(hWnd);
  s->hdr = hdr;
  return (LRESULT) TRUE;
}
int coinit2()
{
  static int coinited = 0;
  if (coinited == 0) {
    coinited = 1;
    CoInitialize(NULL);
    //atexit(CoUninitialize);
  }
  return 0;
}
static BITMAPINFOHEADER icvBitmapHeader(int width, int height, int bpp, int compression /*= BI_RGB*/)
{
  BITMAPINFOHEADER bmih;
  memset(&bmih, 0, sizeof(bmih));
  bmih.biSize = sizeof(bmih);
  bmih.biWidth = width;
  bmih.biHeight = height;
  bmih.biBitCount = (WORD) bpp;
  bmih.biCompression = compression;
  bmih.biPlanes = 1;
  return bmih;
}
int capvfw_getframe(capdev* s0, img_t* im, int flag)
{
  capvfw* s = (capvfw*)s0->x;
  BITMAPINFO vfmt;
  BITMAPINFOHEADER* vfmt0;
  int sz;
  if (NULL == s->m_hWndCap) {
    return FALSE;
  }
  SendMessage(s->m_hWndCap, WM_CAP_GRAB_FRAME_NOSTOP, 0, 0);
  coinit2();
  memset(&vfmt, 0, sizeof(vfmt));
  vfmt0 = &vfmt.bmiHeader;
  if (!s->m_hWndCap) {
    return FALSE;
  }
  sz = capGetVideoFormat(s->m_hWndCap, &vfmt, sizeof(vfmt));
  if (!s->hdr || s->hdr->lpData == 0 || sz == 0) {
    return FALSE;
  }
  imsetsize(im, vfmt0->biHeight, vfmt0->biWidth, 3, 1);
  if (vfmt.bmiHeader.biCompression != BI_RGB ||
      vfmt.bmiHeader.biBitCount != 24) {
    BITMAPINFOHEADER vfmt1 = icvBitmapHeader(vfmt0->biWidth, vfmt0->biHeight, 24, BI_RGB);
    if (s->hic == 0 || s->fourcc != vfmt0->biCompression) {
      if (s->hic) {
        ICDecompressEnd(s->hic);
        ICClose(s->hic);
      }
      s->hic = ICOpen(MAKEFOURCC('V', 'I', 'D', 'C'), vfmt0->biCompression, ICMODE_DECOMPRESS);
      if (s->hic) {
        if (ICDecompressBegin(s->hic, vfmt0, &vfmt1) != ICERR_OK) {
          ICDecompressEnd(s->hic);
          ICClose(s->hic);
          return FALSE;
        }
      }
    }
    if (!s->hic || ICDecompress(s->hic, 0, vfmt0, s->hdr->lpData, &vfmt1, im->tt.data) != ICERR_OK) {
      if (s->hic) {
        ICDecompressEnd(s->hic);
        ICClose(s->hic);
      }
      return FALSE;
    }
    memcpy(im->tt.data, s->hdr->lpData, im->s * im->h);
  }
  else {
    imsetsize(im, vfmt0->biHeight, vfmt0->biWidth, 3, 1);
    memcpy(im->tt.data, s->hdr->lpData, im->s * im->h);
  }
  return TRUE;
}
int capvfw_find(int wIndex) {
  char szDeviceName[ 80 ];
  char szDeviceVersion[ 80 ];
  for (; wIndex < 16; wIndex++) {
    if (capGetDriverDescription(wIndex, szDeviceName, sizeof(szDeviceName), szDeviceVersion, sizeof(szDeviceVersion))) {
      return wIndex;
    }
  }
  return -1;
}
int capvfw_open(capdev* s0, int wIndex, int width, int height)
{
  HWND hWndC = 0;
  capvfw* s = NULL;
  wIndex = capvfw_find(wIndex);
  if (NULL==s0 || wIndex<0) {
    return wIndex;
  }
  if (wIndex>=0) {
    hWndC = capCreateCaptureWindow("My Own Capture Window", WS_POPUP | WS_CHILD, 0, 0, width, height, 0, 0);
    if (!capDriverConnect(hWndC, wIndex)) {
      DestroyWindow(hWndC);
      return -1;
    }
  }
  if (hWndC) {
    CAPTUREPARMS p;
    s = (capvfw*)pmalloc(sizeof(*s));
    memset(s, 0, sizeof(*s));
    s->m_hWndCap = hWndC;
    s->hdr = 0;
    s->hic = 0;
    s->fourcc = (DWORD) - 1;
    capDriverGetCaps(hWndC, &s->caps, sizeof(&s->caps));
    MoveWindow(hWndC, 0, 0, 320, 240, TRUE);
    capSetUserData(hWndC, (size_t) s);
    capSetCallbackOnFrame(hWndC, capvfw_frameCallback);
    capCaptureGetSetup(hWndC, &p, sizeof(CAPTUREPARMS));
    p.dwRequestMicroSecPerFrame = 66667 / 2;
    capCaptureSetSetup(hWndC, &p, sizeof(CAPTUREPARMS));
    //capPreview( hWndC, 1 );
    capPreviewScale(hWndC, FALSE);
    capPreviewRate(hWndC, 1);
  }
  if (hWndC != NULL) {
    s0->x = s;
    s0->getframe = capvfw_getframe;
    s0->close = capvfw_close;
  }
  return wIndex+1;
}

#endif // _VFW_INL_

