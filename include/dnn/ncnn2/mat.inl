
#include "cstd.h"
enum {
  PIXEL_CONVERT_SHIFT = 16,
  PIXEL_FORMAT_MASK = 0x0000ffff,
  PIXEL_CONVERT_MASK = 0xffff0000,

  PIXEL_RGB       = 1,
  PIXEL_BGR       = (1 << 1),
  PIXEL_GRAY      = (1 << 2),
  PIXEL_RGBA      = (1 << 3),

  PIXEL_RGB2BGR   = PIXEL_RGB | (PIXEL_BGR << PIXEL_CONVERT_SHIFT),
  PIXEL_RGB2GRAY  = PIXEL_RGB | (PIXEL_GRAY << PIXEL_CONVERT_SHIFT),

  PIXEL_BGR2RGB   = PIXEL_BGR | (PIXEL_RGB << PIXEL_CONVERT_SHIFT),
  PIXEL_BGR2GRAY  = PIXEL_BGR | (PIXEL_GRAY << PIXEL_CONVERT_SHIFT),

  PIXEL_GRAY2RGB  = PIXEL_GRAY | (PIXEL_RGB << PIXEL_CONVERT_SHIFT),
  PIXEL_GRAY2BGR  = PIXEL_GRAY | (PIXEL_BGR << PIXEL_CONVERT_SHIFT),

  PIXEL_RGBA2RGB  = PIXEL_RGBA | (PIXEL_RGB << PIXEL_CONVERT_SHIFT),
  PIXEL_RGBA2BGR  = PIXEL_RGBA | (PIXEL_BGR << PIXEL_CONVERT_SHIFT),
  PIXEL_RGBA2GRAY = PIXEL_RGBA | (PIXEL_GRAY << PIXEL_CONVERT_SHIFT),
};


// mat process
enum {
  BORDER_CONSTANT = 0,
  BORDER_REPLICATE = 1,
};


// exchange-add operation for atomic operations on reference counters
#if defined __INTEL_COMPILER && !(defined WIN32 || defined _WIN32)
// atomic increment on the linux version of the Intel(tm) compiler
#  define NCNN_XADD(addr, delta) (int)_InterlockedExchangeAdd(const_cast<void*>(reinterpret_cast<volatile void*>(addr)), delta)
#elif defined __GNUC__
#  if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)
#    ifdef __ATOMIC_ACQ_REL
#      define NCNN_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
#    else
#      define NCNN_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), delta, 4)
#    endif
#  else
#    if defined __ATOMIC_ACQ_REL && !defined __clang__
// version for gcc >= 4.7
#      define NCNN_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
#    else
#      define NCNN_XADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
#    endif
#  endif
#elif defined _MSC_VER && _MSC_VER>1200 && !defined RC_INVOKED
#  include <intrin.h>
#  define NCNN_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
#else
CC_INLINE int NCNN_XADD(int* addr, int delta)
{
  int tmp = *addr;
  *addr += delta;
  return tmp;
}
#endif

typedef img_t pBox;




#ifdef HAVE_OPENCL1
bool used_gpu = 0;
#endif

buf_t bfbf(buf_t* bf)
{
  return *bf;
}

//#include "cv1/imgproc/imgwarp.inl"

// resize' : undeclared identifier
// cSize' : undeclared identifier
// cvGetSubRect' : undeclared identifier

//using namespace std;
#if 0
#define mydataFmt float
#else
typedef float mydataFmt;
#endif

struct udata_t;

typedef struct umem_t {
  struct udata_t* u;
  bool modifyp, modifyu;
  int datasize;
  mydataFmt* p;
} umem_t;

#ifdef HAVE_OPENCL1
bool umem_empty(const umem_t* a)
{
  return NULL == a || NULL == a->p;
}
#define UMEM_SETU(a)    (a->modifyp) ? umem_setu(a) : a->udata
#define UMEM_GETU(a)    (a->modifyu) ? umem_getu(a) : a->pdata
udata_t* umem_setu0(const umem_t* a)
{
  if (used_gpu) {
    ocl_device_t* dev = ocl_device_get();
    clEnqueueWriteBuffer(dev->cmdQueue, (cl_mem)a->u, CL_TRUE, 0, a->datasize, a->p, 0, NULL, NULL);
    ((umem_t*)a)->modifyp = 0;
  }

  return a->u;
}
CC_INLINE udata_t* umem_setu(const umem_t* a)
{
  if (a->modifyp && used_gpu) {
    tic;
    ocl_device_t* dev = ocl_device_get();
    clEnqueueWriteBuffer(dev->cmdQueue, (cl_mem)a->u, CL_TRUE, 0, a->datasize, a->p, 0, NULL, NULL);
    ((umem_t*)a)->modifyp = 0;
    toc;
  }

  return a->u;
}

