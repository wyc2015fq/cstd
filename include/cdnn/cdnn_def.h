
#ifndef __CSTD_CDNN_CPU_H__
#define __CSTD_CDNN_CPU_H__

#include "cudnn.h"

struct cdnnContextCpu {
	int x;
};
struct cdnnDimCpu {
	int data[CDNN_DIM_MAX];
	struct { int n, c, h, w; };
	struct { int dim[CDNN_DIM_MAX]; };
};
typedef cdnnDimCpu DataShape;

struct cdnnTensorDescriptorCpu {
	cdnnDimCpu size;
	cdnnDimCpu stride;
	cdnnTensorFormat_t format;
	cdnnDataType_t data_type;
};
struct cdnnFilterDescriptorCpu {
	cdnnDimCpu size;
};
struct cdnnConvolutionDescriptorCpu {
	cdnnDimCpu pad;
	cdnnDimCpu stride;
	cdnnDimCpu dilation;
	cdnnConvolutionMode_t mode;
	cdnnDataType_t compute_type;
	int num_spatial_axes_;
};


#endif // __CSTD_CDNN_CPU_H__
