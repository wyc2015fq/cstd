
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

static void conv_f(float* dst, int dst_step, ISIZE dstSize, const float* weight_data, ISIZE mapSize, const float* src, int src_step, bool inv) {
  int i, j, r, c;
  if (inv) {
    for(j=0;j<dstSize.h;j++) {
      float* dst1 = dst + j*dst_step;
      for(i=0;i<dstSize.w;i++) {
        float sum=0;
        const float* map1 = weight_data+mapSize.w*mapSize.h;
        for(r=0;r<mapSize.h;r++) {
          const float* src1 = src + (j+r)*src_step + i;
          for(c=0;c<mapSize.w;c++) {
            sum+=*(--map1) *src1[c];
          }
        }
        dst1[i] += sum;
      }
    }
  } else {
    for(j=0;j<dstSize.h;j++) {
      float* dst1 = dst + j*dst_step;
      for(i=0;i<dstSize.w;i++) {
        float sum=0;
        const float* map1 = weight_data;
        for(r=0;r<mapSize.h;r++) {
          const float* src1 = src + (j+r)*src_step + i;
          for(c=0;c<mapSize.w;c++) {
            dst1[i]+=(*map1++) *src1[c];
          }
        }
        dst1[i] += sum;
      }
    }
    
  }
}

#define _mm_muladd_ps(a, b, c)  a = _mm_add_ps(a, _mm_mul_ps(b, c))

#include "impl/convolution_1x1.h"
#include "impl/convolution_2x2.h"
#include "impl/convolution_3x3.h"
#include "impl/convolution_5x5.h"

#define CONV3X3S1_NEON_ARGS(args1, j, OPT_DEF) \
	OPT_DEF(args1, j, const img_t*, bottom_data) \
	OPT_DEF(args1, j, img_t*, top_blob) \
	OPT_DEF(args1, j, const float*, _kernel) \
	OPT_DEF(args1, j, const float*, _bias) \