CC_INLINE mydataFmt* umem_getu(const umem_t* a)
{
  if (a->modifyu && used_gpu) {
    tic;
    ocl_device_t* dev = ocl_device_get();
    clEnqueueReadBuffer(dev->cmdQueue, (cl_mem)a->u, CL_TRUE, 0, a->datasize, a->p, 0, NULL, NULL);
    ((umem_t*)a)->modifyu = 0;
    toc;
  }

  return a->p;
}
#else
#define umem_getu(a)   (a)
#define umem_setu(a)   (a)
#define umem_empty(a)  (a)==NULL
#define umem_setu0(a)  (a)
#endif

static void umem_setsize(umem_t* a, int datasize)
{
  if (a->datasize < datasize) {
    a->p = (mydataFmt*)realloc(a->p, datasize);
    a->datasize = datasize;

#ifdef HAVE_OPENCL1

    if (a->u) {
      clReleaseMemObject((cl_mem)a->u);
      a->u = NULL;
    }

    if (used_gpu) {
      ocl_device_t* dev = ocl_device_get();
      a->u = (udata_t*)clCreateBuffer(dev->context, CL_MEM_READ_WRITE, datasize, NULL, NULL);
    }

#endif
  }

  a->modifyp = 0;
  a->modifyu = 0;
  memset(a->p, 0, datasize);
}

static void umem_free(umem_t* a)
{
  if (a->p) {
    free(a->p);
    a->p = NULL;
  }

#ifdef HAVE_OPENCL1

  if (a->u) {
    clReleaseMemObject((cl_mem)a->u);
    a->u = NULL;
  }

#endif

  a->datasize = 0;
  a->modifyp = 0;
  a->modifyu = 0;
}

static double dist_L2(const float* a, const float* b, int n)
{
  double sum = 0;
  int i;

  for (i = 0; i < n; ++i) {
    double d = a[i] - b[i];
    sum += d * d;

    if (d * d > 0.000001) {
      int asdf = 0;
    }
  }

  sum /= n;
  sum = sqrt(sum);
  return sum;
}
#ifdef HAVE_OPENCL1
float* umem_map(const umem_t* a)
{
  ocl_device_t* dev = ocl_device_get();
  cl_int retval = 0;
  float* data = (float*)clEnqueueMapBuffer(dev->cmdQueue, (cl_mem)a->u,
      CL_TRUE, (CL_MAP_READ | CL_MAP_WRITE), 0, a->datasize, 0, 0, 0, &retval);
  return data;
}
float* umem_unmap(const umem_t* a, float* data)
{
  ocl_device_t* dev = ocl_device_get();
  cl_int retval = 0;
  clEnqueueUnmapMemObject(dev->cmdQueue, (cl_mem)a->u, data, 0, NULL, NULL);
  return data;
}

double umem_checku(const umem_t* a)
{
  ocl_device_t* dev = ocl_device_get();
  cl_int retval = 0;
  int n = a->datasize / sizeof(a->p[0]);
  float* data = (float*)clEnqueueMapBuffer(dev->cmdQueue, (cl_mem)a->u,
      CL_TRUE, (CL_MAP_READ | CL_MAP_WRITE), 0, a->datasize, 0, 0, 0, &retval);
  double dis = dist_L2(data, a->p, n);
  clEnqueueUnmapMemObject(dev->cmdQueue, (cl_mem)a->u, data, 0, NULL, NULL);
  return dis;
}

pBox {
  umem_t data[1];
  int flags;
  union{ int width; int w; };
  union{ int height; int h; };
  union{ int channel; int c; };
};
#endif


#if 0
#define SETMODIFYP(a, x)   (a)->data->modifyp = (x)
CC_INLINE int pBox_channels(const pBox* a)
{
  return a->channel;
}
CC_INLINE float* pBox_datap(const pBox* a)
{
  return (float*)(a->data->p);
}
CC_INLINE int pBox_size(const pBox* a)
{
  return a->height * a->width * a->channel;
}

