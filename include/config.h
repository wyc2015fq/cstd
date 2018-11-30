
#ifndef __CONFIG_H__
#define __CONFIG_H__

#define CPU_ONLY

#ifdef __ARM_NEON
#define __arm__
#endif

#ifdef __ARM_NEON
#ifndef CPU_ONLY
#define CPU_ONLY
#endif
#else
//#define USE_CBLAS
#endif


#ifndef CPU_ONLY
#define USE_CUDNN
#endif

// USE_CBLAS

#define countof(arr) sizeof(arr)/sizeof(arr[0])
#define MIN(a, b)  ((a) < (b) ? (a) : (b))
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#define BOUND(x, a, b)   ((x)<(a) ? (a) : ((x)>(b) ? (b) :(x)) )
#define ASSERT(x)  assert(x)
#define CMP(a, b)  ((a)>(b)) - ((a)<(b))
#define T_SWAP(T, a, b)   {T t = a; a=b; b=t;}
#define IFRET(ret, comp)  ret = comp; if (ret) {return ret;}
#define IFCMPRET(ret, a, b)  ret = CMP(a, b);if (ret) {return ret;}
#define MYREALLOC(ptr, n)   *(void**)(&ptr) = realloc(ptr, n*sizeof(*ptr))
#define FREE(ptr)   if (ptr) {free(ptr);ptr = NULL;}
#define ABS(x)  (x)<0 ? -(x) : (x)

#include <stdio.h>
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
