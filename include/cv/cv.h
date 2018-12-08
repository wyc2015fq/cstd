
#ifndef _CV_H_
#define _CV_H_


#include "stdc/stdc.h"

#define CC_EXPORTS
#define CC_INSTRUMENT_REGION()
#define CC_INSTRUMENT_REGION()
#define CC_OCL_RUN(a, b)
#define CC_IPP_RUN(a, b)
#define CC_IPP_RUN_FAST(a)
#define CC_MALLOC_ALIGN 64
#define alignPtr(x, n)   (x)
#define alignSize(x, n)   (x)
//#define CC_Assert assert

struct Scalar {
  double val[4];
  Scalar() {
    val[0] = val[1] = val[2] = val[3] = 0;
  }
  void set1(double x) {
    val[0] = val[1] = val[2] = val[3] = x;
  }
  static Scalar all(double x) {
    Scalar s;
    s.set1(x);
    return s;
  }
};

void scalarToRawData(const Scalar& s, void* _buf, int type, int unroll_to) {
  TypeId depth = CC_MAT_DEPTH(type);
  int cn = CC_MAT_CN(type);
  int elemsize = CC_TYPE_SIZE(depth);
  assert(cn <= 4);
  arrcvt(_buf, depth, s.val, CC_64F, cn);
  memunroll(_buf, cn*elemsize, unroll_to*elemsize);
}

#include "core/base.hpp"
#include "imgproc/imgproc.hpp"

#endif // _CV_H_
