#pragma once

#include "opencv2/core.hpp"

namespace cv
{

CC_EXPORTS_W void setErrorVerbosity(bool verbose);

}

#if 0

namespace cv
{
CC_EXPORTS_W void add(const CvMat* src1, Scalar src2, CvMat* dst, const CvMat* mask=noArray(), int dtype=-1);

CC_EXPORTS_W void subtract(const CvMat* src1, Scalar src2, CvMat* dst, const CvMat* mask=noArray(), int dtype=-1);

CC_EXPORTS_W void multiply(const CvMat* src1, Scalar src2, CvMat* dst, double scale=1, int dtype=-1);

CC_EXPORTS_W void divide(const CvMat* src1, Scalar src2, CvMat* dst, double scale=1, int dtype=-1);

CC_EXPORTS_W void absdiff(const CvMat* src1, Scalar src2, CvMat* dst);

CC_EXPORTS_W void compare(const CvMat* src1, Scalar src2, CvMat* dst, int cmpop);

CC_EXPORTS_W void min(const CvMat* src1, Scalar src2, CvMat* dst);

CC_EXPORTS_W void max(const CvMat* src1, Scalar src2, CvMat* dst);

}
#endif //0
