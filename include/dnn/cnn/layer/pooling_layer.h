


static void pooling2x2s2_max(const float* prevData, float* nextData, DataSize prevSize, DataSize nextSize)
{
  int w = prevSize.w;
  int h = prevSize.h;
  int q, i, len = prevSize.c * prevSize.n;

  int outw = nextSize.w;
  int outh = nextSize.h;

  #pragma omp parallel for
  for (q = 0; q < len; q++) {
    const float* img0 = prevData + q * Data_2DSize(prevSize);
    float* outptr = nextData + q * Data_2DSize(nextSize);

    const float* r0 = img0;
    const float* r1 = img0 + w;

    for (i = 0; i < outh; i++) {
      int remain = outw;

#if CC_NEON
      int nn = outw >> 2;
      remain = outw - (nn << 2);
#if __aarch64__

      for (; nn > 0; nn--) {
        float32x4_t _r00 = vld1q_f32(r0);
        float32x4_t _r10 = vld1q_f32(r1);
        float32x4_t _r01 = vld1q_f32(r0 + 4);
        float32x4_t _r11 = vld1q_f32(r1 + 4);

        float32x4_t _max0 = vmaxq_f32(_r00, _r10);
        float32x4_t _max1 = vmaxq_f32(_r01, _r11);

        float32x4_t _max = vpmaxq_f32(_max0, _max1);

        vst1q_f32(outptr, _max);

        r0 += 8;
        r1 += 8;
        outptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "0:                             \n"
            "pld        [%1, #256]          \n"
            "pld        [%2, #256]          \n"
            "vld1.f32   {d0-d3}, [%1]!      \n"
            "vld1.f32   {d4-d7}, [%2]!      \n"
            "vmax.f32   q0, q0, q2          \n"
            "vmax.f32   q1, q1, q3          \n"
            "vpmax.f32  d4, d0, d1          \n"
            "vpmax.f32  d5, d2, d3          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d4-d5}, [%3]!      \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(r0),     // %1
            "=r"(r1),     // %2
            "=r"(outptr)  // %3
            : "0"(nn),
            "1"(r0),
            "2"(r1),
            "3"(outptr)
            : "cc", "memory", "q0", "q1", "q2", "q3"
        );
      }

#endif // __aarch64__
#endif // CC_NEON

      for (; remain > 0; remain--) {
        float max0 = MAX(r0[0], r0[1]);
        float max1 = MAX(r1[0], r1[1]);

        *outptr = MAX(max0, max1);

        r0 += 2;
        r1 += 2;
        outptr++;
      }

      r0 += w;
      r1 += w;
    }
  }
}