int sconv2d_valid(int h, int w, const float* src, int src_step,int srccn, float* dst, int dst_step, int dstcn,
 int kernel_h, int kernel_w, const float* kernel, const float* bias, int stride_h, int stride_w, int pad_h, int pad_w, int dilation) {
  typedef void (*sconv_func)(int h, int w, const float* src, int src_step, int srccn, float* dst, int dst_step, int dstcn, const float* kernel, const float* bias);
  sconv_func conv = NULL;
  int outh = h - kernel_h + 1;
  int outw = w - kernel_w + 1;
  // kernel_size x stride
  if (kernel_h==kernel_w && stride_h==stride_w && pad_h==pad_w && pad_w==0) {
    static sconv_func conv_func_table[8][4] = {0};
    static int inited = 0;
    int kernel_size = kernel_w;
    int stride = stride_w;
    int pad = pad_w;
  
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

//#include "arm/convolution_5x5.h"
static int convolution_forward(const img_t* src, img_t* dst, const Weight* weight, const float* bias_data CC_DEFAULT(NULL), int dilation CC_DEFAULT(1))
{
  // convolv with NxN kernel
  // value = value + bias
  img_t im[1] = {0};
  int w = src->w;
  int h = src->h;
  int inch = pBox_channels(src);
  int kernel_h = weight->kernel_h;
  int kernel_w = weight->kernel_w;
  int stride_h = weight->stride_h;
  int stride_w = weight->stride_w;
  int pad = weight->pad;
  int num_output = weight->selfChannel;
  ASSERT(kernel_h>0);
  ASSERT(kernel_w>0);
  ASSERT(inch>0);
  stride_h = MAX(1, stride_h);
  stride_w = MAX(1, stride_w);
  dilation = MAX(1, dilation);
  pad = MAX(0, pad);

  //     fprintf(stderr, "Convolution input %d x %d  pad = %d  ksize=%d  stride=%d\n", w, h, pad, kernel_size, stride);
  const int kernel_extent_h = dilation * (kernel_h - 1) + 1;
  const int kernel_extent_w = dilation * (kernel_w - 1) + 1;

  const img_t* bottom_blob_bordered = src;

  if (pad > 0) {
    bottom_blob_bordered = copy_make_border(src, im, pad, pad, pad, pad, BORDER_CONSTANT, 0.f);
  }
  else if (pad == -233) {
    int pad_w = kernel_extent_h + (w - 1) / stride_w * stride_w - w;
    int pad_h = kernel_extent_w + (h - 1) / stride_h * stride_h - h;

    if (pad_w > 0 || pad_h > 0) {
      bottom_blob_bordered = copy_make_border(src, im, pad_h / 2, pad_h - pad_h / 2, pad_w / 2, pad_w - pad_w / 2, BORDER_CONSTANT, 0.f);
    }
  }
  w = bottom_blob_bordered->w;
  h = bottom_blob_bordered->h;
  
  int out_w = (w - kernel_extent_w) / stride_w + 1;
  int out_h = (h - kernel_extent_h) / stride_h + 1;
  pBox_create(dst, out_w, out_h, num_output, src->f);
  
  const float* _kernel = weight->data->p;
  const float* _bias = bias_data ? bias_data : NULL;
  const float* src_data = pBox_datap(src);
  float* dst_data = pBox_datap(dst);
  int srccn = pBox_channels(src);
  
  sconv2d_valid(src->h, src->w, src_data, src->w, srccn, dst_data, dst->w, num_output, kernel_h, kernel_w,
    weight->data->p, bias_data, stride_h, stride_w, pad, pad, dilation);
  imfrees2(im);
  return 0;
}

static int Convolution(enum LayerOp op, const struct Layer* layer, struct Net* net)
{
  const img_t* src = LBLOB(0)->data;
  img_t* dst = LBLOB(1)->data;
  float* weight_data = pBox_datap(WBLOB(0)->data);
  float* bias_data = pBox_datap(WBLOB(1)->data);
  Weight weight[1] = {0};
  ASSERT(WBLOB(0)->data->h==LARG(kernel_h));
  ASSERT(WBLOB(0)->data->w==LARG(kernel_w));
  ASSERT(WBLOB(0)->data->f==LARG(num_output));
  weight->kernel_h = LARG(kernel_h);
  weight->kernel_w = LARG(kernel_w);
  weight->stride_h = LARG(stride_h);
  weight->pad = LARG(pad);
  weight->selfChannel = LARG(num_output);
  weight->data->p = weight_data;
  convolution_forward(src, dst, weight, bias_data, LARG(dilation));
  return 0;
}

// wrap im2col/col2im so we don't have to remember the (long) argument lists
inline void conv_im2col_cpu(const pBox* im, Dtype* col_buff, int kernel_h, int kernel_w, int stride_h, int stride_w, int pad, int dilation) {
  Dtype* data = pBox_datap(im);
  int channels_ = pBox_channels(im);
  im2col_cpu(data, channels_, im->h, im->w, kernel_h, kernel_w, stride_h, stride_w, pad, pad, dilation, dilation, col_buff);
}

inline void conv_col2im_cpu(const Dtype* col_buff, pBox* im, int kernel_h, int kernel_w, int stride_h, int stride_w, int pad, int dilation) {
  Dtype* data = pBox_datap(im);
  int channels_ = pBox_channels(im);
  col2im_cpu(col_buff, channels_, im->h, im->w,kernel_h, kernel_w, stride_h, stride_w, pad, pad,dilation, dilation, data);
}

#ifndef CPU_ONLY
inline void conv_im2col_gpu(BaseConvolutionLayer* s, const Dtype* data, Dtype* col_buff) {
  if (!s->force_nd_im2col_ && s->num_spatial_axes_ == 2) {
    im2col_gpu(data, s->conv_in_channels_,
      s->conv_input_shape_[1], s->conv_input_shape_[2],
      s->kernel_[0], s->kernel_[1],
      s->pad[0], s->pad[1],
      s->stride_[0], s->stride_[1],
      s->dilation[0], s->dilation[1], col_buff);
  } else {
    im2col_nd_gpu(data, num_spatial_axes_, num_kernels_im2col_,
      conv_input_shape_.gpu_data(), col_buffer_.gpu_shape(),
      kernel_.gpu_data(), pad.gpu_data(),
      stride_.gpu_data(), dilation.gpu_data(), col_buff);
  }
}
inline void conv_col2im_gpu(const Dtype* col_buff, Dtype* data) {
  if (!force_nd_im2col_ && num_spatial_axes_ == 2) {
    col2im_gpu(col_buff, conv_in_channels_,
      conv_input_shape_[1], conv_input_shape_[2],
      kernel_[0], kernel_[1],
      pad[0], pad[1],
      stride_[0], stride_[1],
      dilation[0], dilation[1], data);
  } else {
    col2im_nd_gpu(col_buff, s->num_spatial_axes_, s->num_kernels_col2im_,
      s->conv_input_shape_, s->col_buffer_.gpu_shape(),
      s->kernel_, pad, stride_,
      s->dilation, data);
  }
}
#endif

void forward_cpu_gemm(const pBox* input, const Dtype* weights, pBox* output, int kernel_h, int kernel_w, int stride_h, int stride_w, int pad, int dilation) {
  const Dtype* col_buff = pBox_datap(input);
  Dtype* output_data = pBox_datap(output);
  bool is_1x1_ = kernel_h == 1 && kernel_w == 1 && stride_h == 1 && stride_w == 1 && pad == 0;
  Dtype* col_buffer_ = NULL;
  int output_channels_ = pBox_channels(output);
  int input_channels_ = pBox_channels(input);
  int kernel_dim_ = kernel_h*kernel_w * input_channels_;
  int output_dim_ = output->h * output->w;
  int col_buffer_size = output_dim_ * kernel_dim_;
  int group_ = 1;
  if (!is_1x1_) {
    MYREALLOC(col_buffer_, col_buffer_size);
    conv_im2col_cpu(input, col_buffer_, kernel_h, kernel_w, stride_h, stride_w, pad, dilation);
    col_buff = col_buffer_;
  }
  int weight_offset_ = output_channels_ * kernel_dim_ / group_;
  int col_offset_ = output_channels_ * col_buffer_size / group_;
  int output_offset_ = output_channels_ * output_dim_ / group_;
  for (int g = 0; g < group_; ++g) {
    blas_gemm(CblasNoTrans, CblasNoTrans, output_channels_ / group_,
      output_dim_, kernel_dim_,
        (Dtype)1., weights + weight_offset_ * g, col_buff + col_offset_ * g,
        (Dtype)0., output_data + output_offset_ * g);
  }
  FREE(col_buffer_);
}

void backward_cpu_gemm(const pBox* output, const Dtype* weights, pBox* input, int kernel_h, int kernel_w, int stride_h, int stride_w, int pad, int dilation) {
  Dtype* col_buff = pBox_datap(input);
  Dtype* output_data = pBox_datap(output);
  bool is_1x1_ = kernel_h == 1 && kernel_w == 1 && stride_h == 1 && stride_w == 1 && pad == 0;
  Dtype* col_buffer_ = NULL;
  int input_channels_ = pBox_channels(input);
  int kernel_dim_ = kernel_h*kernel_w * input_channels_;
  int output_dim_ = output->h * output->w;
  int col_buffer_size = output_dim_ * kernel_dim_;
  int group_ = 1;
  int output_channels_ = pBox_channels(output);
  if (!is_1x1_) {
    MYREALLOC(col_buffer_, col_buffer_size);
    col_buff = col_buffer_;
  }
  int weight_offset_ = output_channels_ * kernel_dim_ / group_;
  int col_offset_ = output_channels_ * col_buffer_size / group_;
  int output_offset_ = output_channels_ * output_dim_ / group_;
  for (int g = 0; g < group_; ++g) {
    blas_gemm(CblasTrans, CblasNoTrans, kernel_dim_, output_dim_, output_channels_ / group_,
        (Dtype)1., weights + weight_offset_ * g, output_data + output_offset_ * g,
        (Dtype)0., col_buff + col_offset_ * g);
  }
  if (!is_1x1_) {
    conv_col2im_cpu(col_buff, input, kernel_h, kernel_w, stride_h, stride_w, pad, dilation);
  }
  FREE(col_buffer_);
}

void weight_cpu_gemm(const pBox* input, const pBox* output, Dtype* weights, int kernel_h, int kernel_w, int stride_h, int stride_w, int pad, int dilation) {
  const Dtype* col_buff = pBox_datap(input);
  const Dtype* output_data = pBox_datap(output);
  bool is_1x1_ = kernel_h == 1 && kernel_w == 1 && stride_h == 1 && stride_w == 1 && pad == 0;
  Dtype* col_buffer_ = NULL;
  int input_channels_ = pBox_channels(input);
  int kernel_dim_ = kernel_h*kernel_w * input_channels_;
  int output_dim_ = output->h * output->w;
  int col_buffer_size = output_dim_ * kernel_dim_;
  int group_ = 1;
  int output_channels_ = pBox_channels(output);
  if (!is_1x1_) {
    MYREALLOC(col_buffer_, col_buffer_size);
    conv_im2col_cpu(input, col_buffer_, kernel_h, kernel_w, stride_h, stride_w, pad, dilation);
    col_buff = col_buffer_;
  }
  int weight_offset_ = output_channels_ * kernel_dim_ / group_;
  int col_offset_ = output_channels_ * col_buffer_size / group_;
  int output_offset_ = output_channels_ * output_dim_ / group_;
  for (int g = 0; g < group_; ++g) {
    blas_gemm(CblasNoTrans, CblasTrans, output_channels_ / group_,
        kernel_dim_, output_dim_,
        (Dtype)1., output_data + output_offset_ * g, col_buff + col_offset_ * g,
        (Dtype)1., weights + weight_offset_ * g);
  }
  FREE(col_buffer_);
}

void forward_cpu_bias(pBox* output, const Dtype* bias) {
  Dtype* output_data = pBox_datap(output);
  Dtype* bias_multiplier_ = NULL;
  int out_spatial_dim_ = pBox_pixnum(output);
  int num_output_ = pBox_channels(output);
  MYREALLOC(bias_multiplier_, out_spatial_dim_);
  blas_set(out_spatial_dim_, 1, bias_multiplier_);
  blas_gemm(CblasNoTrans, CblasNoTrans, num_output_, out_spatial_dim_, 1, (Dtype)1., bias, bias_multiplier_, (Dtype)1., output_data);
  FREE(bias_multiplier_);
}

void backward_cpu_bias(Dtype* bias, const pBox* input) {
  Dtype* input_data = pBox_datap(input);
  Dtype* bias_multiplier_ = NULL;
  int out_spatial_dim_ = pBox_pixnum(input);
  int num_output_ = pBox_channels(input);
  MYREALLOC(bias_multiplier_, out_spatial_dim_);
  blas_set(out_spatial_dim_, 1, bias_multiplier_);
  blas_gemv(CblasNoTrans, num_output_, out_spatial_dim_, 1., input_data, bias_multiplier_, 1., bias);
  FREE(bias_multiplier_);
}


#ifndef CPU_ONLY


void BaseConvolutionLayer::forward_gpu_gemm(const Dtype* input,
    const Dtype* weights, Dtype* output, bool skip_im2col = false) {
  const Dtype* col_buff = input;
  if (!is_1x1_) {
    if (!skip_im2col) {
      conv_im2col_gpu(input, col_buffer_.mutable_gpu_data());
    }
    col_buff = col_buffer_.gpu_data();
  }
  for (int g = 0; g < group_; ++g) {
    blas_gpu_gemm(CblasNoTrans, CblasNoTrans, output_channels_ /
        group_, output_dim_, kernel_dim_,
        (Dtype)1., weights + weight_offset_ * g, col_buff + col_offset_ * g,
        (Dtype)0., output + output_offset_ * g);
  }
}


void forward_gpu_bias(Dtype* output,
    const Dtype* bias) {
  blas_gpu_gemm(CblasNoTrans, CblasNoTrans, num_output_,
      out_spatial_dim_, 1, (Dtype)1., bias, bias_multiplier_.gpu_data(),
      (Dtype)1., output);
}


void backward_gpu_gemm(const Dtype* output,
    const Dtype* weights, Dtype* input) {
  Dtype* col_buff = col_buffer_.mutable_gpu_data();
  if (is_1x1_) {
    col_buff = input;
  }
  for (int g = 0; g < group_; ++g) {
    blas_gpu_gemm(CblasTrans, CblasNoTrans, kernel_dim_,
        output_dim_, output_channels_ / group_,
        (Dtype)1., weights + weight_offset_ * g, output + output_offset_ * g,
        (Dtype)0., col_buff + col_offset_ * g);
  }
  if (!is_1x1_) {
    conv_col2im_gpu(col_buff, input);
  }
}


void weight_gpu_gemm(const Dtype* input,
    const Dtype* output, Dtype* weights) {
  const Dtype* col_buff = input;
  if (!is_1x1_) {
    conv_im2col_gpu(input, col_buffer_.mutable_gpu_data());
    col_buff = col_buffer_.gpu_data();
  }
  for (int g = 0; g < group_; ++g) {
    blas_gpu_gemm(CblasNoTrans, CblasTrans, output_channels_ / group_,
        kernel_dim_, output_dim_,
        (Dtype)1., output + output_offset_ * g, col_buff + col_offset_ * g,
        (Dtype)1., weights + weight_offset_ * g);
  }
}


void backward_gpu_bias(Dtype* bias,
    const Dtype* input) {
  blas_gpu_gemv(CblasNoTrans, num_output_, out_spatial_dim_, 1.,
      input, bias_multiplier_.gpu_data(), 1., bias);
}

#endif  // !CPU_ONLY

static int ConvolutionLayer(enum LayerOp op, const struct Layer* layer, struct Net* net)
{
  Blob* bottom_blob = LBLOB(0);
  Blob* top_blob = LBLOB(1);
  Blob* weight_blob = WBLOB(0);
  Blob* bias_blob = LARG(bias_term) ? WBLOB(1) : NULL;
  int i, num_ = bottom_blob->data->f;
  int kernel_h = LARG(kernel_h);
  int kernel_w = LARG(kernel_w);
  int stride_h = MAX(1, LARG(stride_h));
  int stride_w = MAX(1, LARG(stride_w));
  int pad = MAX(0, LARG(pad));
  int dilation = MAX(1, LARG(dilation));
  int num_output = MAX(1, LARG(num_output));
  ASSERT(WBLOB(0)->data->h==LARG(kernel_h));
  ASSERT(WBLOB(0)->data->w==LARG(kernel_w));
  ASSERT(WBLOB(0)->data->f==LARG(num_output));

  //BaseConvolutionLayer s[1] = {0};
  //BaseConvolutionLayer_init(s, net, layer, false);
  switch (op) {
  case LO_Init:
    break;
  case LO_Forward:
    {
      int output_h = (bottom_blob->data->h + 2 * pad - (dilation * (kernel_h - 1) + 1)) / stride_h + 1;
      int output_w = (bottom_blob->data->w + 2 * pad - (dilation * (kernel_w - 1) + 1)) / stride_w + 1;
      pBox_create(top_blob->data, output_w, output_h, num_output, bottom_blob->data->f);
      const Dtype* weight_data = pBox_datap(weight_blob->data);
      //const Dtype* bottom_data = pBox_datap(bottom_blob->data);
      //Dtype* top_data = pBox_datap(top_blob->data);
      //int bottom_dim_ = pBox_length(bottom_blob->data);
      //int top_dim_ = pBox_length(top_blob->data);
      for (i = 0; i < num_; ++i) {
        img_t bottom_data_i = pBox_frame(bottom_blob->data, i);
        img_t top_data_i = pBox_frame(top_blob->data, i);
        forward_cpu_gemm(&bottom_data_i, weight_data, &top_data_i, kernel_h, kernel_w, stride_h, stride_w, pad, dilation);
        if (bias_blob) {
          const Dtype* bias_data = pBox_datap(bias_blob->data);
          forward_cpu_bias(&top_data_i, bias_data);
        }
      }
    }
    
    break;
  case LO_Backward:
    {
      pBox_ReshapeLike(bottom_blob->diff, bottom_blob->data);
      Dtype* weight_data = pBox_datap(weight_blob->data);
      Dtype* weight_diff = pBox_datap(weight_blob->diff);
      //const Dtype* top_diff = pBox_datap(top_blob->diff);
      //const Dtype* bottom_data = pBox_datap(bottom_blob->data);
      //Dtype* bottom_diff = pBox_datap(bottom_blob->diff);
      //int bottom_dim_ = pBox_length(bottom_blob->data);
      //int top_dim_ = pBox_length(top_blob->data);
      // Bias gradient, if necessary.
      if (bias_blob) {
        Dtype* bias_diff = pBox_datap(bias_blob->diff);
        for (i = 0; i < num_; ++i) {
          img_t top_diff_i = pBox_frame(top_blob->diff, i);
          backward_cpu_bias(bias_diff, &top_diff_i);
        }
      }
      for (i = 0; i < num_; ++i) {
        img_t bottom_data_i = pBox_frame(bottom_blob->data, i);
        img_t bottom_diff_i = pBox_frame(bottom_blob->diff, i);
        img_t top_diff_i = pBox_frame(top_blob->diff, i);
        Dtype* bias_diff = pBox_datap(bias_blob->diff);
        // gradient w.r.t. weight. Note that we will accumulate diffs.
        weight_cpu_gemm(&bottom_data_i, &top_diff_i, weight_diff, kernel_h, kernel_w, stride_h, stride_w, pad, dilation);
        // gradient w.r.t. bottom data, if necessary.
        backward_cpu_gemm(&top_diff_i, weight_data, &bottom_diff_i, kernel_h, kernel_w, stride_h, stride_w, pad, dilation);
      }
    }
    
    break;
  }
  return 0;
}
