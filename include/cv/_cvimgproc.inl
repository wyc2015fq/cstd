
#ifndef _CC_IMG_PROC_H_
#define _CC_IMG_PROC_H_

#define  CC_COPY(dst, src, len, idx) \
    for((idx) = 0; (idx) < (len); (idx)++) (dst)[idx] = (src)[idx]

#define  CC_SET(dst, val, len, idx)  \
    for((idx) = 0; (idx) < (len); (idx)++) (dst)[idx] = (val)

/* performs convolution of 2d floating-point array with 3x1, 1x3 or separable 3x3 mask */
void icvSepConvSmall3_32f(float* src, int src_step, float* dst, int dst_step,
            CSize src_size, const float* kx, const float* ky, float* buffer);

typedef CStatus (C_STDCALL * CvSobelFixedIPPFunc)
(const void* src, int srcstep, void* dst, int dststep, CSize roi, int aperture);

typedef CStatus (C_STDCALL * CvFilterFixedIPPFunc)
(const void* src, int srcstep, void* dst, int dststep, CSize roi);

#undef   CC_CALC_MIN
#define  CC_CALC_MIN(a, b) if((a) > (b)) (a) = (b)

#undef   CC_CALC_MAX
#define  CC_CALC_MAX(a, b) if((a) < (b)) (a) = (b)

#define CC_MORPH_ALIGN  4

#define CC_WHOLE   0
#define CC_START   1
#define CC_END     2
#define CC_MIDDLE  4

void
icvCrossCorr(const img_t* _img, const img_t* _templ,
              img_t* _corr, CPoint anchor=cPoint(0,0));

CStatus C_STDCALL
icvCopyReplicateBorder_8u(const uchar* src, int srcstep, CSize srcroi,
                           uchar* dst, int dststep, CSize dstroi,
                           int left, int right, int cn, const uchar* value = 0);

img_t* icvIPPFilterInit(const img_t* src, int stripe_size, CSize ksize);

int icvIPPFilterNextStripe(const img_t* src, img_t* temp, int y,
                            CSize ksize, CPoint anchor);

int icvIPPSepFilter(const img_t* src, img_t* dst, const img_t* kernelX,
                     const img_t* kernelY, CPoint anchor);

#define ICC_WARP_SHIFT          10
#define ICC_WARP_MASK           ((1 << ICC_WARP_SHIFT) - 1)

#define ICC_LINEAR_TAB_SIZE     (ICC_WARP_MASK+1)
extern float icvLinearCoeffs[(ICC_LINEAR_TAB_SIZE+1)*2];
void icvInitLinearCoeffTab();

#define ICC_CUBIC_TAB_SIZE   (ICC_WARP_MASK+1)
extern float icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE+1)*2];

void icvInitCubicCoeffTab();

CStatus C_STDCALL icvGetRectSubPix_8u_C1R
(const uchar* src, int src_step, CSize src_size,
  uchar* dst, int dst_step, CSize win_size, CPoint2D32f center);
CStatus C_STDCALL icvGetRectSubPix_8u32f_C1R
(const uchar* src, int src_step, CSize src_size,
  float* dst, int dst_step, CSize win_size, CPoint2D32f center);
CStatus C_STDCALL icvGetRectSubPix_32f_C1R
(const float* src, int src_step, CSize src_size,
  float* dst, int dst_step, CSize win_size, CPoint2D32f center);

#endif /*_CC_INTERNAL_H_*/
