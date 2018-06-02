#include "cstd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int int_end_swap(int x)
{
  int y = ((x >> 24) & 0xff) | (((x >> 16) & 0xff) << 8) | (((x >> 8) & 0xff) << 16) | (((x) & 0xff) << 24);
  return y;
}
int test_int_end_swap()
{
  unsigned int x, y;
  char buf[256];
  while (1) {
    gets(buf);
    if (strlen(buf) <= 0) {
      break;
    }
    sscanf(buf, "%u", &x);
    y = int_end_swap(x);
    printf("%u\n", y);
  }
  return 0;
}
#include "test_mp.inl"
#if 0
//#include "test_var.inl"
#include "math1/Matrix.inl"
#include "math1/Distance.inl"
#include "test_imath.inl"
#include "test_mscript.inl"
#include "test_snak_matrix.inl"
#include "test_group.inl"
#include "test_regexp.inl"
#include "test_integer_division.inl"
#include "test_travase.inl"
#endif
// n分为不超过m个数和的划分数
int test_math()
{
#if 0
  test_int_end_swap();
  test_imath();
  test_snak_matrix();
  test_mscript();
  test_var();
  test_group();
  test_travase();
  test_integer_division();
  test_regexp();
  test_mpz();
#endif
  return 0;
}

