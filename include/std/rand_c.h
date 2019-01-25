
#ifndef _RAND_INL_
#define _RAND_INL_

#include "inttypes_c.h"
#include "types_c.h"
//#include "types_c.h"
//#include "mem_c.h"
//#include "img_c.h"

//#define CC_RAND_UNI      0
//#define CC_RAND_NORMAL   1
///////////////////////////// Functions Declaration //////////////////////////////////////
/*
   Multiply-with-carry generator is used here:
   temp = ( A*X(n) + carry )
   X(n+1) = temp mod (2^32)
   carry = temp / (2^32)
*/
#define  CC_1F              0x3f800000
#define  ICC_RNG_NEXT(x)    ((uint64)(unsigned)(x)*1554115554 + ((x) >> 32))
#define  ICC_CVT_FLT(x)     (((unsigned)(x) >> 9)|CC_1F)
#define  ICC_1D             CC_BIG_INT(0x3FF0000000000000)
#define  ICC_CVT_DBL(x)     (((uint64)(unsigned)(x) << 20)|((x) >> 44)|ICC_1D)

//#define CC_RNG_COEFF 1554115554
#define MYRANDMAX32 (4294967296.)
#define CC_RNG_COEFF (4164903690U)
#define INV_2_32  (2.3283064365386962890625e-10) // 2**-32
#define INV_2_64  (5.4210108624275221700372640043497e-20)   // 2**-64

/* returns random floating-point number between 0 and 1 */
#define RNG_NEXT(x)     (x = ((uint64)(unsigned)(x)*CC_RNG_COEFF + ((x) >> 32)))

//#define cvRandReal(r)  (cvRandInt(r)*2.3283064365386962890625e-10) /* 2^-32 */
//#define cvRNG(seed)   _g_rng=((uint64)(seed ? seed : (int64)-1))
/* returns random 32-bit unsigned integer */
//#define cvRandInt(rng)   (unsigned)( (rng) = (uint64)(unsigned)(rng)*(CC_RNG_COEFF) + ((rng) >> 32) )


#if 0
#define RAND_INIT     srand((int)time(NULL))
#define UNIFORM01()   ((rand() % 32767 + 0.f) / 32767.f)
#define UNIFORM(m)    (rand() % (m))
#define GAUSS()       (UNIFORM01()+UNIFORM01()+UNIFORM01()+UNIFORM01()+UNIFORM01()+UNIFORM01())
#else
#define UNIFORM01(rng)
#endif

typedef struct rng_t_ rng_t;

struct rng_t_ {
  uint32 (*gen_int32)(rng_t* r);
};

// uniform_smallint£ºÔÚÐ¡ÕûÊýÓòÄÚµÄ¾ùÔÈ·Ö²¼
// uniform_int£ºÔÚÕûÊýÓòÉÏµÄ¾ùÔÈ·Ö²¼
// uniform_01£ºÔÚÇø¼ä[0,1]ÉÏµÄÊµÊýÁ¬Ðø¾ùÔÈ·Ö²¼
// uniform_real£ºÔÚÇø¼ä[min,max]ÉÏµÄÊµÊýÁ¬Ðø¾ùÔÈ·Ö²¼
// bernoulli_distribution£º²®Å¬Àû·Ö²¼
// binomial_distribution£º¶þÏî·Ö²¼
// cauchy_distribution£º¿ÂÎ÷£¨ÂåÂ××È£©·Ö²¼
// gamma_distribution£ºÙ¤Âí·Ö²¼
// poisson_distribution£º²´ËÉ·Ö²¼
// geometric_distribution£º¼¸ºÎ·Ö²¼
// triangle_distribution£ºÈý½Ç·Ö²¼
// exponential_distribution£ºÖ¸Êý·Ö²¼
// normal_distribution£ºÕýÌ¬·Ö²¼
// lognormal_distribution£º¶ÔÊýÕýÌ¬·Ö²¼
// uniform_on_sphere£ºÇòÃæ¾ùÔÈ·Ö²¼

// Linear congruential generator
// ÏßÐÔÍ¬ÓàÎ±Ëæ»úÊýÉú³ÉÆ÷
// ·µ»ØÒ»¸öÎÞ·ûºÅ32Î»ÕûÐÍµÄÎ±Ëæ»úÊý
static unsigned int lc_int32(unsigned int* pseed)
{
  unsigned int x;
#if 1
  enum {a = 1103515245, c = 12345};
#else
  enum {a = 134775813, c = 1};
  enum {a = 1664525, c = 1013904223};
  enum {a = 22695477, c = 1};
  enum {a = 214013, c = 2531011};
#endif
  x = *pseed = (*pseed) * a + c;
  //r = (r << 16) | ((r >> 16) & 0xFFFF);
  return x;
}

/* Return random 32-bit unsigned integer: */
CC_INLINE unsigned int lcg64_int32(uint64* r)
{
  (*r) = (uint64)(unsigned) (*r) * CC_RNG_COEFF + ((*r) >> 32);
  return (unsigned) (*r);
}
CC_INLINE double lcg_i32(uint32* r)
{
  return (*r) = (*r) * 1664525 + 1013904223; // Numerical Recipes
}
CC_INLINE double lcg_f32(uint32* r)
{
  suf32_t u;
  (*r) = (*r) * 1664525 + 1013904223; // Numerical Recipes
  u.u = ((*r) >> 9) | 0x3F800000;
  return u.f - 1.0f;
}
CC_INLINE double lcg_f64(uint64* r)
{
  suf64_t u;
  (*r) = (*r) * 2862933555777941757UL + 3037000493UL; // http://nuclear.llnl.gov/CNP/rng/rngman/node4.html
  u.u = ((*r) >> 12) | ((uint64)(0x3FF00000) << 32);
  return u.f - 1;
}
/* Returns random floating-point number between 0 and 1: */
struct lcg64_t {
  rng_t rng[1];
  uint64 x;
};
static uint32 rng_lcg64_gen_int32(rng_t* r)
{
  lcg64_t* r64 = container_of(r, lcg64_t, rng);
  return lcg64_int32(&r64->x);
}

static rng_t* rng_lcg64_init(lcg64_t* r, uint64 x)
{
  r->rng->gen_int32 = rng_lcg64_gen_int32;
  return r->rng;
}

static rng_t* rng_static()
{
  static lcg64_t lcg64[1] = {0};
  if (NULL == lcg64->rng->gen_int32) {
    rng_lcg64_init(lcg64, 100);
  }
  return lcg64->rng;
}
#define RNG_STATIC(r)  r = r ? r : rng_static()
// generates a random number on [0,0x7fffffff]-interval
//#define rng_int32()  (r)->gen_int32(r)
static uint32 rng_int32(rng_t* r)
{
  RNG_STATIC(r);
  return (r)->gen_int32(r);
}

static uint32 rng_int31(rng_t* r)
{
  RNG_STATIC(r);
  return (uint32)(rng_int32(r) >> 1);
}

