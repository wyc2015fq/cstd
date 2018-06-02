{
#define ADDY0(_i, _j)   (ADDX(_i+_j*al))

  switch (cch) {
#if 1
#define __cch              2

  case __cch:
#define ADDY(_i)        REPI2(_i, 0, ADDY0)
#include "imresize_downsample_case_sc.inl"
    break;
#else

  case 1:
#define __cch              1
#include "imresize_downsample_case_sc.inl"
    break;

  case 2: {
#define __cch              2
#include "imresize_downsample_case_sc.inl"
  }
  break;
#endif
  }
}

#undef ADDY0
#undef ADDX
#undef __ccw
