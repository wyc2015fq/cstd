
#include "copy.h"

static void sfill(int n, float* data, float x)
{
  int i = 0;
  if (0 == x) {
    memset(data, 0, n * sizeof(*data));
    return;
  }

  for (; i < n - 3; i += 4) {
    data[i] = x;
    data[i + 1] = x;
    data[i + 2] = x;
    data[i + 3] = x;
  }

  for (; i < n; i++) {
    data[i] = x;
  }
}
static int sconv1d_full(int nx, const float* x, int nk, const float* ker, float* y) {
  int i, j;
  for (i = 0; i<nx; ++i) {
    float* y1 = y + i;
    for (j = 0; j<nk; ++j) {
      y1[j] += x[i] * ker[j];
    }
  }
  return 0;
}

static int sconv1d_valid(int nx, const float* x, int nk, const float* ker, float* y) {
  int i, j, ny = nx - nk + 1;
  for (i = 0; i<ny; ++i) {
    const float* x1 = x + i;
    for (j = 0; j<nk; ++j) {
      y[i] += x1[j] * ker[j];
    }
  }
  return 0;
}

static void conv_f(float* dst, int dst_step, int dst_w, int dst_h, const float* weight_data, int map_w, int map_h, const float* src, int src_step, int inv) {
  int i, j, r, c;
  if (inv) {
    for (j = 0; j<dst_h; j++) {
      float* dst1 = dst + j*dst_step;
      for (i = 0; i<dst_w; i++) {
        float sum = 0;
        const float* map1 = weight_data + map_w*map_h;
        for (r = 0; r<map_h; r++) {
          const float* src1 = src + (j + r)*src_step + i;
          for (c = 0; c<map_w; c++) {
            sum += *(--map1) *src1[c];
          }
        }
        dst1[i] += sum;
      }
    }
  }
  else {
    for (j = 0; j<dst_h; j++) {
      float* dst1 = dst + j*dst_step;
      for (i = 0; i<dst_w; i++) {
        float sum = 0;
        const float* map1 = weight_data;
        for (r = 0; r<map_h; r++) {
          const float* src1 = src + (j + r)*src_step + i;
          for (c = 0; c<map_w; c++) {
            dst1[i] += (*map1++) *src1[c];
          }
        }
        dst1[i] += sum;
      }
    }

  }
}

#include "conv_1x1.h"
#include "conv_2x2.h"
#include "conv_3x3b.h"
#include "conv_5x5.h"

#define CONV3X3S1_NEON_ARGS(args1, j, OPT_DEF) \
	OPT_DEF(args1, j, const Blob*, inData) \
	OPT_DEF(args1, j, Blob*, next) \
	OPT_DEF(args1, j, const float*, _kernel) \
	OPT_DEF(args1, j, const float*, _bias) \

