#include <random>
#include <limits>
#include "cpu.hpp"

void cpu_caffe_memset(const size_t N, const int alpha, void* X)
{
  memset(X, alpha, N);
}
void cpu_caffe_memcpy(const size_t N, const void* X, void* Y) {
  memcpy(Y, X, N);
}

unsigned int caffe_rng_rand()
{
  return (*caffe_rng())();
}

inline bool is_a_ge_zero_and_a_lt_b(int a, int b)
{
  return static_cast<unsigned>(a) < static_cast<unsigned>(b);
}
#define Stype double

#define FUN(NAME)  cpu_##NAME
#define CBLASFUN(NAME)  cblas_s##NAME
#define Dtype float
#include "math_functions_impl.inl"
#undef Dtype
#undef CBLASFUN
#undef FUN
#undef Stype


#define RETURN_TYPE(TF, OP)  if (type==TF) { OP }


#undef _CONTEXT
