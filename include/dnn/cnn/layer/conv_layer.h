
static void blas_sconv2d(const float* inData, float* outData, const float* weights, const float* biasData,
                  DataSize inSize, DataSize outSize, DataSize kernelSize, DataSize strideSize,
                  DataSize dilationSize,IRECT pad,int groups DEFAULT(1)) {
  // Groups
  int o_c = outSize.c;
  int o_g = outSize.c / groups;
  int k_g = inSize.c / groups;
  int o_head, k_head;
  int pad_h = pad.t+pad.b;
  int pad_w = pad.l+pad.r;
  int n, g, o, k, y, x, p, q;
  // Convolution
  for (n = 0; n < outSize.n; n++) {
    for (g = 0; g < groups; g++) {
      o_head = o_g * g;
      k_head = k_g * g;
      for (o = 0; o < o_g; o++) {
        for (k = 0; k < k_g; k++) {
          for (y = 0; y < outSize.h; y++) {
            for (x = 0; x < outSize.w; x++) {
              int out_offset = ((n*o_g+o + o_head)*outSize.h+y)*outSize.w+x;
              for (p = 0; p < kernelSize.h; p++) {
                for (q = 0; q < kernelSize.w; q++) {
                  int in_y = y * strideSize.h - pad_h + p * dilationSize.h;
                  int in_x = x * strideSize.w - pad_w + q * dilationSize.w;
                  if (is_a_ge_zero_and_a_lt_b(in_y, inSize.h) && is_a_ge_zero_and_a_lt_b(in_x, inSize.w)) {
                    int weight_offset = (((o + o_head)*k_g+k)*kernelSize.h+p)*kernelSize.w+q;
                    int in_offset = ((n*k_g+ k + k_head)*inSize.h+in_y)*inSize.w+in_x;
                    outData[out_offset] += inData[in_offset] * weights[weight_offset];
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  // Bias
  if (biasData) {
    for (n = 0; n < outSize.n; n++) {
      for (o = 0; o < o_c; o++) {
        for (y = 0; y < outSize.h; y++) {
          for (x = 0; x < outSize.w; x++) {
            int out_offset = ((n*o_c+o)*outSize.h+y)*outSize.w+x;
            outData[out_offset] += biasData[o];
          }
        }
      }
    }
  }
}


static int sconv1d_full(int nx, const float* x, int nk, const float* ker, float* y) {
  int i, j;
  for (i=0; i<nx; ++i) {
    float* y1 = y+i;
    for (j=0; j<nk; ++j) {
      y1[j] += x[i] * ker[j];
    }
  }
  return 0;
}

static int sconv1d_valid(int nx, const float* x, int nk, const float* ker, float* y) {
  int i, j, ny = nx-nk+1;
  for (i=0; i<ny; ++i) {
    const float* x1 = x+i;
    for (j=0; j<nk; ++j) {
      y[i] += x1[j] * ker[j];
    }
  }
  return 0;
}

static void conv_f(float* dst, int dst_step, int dst_w, int dst_h, const float* weight_data, int map_w, int map_h, const float* src, int src_step, int inv) {
  int i, j, r, c;
  if (inv) {
    for(j=0;j<dst_h;j++) {
      float* dst1 = dst + j*dst_step;
      for(i=0;i<dst_w;i++) {
        float sum=0;
        const float* map1 = weight_data+map_w*map_h;
        for(r=0;r<map_h;r++) {
          const float* src1 = src + (j+r)*src_step + i;
          for(c=0;c<map_w;c++) {
            sum+=*(--map1) *src1[c];
          }
        }
        dst1[i] += sum;
      }
    }
  } else {
    for(j=0;j<dst_h;j++) {
      float* dst1 = dst + j*dst_step;
      for(i=0;i<dst_w;i++) {
        float sum=0;
        const float* map1 = weight_data;
        for(r=0;r<map_h;r++) {
          const float* src1 = src + (j+r)*src_step + i;
          for(c=0;c<map_w;c++) {
            dst1[i]+=(*map1++) *src1[c];
          }
        }
        dst1[i] += sum;
      }
    }
    
  }
}

#include "conv_1x1.h"
#include "conv_2x2.h"
#include "conv_3x3.h"
#include "conv_5x5.h"

#define CONV3X3S1_NEON_ARGS(args1, j, OPT_DEF) \
	OPT_DEF(args1, j, const Blob*, prevData) \
	OPT_DEF(args1, j, Blob*, next) \
	OPT_DEF(args1, j, const float*, _kernel) \
	OPT_DEF(args1, j, const float*, _bias) \

static int conv2d_fast_valid(int h, int w, const float* src, int src_step,int srccn, float* dst, int dst_step, int dstcn,
 DataSize kernelSize, const float* kernelData, const float* bias, DataSize strideSize, DataSize dilationSize) {
  typedef void (*sconv_func)(int h, int w, const float* src, int src_step, int srccn, float* dst, int dst_step, int dstcn, const float* kernelData, const float* bias);
  sconv_func conv = NULL;
  int outh = h - kernelSize.h + 1;
  int outw = w - kernelSize.w + 1;
  // kernel_size x stride
  if (kernelSize.h==kernelSize.w && strideSize.h==strideSize.w) {
    static sconv_func conv_func_table[8][4] = {0};
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
              float val = sptr[ space_ofs[k] ]; // 20.72
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

enum PAD_MODE {
  PAD_SAME,
  PAD_VALID,
};

static IRECT mode2pad(DataSize prevSize, DataSize kernelSize, DataSize strideSize, DataSize dilationSize, enum PAD_MODE mode) {
  IRECT pad={0};
  if (mode==PAD_SAME) {
    const int kernel_extent_h = dilationSize.h * (kernelSize.h - 1) + 1;
    const int kernel_extent_w = dilationSize.w * (kernelSize.w - 1) + 1;
    int pad_w = kernel_extent_h + (prevSize.w - 1) / strideSize.w * strideSize.w - prevSize.w;
    int pad_h = kernel_extent_w + (prevSize.h - 1) / strideSize.h * strideSize.h - prevSize.h;
    
    if (pad_w > 0 || pad_h > 0) {
      pad = iRECT(pad_w / 2, pad_h / 2, pad_w - pad_w / 2, pad_h - pad_h / 2);
    }
  }
  return pad;
}
static float* copy_make_border(const float* src, float** pdst, DataSize srcSize, DataSize* pdstSize,
                               IRECT pad, enum CBorderTypes type, float v)
{
	DataSize dstSize = { srcSize.n, srcSize.c, srcSize.h + pad.t + pad.b, srcSize.w + pad.l + pad.r };
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

static float* conv2d_fast(const float* prevData, const float* kernelData, const float* bias, float* nextData,
                          DataSize prevSize, DataSize kernelSize, DataSize nextSize,
                          DataSize strideSize, DataSize dilationSize, enum PAD_MODE mode) {
  // convolv with NxN kernel
  // value = value + bias
  float* srcex = NULL;
  int i, num = prevSize.n;
  IRECT pad=mode2pad(prevSize, kernelSize, strideSize, dilationSize, mode);
  ASSERT(prevSize.n==nextSize.n);
  ASSERT(prevSize.c==kernelSize.c);
  ASSERT(kernelSize.n==nextSize.c);
  ASSERT(prevSize.c>0);
  strideSize.h = MAX(1, strideSize.h);
  strideSize.w = MAX(1, strideSize.w);
  //dilationSize = MAX(1, dilationSize);
  
  //     fprintf(stderr, "Convolution prevData %d x %d  pad = %d  ksize=%d  stride=%d\n", w, h, pad, kernel_size, stride);
  //int dst_w = (srcSize.w - kernelSize.extent_w) / strideSize.w + 1;
  //int dst_h = (srcSize.h - kernelSize.extent_h) / strideSize.h + 1;
  
  for (i=0; i<prevSize.n; ++i) {
    DataSize srcSize = dataSize(1, prevSize.c, prevSize.h, prevSize.w);
    DataSize dstSize = dataSize(1, nextSize.c, nextSize.h, nextSize.w);
    float* dst = nextData + i * Data_3DSize(nextSize);
    const float* src = prevData + i * Data_3DSize(prevSize);
    if (pad.l || pad.t || pad.r || pad.b) {
      src = copy_make_border(src, &srcex, prevSize, &srcSize, pad, CC_BORDER_CONSTANT, 0.f);
    }
    conv2d_fast_valid(srcSize.h, srcSize.w, src, srcSize.w, srcSize.c, dst, dstSize.w, kernelSize.n, kernelSize,
      kernelData, bias, strideSize, dilationSize);
  }
  if (srcex) {
    free(srcex);
  }
  return nextData;
}
//#include "arm/convolution_5x5.h"
static float* conv2d_forward(const float* prevData, const float* kernelData, const float* bias, float* nextData,
                         DataSize prevSize, DataSize kernelSize, DataSize nextSize,
                         DataSize strideSize, DataSize dilationSize, enum PAD_MODE mode) {
  if (1) {
    IRECT pad = mode2pad(prevSize, kernelSize, strideSize, dilationSize, mode);
    int is_1x1_ = kernelSize.h == 1 && kernelSize.w == 1 && strideSize.h == 1 && strideSize.w == 1 && (mode==PAD_VALID);
    float* buffer_ = NULL;
    int next_dim = Data_2DSize(nextSize);
    int kernel_dim = Data_3DSize(kernelSize);
    int i;
    ASSERT(prevSize.n==nextSize.n);
    ASSERT(prevSize.c==kernelSize.c);
    ASSERT(kernelSize.n==nextSize.c);
    
    if (!is_1x1_) {
      MYREALLOC(buffer_, (next_dim * kernel_dim));
    }
    for (i=0; i<prevSize.n; ++i) {
      float* dst = nextData + i * Data_3DSize(nextSize);
      const float* src = prevData + i * Data_3DSize(prevSize);
      const float* col_buff = src;
      if (!is_1x1_) {
        col_buff = sim2col(src, buffer_, prevSize, kernelSize, strideSize, pad, dilationSize);
      }
      blas_sgemm(CblasNoTrans, CblasNoTrans, nextSize.c, next_dim, kernel_dim, 1.f, kernelData, col_buff, 0.f, dst);
    }
    if (bias) {
      float* bias_multiplier = NULL;
      MYREALLOC(buffer_, next_dim);
      bias_multiplier = buffer_;
      blas_sset(next_dim, 1, bias_multiplier);
      for (i=0; i<prevSize.n; ++i) {
        float* dst = nextData + i * Data_3DSize(nextSize);
        blas_sgemm(CblasNoTrans, CblasNoTrans, nextSize.c, next_dim, 1, (float)1., bias, bias_multiplier, (float)1., dst);
      }
    }
    FREE(buffer_);
  }
  if (0) {
    conv2d_fast(prevData, kernelData, bias, nextData, prevSize, kernelSize, nextSize, strideSize, dilationSize, mode);
  }
  
  if (0) {
    ASSERT(prevSize.n==nextSize.n);
    ASSERT(prevSize.c==kernelSize.c);
    ASSERT(kernelSize.n==nextSize.c);
    if (mode == PAD_VALID) {
      // VALID
      //const DataSize prevSize = dataSize(in, ic, iw, ih);
      //const DataSize kernelSize = dataSize(kn, ic, kw, kh);
      //const DataSize nextSize = dataSize(in, kn, ow, oh);
      int in=prevSize.n, ic=prevSize.c, iw=prevSize.w, ih=prevSize.h,
        kn=kernelSize.n, kw=kernelSize.w, kh=kernelSize.h, ow=nextSize.w, oh=nextSize.h;
      int nn, nc, nh, nw, kc, nextIdx;
      ASSERT(1==dilationSize.w && 1==dilationSize.h);
      for (nn = 0; nn < in; nn++) {
        for (nc = 0; nc < nextSize.c; nc++) {
          for (nh = 0; nh < nextSize.h; nh++) {
            for (nw = 0; nw < nextSize.w; nw++) {
              int inStartX = nw * strideSize.w;
              int inStartY = nh * strideSize.h;
              float sum = bias ? bias[nc] : 0.f;
              for (kc = 0; kc < kernelSize.c; kc++) {
                for (kh = 0; kh < kernelSize.h; kh++) {
                  for (kw = 0; kw < kernelSize.w; kw++) {
                    int prevIdx = Data_4DIndex(prevSize, nn, kc, inStartY + kh, inStartX + kw);
                    int kernelIdx = Data_4DIndex(kernelSize, nc, kc, kh, kw);
                    sum += prevData[prevIdx] * kernelData[kernelIdx];
                  }
                }
              }
              nextIdx = Data_4DIndex(nextSize, nn, nc, nh, nw);
              nextData[nextIdx] = sum;
            }
          }
        }
      }
    }
    else if (mode == PAD_SAME) {
      int nn, nc, nh, nw, kc, nextIdx;
      // SAME
      //const DataSize prevSize = dataSize(in, ic, iw, ih);
      //const DataSize kernelSize = dataSize(kn, ic, kw, kh);
      //const DataSize nextSize = dataSize(in, kn, ow, oh);
      int in=prevSize.n, ic=prevSize.c, iw=prevSize.w, ih=prevSize.h,
        kn=kernelSize.n, kw=kernelSize.w, kh=kernelSize.h, ow=nextSize.w, oh=nextSize.h;
      ASSERT(prevSize.n==nextSize.n);
      ASSERT(prevSize.c==kernelSize.c);
      ASSERT(kernelSize.n==nextSize.c);
      ASSERT(1==dilationSize.w && 1==dilationSize.h);
      for (nn = 0; nn < in; nn++) {
        for (nc = 0; nc < nextSize.c; nc++) {
          for (nh = 0; nh < nextSize.h; nh++) {
            for (nw = 0; nw < nextSize.w; nw++) {
              const int inStartX = nw - kw / 2;
              const int inStartY = nh - kh / 2;
              float sum = 0;
              for (kc = 0; kc < kernelSize.c; kc++) {
                for (kh = 0; kh < kernelSize.h; kh++) {
                  for (kw = 0; kw < kernelSize.w; kw++) {
                    const int inY = inStartY + kh;
                    const int inX = inStartX + kw;
                    if (inY >= 0 && inY < (int)prevSize.h && inX >= 0 && inX < (int)prevSize.w) {
                      int prevIdx = Data_4DIndex(prevSize, nn, kc, inY, inX);
                      int kernelIdx = Data_4DIndex(kernelSize, nc, kc, kh, kw);
                      sum += prevData[prevIdx] * kernelData[kernelIdx];
                    }
                  }
                }
              }
              nextIdx = Data_4DIndex(nextSize, nn, nc, nh, nw);
              if (bias) {
                int biasIdx = nc;
                sum += bias[biasIdx];
              }
              nextData[nextIdx] = sum;
            }
          }
        }
      }
    }
  }
  return nextData;
}

static void conv2d_backward(const float* nextDiff, const float* kernelData, float* prevDiff,
                       DataSize prevSize, DataSize kernelSize, DataSize nextSize,
                       DataSize strideSize, DataSize dilationSize, enum PAD_MODE mode) {
  if (1) {
    IRECT pad = mode2pad(prevSize, kernelSize, strideSize, dilationSize, mode);
    int is_1x1_ = kernelSize.h == 1 && kernelSize.w == 1 && strideSize.h == 1 && strideSize.w == 1 && (mode==PAD_VALID);
    float* buffer_ = NULL;
    int next_dim = Data_2DSize(nextSize);
    int kernel_dim = Data_3DSize(kernelSize);
    int i;
    
    if (!is_1x1_) {
      MYREALLOC(buffer_, (next_dim * kernel_dim));
    }
    for (i=0; i<prevSize.n; ++i) {
      const float* dst = nextDiff + i * Data_3DSize(nextSize);
      float* src = prevDiff + i * Data_3DSize(prevSize);
      float* col_buff = is_1x1_ ? src : buffer_;
      //tic;
      blas_sgemm(CblasTrans, CblasNoTrans, kernel_dim, next_dim, nextSize.c, 1.f, kernelData, dst, 0.f, col_buff);
      //toc;
      if (!is_1x1_) {
        //tic;
        scol2im(col_buff, src, prevSize, kernelSize, strideSize, pad, dilationSize);
        //toc;
      }
    }
    FREE(buffer_);
  }
  if (0) {
    int nn, nc, nh, nw, kc, kh, kw;
    DISPATCH_WORKER_BEGIN(prevSize.n) {
      for (nn = start; nn < stop; nn++) {
        for (nc = 0; nc < nextSize.c; nc++) {
          for (nh = 0; nh < nextSize.h; nh++) {
            for (nw = 0; nw < nextSize.w; nw++) {
              const int inStartX = nw * strideSize.w;
              const int inStartY = nh * strideSize.h;
              const int nextDiffIdx = Data_4DIndex(nextSize, nn, nc, nh, nw);
              const int kn = nc;
              for (kc = 0; kc < kernelSize.c; kc++) {
                for (kh = 0; kh < kernelSize.h; kh++) {
                  for (kw = 0; kw < kernelSize.w; kw++) {
                    const int inY = inStartY + kh;
                    const int inX = inStartX + kw;
                    if (inY >= 0 && inY < prevSize.h && inX >= 0 && inX < prevSize.w) {
                      const int prevDiffIdx = Data_4DIndex(prevSize, nn, kc, inY, inX);
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

static void conv2d_backward_weight(const float* prevData, const float* nextDiff, float* kernelDiff, 
                     DataSize prevSize, DataSize kernelSize, DataSize nextSize,
                     DataSize strideSize, DataSize dilationSize, enum PAD_MODE mode) {
  if (1) {
    IRECT pad = mode2pad(prevSize, kernelSize, strideSize, dilationSize, mode);
    int is_1x1_ = kernelSize.h == 1 && kernelSize.w == 1 && strideSize.h == 1 && strideSize.w == 1 && (mode==PAD_VALID);
    float* buffer_ = NULL;
    int next_dim = Data_2DSize(nextSize);
    int kernel_dim = Data_3DSize(kernelSize);
    int i;
    if (!is_1x1_) {
      MYREALLOC(buffer_, (next_dim * kernel_dim));
    }
    
    for (i=0; i<prevSize.n; ++i) {
      const float* src = prevData + i*Data_3DSize(prevSize);
      const float* dst = nextDiff + i*Data_3DSize(nextSize);
      const float* col_buff = src;
      if (!is_1x1_) {
        col_buff = sim2col(src, buffer_, prevSize, kernelSize, strideSize, pad, dilationSize);
      }
      blas_sgemm(CblasNoTrans, CblasTrans, nextSize.c, kernel_dim, next_dim, 1., dst, col_buff, 1., kernelDiff);
    }
    FREE(buffer_);
  }
  if (0) {
    int nn, nc, nh, nw, kc, kh, kw;
    //update kernel gradient
    for (nn = 0; nn < nextSize.n; nn++) {
      for (nc = 0; nc < nextSize.c; nc++) {
        for (nh = 0; nh < nextSize.h; nh++) {
          for (nw = 0; nw < nextSize.w; nw++) {
            const int inStartX = nw * strideSize.w;
            const int inStartY = nh * strideSize.h;
            const int nextDiffIdx = Data_4DIndex(nextSize, nn, nc, nh, nw);
            const int kn = nc;
            for (kc = 0; kc < kernelSize.c; kc++) {
              for (kh = 0; kh < kernelSize.h; kh++) {
                for (kw = 0; kw < kernelSize.w; kw++) {
                  const int inY = inStartY + kh;
                  const int inX = inStartX + kw;
                  if (inY >= 0 && inY < prevSize.h && inX >= 0 && inX < prevSize.w) {
                    const int kernelIdx = Data_4DIndex(kernelSize, kn, kc, kh, kw);
                    const int prevIdx = Data_4DIndex(prevSize, nn, kc, inY, inX);
                    kernelDiff[kernelIdx] += prevData[prevIdx] * nextDiff[nextDiffIdx];
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

static void conv2d_backward_bias(float* biasDiff, const float* nextDiff, DataSize nextSize) {
  if (1) {
    float* bias_multiplier = NULL;
    int i, out_dim = Data_2DSize(nextSize);
    MYREALLOC(bias_multiplier, out_dim);
    blas_sset(out_dim, 1, bias_multiplier);
    for (i=0; i<nextSize.n; ++i) {
      const float* src = nextDiff + i*Data_3DSize(nextSize);
      blas_sgemv(CblasNoTrans, nextSize.c, out_dim, 1., src, bias_multiplier, 1., biasDiff);
    }
    FREE(bias_multiplier);
  }
  if (0) {
    int nn, nc, nh, nw;
    for (nn = 0; nn < nextSize.n; nn++) {
      for (nc = 0; nc < nextSize.c; nc++) {
        const int biasIdx = nc;
        for (nh = 0; nh < nextSize.h; nh++) {
          for (nw = 0; nw < nextSize.w; nw++) {
            const int nextDiffIdx = Data_4DIndex(nextSize, nn, nc, nh, nw);
            biasDiff[biasIdx] += nextDiff[nextDiffIdx];
          }
        }
      }
    }
  }
  //div by batch size
}

static int test_conv() {
  float* prev = NULL;
  float* kernel = NULL;
  float* bias = NULL;
  float* next = NULL;
  float* next2 = NULL;
  float* next3 = NULL;
  DataSize prevSize = dataSize(20,3,4,5);
  DataSize nextSize = dataSize(20,6,4,5);
  DataSize kernelSize = dataSize(6,3,3,3);
  DataSize biasSize = dataSize(6,1,1,1);
  int i, n;
  resetZero(&prev, prevSize);
  resetZero(&next, nextSize);
  resetZero(&next2, nextSize);
  resetZero(&next3, nextSize);
  resetZero(&kernel, kernelSize);
  resetZero(&bias, biasSize);
  uniform_distribution_init(prev, Data_total(prevSize), 0, 1);
  uniform_distribution_init(kernel, Data_total(kernelSize), 0, 1);
  uniform_distribution_init(prev, Data_total(prevSize), 0, 1);
  //uniform_distribution_init(bias, Data_total(biasSize), 0, 1);
  //conv2d_gen(prev, kernel, bias, next, prevSize, kernelSize, nextSize, 1, 1, 1, "S");
  //conv2d_gemm(prev, kernel, bias, next2, prevSize, kernelSize, nextSize, 1, 1, 1, "S");
  //conv2d_fast(prev, kernel, bias, next3, prevSize, kernelSize, nextSize, 1, 1, 1, "S");
  n = Data_total(prevSize);
  for (i=0; i<n; ++i) {
    EXPECT_NEAR(next2[i], next[i], 1e-4);
    EXPECT_NEAR(next3[i], next[i], 1e-4);
  }
  free(kernel);
  free(prev);
  free(next);
  free(next2);
  free(next3);
  free(bias);
  return 0;
}

static DataSize conv_calc_outsize(DataSize input, DataSize kernel, int stride_w, int stride_h, int dilation, enum Method pad_type)
{
  DataSize output;
  const int kernel_extent_h = dilation * (kernel.h - 1) + 1;
  const int kernel_extent_w = dilation * (kernel.w - 1) + 1;

  output.n = input.n;
  output.c = kernel.n;
  if (pad_type == M_VALID) {
    output.w = (input.w - kernel_extent_w) / stride_w + 1;
    output.h = (input.h - kernel_extent_h) / stride_h + 1;
  }
  else if (pad_type == M_SAME) {
    output.w = input.w;
    output.h = input.h;
  }
  return output;
}
static Dtype ConvolutionLayer(enum LayerOp op, struct Net* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  struct Blob* kernel = WBLOB(0);
  struct Blob* bias = layer->w_size>1 ? WBLOB(1) : NULL;
  int stride_w = MAX(1, IARG(stride_w));
  int stride_h = MAX(1, IARG(stride_h));
  int dilation = MAX(1, IARG(dilation));
  enum Method pad_type = MARG(pad_type);
  if (pad_type != M_SAME) {
    pad_type = M_VALID;
  }
  enum PAD_MODE mode = pad_type==M_SAME? PAD_SAME : PAD_VALID;
  DataSize strideSize = dataSize(1, 1, stride_h, stride_w);
  DataSize dilationSize = dataSize(dilation, dilation, dilation, dilation);
  switch (op) {
  case Op_Forward:
    //test_conv();
    next->size = conv_calc_outsize(prev->size, kernel->size, stride_w, stride_h, dilation, pad_type);
    {
      const float* prevData = getData(prev);
      const float* kernelData = getData(kernel);
      const float* biasData = bias ? getData(bias) : NULL;
      float* nextData = setDataZero(next);
      DISPATCH_WORKER_BEGIN(prev->size.n) {
        DataSize prevSize = prev->size;
        prevSize.n = stop - start;
        //((1) ? conv2d_gen : conv2d_cpu_gemm)
        //conv2d_gen
        //conv2d_fast
        conv2d_forward(prevData + start * Data_3DSize(prev->size), kernelData, biasData, nextData + start * Data_3DSize(next->size),
          prevSize, kernel->size, next->size, strideSize, dilationSize, mode);
      };
      DISPATCH_WORKER_END;
      //showmat("prev", prevData, prev->size);    showmat("kernel", kernel->data, kernel->size);    WaitKey(-1);
    }
    break;
  case Op_Backward:
    {
      const float* nextDiff = getDiff(next);
      const float* kernelData = getData(kernel);
      float* prevDiff = setDiffZero(prev);
      //update bias gradient
      //tic;
      if (bias) {
        float* biasDiff = setDiffZero(bias);
        conv2d_backward_bias(biasDiff, nextDiff, next->size);
        div_inplace(biasDiff, (float)next->size.n, next->size.c);
      }
      //toc;
      //update this layer's param
      //tic;
      if (kernel) {
        const float* prevData = getData(prev);
        float* kernelDiff = setDiffZero(kernel);
        conv2d_backward_weight(prevData, nextDiff, kernelDiff, prev->size, kernel->size, next->size, strideSize, dilationSize, mode);
        div_inplace(kernelDiff, (float)next->size.n, Data_total(kernel->size));
      }
      //toc;
      conv2d_backward(nextDiff, kernelData, prevDiff, prev->size, kernel->size, next->size, strideSize, dilationSize, mode);
    }
    break;
  default:
    break;
  }
  return 0;
}

static int net_conv(struct Net* net, int input, int* w, int* b, int num_output, int kernel_w, int kernel_h, int stride_w, int stride_h, enum Method pad_type)
{
  const char* type = "conv";
  int enabledBias = b != NULL;
  struct Layer* layer = net_addLayer(net, 1, 1, 1 + enabledBias, ConvolutionLayer, type);
  DataSize prevSize, kernelSize;
  int dilation = 1;
  easyAssert(stride_w > 0 && stride_h > 0, "step is invalidate.");
  IARG(kernel_w) = kernel_w;
  IARG(kernel_h) = kernel_h;
  IARG(stride_w) = stride_w;
  IARG(stride_h) = stride_h;
  IARG(num_output) = num_output;
  IARG(dilation) = dilation;
  MARG(pad_type) = pad_type;
  LIBLOB(0) = input;
  prevSize = IBLOB(0)->size;
  kernelSize = dataSize(num_output, prevSize.c, kernel_h, kernel_w);
  LWBLOB(0) = *w = net_addBlob(net, kernelSize, type, "w");
  //Blob_init_normal(net, *w, 0.0f, 0.1);
  Blob_init_xavier(BLOB(*w));
  if (b) {
    LWBLOB(1) = *b = net_addBlob(net, dataSize(num_output, 1, 1, 1), type, "b");
    Blob_init_const(BLOB(*b), 0.f);
  }
  return LOBLOB(0) = net_addBlob(net, conv_calc_outsize(prevSize, kernelSize, stride_w, stride_h, dilation, pad_type), type, "o");
}
static void matMul1(const float* A, int al, int ai, const float* B, int bl, int bi, float* C, int cl, int ci, int M, int N, int K)
{
  int i, j, k;

  for (i = 0; i < M; ++i) {
    for (j = 0; j < N; ++j) {
      const float* a = A + i * al;
      const float* b = B + j * bl;
      float s = 0;

      for (k = 0; k < K; ++k) {
        s += a[k * ai] * b[k * bi];
      }

      C[i * cl + j * ci] = s;
    }
  }
}

static pBox* conv(const Weight* weight, const pBox* box, pBox* outpBox)
{
  int outh = (box->h - weight->size[0].h) / weight->strideSize.h + 1;
  int outw = (box->w - weight->size[0].w) / weight->strideSize.w + 1;
  int Matrix_count = outpBox->h * outpBox->w;
  pBox tmp[1] = {0};
  if (box==outpBox) {
    box = pBox_clone(box, tmp);
  }
  pBoxsetsize(outpBox, outh, outw, weight->size[0].n, box->size.n);
  if (NULL==outpBox->data) {
    return 0;
  }

  if (1) {
    DataSize dilationSize = dataSize(1,1,1,1);
    conv2d_fast(box->data, weight->data[0].p, weight->data[1].p, outpBox->data, box->size, weight->size[0], outpBox->size, weight->strideSize, dilationSize, PAD_VALID);
  }
#if 0
  for (f = 0; f < box->size.n; ++f) {
    img_t pbox2 = pBox_frame(box, f);
    img_t tmp_matrix[1] = {0};
    const float* tmp_matrixp = pBox_datap(tmp_matrix);
    float* outpBoxp = pBox_datapf(outpBox, f);
    _feature2Matrix(&pbox2, tmp_matrix, weight);

    if (0) {
      tic;
      //matMul_block(weight->data->p, tmp_matrix->w, 1, tmp_matrixp, tmp_matrix->w, 1, outpBoxp, tmp_matrix->h, 1, weight->size[0].n, tmp_matrix->h, tmp_matrix->w);
      toc;
    }

    if (0) {
      tic;
      matMul(weight->data->p, tmp_matrix->w, 1, tmp_matrixp, tmp_matrix->w, 1, outpBoxp, tmp_matrix->h, 1, weight->size[0].n, tmp_matrix->h, tmp_matrix->w);
      toc;
    }

#ifdef HAVE_OPENCL1

    if (used_gpu) {
      tic;
      umem_setu(weight->data);
      umem_setu(tmp_matrix->data);
      outpBox->data->modifyu = 1;
      int astep = matrix->w, ai = 1, bstep = matrix->w, bi = 1, cstep = matrix->h, ci = 1, M = weight->size[0].n, N = matrix->h, K = matrix->w;
      size_t _globalsize[] = {M, N};
      buf_t args[] = {
        clarg(weight->data->u),
        clarg(matrix->data->u),
        clarg(outpBox->data->u),
        clarg(astep),
        clarg(ai),
        clarg(bstep),
        clarg(bi),
        clarg(cstep),
        clarg(ci),
        clarg(M),
        clarg(N),
        clarg(K),
      };
      static const char* matMul_progstr =
          "__kernel void matMul(__global float* A,"
          "        __global float* B,  "
          "        __global float* C,  "
          "        int astep, int ai, "
          "        int bstep, int bi, "
          "        int cstep, int ci, "
          "        int M, int N, int K)  "
          "{\n"
          "    int i = get_global_id(0);\n"
          "    int j = get_global_id(1), k;\n"
          "    float s = 0;\n"
          "    int aoff = i*astep;\n"
          "    int boff = j*bstep;\n"
          "    for (k=0; k<K; ++k) {\n"
          "      s += A[aoff+k*ai] * B[boff+k*bi];\n"
          "    }\n"
          "    C[i*cstep+j*ci] = s;\n"
          "}"
          ;

      static ProgramSource matMul_oclsrc(matMul_progstr);
      matMul_oclsrc.run("matMul", countof(_globalsize), _globalsize, NULL, countof(args), args);
      //umem_getu(outpBox->data);
      toc;
    }

#endif

#ifdef USE_OPENBLAS
    tic;

    if (weight->pad == 0) {
      //C←αAB + βC
      //                1              2            3              4     C's size    5              k     alpha     A*              A'col             B*           B'col    beta      C*           C'col
      cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, weight->size[0].n, tmp_matrix->h, tmp_matrix->w, 1, weight->data->p, tmp_matrix->w, tmp_matrixp, tmp_matrix->w, 0, outpBoxp, tmp_matrix->h);
    }
    else {
      ASSERT(0);
      pBox padpbox[1] = {0};
      featurePad(box, padpbox, weight->pad);
      //C←αAB + βC
      //                1              2            3              4     C's size    5              k     alpha     A*              A'col             B*           B'col    beta      C*           C'col
      cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, weight->size[0].n, tmp_matrix->h, tmp_matrix->w, 1, weight->data->p, tmp_matrix->w, tmp_matrixp, tmp_matrix->w, 0, outpBoxp, tmp_matrix->h);
      freepBox(padpbox);
    }

    toc;
#endif
    imfree(tmp_matrix);
  }
#endif
  pBox_free(tmp);
  return outpBox;
}