static int conv2d_fast_valid(int h, int w, const float* src, int src_step, int srccn, float* dst, int dst_step, int dstcn,
  DataSize kernelSize, const float* kernelData, const float* bias, DataSize strideSize, DataSize dilationSize) {
  typedef void(*sconv_func)(int h, int w, const float* src, int src_step, int srccn, float* dst, int dst_step, int dstcn, const float* kernelData, const float* bias);
  sconv_func conv = NULL;
  int outh = h - kernelSize.h + 1;
  int outw = w - kernelSize.w + 1;
  // kernel_size x stride
  if (kernelSize.h == kernelSize.w && strideSize.h == strideSize.w && kernelSize.h<8 && strideSize.h<4) {
    static sconv_func conv_func_table[8][4] = { 0 };
    static int inited = 0;
    int kernel_size = kernelSize.w;
    int stride = strideSize.w;

    if (!inited) {
      inited = 1;
#define CONV_DEF_SET(kernel_size, stride)    conv_func_table[kernel_size-1][stride-1] = sconv ## kernel_size ## x ## kernel_size ## s ## stride
      CONV_DEF_SET(1, 1);
      CONV_DEF_SET(2, 1);
      CONV_DEF_SET(3, 1);
      CONV_DEF_SET(5, 1);
#undef CONV_DEF_SET
      //conv_func_table[3 - 1][1 - 1] = conv3x3s1_neon_thd;
    }

    conv = conv_func_table[kernel_size - 1][stride - 1];
    //printf(" kernel_size %d stride %d", kernel_size, stride);
  }

  if (conv) {
    conv(h, w, src, src_step, srccn, dst, dst_step, dstcn, kernelData, bias);
  }
  else {
    //#include "x86/convolution_5x5.h"
    const int maxk = kernelSize.h * kernelSize.w;
    int i, j, k, p, q;
    // kernel offsets
    int* space_ofs = NULL;
    const float* weight_data_ptr = kernelData;
    MYREALLOC(space_ofs, maxk);
    {
      int p1 = 0;
      int p2 = 0;
      int gap = w * dilationSize.w - kernelSize.w * dilationSize.w;

      for (i = 0; i < kernelSize.h; i++) {
        for (j = 0; j < kernelSize.w; j++) {
          space_ofs[p1] = p2;
          p1++;
          p2 += dilationSize.w;
        }

        p2 += gap;
      }
    }

    // num_next
#pragma omp parallel for
    for (p = 0; p < dstcn; p++) {
      float* outptr = dst + p * outh*dst_step;

      for (i = 0; i < outh; i++) {
        for (j = 0; j < outw; j++) {
          float sum = 0.f;
          const float* kptr;

          if (bias) {
            sum = bias[p];
          }

          kptr = weight_data_ptr + maxk * srccn * p;

          // channels
          for (q = 0; q < srccn; q++) {
            const float* sptr = src + (q * h + i * strideSize.h)*src_step + j * strideSize.w;

            for (k = 0; k < maxk; k++) { // 29.23
              float val = sptr[space_ofs[k]]; // 20.72
              float w = kptr[k];
              sum += val * w; // 41.45
            }

            kptr += maxk;
          }

          outptr[j] = sum;
        }

        outptr += outw;
      }
    }

    FREE(space_ofs);
  }
  return 0;
}


static float* copy_make_border(const float* src, float** pdst, DataSize srcSize, DataSize* pdstSize,
  IRECT pad, enum CBorderTypes type, float v)
{
  DataSize dstSize = dataSize(srcSize.n, srcSize.c, srcSize.h + pad.t + pad.b, srcSize.w + pad.l + pad.r);;
  //dstSize.set(srcSize.n, srcSize.c, srcSize.h + pad.t + pad.b, srcSize.w + pad.l + pad.r );
  int i;
  int srcCount = Data_2DSize(srcSize);
  int dstCount = Data_2DSize(dstSize);
  if (pdstSize) {
    *pdstSize = dstSize;
  }

  if (dstSize.w == srcSize.w && dstSize.h == srcSize.h) {
    clone(pdst, src, Data_total(dstSize));
    return *pdst;
  }

  resetArr(pdst, Data_total(dstSize));
  // unroll image channel
#pragma omp parallel for
  for (i = 0; i < srcSize.c; i++) {
    copy_make_border_image(src + i*srcCount, *pdst + i*dstCount, srcSize.w, srcSize.h, dstSize.w, dstSize.h, pad.t, pad.l, type, v);
  }

  return *pdst;
}

static float* copy_cut_border(const float* src, float** pdst, DataSize srcSize, DataSize* pdstSize, int top, int bottom, int left, int right)
{
  DataSize dstSize = dataSize(srcSize.n, srcSize.c, srcSize.h - top - bottom, srcSize.w - left - right);
  int i, n = srcSize.n*srcSize.c;
  int srcCount = Data_2DSize(srcSize);
  int dstCount = Data_2DSize(dstSize);
  if (pdstSize) {
    *pdstSize = dstSize;
  }

  if (dstSize.w == srcSize.w && dstSize.h == srcSize.h) {
    clone(pdst, src, Data_total(srcSize));
    return *pdst;
  }
  resetArr(pdst, Data_total(dstSize));

  // unroll image channel
#pragma omp parallel for
  for (i = 0; i < n; i++) {
    copy_cut_border_image(src + i*srcCount, *pdst + i*dstCount, srcSize.w, srcSize.h, dstSize.w, dstSize.h, top, left);
  }

  return *pdst;
}