static void pooling3x3s2_max(const float* prevData, float* nextData, DataSize prevSize, DataSize nextSize)
{
  int w = prevSize.w;
  int h = prevSize.h;
  int q, i, len = prevSize.c * prevSize.n;

  int outw = nextSize.w;
  int outh = nextSize.h;

  const int tailstep = w - 2 * outw + w;

  #pragma omp parallel for

  for (q = 0; q < len; q++) {
    const float* img0 = prevData + q * Data_2DSize(prevSize);
    float* outptr = nextData + q * Data_2DSize(nextSize);

    const float* r0 = img0;
    const float* r1 = img0 + w;
    const float* r2 = img0 + w * 2;

    for (i = 0; i < outh; i++) {
      int remain = outw;

#if CC_SSE2
#define _mm_loadu_ps2(a)  _mm_set_ps(*(a+6), *(a+4), *(a+2), *(a))

      for (; remain > 3; remain -= 4) {
        __m128 _max0 = _mm_max_ps(_mm_loadu_ps2(r0), _mm_loadu_ps2(r0 + 1));
        __m128 _max1 = _mm_max_ps(_mm_loadu_ps2(r1), _mm_loadu_ps2(r1 + 1));
        __m128 _max, _max2 = _mm_max_ps(_mm_loadu_ps2(r2), _mm_loadu_ps2(r2 + 1));

        _max0 = _mm_max_ps(_max0, _mm_loadu_ps2(r0 + 2));
        _max1 = _mm_max_ps(_max1, _mm_loadu_ps2(r1 + 2));
        _max2 = _mm_max_ps(_max2, _mm_loadu_ps2(r2 + 2));

        _max = _mm_max_ps(_mm_max_ps(_max0, _max1), _max2);
        _mm_storeu_ps(outptr, _max);

        r0 += 8;
        r1 += 8;
        r2 += 8;
        outptr += 4;
      }

#endif

#if CC_NEON
      int nn = outw >> 2;
      remain = outw - (nn << 2);
#if __aarch64__
      float32x4x2_t _r0 = vld2q_f32(r0);
      float32x4x2_t _r1 = vld2q_f32(r1);
      float32x4x2_t _r2 = vld2q_f32(r2);

      for (; nn > 0; nn--) {
        float32x4x2_t _r0n = vld2q_f32(r0 + 8);
        float32x4x2_t _r1n = vld2q_f32(r1 + 8);
        float32x4x2_t _r2n = vld2q_f32(r2 + 8);

        float32x4_t _max0 = vmaxq_f32(_r0.val[0], _r0.val[1]);
        float32x4_t _max1 = vmaxq_f32(_r1.val[0], _r1.val[1]);
        float32x4_t _max2 = vmaxq_f32(_r2.val[0], _r2.val[1]);

        float32x4_t _r02 = vextq_f32(_r0.val[0], _r0n.val[0], 1);
        float32x4_t _r12 = vextq_f32(_r1.val[0], _r1n.val[0], 1);
        float32x4_t _r22 = vextq_f32(_r2.val[0], _r2n.val[0], 1);

        _max0 = vmaxq_f32(_max0, _r02);
        _max1 = vmaxq_f32(_max1, _r12);
        _max2 = vmaxq_f32(_max2, _r22);

        float32x4_t _max = vmaxq_f32(vmaxq_f32(_max0, _max1), _max2);

        vst1q_f32(outptr, _max);

        _r0 = _r0n;
        _r1 = _r1n;
        _r2 = _r2n;

        r0 += 8;
        r1 += 8;
        r2 += 8;
        outptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "pld        [%1, #256]          \n"
            "vld2.f32   {d0-d3}, [%1]!      \n"// q0 = 0 2 4 6  q1 = 1 3 5 7
            "pld        [%2, #256]          \n"
            "vld2.f32   {d4-d7}, [%2]!      \n"
            "pld        [%3, #256]          \n"
            "vld2.f32   {d8-d11}, [%3]!     \n"
            "0:                             \n"
            "pld        [%1, #256]          \n"
            "vld2.f32   {d12-d15}, [%1]!    \n"// q6 = 8 10 12 14  q7 = 9 11 13 15

            "vmax.f32   q12, q0, q1         \n"
            "vmax.f32   q13, q2, q3         \n"

            "pld        [%2, #256]          \n"
            "vld2.f32   {d16-d19}, [%2]!    \n"

            "vmax.f32   q14, q4, q5         \n"
            "vext.32    q0, q0, q6, #1      \n"

            "pld        [%3, #256]          \n"
            "vld2.f32   {d20-d23}, [%3]!    \n"

            "vext.32    q2, q2, q8, #1      \n"

            "vmax.f32   q12, q12, q0        \n"
            "vext.32    q4, q4, q10, #1     \n"

            "vmax.f32   q13, q13, q2        \n"
            "vmax.f32   q14, q14, q4        \n"
            "vmax.f32   q12, q12, q13       \n"

            "vorr       q0, q6, q6          \n"
            "vorr       q1, q7, q7          \n"
            "vmax.f32   q12, q12, q14       \n"

            "vorr       q2, q8, q8          \n"
            "vorr       q3, q9, q9          \n"
            "vorr       q4, q10, q10        \n"
            "vorr       q5, q11, q11        \n"

            "subs       %0, #1              \n"
            "vst1.f32   {d24-d25}, [%4]!    \n"
            "bne        0b                  \n"
            "sub        %1, #32             \n"
            "sub        %2, #32             \n"
            "sub        %3, #32             \n"
            : "=r"(nn),     // %0
            "=r"(r0),     // %1
            "=r"(r1),     // %2
            "=r"(r2),     // %3
            "=r"(outptr)  // %4
            : "0"(nn),
            "1"(r0),
            "2"(r1),
            "3"(r2),
            "4"(outptr)
            : "cc", "memory", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10", "q11", "q12", "q13", "q14"
        );
      }

#endif // __aarch64__
#endif // CC_NEON

      for (; remain > 0; remain--) {
        float max0 = MAX(MAX(r0[0], r0[1]), r0[2]);
        float max1 = MAX(MAX(r1[0], r1[1]), r1[2]);
        float max2 = MAX(MAX(r2[0], r2[1]), r2[2]);

        *outptr = MAX(MAX(max0, max1), max2);

        r0 += 2;
        r1 += 2;
        r2 += 2;
        outptr++;
      }

      r0 += tailstep;//1 + w;
      r1 += tailstep;//1 + w;
      r2 += tailstep;//1 + w;
    }
  }
}

