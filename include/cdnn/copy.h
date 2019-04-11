
#if 0
inline bool is_a_ge_zero_and_a_lt_b(int a, int b)
{
	return static_cast<unsigned>(a) < static_cast<unsigned>(b);
}
#else
#define is_a_ge_zero_and_a_lt_b(a, b)   (static_cast<unsigned>(a) < static_cast<unsigned>(b))
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


//#define blas_sset     cpu_caffe_set
void FUN(caffe_set)(const int N, const Stype alpha, Dtype* Y)
{
	if (alpha == 0) {
		memset(Y, 0, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
	}
	else {
		Dtype alpha_ = (Dtype)alpha;
		for (int i = 0; i < N; ++i) {
			Y[i] = alpha_;
		}
	}
}

#define setzero(data)  memset(data, 0, sizeof(*(data)))


// mat process
static void FUN(copy_make_border_image)(const Dtype* src, Dtype* dst, int src_w, int src_h, int dst_w, int dst_h, int top, int left, enum CBorderTypes type, Dtype v)
{
  const Dtype* ptr = (src);
  Dtype* outptr = (dst);

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

static Dtype* FUN(copy_cut_border_image)(const Dtype* src, Dtype* dst, int src_w, int src_h, int dst_w, int dst_h, int top, int left)
{
  int x, y;

  const Dtype* ptr = src + left + src_w * top;
  Dtype* outptr = (dst);

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