int pBoxsetsize(pBox* a, int h, int w, int c = 1)
{
  a->height = h;
  a->width = w;
  a->channel = c;
  int datasize = h * w * c * sizeof(float);
  umem_setsize(a->data, datasize);
  memset(a->data->p, 0, datasize);
  return 0;
}

CC_INLINE void freepBox(pBox* pbox)
{
  umem_free(pbox->data);
}
CC_INLINE void pBoxfrees(pBox* pbox, int n)
{
  for (int i = 0; i < n; ++i) {
    umem_free(pbox[i].data);
  }
}
#else
#define SETMODIFYP(a, x)   (0)
CC_INLINE int pBox_channels(const pBox* a)
{
  return a->c / sizeof(float);
}
CC_INLINE int pBox_length(const pBox* a)
{
  return (a->c * a->h * a->w) / sizeof(float);
}
CC_INLINE float* pBox_datap(const pBox* a)
{
  return (float*)(a->data);
}
CC_INLINE float* pBox_datapf(const pBox* a, int f)
{
  return (float*)(a->data + f * a->h * a->w * a->c);
}
CC_INLINE int pBox_size(const pBox* a)
{
  return a->f * a->height * a->width * a->c / sizeof(float);
}
CC_INLINE int pBoxsetsize1(pBox* a, int h, int w, int c, int f CC_DEFAULT(1))
{
  cvSetMats(a, h, w, CC_32F, c, f, NULL, CC_AUTOSTEP);
  return 0;
}

CC_INLINE int pBoxsetsize(pBox* a, int h, int w, int c CC_DEFAULT(1), int f CC_DEFAULT(1))
{
  cvSetMats(a, h, w, CC_32F, c, f, NULL, CC_AUTOSTEP);
  return 0;
}

CC_INLINE int pBox_create(pBox* a, int w, int h, int c CC_DEFAULT(1), int f CC_DEFAULT(1))
{
  cvSetMats(a, h, w, CC_32F, c, f, NULL, CC_AUTOSTEP);
  return 0;
}

CC_INLINE int pBox_dims(const pBox* a)
{
  return a->w>1 + a->h>0 + a->f>0;
}

CC_INLINE int bf_pBoxsetsize(buf_t* bf, pBox* a, int h, int w, int c CC_DEFAULT(1), int f CC_DEFAULT(1))
{
  bf_cvSetMats(bf, a, h, w, CC_32F, c, f, NULL, CC_AUTOSTEP);
  return 0;
}

CC_INLINE void freepBox(pBox* pbox)
{
  imfree(pbox);
}
CC_INLINE void pBoxfrees(pBox* pbox, int n)
{
  freeims(pbox, n);
}
#endif
#define pBox_total pBox_size
#define pBox_empty imempty
CC_INLINE size_t cvMatTotal(const img_t* mat)
{
  return pBox_size(mat);
}
CC_INLINE void pBox_fill(img_t* mat, float _v)
{
  size_t i = 0, _total = cvMatTotal(mat);
  float* data = pBox_datap(mat);

  for (; i < _total; i += 4) {
    data[i] = _v;
    data[i + 1] = _v;
    data[i + 2] = _v;
    data[i + 3] = _v;
  }

  for (; i < _total; i++) {
    data[i] = _v;
  }
}

CC_INLINE float* pBox_channel_p(const img_t* mat, int c)
{
  return ((float*)mat->data) + mat->h * mat->w * c;
}
CC_INLINE img_t pBox_channel(const img_t* mat, int c)
{
  img_t im = *mat;
  im.data = (uchar*)pBox_channel_p(mat, c);
  im.c = sizeof(float);
  return im;
}
CC_INLINE img_t pBox_frame(const img_t* mat, int f)
{
  img_t im = *mat;
  im.data = (uchar*)pBox_datapf(mat, f);
  im.f = 1;
  return im;
}
CC_INLINE float* pBox_at(const img_t* mat, int c, int row, int col)
{
  return ((float*)mat->data) + mat->h * mat->w * c + mat->w * row + col;
}
CC_INLINE float* pBox_crow(const img_t* mat, int c, int row)
{
  return ((float*)mat->data) + mat->h * mat->w * c + mat->w * row;
}
CC_INLINE float* pBox_row0(const img_t* mat, int row)
{
  return ((float*)mat->data) + mat->w * row;
}


