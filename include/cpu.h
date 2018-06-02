
#ifndef _CPU_H_
#define _CPU_H_

typedef enum CpuFeature {
  CC_CPU_NONE             = 0,
  CC_CPU_MMX              = 1,
  CC_CPU_SSE              = 2,
  CC_CPU_SSE2             = 3,
  CC_CPU_SSE3             = 4,
  CC_CPU_SSSE3            = 5,
  CC_CPU_SSE4_1           = 6,
  CC_CPU_SSE4_2           = 7,
  CC_CPU_POPCNT           = 8,
  CC_CPU_FP16             = 9,
  CC_CPU_AVX              = 10,
  CC_CPU_AVX2             = 11,
  CC_CPU_FMA3             = 12,
  CC_CPU_AVX_512F         = 13,
  CC_CPU_AVX_512BW        = 14,
  CC_CPU_AVX_512CD        = 15,
  CC_CPU_AVX_512DQ        = 16,
  CC_CPU_AVX_512ER        = 17,
  CC_CPU_AVX_512IFMA512   = 18,
  CC_CPU_AVX_512PF        = 19,
  CC_CPU_AVX_512VBMI      = 20,
  CC_CPU_AVX_512VL        = 21,
  CC_CPU_NEON             = 100,
  CC_HARDWARE_MAX_FEATURE = 255,
  CC_MAX_FEATURE = CC_HARDWARE_MAX_FEATURE
} CpuFeature;

typedef struct cpu_info_t
{
  int x86_family;
  int l1, l2, l3;
  bool have[CC_MAX_FEATURE+1];
} cpu_info_t;

static cpu_info_t* get_cpu_info();

#define checkHardwareSupport()  cpu_check_support

CC_INLINE bool cpu_check_support(CpuFeature feature) {
  return get_cpu_info()->have[feature];
}

#endif // _CPU_H_

