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

void cpu_caffe_rng_bernoulli_u(const int n, const double p, unsigned int* r)
{
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_GE(p, 0);
  CHECK_LE(p, 1);
  std::bernoulli_distribution random_distribution(p);
  rng_t* rng = caffe_rng();
  for (int i = 0; i < n; ++i) {
    r[i] = static_cast<unsigned int>(random_distribution(*rng));
  }
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