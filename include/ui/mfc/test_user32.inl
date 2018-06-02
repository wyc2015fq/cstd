
#include <windows.h>
enum {
  OBM_CHECKBOXES,
};

typedef struct tagUPDATELAYEREDWINDOWINFO
{
  DWORD                cbSize;
  HDC                  hdcDst;
  POINT CONST*         pptDst;
  SIZE CONST*          psize;
  HDC                  hdcSrc;
  POINT CONST*         pptSrc;
  COLORREF             crKey;
  BLENDFUNCTION CONST* pblend;
  DWORD                dwFlags;
  RECT CONST*          prcDirty;
} UPDATELAYEREDWINDOWINFO, *PUPDATELAYEREDWINDOWINFO;

#include "user32/libuser32.inl"

int test_user32() {
  //BOOL
  return 0;
}