struct pRelu {
  umem_t data[1];
  int width;
};

typedef struct Weight {
  umem_t data[1];
  umem_t bias[1];
  int lastChannel;
  int selfChannel;
  int kernelSize;
  int stride;
  int pad;
  float* kernel_tm;
} Weight;

static void freepRelu(struct pRelu* prelu)
{
  umem_free(prelu->data);
}

static void freeWeight(struct Weight* weight)
{
  umem_free(weight->data);
  umem_free(weight->bias);

  if (weight->kernel_tm) {
    FREE(weight->kernel_tm);
  }
}


static void substract_mean_normalize(int c, int h, int w, float* data, int cstep, const float* mean_vals, const float* norm_vals)
{
  int q, size = w * h;

  if (mean_vals && !norm_vals) {
    // substract mean only
    #pragma omp parallel for
    for (q = 0; q < c; q++) {
      float* ptr = data + cstep * q;
      const float mean = mean_vals[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
      float32x4_t _mean = vdupq_n_f32(mean);

      for (; nn > 0; nn--) {
        float32x4_t _ptr = vld1q_f32(ptr);
        _ptr = vsubq_f32(_ptr, _mean);
        vst1q_f32(ptr, _ptr);
        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "vdup.f32   q1, %4              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vsub.f32   q0, q0, q1          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr),
            "r"(mean)     // %4
            : "cc", "memory", "q0", "q1"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr -= mean;
        ptr++;
      }
    }
  }
  else if (!mean_vals && norm_vals) {
    // normalize only
    #pragma omp parallel for
    for (q = 0; q < c; q++) {
      float* ptr = data + cstep * q;
      const float norm = norm_vals[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
      float32x4_t _norm = vdupq_n_f32(norm);

      for (; nn > 0; nn--) {
        float32x4_t _ptr = vld1q_f32(ptr);
        _ptr = vmulq_f32(_ptr, _norm);
        vst1q_f32(ptr, _ptr);
        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "vdup.f32   q1, %4              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vmul.f32   q0, q0, q1          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr),
            "r"(norm)     // %4
            : "cc", "memory", "q0", "q1"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr *= norm;
        ptr++;
      }
    }
  }
  else if (mean_vals && norm_vals) {
    // substract mean and normalize
    #pragma omp parallel for
    for (q = 0; q < c; q++) {
      float* ptr = data + cstep * q;
      const float mean = mean_vals[q];
      const float norm = norm_vals[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
      float32x4_t _mean = vdupq_n_f32(mean);
      float32x4_t _norm = vdupq_n_f32(norm);

      for (; nn > 0; nn--) {
        float32x4_t _ptr = vld1q_f32(ptr);
        _ptr = vsubq_f32(_ptr, _mean);
        _ptr = vmulq_f32(_ptr, _norm);
        vst1q_f32(ptr, _ptr);
        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "vdup.f32   q1, %4              \n"
            "vdup.f32   q2, %5              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vsub.f32   q0, q0, q1          \n"
            "vmul.f32   q0, q0, q2          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr),
            "r"(mean),    // %4
            "r"(norm)     // %5
            : "cc", "memory", "q0", "q1", "q2"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr = (*ptr - mean) * norm;
        ptr++;
      }
    }
  }
}

// convert half precision floating point to float
static float half2float(unsigned short value)
{
  // 1 : 5 : 10
  unsigned short sign = (value & 0x8000) >> 15;
  unsigned short exponent = (value & 0x7c00) >> 10;
  unsigned short significand = value & 0x03FF;

  //     fprintf(stderr, "%d %d %d\n", sign, exponent, significand);

  // 1 : 8 : 23
  union {
    unsigned int u;
    float f;
  } tmp;

  if (exponent == 0) {
    if (significand == 0) {
      // zero
      tmp.u = (sign << 31);
    }
    else {
      // denormal
      exponent = 0;

      // find non-zero bit
      while ((significand & 0x200) == 0) {
        significand <<= 1;
        exponent++;
      }

      significand <<= 1;
      significand &= 0x3FF;
      tmp.u = (sign << 31) | ((-exponent + (-15 + 127)) << 23) | (significand << 13);
    }
  }
  else if (exponent == 0x1F) {
    // infinity or NaN
    tmp.u = (sign << 31) | (0xFF << 23) | (significand << 13);
  }
  else {
    // normalized
    tmp.u = (sign << 31) | ((exponent + (-15 + 127)) << 23) | (significand << 13);
  }

  return tmp.f;
}

static int from_float16(float* data32, const unsigned short* data16, int size)
{
  float* ptr = data32;

#if __ARM_NEON && (__ARM_FP & 2)
  int nn = cpu_support_arm_vfpv4() ? size >> 2 : 0;
  int remain = size - (nn << 2);
#else
  int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON && (__ARM_FP & 2)
#if __aarch64__

  if (nn > 0) {
    asm volatile(
        "0:                             \n"
        "ldr    d0, [%1], #8            \n"
        "fcvtl  v1.4s, v0.4h            \n"
        "subs   %w0, %w0, #1            \n"
        "str    q1, [%2], #16           \n"
        "bne    0b                      \n"
        : "=r"(nn),     // %0
        "=r"(data16),   // %1
        "=r"(ptr)     // %2
        : "0"(nn),
        "1"(data16),
        "2"(ptr)
        : "cc", "memory", "v0", "v1"
    );
  }

#else

  if (nn > 0) {
    asm volatile(
        "0:                             \n"
        "pld        [%1, #64]           \n"
        "vld1.s16   {d0}, [%1 :64]!     \n"
        "vcvt.f32.f16 q1, d0            \n"
        "subs       %0, #1              \n"
        "vst1.f32   {d2-d3}, [%2 :128]! \n"
        "bne        0b                  \n"
        : "=r"(nn),     // %0
        "=r"(data16),   // %1
        "=r"(ptr)     // %2
        : "0"(nn),
        "1"(data16),
        "2"(ptr)
        : "cc", "memory", "q0", "q1"
    );
  }

#endif // __aarch64__
#endif // __ARM_NEON

  for (; remain > 0; remain--) {
    *ptr = half2float(*data16);

    data16++;
    ptr++;
  }

  return 0;
}

static void copy_make_border_image(const img_t* src, img_t* dst, int top, int left, int type, float v)
{
  int w = dst->w;
  int h = dst->h;

  const float* ptr = pBox_datap(src);
  float* outptr = pBox_datap(dst);

  if (type == BORDER_CONSTANT) {
    int y = 0;

    // fill top
    for (; y < top; y++) {
      int x = 0;

      for (; x < w; x++) {
        outptr[x] = v;
      }

      outptr += w;
    }

    // fill center
    for (; y < (top + src->h); y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = v;
      }

      for (; x < (left + src->w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < w; x++) {
        outptr[x] = v;
      }

      ptr += src->w;
      outptr += w;
    }

    // fill bottom
    for (; y < h; y++) {
      int x = 0;

      for (; x < w; x++) {
        outptr[x] = v;
      }

      outptr += w;
    }
  }
  else if (type == BORDER_REPLICATE) {
    int y = 0;

    // fill top
    for (; y < top; y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src->w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < w; x++) {
        outptr[x] = ptr[src->w - 1];
      }

      outptr += w;
    }

    // fill center
    for (; y < (top + src->h); y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src->w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < w; x++) {
        outptr[x] = ptr[src->w - 1];
      }

      ptr += src->w;
      outptr += w;
    }

    // fill bottom
    ptr -= src->w;

    for (; y < h; y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src->w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < w; x++) {
        outptr[x] = ptr[src->w - 1];
      }

      outptr += w;
    }
  }
}

