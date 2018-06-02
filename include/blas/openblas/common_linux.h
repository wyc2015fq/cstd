
#ifndef COMMON_LINUX_H
#define COMMON_LINUX_H

#ifndef ASSEMBLER

#include <syscall.h>

extern long int syscall (long int __sysno, ...);

#ifndef MPOL_PREFERRED
#define MPOL_PREFERRED 1
#endif

#ifndef MPOL_INTERLEAVE
#define MPOL_INTERLEAVE 3
#endif

#if defined(ARCH_IA64) && defined(__ECC)
#ifndef __NR_mbind
#define __NR_mbind			1259
#endif
#ifndef __NR_get_mempolicy
#define __NR_get_mempolicy		1260
#endif
#ifndef __NR_set_mempolicy
#define __NR_set_mempolicy		1261
#endif
#endif



static inline int my_mbind(void *addr, unsigned long len, int mode,
			   unsigned long *nodemask, unsigned long maxnode,
			   unsigned flags) {
#if defined (__LSB_VERSION__)
// So far,  LSB (Linux Standard Base) don't support syscall().
// https://lsbbugs.linuxfoundation.org/show_bug.cgi?id=3482
        return 0;
#else
#if defined (LOONGSON3B)
#if defined (__64BIT__)
	return syscall(SYS_mbind, addr, len, mode, nodemask, maxnode, flags);
#else
	return 0; //NULL Implementation on Loongson 3B 32bit.
#endif
#else
//Fixed randomly SEGFAULT when nodemask==NULL with above Linux 2.6.34
//	unsigned long null_nodemask=0;
	return syscall(SYS_mbind, addr, len, mode, nodemask, maxnode, flags);
#endif
#endif
}

static inline int my_set_mempolicy(int mode, const unsigned long *addr, unsigned long flag) {
#if defined (__LSB_VERSION__)
// So far,  LSB (Linux Standard Base) don't support syscall().
// https://lsbbugs.linuxfoundation.org/show_bug.cgi?id=3482
  return 0;
#else
  return syscall(SYS_set_mempolicy, mode, addr, flag);
#endif
}

static inline int my_gettid(void) {
#ifdef SYS_gettid
return syscall(SYS_gettid);
#else
return getpid();
#endif
}

#endif
#endif
