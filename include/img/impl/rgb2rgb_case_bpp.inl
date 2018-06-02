
switch (bbpp)
{
#if 1

case 1: {
#define PUT_IDX()   (GET_IDX(0) | GET_IDX(1) | GET_IDX(2) | GET_IDX(3) |GET_IDX(4) | GET_IDX(5) | GET_IDX(6) | GET_IDX(7))
  enum {__bit = 3};
  typedef uchar b_type;
#include "rgb2rgb_case_bit.inl"
}
break;
#else

case 32: {
#define PUT_RGB()   (B[0] = r, B[1] = g, B[2] = b)
  enum {bi = 4};
  typedef uchar b_type;
#include "rgb2rgb_case_rgb.inl"
}
break;

case 24: {
#define PUT_RGB()   (B[0] = r, B[1] = g, B[2] = b)
  enum {bi = 3};
  typedef uchar b_type;
#include "rgb2rgb_case_rgb.inl"
}
break;

case 16: {
#define PUT_RGB()   (B[0] = (b_type)RGBTOSHORT(r, g, b, 5, 6, 5))
  enum {bi = 1};
  typedef ushort b_type;
#include "rgb2rgb_case_rgb.inl"
}
break;

case 15: {
#define PUT_RGB()   (B[0] = (b_type)RGBTOSHORT(r, g, b, 5, 5, 5))
  enum {bi = 1};
  typedef ushort b_type;
#include "rgb2rgb_case_rgb.inl"
}
break;

case 12: {
#define PUT_RGB()   (B[0] = (b_type)RGBTOSHORT(r, g, b, 4, 4, 4))
  enum {bi = 1};
  typedef ushort b_type;
#include "rgb2rgb_case_rgb.inl"
}
break;

case 8: {
#define PUT_IDX()   (GET_IDX(0))
  enum {__bit = 0};
  typedef uchar b_type;
#include "rgb2rgb_case_bit.inl"
}
break;

case 4: {
#define PUT_IDX()   (GET_IDX(0) | GET_IDX(1))
  enum {__bit = 1};
  typedef uchar b_type;
#include "rgb2rgb_case_bit.inl"
}
break;

case 2: {
#define PUT_IDX()   (GET_IDX(0) | GET_IDX(1) | GET_IDX(2) | GET_IDX(3))
  enum {__bit = 2};
  typedef uchar b_type;
#include "rgb2rgb_case_bit.inl"
}
break;

case 1: {
#define PUT_IDX()   (GET_IDX(0) | GET_IDX(1) | GET_IDX(2) | GET_IDX(3) |GET_IDX(4) | GET_IDX(5) | GET_IDX(6) | GET_IDX(7))
  enum {__bit = 3};
  typedef uchar b_type;
#include "rgb2rgb_case_bit.inl"
}
break;
#endif

default:
  ASSERT(0);
  break;
}

#undef GET_RGB
#undef GET_IDX