static img_t* copy_make_border(buf_t* bf, const img_t* src, img_t* dst, int top, int bottom, int left, int right, int type, float v)
{
  int w = src->w + left + right;
  int h = src->h + top + bottom;
  int channels, q;

  if (w == src->w && h == src->h) {
    return cvCloneMat(dst, src);
  }

  channels = pBox_channels(src);

  bf_pBoxsetsize(bf, dst, h, w, channels, 1);

  if (imempty(dst)) {
    return dst;
  }

  // unroll image channel
  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    const img_t m = pBox_channel(src, q);
    img_t borderm = pBox_channel(dst, q);

    copy_make_border_image(&m, &borderm, top, left, type, v);
  }

  return dst;
}

static img_t* copy_cut_border_image(const img_t* src, img_t* dst, int top, int left)
{
  int x, y;
  int w = dst->w;
  int h = dst->h;

  const float* ptr = pBox_at(src, 0, top, left);
  float* outptr = pBox_datap(dst);

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      outptr[x] = ptr[x];
    }

    outptr += w;
    ptr += src->w;
  }

  return dst;
}

static img_t* copy_cut_border(buf_t* bf, const img_t* src, img_t* dst, int top, int bottom, int left, int right)
{
  int q, channels;
  int w = src->w - left - right;
  int h = src->h - top - bottom;

  if (w == src->w && h == src->h) {
    return cvCloneMat(dst, src);
  }

  channels = pBox_channels(src);
  
  bf_pBoxsetsize(bf, dst, h, w, channels, 1);
  
  if (imempty(dst)) {
    return dst;
  }
  
  // unroll image channel
#pragma omp parallel for
  for (q = 0; q < channels; q++) {
    const img_t m = pBox_channel(src, q);
    img_t cutm = pBox_channel(dst, q);
    
    copy_cut_border_image(&m, &cutm, top, left);
  }
  
  return dst;
}

