
#ifndef __CSTD_CDNN_CPU_H__
#define __CSTD_CDNN_CPU_H__

#include "cudnn.h"

struct cdnnContextCpu {
	int x;
};
union cdnnDimCpu {
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


int dim_count(const int* dim, int i, int end) {
	int count = 1;
	for (; i < end; ++i) {
		count *= dim[i];
	}
	return count;
}
int dim_ndim(const int* dim) {
	int i = 0;
	for (; i < CDNN_DIM_MAX && dim[i]>0; ++i) {}
	return i;
}

#endif // __CSTD_CDNN_CPU_H__
