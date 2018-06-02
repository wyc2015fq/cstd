




#if defined(OS_WINDOWS) && (defined(__MINGW32__) || defined(__MINGW64__))
#include <conio.h>
#undef  printf
#define printf	_cprintf
#endif

#ifdef __ELF__
int __xerbla(char *message, blasint *info, blasint length){

  printf(" ** On entry to %6s parameter number %2d had an illegal value\n",
	  message, *info);

  return 0;
}

int BLASFUNC(xerbla)(char *, blasint *, blasint) __attribute__ ((weak, alias ("__xerbla")));

#else

int BLASFUNC(xerbla)(char *message, blasint *info, blasint length){

  printf(" ** On entry to %6s parameter number %2d had an illegal value\n",
	  message, *info);

  return 0;
}

#endif
