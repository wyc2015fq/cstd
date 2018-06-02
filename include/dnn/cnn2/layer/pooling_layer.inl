


static void pooling2x2s2_max(const Blob* bottom_data, Blob* top_blob)
{
  int w = bottom_data->w;
  int h = bottom_data->h;
  int q, i, inch = Blob_channels(bottom_data);

  int outw = top_blob->w;
  int outh = top_blob->h;
  int outch = Blob_channels(top_blob);

  #pragma omp parallel for

  for (q = 0; q < inch; q++) {
    const float* img0 = Blob_channel_p(bottom_data, q);
    float* outptr = Blob_channel_p(top_blob, q);

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

static void pooling3x3s2_max(const Blob* bottom_data, Blob* top_blob)
{
  int w = bottom_data->w;
  int h = bottom_data->h;
  int q, i, inch = Blob_channels(bottom_data);

  int outw = top_blob->w;
  int outh = top_blob->h;
  int outch = Blob_channels(top_blob);

  const int tailstep = w - 2 * outw + w;

  #pragma omp parallel for

  for (q = 0; q < inch; q++) {
    const float* img0 = Blob_channel_p(bottom_data, q);
    float* outptr = Blob_channel_p(top_blob, q);

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

static int pooling_forward(const Blob* bottom_data, Blob* top_blob, Method pool,int kernel_h,int kernel_w,int stride_h,int stride_w,int pad,int global)
{
  // max value in NxN window
  // avg value in NxN window

  Blob im[2] = {0};
  int w = bottom_data->w;
  int h = bottom_data->h;
  int channels = Blob_channels(bottom_data);
  int q, i, size = w * h;
  int outw, outh, wtail, htail;
  const Blob* bottom_blob_bordered = bottom_data;

  //     fprintf(stderr, "Pooling     input %d x %d  pad = %d  ksize=%d  stride=%d\n", w, h, pad, kernel_size, stride);
  if (global) {
    pBoxsetsize(top_blob, 1, 1, channels, 1);
  if (NULL==top_blob->data) {
    return 0;
  }

    if (imempty(top_blob)) {
      return -100;
    }

    if (pool == Method_MAX) {
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = Blob_channel_p(bottom_data, q);
        float* outptr = Blob_channel_p(top_blob, q);

        float max = ptr[0];

        for (i = 0; i < size; i++) {
          max = MAX(max, ptr[i]);
        }

        outptr[0] = max;
      }
    }
    else if (pool == Method_AVE) {
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* ptr = Blob_channel_p(bottom_data, q);
        float* outptr = Blob_channel_p(top_blob, q);

        float sum = 0.f;

        for (i = 0; i < size; i++) {
          sum += ptr[i];
        }

        outptr[0] = sum / size;
      }
    }

    return 0;
  }


  if (pad > 0) {
    bottom_blob_bordered = copy_make_border(bottom_data, im, pad, pad, pad, pad, BORDER_CONSTANT, 0.f);
    w = bottom_blob_bordered->w;
    h = bottom_blob_bordered->h;
  }
  else if (pad == -233) {
    int wpad = kernel_w + (w - 1) / stride_w * stride_w - w;
    int hpad = kernel_h + (h - 1) / stride_h * stride_h - h;

    if (wpad > 0 || hpad > 0) {
      bottom_blob_bordered = copy_make_border(bottom_data, im, hpad / 2, hpad - hpad / 2, wpad / 2, wpad - wpad / 2, BORDER_CONSTANT, 0.f);
    }

    w = bottom_blob_bordered->w;
    h = bottom_blob_bordered->h;
  }

  outw = (w - kernel_w) / stride_w + 1;
  outh = (h - kernel_h) / stride_h + 1;

  wtail = (w - kernel_w) % stride_w;
  htail = (h - kernel_h) % stride_h;

  if (pad == -233 || pad == -2333) {
    wtail = 0;
    htail = 0;
  }

  if (wtail != 0 || htail != 0) {
    int wtailpad = 0;
    int htailpad = 0;
    Blob* bottom_blob_bordered2 = im + 1;

    if (wtail != 0) {
      wtailpad = kernel_w - wtail;
    }

    if (htail != 0) {
      htailpad = kernel_h - htail;
    }


    if (pool == Method_MAX) {
      copy_make_border(bottom_blob_bordered, bottom_blob_bordered2, 0, htailpad, 0, wtailpad, BORDER_REPLICATE, 0.f);
    }
    else if (pool == Method_AVE) {
      copy_make_border(bottom_blob_bordered, bottom_blob_bordered2, 0, htailpad, 0, wtailpad, BORDER_CONSTANT, 0.f);
    }

    bottom_blob_bordered = bottom_blob_bordered2;

    w = bottom_blob_bordered->w;
    h = bottom_blob_bordered->h;

    if (wtail != 0) {
      outw += 1;
    }

    if (htail != 0) {
      outh += 1;
    }
  }

  pBoxsetsize(top_blob, outh, outw, channels, 1);
  if (NULL==top_blob->data) {
    return 0;
  }

  if (kernel_h == 2 && kernel_w == 2 && pool == Method_MAX && stride_h == 2 && stride_w == 2) {
    pooling2x2s2_max(bottom_blob_bordered, top_blob);
  }
  else if (kernel_h == 3 && kernel_w == 3 && pool == Method_MAX && stride_h == 2 && stride_w == 2) {
    pooling3x3s2_max(bottom_blob_bordered, top_blob);
  }
  else {
    int i, j, k, q, mw;
  const int maxk = kernel_h * kernel_w;
    // kernel offsets
    int* space_ofs = NULL;
    MYREALLOC(space_ofs, maxk);
    {
      int p1 = 0;
      int p2 = 0;
      int gap = w - kernel_w;

      for (i = 0; i < kernel_h; i++) {
        for (j = 0; j < kernel_w; j++) {
          space_ofs[p1] = p2;
          p1++;
          p2++;
        }

        p2 += gap;
      }
    }

    mw = bottom_blob_bordered->w;

    if (pool == Method_MAX) {
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* mdata = Blob_channel_p(bottom_blob_bordered, q);
        float* outptr = Blob_channel_p(top_blob, q);

        for (i = 0; i < outh; i++) {
          for (j = 0; j < outw; j++) {
            const float* sptr = mdata + mw * i * stride_h + j * stride_w;

            float max = sptr[0];

            for (k = 0; k < maxk; k++) {
              float val = sptr[ space_ofs[k] ];
              max = MAX(max, val);
            }

            outptr[j] = max;
          }

          outptr += outw;
        }
      }
    }
    else if (pool == Method_AVE) {
      #pragma omp parallel for

      for (q = 0; q < channels; q++) {
        const float* mdata = Blob_channel_p(bottom_blob_bordered, q);
        float* outptr = Blob_channel_p(top_blob, q);

        for (i = 0; i < outh; i++) {
          for (j = 0; j < outw; j++) {
            const float* sptr = mdata + mw * i * stride_h + j * stride_w;

            float sum = 0;

            for (k = 0; k < maxk; k++) {
              float val = sptr[ space_ofs[k] ];
              sum += val;
            }

            outptr[j] = sum / maxk;
          }

          outptr += outw;
        }

        // fix tail pad
        if (wtail != 0) {
          const float scale = (float)kernel_w / wtail;

          outptr = Blob_channel_p(top_blob, q) + outw - 1;

          for (i = 0; i < outh; i++) {
            *outptr *= scale;
            outptr += outw;
          }
        }

        if (htail != 0) {
          const float scale = (float)kernel_h / htail;

          outptr = Blob_crow(top_blob, q, outh - 1);

          for (i = 0; i < outw; i++) {
            outptr[i] *= scale;
          }
        }
      }
    }

    FREE(space_ofs);
  }
  imfrees2(im);

  return 0;
}

static int Pooling(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  const Blob* bottom_data = LBLOB(0)->data;
  Blob* top_blob = net->blobs[layer->blobs[1]].data;
  pooling_forward(bottom_data, top_blob, EARG(pool),LARG(kernel_h),LARG(kernel_w),LARG(stride_h),LARG(stride_w),LARG(pad),LARG(global));
  return 0;
}

// TODO(Yangqing): Is there a faster way to do pooling in the channel-first
// case?

static int PoolingLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer) {
  Blob* bottom_blob = LBLOB(0);
  Blob* top_blob = LBLOB(1);
  int i, n, c, ph, pw;
  int bottom_pixnum = bottom_blob->data->h * bottom_blob->data->w;
  int top_pixnum = top_blob->data->h * top_blob->data->w;
  
  int kernel_h = LARG(kernel_h);
  int kernel_w = LARG(kernel_w);
  int stride_h_ = LARG(stride_h);
  int stride_w_ = LARG(stride_w);
  int pad_h_ = LARG(pad);
  int pad_w_ = LARG(pad);
  int channels_;
  int input_h, input_w;
  int output_h, output_w;
  
  int Flag_;

  if (LARG(global)) {
    kernel_h = bottom_blob->data->h;
    kernel_w = bottom_blob->data->w;
  }
  CHECK_GT0(kernel_h, 0, "Filter dimensions cannot be zero.");
  CHECK_GT0(kernel_w, 0, "Filter dimensions cannot be zero.");

  if (LARG(global)) {
    CHECK0(pad_h_ == 0 && pad_w_ == 0 && stride_h_ == 1 && stride_w_ == 1,
      "With Global_pooling: true; only pad = 0 and stride = 1");
  }
  if (pad_h_ != 0 || pad_w_ != 0) {
    CHECK0( LARG(pool) == Method_AVE
        || LARG(pool) == Method_MAX
        || LARG(pool) == Method_DEF
        || LARG(pool) == Method_DEF_ALL
        || LARG(pool) == Method_DEF_ALL2
        || LARG(pool) == Method_DEF_ALL3
        || LARG(pool) == Method_DEF_ALL4
        || LARG(pool) == Method_LOWRES,
        "Padding implemented only for average and MAX pooling.");
    CHECK_LT(pad_h_, kernel_h);
    CHECK_LT(pad_w_, kernel_w);
  }
//  LOG(INFO)<<"this is the end in pool_setup";
  Flag_ = true;

  channels_ = Blob_channels(bottom_blob->data);
  input_h = bottom_blob->data->h;
  input_w = bottom_blob->data->w;

  output_h = (int)(ceil((float)(input_h + 2 * pad_h_ - kernel_h) / stride_h_)) + 1;
  output_w = (int)(ceil((float)(input_w + 2 * pad_w_ - kernel_w) / stride_w_)) + 1;
  if (pad_h_ || pad_w_) {
    // If we have padding, ensure that the last pooling starts strictly
    // inside the image (instead of at the padding); otherwise clip the last.
    if ((output_h - 1) * stride_h_ >= input_h + pad_h_) {
      --output_h;
    }
    if ((output_w - 1) * stride_w_ >= input_w + pad_w_) {
      --output_w;
    }
    CHECK_LT((output_h - 1) * stride_h_, input_h + pad_h_);
    CHECK_LT((output_w - 1) * stride_w_, input_w + pad_w_);
  }
#if 0
  top[0]->Reshape(bottom_blob->data->f, channels_, output_h, output_w);// set output mapsize
  //top[0]->NonNeg_ = true;
  if (top.size() > 1) {
    top[1]->ReshapeLike(*top[0]);
  }
  // If MAX pooling, we will initialize the vector index part.
  if (LARG(pool) == Method_MAX && top.size() == 1) {
    max_idx_.Reshape(bottom_blob->num(), channels_, output_h,
        output_w);
  }
  // If stochastic pooling, we will initialize the random index part.
  if (LARG(pool) ==
      Method_STOCHASTIC) {
    rand_idx_.Reshape(bottom_blob->num(), channels_, output_h,
      output_w);
  }

  kernel_size_ = kernel_h;
  stride_ = stride_h_;
  pad_ = pad_h_;
  channels_ = bottom_blob->channels();

  if ( (LARG(pool) == Method_DEF) && Flag_ )//set flag in layer_setup
  {
	 // LOG(INFO) << "Deformation layer setup";
	  blobl_a_min_ = thilayer_param_.pooling_param().blobs_a_min();  //read from prototxt, set the mininum of [c1 c2 c3 c4]
      N_ = input_w * input_h;   //the input mapsize
      // for deformation layer
      thiblobs_.resize(1); // This blos stores the defw1-defw4
      thiblobs_[0].reset(new Blob(1, 1, channels_, 4));   //for [c1 c2 c3 c4]
      
      Dtype* defw = thiblobs_[0]->mutable_cpu_data();
      LOG(INFO) << "top_buffer_.Reshape" << bottom_blob->num() <<" " << channels_ <<" "<< input_h <<" "<< input_w;
      
      top_buffer_.Reshape(bottom_blob->num(), channels_, input_h, input_w);   //blob data
      LOG(INFO) << "blobl_a_min_:" << blobl_a_min_;
/*       LOG(INFO) << "top_buffer_.mutable_cpu_data";     
      Dtype* top_buffer_data = top_buffer_.mutable_cpu_data();*/
      
      dh_.Reshape(bottom_blob->num(), channels_, 1, 1);   //use to represent the dx
      dv_.Reshape(bottom_blob->num(), channels_, 1, 1);   //dy, which use to calculate the gradient in bp
      Iv_.resize((N_ * channels_*bottom_blob->num() ));     // use to represent the distance in dt
      Ih_.resize((N_ * channels_*bottom_blob->num() ));
      tmpIx_.resize((N_ ));              //use in dt, first calculate horizontal, vertical
      tmpIy_.resize((N_ ));
      defh_.resize((channels_ ));   //   use to represent the part location? yes or no
      defv_.resize((channels_ ));   //
      defp_.resize((channels_ ));  //    h*height + v
      Mdt_.resize((N_));                 //    to save the results in dt
      tmpM_.resize(N_);                 //use in LOW_Res
/*      Iv_.reset(new SyncedMemory(N_ * channels_*bottom_blob->num() * sizeof(int)));
      Ih_.reset(new SyncedMemory(N_ * channels_*bottom_blob->num() * sizeof(int)));
      tmpIx_.reset(new SyncedMemory(N_ * sizeof(int)));
      tmpIy_.reset(new SyncedMemory(N_ * sizeof(int)));
      defh_.reset(new SyncedMemory(channels_ * sizeof(int)));
      defv_.reset(new SyncedMemory(channels_ * sizeof(int)));
      defp_.reset(new SyncedMemory(channels_ * sizeof(int)));
      Mdt_.reset(new SyncedMemory(N_ * sizeof(Dtype)));
      tmpM_.reset(new SyncedMemory(N_ * sizeof(Dtype)));
*/      parts_num_ = 9;     // part
      CHECK_EQ(parts_num_, 9);
      CHECK_EQ(channels_ % parts_num_, 0);
     for (int p = 0; p < parts_num_/3; ++p)
      {
          // 0 1
          // 2 3
          hpos_[p*3] = floor((p*input_h)/3) + 1;  // save the part start location
          hpos_[p*3+1] = floor((p*input_h)/3) + 1;
          hpos_[p*3+2] = floor((p*input_h)/3) + 1;
          vpos_[p*3] = 0 + 1;
          vpos_[p*3+1] = floor((1*input_w)/3) + 1;
          vpos_[p*3+2] = floor((2*input_w)/3)  + 1;
      }

    int* defh = defh_.data();
    int* defv = defv_.data();
    int* defp = defp_.data();


      for (int c = 0; c < channels_; c++)
      {
          int c1 = c % parts_num_;
          defh[c] = hpos_[c1];   //select one part
          defv[c] = vpos_[c1];
//          LOG(INFO) << "hpos_[" <<c <<"]: " << hpos_[c1]<< " vpos_[" <<c <<"]: " << vpos_[c1] << " input_w:" << input_w;
          defp[c] = defh[c1] * input_w+defv[c1];  // calculate the positon, which will be used in find the proper postion to get data
//          LOG(INFO) << "defp[" <<c1 <<"]: " << defp[c1];
          defw[c*4+0] = blobl_a_min_; // it is same as setting in matlab
          defw[c*4+1] = Dtype(0);
          defw[c*4+2] = blobl_a_min_;
          defw[c*4+3] = Dtype(0);
      }
      LOG(INFO) << "setup defw:" << defw[0*4+0] << ", "<< defw[0*4+1]<< ", " << defw[0*4+2]<< ", " << defw[0*4+3];
      output_w = 1;
      output_h = 1;
      top[0]->Reshape(bottom_blob->num(), channels_, output_h,
              output_w);
  }
  else
  { 
    if (Flag_)
    {   
      if ( (LARG(pool) == Method_DEF_ALL) || (LARG(pool) == Method_DEF_ALL2) || (LARG(pool) == Method_DEF_ALL3) || (LARG(pool) == Method_DEF_ALL4))
      {
        if(LARG(pool) == Method_DEF_ALL){
          output_h = output_h -1;
          output_w = output_w -1;
        }
        
        
        //  LOG(INFO)<<"this is the start in pool_reshape";
        int Nparam;
        //		  LOG(INFO) << "Deformation layer 2 setup";
        blobl_a_min_ = thilayer_param_.pooling_param().blobs_a_min();
        N_ = input_w * input_h;  //each map size
        // for deformation layer
        //		  LOG(INFO) << "resize";
        thiblobs_.resize(1); // This blos stores the defw1-defw4
        
        if ( (LARG(pool) == Method_DEF_ALL3) || (LARG(pool) == Method_DEF_ALL4) )
          Nparam = kernel_size_*kernel_size_;     //set kernel size
        else
          Nparam = 4;
        
        //		  LOG(INFO) << "reset: "<<channels_ <<", " << Nparam;
        thiblobs_[0].reset(new Blob(1, 1, channels_, Nparam));   //used to represent defw
        
        //		  LOG(INFO) << "Reshape mutable_cpu_data ";
        Dtype* defw = thiblobs_[0]->mutable_cpu_data();
        //		  LOG(INFO) << "top_buffer_.Reshape" << bottom_blob->num() <<" " << channels_ <<" "<< input_h <<" "<< input_w;
        
        //		  LOG(INFO) << "Reshape 1 ";
        top_buffer_.Reshape(bottom_blob->num(), channels_, input_h, input_w);
        //		  LOG(INFO) << "blobl_a_min_:" << blobl_a_min_;
        //		  LOG(INFO) << "Nparam:" << Nparam << "channels:" << channels_ << "output_h: " << output_h << "output_w:" << output_w;
        //		  LOG(INFO) << "Reshape 2";
        dh_.Reshape(bottom_blob->num(), channels_, output_h, output_w);
        dv_.Reshape(bottom_blob->num(), channels_, output_h, output_w);
        //		  dh2_.Reshape(bottom_blob->num(), channels_, output_h, output_w);
        //		  dv2_.Reshape(bottom_blob->num(), channels_, output_h, output_w);
        
        //  LOG(INFO)<<"this is the start 2 in pool_reshape";
        if ( (LARG(pool) == Method_DEF_ALL3) || (LARG(pool) == Method_DEF_ALL4))
        {
          //              Iv_.resize((channels_*bottom_blob->num()*output_h*output_w ));
          Ih_.resize((channels_*bottom_blob->num()*output_h*output_w ));   //
        }
        else
        {
          Iv_.resize((channels_*bottom_blob->num()*input_h*input_w ));   //each point in  inputmap
          Ih_.resize((channels_*bottom_blob->num()*input_h*input_w ));
        }
        //  LOG(INFO)<<"this is the start 3 in pool_reshape";
        //		  LOG(INFO) << "tmpIx_";
        tmpIx_.resize((N_ ));  // for one map, n = width * height
        tmpIy_.resize((N_ ));
        defh_.resize((channels_ )); // part location
        defv_.resize((channels_ ));
        defp_.resize((channels_ ));
        Mdt_.resize((N_));
        tmpM_.resize(N_);
        
        LOG(INFO) << "setting defw" << " blobl_a_min_:" << blobl_a_min_;
        if ( (LARG(pool) == Method_DEF_ALL3) || (LARG(pool) == Method_DEF_ALL4) )
          for (int c = 0; c < channels_; c++)
          {
            int defwidx = 0;
            int center = kernel_size_/2;
            for (int kh=0; kh<kernel_size_; kh++) {
              for (int kv=0; kv<kernel_size_; kv++)
              {
                if (blobl_a_min_ > 0)
                  defw[c*Nparam+defwidx] = -blobl_a_min_* ( (kh-center)*(kh-center) + (kv-center)*(kv-center) );
                else
                  defw[c*Nparam+defwidx] = 0;
                defwidx++;
              }
            }
              //				  for (int parami = 0; parami < Nparam; ++parami)
              //					  defw[c*Nparam+parami] = Dtype(0);
              //		  LOG(INFO) << "defwidx: " <<defwidx;
          }
          else
            for (int c = 0; c < channels_; c++)
            {
              defw[c*4+0] = blobl_a_min_;
              defw[c*4+1] = Dtype(0);
              defw[c*4+2] = blobl_a_min_;
              defw[c*4+3] = Dtype(0);
            }
            
            LOG(INFO) << "setup defw:" << defw[0*4+0] << ", "<< defw[0*4+1]<< ", " << defw[0*4+2]<< ", " << defw[0*4+3];
            //		  top[0]->Reshape(bottom_blob->num(), channels_, output_h,
            //				  output_w);
            top[0]->Reshape(bottom_blob->num(), output_h * output_w , 1, 1);
      }
      else
      {
        if ( (LARG(pool) == Method_LOWRES) )
        {
          CHECK_EQ(channels_, 3)  << "PoolingLayer LOWRES only takes 3 channels";
          N_ = input_w * input_h;
          tmpM_.resize(N_*channels_);
          top[0]->Reshape(bottom_blob->num(), channels_*2+1, output_h,
            output_w);
        }
        else
        {
          top[0]->Reshape(bottom_blob->num(), channels_, output_h,
            output_w);
        }
      }
    }
  }
  Flag_ = false;
#endif

  switch (op) {
  case LO_Init:
    break;
  case LO_Forward:
    {
      Blob_create(top_blob->data, output_w, output_h, channels_, bottom_blob->data->f);
      const Dtype* bottom_data = Blob_datap(bottom_blob->data);
      Dtype* top_data = Blob_datap(top_blob->data);
      const int top_count = Blob_count(top_blob->data);
      int* mask = NULL;
      // suppress warnings about uninitalized variables
      // Different pooling methods. We explicitly do the switch outside the for
      // loop to save time, although this results in more code.
      switch (LARG(pool)) {
      case Method_MAXI:
      case Method_MAX:
        // Initialize
        Blob_ReshapeLike(top_blob->im, top_blob->data);
        mask = (int*)Blob_datap(top_blob->im);
        blas_set(top_count, -1, mask);
        blas_set(top_count, (-FLT_MAX), top_data);
        // The main loop
        for (n = 0; n < bottom_blob->data->f; ++n) {
          for (c = 0; c < channels_; ++c) {
            for (ph = 0; ph < output_h; ++ph) {
              for (pw = 0; pw < output_w; ++pw) {
                int hstart = ph * stride_h_ - pad_h_;
                int wstart = pw * stride_w_ - pad_w_;
                int hend = MIN(hstart + kernel_h, input_h);
                int wend = MIN(wstart + kernel_w, input_w);
                hstart = MAX(hstart, 0);
                wstart = MAX(wstart, 0);
                const int out_index = ph * output_w + pw;
                //const int index = hstart * input_w + wstart;                top_data[out_index] = bottom_data[index];                mask[out_index] = index;
                for (int h = hstart; h < hend; ++h) {
                  for (int w = wstart; w < wend; ++w) {
                    const int index = h * input_w + w;
                    if (bottom_data[index] > top_data[out_index]) {
                      top_data[out_index] = bottom_data[index];
                      mask[out_index] = index;
                    }
                  }
                }
              }
            }
            // compute offset
            bottom_data += bottom_pixnum;
            top_data += top_pixnum;
            mask += top_pixnum;
          }
        }
        break;
      case Method_AVE:
        for (i = 0; i < top_count; ++i) {
          top_data[i] = 0;
        }
        // The main loop
        for (n = 0; n < bottom_blob->data->f; ++n) {
          for (c = 0; c < channels_; ++c) {
            for (ph = 0; ph < output_h; ++ph) {
              for (pw = 0; pw < output_w; ++pw) {
                int hstart = ph * stride_h_ - pad_h_;
                int wstart = pw * stride_w_ - pad_w_;
                int hend = MIN(hstart + kernel_h, input_h + pad_h_);
                int wend = MIN(wstart + kernel_w, input_w + pad_w_);
                int pool_size = (hend - hstart) * (wend - wstart);
                hstart = MAX(hstart, 0);
                wstart = MAX(wstart, 0);
                hend = MIN(hend, input_h);
                wend = MIN(wend, input_w);
                for (int h = hstart; h < hend; ++h) {
                  for (int w = wstart; w < wend; ++w) {
                    top_data[ph * output_w + pw] += bottom_data[h * input_w + w];
                  }
                }
                top_data[ph * output_w + pw] /= pool_size;
              }
            }
            // compute offset
            bottom_data += bottom_pixnum;
            top_data += top_pixnum;
          }
        }
        break;
      default:
        LOG(GLOG_FATAL, "Unknown pooling method.");
        break;
      }
    }
    break;
  case LO_Backward:
    {
      Blob_ReshapeLike(bottom_blob->diff, bottom_blob->data);
      const Dtype* top_diff = Blob_datap(top_blob->diff);
      Dtype* bottom_diff = Blob_datap(bottom_blob->diff);
      // Different pooling methods. We explicitly do the switch outside the for
      // loop to save time, although this results in more codes.
      blas_set(Blob_count(bottom_blob->diff), (0), bottom_diff);
      // We'll output the mask to top[1] if it's of size >1.
      const int* mask = NULL;  // suppress warnings about uninitialized variables
      switch (LARG(pool)) {
      case Method_MAX:
        // The main loop
        mask = (int*)Blob_datap(top_blob->im);
        for (n = 0; n < top_blob->diff->f; ++n) {
          for (c = 0; c < channels_; ++c) {
            for (ph = 0; ph < output_h; ++ph) {
              for (pw = 0; pw < output_w; ++pw) {
                const int index = ph * output_w + pw;
                const int bottom_index = mask[index];
                bottom_diff[bottom_index] += top_diff[index];
              }
            }
            bottom_diff += bottom_pixnum;
            top_diff += top_pixnum;
            mask += top_pixnum;
          }
        }
        break;
      case Method_AVE:
        // The main loop
        for (n = 0; n < top_blob->diff->f; ++n) {
          for (c = 0; c < channels_; ++c) {
            for (ph = 0; ph < output_h; ++ph) {
              for (pw = 0; pw < output_w; ++pw) {
                int hstart = ph * stride_h_ - pad_h_;
                int wstart = pw * stride_w_ - pad_w_;
                int hend = MIN(hstart + kernel_h, input_h + pad_h_);
                int wend = MIN(wstart + kernel_w, input_w + pad_w_);
                int pool_size = (hend - hstart) * (wend - wstart);
                hstart = MAX(hstart, 0);
                wstart = MAX(wstart, 0);
                hend = MIN(hend, input_h);
                wend = MIN(wend, input_w);
                for (int h = hstart; h < hend; ++h) {
                  for (int w = wstart; w < wend; ++w) {
                    bottom_diff[h * input_w + w] += top_diff[ph * output_w + pw] / pool_size;
                  }
                }
              }
            }
            // offset
            bottom_diff += bottom_pixnum;
            top_diff += top_pixnum;
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



#if 0

#define INF 1E20
static inline int square(int x) { return x*x; }


void max_filter_1d(const Dtype *vals, Dtype *out_vals, int *I, 
                          int step, int n, Dtype a, Dtype b, int s) {
  for (int i = 0; i < n; i++) {
    Dtype max_val = -INF;
    int argmax     = 0;
    int first      = MAX(0, i-s);
    int last       = MIN(n-1, i+s);
    for (int j = first; j <= last; j++) {
      Dtype val = *(vals + j*step) - a*square(i-j) - b*(i-j);
      if (val > max_val) {
        max_val = val;
        argmax  = j;
      }
    }
    *(out_vals + i*step) = max_val;
    *(I + i*step) = argmax;
  }
}



void max_filter_1d(Dtype *vals, Dtype *out_vals, int *I, 
                          int step, int n, Dtype a, Dtype b, int s) {
  for (int i = 0; i < n; i++) {
    Dtype max_val = -INF;
    int argmax     = 0;
    int first      = MAX(0, i-s);
    int last       = MIN(n-1, i+s);
    for (int j = first; j <= last; j++) {
      Dtype val = *(vals + j*step) - a*square(i-j) - b*(i-j);
      if (val > max_val) {
        max_val = val;
        argmax  = j;
      }
    }
    *(out_vals + i*step) = max_val;
    *(I + i*step) = argmax;
  }
}


void dt1d(Dtype *src, Dtype *dst, int *ptr, int step, int n, Dtype a, Dtype b, int * v, Dtype *z) {
  int k = 0;
  v[0] = 0;
  z[0] = -INF;
  z[1] = +INF;
  for (int q = 1; q <= n-1; q++) {
    Dtype s = ((src[q*step] - src[v[k]*step]) - b*(q - v[k]) + a*(square(q) - square(v[k]))) / (2*a*(q-v[k]));
    while (s <= z[k]) {
      // Update pointer
      k--;
      s  = ((src[q*step] - src[v[k]*step]) - b*(q - v[k]) + a*(square(q) - square(v[k]))) / (2*a*(q-v[k]));
    }
    k++;
    v[k]   = q;
    z[k]   = s;
    z[k+1] = +INF;
  }

   k = 0;
   for (int q = 0; q <= n-1; q++) {
     while (z[k+1] < q)
       k++;
     dst[q*step] = a*square(q-v[k]) + b*(q-v[k]) + src[v[k]*step];
     ptr[q*step] = v[k];
  }

}

void dt1d(const Dtype *src, Dtype *dst, int *ptr, int step, int n, Dtype a, Dtype b, int * v, Dtype *z) {
  int k = 0;
  v[0] = 0;
  z[0] = -INF;
  z[1] = +INF;
  for (int q = 1; q <= n-1; q++) {
    Dtype s = ((src[q*step] - src[v[k]*step]) - b*(q - v[k]) + a*(square(q) - square(v[k]))) / (2*a*(q-v[k]));
    while (s <= z[k]) {
      // Update pointer
      k--;
      s  = ((src[q*step] - src[v[k]*step]) - b*(q - v[k]) + a*(square(q) - square(v[k]))) / (2*a*(q-v[k]));
    }
    k++;
    v[k]   = q;
    z[k]   = s;
    z[k+1] = +INF;
  }

   k = 0;
   for (int q = 0; q <= n-1; q++) {
     while (z[k+1] < q)
       k++;
     dst[q*step] = a*square(q-v[k]) + b*(q-v[k]) + src[v[k]*step];
     ptr[q*step] = v[k];
  }
}

void PoolingLayer::dt(int dims0, int dims1, const Dtype * vals, Dtype av, Dtype bv, Dtype ah, Dtype bh, int n, int ch) { 
  
  // Read in deformation coefficients, negating to define a cost
  // Read in offsets for output grid, fixing MATLAB 0-1 indexing
  
  Dtype* tmpM = tmpM_.data();
  Dtype* Mdt = Mdt_.data();
  int* tmpIy = tmpIy_.data();
  int* tmpIx = tmpIx_.data();
  int* Ih = Ih_.data();
  int* Iv = Iv_.data();
  int maxdim = dims0;
  if (dims0 < dims1)
    maxdim = dims1;
  int   *vp = new int[maxdim];
  Dtype *zp = new Dtype[maxdim+1];
  
  Ih = Ih + (n*channels_+ch)*dims0*dims1;
  Iv = Iv + (n*channels_+ch)*dims0*dims1;
  
  if (LARG(pool) == Method_DEF)
  {
    for (int h = 0; h < dims1; h++)
      //for each vertical location, scan horizontally
      dt1d(vals+h*dims0, tmpM+h*dims0, tmpIy+h*dims0, 1, dims0, -av, -bv, vp, zp);  
    for (int v = 0; v < dims0; v++)
      //for each horizontal location, scan vertically
      dt1d(tmpM+v, Mdt+v, tmpIx+v, dims0, dims1, -ah, -bh, vp, zp);
  }
  else
  {
    for (int h = 0; h < dims1; h++)
      //for each vertical location, scan horizontally
      max_filter_1d(vals+h*dims0, tmpM+h*dims0, tmpIy+h*dims0, 1, dims0, -av, -bv, kernel_size_);  
    for (int v = 0; v < dims0; v++)
      //for each horizontal location, scan vertically
      max_filter_1d(tmpM+v, Mdt+v, tmpIx+v, dims0, dims1, -ah, -bh, kernel_size_);
  }
  delete [] vp;
  delete [] zp;
  
  for (int h = 0; h < dims1; h++) {
    for (int v = 0; v < dims0; v++) {
      int p = h*dims0+v;
      
      Ih[p] = tmpIx[p]; // store the best in vertical direction
      
      Iv[p] = tmpIy[v+tmpIx[p]*dims0]; // store the best in horizontal direction
    }
  }
}



#define	round(x)	((x-floor(x))>0.5 ? ceil(x) : floor(x))

// reduce(im) resizes im to half its size, using a 5-tap binomial filter for anti-aliasing
// (see Burt & Adelson's Laplacian Pyramid paper)

// reduce each column
// result is transposed, so we can apply it twice for a complete reduction

void PoolingLayer::reduce1dtran(Dtype *src, int sheight, Dtype *dst, int dheight, 
                                int width, int chan) {
  // resize each column of each color channel
  //bzero(dst, chan*width*dheight*sizeof(double));
  memset(dst, 0, chan*width*dheight*sizeof(Dtype));
  int y;
  Dtype *s, *d;
  
  for (int c = 0; c < chan; c++) {
    for (int x = 0; x < width; x++) {
      s  = src + c*width*sheight + x*sheight;
      d  = dst + c*dheight*width + x;
      
      // First row
      *d = s[0]*.6875 + s[1]*.2500 + s[2]*.0625;      
      
      for (y = 1; y < dheight-2; y++) {	
        s += 2;
        d += width;
        *d = s[-2]*0.0625 + s[-1]*.25 + s[0]*.375 + s[1]*.25 + s[2]*.0625;
      }
      
      // Last two rows
      s += 2;
      d += width;
      if (dheight*2 <= sheight) {
        *d = s[-2]*0.0625 + s[-1]*.25 + s[0]*.375 + s[1]*.25 + s[2]*.0625;
      } else {
        *d = s[1]*.3125 + s[0]*.3750 + s[-1]*.2500 + s[-2]*.0625;
      }
      s += 2;
      d += width;
      *d = s[0]*.6875 + s[-1]*.2500 + s[-2]*.0625;
    }
  }
}


void PoolingLayer::reduce1dtran(const Dtype *src, int sheight, Dtype *dst, int dheight, 
                                int width, int chan) {
  // resize each column of each color channel
  //bzero(dst, chan*width*dheight*sizeof(double));
  memset(dst, 0, chan*width*dheight*sizeof(Dtype));
  int y;
  Dtype *d;
  const Dtype *s;
  
  for (int c = 0; c < chan; c++) {
    for (int x = 0; x < width; x++) {
      s  = src + c*width*sheight + x*sheight;
      d  = dst + c*dheight*width + x;
      
      // First row
      *d = s[0]*.6875 + s[1]*.2500 + s[2]*.0625;      
      
      for (y = 1; y < dheight-2; y++) {	
        s += 2;
        d += width;
        *d = s[-2]*0.0625 + s[-1]*.25 + s[0]*.375 + s[1]*.25 + s[2]*.0625;
      }
      
      // Last two rows
      s += 2;
      d += width;
      if (dheight*2 <= sheight) {
        *d = s[-2]*0.0625 + s[-1]*.25 + s[0]*.375 + s[1]*.25 + s[2]*.0625;
      } else {
        *d = s[1]*.3125 + s[0]*.3750 + s[-1]*.2500 + s[-2]*.0625;
      }
      s += 2;
      d += width;
      *d = s[0]*.6875 + s[-1]*.2500 + s[-2]*.0625;
    }
  }
}


#endif