enum POOLING_TYPE{
POOLING_MAX,
POOLING_AVE,
};
static int pooling_valid(const float* prevData, float* nextData, DataSize prevSize, DataSize nextSize, enum POOLING_TYPE pool,DataSize kernelSize,DataSize strideSize) {
  int i, j, k, q, mw;
  const int maxk = kernelSize.h * kernelSize.w;
  // kernel offsets
  int* space_ofs = NULL;
  MYREALLOC(space_ofs, maxk);
  {
    int p1 = 0;
    int p2 = 0;
    int gap = prevSize.w - kernelSize.w;
    
    for (i = 0; i < kernelSize.h; i++) {
      for (j = 0; j < kernelSize.w; j++) {
        space_ofs[p1++] = p2;
        p1++;
        p2++;
      }
      
      p2 += gap;
    }
  }
  
  mw = prevSize.w;
  
  if (pool == POOLING_MAX) {
#pragma omp parallel for
    
    for (q = 0; q < prevSize.c; q++) {
      const float* mdata = prevData + q * Data_2DSize(prevSize);
      float* outptr = nextData + q * Data_2DSize(nextSize);
      
      for (i = 0; i < nextSize.h; i++) {
        for (j = 0; j < nextSize.w; j++) {
          const float* sptr = mdata + mw * i * strideSize.h + j * strideSize.w;
          
          float max = sptr[0];
          
          for (k = 0; k < maxk; k++) {
            float val = sptr[ space_ofs[k] ];
            max = MAX(max, val);
          }
          
          outptr[j] = max;
        }
        
        outptr += nextSize.w;
      }
    }
  }
  else if (pool == POOLING_AVE) {
  int wtail = (prevSize.w - kernelSize.w) % strideSize.w;
  int htail = (prevSize.h - kernelSize.h) % strideSize.h;
    
#pragma omp parallel for
    for (q = 0; q < prevSize.c; q++) {
      const float* mdata = prevData + q * Data_2DSize(prevSize);
      float* outptr = nextData + q * Data_2DSize(nextSize);
      
      for (i = 0; i < nextSize.h; i++) {
        for (j = 0; j < nextSize.w; j++) {
          const float* sptr = mdata + mw * i * strideSize.h + j * strideSize.w;
          
          float sum = 0;
          
          for (k = 0; k < maxk; k++) {
            float val = sptr[ space_ofs[k] ];
            sum += val;
          }
          
          outptr[j] = sum / maxk;
        }
        
        outptr += nextSize.w;
      }
      
      // fix tail pad
      if (wtail != 0) {
        const float scale = (float)kernelSize.w / wtail;
        
        outptr = nextData + q*Data_2DSize(nextSize) + nextSize.w - 1;
        
        for (i = 0; i < nextSize.h; i++) {
          *outptr *= scale;
          outptr += nextSize.w;
        }
      }
      
      if (htail != 0) {
        const float scale = (float)kernelSize.h / htail;
        
        outptr = nextData + q * Data_2DSize(nextSize) + (nextSize.h - 1) * nextSize.w;
        
        for (i = 0; i < nextSize.w; i++) {
          outptr[i] *= scale;
        }
      }
    }
  }
  FREE(space_ofs);
  return 0;
}

static int pooling_forward(const float* prevData, float* nextData, DataSize prevSize, DataSize nextSize,
                           enum POOLING_TYPE pool,DataSize kernelSize,DataSize strideSize,enum PAD_MODE mode, DataSize dilationSize,int global)
{
  // max value in NxN window
  // avg value in NxN window
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;
  int q, i, size = w * h;
  //int outw, outh, wtail, htail;
  const float* prevData_bordered = prevData;
  float* srcex = NULL;
  IRECT pad;

  //     fprintf(stderr, "Pooling     input %d x %d  pad = %d  ksize=%d  stride=%d\n", w, h, pad, kernelSize.size, stride);
  if (global) {
    // pBoxsetsize(nextData, 1, 1, channels, 1);
    if (pool == POOLING_MAX) {
      #pragma omp parallel for
      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + q * Data_2DSize(prevSize);
        float* outptr = nextData + q * Data_2DSize(nextSize);

        float max = ptr[0];

        for (i = 0; i < size; i++) {
          max = MAX(max, ptr[i]);
        }

        outptr[0] = max;
      }
    }
    else if (pool == POOLING_AVE) {
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = prevData + q * Data_2DSize(prevSize);
        float* outptr = nextData + q * Data_2DSize(nextSize);

        float sum = 0.f;

        for (i = 0; i < size; i++) {
          sum += ptr[i];
        }

        outptr[0] = sum / size;
      }
    }

    return 0;
  }
  
  pad = mode2pad(prevSize, kernelSize, strideSize, dilationSize, mode);
  
  //outw = (w - kernelSize.w) / strideSize.w + 1;
  //outh = (h - kernelSize.h) / strideSize.h + 1;
  
  //wtail = (w - kernelSize.w) % strideSize.w;
  //htail = (h - kernelSize.h) % strideSize.h;
  
  for (i=0; i<prevSize.n; ++i) {
    DataSize srcSize = dataSize(1, prevSize.c, prevSize.h, prevSize.w);
    DataSize dstSize = dataSize(1, nextSize.c, nextSize.h, nextSize.w);
    float* dst = nextData + i * Data_3DSize(nextSize);
    const float* src = prevData + i * Data_3DSize(prevSize);
    if (pad.l || pad.t || pad.r || pad.b) {
      src = copy_make_border(prevData, &srcex, prevSize, &prevSize, pad, CC_BORDER_CONSTANT, 0.f);
    }
    
    if (kernelSize.h == 2 && kernelSize.w == 2 && pool == POOLING_MAX && strideSize.h == 2 && strideSize.w == 2) {
      pooling2x2s2_max(src, dst, srcSize, dstSize);
    }
    else if (kernelSize.h == 3 && kernelSize.w == 3 && pool == POOLING_MAX && strideSize.h == 2 && strideSize.w == 2) {
      pooling3x3s2_max(src, dst, srcSize, dstSize);
    }
    else {
      pooling_valid(src, dst, srcSize, dstSize, pool, kernelSize, strideSize);
    }
  }
  FREE(srcex);
  return 0;
}

