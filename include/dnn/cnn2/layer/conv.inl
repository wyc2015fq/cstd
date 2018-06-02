
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

#define _mm_muladd_ps(a, b, c)  a = _mm_add_ps(a, _mm_mul_ps(b, c))

#include "conv_1x1.h"
#include "conv_2x2.h"
#include "conv_3x3.h"
#include "conv_5x5.h"

#define CONV3X3S1_NEON_ARGS(args1, j, OPT_DEF) \
	OPT_DEF(args1, j, const Blob*, bottom_data) \
	OPT_DEF(args1, j, Blob*, top_blob) \
	OPT_DEF(args1, j, const float*, _kernel) \
	OPT_DEF(args1, j, const float*, _bias) \

int conv2d_fast_valid(int h, int w, const float* src, int src_step,int srccn, float* dst, int dst_step, int dstcn,
 int kernel_h, int kernel_w, const float* kernel, const float* bias, int stride_h, int stride_w, int dilation) {
  typedef void (*sconv_func)(int h, int w, const float* src, int src_step, int srccn, float* dst, int dst_step, int dstcn, const float* kernel, const float* bias);
  sconv_func conv = NULL;
  int outh = h - kernel_h + 1;
  int outw = w - kernel_w + 1;
  // kernel_size x stride
  if (kernel_h==kernel_w && stride_h==stride_w) {
    static sconv_func conv_func_table[8][4] = {0};
    static int inited = 0;
    int kernel_size = kernel_w;
    int stride = stride_w;
  
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
    conv(h, w, src, src_step, srccn, dst, dst_step, dstcn, kernel, bias);
  }
  else {
    //#include "x86/convolution_5x5.h"
    const int maxk = kernel_h * kernel_w;
  
    int i, j, k, p, q;
    // kernel offsets
    int* space_ofs = NULL;
    const float* weight_data_ptr = kernel;
    MYREALLOC(space_ofs, maxk);
    {
      int p1 = 0;
      int p2 = 0;
      int gap = w * dilation - kernel_w * dilation;
    
      for (i = 0; i < kernel_h; i++) {
        for (j = 0; j < kernel_w; j++) {
          space_ofs[p1] = p2;
          p1++;
          p2 += dilation;
        }
      
        p2 += gap;
      }
    }
  
    // num_output
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
            const float* sptr = src + (q * h + i * stride_h)*src_step + j * stride_w;
          
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

IRECT conv2d_mode2pad(DataSize inputSize, DataSize kernelSize, int stride_w, int stride_h, int dilation, const char* mode) {
  IRECT pad={0};
  if (*mode=='S') {
    const int kernel_extent_h = dilation * (kernelSize.h - 1) + 1;
    const int kernel_extent_w = dilation * (kernelSize.w - 1) + 1;
    int pad_w = kernel_extent_h + (inputSize.w - 1) / stride_w * stride_w - inputSize.w;
    int pad_h = kernel_extent_w + (inputSize.h - 1) / stride_h * stride_h - inputSize.h;
    
    if (pad_w > 0 || pad_h > 0) {
      pad = iRECT(pad_w / 2, pad_h / 2, pad_w - pad_w / 2, pad_h - pad_h / 2);
    }
  }
  return pad;
}
//#include "arm/convolution_5x5.h"
static float* conv2d_forward(const float* input, const float* kernel, const float* bias, float* output,
                         DataSize inputSize, DataSize kernelSize, DataSize outputSize,
                         int stride_h, int stride_w, int dilation, const char* mode) {
  if (1) {
    IRECT pad = conv2d_mode2pad(inputSize, kernelSize, stride_w, stride_h, dilation, mode);
    int is_1x1_ = kernelSize.h == 1 && kernelSize.w == 1 && stride_h == 1 && stride_w == 1 && (*mode=='V');
    float* buffer_ = NULL;
    int output_dim = Data_2DSize(outputSize);
    int kernel_dim = Data_3DSize(kernelSize);
    int i;
    ASSERT(inputSize.n==outputSize.n);
    ASSERT(inputSize.c==kernelSize.c);
    ASSERT(kernelSize.n==outputSize.c);
    
    if (!is_1x1_) {
      MYREALLOC(buffer_, (output_dim * kernel_dim));
    }
    for (i=0; i<inputSize.n; ++i) {
      float* dst = output + i * Data_3DSize(outputSize);
      const float* src = input + i * Data_3DSize(inputSize);
      const float* col_buff = src;
      if (!is_1x1_) {
        col_buff = im2col_cpu(src, buffer_, inputSize, kernelSize.h, kernelSize.w, stride_h, stride_w, pad, dilation, dilation);
      }
      blas_sgemm(CblasNoTrans, CblasNoTrans, outputSize.c, output_dim, kernel_dim, 1.f, kernel, col_buff, 0.f, dst);
    }
    if (bias) {
      float* bias_multiplier = NULL;
      MYREALLOC(buffer_, output_dim);
      bias_multiplier = buffer_;
      blas_sset(output_dim, 1, bias_multiplier);
      for (i=0; i<inputSize.n; ++i) {
        float* dst = output + i * Data_3DSize(outputSize);
        blas_sgemm(CblasNoTrans, CblasNoTrans, outputSize.c, output_dim, 1, (float)1., bias, bias_multiplier, (float)1., dst);
      }
    }
    FREE(buffer_);
  }
  if (0) {
    // convolv with NxN kernel
    // value = value + bias
    float* srcex = NULL;
    int kernel_h = kernelSize.h;
    int kernel_w = kernelSize.w;
    int i;
    IRECT pad=conv2d_mode2pad(inputSize, kernelSize, stride_w, stride_h, dilation, mode);
    mode = mode ? mode : "V";
    ASSERT(inputSize.n==outputSize.n);
    ASSERT(inputSize.c==kernelSize.c);
    ASSERT(kernelSize.n==outputSize.c);
    ASSERT(inputSize.c>0);
    stride_h = MAX(1, stride_h);
    stride_w = MAX(1, stride_w);
    dilation = MAX(1, dilation);
    
    //     fprintf(stderr, "Convolution input %d x %d  pad = %d  ksize=%d  stride=%d\n", w, h, pad, kernel_size, stride);
    //int dst_w = (srcSize.w - kernel_extent_w) / stride_w + 1;
    //int dst_h = (srcSize.h - kernel_extent_h) / stride_h + 1;
    
    for (i=0; i<inputSize.n; ++i) {
      DataSize srcSize = inputSize;
      DataSize dstSize = outputSize;
      float* dst = output + i * Data_3DSize(outputSize);
      const float* src = input + i * Data_3DSize(inputSize);
      if (pad.l || pad.t || pad.r || pad.b) {
        src = copy_make_border(src, &srcex, inputSize, &srcSize, pad, BORDER_CONSTANT, 0.f);
      }
      conv2d_fast_valid(srcSize.h, srcSize.w, src, srcSize.w, srcSize.c, dst, dstSize.w, kernelSize.n, kernel_h, kernel_w,
        kernel, bias, stride_h, stride_w, dilation);
    }
    if (srcex) {
      free(srcex);
    }
  }
  
  if (0) {
    mode = mode ? mode : "V";
    ASSERT(inputSize.n==outputSize.n);
    ASSERT(inputSize.c==kernelSize.c);
    ASSERT(kernelSize.n==outputSize.c);
    if (*mode == 'V') {
      // VALID
      //const DataSize inputSize = dataSize(in, ic, iw, ih);
      //const DataSize kernelSize = dataSize(kn, ic, kw, kh);
      //const DataSize outputSize = dataSize(in, kn, ow, oh);
      int in=inputSize.n, ic=inputSize.c, iw=inputSize.w, ih=inputSize.h,
        kn=kernelSize.n, kw=kernelSize.w, kh=kernelSize.h, ow=outputSize.w, oh=outputSize.h;
      int nn, nc, nh, nw, kc, nextIdx;
      ASSERT(1==dilation);
      for (nn = 0; nn < in; nn++) {
        for (nc = 0; nc < outputSize.c; nc++) {
          for (nh = 0; nh < outputSize.h; nh++) {
            for (nw = 0; nw < outputSize.w; nw++) {
              int inStartX = nw * stride_w;
              int inStartY = nh * stride_h;
              float sum = bias ? bias[nc] : 0.f;
              for (kc = 0; kc < kernelSize.c; kc++) {
                for (kh = 0; kh < kernelSize.h; kh++) {
                  for (kw = 0; kw < kernelSize.w; kw++) {
                    int prevIdx = Data_4DIndex(inputSize, nn, kc, inStartY + kh, inStartX + kw);
                    int kernelIdx = Data_4DIndex(kernelSize, nc, kc, kh, kw);
                    sum += input[prevIdx] * kernel[kernelIdx];
                  }
                }
              }
              nextIdx = Data_4DIndex(outputSize, nn, nc, nh, nw);
              output[nextIdx] = sum;
            }
          }
        }
      }
    }
    else if (*mode == 'S') {
      int nn, nc, nh, nw, kc, nextIdx;
      // SAME
      //const DataSize inputSize = dataSize(in, ic, iw, ih);
      //const DataSize kernelSize = dataSize(kn, ic, kw, kh);
      //const DataSize outputSize = dataSize(in, kn, ow, oh);
      int in=inputSize.n, ic=inputSize.c, iw=inputSize.w, ih=inputSize.h,
        kn=kernelSize.n, kw=kernelSize.w, kh=kernelSize.h, ow=outputSize.w, oh=outputSize.h;
      ASSERT(inputSize.n==outputSize.n);
      ASSERT(inputSize.c==kernelSize.c);
      ASSERT(kernelSize.n==outputSize.c);
      ASSERT(1==dilation);
      for (nn = 0; nn < in; nn++) {
        for (nc = 0; nc < outputSize.c; nc++) {
          for (nh = 0; nh < outputSize.h; nh++) {
            for (nw = 0; nw < outputSize.w; nw++) {
              const int inStartX = nw - kw / 2;
              const int inStartY = nh - kh / 2;
              float sum = 0;
              for (kc = 0; kc < kernelSize.c; kc++) {
                for (kh = 0; kh < kernelSize.h; kh++) {
                  for (kw = 0; kw < kernelSize.w; kw++) {
                    const int inY = inStartY + kh;
                    const int inX = inStartX + kw;
                    if (inY >= 0 && inY < (int)inputSize.h && inX >= 0 && inX < (int)inputSize.w) {
                      int prevIdx = Data_4DIndex(inputSize, nn, kc, inY, inX);
                      int kernelIdx = Data_4DIndex(kernelSize, nc, kc, kh, kw);
                      sum += input[prevIdx] * kernel[kernelIdx];
                    }
                  }
                }
              }
              nextIdx = Data_4DIndex(outputSize, nn, nc, nh, nw);
              if (bias) {
                int biasIdx = nc;
                sum += bias[biasIdx];
              }
              output[nextIdx] = sum;
            }
          }
        }
      }
    }
  }
  return output;
}

void conv2d_backward(const float* nextDiff, const float* kernelData, float* prevDiff,
                       DataSize prevSize, DataSize kernelSize, DataSize nextSize,
                       int stride_h, int stride_w, int dilation, const char* mode) {
  if (1) {
    IRECT pad = conv2d_mode2pad(prevSize, kernelSize, stride_w, stride_h, dilation, mode);
    int is_1x1_ = kernelSize.h == 1 && kernelSize.w == 1 && stride_h == 1 && stride_w == 1 && (*mode=='V');
    float* buffer_ = NULL;
    int output_dim = Data_2DSize(nextSize);
    int kernel_dim = Data_3DSize(kernelSize);
    int i;
    
    if (!is_1x1_) {
      MYREALLOC(buffer_, (output_dim * kernel_dim));
    }
    for (i=0; i<prevSize.n; ++i) {
      const float* dst = nextDiff + i * Data_3DSize(nextSize);
      float* src = prevDiff + i * Data_3DSize(prevSize);
      float* col_buff = is_1x1_ ? src : buffer_;
      //tic;
      blas_sgemm(CblasTrans, CblasNoTrans, kernel_dim, output_dim, nextSize.c, 1.f, kernelData, dst, 0.f, col_buff);
      //toc;
      if (!is_1x1_) {
        //tic;
        col2im_cpu(col_buff, src, prevSize, kernelSize.h, kernelSize.w, stride_h, stride_w, pad, dilation, dilation);
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
              const int inStartX = nw * stride_w;
              const int inStartY = nh * stride_h;
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

void conv2d_backward_weight(const float* prevData, const float* nextDiff, float* kernelDiff, 
                     DataSize prevSize, DataSize kernelSize, DataSize nextSize,
                     int stride_h, int stride_w, int dilation, const char* mode) {
  if (1) {
    IRECT pad = conv2d_mode2pad(prevSize, kernelSize, stride_w, stride_h, dilation, mode);
    int is_1x1_ = kernelSize.h == 1 && kernelSize.w == 1 && stride_h == 1 && stride_w == 1 && (*mode=='V');
    float* buffer_ = NULL;
    int output_dim = Data_2DSize(nextSize);
    int kernel_dim = Data_3DSize(kernelSize);
    int i;
    if (!is_1x1_) {
      MYREALLOC(buffer_, (output_dim * kernel_dim));
    }
    
    for (i=0; i<prevSize.n; ++i) {
      const float* src = prevData + i*Data_3DSize(prevSize);
      const float* dst = nextDiff + i*Data_3DSize(nextSize);
      const float* col_buff = src;
      if (!is_1x1_) {
        col_buff = im2col_cpu(src, buffer_, prevSize, kernelSize.h, kernelSize.w, stride_h, stride_w, pad, dilation, dilation);
      }
      blas_sgemm(CblasNoTrans, CblasTrans, nextSize.c, kernel_dim, output_dim, 1., dst, col_buff, 1., kernelDiff);
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
            const int inStartX = nw * stride_w;
            const int inStartY = nh * stride_h;
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

void conv2d_backward_bias(float* biasDiff, const float* nextDiff, DataSize nextSize) {
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

int test_conv() {
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
  //conv2d_cpu_gemm(prev, kernel, bias, next2, prevSize, kernelSize, nextSize, 1, 1, 1, "S");
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
