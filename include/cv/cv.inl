
#ifndef _CC_INL_
#define _CC_INL_

#include "cstd.h"
#include "img/imgopt.inl"
#include "cvtype.inl"
//#include "core/types.hpp"
//#include "img/color.inl"



//#undef CC_DEFAULT
//#define CC_DEFAULT(val)
//typedef img_t* InputOutputArray;

#include "tables.inl"
//#include "array.inl"

#include "copy.inl"
#include "core/copy.inl"
#include "core/utility.inl"

//#include "core/persistence.hpp"
//#include "core/persistence.inl"

#include "core/mathfuncs.inl"
#include "core/hal_replacement.inl"
#include "rand.inl"
#include "lut.inl"
#include "convert.inl"
//#include "core/convert.inl"
#include "arithm.inl"
//#include "core/arithm.inl"
#include "mathfuncs.inl"
#include "sumpixels.inl"
#include "svd.inl"
#include "minmaxloc.inl"
#include "matmul.inl"
#include "norm.inl"
#include "utils.inl"
#include "core/matrix.inl"
#include "core/dxt.inl"
#include "cmp.inl"
#include "accum.inl"
#include "meansdv.inl"
#include "out.inl"
#include "fundam.inl"
#include "calibration.inl"
#include "templmatch.inl"
#include "filter.inl"
#include "smooth.inl"
#include "deriv.inl"
#include "adapthresh.inl"
#include "pyramids.inl"
#include "canny.inl"
#include "moments.inl"
#include "camshift.inl"
#include "floodfill.inl"
#include "imgwarp.inl"
//#include "drawing.inl"
//#include "samplers.inl"
#include "core/command_line_parser.inl"
#include "jacobieigens.inl"
//#include "undistort.inl"
#include "math/mat3.inl"
#include "color.inl"
//#include "compat.h"
//#include "core/hal_internal_openblas.inl"
//#include "core/hal_internal.inl"

//#include "core/stat.inl"
#include "core/stat.inl"

#include "core/lapack.inl"

#include "core/split.inl"
#include "core/merge.inl"

#include "imgproc/drawing.inl"
#include "imgproc/sumpixels.inl"
#include "imgproc/imgwarp.inl"
#include "imgproc/undistort.inl"
#include "imgproc/samplers.inl"
#include "morph_old.inl"
//#include "imgproc/morph.inl"
//#include "imgproc/filter.inl"


#include "calib3d/epnp.inl"
#include "calib3d/ptsetreg.inl"
#include "calib3d/fundam.inl"
#include "calib3d/compat_ptsetreg.inl"
#include "calib3d/calibration.inl"
#include "calib3d/levmarq.inl"
#include "calib3d/solvepnp.inl"


#if 0
#include "objdetect/cascadedetect.inl"

#include "clm/libclm.inl"
#include "thresh.inl"

#include "inpaint.inl"
#include "calibinit.inl"
#include "distransform.inl"
#include "switcher.inl"

#include "image.inl"
#include "approx.inl"
#include "alloc.inl"
#include "calccontrasthistogram.inl"
#include "calcimagehomography.inl"
#include "condens.inl"
#include "contours.inl"
#include "contourtree.inl"
#include "convhull.inl"
#include "convolve.inl"
#include "corner.inl"
#include "cornersubpix.inl"

#include "datastructs.inl"
#include "dominants.inl"
#include "emd.inl"
#include "error.inl"
#include "featureselect.inl"
#include "geometry.inl"
#include "haar.inl"
#include "histogram.inl"
#include "hough.inl"
#include "kalman.inl"
#include "linefit.inl"
#include "lkpyramid.inl"
#include "logic.inl"
#include "matchcontours.inl"
#include "motempl.inl"
#include "optflowbm.inl"
#include "optflowhs.inl"
#include "optflowlk.inl"
#include "outtext.inl"
#include "persistence.inl"
#include "pgh.inl"
#include "posit.inl"

#include "pyrsegmentation.inl"
#include "rotcalipers.inl"
#include "segmentation.inl"
#include "snakes.inl"
#include "subdivision2d.inl"
#include "switcher.inl"

#endif

#if 0
void cvtColor(const img_t* src, img_t* dst, ColorCvtCode code) {
  IMFUN_BEGIN(src==dst, src);
  CSize size = iSIZE(src->w, src->h);
  cvCvtColor(src->tt.data, src->s, src->c, dst->tt.data, dst->s, dst->c, CC_BGRA2GRAY, CC_8U, size);
  IMFUN_END();
}
#else
#endif

#if 0
void convertScaleAbs(const img_t* src, TypeId src_type, img_t* dst, double scale, double shift) {
  IMFUN_BEGIN(src==dst, src);
  imsetsize(dst, src->h, src->w, src->c, 1);
  cvConvertScaleAbs(src->tt.data, src->s, dst->tt.data, dst->s, scale, shift, src_type, src->c, src->h, src->w);
  IMFUN_END();
}
#endif

//void dft(const img_t* src, img_t* dst) {  cvDFT}


#include "img.inl"


#undef C_CDECL
#undef C_STDCALL


#endif // _CC_INL_
