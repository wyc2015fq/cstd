
#include "cstd.h"
#include "cfile.h"
#include "cv/cv.inl"
#include "face/face.inl"
//#include "draw/imdraw.inl"
//#include "sys/threadpool.inl"


// 1-16 Á³
// 17-21 ×óÃ¼
// 22-26 ÓÒÃ¼
// 27-30 ±ÇÁº
// 31-35 ±Ç×ÓÏÂ²¿
// 36-41 ×óÑÛ
// 42-47 ÓÒÑÛ
// 48-59 ×ìÍâÂÖÀª
// 60-67 ×ìÍâÂÖÀª
int drawkonhon(img_t* img, const float* pt)
{
  FPOINT pts[256];
  int i0 = 48, i1 = 60, i2 = 68;
  int n1 = i1 - i0;
  int n2 = i2 - i1;
  int i, len[] = {n1, n2};
  const float* pt0 = pt + i0 * 2;
  const float* pt1 = pt + i1 * 2;

  for (i = 0; i < n1; ++i) {
    const float* p = pt0 + i * 2;
    pts[i].x = p[0];
    pts[i].y = p[1];
  }

  for (i = 0; i < n2; ++i) {
    const float* p = pt1 + (n2 - 1 - i) * 2;
    pts[n1 + i].x = p[0];
    pts[n1 + i].y = p[1];
  }

  uint clr = _RGBA(200, 50, 50, 70);
  //imdrawaa_poly_solid(img, false, NULL, pts, len, 2, clr);
  return 0;
}
