#include "cstd.h"
#include <math.h>
int rand01_()
{
  return rand() % 7 == 0;
}
int rand2()
{
  int b, a;
  for (; 1;) {
    a = rand01_();
    b = rand01_();
    if (b != a) {
      return b > a;
    }
  }
  return 0;
}
int rand4()
{
  return (rand2() << 1) | rand2();
}
int rand8()
{
  return (rand4() << 2) | rand4();
}
int rand_log(int n)
{
  int i, k = 0;
  for (i = 0; i < n; ++i) {
    k = (k << 1) | rand2();
  }
  return k;
}
int randn(int n)
{
  int i, a, k = rand_log(16);
  return k % n;
}
int test_rand01()
{
  int i, j;
  for (j = 0; j < 100; ++j) {
    int tt = 0;
    for (i = 0; i < 1000; ++i) {
      tt += randn(5) == 0;
    }
    printf("%d\n", tt);
  }
  return 0;
}

