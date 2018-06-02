
enum {
  DCT_LINE,
  DCT_RECT,
  DCT_POLY,
  DCT_ELLIPSE,
  DCT_BORDER,
};

enum {
  DCT_INT,
  DCT_FLOAT,
  DCT_DOUBLE,
};

int pt2dpt(void* args, int type, int n, int shift, DPOINT* pt)
{
  int i;
  IPOINT* ipt = (IPOINT*)args;
  FPOINT* fpt = (FPOINT*)args;
  DPOINT* dpt = (DPOINT*)args;

  switch (type) {
  case DCT_INT:
    for (i = 0; i < n; ++i) {
      pt[i++] = dPOINT(ipt[i].x * 1. / shift, ipt[i].y * 1. / shift);
    }

    break;

  case DCT_FLOAT:
    for (i = 0; i < n; ++i) {
      pt[i++] = dPOINT(fpt[i].x, fpt[i].y);
    }

    break;

  case DCT_DOUBLE:
    for (i = 0; i < n; ++i) {
      pt[i++] = dPOINT(dpt[i].x, dpt[i].y);
    }

    break;
  }

  return n;
}

int dcdraw(dc_t* dc, int shape, int type, void* args, int* lens, int n, int shift)
{
  img_t* im = dc->im;
  int step = im->s, cn = im->c;
  uchar* img = im->tt.data;
  uchar* pclr = (uchar*)(&dc->clrFill);
  char* arg = (char*)(args);
  IRECT clip = dc->clip;

#define GET_PIX(pclr, x, y)   {}
#define PUT_PIX(ptr_x, pclr, alpha)   { PIXBLEND(ptr_x[0], pclr[0], alpha); PIXBLEND(ptr_x[1], pclr[1], alpha); PIXBLEND(ptr_x[2], pclr[2], alpha); }
  //#include "dcdraw_impl.inl"
#include "dcdraw_impl_brush.inl"
#undef PUT_PIX
  return 0;
}
