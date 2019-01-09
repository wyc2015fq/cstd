
#include "cstd.h"
#include "cpu.h"

#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64)
  #define EIGEN_ARCH_x86_64 1
#else
  #define EIGEN_ARCH_x86_64 0
#endif

#if defined(__i386__) || defined(_M_IX86) || defined(_X86_) || defined(__i386)
  #define EIGEN_ARCH_i386 1
#else
  #define EIGEN_ARCH_i386 0
#endif


#if EIGEN_ARCH_x86_64 || EIGEN_ARCH_i386
  #define EIGEN_ARCH_i386_OR_x86_64 1
#else
  #define EIGEN_ARCH_i386_OR_x86_64 0
#endif

struct CacheSize {
  int l1, l2, l3;
};

CC_INLINE int manage_caching_sizes_helper(int a, int b)
{
  return a<=0 ? b : a;
}

CacheSize getCacheSizes() {
  CacheSize sz = {0};
#if EIGEN_ARCH_i386_OR_x86_64
  const int defaultL1CacheSize = 32*1024;
  const int defaultL2CacheSize = 256*1024;
  const int defaultL3CacheSize = 2*1024*1024;
#else
  const int defaultL1CacheSize = 16*1024;
  const int defaultL2CacheSize = 512*1024;
  const int defaultL3CacheSize = 512*1024;
#endif
  cpu_info_t* f = get_cpu_info();
  sz.l1 = f->l1;
  sz.l2 = f->l2;
  sz.l3 = f->l3;
  sz.l1 = manage_caching_sizes_helper(sz.l1, defaultL1CacheSize);
  sz.l2 = manage_caching_sizes_helper(sz.l2, defaultL2CacheSize);
  sz.l3 = manage_caching_sizes_helper(sz.l3, defaultL3CacheSize);
  return sz;
}

int test_cpu() {
  cpu_info_t* f = get_cpu_info();
  getCacheSizes();
  return 0;
}