#if 0
// TODO(Yangqing): Is there a faster way to do pooling in the channel-first
// case?
static Dtype PoolingLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  Blob* prev_blob = IBLOB(0);
  Blob* nextData = LBLOB(1);
  int i, n, c, ph, pw;
  int prev_pixnum = prevData->h * prevData->w;
  int next_pixnum = nextData->h * nextData->w;
  
  int kernelSize.h = LARG(kernelSize.h);
  int kernelSize.w = LARG(kernelSize.w);
  int strideSize.h_ = LARG(strideSize.h);
  int strideSize.w_ = LARG(strideSize.w);
  int pad_h_ = LARG(pad);
  int pad_w_ = LARG(pad);
  int channels_;
  int input_h, input_w;
  int output_h, output_w;
  
  int Flag_;

  if (LARG(global)) {
    kernelSize.h = prevData->h;
    kernelSize.w = prevData->w;
  }
  CHECK_GT0(kernelSize.h, 0, "Filter dimensions cannot be zero.");
  CHECK_GT0(kernelSize.w, 0, "Filter dimensions cannot be zero.");

  if (LARG(global)) {
    CHECK0(pad_h_ == 0 && pad_w_ == 0 && strideSize.h_ == 1 && strideSize.w_ == 1,
      "With Global_pooling: true; only pad = 0 and stride = 1");
  }
