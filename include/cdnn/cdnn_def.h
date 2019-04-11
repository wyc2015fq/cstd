
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
	//cdnnDimCpu stride;
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

struct cdnnActivationDescriptorCpu {
	cdnnActivationMode_t mode;
	cdnnNanPropagation_t reluNanOpt;
	double coef;
};

//! Various border types, image boundaries are denoted with `|`
//! @see borderInterpolate, copyMakeBorder
enum CBorderTypes {
	CC_BORDER_CONSTANT = 0, //!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`
	CC_BORDER_REPLICATE = 1, //!< `aaaaaa|abcdefgh|hhhhhhh`
	CC_BORDER_REFLECT = 2, //!< `fedcba|abcdefgh|hgfedcb`
	CC_BORDER_WRAP = 3, //!< `cdefgh|abcdefgh|abcdefg`
	CC_BORDER_REFLECT_101 = 4, //!< `gfedcb|abcdefgh|gfedcba`
	CC_BORDER_TRANSPARENT = 5, //!< `uvwxyz|absdefgh|ijklmno`

	CC_BORDER_REFLECT101 = CC_BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
	CC_BORDER_DEFAULT = CC_BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
	CC_BORDER_ISOLATED = 16 //!< do not look outside of ROI
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
int dim_cmp(const int* dim1, const int* dim2, int i, int end) {
	for (;i<end;++i) {
		int t = dim1[i] - dim2[i];
		if (t) { return t>0?1:-1; }
	}
	return 0;
}

#endif // __CSTD_CDNN_CPU_H__
