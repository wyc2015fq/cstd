#include "./res/xpm/obm_checkboxes.xpm"
#include "img/imgio.inl"
HBITMAP LoadXpm(const char** xpm)
{
  BITMAP bmp = {0};
  HBITMAP hbmp = NULL;
  img_t im[1] = {0};
  xpm_load_chars(xpm, 4, im, 0, 0);
  bmp.bmWidthBytes = im->s;
  bmp.bmHeight = im->h;
  bmp.bmWidth = im->w;
  bmp.bmBitsPixel = 32;
  bmp.bmPlanes = 1;
  bmp.bmBits = im->tt.data;
  hbmp = CreateBitmapIndirect(&bmp);
  //printf("%s\n", GetErrorMsg(0));
  imfree(im);
  return hbmp;
}

