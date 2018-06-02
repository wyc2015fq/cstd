#include "cstd.h"
int partition(int* array, int left, int right)
{
  int pos;
  int t;
  if (array == NULL) {
    return -1;
  }
  pos = right;
  right--;
  while (left <= right) {
    while (left < pos && array[left] <= array[pos]) {
      left++;
    }
    while (right >= 0 && array[right] > array[pos]) {
      right--;
    }
    if (left >= right) {
      break;
    }
    CV_SWAP(array[left], array[right], t);
  }
  CV_SWAP(array[left], array[pos], t);
  return left;
}
int getMidIndex(int* array, int size)
{
  int left, right;
  int midPos, index;
  if (array == NULL || size <= 0) {
    return -1;
  }
  left = 0;
  right = size - 1;
  midPos = right >> 1;
  index = -1;
  while (index != midPos) {
    index = partition(array, left, right);
    if (index < midPos) {
      left = index + 1;
    }
    else if (index > midPos) {
      right = index - 1;
    }
    else {
      break;
    }
  }
  assert(index == midPos);
  return array[index];
}
//寻找kmin算法如下：
int findKMin(int* array, int size, int k)
{
  int left, right;
  int index;
  if (array == NULL || size <= 0) {
    return -1;
  }
  left = 0;
  right = size - 1;
  index = -1;
  while (index != k) {
    index = partition(array, left, right);
    if (index < k) {
      left = index + 1;
    }
    else if (index > k) {
      right = index - 1;
    }
    else {
      break;
    }
  }
  assert(index == k);
  return array[index];
}
int find_kmin_u8_ii(const int* array, int size, int k, int ishift, int ii, int* pii)
{
  uint cnt[256] = {0};
  int i, n = 0;
  int shift = ishift * 8;
  int shift0 = shift + 8;
  if (3 == ishift) {
    for (i = 0; i < size; ++i) {
      uint aa = array[i];
      cnt[(aa >> shift) & 0xff]++;
    }
  }
  else {
    uint mask = (0xffffffff >> shift0) << shift0;
    for (i = 0; i < size; ++i) {
      uint aa = array[i];
      int tt = aa & mask;
      if (ii == tt) {
        cnt[(aa >> shift) & 0xff]++;
      }
    }
  }
  for (i = 0; i < 256 && n <= k; ++i) {
    n += cnt[i];
  }
  --i;
  k -= (n - cnt[i]);
  pii[ishift] = i;
  return k;
}
// 求k(0序)大的数
int find_kmin1(const int* array, int size, int k)
{
  int i, ii[4] = {0}, out = 0;
  for (i = 4; --i >= 0;) {
    k = find_kmin_u8_ii(array, size, k, i, out, ii);
    out |= (ii[i] << (i * 8));
    if (!k) {
      return out;
    }
  }
  ASSERT(0);
  return out;
}
int function(void)
{
  static int i, state = 0;
  switch (state) {
  case 0: /* start of function */
    for (i = 0; i < 10; i++) {
      state = __LINE__ + 2; /* so we will come back to "case __LINE__" */
      return i;
    case __LINE__:; /* resume control straight after the return */
    }
  }
}
int test_kmin()
{
  int N = 10000000;
  typedef int T;
  struct {
    double time;
    T* x;
  } tt[2];
  T* x = MALLOC(T, (countof(tt) + 1) * N);
  int i, j = 0;
  T* xx1, t;
  for (i = 0; i < countof(tt); ++i) {
    tt[i].time = 0;
    tt[i].x = x + (i + 1) * N;
  }
  for (j = 0; j < 10; ++j) {
    if (1) {
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
      t = find_kmin1(xx, N, N / 2);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(t == (N) / 2);
      xx1 = xx;
    }
    if (1) {
      T* xx = tt[i++].x;
      utime_start(_start_time);
      t = getMidIndex(xx, N);
      printf("%f ", utime_elapsed(_start_time));
      ASSERT(t == (N - 1) / 2);
      xx1 = xx;
    }
    printf("\n");
  }
  FREE(x);
  return 0;
}

