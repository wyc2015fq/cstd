{
  switch (im->c) {
  case 1:
#define PUTPIXX(dst, x, y)          { GETCLR(pclr, x, y); (dst[0]=pclr[0]); }
#include "imdrawrectimpl.inl"
    break;
  case 3:
  case 4:
#define PUTPIXX(dst, x, y)          {GETCLR(pclr, x, y); (dst[0]=pclr[0]), (dst[1]=pclr[1]), (dst[2]=pclr[2]); }
#include "imdrawrectimpl.inl"
    break;
  default:
    ASSERT(0);
    break;
  }
#undef GETCLR
}