static void my_conv2d_fast(const Dtype* inData, float* outData, const Dtype* weights, const Dtype* biasData,
  DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
  int dilation_h, int dilation_w, int pad_h, int pad_w, int group_, bool cross_correlation) 
//static float* conv2d_fast(const float* inData, const float* kernelData, const float* bias, float* outData,
//  DataShape inSize, DataShape kernelSize, DataShape outSize, DataShape strideSize, DataShape dilationSize, )
{
  // convolv with NxN kernel
  // value = value + bias
  const float* kernelData = weights;
  float* srcex = NULL;
  Dtype* inv_kernel = NULL;
  int i, num = inSize.n;
  IRECT pad = iRECT(pad_w, pad_h, pad_w, pad_h);
  //IRECT pad = mode2pad(inSize, kernelSize, strideSize, dilationSize, mode);
  ASSERT(inSize.n == outSize.n);
  DataSize kernelSize = dataSize(outSize.c, inSize.c, kernel_h, kernel_w);
  DataSize strideSize = dataSize(1, 1, stride_h, stride_w);
  DataSize dilationSize = dataSize(1, 1, dilation_h, dilation_w);
  //ASSERT(inSize.c == kernelSize.c);
  //ASSERT(kernelSize.n == outSize.c);
  ASSERT(inSize.c>0);
  //strideSize.h = MAX(1, strideSize.h);
  //strideSize.w = MAX(1, strideSize.w);
  //dilationSize = MAX(1, dilationSize);

  if (!cross_correlation) {
    int n = outSize.c*inSize.c, m = kernel_h*kernel_w;
    inv_kernel = (Dtype*)caffe_malloc(n*m * sizeof(Dtype));
    memcpy(inv_kernel, weights, n*m * sizeof(Dtype));
    FUN(rev2d)(n, m, inv_kernel);
    weights = inv_kernel;
  }
  kernelData = weights;
  //     fprintf(stderr, "Convolution inData %d x %d  pad = %d  ksize=%d  stride=%d\n", w, h, pad, kernel_size, stride);
  //int dst_w = (srcSize.w - kernelSize.extent_w) / strideSize.w + 1;
  //int dst_h = (srcSize.h - kernelSize.extent_h) / strideSize.h + 1;

  DataSize inSize1 = dataSize(inSize.n, inSize.c, inSize.h, inSize.w);
  for (i = 0; i<inSize.n; ++i) {
    DataSize srcSize = dataSize(1, inSize.c, inSize.h, inSize.w);
    DataSize dstSize = dataSize(1, outSize.c, outSize.h, outSize.w);
    float* dst = outData + i * Data_3DSize(outSize);
    const float* src = inData + i * Data_3DSize(inSize);
    if (pad.l || pad.t || pad.r || pad.b) {
      src = copy_make_border(src, &srcex, inSize1, &srcSize, pad, CC_BORDER_CONSTANT, 0.f);
    }
    conv2d_fast_valid(srcSize.h, srcSize.w, src, srcSize.w, srcSize.c, dst, dstSize.w, kernelSize.n, kernelSize,
      kernelData, biasData, strideSize, dilationSize);
  }
  if (srcex) {
    free(srcex);
  }
  if (inv_kernel) { caffe_free(inv_kernel); }
  return ;
}