static img_t* resize_bilinear_image(const img_t* src, img_t* dst, int w, int h)
{
  double scale_x = (double)src->w / w;
  double scale_y = (double)src->h / h;

  int* buf = MALLOC(int, w + h + w * 2 + h * 2);

  int* xofs = buf;//new int[w];
  int* yofs = buf + w;//new int[h];

  float* alpha = (float*)(buf + w + h);//new float[w * 2];
  float* beta = (float*)(buf + w + h + w * 2); //new float[h * 2];

  float fx, fy;
  float* _rowsbuf, *rows0, *rows1;
  int sx, sy, dy, dx;
  int prev_sy1;

  for (dx = 0; dx < w; dx++) {
    fx = (float)((dx + 0.5) * scale_x - 0.5);
    sx = fx;//cvFloor(fx);
    fx -= sx;

    if (sx >= src->w - 1) {
      sx = src->w - 2;
      fx = 1.f;
    }

    xofs[dx] = sx;

    alpha[dx * 2    ] = 1.f - fx;
    alpha[dx * 2 + 1] = fx;
  }

  for (dy = 0; dy < h; dy++) {
    fy = (float)((dy + 0.5) * scale_y - 0.5);
    sy = fy;//cvFloor(fy);
    fy -= sy;

    if (sy >= src->h - 1) {
      sy = src->h - 2;
      fy = 1.f;
    }

    yofs[dy] = sy;

    beta[dy * 2    ] = 1.f - fy;
    beta[dy * 2 + 1] = fy;
  }

  // loop body
  _rowsbuf = MALLOC(float, 2 * (w + 1));
  rows0 = _rowsbuf;
  rows1 = _rowsbuf + w + 1;

  prev_sy1 = -1;

  for (dy = 0; dy < h; dy++) {
    int sy = yofs[dy];

    if (sy == prev_sy1) {
      // hresize one row
      float* rows1p;
      int dx = 0;
      const float* S1 = pBox_crow(src, 0, sy + 1);
      const float* alphap = alpha;
      float* rows0_old = rows0;
      rows0 = rows1;
      rows1 = rows0_old;
      rows1p = rows1;
#if __ARM_NEON

      for (; dx + 1 < w; dx += 2) {
        int sx = xofs[dx];
        int sxn = xofs[dx + 1];
        const float* S1p = S1 + sx;
        const float* S1np = S1 + sxn;

        float32x4_t _a = vld1q_f32(alphap);
        float32x2_t _S1 = vld1_f32(S1p);
        float32x2_t _S1n = vld1_f32(S1np);

        float32x4_t _S1S1n = vcombine_f32(_S1, _S1n);
        float32x4_t _ms1 = vmulq_f32(_S1S1n, _a);
        float32x2_t _rows1 = vpadd_f32(vget_low_f32(_ms1), vget_high_f32(_ms1));

        vst1_f32(rows1p + dx, _rows1);

        alphap += 4;
      }

#endif // __ARM_NEON

      for (; dx < w; dx++) {
        int sx = xofs[dx];
        const float* S1p = S1 + sx;

        float a0 = alphap[0];
        float a1 = alphap[1];
        rows1p[dx] = S1p[0] * a0 + S1p[1] * a1;

        alphap += 2;
      }
    }
    else {
      // hresize two rows
      const float* S0 = pBox_crow(src, 0, sy);
      const float* S1 = pBox_crow(src, 0, sy + 1);

      const float* alphap = alpha;
      float* rows0p = rows0;
      float* rows1p = rows1;
      int dx = 0;
#if __ARM_NEON

      for (; dx + 1 < w; dx += 2) {
        int sx = xofs[dx];
        int sxn = xofs[dx + 1];
        const float* S0p = S0 + sx;
        const float* S1p = S1 + sx;
        const float* S0np = S0 + sxn;
        const float* S1np = S1 + sxn;

        float32x4_t _a = vld1q_f32(alphap);
        float32x2_t _S0 = vld1_f32(S0p);
        float32x2_t _S1 = vld1_f32(S1p);
        float32x2_t _S0n = vld1_f32(S0np);
        float32x2_t _S1n = vld1_f32(S1np);

        float32x4_t _S0S0n = vcombine_f32(_S0, _S0n);
        float32x4_t _S1S1n = vcombine_f32(_S1, _S1n);
        float32x4_t _ms0 = vmulq_f32(_S0S0n, _a);
        float32x4_t _ms1 = vmulq_f32(_S1S1n, _a);
        float32x2_t _rows0 = vpadd_f32(vget_low_f32(_ms0), vget_high_f32(_ms0));
        float32x2_t _rows1 = vpadd_f32(vget_low_f32(_ms1), vget_high_f32(_ms1));

        vst1_f32(rows0p + dx, _rows0);
        vst1_f32(rows1p + dx, _rows1);

        alphap += 4;
      }

#endif // __ARM_NEON

      for (; dx < w; dx++) {
        int sx = xofs[dx];
        const float* S0p = S0 + sx;
        const float* S1p = S1 + sx;

        float a0 = alphap[0];
        float a1 = alphap[1];
        rows0p[dx] = S0p[0] * a0 + S0p[1] * a1;
        rows1p[dx] = S1p[0] * a0 + S1p[1] * a1;

        alphap += 2;
      }
    }

    prev_sy1 = sy + 1;

    {
    // vresize
    float b0 = beta[0];
    float b1 = beta[1];

    float* rows0p = rows0;
    float* rows1p = rows1;
    float* Dp = pBox_crow(dst, 0, dy);

#if __ARM_NEON
    int nn = w >> 3;
#else
    int nn = 0;
#endif
    int remain = w - (nn << 3);

#if __ARM_NEON
    float32x4_t _b0 = vdupq_n_f32(b0);
    float32x4_t _b1 = vdupq_n_f32(b1);

    for (; nn > 0; nn--) {
      float32x4_t _rows0 = vld1q_f32(rows0p);
      float32x4_t _rows1 = vld1q_f32(rows1p);

      float32x4_t _D = vmulq_f32(_rows0, _b0);
      _D = vmlaq_f32(_D, _rows1, _b1);

      vst1q_f32(Dp, _D);

      float32x4_t _rows0n = vld1q_f32(rows0p + 4);
      float32x4_t _rows1n = vld1q_f32(rows1p + 4);

      float32x4_t _Dn = vmulq_f32(_rows0n, _b0);
      _Dn = vmlaq_f32(_Dn, _rows1n, _b1);

      vst1q_f32(Dp + 4, _Dn);

      Dp += 8;
      rows0p += 8;
      rows1p += 8;
    }

#endif // __ARM_NEON

    for (; remain; --remain) {
      //             D[x] = rows0[x]*b0 + rows1[x]*b1;
      *Dp++ = *rows0p++ * b0 + *rows1p++ * b1;
    }


    }
    beta += 2;
  }

  FREE(_rowsbuf);
  FREE(buf);
  return dst;
}

static img_t* resize_bilinear(buf_t* bf, const img_t* src, img_t* dst, int w, int h)
{
  int channels, q;
  if (w == src->w && h == src->h) {
    return cvCloneMat(dst, src);
  }
  
  channels = pBox_channels(src);
  
  bf_pBoxsetsize(bf, dst, h, w, channels, 1);
  
  if (imempty(dst)) {
    return dst;
  }
  
  // unroll image channel
#pragma omp parallel for
  
  for (q = 0; q < channels; q++) {
    const img_t m = pBox_channel(src, q);
    img_t resizem = pBox_channel(dst, q);
    
    resize_bilinear_image(&m, &resizem, w, h);
  }
  
  return dst;
}

