
#include "myblas/myblas.h"

struct Udata {
  float* p;
  void* u;
};
#if 0
float* resetArr(float** pdata, int count)
{
  return *pdata = (float*)realloc(*pdata, sizeof(float)*count);
}
#else
#define freeArr(pdata)  (free(pdata), pdata = NULL)
#define resetArr(pdata, count)   (*(char**)(pdata) = (char*)realloc(*(pdata), sizeof(**(pdata))*count), *(pdata))
#endif
static float* reset(float** data, DataSize size)
{
  return resetArr(data, Data_total(size));
}
static void* clone(void* dst, const void* src, int count)
{
  char** pdst = (char**)dst;
  if (*pdst!=(char*)src) {
    resetArr(pdst, count);
    memcpy(*pdst, src, count);
  }
  return *pdst;
}
static float* fillArr(float* data, int count, const float item)
{
  int i;
  for (i = 0; i < count; ++i) {
    data[i] = item;
  }
  return data;
}
static float* fillData(float* data, DataSize size, const float item)
{
  int i, count = Data_total(size);
  for (i = 0; i < count; ++i) {
    data[i] = item;
  }
  return data;
}
static float* resetArrZero(float** data, int count)
{
  return fillArr(resetArr(data, count), count, 0.f);
}
static float* resetZero(float** data, DataSize size)
{
  resetArr(data, Data_total(size));
  ASSERT(*data);
  return fillData(*data, size, 0.f);
}
static float* resetUdataArr(struct Udata* data, int count)
{
  return resetArr(&data->p, sizeof(float) * count);
}
static float* resetUdata(struct Udata* data, DataSize size)
{
  return resetArr(&data->p, Data_total(size));
}
#define setzero(data)  memset(data, 0, sizeof(*(data)))

static int* random_shuffle(int n, int* arr)
{
  int i, j;
  int t;
  for (i = n - 1; i > 0; --i) {
    j = rand() % i;
    t = arr[i], arr[i] = arr[j], arr[j] = t;
  }
  return arr;
}
static void* mem_shuffle(int n, void* arr, int elemsize, const int* shuffle)
{
  char* tmp = MALLOC(char, n*elemsize);
  char* ptr = (char*)arr;
  int i, j;
  memcpy(tmp, arr, n*elemsize);
  for (i=0; i<n; ++i) {
    j = shuffle[i];
    memcpy(ptr + i*elemsize, tmp + j*elemsize, elemsize);
  }
  FREE(tmp);
  return arr;
}
static int* init_perm(int n, int* arr) {
  int i;
  for (i = 0; i < n; ++i) {
    arr[i] = i;
  }
  return arr;
}
static int* get_shuffle(int n, int** parr)
{
  int* arr = resetArr(parr, n);
  init_perm(n, arr);
  return random_shuffle(n, arr);
}
static double uniform()
{
  return (double)rand() / RAND_MAX;
}
static double gaussrand()
{
  double x = 0;
  int i, NSUM = 25;
  for (i = 0; i < NSUM; i++) {
    x += (double)rand() / RAND_MAX;
  }
  x -= NSUM / 2.0;
  x /= sqrt(NSUM / 12.0);
  return x;
}
static int bernoulli(double p)
{
  double u_0_1 = uniform();
  return ((u_0_1) <= p) ? 1 : 0;
}

#include "std/rand_c.h"
#ifndef _RAND_INL_
#define rng_int32(seed)          rand()
#define rng_uniform(seed, a, b)  (uniform() * ((b)-(a)) + (a))
#define rng_normal(seed, m, s)   (gaussrand()* (s) + (m))
#define rng_bernoulli(seed, p)   bernoulli(p)
#endif // _RAND_INL_

#if 0
#define SWAP_T(a, b, T)  {T t=a;a=b,b=t;}
#define CC_INLINE static
#define ASSERT assert
typedef struct {
  int l, t, r, b;
} IRECT;
#endif

#if 0
#include "ThreadPool.inl"
#define DISPATCH_WORKER_BEGIN(n) { int _number = n; auto _worker = [&](int start, int stop)
#define DISPATCH_WORKER_END ; dispatch_worker(_worker, _number); }
#else
#define set_thread_num(x)
#define DISPATCH_WORKER_BEGIN(n) { int start = 0, stop = n;
#define DISPATCH_WORKER_END ; }
#endif


#define EXPECT_NEAR(a, b, e)  ASSERT(FNEAR(a, b, e))
#define TESTFUN(fun)  int fun(); int fun##_tested = fun(); int fun()
#define TYPED_TEST(app, fun)   TESTFUN(app ## fun)

static unsigned int blas_rng_rand() {
  return rng_int32(NULL);
}

// float blas_nextafter(const float b) { return nextafterf( b, std::numeric_limits::max());}
static void blas_rng_uniform(int n, const float a, const float b, float* r) {
  int i;
  ASSERT(n>0);
  ASSERT(r);
  ASSERT(a<b);
  for (i = 0; i < n; ++i) {
    r[i] = rng_uniform(NULL, a, b);
  }
}

static void blas_rng_gaussian(int n, const float _Mean, const float sigma, float* r) {
  int i;
  ASSERT(n>0);
  ASSERT(r);
  ASSERT(sigma> 0);
  for (i = 0; i < n; ++i) {
    r[i] = rng_normal(NULL, _Mean, sigma);
  }
}

static void blas_srng_bernoulli(int n, const float p, int* r) {
  int i;
  ASSERT(n>0);
  ASSERT(r);
  ASSERT(p>0 && p<1);
  for (i = 0; i < n; ++i) {
    r[i] = rng_bernoulli(NULL, p);
  }
}

