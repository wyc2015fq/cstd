
#ifndef _SHOW_CNSUN_LOGO_INL_
#define _SHOW_CNSUN_LOGO_INL_

int cnsun_logo_width = 160;
uchar cnsun_logo[] = {
#include "cnsun-logo.txt"
};

int show_cnsun_logo(img_t* im) {
  int i, j;
  if (3==im->c) {
    for (i=0; i<cnsun_logo_width/4; ++i) {
      for (j=0; j<cnsun_logo_width; ++j) {
        uchar t = cnsun_logo[i*cnsun_logo_width+j];
        uchar* p = im->tt.data + i*im->s + j*im->c;
        p[0] = t;
        p[1] = t;
        p[2] = t;
      }
    }
  }
  return 0;
}

#endif // _SHOW_CNSUN_LOGO_INL_
