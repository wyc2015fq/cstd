switch (im->c) {
case 1:
//#define PUT_PIX(dst, _alpha, _x, _y)   { GETCLR(pclr, _a, _x, _y, _alpha); PIXBLEND(dst[0], pclr[0], _a); }
//#include "imdrawaa_poly_impl.inl"
  break;
case 3:
#define PUT_PIX(dst, _alpha, _x, _y)   { GETCLR(_x, _y); _a = (_alpha * pclr[3])>>8; if (_a) {PIXBLEND(dst[0], pclr[0], _a*pmask[0]>>8); PIXBLEND(dst[1], pclr[1], _a*pmask[1]>>8); PIXBLEND(dst[2], pclr[2], _a*pmask[2]>>8);} }
#include "imdrawaa_poly_impl.inl"
  break;
case 4:
#define PUT_PIX(dst, _alpha, _x, _y)   { GETCLR(_x, _y); _a = (_alpha * pclr[3])>>8; if (_a) {PIXBLEND(dst[0], pclr[0], _a*pmask[0]>>8); PIXBLEND(dst[1], pclr[1], _a*pmask[1]>>8); PIXBLEND(dst[2], pclr[2], _a*pmask[2]>>8);PIXBLEND(dst[3], pclr[3], _a*pmask[3]>>8);} }
#include "imdrawaa_poly_impl.inl"
  break;
default:
  ASSERT(0);
  break;
}
#undef GETCLR
#undef GETCLR_INIT