#if 0
//#include "arm/convolution_5x5.h"
static float* conv2d_forward(const float* inData, const float* kernelData, const float* bias, float* outData,
  DataShape inSize, DataShape kernelSize, DataShape outSize,
  DataShape strideSize, DataShape dilationSize, enum PAD_MODE mode) {
  if (0) {
    //conv2d_fast(inData, kernelData, bias, outData, inSize, kernelSize, outSize, strideSize, dilationSize, mode);
  }

  if (0) {
    ASSERT(inSize.n == outSize.n);
    ASSERT(inSize.c == kernelSize.c);
    ASSERT(kernelSize.n == outSize.c);
    if (mode == PAD_VALID) {
      // VALID
      //const DataShape inSize = dataSize(in, ic, iw, ih);
      //const DataShape kernelSize = dataSize(kn, ic, kw, kh);
      //const DataShape outSize = dataSize(in, kn, ow, oh);
      int in = inSize.n, ic = inSize.c, iw = inSize.w, ih = inSize.h,
        kn = kernelSize.n, kw = kernelSize.w, kh = kernelSize.h, ow = outSize.w, oh = outSize.h;
      int nn, nc, nh, nw, kc, nextIdx;
      ASSERT(1 == dilationSize.w && 1 == dilationSize.h);
      for (nn = 0; nn < in; nn++) {
        for (nc = 0; nc < outSize.c; nc++) {
          for (nh = 0; nh < outSize.h; nh++) {
            for (nw = 0; nw < outSize.w; nw++) {
              int inStartX = nw * strideSize.w;
              int inStartY = nh * strideSize.h;
              float sum = bias ? bias[nc] : 0.f;
              for (kc = 0; kc < kernelSize.c; kc++) {
                for (kh = 0; kh < kernelSize.h; kh++) {
                  for (kw = 0; kw < kernelSize.w; kw++) {
                    int prevIdx = Data_4DIndex(inSize, nn, kc, inStartY + kh, inStartX + kw);
                    int kernelIdx = Data_4DIndex(kernelSize, nc, kc, kh, kw);
                    sum += inData[prevIdx] * kernelData[kernelIdx];
                  }
                }
              }
              nextIdx = Data_4DIndex(outSize, nn, nc, nh, nw);
              outData[nextIdx] = sum;
            }
          }
        }
      }
    }
    else if (mode == PAD_SAME) {
      int nn, nc, nh, nw, kc, nextIdx;
      // SAME
      //const DataShape inSize = dataSize(in, ic, iw, ih);
      //const DataShape kernelSize = dataSize(kn, ic, kw, kh);
      //const DataShape outSize = dataSize(in, kn, ow, oh);
      int in = inSize.n, ic = inSize.c, iw = inSize.w, ih = inSize.h,
        kn = kernelSize.n, kw = kernelSize.w, kh = kernelSize.h, ow = outSize.w, oh = outSize.h;
      ASSERT(inSize.n == outSize.n);
      ASSERT(inSize.c == kernelSize.c);
      ASSERT(kernelSize.n == outSize.c);
      ASSERT(1 == dilationSize.w && 1 == dilationSize.h);
      for (nn = 0; nn < in; nn++) {
        for (nc = 0; nc < outSize.c; nc++) {
          for (nh = 0; nh < outSize.h; nh++) {
            for (nw = 0; nw < outSize.w; nw++) {
              const int inStartX = nw - kw / 2;
              const int inStartY = nh - kh / 2;
              float sum = 0;
              for (kc = 0; kc < kernelSize.c; kc++) {
                for (kh = 0; kh < kernelSize.h; kh++) {
                  for (kw = 0; kw < kernelSize.w; kw++) {
                    const int inY = inStartY + kh;
                    const int inX = inStartX + kw;
                    if (inY >= 0 && inY < (int)inSize.h && inX >= 0 && inX < (int)inSize.w) {
                      int prevIdx = Data_4DIndex(inSize, nn, kc, inY, inX);
                      int kernelIdx = Data_4DIndex(kernelSize, nc, kc, kh, kw);
                      sum += inData[prevIdx] * kernelData[kernelIdx];
                    }
                  }
                }
              }
              nextIdx = Data_4DIndex(outSize, nn, nc, nh, nw);
              if (bias) {
                int biasIdx = nc;
                sum += bias[biasIdx];
              }
              outData[nextIdx] = sum;
            }
          }
        }
      }
    }
  }
  return outData;
}

