#include "popcount.inl"
int test_popcount()
{
  enum {N = 1000000};
  int* ret = MALLOC(int, N);
  //性能测试
#define TEST_POPCOUNT_DEF(func)  {utime_start(_start_time); unsigned int i; for (i=0; i<N; ++i) { ret[i] = func(i); } printf("%s\t: %f\n", #func , utime_elapsed(_start_time));}
  TEST_POPCOUNT_DEF(popcnt_iterated);
  TEST_POPCOUNT_DEF(popcnt_sparse);
  TEST_POPCOUNT_DEF(popcnt_dense);
  TEST_POPCOUNT_DEF(popcnt_lookup);
  TEST_POPCOUNT_DEF(popcnt_parallel);
  TEST_POPCOUNT_DEF(popcnt_nifty);
  TEST_POPCOUNT_DEF(popcnt_hacker);
  TEST_POPCOUNT_DEF(popcnt_hakmem);
#undef TEST_POPCOUNT_DEF
  //正确性测试
#define TEST_POPCOUNT_DEF(func)  {utime_start(_start_time); unsigned int i; for (i=0; i<N; ++i) { ASSERT(ret[i] == func(i)); } printf("%s\t: %f\n", #func , utime_elapsed(_start_time));}
  TEST_POPCOUNT_DEF(popcnt_iterated);
  TEST_POPCOUNT_DEF(popcnt_sparse);
  TEST_POPCOUNT_DEF(popcnt_dense);
  TEST_POPCOUNT_DEF(popcnt_lookup);
  TEST_POPCOUNT_DEF(popcnt_parallel);
  TEST_POPCOUNT_DEF(popcnt_nifty);
  TEST_POPCOUNT_DEF(popcnt_hacker);
  TEST_POPCOUNT_DEF(popcnt_hakmem);
#undef TEST_POPCOUNT_DEF
  FREE(ret);
  return 0;
}

