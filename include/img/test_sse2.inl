#include <xmmintrin.h>

#include <windows.h>

void ScaleValue1(float* pArray, int dwCount, float fScale)
{
  int i = 0;
  __m128 e_Scale = _mm_set_ps1(fScale);

  for (; i < dwCount; i += 4) {
    *(__m128*)(pArray + i) = _mm_mul_ps(*(__m128*)(pArray + i), e_Scale);
  }
}

void ScaleValue2(float* pArray, int dwCount, float fScale)
{
  int i = 0;

  for (; i < dwCount - 3; i += 4) {
    pArray[i] *= fScale;
    pArray[i + 1] *= fScale;
    pArray[i + 2] *= fScale;
    pArray[i + 3] *= fScale;
  }

  for (; i < dwCount; i++) {
    pArray[i] *= fScale;
  }
}


int test_sse2()
{
  int i;
  enum {ARRAYCOUNT = 10000 };
  float __declspec(align(16)) Array[ARRAYCOUNT];
  memset(Array, 0, sizeof(float) * ARRAYCOUNT);

  {
    utime_start(_start_time);

    for (i = 0; i < 100000; i++) {
      ScaleValue1(Array, ARRAYCOUNT, 1000.0f);
    }

    printf("Use SSE: %f\n", utime_elapsed(_start_time));
  }
  {
    utime_start(_start_time);

    for (i = 0; i < 100000; i++) {
      ScaleValue2(Array, ARRAYCOUNT, 1000.0f);
    }

    printf("Not Use SSE: %f\n", utime_elapsed(_start_time));
  }
  return 0;
}
