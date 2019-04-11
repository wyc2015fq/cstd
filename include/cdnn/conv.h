
//#include "std/types_c.h"
//#include "conv_fast.h"
#include "copy.h"
#include "im2col.h"
//#define conv2d_fast cpu_conv2d
#define conv2d_fast my_conv2d_fast
//#define conv2d_fast conv2d_gemm_fw

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

void FUN(caffe_gemm)(const CBLAS_TRANSPOSE TransA,
	const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
	const Dtype* alpha, const Dtype* A, const Dtype* B, const Dtype* beta,
	Dtype* C) {
	int lda = (TransA == CblasNoTrans) ? K : M;
	int ldb = (TransB == CblasNoTrans) ? N : K;
	FUN(gemm)(CblasRowMajor, TransA, TransB, M, N, K, *alpha, A, lda, B, ldb, *beta, C, N);
	//my_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
}


#if 1

cdnnStatus_t FUN(ConvolutionForward)(cdnnHandle_t handle, const Dtype *alpha, const cdnnTensorDescriptorCpu* xDesc, const Dtype *x,
	const cdnnFilterDescriptorCpu* wDesc, const Dtype *w, const cdnnConvolutionDescriptorCpu* convDesc, cdnnConvolutionFwdAlgo_t algo,
	void *workSpace, size_t workSpaceSizeInBytes, const Dtype *beta, const cdnnTensorDescriptorCpu* yDesc, Dtype *y)
{
	//static void FUN(conv_forward_gemm)(const Dtype* inData, Dtype* outData, const Dtype* weights, const Dtype* biasData,
    // DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
	//int dilation_h, int dilation_w, int pad_h, int pad_w, int group_, bool cross_correlation);
	const Dtype* inData = (const Dtype*)x;
	Dtype* outData = (Dtype*)y;
	const Dtype* weights = (const Dtype*)w;
	DataShape inSize = xDesc->size;
	DataShape outSize = yDesc->size;
	int kernel_h = wDesc->size.dim[2];
	int kernel_w = wDesc->size.dim[3];
	int pad_h = convDesc->pad.dim[0];
	int pad_w = convDesc->pad.dim[1];
	int stride_h = convDesc->stride.dim[0];
	int stride_w = convDesc->stride.dim[1];
	int dilation_h = convDesc->dilation.dim[0];
	int dilation_w = convDesc->dilation.dim[1];
	int cross_correlation = CDNN_CROSS_CORRELATION==convDesc->mode;
	//const Dtype* biasData = (const Dtype*)w;
#if 0
  if (0) {
    conv2d_fast(inData, outData, weights, biasData,
      inSize, outSize, kernel_h, kernel_w, stride_h, stride_w,
      dilation_h, dilation_w, pad_h, pad_w, group_, cross_correlation);
    return;
  }
#endif
  int ndim = dim_ndim(inSize.dim);
  int group_ = 1;
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
  //uutime a;
  //a.restart();
  if (!cross_correlation) {
    int n = outSize.c*inSize.c, m = kernel_h*kernel_w;
    inv_kernel = (Dtype*)malloc(n*m * sizeof(Dtype));
    memcpy(inv_kernel, weights, n*m * sizeof(Dtype));
    FUN(rev2d)(n, m, inv_kernel);
    weights = inv_kernel;
  }
  //LOG(INFO) << "rev2d " << a.elapsed();

  //a.restart();
  int in_size2 = dim_count(inSize.dim, 1, ndim);
  int out_size2 = dim_count(outSize.dim, 1, ndim);

  for (int i = 0; i < inSize.n; ++i) {
	  const Dtype* in_data = inData + i*in_size2;
	  Dtype* out_data = outData + i*out_size2;
	  const Dtype* col_buff = in_data;
	  if (!is_1x1_) {
		  col_buffer_ = (Dtype*)malloc(col_buffer_size_ * sizeof(Dtype));
		  FUN(im2col)(in_data, inSize.c, inSize.h, inSize.w, kernel_h, kernel_w, pad_h, pad_w, stride_h, stride_w, dilation_h, dilation_w, col_buffer_);
		  col_buff = col_buffer_;
	  }
	  //LOG(INFO) << "im2col " << a.elapsed();
	  //a.restart();
	  for (int g = 0; g < group_; ++g) {
		  FUN(caffe_gemm)(CblasNoTrans, CblasNoTrans, conv_out_channels_ / group_, conv_out_spatial_dim_, kernel_dim_,
			  alpha, weights + weight_offset_ * g, col_buff + col_offset_ * g, beta, out_data + output_offset_ * g);
	  }
  }
  if (1. != *alpha && 0 != *beta) {
	  int count = dim_count(outSize.dim, 0, ndim);
	  //FUN(axpy)();
	  //my_sscalb(count, alpha, outData, 1, beta);
  }
  //LOG(INFO) << "caffe_gemm " << a.elapsed();
  if (col_buffer_) { free(col_buffer_); }
  if (inv_kernel) { free(inv_kernel); }
  return CDNN_STATUS_SUCCESS;
}

#endif