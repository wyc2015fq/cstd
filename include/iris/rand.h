
#ifndef _RAND_H_
#define _RAND_H_

// y[n+1]=5^13*y[n](mode 2^36) y[0]=1, x[n]=y[n]*2^-36(周期约为2*10^10)
static unsigned __int64 rand_seed_yn = 1;
#define POW_5_13 ((__int64)5*5*5*5*5*5*5*5*5*5*5*5*5)
unsigned __int64 rand_1()
{
  __int64 aaa = POW_5_13;
  rand_seed_yn = (POW_5_13 * rand_seed_yn) % ((unsigned __int64)1 << 36);
  return rand_seed_yn;
}

int test_rand()
{
  int i;

  for (i = 0; i < 1000; ++i) {
    int x = rand_1() % 1000000;
    printf("%d\n", x);
  }

  return 0;
}

#endif // _RAND_H_
