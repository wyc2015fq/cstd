
static void FUN(conv2d)(const Dtype* inData, float* outData, const Dtype* weights, const Dtype* biasData,
  DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
  int dilation_h, int dilation_w, int pad_h, int pad_w, int group_, bool cross_correlation) {
  // group_
  int o_c = outSize.c;
  int o_g = outSize.c / group_;
  int k_g = inSize.c / group_;
  int o_head, k_head;
  int n, g, o, k, y, x, p, q;
  // Convolution
  for (n = 0; n < outSize.n; n++) {
    for (g = 0; g < group_; g++) {
      o_head = o_g * g;
      k_head = k_g * g;
      for (o = 0; o < o_g; o++) {
        for (k = 0; k < k_g; k++) {
          for (y = 0; y < outSize.h; y++) {
            for (x = 0; x < outSize.w; x++) {
              int out_offset = ((n*o_g + o + o_head)*outSize.h + y)*outSize.w + x;
              const Dtype* w = weights + ((o + o_head)*k_g + k)*kernel_h*kernel_w;
              for (p = 0; p < kernel_h; p++) {
                for (q = 0; q < kernel_w; q++) {
                  int in_y = y * stride_h - pad_h + p * dilation_h;
                  int in_x = x * stride_w - pad_w + q * dilation_w;
                  if (is_a_ge_zero_and_a_lt_b(in_y, inSize.h) && is_a_ge_zero_and_a_lt_b(in_x, inSize.w)) {
                    int weight_offset = cross_correlation ? (((o + o_head)*k_g + k)*kernel_h + p)*kernel_w + q
                      : (((o + o_head)*k_g + k)*kernel_h + kernel_h - 1 - p)*kernel_w + kernel_w - 1 - q;
                    int in_offset = ((n*k_g + k + k_head)*inSize.h + in_y)*inSize.w + in_x;
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
            int out_offset = ((n*o_c + o)*outSize.h + y)*outSize.w + x;
            outData[out_offset] += biasData[o];
          }
        }
      }
    }
  }
}

#if 1

void FUN(rev)(int count, Dtype* f) {
  Dtype* e = f + count;
  for (; f < e--;++f) {
    Dtype t1 = *f;
    Dtype t2 = *e;
    *e = t1;
    *f = t2;
  }
}
void FUN(rev2d)(int N, int M, Dtype* f) {
  int i;
  for (i=0; i<N; ++i) {
    FUN(rev)(M, f);
    f += M;
  }
}

static void conv2d_gemm_fw(const Dtype* inData, float* outData, const Dtype* weights, const Dtype* biasData,
  DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
  int dilation_h, int dilation_w, int pad_h, int pad_w, int group_, bool cross_correlation) {
  const Dtype* col_buff = inData;
  Dtype* col_buffer_ = NULL;
  bool skip_im2col = false;
  int conv_out_channels_ = outSize.c;
  int conv_out_spatial_dim_ = outSize.h*outSize.w;
  int kernel_dim_ = kernel_h*kernel_w*inSize.c;
  int col_buffer_size_ = kernel_dim_ * group_ * conv_out_spatial_dim_;
  int weight_offset_ = conv_out_channels_ * kernel_dim_ / group_;
  int output_offset_ = conv_out_channels_ * conv_out_spatial_dim_ / group_;
  int col_offset_ = kernel_dim_ * conv_out_spatial_dim_;
  bool is_1x1_ = true;
  Dtype* inv_kernel = NULL;
  is_1x1_ &= kernel_h == 1 && stride_h == 1 && pad_h == 0;
  is_1x1_ &= kernel_w == 1 && stride_w == 1 && pad_w == 0;
  if (1) {
    int n = outSize.c*inSize.c, m=kernel_h*kernel_w;
    inv_kernel = (Dtype*)caffe_malloc(n*m*sizeof(Dtype));
    memcpy(inv_kernel, weights, n*m*sizeof(Dtype));
    FUN(rev2d)(n, m, inv_kernel);
    weights = inv_kernel;
  }

  if (!is_1x1_) {
    if (!skip_im2col) {
      col_buffer_ = (Dtype*)caffe_malloc(col_buffer_size_*sizeof(Dtype));
      im2col(inData, inSize.c,inSize.h, inSize.w,kernel_h, kernel_w,pad_h, pad_w,stride_h, stride_w,dilation_h, dilation_w, col_buffer_);
    }
    col_buff = col_buffer_;
  }
  for (int g = 0; g < group_; ++g) {
    caffe_gemm(CblasNoTrans, CblasNoTrans, conv_out_channels_ / group_, conv_out_spatial_dim_, kernel_dim_,
      (Dtype)1., weights + weight_offset_ * g, col_buff + col_offset_ * g, (Dtype)0., outData + output_offset_ * g);
  }
  if (col_buffer_) { caffe_free(col_buffer_); }
  if (inv_kernel) { caffe_free(inv_kernel); }
}

#endif