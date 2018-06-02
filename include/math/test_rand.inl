
#include "rand.inl"

#ifdef TEST
#include "cstd.h"
#include "cmath.h"
int test_rand()
{
  int low = 0, high = 100, i;
  double mu = 0.0, sigma = 1.0;
  double beta = 2.0;
  double lambda = 4.0;
  double p = 0.5;
  enum {N = 10};
  double rs[N] = {0};
  double tmp[N] = {0};
  double irs[N] = {0};
  double (*mean)(int n, const double* vec) = vec_mean_f64;
  double (*var)(int n, const double* vec) = vec_variance_f64;
  rng_t r[1] = {0};
  rng_mt19937_init(r, 37);
  printf("Uniform distribution: U ~ ( %d, %d )\n", low, high);
  for (i = 0;i < N;++i) {
    printf("%5f ", rng_uniform(r, low, high));
  }
  printf("\n");
  rng_uniform_fill(r, low, high, N, irs);
  printf("mean (theoretical generated): %f %f\n", mean(N, irs), (high - low) / 2.);
  printf("variance (theoretical generated): %f %d\n", var(N, irs), (high - low) * (high - low) / 12);
  printf("Normal distribution: N ~ ( %f, %f )\n", mu, sigma);
  for (i = 0;i < N;++i) {
    printf("%f ", rng_normal(r, mu, sigma));
  }
  printf("\n");
  rng_normal_fill(r, mu, sigma, N, rs);
  printf("mean (theoretical generated): ", mean(N, rs), mu);
  printf("variance (theoretical generated): %f %f \n", var(N, rs), sigma * sigma);
  printf("Exponential distribution: E ~ ( %f )\n", beta);
  for (i = 0;i < N;++i) {
    printf("%f ", rng_exponential(r, beta));
  }
  printf("\n");
  rng_exponential_fill(r, beta, N, rs);
  printf("mean (theoretical generated): %f, %f\n", mean(N, rs), beta);
  printf("variance (theoretical generated): %f %f\n", var(N, rs), beta * beta);
  printf("Rayleigh distribution: R ~ ( %f )", sigma);
  for (i = 0;i < N;++i) {
    printf("%f ", rng_rayleigh(r, sigma));
  }
  printf("\n");
  vec_randr(r, sigma, N, rs);
  printf("mean (theoretical generated): %f %f\n", mean(N, rs), sigma * sqrt(PI / 2.0));
  printf("variance (theoretical generated): %f %f", var(N, rs), (2 - PI / 2)*sigma * sigma);
  printf("Poisson distribution: B ~ ( %f )", p);
  for (i = 0;i < N;++i) {
    printf("%f ", rng_poisson(r, lambda));
  }
  printf("\n");
  rng_poisson_fill(r, lambda, N, irs);
  for (i = 0;i < N;++i) {
    tmp[i] = irs[i];
  }
  printf("mean (theoretical generated): %f %f\n", mean(N, tmp), lambda);
  printf("variance (theoretical generated): %f %f\n", var(N, tmp), lambda);
  printf("Bernoulli distribution: B ~ ( %f )", p);
  for (i = 0;i < N;++i) {
    printf("%f ", rng_bernoulli(r, p));
  }
  printf("\n");
  rng_bernoulli_fill(r, p, N, irs);
  for (i = 0;i < N;++i) {
    tmp[i] = irs[i];
  }
  printf("mean (theoretical generated): %f %f\n", mean(N, tmp), p);
  printf("variance (theoretical generated): %f %f\n", var(N, tmp), p * (1 - p));
  return 0;
}
int test_mt19937ar(void)
{
  int i;
  unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
  rng_t r[1] = {0};
  rng_mt19937_init(r, 10);
  //mt19937ar_init_by_array(r, init, length);
  printf("1000 outputs of rng_int32(r)\n");
  
  for (i = 0; i < 1000; i++) {
    printf("%10lu ", rng_int32(r));
    
    if (i % 5 == 4) {
      printf("\n");
    }
  }
  
  printf("\n1000 outputs of mt19937ar_real2(r)\n");
  
  for (i = 0; i < 1000; i++) {
    printf("%10.8f ", rng_real2(r));
    
    if (i % 5 == 4) {
      printf("\n");
    }
  }
  
  return 0;
}
#endif
