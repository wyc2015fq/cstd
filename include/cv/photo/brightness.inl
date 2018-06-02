
#include "cstd.h"

int brightness(int h, int w, const uchar* src, int srcstep, uchar* dst, int dststep, int cn, int brightness)
{
  uchar lookupTable[256];  
  int i, j;
  for (i = 0; i < 256; i++) {
    double t = i + sin(M_PI*i / 255.0f)*brightness;
    lookupTable[i] = (uchar)BOUND(t, 0, 255);
  }  
  for (i = 0; i < h; ++i) {
    const uchar* src1 = src + i*srcstep;
    uchar* dst1 = dst + i*dststep;
    for (j = 0; j < w*cn; ++j) {
      dst1[j] = lookupTable[src1[j]];
    }
  }
  return 0;
}