static void conv2d_backward(const float* nextDiff, const float* kernelData, float* prevDiff,
  DataShape inSize, DataShape kernelSize, DataShape outSize,
  DataShape strideSize, DataShape dilationSize, enum PAD_MODE mode) {
  if (1) {
    int nn, nc, nh, nw, kc, kh, kw;
    DISPATCH_WORKER_BEGIN(inSize.n) {
      for (nn = start; nn < stop; nn++) {
        for (nc = 0; nc < outSize.c; nc++) {
          for (nh = 0; nh < outSize.h; nh++) {
            for (nw = 0; nw < outSize.w; nw++) {
              const int inStartX = nw * strideSize.w;
              const int inStartY = nh * strideSize.h;
              const int nextDiffIdx = Data_4DIndex(outSize, nn, nc, nh, nw);
              const int kn = nc;
              for (kc = 0; kc < kernelSize.c; kc++) {
                for (kh = 0; kh < kernelSize.h; kh++) {
                  for (kw = 0; kw < kernelSize.w; kw++) {
                    const int inY = inStartY + kh;
                    const int inX = inStartX + kw;
                    if (inY >= 0 && inY < inSize.h && inX >= 0 && inX < inSize.w) {
                      const int prevDiffIdx = Data_4DIndex(inSize, nn, kc, inY, inX);
                      const int kernelIdx = Data_4DIndex(kernelSize, kn, kc, kh, kw);
                      prevDiff[prevDiffIdx] += kernelData[kernelIdx] * nextDiff[nextDiffIdx];
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    DISPATCH_WORKER_END;
  }
}

static void conv2d_backward_weight(const float* inData, const float* nextDiff, float* kernelDiff,
  DataShape inSize, DataShape kernelSize, DataShape outSize,
  DataShape strideSize, DataShape dilationSize, enum PAD_MODE mode) {

  if (1) {
    int nn, nc, nh, nw, kc, kh, kw;
    //update kernel gradient
    for (nn = 0; nn < outSize.n; nn++) {
      for (nc = 0; nc < outSize.c; nc++) {
        for (nh = 0; nh < outSize.h; nh++) {
          for (nw = 0; nw < outSize.w; nw++) {
            const int inStartX = nw * strideSize.w;
            const int inStartY = nh * strideSize.h;
            const int nextDiffIdx = Data_4DIndex(outSize, nn, nc, nh, nw);
            const int kn = nc;
            for (kc = 0; kc < kernelSize.c; kc++) {
              for (kh = 0; kh < kernelSize.h; kh++) {
                for (kw = 0; kw < kernelSize.w; kw++) {
                  const int inY = inStartY + kh;
                  const int inX = inStartX + kw;
                  if (inY >= 0 && inY < inSize.h && inX >= 0 && inX < inSize.w) {
                    const int kernelIdx = Data_4DIndex(kernelSize, kn, kc, kh, kw);
                    const int prevIdx = Data_4DIndex(inSize, nn, kc, inY, inX);
                    kernelDiff[kernelIdx] += inData[prevIdx] * nextDiff[nextDiffIdx];
                  }
                }
              }
            }
          }
        }
      }
    }
    //div by batch size
  }
}

static void conv2d_backward_bias(float* biasDiff, const float* nextDiff, DataShape outSize) {
  if (1) {
    int nn, nc, nh, nw;
    for (nn = 0; nn < outSize.n; nn++) {
      for (nc = 0; nc < outSize.c; nc++) {
        const int biasIdx = nc;
        for (nh = 0; nh < outSize.h; nh++) {
          for (nw = 0; nw < outSize.w; nw++) {
            const int nextDiffIdx = Data_4DIndex(outSize, nn, nc, nh, nw);
            biasDiff[biasIdx] += nextDiff[nextDiffIdx];
          }
        }
      }
    }
  }
  //div by batch size
}
#endif

