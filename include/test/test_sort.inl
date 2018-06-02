#include "cstd.h"
#include "sort.hpp"
#include "timsort.hpp"
#include "smoothsort.hpp"
//#include "treesort.hpp"
#include <algorithm>
template <typename T>
struct less {
  inline bool operator()(const T& x, const T& y) {
    return x < y;
  }
};
int test_sort()
{
  int N = 1000000;
  typedef double T;
  struct {
    double time;
    T* x;
  } tt[10];
  T* x = MALLOC(T, (countof(tt) + 1) * N);
  int i, j = 0;
  T* xx1;
  using namespace std;
  less<T> lss;
  for (i = 0; i < countof(tt); ++i) {
    tt[i].time = 0;
    tt[i].x = x + (i + 1) * N;
  }
  for (j = 0; j < 10; ++j) {
    if (0) {
      //在数据基本有序时性能对比
      //0.050433 0.019476 0.008354
      //0.019903 0.009157 0.008194
      for (i = 0; i < N; ++i) {
        x[i] = i;
      }
      for (i = 0; i < N; ++i) {
        int t1 = rand() % N;
        int t2 = rand() % N;
        T t;
        CV_SWAP(x[t1], x[t2], t);
      }
    }
    else {
      //0.059928 0.019403 0.031794
      //0.039639 0.019480 0.032582
      for (i = 0; i < N; ++i) {
        x[i] = rand();
      }
    }
    for (i = 0; i < countof(tt); ++i) {
      memcpy(tt[i].x, x, N * sizeof(T));
    }
    i = 0;
    {
      T* xx = tt[i++].x;
      utime_start(_start_time);
      std::sort(xx, xx + N, lss);
      printf("%f ", utime_elapsed(_start_time));
      xx1 = xx;
    }
    {
      T* xx = tt[i++].x;
      utime_start(_start_time);
      quicksort(xx, N, lss);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(0 == memcmp(xx1, xx, N * sizeof(T)));
    }
    {
      T* xx = tt[i++].x;
      utime_start(_start_time);
      timsort(xx, N, lss);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(0 == memcmp(xx1, xx, N * sizeof(T)));
    }
    printf("\n");
  }
  FREE(x);
  return 0;
}

