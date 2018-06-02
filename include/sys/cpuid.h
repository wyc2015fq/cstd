#if defined(__i386__) && defined(__PIC__)
/* %ebx may be the PIC register.  */
#if __GNUC__ >= 3
#define __cpuid(level, a, b, c, d)            \
  __asm__ ("xchg{l}\t{%%}ebx, %1\n\t"            \
       "cpuid\n\t"                    \
       "xchg{l}\t{%%}ebx, %1\n\t"            \
       : "=a" (a), "=r" (b), "=c" (c), "=d" (d)    \
       : "0" (level))

#define __cpuid_count(level, count, a, b, c, d)        \
  __asm__ ("xchg{l}\t{%%}ebx, %1\n\t"            \
       "cpuid\n\t"                    \
       "xchg{l}\t{%%}ebx, %1\n\t"            \
       : "=a" (a), "=r" (b), "=c" (c), "=d" (d)    \
       : "0" (level), "2" (count))
#else
/* Host GCCs older than 3.0 weren't supporting Intel asm syntax
   nor alternatives in i386 code.  */
#define __cpuid(level, a, b, c, d)            \
  __asm__ ("xchgl\t%%ebx, %1\n\t"            \
       "cpuid\n\t"                    \
       "xchgl\t%%ebx, %1\n\t"            \
       : "=a" (a), "=r" (b), "=c" (c), "=d" (d)    \
       : "0" (level))

#define __cpuid_count(level, count, a, b, c, d)        \
  __asm__ ("xchgl\t%%ebx, %1\n\t"            \
       "cpuid\n\t"                    \
       "xchgl\t%%ebx, %1\n\t"            \
       : "=a" (a), "=r" (b), "=c" (c), "=d" (d)    \
       : "0" (level), "2" (count))
#endif
#else
#define __cpuid(level, a, b, c, d)            \
  __asm__ ("cpuid\n\t"                    \
       : "=a" (a), "=b" (b), "=c" (c), "=d" (d)    \
       : "0" (level))

#define __cpuid_count(level, count, a, b, c, d)        \
  __asm__ ("cpuid\n\t"                    \
       : "=a" (a), "=b" (b), "=c" (c), "=d" (d)    \
       : "0" (level), "2" (count))
#endif