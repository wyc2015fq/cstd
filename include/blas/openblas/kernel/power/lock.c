
static void __inline blas_lock(volatile BLASULONG *address){

#ifdef __GNUC__

  BLASLONG int ret, val = 1;

  __asm__ __volatile__ (
	   "      .machine \"any\" ;"
	   "0:    lwarx %0,0, %1 ;"
	   "      cmpwi  0,%0,0;"
	   "      bne 1f;"
	   "      stwcx. %2,0, %1 ;"
	   "      bne- 0b;"
	   "1:    "
	: "=&r"(ret)
	: "r"(address), "r" (val)
	: "cr0", "memory");

#else
  while (*address) {};
  *address = 1;
#endif
}
