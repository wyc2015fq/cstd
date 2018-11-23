#ifndef CAFFE_UTIL_CUDNN_H_
#define CAFFE_UTIL_CUDNN_H_
#ifdef USE_CUDNN

#include <cudnn.h>
#include "types.h"


template <typename Dtype> class dataType;
template<> class dataType<float>
{
public:
  static const cudnnDataType_t type = CUDNN_DATA_FLOAT;
};
template<> class dataType<double>
{
public:
  static const cudnnDataType_t type = CUDNN_DATA_DOUBLE;
};


#define CUDNN_VERSION_MIN(major, minor, patch) \
    (CUDNN_VERSION >= (major * 1000 + minor * 100 + patch))

#define CUDNN_CHECK(condition) \
  do { \
    cudnnStatus_t status = condition; \
    CHECK_EQ(status, CUDNN_STATUS_SUCCESS) << " "\
      << cudnnGetErrorString(status); \
  } while (0)

static const char* cudnnGetErrorString(cudnnStatus_t status)
{
  switch (status) {
  case CUDNN_STATUS_SUCCESS:
    return "CUDNN_STATUS_SUCCESS";
  case CUDNN_STATUS_NOT_INITIALIZED:
    return "CUDNN_STATUS_NOT_INITIALIZED";
  case CUDNN_STATUS_ALLOC_FAILED:
    return "CUDNN_STATUS_ALLOC_FAILED";
  case CUDNN_STATUS_BAD_PARAM:
    return "CUDNN_STATUS_BAD_PARAM";
  case CUDNN_STATUS_INTERNAL_ERROR:
    return "CUDNN_STATUS_INTERNAL_ERROR";
  case CUDNN_STATUS_INVALID_VALUE:
    return "CUDNN_STATUS_INVALID_VALUE";
  case CUDNN_STATUS_ARCH_MISMATCH:
    return "CUDNN_STATUS_ARCH_MISMATCH";
  case CUDNN_STATUS_MAPPING_ERROR:
    return "CUDNN_STATUS_MAPPING_ERROR";
  case CUDNN_STATUS_EXECUTION_FAILED:
    return "CUDNN_STATUS_EXECUTION_FAILED";
  case CUDNN_STATUS_NOT_SUPPORTED:
    return "CUDNN_STATUS_NOT_SUPPORTED";
  case CUDNN_STATUS_LICENSE_ERROR:
    return "CUDNN_STATUS_LICENSE_ERROR";
#if CUDNN_VERSION_MIN(6, 0, 0)
  case CUDNN_STATUS_RUNTIME_PREREQUISITE_MISSING:
    return "CUDNN_STATUS_RUNTIME_PREREQUISITE_MISSING";
#endif
  }
  return "Unknown cudnn status";
}


static cudnnDataType_t dataTypeCvt(TypeFlag flag) {
  switch (flag) {
#define DEF(a, b, c)  case TF_##a: return c;
    TYPEFLAGDEF_DEF(DEF)
#undef DEF
  }
  assert(0);
  return CUDNN_DATA_FLOAT;
}

static void createTensor4dDesc(cudnnTensorDescriptor_t* desc)
{
  CUDNN_CHECK(cudnnCreateTensorDescriptor(desc));
}

static void setTensor4dDesc(cudnnTensorDescriptor_t desc, cudnnDataType_t type,
  int n, int c, int h, int w,
  int stride_n, int stride_c, int stride_h, int stride_w)
{
  CUDNN_CHECK(cudnnSetTensor4dDescriptorEx(desc, type,
    n, c, h, w, stride_n, stride_c, stride_h, stride_w));
}

static void setTensor4dDesc(cudnnTensorDescriptor_t desc, cudnnDataType_t type, int n, int c, int h, int w)
{
  const int stride_w = 1;
  const int stride_h = w * stride_w;
  const int stride_c = h * stride_h;
  const int stride_n = c * stride_c;
  setTensor4dDesc(desc, type, n, c, h, w,
    stride_n, stride_c, stride_h, stride_w);
}

static void setTensor4dDesc(cudnnTensorDescriptor_t desc, cudnnDataType_t type, DataShape shape) {
  setTensor4dDesc(desc, type, shape.n, shape.c, shape.h, shape.w);
}

static void createFilterDesc(cudnnFilterDescriptor_t* desc, cudnnDataType_t type,
  int n, int c, int h, int w)
{
  CUDNN_CHECK(cudnnCreateFilterDescriptor(desc));
#if CUDNN_VERSION_MIN(5, 0, 0)
  CUDNN_CHECK(cudnnSetFilter4dDescriptor(*desc, type,
    CUDNN_TENSOR_NCHW, n, c, h, w));
#else
  CUDNN_CHECK(cudnnSetFilter4dDescriptor_v4(*desc, type,
    CUDNN_TENSOR_NCHW, n, c, h, w));
#endif
}

static void createConvolutionDesc(cudnnConvolutionDescriptor_t* conv)
{
  CUDNN_CHECK(cudnnCreateConvolutionDescriptor(conv));
}

static void setConvolutionDesc(cudnnConvolutionDescriptor_t conv, cudnnDataType_t type,
  cudnnTensorDescriptor_t bottom, cudnnFilterDescriptor_t filter,
  int pad_h, int pad_w, int stride_h, int stride_w )
{
#if CUDNN_VERSION_MIN(6, 0, 0)
  CUDNN_CHECK(cudnnSetConvolution2dDescriptor(conv,
    pad_h, pad_w, stride_h, stride_w, 1, 1, CUDNN_CROSS_CORRELATION,
    type));
#else
  CUDNN_CHECK(cudnnSetConvolution2dDescriptor(conv,
    pad_h, pad_w, stride_h, stride_w, 1, 1, CUDNN_CROSS_CORRELATION));
#endif
}

static void createPoolingDesc(cudnnPoolingDescriptor_t* pool_desc, const PoolMethod poolmethod, 
  int h, int w, int pad_h, int pad_w, int stride_h, int stride_w)
{
  cudnnPoolingMode_t        mode;
  if (poolmethod == PoolMethod_MAX) {
    mode = CUDNN_POOLING_MAX;
  }
  else if (poolmethod == PoolMethod_AVE) {
    mode = CUDNN_POOLING_AVERAGE_COUNT_INCLUDE_PADDING;
  } else {
    LOG(FATAL) << "Unknown pooling method.";
  }
  CUDNN_CHECK(cudnnCreatePoolingDescriptor(pool_desc));
#if CUDNN_VERSION_MIN(5, 0, 0)
  CUDNN_CHECK(cudnnSetPooling2dDescriptor(*pool_desc, mode,
    CUDNN_PROPAGATE_NAN, h, w, pad_h, pad_w, stride_h, stride_w));
#else
  CUDNN_CHECK(cudnnSetPooling2dDescriptor_v4(*pool_desc, *mode,
    CUDNN_PROPAGATE_NAN, h, w, pad_h, pad_w, stride_h, stride_w));
#endif
}

static void createActivationDescriptor(cudnnActivationDescriptor_t* activ_desc,
  cudnnActivationMode_t mode)
{
  CUDNN_CHECK(cudnnCreateActivationDescriptor(activ_desc));
  CUDNN_CHECK(cudnnSetActivationDescriptor(*activ_desc, mode,
    CUDNN_PROPAGATE_NAN, (0)));
}

#endif  // USE_CUDNN
#endif  // CAFFE_UTIL_CUDNN_H_