//  LOG(INFO)<<"this is the end in pool_setup";
  Flag_ = true;

  channels_ = Blob_channels(prevData);
  input_h = prevData->h;
  input_w = prevData->w;

  output_h = (int)(ceil((float)(input_h + 2 * pad_h_ - kernelSize.h) / strideSize.h_)) + 1;
  output_w = (int)(ceil((float)(input_w + 2 * pad_w_ - kernelSize.w) / strideSize.w_)) + 1;
  if (pad_h_ || pad_w_) {
    // If we have padding, ensure that the last pooling starts strictly
    // inside the image (instead of at the padding); otherwise clip the last.
    if ((output_h - 1) * strideSize.h_ >= input_h + pad_h_) {
      --output_h;
    }
    if ((output_w - 1) * strideSize.w_ >= input_w + pad_w_) {
      --output_w;
    }
    CHECK_LT((output_h - 1) * strideSize.h_, input_h + pad_h_);
    CHECK_LT((output_w - 1) * strideSize.w_, input_w + pad_w_);
  }

  switch (op) {
  case Op_Forward:
    {
      const Dtype* prevData = BlobDatap(prevData);
      Dtype* nextData = BlobDatap(nextData);
      const int next_count = Blob_count(nextData);
      int* mask = NULL;
      // suppress warnings about uninitalized variables
      // Different pooling methods. We explicitly do the switch outside the for
      // loop to save time, although this results in more code.
      switch (LARG(pool)) {
      case POOLING_MAXI:
      case POOLING_MAX:
        // Initialize
        Blob_ReshapeLike(nextData->im, nextData);
        mask = (int*)BlobDatap(nextData->im);
        blas_sset(next_count, -1, mask);
        blas_sset(next_count, (-FLT_MAX), nextData);
        // The main loop
        for (n = 0; n < prevData->f; ++n) {
          for (c = 0; c < channels_; ++c) {
            for (ph = 0; ph < output_h; ++ph) {
              for (pw = 0; pw < output_w; ++pw) {
                int hstart = ph * strideSize.h_ - pad_h_;
                int wstart = pw * strideSize.w_ - pad_w_;
                int hend = MIN(hstart + kernelSize.h, input_h);
                int wend = MIN(wstart + kernelSize.w, input_w);
                hstart = MAX(hstart, 0);
                wstart = MAX(wstart, 0);
                const int out_index = ph * output_w + pw;
                //const int index = hstart * input_w + wstart;                nextData[out_index] = prevData[index];                mask[out_index] = index;
                for (h = hstart; h < hend; ++h) {
                  for (w = wstart; w < wend; ++w) {
                    const int index = h * input_w + w;
                    if (prevData[index] > nextData[out_index]) {
                      nextData[out_index] = prevData[index];
                      mask[out_index] = index;
                    }
                  }
                }
              }
            }
            // compute offset
            prevData += prev_pixnum;
            nextData += next_pixnum;
            mask += next_pixnum;
          }
        }
        break;
      case POOLING_AVE:
        for (i = 0; i < next_count; ++i) {
          nextData[i] = 0;
        }
        // The main loop
        for (n = 0; n < prevData->f; ++n) {
          for (c = 0; c < channels_; ++c) {
            for (ph = 0; ph < output_h; ++ph) {
              for (pw = 0; pw < output_w; ++pw) {
                int hstart = ph * strideSize.h_ - pad_h_;
                int wstart = pw * strideSize.w_ - pad_w_;
                int hend = MIN(hstart + kernelSize.h, input_h + pad_h_);
                int wend = MIN(wstart + kernelSize.w, input_w + pad_w_);
                int pool_size = (hend - hstart) * (wend - wstart);
                hstart = MAX(hstart, 0);
                wstart = MAX(wstart, 0);
                hend = MIN(hend, input_h);
                wend = MIN(wend, input_w);
                for (h = hstart; h < hend; ++h) {
                  for (w = wstart; w < wend; ++w) {
                    nextData[ph * output_w + pw] += prevData[h * input_w + w];
                  }
                }
                nextData[ph * output_w + pw] /= pool_size;
              }
            }
            // compute offset
            prevData += prev_pixnum;
            nextData += next_pixnum;
          }
        }
        break;
      default:
        LOG(GLOG_FATAL, "Unknown pooling method.");
        break;
      }
    }
    break;
  case Op_Backward:
    {
      Blob_ReshapeLike(prev_blob->diff, prevData);
      const Dtype* next_diff = BlobDatap(nextData->diff);
      Dtype* prev_diff = BlobDatap(prev_blob->diff);
      // Different pooling methods. We explicitly do the switch outside the for
      // loop to save time, although this results in more codes.
      blas_sset(Blob_count(prev_blob->diff), (0), prev_diff);
      // We'll output the mask to next[1] if it's of size >1.
      const int* mask = NULL;  // suppress warnings about uninitialized variables
      switch (LARG(pool)) {
      case POOLING_MAX:
        // The main loop
        mask = (int*)BlobDatap(nextData->im);
        for (n = 0; n < nextData->diff->f; ++n) {
          for (c = 0; c < channels_; ++c) {
            for (ph = 0; ph < output_h; ++ph) {
              for (pw = 0; pw < output_w; ++pw) {
                const int index = ph * output_w + pw;
                const int prev_index = mask[index];
                prev_diff[prev_index] += next_diff[index];
              }
            }
            prev_diff += prev_pixnum;
            next_diff += next_pixnum;
            mask += next_pixnum;
          }
        }
        break;
      case POOLING_AVE:
        // The main loop
        for (n = 0; n < nextData->diff->f; ++n) {
          for (c = 0; c < channels_; ++c) {
            for (ph = 0; ph < output_h; ++ph) {
              for (pw = 0; pw < output_w; ++pw) {
                int hstart = ph * strideSize.h_ - pad_h_;
                int wstart = pw * strideSize.w_ - pad_w_;
                int hend = MIN(hstart + kernelSize.h, input_h + pad_h_);
                int wend = MIN(wstart + kernelSize.w, input_w + pad_w_);
                int pool_size = (hend - hstart) * (wend - wstart);
                hstart = MAX(hstart, 0);
                wstart = MAX(wstart, 0);
                hend = MIN(hend, input_h);
                wend = MIN(wend, input_w);
                for (h = hstart; h < hend; ++h) {
                  for (w = wstart; w < wend; ++w) {
                    prev_diff[h * input_w + w] += next_diff[ph * output_w + pw] / pool_size;
                  }
                }
              }
            }
            // offset
            prev_diff += prev_pixnum;
            next_diff += next_pixnum;
          }
        }
        break;
      default:
        LOG(GLOG_FATAL, "Unknown pooling method.");
      }
    }
    break;
  }
  return 0;
}
#endif
static DataSize pool_calc_size(DataSize prevSize, int kernel_w, int kernel_h, int stride_w, int stride_h, enum Method pad_type)
{
  DataSize outputSize;
  easyAssert(prevSize.n && prevSize.w >= kernel_w && prevSize.h >= kernel_h, "poolingKernelSize parameters invalidate.");
  outputSize.n = prevSize.n;
  outputSize.c = prevSize.c;
  if (pad_type == M_VALID) {
    outputSize.w = (prevSize.w - kernel_w) / stride_w + 1;
    outputSize.h = (prevSize.h - kernel_h) / stride_h + 1;
  }
  else if (pad_type == M_SAME ||1) {
    outputSize.w = (int)ceil((float)prevSize.w / (float)stride_w);
    outputSize.h = (int)ceil((float)prevSize.h / (float)stride_h);
  } else {
    ASSERT(0);
  }
  return outputSize;
}
static Dtype PoolingLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int stride_w = IARG(stride_w);
  int stride_h = IARG(stride_h);
  int kernel_w = IARG(kernel_w);
  int kernel_h = IARG(kernel_h);
  enum Method pool = MARG(pool);
  enum Method pad_type = MARG(pad_type);
  DataSize prevSize = prev->size;
  int nn, nc, nh, nw, ph, pw;
  kernel_w = MAX(1, kernel_w);
  kernel_h = MAX(1, kernel_h);
  stride_w = MAX(1, stride_w);
  stride_h = MAX(1, stride_h);
  switch (op) {
  case Op_Forward:
    next->size = pool_calc_size(prevSize, kernel_w, kernel_h, stride_w, stride_h, pad_type); {
    const float* prevData = getData(prev);
    float* nextData = setDataZero(next);
    float* maxIdxesData = (MNARG(phase) == M_Train && pool == M_Max) ? setBuff(next, 0) : NULL;
    for (nn = 0; nn < next->size.n; nn++) {
      for (nc = 0; nc < next->size.c; nc++) {
        for (nh = 0; nh < next->size.h; nh++) {
          for (nw = 0; nw < next->size.w; nw++) {
            const int inStartX = nw * stride_w;
            const int inStartY = nh * stride_h;
            const int nextDataIdx = Data_4DIndex(next->size, nn, nc, nh, nw);
            float result = 0;
            int maxIdx = 0;
            if (pool == M_Max) {
              for (ph = 0; ph < kernel_h; ph++) {
                for (pw = 0; pw < kernel_w; pw++) {
                  const int inY = inStartY + ph;
                  const int inX = inStartX + pw;
                  if (inY >= 0 && inY < prevSize.h && inX >= 0 && inX < prevSize.w) {
                    const int prevDataIdx = Data_4DIndex(prev->size, nn, nc, inY, inX);
                    if (result < prevData[prevDataIdx]) {
                      result = prevData[prevDataIdx];
                      maxIdx = ph * kernel_w + pw;
                    }
                  }
                }
              }
              if (maxIdxesData) {
                maxIdxesData[nextDataIdx] = (float)maxIdx;
              }
            }
            else if (pool == M_Mean) {
              for (ph = 0; ph < kernel_h; ph++) {
                for (pw = 0; pw < kernel_w; pw++) {
                  const int inY = inStartY + ph;
                  const int inX = inStartX + pw;
                  if (inY >= 0 && inY < prevSize.h && inX >= 0 && inX < prevSize.w) {
                    const int prevDataIdx = Data_3DIndex(prev->size, nc, inY, inX);
                    result += prevData[prevDataIdx];
                  }
                }
              }
              result /= kernel_w * kernel_h;
            }
            nextData[nextDataIdx] = result;
          }//ow
        }//oh
      }//oc
    }//on
    //showmat("prev", prevData, prev->size);    showmat("next", nextData, next->size);    WaitKey(-1);
  }
  break;
  case Op_Backward: {
    //update prevDiff
    const float* maxIdxesData = (pool == M_Max) ? getBuff(next, 0) : NULL;
    float* prevDiff0 = setDiffZero(prev);
    //calculate current inner diff
    //none
    //pass next layer's diff to previous layer
    for (nn = 0; nn < next->size.n; nn++) {
      const float* nextDiff = getDiff(next) + nn * Data_3DSize(next->size);
      float* prevDiff = prevDiff0 + nn * Data_3DSize(prev->size);
      for (nc = 0; nc < next->size.c; nc++) {
        for (nh = 0; nh < next->size.h; nh++) {
          for (nw = 0; nw < next->size.w; nw++) {
            const int inStartX = nw * stride_w;
            const int inStartY = nh * stride_h;
            const int nextDataIdx = Data_3DIndex(next->size, nc, nh, nw);
            if (pool == M_Max) {
              for (ph = 0; ph < kernel_h; ph++) {
                for (pw = 0; pw < kernel_w; pw++) {
                  const int inY = inStartY + ph;
                  const int inX = inStartX + pw;
                  if (inY >= 0 && inY < prevSize.h && inX >= 0 && inX < prevSize.w) {
                    const int prevDiffIdx = Data_3DIndex(prev->size, nc, inY, inX);
                    if (ph * kernel_w + pw == maxIdxesData[nextDataIdx]) {
                      prevDiff[prevDiffIdx] += nextDiff[nextDataIdx];
                    }
                  }
                }
              }
            }
            else if (pool == M_Mean) {
              int kernel_size = kernel_w * kernel_h;
              const float meanDiff = nextDiff[nextDataIdx] / (float)(kernel_size);
              for (ph = 0; ph < kernel_h; ph++) {
                for (pw = 0; pw < kernel_w; pw++) {
                  const int inY = inStartY + ph;
                  const int inX = inStartX + pw;
                  if (inY >= 0 && inY < prevSize.h && inX >= 0 && inX < prevSize.w) {
                    const int prevDiffIdx = Data_3DIndex(prev->size, nc, inY, inX);
                    prevDiff[prevDiffIdx] += meanDiff;
                  }
                }
              }
            }
          }
        }
      }
    }
    //update this layer's param
    //nop
  }
  break;
  default:
    break;
  }
  return 0;
}
static int net_pooling(struct Net* net, int input, enum Method pool, int kernel_w, int kernel_h, int stride_w, int stride_h)
{
  const char* type = "pool";
  struct Layer* layer = net_addLayer(net, 1, 1, 0, PoolingLayer, type);
  DataSize prevSize;
  enum Method pad_type = M_SAME;
  easyAssert(kernel_w > 1 && kernel_h > 1 && stride_w > 0 && stride_h > 0, "parameters invalidate.");
  IARG(kernel_w) = kernel_w;
  IARG(kernel_h) = kernel_h;
  IARG(stride_w) = stride_w;
  IARG(stride_h) = stride_h;
  MARG(pool) = pool;
  MARG(pad_type) = pad_type;
  LIBLOB(0) = input;
  prevSize = IBLOB(0)->size;
  return LOBLOB(0) = net_addBlob(net, pool_calc_size(prevSize, kernel_w, kernel_h, stride_w, stride_h, pad_type), type, "o");
}

