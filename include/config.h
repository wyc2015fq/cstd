
#ifndef __CONFIG_H__
#define __CONFIG_H__

#pragma warning(disable:4068 4756)

#include "std/stddef_c.h"
//#define USE_OPENCV
//#define CPU_ONLY

#ifdef __ARM_NEON
#define __arm__
#if __aarch64__
#define __ARM_V8
#else
#define __ARM_V7
#endif
#endif

#ifdef __ARM_NEON
#ifndef CPU_ONLY
#define CPU_ONLY
#endif
#else
#define USE_CBLAS
#endif


#ifndef CPU_ONLY
#define USE_CUDNN
#endif

// USE_CBLAS

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON
/////////////////////

#if 1
//#define countof(arr) sizeof(arr)/sizeof(arr[0])
//#define MIN(a, b)  ((a) < (b) ? (a) : (b))
//#define MAX(a, b)  ((a) > (b) ? (a) : (b))
//#define BOUND(x, a, b)   ((x)<(a) ? (a) : ((x)>(b) ? (b) :(x)) )
//#define ASSERT(x)  assert(x)
//#define T_SWAP(T, a, b)   {T t = a; a=b; b=t;}
#define CMP(a, b)  ((a)>(b)) - ((a)<(b))
#define IFRET(ret, comp)  ret = comp; if (ret) {return ret;}
#define IFCMPRET(ret, a, b)  ret = CMP(a, b);if (ret) {return ret;}
//#define MYREALLOC(ptr, n)   *(void**)(&ptr) = realloc(ptr, n*sizeof(*ptr))
//#define FREE(ptr)   if (ptr) {free(ptr);ptr = NULL;}
#define ABS(x)  (x)<0 ? -(x) : (x)
#endif

#include <stdio.h>
#define OUT_FILE(name, n, a, fmt)  { FILE* pf = fopen(name, "wb"); if (pf) { \
  for (int i = 0; i < n; ++i) { fprintf(pf, fmt, a[i]);  }  fclose(pf); } }

static int out_file(const char* name, int n, const float* a) {
  FILE* pf = fopen(name, "wb");
  if (pf) {
    for (int i = 0; i < n; ++i) {
      fprintf(pf, "%f,\n", a[i]);
    }
    fclose(pf);
  }
  return 0;
}


#endif // __CONFIG_H__
