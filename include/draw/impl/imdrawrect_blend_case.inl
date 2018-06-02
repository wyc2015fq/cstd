{
  switch (im->c) {
  case 1:
//#define PUTPIXX(dst, x, y)          { GETCLR(pclr, x, y); PIXBLEND(dst[0], pclr[0], pclr[3]); }
//#include "imdrawrectimpl.inl"
    break;
  case 3:
#define PUTPIXX(dst, x, y)          { GETCLR(pclr, x, y); PIXBLEND(dst[0], pclr[0], pclr[3]), PIXBLEND(dst[1], pclr[1], pclr[3]), PIXBLEND(dst[2], pclr[2], pclr[3]); }
#include "imdrawrectimpl.inl"
  case 4:
#define PUTPIXX(dst, x, y)          { GETCLR(pclr, x, y); PIXBLEND(dst[0], pclr[0], pclr[3]), PIXBLEND(dst[1], pclr[1], pclr[3]), PIXBLEND(dst[2], pclr[2], pclr[3]); dst[3] = 255; }
#include "imdrawrectimpl.inl"
    break;
  case 0:
    break;
  default:
    ASSERT(0);
    break;
  }
#undef GETCLR
}