static void blas_irng_bernoulli(int n, const float p, unsigned int* r) {
  int i;
  ASSERT(n>0);
  ASSERT(r);
  ASSERT(p>0 && p<1);
  for (i = 0; i < n; ++i) {
    r[i] = rng_bernoulli(NULL, p);
  }
}
static void fill_PositiveUnitball(float* data, DataSize size) {
  int i, j, dim, count = Data_total(size);
  blas_rng_uniform(count, 0, 1, data);
  // We expect the filler to not be called very frequently, so we will
  // just use a simple implementation
  dim = count / size.n;
  for (i = 0; i < size.n; ++i) {
    float sum = 0;
    for (j = 0; j < dim; ++j) {
      sum += data[i * dim + j];
    }
    for (j = 0; j < dim; ++j) {
      data[i * dim + j] /= sum;
    }
  }
}

enum FillMethod {
  FillMethod_AVERAGE,
  FillMethod_FAN_IN,
  FillMethod_FAN_OUT,
};
#define DEFAULT(x)   =x

static void fill_Xavier(float* data, DataSize size, enum FillMethod variance_norm DEFAULT(FillMethod_FAN_IN)) {
  int count = Data_total(size);
  int fan_in = count / size.n;
  int fan_out = count / size.c;
  float scale, n = fan_in;  // default to fan_in
  if (variance_norm == FillMethod_AVERAGE) {
    n = (fan_in + fan_out) / (float)(2);
  } else if (variance_norm == FillMethod_FAN_OUT) {
    n = fan_out;
  }
  scale = sqrt((float)(3) / n);
  blas_rng_uniform(count, -scale, scale, data);
}

static void fill_MSRA(float* data, DataSize size, enum FillMethod variance_norm, float alpha) {
  int count = Data_total(size);
  int fan_in = count / size.n;
  int fan_out = count / size.c;
  float std, n = fan_out;  // default to fan_in
  if (variance_norm == FillMethod_AVERAGE) {
    n = (fan_in + fan_out) / (float)(2);
  } else if (variance_norm == FillMethod_FAN_IN) {
    n = fan_in;
  }
  std = sqrt((float)(2) / n / (1 + alpha * alpha));
  blas_rng_gaussian(count, (0), std, data);
}

static void fill_Bilinear(float* data, DataSize size) {
  int count = Data_total(size);
  //CHECK_EQ(blob->num_axes(), 4) << "Blob must be 4 dim.";
  //CHECK_EQ(blob->input_w(), blob->input_h()) << "Filter must be square";
  int f = ceil(size.w / 2.);
  float c = (2 * f - 1 - f % 2) / (2. * f);
  int i;for (i = 0; i < count; ++i) {
    float x = i % size.w;
    float y = (i / size.w) % size.h;
    data[i] = (1 - fabs(x / f - c)) * (1 - fabs(y / f - c));
  }
}
static void fill_GaussianUnitBall(float* data, DataSize size) {
  int count = Data_total(size);
  int fan_in = count / size.n;
  int i, n = fan_in;  // default to fan_in
  float sum_sq;
  blas_rng_gaussian(count, (0), 1, data);
  for (i = 0; i < size.n; i++) {
    sum_sq = blas_sdot(n, data + i * n, data + i * n) + 1e-12;
    blas_sscale(n, (float)(1.0) / sqrt(sum_sq), data + i * n, data + i * n);
  }
}

static void normal_distribution_init(float* data, int count, const float mean_value, const float standard_deviation)
{
  int i;
  for (i = 0; i < count; i++) {
    //data[i] = dist(engine);
    data[i] = (float)gaussrand() * standard_deviation + mean_value;
  }
}
static void uniform_distribution_init(float* data, int count, const float low_value, const float high_deviation)
{
  int i;
  float a = high_deviation - low_value;
  for (i = 0; i < count; i++) {
    //data[i] = dist(engine);
    data[i] = (float)uniform() * a + low_value;
  }
}
static void uniform_distribution_init_int(int* data, int count, int low_value, int high_deviation)
{
  int i;
  int a = high_deviation - low_value;
  for (i = 0; i < count; i++) {
    //data[i] = dist(engine);
    data[i] = rand() % a + low_value;
  }
}
static void const_distribution_init(float* data, int count, float const_value)
{
  int i;
  for (i = 0; i < count; i++) {
    data[i] = const_value;
  }
}
static void xavier_init(float* data, int count, int fan_in, int fan_out)
{
  float weight_base = (float)sqrt(6.0f / (float)(fan_in + fan_out));
  uniform_distribution_init(data, count, -weight_base, weight_base);
}

#if 0

static int showmat_1(const char* name, const float* data, DataSize size, int pn, int pc) {
  const float* imageData = data + Data_4DIndex(size, pn, pc, 0, 0);
  img_t image[1] = {0};
  cvSetMat(image, size.h, size.w, CC_32F, 1, NULL, CC_AUTOSTEP);
  ShowImagePal(name, size.h, size.w, (const uchar*)imageData, size.w*sizeof(*data), sizeof(*data), PF_32bppGrayFloat, 0);
  imfree(image);
  return 0;
}

static int showmat(const char* name, const float* data, DataSize size) {
  int pn, pc;
  char buf[256];
  for (pn = 0; pn < size.n; pn++) {
    for (pc = 0; pc < size.c; pc++) {
      _snprintf(buf, 256, "%s_n%d_c%d_h%d_w%d", name, pn, pc, size.h, size.w);
      showmat_1(buf, data, size, pn, pc);
    }
  }
  return 0;
}
#endif

