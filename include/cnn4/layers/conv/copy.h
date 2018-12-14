
//! Various border types, image boundaries are denoted with `|`
//! @see borderInterpolate, copyMakeBorder
enum CBorderTypes {
  CC_BORDER_CONSTANT = 0, //!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`
  CC_BORDER_REPLICATE = 1, //!< `aaaaaa|abcdefgh|hhhhhhh`
  CC_BORDER_REFLECT = 2, //!< `fedcba|abcdefgh|hgfedcb`
  CC_BORDER_WRAP = 3, //!< `cdefgh|abcdefgh|abcdefg`
  CC_BORDER_REFLECT_101 = 4, //!< `gfedcb|abcdefgh|gfedcba`
  CC_BORDER_TRANSPARENT = 5, //!< `uvwxyz|absdefgh|ijklmno`

  CC_BORDER_REFLECT101 = CC_BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
  CC_BORDER_DEFAULT = CC_BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
  CC_BORDER_ISOLATED = 16 //!< do not look outside of ROI
};

#if 0
#include "ThreadPool.inl"
#define DISPATCH_WORKER_BEGIN(n) { int _number = n; auto _worker = [&](int start, int stop)
#define DISPATCH_WORKER_END ; dispatch_worker(_worker, _number); }
#else
#define set_thread_num(x)
#define DISPATCH_WORKER_BEGIN(n) { int start = 0, stop = n;
#define DISPATCH_WORKER_END ; }
#endif


#define blas_sset cpu_caffe_set

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
  if (*pdst != (char*)src) {
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
#define setzero(data)  memset(data, 0, sizeof(*(data)))


// mat process
static void copy_make_border_image(const float* src, float* dst, int src_w, int src_h, int dst_w, int dst_h, int top, int left, enum CBorderTypes type, float v)
{
  const float* ptr = (src);
  float* outptr = (dst);

  if (type == CC_BORDER_CONSTANT) {
    int y = 0;

    // fill top
    for (; y < top; y++) {
      int x = 0;

      for (; x < dst_w; x++) {
        outptr[x] = v;
      }

      outptr += dst_w;
    }

    // fill center
    for (; y < (top + src_h); y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = v;
      }

      for (; x < (left + src_w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < dst_w; x++) {
        outptr[x] = v;
      }

      ptr += src_w;
      outptr += dst_w;
    }

    // fill bottom
    for (; y < dst_h; y++) {
      int x = 0;

      for (; x < dst_w; x++) {
        outptr[x] = v;
      }

      outptr += dst_w;
    }
  }
  else if (type == CC_BORDER_REPLICATE) {
    int y = 0;

    // fill top
    for (; y < top; y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src_w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < dst_w; x++) {
        outptr[x] = ptr[src_w - 1];
      }

      outptr += dst_w;
    }

    // fill center
    for (; y < (top + src_h); y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src_w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < dst_w; x++) {
        outptr[x] = ptr[src_w - 1];
      }

      ptr += src_w;
      outptr += dst_w;
    }

    // fill bottom
    ptr -= src_w;

    for (; y < dst_h; y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src_w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < dst_w; x++) {
        outptr[x] = ptr[src_w - 1];
      }

      outptr += dst_w;
    }
  }
}

static float* copy_cut_border_image(const float* src, float* dst, int src_w, int src_h, int dst_w, int dst_h, int top, int left)
{
  int x, y;

  const float* ptr = src + left + src_w * top;
  float* outptr = (dst);

  for (y = 0; y < dst_h; y++) {
    for (x = 0; x < dst_w; x++) {
      outptr[x] = ptr[x];
    }

    outptr += dst_w;
    ptr += src_w;
  }

  return dst;
}

void FUN(rev)(int count, Dtype* f) {
  Dtype* e = f + count;
  for (; f < e--; ++f) {
    Dtype t1 = *f;
    Dtype t2 = *e;
    *e = t1;
    *f = t2;
  }
}
void FUN(rev2d)(int N, int M, Dtype* f) {
  int i;
  for (i = 0; i < N; ++i) {
    FUN(rev)(M, f);
    f += M;
  }
}
