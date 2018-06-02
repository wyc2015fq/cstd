
#ifndef COMMON_ARM
#define COMMON_ARM

#if defined(ARMV5) || defined(ARMV6)

#define MB
#define WMB

#else

#define MB   __asm__ __volatile__ ("dmb  ish" : : : "memory")
#define WMB  __asm__ __volatile__ ("dmb  ishst" : : : "memory")

#endif

#define INLINE inline

#define RETURN_BY_COMPLEX

#ifndef ASSEMBLER

#if defined(ARMV6) || defined(ARMV7) || defined(ARMV8)

static void __inline blas_lock(volatile BLASULONG *address){

  int register ret;

  do {
    while (*address) {YIELDING;};

    __asm__ __volatile__(
                         "ldrex r2, [%1]      \n\t"
                         "strex %0, %2, [%1]  \n\t"
                         "orr   %0, r2        \n\t"
                         : "=&r"(ret)
                         : "r"(address), "r"(1)
                         : "memory", "r2"
    );

  } while (ret);
  MB;
}

#define BLAS_LOCK_DEFINED
#endif

static inline int blas_quickdivide(blasint x, blasint y){
  return x / y;
}

#if !defined(HAVE_VFP)
/* no FPU, soft float */
#define GET_IMAGE(res)
#elif defined(DOUBLE)
#define GET_IMAGE(res)  __asm__ __volatile__("vstr.f64 d1, %0" : "=m"(res) : : "memory")
#else
#define GET_IMAGE(res)  __asm__ __volatile__("vstr.f32 s1, %0" : "=m"(res) : : "memory")
#endif

#define GET_IMAGE_CANCEL

#endif


#ifndef F_INTERFACE
#define REALNAME ASMNAME
#else
#define REALNAME ASMFNAME
#endif

#if defined(ASSEMBLER) && !defined(NEEDPARAM)

#define PROLOGUE \
	.arm		 ;\
	.global	REALNAME ;\
	.func	REALNAME  ;\
REALNAME:

#define EPILOGUE

#define PROFCODE

#endif


#define SEEK_ADDRESS

#ifndef PAGESIZE
#define PAGESIZE        ( 4 << 10)
#endif
#define HUGE_PAGESIZE   ( 4 << 20)

#define BUFFER_SIZE     (16 << 20)


#define BASE_ADDRESS (START_ADDRESS - BUFFER_SIZE * MAX_CPU_NUMBER)

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

#if !defined(ARMV5) && !defined(ARMV6) && !defined(ARMV7) && !defined(ARMV8)
#error "you must define ARMV5, ARMV6, ARMV7 or ARMV8"
#endif

#endif