/* generates a random number on [0,1]-real-interval */
static double rng_real1(rng_t* r)
{
  return rng_int32(r) * (1.0 / 4294967295.0);
  /* divided by 2^32-1 */
}
// [0, 1)
#define rng_real rng_real2
/* generates a random number on [0,1)-real-interval */
static double rng_real2(rng_t* r)
{
  return rng_int32(r) * (1.0 / 4294967296.0);
  /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
static double rng_real3(rng_t* r)
{
  return (((double)rng_int32(r)) + 0.5) * (1.0 / MYRANDMAX32);
  /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
static double rng_res53(rng_t* r)
{
  unsigned long a = rng_int32(r) >> 5, b = rng_int32(r) >> 6;
  return(a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
}

//#include "ziggurat2.inl"

// compute next value
static double rng_normal(rng_t* r, double _Mean, double _Sigma)
{
  // Knuth, vol. 2, p. 122, alg. P
  double _Res, _V1, _V2, _Sx, _Fx;
  RNG_STATIC(r);
  for (; ; ) {
    // reject bad values
    _V1 = 2 * rng_real(r) - 1.0;
    _V2 = 2 * rng_real(r) - 1.0;
    _Sx = _V1 * _V1 + _V2 * _V2;
    if (_Sx < 1.0) {
      break;
    }
  }
  _Fx = sqrt(-2.0 * log(_Sx) / _Sx);
  _Res = _Fx * _V1;
  return (_Res * _Sigma + _Mean);
}

static void rng_normal_boxmuller_fill(rng_t* r, double* data, int count)
{
  int i;
  static const double twopi = (double)(2.0 * 3.14159265358979323846);
  assert(count % 2 == 0);
  for (i = 0; i < count; i += 2) {
    double u1 = 1.0f - rng_real(r); // [0, 1) -> (0, 1]
    double u2 = rng_real(r);
    double radius = sqrt(-2 * log(u1));
    double theta = twopi * u2;
    data[i    ] = radius * cos(theta);
    data[i + 1] = radius * sin(theta);
  }
}

#if 0
static double rng_normal_ziggurat(rng_t* r, double _Mean, double _Sigma)
{
  double _Res = ziggurat(r, r);
  return (_Res * _Sigma + _Mean);
}
#endif

static int rng_uniform(rng_t* r, int low, int high)
{
  int t = high - low;
  uint32 x = rng_int32(r);
  return low + (x%t);
}
// Return an Uniform[low, high] distributed pseudorandom number.
static double rng_uniform(rng_t* r, double low, double high)
{
  double u_0_1 = rng_real3(r);
  return low + (u_0_1 * (high - low));
}

// Return a exponential~(beta) distributed pseudorandom number.
static double rng_exponential(rng_t* r, double beta)
{
  double u_0_1 = rng_real(r);
  return (-beta * log(u_0_1));
}
// Return a rayleigh~(sigma) distributed pseudorandom number.
static double rng_rayleigh(rng_t* r, double sigma)
{
  double u_0_1 = rng_real(r);
  return (sigma * sqrt(-2.0 * log(u_0_1)));
}
// Return a Poisson~(lambda) distributed pseudorandom number.
static int rng_poisson(rng_t* r, double lambda)
{
  double u_0_1 = 0.0, q = exp(-lambda), p = 1.0;
  int k = 0;
  while (p >= q) {
    u_0_1 = rng_real(r);
    p *= u_0_1;
    k++;
  }
  return k - 1;
}
// Return a bernoulli~(p) distributed pseudorandom number.
static int rng_bernoulli(rng_t* r, double p)
{
  double u_0_1 = rng_real(r);
  return ((u_0_1) <= p) ? 1 : 0;
}
// Return an uniform[low, high] distributed pseudorandom sequence of size N.
static int rng_uniform_fill(rng_t* r, double low, double high, int N, double* rs)
{
  double u_0_1 = 0.0;
  int i;
  for (i = 0; i < N; ++i) {
    u_0_1 = rng_real(r);
    rs[i] = low + (u_0_1 * (high - low));
  }
  return 0;
}
// Return an Normal~(mu, sigma) distributed pseudorandom sequence of size N.
static int rng_normal_fill(rng_t* r, double mu, double sigma, int N, double* rs)
{
  int i;
  for (i = 0; i < N; ++i) {
    rs[i] = rng_normal(r, mu, sigma);
  }
  return 0;
}
// Return an Exponential~(beta) distributed pseudorandom sequence of size N.
static int rng_exponential_fill(rng_t* r, double beta, int N, double* rs)
{
  double u_0_1 = 0.0;
  int i;
  for (i = 0; i < N; ++i) {
    u_0_1 = rng_real(r);
    rs[i] = (-beta * log(u_0_1));
  }
  return 0;
}
// Return an Rayleigh~(sigma) distributed pseudorandom sequence of size N.
static int vec_randr(rng_t* r, double sigma, int N, double* rs)
{
  double u_0_1 = 0.0;
  int i;
  for (i = 0; i < N; ++i) {
    u_0_1 = rng_real(r);
    rs[i] = (sigma * sqrt(-2.0 * log(u_0_1)));
  }
  return 0;
}
// Return a Poisson~(lambda) distributed pseudorandom sequence of size N.
static int rng_poisson_fill(rng_t* r, double lambda, int N, double* rs)
{
  double u_0_1 = 0.0, q = exp(-lambda), p = 1.0;
  int k = 0;
  int i;
  for (i = 0; i < N; ++i) {
    k = 0;
    p = 1.0;
    while (p >= q) {
      u_0_1 = rng_real(r);
      p *= u_0_1;
      k++;
    }
    rs[i] = k - 1;
  }
  return 0;
}
// Return a bernoulli~(p) distributed pseudorandom sequence of size N.
static int rng_bernoulli_fill(rng_t* r, double p, int N, double* rs)
{
  //double u_0_1 = 0.0;
  int i;
  unsigned int up = (unsigned int)(p * MYRANDMAX32);
  for (i = 0; i < N; ++i) {
    rs[i] = (rng_int32(r) <= up);
  }
  return 0;
}

typedef int (VEC2I)[2];
typedef float (VEC2F)[2];
typedef double (VEC2D)[2];

///////////////////////////// Functions Declaration //////////////////////////////////////
/*
Multiply-with-carry generator is used here:
temp = ( A*X(n) + carry )
X(n+1) = temp mod (2^32)
carry = temp / (2^32)
*/

/***************************************************************************************\
* Pseudo-Random Number Generators (PRNGs) *
\***************************************************************************************/
typedef struct DivStruct {
  unsigned d;
  unsigned M;
  int sh1, sh2;
  int delta;
} DivStruct;
#define RANDI_IMPL(T)  \
static void randi_##T(T* arr, int len, uint64* state, const void* pv, BOOL fast_int_mode, BOOL small_flag ) {\
  if (fast_int_mode) {\
    const VEC2I* p = (const VEC2I*)pv;\
    uint64 temp = *state;\
    int i;\
    if (!small_flag) {  \
      for (i = 0;i <= len - 4;i += 4) {  \
        int t0, t1;\
        RNG_NEXT(temp);\
        t0 = ((int)temp & p[i][0]) + p[i][1];\
        RNG_NEXT(temp);\
        t1 = ((int)temp & p[i + 1][0]) + p[i + 1][1];\
        arr[i] = (T)(t0);\
        arr[i + 1] = (T)(t1);\
        RNG_NEXT(temp);\
        t0 = ((int)temp & p[i + 2][0]) + p[i + 2][1];\
        RNG_NEXT(temp);\
        t1 = ((int)temp & p[i + 3][0]) + p[i + 3][1];\
        arr[i + 2] = (T)(t0);\
        arr[i + 3] = (T)(t1);\
      }  \
    }  \
    else {  \
      for (i = 0;i <= len - 4;i += 4) {  \
        int t0, t1, t;\
        RNG_NEXT(temp);\
        t = (int)temp;\
        t0 = (t & p[i][0]) + p[i][1];\
        t1 = ((t >> 8) & p[i + 1][0]) + p[i + 1][1];\
        arr[i] = (T)(t0);\
        arr[i + 1] = (T)(t1);\
        t0 = ((t >> 16) & p[i + 2][0]) + p[i + 2][1];\
        t1 = ((t >> 24) & p[i + 3][0]) + p[i + 3][1];\
        arr[i + 2] = (T)(t0);\
        arr[i + 3] = (T)(t1);\
      }  \
    }  \
    for (;i < len;i++) {  \
      int t0;\
      RNG_NEXT(temp);\
      t0 = ((int)temp & p[i][0]) + p[i][1];\
      arr[i] = (T)(t0);\
    }  \
    *state = temp;\
  } else {\
    const DivStruct* p = (const DivStruct*)pv;\
    uint64 temp = *state;\
    int i = 0;\
    unsigned t0, t1, v0, v1;\
    for (i = 0;i <= len - 4;i += 4) {\
      RNG_NEXT(temp);\
      t0 = (unsigned)temp;\
      RNG_NEXT(temp);\
      t1 = (unsigned)temp;\
      v0 = (unsigned)(((uint64)t0 * p[i].M) >> 32);\
      v1 = (unsigned)(((uint64)t1 * p[i + 1].M) >> 32);\
      v0 = (v0 + ((t0 - v0) >> p[i].sh1)) >> p[i].sh2;\
      v1 = (v1 + ((t1 - v1) >> p[i + 1].sh1)) >> p[i + 1].sh2;\
      v0 = t0 - v0 * p[i].d + p[i].delta;\
      v1 = t1 - v1 * p[i + 1].d + p[i + 1].delta;\
      arr[i] = (T)((int)v0);\
      arr[i + 1] = (T)((int)v1);\
      RNG_NEXT(temp);\
      t0 = (unsigned)temp;\
      RNG_NEXT(temp);\
      t1 = (unsigned)temp;\
      v0 = (unsigned)(((uint64)t0 * p[i + 2].M) >> 32);\
      v1 = (unsigned)(((uint64)t1 * p[i + 3].M) >> 32);\
      v0 = (v0 + ((t0 - v0) >> p[i + 2].sh1)) >> p[i + 2].sh2;\
      v1 = (v1 + ((t1 - v1) >> p[i + 3].sh1)) >> p[i + 3].sh2;\
      v0 = t0 - v0 * p[i + 2].d + p[i + 2].delta;\
      v1 = t1 - v1 * p[i + 3].d + p[i + 3].delta;\
      arr[i + 2] = (T)((int)v0);\
      arr[i + 3] = (T)((int)v1);\
    }\
    for (;i < len;i++) {\
      RNG_NEXT(temp);\
      t0 = (unsigned)temp;\
      v0 = (unsigned)(((uint64)t0 * p[i].M) >> 32);\
      v0 = (v0 + ((t0 - v0) >> p[i].sh1)) >> p[i].sh2;\
      v0 = t0 - v0 * p[i].d + p[i].delta;\
      arr[i] = (T)((int)v0);\
    }\
    *state = temp;\
  } \
}

RANDI_IMPL(int8)
RANDI_IMPL(int16)
RANDI_IMPL(int32)
RANDI_IMPL(uint8)
RANDI_IMPL(uint16)
RANDI_IMPL(uint32)
//template <typename T> RANDI_IMPL(T)
#undef RANDI_IMPL


static void randi_T_fast(int* arr, int len, uint64* state, const void* pv, BOOL small_flag )
{
  typedef int T;
  const VEC2I* p = (const VEC2I*)pv;
  uint64 temp = *state;
  int i;
  if (!small_flag) {
    for (i = 0; i <= len - 4; i += 4) {
      int t0, t1;
      RNG_NEXT(temp);
      t0 = ((int)temp & p[i][0]) + p[i][1];
      RNG_NEXT(temp);
      t1 = ((int)temp & p[i + 1][0]) + p[i + 1][1];
      arr[i] = (T)(t0);
      arr[i + 1] = (T)(t1);
      RNG_NEXT(temp);
      t0 = ((int)temp & p[i + 2][0]) + p[i + 2][1];
      RNG_NEXT(temp);
      t1 = ((int)temp & p[i + 3][0]) + p[i + 3][1];
      arr[i + 2] = (T)(t0);
      arr[i + 3] = (T)(t1);
    }
  } else {
    for (i = 0; i <= len - 4; i += 4) {
      int t0, t1, t;
      RNG_NEXT(temp);
      t = (int)temp;
      t0 = (t & p[i][0]) + p[i][1];
      t1 = ((t >> 8) & p[i + 1][0]) + p[i + 1][1];
      arr[i] = (T)(t0);
      arr[i + 1] = (T)(t1);
      t0 = ((t >> 16) & p[i + 2][0]) + p[i + 2][1];
      t1 = ((t >> 24) & p[i + 3][0]) + p[i + 3][1];
      arr[i + 2] = (T)(t0);
      arr[i + 3] = (T)(t1);
    }
  }
  for (; i < len; i++) {
    int t0;
    RNG_NEXT(temp);
    t0 = ((int)temp & p[i][0]) + p[i][1];
    arr[i] = (T)(t0);
  }
  *state = temp;
  return ;
}

static void randi_T(int* arr, int len, uint64* state, const void* pv, BOOL small_flag )
{
  typedef int T;
  const DivStruct* p = (const DivStruct*)pv;
  uint64 temp = *state;
  int i = 0;
  unsigned t0, t1, v0, v1;
  for (i = 0; i <= len - 4; i += 4) {
    RNG_NEXT(temp);
    t0 = (unsigned)temp;
    RNG_NEXT(temp);
    t1 = (unsigned)temp;
    v0 = (unsigned)(((uint64)t0 * p[i].M) >> 32);
    v1 = (unsigned)(((uint64)t1 * p[i + 1].M) >> 32);
    v0 = (v0 + ((t0 - v0) >> p[i].sh1)) >> p[i].sh2;
    v1 = (v1 + ((t1 - v1) >> p[i + 1].sh1)) >> p[i + 1].sh2;
    v0 = t0 - v0 * p[i].d + p[i].delta;
    v1 = t1 - v1 * p[i + 1].d + p[i + 1].delta;
    arr[i] = (T)((int)v0);
    arr[i + 1] = (T)((int)v1);
    RNG_NEXT(temp);
    t0 = (unsigned)temp;
    RNG_NEXT(temp);
    t1 = (unsigned)temp;
    v0 = (unsigned)(((uint64)t0 * p[i + 2].M) >> 32);
    v1 = (unsigned)(((uint64)t1 * p[i + 3].M) >> 32);
    v0 = (v0 + ((t0 - v0) >> p[i + 2].sh1)) >> p[i + 2].sh2;
    v1 = (v1 + ((t1 - v1) >> p[i + 3].sh1)) >> p[i + 3].sh2;
    v0 = t0 - v0 * p[i + 2].d + p[i + 2].delta;
    v1 = t1 - v1 * p[i + 3].d + p[i + 3].delta;
    arr[i + 2] = (T)((int)v0);
    arr[i + 3] = (T)((int)v1);
  }
  for (; i < len; i++) {
    RNG_NEXT(temp);
    t0 = (unsigned)temp;
    v0 = (unsigned)(((uint64)t0 * p[i].M) >> 32);
    v0 = (v0 + ((t0 - v0) >> p[i].sh1)) >> p[i].sh2;
    v0 = t0 - v0 * p[i].d + p[i].delta;
    arr[i] = (T)((int)v0);
  }
  *state = temp;
  return ;
}

static void randf_32f(float* arr, int len, uint64* state, const VEC2F* p)
{
  uint64 temp = *state;
  int i = 0;
  for (; i <= len - 4; i += 4) {
    float f[4];
    f[0] = (float)(int)(RNG_NEXT(temp));
    f[1] = (float)(int)(RNG_NEXT(temp));
    f[2] = (float)(int)(RNG_NEXT(temp));
    f[3] = (float)(int)(RNG_NEXT(temp));
    // handwritten SSE is required not for performance but for numerical stability!
    // both 32-bit gcc and MSVC compilers trend to generate double precision SSE
    // while 64-bit compilers generate single precision SIMD instructions
    // so manual vectorisation forces all compilers to the single precision
    arr[i + 0] = f[0] * p[i + 0][0] + p[i + 0][1];
    arr[i + 1] = f[1] * p[i + 1][0] + p[i + 1][1];
    arr[i + 2] = f[2] * p[i + 2][0] + p[i + 2][1];
    arr[i + 3] = f[3] * p[i + 3][0] + p[i + 3][1];
  }
  for (; i < len; i++) {
    RNG_NEXT(temp);
    arr[i] = (int)temp * p[i][0] + p[i][1];
  }
  *state = temp;
}
static void randf_64f(double* arr, int len, uint64* state, const VEC2D* p)
{
  uint64 temp = *state;
  int64 v = 0;
  int i;
  for (i = 0; i <= len - 4; i += 4) {
    double f0, f1;
    RNG_NEXT(temp);
    v = (temp >> 32) | (temp << 32);
    f0 = v * p[i][0] + p[i][1];
    RNG_NEXT(temp);
    v = (temp >> 32) | (temp << 32);
    f1 = v * p[i + 1][0] + p[i + 1][1];
    arr[i] = f0;
    arr[i + 1] = f1;
    RNG_NEXT(temp);
    v = (temp >> 32) | (temp << 32);
    f0 = v * p[i + 2][0] + p[i + 2][1];
    RNG_NEXT(temp);
    v = (temp >> 32) | (temp << 32);
    f1 = v * p[i + 3][0] + p[i + 3][1];
    arr[i + 2] = f0;
    arr[i + 3] = f1;
  }
  for (; i < len; i++) {
    RNG_NEXT(temp);
    v = (temp >> 32) | (temp << 32);
    arr[i] = v * p[i][0] + p[i][1];
  }
  *state = temp;
}

// The code below implements the algorithm described in
// "The Ziggurat Method for Generating Random Variables"
// by Marsaglia and Tsang, Journal of Statistical Software.
static void randn_0_1_32f(float* arr, int len, uint64* state)
{
  const float r = 3.442620f;// The start of the right tail
  const float rng_flt = 2.3283064365386962890625e-10f;// 2^-32
  static unsigned kn[128];
  static float wn[128], fn[128];
  uint64 temp = *state;
  static bool initialized = false;
  int i;
  if (!initialized) {
    const double m1 = 2147483648.0;
    double dn = 3.442619855899, tn = dn, vn = 9.91256303526217e-3;
    // Set up the tables
    double q = vn / exp(-.5 * dn * dn);
    kn[0] = (unsigned)((dn / q) * m1);
    kn[1] = 0;
    wn[0] = (float)(q / m1);
    wn[127] = (float)(dn / m1);
    fn[0] = 1.f;
    fn[127] = (float)exp(-.5 * dn * dn);
    for (i = 126; i >= 1; i--) {
      dn = sqrt(-2.*log(vn / dn + exp(-.5 * dn * dn)));
      kn[i + 1] = (unsigned)((dn / tn) * m1);
      tn = dn;
      fn[i] = (float)exp(-.5 * dn * dn);
      wn[i] = (float)(dn / m1);
    }
    initialized = true;
  }
  for (i = 0; i < len; i++) {
    float x, y;
    for (;;) {
      int iz, hz = (int)temp;
      RNG_NEXT(temp);
      iz = hz & 127;
      x = hz * wn[iz];
      if ((unsigned)abs(hz) < kn[iz]) {
        break;
      }
      if (iz == 0) { // iz==0, handles the base strip
        do {
          x = (unsigned)temp * rng_flt;
          RNG_NEXT(temp);
          y = (unsigned)temp * rng_flt;
          RNG_NEXT(temp);
          x = (float)(-log(x + FLT_MIN) * 0.2904764);
          y = (float) - log(y + FLT_MIN);
        } // .2904764 is 1/r
        while (y + y < x * x);
        x = hz > 0 ? r + x : -r - x;
        break;
      }
      // iz > 0, handle the wedges of other strips
      y = (unsigned)temp * rng_flt;
      RNG_NEXT(temp);
      if (fn[iz] + y * (fn[iz - 1] - fn[iz]) < exp(-.5 * x * x)) {
        break;
      }
    }
    arr[i] = x;
  }
  *state = temp;
}
static double gaussian(double sigma, uint64* state)
{
  float temp;
  randn_0_1_32f(&temp, 1, state);
  return temp * sigma;
}
#define RANDNSCALE_IMPL(T) \
static void randnScale_##T(const float* src, T* dst, int len, int cn, const double* mean, const double* stddev, bool stdmtx)\
{\
  int i, j, k;\
  if (!stdmtx) {\
    if (cn == 1) {\
      double b = mean[0], a = stddev[0];\
      for (i = 0;i < len;i++) {\
        dst[i] = (T)(src[i] * a + b);\
      }\
    }\
    else {\
      for (i = 0;i < len;i++, src += cn, dst += cn) {\
        for (k = 0;k < cn;k++) {\
          dst[k] = (T)(src[k] * stddev[k] + mean[k]);\
        }\
      }\
    }\
  }\
  else {\
    for (i = 0;i < len;i++, src += cn, dst += cn) {\
      for (j = 0;j < cn;j++) {\
        double s = mean[j];\
        for (k = 0;k < cn;k++) {\
          s += src[k] * stddev[j * cn + k];\
        }\
        dst[j] = (T)(s);\
      }\
    }\
  }\
}

#if 0
RANDNSCALE_IMPL(int8)
RANDNSCALE_IMPL(int16)
RANDNSCALE_IMPL(int32)
RANDNSCALE_IMPL(uint8)
RANDNSCALE_IMPL(uint16)
RANDNSCALE_IMPL(uint32)
RANDNSCALE_IMPL(float)
RANDNSCALE_IMPL(double)
#endif
//template <typename T> RANDNSCALE_IMPL(T)
#undef RANDNSCALE_IMPL

static void randnScale_flaot(const float* src, float* dst, int len, int cn, const double* mean, const double* stddev, BOOL stdmtx)
{
  typedef float T;
  int i, j, k;
  if (!stdmtx) {
    if (cn == 1) {
      double b = mean[0], a = stddev[0];
      for (i = 0; i < len; i++) {
        dst[i] = (T)(src[i] * a + b);
      }
    } else {
      for (i = 0; i < len; i++, src += cn, dst += cn) {
        for (k = 0; k < cn; k++) {
          dst[k] = (T)(src[k] * stddev[k] + mean[k]);
        }
      }
    }
  } else {
    for (i = 0; i < len; i++, src += cn, dst += cn) {
      for (j = 0; j < cn; j++) {
        double s = mean[j];
        for (k = 0; k < cn; k++) {
          s += src[k] * stddev[j * cn + k];
        }
        dst[j] = (T)(s);
      }
    }
  }
}
static void randnScale_double(const float* src, double* dst, int len, int cn, const double* mean, const double* stddev, BOOL stdmtx)
{
  typedef double T;
  int i, j, k;
  if (!stdmtx) {
    if (cn == 1) {
      double b = mean[0], a = stddev[0];
      for (i = 0; i < len; i++) {
        dst[i] = (T)(src[i] * a + b);
      }
    } else {
      for (i = 0; i < len; i++, src += cn, dst += cn) {
        for (k = 0; k < cn; k++) {
          dst[k] = (T)(src[k] * stddev[k] + mean[k]);
        }
      }
    }
  } else {
    for (i = 0; i < len; i++, src += cn, dst += cn) {
      for (j = 0; j < cn; j++) {
        double s = mean[j];
        for (k = 0; k < cn; k++) {
          s += src[k] * stddev[j * cn + k];
        }
        dst[j] = (T)(s);
      }
    }
  }
}
#define RNG_FILL_IMPL(T, intmin, intmax)
RNG_FILL_IMPL(int8, -128., 128.)
RNG_FILL_IMPL(int16, -32768., 32768.)
RNG_FILL_IMPL(int32, (double)INT_MIN, (double)INT_MAX)
RNG_FILL_IMPL(uint8, 0., 256.)
RNG_FILL_IMPL(uint16, 0., 65536.)
RNG_FILL_IMPL(uint32, 0, _UI32_MAX)
#undef RNG_FILL_IMPL
#define BLOCK_SIZE 1024

#if 0
template <typename T, int intmin, int intmax>
static void RNG_fill_unifrom(int total, T* arr, int cn, const double* p1, const double* p2, int n, uint64* state, bool saturateRange)
{
  uchar _buf[BLOCK_SIZE];
  int j, fast_int_mode = 0, smallFlag = 1, blockSize = 0;
  VEC2I* ip = 0, *ip2 = 0;
  void* param = NULL;
  BUFUSEBEGIN(_buf, BLOCK_SIZE);
  BUFMALLOC2(ip, n);
  for (j = 0, fast_int_mode = 1; j < n; j++) {
    double a = MIN(p1[j], p2[j]);
    double b = MAX(p1[j], p2[j]);
    if (saturateRange) {
      a = MAX(a, intmin);
      b = MIN(b, intmax);
    }
    ip[j][1] = cvCeil(a);
    int idiff = ip[j][0] = cvFloor(b) - ip[j][1] - 1;
    double diff = b - a;
    fast_int_mode &= diff <= 4294967296. && (idiff & (idiff + 1)) == 0;
    if (fast_int_mode) {
      smallFlag &= idiff <= 255;
    } else {
      if (diff > INT_MAX) {
        ip[j][0] = INT_MAX;
      }
      if (a < INT_MIN / 2) {
        ip[j][1] = INT_MIN / 2;
      }
    }
  }
  if (!fast_int_mode) {
    DivStruct* ds = 0;
    blockSize = MIN(GETBUFLEN() / (cn * sizeof(*ds)), total);
    BUFMALLOC2(ds, cn * blockSize);
    for (j = 0; j < n; ++j) {
      ds[j].delta = ip[j][1];
      unsigned d = ds[j].d = (unsigned)(ip[j][0] + 1);
      int l = 0;
      while (((uint64)1 << l) < d) {
        l++;
      }
      ds[j].M = (unsigned)(((uint64)1 << 32) * (((uint64)1 << l) - d) / d) + 1;
      ds[j].sh1 = min(l, 1);
      ds[j].sh2 = max(l - 1, 0);
    }
    for (j = n; j < cn; ++j) {
      ds[j] = ds[j - n];
    }
    for (j = cn; j < cn * blockSize; ++j) {
      ds[j] = ds[j - cn];
    }
    param = ds;
  } else {
    blockSize = MIN(GETBUFLEN() / (cn * sizeof(*ip)), total);
    BUFMALLOC2(ip2, cn * blockSize);
    MEMCPY(ip2, ip, n);
    for (j = n; j < cn; ++j) {
      ip2[j][0] = ip2[j - n][0];
      ip2[j][1] = ip2[j - n][1];
    }
    for (j = cn; j < cn * blockSize; ++j) {
      ip2[j][0] = ip2[j - cn][0];
      ip2[j][1] = ip2[j - cn][1];
    }
    param = ip2;
  }
  for (j = 0; j < total; j += blockSize) {
    int len = MIN(total - j, blockSize);
    randi_T(arr, len * cn, state, param, fast_int_mode, smallFlag != 0);
    arr += len;
  }
  BUFUSEEND();
  return ;
}
#endif
static void RNG_fill_unifrom_float(int total, float* arr, int cn, const double* p1, const double* p2, int n, uint64* state)
{
  VEC2F* ip2 = 0;
  int j, blockSize;
  uchar _buf[BLOCK_SIZE];
  BUFUSEBEGIN(_buf, BLOCK_SIZE);
  blockSize = MIN((int)(GETBUFLEN() / (cn * sizeof(*ip2))), total);
  BUFMALLOC2(ip2, cn * blockSize);
  for (j = 0; j < n; j++) {
    ip2[j][0] = (float)(MIN(FLT_MAX, p2[j] - p1[j]) * INV_2_32);
    ip2[j][1] = (float)((p2[j] + p1[j]) * 0.5);
  }
  for (j = n; j < cn; ++j) {
    ip2[j][0] = ip2[j - n][0];
    ip2[j][1] = ip2[j - n][1];
  }
  for (j = cn; j < cn * blockSize; ++j) {
    ip2[j][0] = ip2[j - cn][0];
    ip2[j][1] = ip2[j - cn][1];
  }
  for (j = 0; j < total; j += blockSize) {
    int len = MIN(total - j, blockSize);
    randf_32f(arr, len * cn, state, ip2);
    arr += len;
  }
  BUFUSEEND();
  return ;
}
static void RNG_fill_unifrom_double(int total, double* arr, int cn, const double* p1, const double* p2, int n, uint64* state)
{
  VEC2D* ip2 = 0;
  int j, blockSize;
  uchar _buf[BLOCK_SIZE];
  BUFUSEBEGIN(_buf, BLOCK_SIZE);
  blockSize = MIN((int)(GETBUFLEN() / (cn * sizeof(*ip2))), total);
  BUFMALLOC2(ip2, cn * blockSize);
  for (j = 0; j < cn; j++) {
    ip2[j][0] = MIN(DBL_MAX, p2[j] - p1[j]) * INV_2_64;
    ip2[j][1] = ((p2[j] + p1[j]) * 0.5);
  }
  for (j = n; j < cn; ++j) {
    ip2[j][0] = ip2[j - n][0];
    ip2[j][1] = ip2[j - n][1];
  }
  for (j = cn; j < cn * blockSize; ++j) {
    ip2[j][0] = ip2[j - cn][0];
    ip2[j][1] = ip2[j - cn][1];
  }
  for (j = 0; j < total; j += blockSize) {
    int len = MIN(total - j, blockSize);
    randf_64f(arr, len * cn, state, ip2);
    arr += len;
  }
  BUFUSEEND();
  return ;
}

static void RNG_fill_normal_float(int total, float* arr, int cn, const double* mean, const double* stddev, int n, uint64* state, BOOL stdmtx)
{
  uchar _buf[BLOCK_SIZE];
  int j, blockSize;
  float* nbuf = 0;
  BUFUSEBEGIN(_buf, BLOCK_SIZE);
  blockSize = cn * MIN((int)(GETBUFLEN() / (cn * sizeof(float))), total);
  BUFMALLOC2(nbuf, blockSize);
  for (j = 0; j < total; j += blockSize) {
    int len = MIN(total - j, blockSize);
    randn_0_1_32f(nbuf, len * cn, state);
    randnScale_flaot(nbuf, arr, len, cn, mean, stddev, stdmtx);
    arr += len;
  }
  BUFUSEEND();
  return ;
}

static void RNG_fill_normal_double(int total, double* arr, int cn, const double* mean, const double* stddev, int n, uint64* state, BOOL stdmtx)
{
  uchar _buf[BLOCK_SIZE];
  int j, blockSize;
  float* nbuf = 0;
  BUFUSEBEGIN(_buf, BLOCK_SIZE);
  blockSize = cn * MIN((int)(GETBUFLEN() / (cn * sizeof(float))), total);
  BUFMALLOC2(nbuf, blockSize);
  for (j = 0; j < total; j += blockSize) {
    int len = MIN(total - j, blockSize);
    randn_0_1_32f(nbuf, len * cn, state);
    randnScale_double(nbuf, arr, len, cn, mean, stddev, stdmtx);
    arr += len;
  }
  BUFUSEEND();
  return ;
}

#if 0
template <typename T, int intmin, int intmax>
static void RNG_fill(T* _mat, int disttype, const double* _param1, int n1, const double* _param2, int n2, bool saturateRange, int cn)
{
  enum {BLOCK_SIZE = 1024};
  uchar _parambuf[BLOCK_SIZE];
  int j, k, fast_int_mode = 0, smallFlag = 1;
  VEC2I* ip = 0;
  VEC2D* dp = 0;
  VEC2F* fp = 0;
  DivStruct* ds = 0;
  uchar* mean = 0;
  uchar* stddev = 0;
  bool stdmtx = false;
  int depth = 0;
  BUFUSEBEGIN(_parambuf, BLOCK_SIZE);
  if (disttype == CC_RAND_UNI) {
    const double* p1 = _param1;
    const double* p2 = _param2;
    if ( n1 < cn ) {
      double* p1a = NULL;
      BUFMALLOC2(p1a, cn);
      MEMCPY(p1a, p1, n1);
      p1 = p1a;
      for ( j = n1; j < cn; j++ ) {
        p1a[j] = p1a[j - n1];
      }
    }
    if ( n2 < cn ) {
      double* p2a = NULL;
      BUFMALLOC2(p2a, cn);
      MEMCPY(p2a, p2, n2);
      p2 = p2a;
      for ( j = n2; j < cn; j++ ) {
        p2a[j] = p2a[j - n2];
      }
    }
    if (depth <= 1) {
      func = randTab[fast_int_mode];
    } else {
      func = randTab[0][depth];
    }
    CC_Assert(func != 0);
  } else if (disttype == CC_RAND_NORMAL) {
    _parambuf.allocate(MAX(n1, cn) + MAX(n2, cn));
    double* parambuf = _parambuf;
    int ptype = depth == CC_64F ? CC_64F : CC_32F;
    int esz = (int)CC_ELEM_SIZE(ptype);
    if (_param1.isContinuous() && _param1.type() == ptype) {
      mean = _param1_ptr;
    } else {
      Mat tmp(_param1.size(), ptype, parambuf);
      _param1.convertTo(tmp, ptype);
      mean = (uchar*)parambuf;
    }
    if (n1 < cn) {
      for (j = n1 * esz; j < cn * esz; j++) {
        mean[j] = mean[j - n1 * esz];
      }
    }
    if (_param2.isContinuous() && _param2.type() == ptype) {
      stddev = _param2_ptr;
    } else {
      Mat tmp(_param2.size(), ptype, parambuf + cn);
      _param2.convertTo(tmp, ptype);
      stddev = (uchar*)(parambuf + cn);
    }
    if (n2 < cn) {
      for (j = n1 * esz; j < cn * esz; j++) {
        stddev[j] = stddev[j - n1 * esz];
      }
    }
    stdmtx = _param2_rows == cn && _param2_cols == cn;
  } else {
    CC_Error(CC_StsBadArg, "Unknown distribution type");
  }
  uchar* ptr;
  int total = (int)it.size, blockSize = MIN((BLOCK_SIZE + cn - 1) / cn, total);
  size_t esz = mat.elemSize();
  uchar* param = 0;
  float* nbuf = 0;
  if (disttype == UNIFORM) {
    buf.allocate(blockSize * cn * 4);
    param = (uchar*)(double*)buf;
    if (ip) {
      if (ds) {
        DivStruct* p = (DivStruct*)param;
        for (j = 0; j < blockSize * cn; j += cn) {
          for (k = 0; k < cn; k++) {
            p[j + k] = ds[k];
          }
        }
      } else {
        VEC2I* p = (VEC2I*)param;
        for (j = 0; j < blockSize * cn; j += cn) {
          for (k = 0; k < cn; k++) {
            p[j + k] = ip[k];
          }
        }
      }
    } else if (fp) {
      VEC2F* p = (VEC2F*)param;
      for (j = 0; j < blockSize * cn; j += cn) {
        for (k = 0; k < cn; k++) {
          p[j + k] = fp[k];
        }
      }
    } else {
      VEC2D* p = (VEC2D*)param;
      for (j = 0; j < blockSize * cn; j += cn) {
        for (k = 0; k < cn; k++) {
          p[j + k] = dp[k];
        }
      }
    }
  } else {
    buf.allocate((blockSize * cn + 1) / 2);
    nbuf = (float*)(double*)buf;
  }
  for (size_t i = 0; i < nplanes; i++) {
    for (j = 0; j < total; j += blockSize) {
      int len = MIN(total - j, blockSize);
      if (disttype == CC_RAND_UNI) {
        func(ptr, len * cn, &state, param, fast_int_mode, smallFlag != 0);
      } else {
        randn_0_1_32f(nbuf, len * cn, &state);
        scaleFunc(nbuf, ptr, len, cn, mean, stddev, stdmtx);
      }
      ptr += len * esz;
    }
  }
  BUFUSEEND();
}
#endif

static void randShuffle(int _arr_rows, int _arr_cols, uchar* _arr_ptr, int _arr_step, int cn, uint64* r, double iterFactor)
{
  int i, sz = _arr_rows * _arr_cols, iters = (int)(iterFactor * sz + 0.5);
  typedef int arr_type;
  uint64 rng = *r;
  arr_type t;
  if ((_arr_cols * cn) == _arr_step) {
    arr_type* arr = (arr_type*)_arr_ptr;
    for (i = 0; i < iters; i++) {
      int j = (unsigned)RNG_NEXT(rng) % sz, k = (unsigned)RNG_NEXT(rng) % sz;
      CC_SWAP(arr[j], arr[k], t);
    }
  } else {
    uchar* data = _arr_ptr;
    size_t step = _arr_step;
    int cols = _arr_cols;
    for (i = 0; i < iters; i++) {
      int j1 = (unsigned)RNG_NEXT(rng) % sz, k1 = (unsigned)RNG_NEXT(rng) % sz;
      int j0 = j1 / cols, k0 = k1 / cols;
      j1 -= j0 * cols;
      k1 -= k0 * cols;
      CC_SWAP(((arr_type*)(data + step * j0))[j1], ((arr_type*)(data + step * k0))[k1], t);
    }
  }
  *r = rng;
}
/***************************************************************************************\
ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é²¢ï¿½ï¿½ï¿½ï¿½ RNG ×´Ì¬
_g_rng ï¿½ï¿½ cvRNG ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½ï¿½ RNG ×´Ì¬.
arr ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿?
dist_type ï¿½Ö²ï¿½ï¿½ï¿½ï¿½ï¿½:
CC_RAND_UNI - ï¿½ï¿½ï¿½È·Ö²ï¿½
CC_RAND_NORMAL - ï¿½ï¿½Ì¬ï¿½Ö²ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ë¹ï¿½Ö²ï¿½
param1 ï¿½Ö²ï¿½ï¿½Äµï¿½Ò»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç¾ï¿½ï¿½È·Ö²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Î§ï¿½Ä±ï¿½ï¿½Â±ß½ç¡£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì¬ï¿½Ö²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ½ï¿½ï¿½Öµï¿½ï¿½
param2 ï¿½Ö²ï¿½ï¿½ÄµÚ¶ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç¾ï¿½ï¿½È·Ö²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Î§ï¿½Ä¿ï¿½ï¿½Ï±ß½ç¡£ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ì¬ï¿½Ö²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä±ï¿½×¼ï¿½î¡£
ï¿½ï¿½ï¿½ï¿½ cvRandArr ï¿½Ã¾ï¿½ï¿½È·Ö²ï¿½ï¿½Ä»ï¿½ï¿½ï¿½ï¿½ï¿½Ì¬ï¿½Ö²ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½é¡?
ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð¸Ãºï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ò»Ð©ï¿½ï¿½Ì¬ï¿½Ö²ï¿½ï¿½Ä¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Î¬ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Î»ï¿½Ã¡ï¿½
\***************************************************************************************/
// uint64* state
// const int* param
#define _RAND_BITS(_height, _width, arr, step, param1, param2, state, worktype)\
  do {                                                                          \
    uint64 temp = state;                                                         \
    int size_height = _height, size_width = _width, larr;                       \
    int _p1 = (int)param1, _p2 = (int)(param2 - param1 - 1);                    \
    for( larr=0; size_height--; larr += step ) {                                \
      int i = 0, k = 3;                                                         \
      if( _p2 > 255 ) {                                                         \
        for( ; i <= size_width - 4; i += 4 ) {                                  \
          temp = ICC_RNG_NEXT(temp);                                            \
          arr[larr+i  ] = (worktype)((int)(((unsigned)temp & _p2) + _p1));      \
          temp = ICC_RNG_NEXT(temp);                                            \
          arr[larr+i+1] = (worktype)((int)(((unsigned)temp & _p2) + _p1));      \
          temp = ICC_RNG_NEXT(temp);                                            \
          arr[larr+i+2] = (worktype)((int)(((unsigned)temp & _p2) + _p1));      \
          temp = ICC_RNG_NEXT(temp);                                            \
          arr[larr+i+3] = (worktype)((int)(((unsigned)temp & _p2) + _p1));      \
          if( !--k ) {                                                          \
            k = 3;                                                              \
          }                                                                     \
        }                                                                       \
      } else {                                                                  \
        for( ; i <= size_width - 4; i += 4 ) {                                  \
          temp = ICC_RNG_NEXT(temp);                                            \
          arr[i  ] = (worktype)((int)(((unsigned)(temp      ) & _p2) + _p1));   \
          arr[i+1] = (worktype)((int)(((unsigned)(temp >> 8 ) & _p2) + _p1));   \
          arr[i+2] = (worktype)((int)(((unsigned)(temp >> 16) & _p2) + _p1));   \
          arr[i+3] = (worktype)((int)(((unsigned)(temp >> 24) & _p2) + _p1));   \
          if( !--k ) {                                                          \
            k = 3;                                                              \
          }                                                                     \
        }                                                                       \
      }                                                                         \
      for( ; i < size_width; i++ ) {                                            \
        temp = ICC_RNG_NEXT(temp);                                              \
        arr[larr+i] = (worktype)((int)(((unsigned)temp & _p2) + _p1));          \
      }                                                                         \
    }                                                                           \
    state = temp;                                                               \
  } while(0)
// uint64* state
// const double* param
#define _RAND(_height, _width, arr, step, param1, param2, state, worktype)    \
  do {                                                                          \
    uint64 temp = state;                                                         \
    int size_height = _height, size_width = _width, larr;                       \
    worktype _p1 = param1 - (param2 - param1), _p2 = param2 - param1;           \
    for( larr=0; size_height--; larr += step ) {                                \
      int i = 0, k = 3;                                                         \
      for( ; i <= size_width - 4; i += 4 ) {                                    \
        suf32_t t0, t1;                                                         \
        temp = ICC_RNG_NEXT(temp);                                              \
        t0.u = ICC_CVT_FLT(temp);                                               \
        temp = ICC_RNG_NEXT(temp);                                              \
        t1.u = ICC_CVT_FLT(temp);                                               \
        arr[larr+i  ] = (worktype)(t0.f * _p2 + _p1);                           \
        arr[larr+i+1] = (worktype)(t1.f * _p2 + _p1);                           \
        temp = ICC_RNG_NEXT(temp);                                              \
        t0.u = ICC_CVT_FLT(temp);                                               \
        temp = ICC_RNG_NEXT(temp);                                              \
        t1.u = ICC_CVT_FLT(temp);                                               \
        arr[larr+i+2] = (worktype)(t0.f * _p2 + _p1);                           \
        arr[larr+i+3] = (worktype)(t1.f * _p2 + _p1);                           \
        if( !--k ) {                                                            \
          k = 3;                                                                \
        }                                                                       \
      }                                                                         \
      for( ; i < size_width; i++ ) {                                            \
        suf32_t t0;                                                             \
        temp = ICC_RNG_NEXT(temp);                                              \
        t0.u = ICC_CVT_FLT(temp);                                               \
        arr[larr+i] = (worktype)(t0.f * _p2 + _p1);                             \
      }                                                                         \
    }                                                                           \
    state = temp;                                                               \
  } while(0)

// The code below implements algorithm from the paper:
// G. Marsaglia and W.W. Tsang,
// The Monty Python method for generating random variables,
// ACM Transactions on Mathematical Software, Vol. 24, No. 3,
// Pages 341-350, September, 1998.
static int icvRandn_0_1_32f_C1R_(float* arr, int len, uint64* state)
{
  int i;
  uint64 temp = *state;
  temp = ICC_RNG_NEXT(temp);
  for (i = 0; i < len; i++) {
    double x, y, v, ax, bx;
    for (;;) {
      x = ((int) temp) * 1.167239e-9;
      temp = ICC_RNG_NEXT(temp);
      ax = fabs(x);
      v = 2.8658 - ax * (2.0213 - 0.3605 * ax);
      y = ((unsigned) temp) * 2.328306e-10;
      temp = ICC_RNG_NEXT(temp);
      if (y < v || ax < 1.17741) {
        break;
      }
      bx = x;
      x = bx > 0 ? 0.8857913 * (2.506628 - ax) : -0.8857913 * (2.506628 - ax);
      if (y > v + 0.0506) {
        break;
      }
      if (log(y) < .6931472 - .5 * bx * bx) {
        x = bx;
        break;
      }
      if (log(1.8857913 - y) < .5718733 - .5 * x * x) {
        break;
      }
      do {
        v = ((int) temp) * 4.656613e-10;
        x = -log(fabs(v)) * .3989423;
        temp = ICC_RNG_NEXT(temp);
        y = -log(((unsigned) temp) * 2.328306e-10);
        temp = ICC_RNG_NEXT(temp);
      } while (y + y < x * x);
      x = v > 0 ? 2.506628 + x : -2.506628 - x;
      break;
    }
    arr[ i ] = (float) x;
  }
  *state = temp;
  return 0;
}
#define RAND_BUF_SIZE  96
// uint64* state
// const double* param
#define _RANDN(_height, _width, arr, _step, param1, param2, state, worktype)  \
  do {                                                                          \
    float buffer[RAND_BUF_SIZE];                                                \
    int size_height = _height, size_width = _width, larr;                       \
    float _p1 = (float)(param1), _p2 = (float)(param2);                         \
    for( larr=0; size_height--; larr += _step ) {                               \
      int i, j, len = RAND_BUF_SIZE;                                            \
      for( i = 0; i < size_width; i += RAND_BUF_SIZE ) {                        \
        int k = 3;                                                              \
        if( i + len > size_width )                                              \
          len = size_width - i;                                                 \
        \
        icvRandn_0_1_32f_C1R_( buffer, len, &state );                            \
        for( j = 0; j <= len - 4; j += 4 ) {                                    \
          arr[larr+i+j  ] = (worktype)(buffer[j  ]*_p2 + _p1);                  \
          arr[larr+i+j+1] = (worktype)(buffer[j+1]*_p2 + _p1);                  \
          arr[larr+i+j+2] = (worktype)(buffer[j+2]*_p2 + _p1);                  \
          arr[larr+i+j+3] = (worktype)(buffer[j+3]*_p2 + _p1);                  \
          if( --k == 0 ) {                                                      \
            k = 3;                                                              \
          }                                                                     \
        }                                                                       \
        for( ; j < len; j++ ) {                                                 \
          arr[larr+i+j] = (worktype)( buffer[j]*_p2 + _p1 );                    \
        }                                                                       \
      }                                                                         \
    }                                                                           \
  } while(0)
#define RANDARR(_height, _width, arr, _step, param1, param2, _rng, disttype, worktype)\
  do {                                                                          \
    if( disttype == CC_RAND_UNI ) {                                             \
      int t0 = (int)ceil( param1 );                                             \
      int t1 = (int)floor( param2 ) - t0;                                       \
      double diff = param1 - param2;                                            \
      if (0&&INT_MIN <= diff && diff <= INT_MAX && (t1 & (t1 - 1)) == 0) {      \
        _RAND_BITS(_height, _width, arr, _step, param1, param2,_rng,worktype); \
      } else {                                                                  \
        _RAND(_height, _width, arr, _step, param1, param2, _rng, worktype);     \
      }                                                                         \
    } else if( disttype == CC_RAND_NORMAL ) {                                   \
      _RANDN(_height, _width, arr, _step, param1, param2, _rng, worktype);      \
    } else {                                                                    \
      /* CC_ERROR( CC_StsBadArg, "Unknown distribution type" ); */              \
    }                                                                           \
  } while(0)
#define RANDARR_1D(_num, arr, param1, param2, _rng, disttype, worktype) \
  cvRandArr(1, _num, arr, _num, param1, param2, _rng, disttype, worktype)

#include "codec/base64.inl"
static int randstr(rng_t* r, char* buf, int len, const char* map1)
{
  int i, maplen;
  map1 = map1 ? map1 : "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  maplen = strlen(map1);
  for (i = 0; i < len; ++i) {
    int j = rng_int32(r) % maplen;
    buf[i] = map1[j];
  }
  buf[len] = 0;
  return len;
}
static int randstrnum(rng_t* r, char* buf, int len)
{
  int i;
  buf[0] = rng_int32(r) % 9 + '1';
  for (i = 1; i < len - 1; ++i) {
    buf[i] = rng_int32(r) % 10 + '0';
  }
  buf[len - 1] = 0;
  return len - 1;
}
static int rand_range(rng_t* r, int minValue, int maxValue)
{
  assert(minValue < maxValue);
  return rng_int32(r) % (maxValue - minValue) + minValue;
}
static int randstr_cn(rng_t* r, char* out, int n)
{
  int i;
  for (i = 0; i < n - 1; i += 2) {
    out[i + 0] = rand_range(r, 0xb0, 0xf7);
    out[i + 1] = rand_range(r, 0xa1, 0xfe);
  }
  out[i] = 0;
  return i;
}
// Ëæ»úÅÅÁÐ
static int rand_permutation(rng_t* r, uint* data, int count)
{
  int i, j, t;
  for (i = 0; i < count; ++i) {
    data[i] = i;
  }
  for (i = 0; i < count; ++i) {
    j = rand_range(r, i, count);
    t = data[i], data[i] = data[j], data[j] = t;
  }
  return 0;
}
static int rand_darr(rng_t* r, double* darr, int n, double dmin, double dmax)
{
  int i;
  double drange = dmax - dmin;
  for (i = 0; i < n; ++i) {
    double d = rng_real(r);
    darr[i] = d * drange + dmin;
  }
  return 0;
}
#define RANDPT(n, pt, xmin, xmax, ymin, ymax)  {int i; for (i = 0; i < n; ++i) { pt[i].x = rand_range(xmin, xmax); pt[i].y = rand_range(ymin, ymax); } }
//
static int randsamploy(rng_t* r, int n, DPOINT* pt, double xmin, double xmax, double ymin, double ymax)
{
  double x = (xmin + xmax) * 0.5, y = (ymin + ymax) * 0.5;
  double rx = (xmax - xmin) * 0.5, ry = (ymax - ymin) * 0.5;
  int i;
  for (i = 0; i < n; ++i) {
    pt[i].x = x + sin(2 * CC_PI * i / n) * 0.5 * rx - 0.5 * rng_real(r) * rx;
    pt[i].y = y + cos(2 * CC_PI * i / n) * 0.5 * ry - 0.5 * rng_real(r) * ry;
  }
  return 0;
}
static COLOR rand_color(rng_t* r)
{
  return _RGBA(rng_int32(r) & 0xff, rng_int32(r) & 0xff, rng_int32(r) & 0xff, rng_int32(r) & 0xff);
}
static int iRandArr(rng_t* r, int count, int dim, void* arr, int step, const int* imin, const int* imax)
{
  int i, j;
  for (i = 0; i < count; ++i) {
    int* arr1 = (int*)((uchar*)arr + i * step);
    for (j = 0; j < dim; ++j) {
      arr1[j] = rng_int32(r) % (imax[j] - imin[j]) + imin[j];
    }
  }
  return 0;
}

#define mtrand32() rng_int32(mt_static())
#define mtrand31() rng_int31(mt_static())
#define mtreal3()  rng_real2(mt_static())

#define SHUFFLE(r, array, arrlen, t) { \
  int m = arrlen; \
  while (m) { \
    int i = rng_int32(r) % m--; \
    CC_SWAP(array[m], array[i], t); \
  } \
}

#include "random/mt19937ar.h"

static uint32 rng_mt19937_gen_int32(rng_t* r)
{
  mt19937ar_t* x = container_of(r, mt19937ar_t, rng);
  return mt19937ar_int32(x);
}
static rng_t* rng_mt19937_init(mt19937ar_t* mt, unsigned long s)
{
  mt->mti = MT19937AR_N + 1;
  mt19937ar_init(mt, s);
  mt->rng->gen_int32 = rng_mt19937_gen_int32;
  return mt->rng;
}

static rng_t* mt_static()
{
  static mt19937ar_t mt[1] = {0};
  static int inited = 0;
  if (!inited) {
    rng_mt19937_init(mt, 10);
    inited = 1;
  }
  return mt->rng;
}
static void mtsrand(unsigned long s)
{
  rng_t* r = mt_static();
  mt19937ar_t* x = container_of(r, mt19937ar_t, rng);
  rng_mt19937_init(x, s);
}


#endif // _RAND_INL_
