#ifndef CAFFE_UTIL_MKL_ALTERNATE_H_
#define CAFFE_UTIL_MKL_ALTERNATE_H_

#ifdef USE_MKL

#include <mkl.h>

#else  // If use MKL, simply include the MKL header

extern "C" {
#include <cblas.h>
}
#include <math.h>

#endif  // USE_MKL
#endif  // CAFFE_UTIL_MKL_ALTERNATE_H_