static pBox* maxPooling(const pBox* box, pBox* outpBox, int kernel, int stride)
{
  DataSize kernelSize = dataSize(1, 1, kernel, kernel);
  DataSize strideSize = dataSize(1, 1, stride, stride);
  DataSize dilationSize = dataSize(1, 1, 1, 1);
  int inch = pBox_channels(box);
  int outh = ceil((float)(box->h - kernelSize.h) / strideSize.h + 1);
  int outw = ceil((float)(box->w - kernelSize.w) / strideSize.w + 1);
  pBox tmp[1] = {0};
  if (box==outpBox) {
    box = pBox_clone(box, tmp);
  }
  pBoxsetsize(outpBox, outh, outw, inch, box->size.n);
  if (0) {
    pooling_forward(box->data, outpBox->data, box->size, outpBox->size, POOLING_MAX, kernelSize, strideSize, PAD_SAME, dilationSize, 0);
  }
  if (1) {
    int i, c, row, col, kernelRow;
    int inchf = inch * box->size.n;
    if (NULL==outpBox->data) {
      return 0;
    }
    
#ifdef HAVE_OPENCL1
    
    if (used_gpu) {
      tic;
      umem_setu(box->data);
      outpBox->data->modifyu = 1;
      int Matrix_count = outpBox->h * outpBox->w;
      int pbox_count = box->h * box->w;
      int ih = box->h;
      int iw = box->w;
      int ow = outpBox->w;
      size_t _globalsize[] = {inch, outpBox->h, outpBox->w};
      buf_t args[] = {
        clarg(box->data->u),
          clarg(outpBox->data->u),
          clarg(pbox_count),
          clarg(Matrix_count),
          clarg(ih),
          clarg(iw),
          clarg(ow),
          clarg(stride),
          clarg(kernelSize.size),
      };
      static const char* maxPooling_progstr =
        "__kernel void maxPooling(__global float* pIn, "
        "        __global float* pOut,  "
        "        int pbox_count, int Matrix_count, int ih, int iw, int ow, int stride, int kernelSize.size)  "
        "{"
        "    int c = get_global_id(0);"
        "    int row = get_global_id(1);"
        "    int col = get_global_id(2);"
        "    int off = c*pbox_count + row*stride*iw + col*stride;"
        "    float maxNum = pIn[off];"
        "    int diffh = row*stride-ih+1;"
        "    int diffw = col*stride-iw+1;"
        "    for (int kernelRow = 0; kernelRow < kernelSize.size; kernelRow++){"
        "      if((kernelRow+diffh)>0)break;"
        "      for(int i=0;i<kernelSize.size;i++){"
        "        if((i+diffw)>0)break;"
        "        if(maxNum<pIn[off+i+kernelRow*iw])maxNum=pIn[off+i+kernelRow*iw];"
        "      }"
        "    }"
        "    pOut[c*Matrix_count+row*ow+col] = maxNum;"
        "}"
        ;
      static ProgramSource  maxPooling_oclsrc(maxPooling_progstr);
      maxPooling_oclsrc.run("maxPooling", countof(_globalsize), _globalsize, NULL, countof(args), args);
      //umem_getu(outpBox->data);
      toc;
      return ;
    }
    
#endif
    
    if (0) {
      int Matrix_count = outpBox->h * outpBox->w;
      int pbox_count = box->h * box->w;
      int ih = box->h;
      int iw = box->w;
      int ow = outpBox->w;
      const Dtype* pIn = pBox_datap(box);
      Dtype* pOut = pBox_datap(outpBox);
      const Dtype* pIn0;
      umem_getu(box->data);
      SETMODIFYP(outpBox, 1);
      
      tic;
      {
#pragma omp parallel for
        
        for (c = 0; c < inchf; c++) {
          pIn0 = pIn + c * box->h * box->w;
          
          for (row = 0; row < outpBox->h; row ++) {
            for (col = 0; col < outpBox->w; col++) {
              int off = c * pbox_count + row * strideSize.h * iw + col * strideSize.w;
              Dtype maxNum = pIn0[off];
              int diffh = row * strideSize.h - ih + 1;
              int diffw = col * strideSize.w - iw + 1;
              
              for (kernelRow = 0; kernelRow < kernelSize.h; kernelRow++) {
                if ((kernelRow + diffh) > 0) {
                  break;
                }
                
                for (i = 0; i < kernelSize.w; i++) {
                  if ((i + diffw) > 0) {
                    break;
                  }
                  
                  if (maxNum < pIn0[off + i + kernelRow * iw]) {
                    maxNum = pIn0[off + i + kernelRow * iw];
                  }
                }
              }
              
              *pOut++ = maxNum;
            }
          }
        }
      }
      toc;
    }
    
    tic;
    umem_getu(box->data);
    SETMODIFYP(outpBox, 1);
    if (1) {
      const Dtype* pIn = pBox_datap(box);
      Dtype* p = pBox_datap(outpBox);
      const Dtype* pIn0;
      const Dtype* pIn1;
      const Dtype* ptemp;
      const Dtype* ptemp1;
      Dtype maxNum = 0;
      
      if ((box->w - kernelSize.w) % strideSize.w == 0 && (box->h - kernelSize.h) % strideSize.h == 0) {
        if (2 == kernelSize.h && 2 == kernelSize.w) {
#pragma omp parallel for
          
          for (c = 0; c < inchf; c++) {
            pIn0 = pIn + c * box->h * box->w;
            
            for (row = 0; row < outpBox->h; row ++) {
              pIn1 = pIn0 + row * strideSize.h * box->w;
              
              for (col = 0; col < outpBox->w; col++) {
                ptemp = pIn1 + col * strideSize.w;
                *p++ = MAX4(ptemp[0], ptemp[1], ptemp[box->w], ptemp[box->w + 1]);
              }
            }
          }
        }
        else if (3 == kernelSize.h && 3 == kernelSize.w) {
          int w = box->w;
          int w2 = box->w * 2;
          
#pragma omp parallel for
          
          for (c = 0; c < inchf; c++) {
            pIn0 = pIn + c * box->h * box->w;
            
            for (row = 0; row < outpBox->h; row ++) {
              pIn1 = pIn0 + row * strideSize.h * box->w;
              
              for (col = 0; col < outpBox->w; col++) {
                const Dtype* ptemp = pIn1 + col * strideSize.w;
                float t0 = MAX3(ptemp[0], ptemp[1], ptemp[2]);
                float t1 = MAX3(ptemp[w], ptemp[w + 1], ptemp[w + 2]);
                float t2 = MAX3(ptemp[w2], ptemp[w2 + 1], ptemp[w2 + 2]);
                *p++ = MAX3(t0, t1, t2);
              }
            }
          }
        }
        else {
#pragma omp parallel for
          
          for (c = 0; c < inchf; c++) {
            pIn0 = pIn + c * box->h * box->w;
            
            for (row = 0; row < outpBox->h; row ++) {
              pIn1 = pIn0 + row * strideSize.h * box->w;
              
              for (col = 0; col < outpBox->w; col++) {
                ptemp = pIn1 + col * strideSize.w;
                maxNum = *ptemp;
                
                for (kernelRow = 0; kernelRow < kernelSize.h; kernelRow++) {
                  for (i = 0; i < kernelSize.w; i++) {
                    if (maxNum < * (ptemp + i + kernelRow * box->w)) {
                      maxNum = *(ptemp + i + kernelRow * box->w);
                    }
                  }
                }
                
                *p++ = maxNum;
              }
            }
          }
        }
      }
      else {
        int w = box->w;
        int diffh = 0, diffw = 0;
        
#pragma omp parallel for
        
        for (c = 0; c < inchf; c++) {
          float* p0 = p;
          pIn0 = pIn + c * box->h * box->w;
          for (row = 0; row < outpBox->h; row ++) {
            pIn1 = pIn0 + row * strideSize.h * box->w;
            diffh = MIN(box->h - row * strideSize.h, kernelSize.h);
            
            for (col = 0; col < outpBox->w; col++) {
              ptemp = pIn1 + col * strideSize.w;
              maxNum = *ptemp;
              diffw = MIN(box->w - col * strideSize.w, kernelSize.w);
              
              for (kernelRow = 0; kernelRow < diffh; kernelRow++) {
                ptemp1 = ptemp + kernelRow * box->w;
                
                for (i = 0; i < diffw; i++) {
                  if (maxNum < ptemp1[i]) {
                    maxNum = ptemp1[i];
                  }
                }
              }
              
              if (p == p0 && 0 == maxNum) {
                int asdf = 0;
              }
              
              *p++ = maxNum;
            }
          }
          
          if (0 == *p0) {
            int asdf = 0;
          }
        }
        
        //ASSERT(p == (float*)(outpBox->data + outSize.h * outSize.w * outSize.c * outSize.n));
      }
      
      toc;
    }
  }
  pBox_free(tmp);
  return outpBox;
}
