
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "wstd/utime.hpp"
using namespace std;

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif


float sum_array(float* arr, int len)
{
  if (NULL == arr || len<1)
  {
    printf("input error\n");
    return 0;
  }
  float sum(0.0);
  for (long i = 0; i<len; ++i)
  {
    sum += *arr++;
  }

  return sum;
}

float sum_array_neon(float* arr, int len)
{
  if (NULL == arr || len < 1)
  {
    printf("input error\n");
    return 0;
  }
  long dim4 = len / 4;
  int left4 = len % 4;
  float32x4_t sum_vec = vdupq_n_f32(0.0);
  for (; dim4 > 0; dim4--, arr += 4)
  {
    float32x4_t data_vec = vld1q_f32(arr);
    sum_vec = vaddq_f32(sum_vec, data_vec);
  }
  float sum = vgetq_lane_f32(sum_vec, 0) + vgetq_lane_f32(sum_vec, 1) + vgetq_lane_f32(sum_vec, 2) + vgetq_lane_f32(sum_vec, 3);
  for (; left4 > 0; left4--, arr++)
  {
    sum += (*arr);
  }

  return sum;

}


int test_neon()
{
  long l = 1000000;
  float* array = (float*)malloc(l*sizeof(float));
  int times = 1000;
  printf("l = %ld\n", l);
  for (long i = 0; i < l; i++)
  {
    array[i] = i/1000.;
  }
  uutime a;
  {
    a.restart();
    for (int round = 0; round < times; round++)
    {
      float Sum = sum_array(array, l);
      //cout<<Sum<<endl;
    }
    clock_t endtime = clock();
    printf("%lf\n", a.elapsed());
  }
  {
    a.restart();
    for (int round = 0; round < times; round++)
    {
      float Sum = sum_array_neon(array, l);
      //cout<<Sum<<endl;
    }
    clock_t endtime = clock();
    printf("%lf\n", a.elapsed());
  }
  free(array);

  return 0;
}
