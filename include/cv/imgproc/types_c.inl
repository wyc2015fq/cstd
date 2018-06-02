

#ifndef OPENCC_IMGPROC_TYPES_C_H
#define OPENCC_IMGPROC_TYPES_C_H

#include "opencv2/core/core_c.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup imgproc_c
  @{
*/

/** Connected component structure */
typedef struct CConnectedComp
{
    double area;    /**<area of the connected component  */
    CScalar value; /**<average color of the connected component */
    CRect rect;    /**<ROI of the component  */
    CvSeq* contour; /**<optional component boundary
                      (the contour might have child contours corresponding to the holes)*/
}
CConnectedComp;

/** Image smooth methods */
enum SmoothMethod_c
{
    /** linear convolution with \f$\texttt{size1}\times\texttt{size2}\f$ box kernel (all 1's). If
    you want to smooth different pixels with different-size box kernels, you can use the integral
    image that is computed using integral */
    CC_BLUR_NO_SCALE =0,
    /** linear convolution with \f$\texttt{size1}\times\texttt{size2}\f$ box kernel (all
    1's) with subsequent scaling by \f$1/(\texttt{size1}\cdot\texttt{size2})\f$ */
    CC_BLUR  =1,
    /** linear convolution with a \f$\texttt{size1}\times\texttt{size2}\f$ Gaussian kernel */
    CC_GAUSSIAN  =2,
    /** median filter with a \f$\texttt{size1}\times\texttt{size1}\f$ square aperture */
    CC_MEDIAN =3,
    /** bilateral filter with a \f$\texttt{size1}\times\texttt{size1}\f$ square aperture, color
    sigma= sigma1 and spatial sigma= sigma2. If size1=0, the aperture square side is set to
    cRound(sigma2\*1.5)\*2+1. See bilateralFilter */
    CC_BILATERAL =4
};

/** Filters used in pyramid decomposition */
enum
{
    CC_GAUSSIAN_5x5 = 7
};

/** Special filters */
enum
{
    CC_SCHARR =-1,
    CC_MAX_SOBEL_KSIZE =7
};

/** Constants for color conversion */
enum
{
    CC_BGR2BGRA    =0,
    CC_RGB2RGBA    =CC_BGR2BGRA,

    CC_BGRA2BGR    =1,
    CC_RGBA2RGB    =CC_BGRA2BGR,

    CC_BGR2RGBA    =2,
    CC_RGB2BGRA    =CC_BGR2RGBA,

    CC_RGBA2BGR    =3,
    CC_BGRA2RGB    =CC_RGBA2BGR,

    CC_BGR2RGB     =4,
    CC_RGB2BGR     =CC_BGR2RGB,

    CC_BGRA2RGBA   =5,
    CC_RGBA2BGRA   =CC_BGRA2RGBA,

    CC_BGR2GRAY    =6,
    CC_RGB2GRAY    =7,
    CC_GRAY2BGR    =8,
    CC_GRAY2RGB    =CC_GRAY2BGR,
    CC_GRAY2BGRA   =9,
    CC_GRAY2RGBA   =CC_GRAY2BGRA,
    CC_BGRA2GRAY   =10,
    CC_RGBA2GRAY   =11,

    CC_BGR2BGR565  =12,
    CC_RGB2BGR565  =13,
    CC_BGR5652BGR  =14,
    CC_BGR5652RGB  =15,
    CC_BGRA2BGR565 =16,
    CC_RGBA2BGR565 =17,
    CC_BGR5652BGRA =18,
    CC_BGR5652RGBA =19,

    CC_GRAY2BGR565 =20,
    CC_BGR5652GRAY =21,

    CC_BGR2BGR555  =22,
    CC_RGB2BGR555  =23,
    CC_BGR5552BGR  =24,
    CC_BGR5552RGB  =25,
    CC_BGRA2BGR555 =26,
    CC_RGBA2BGR555 =27,
    CC_BGR5552BGRA =28,
    CC_BGR5552RGBA =29,

    CC_GRAY2BGR555 =30,
    CC_BGR5552GRAY =31,

    CC_BGR2XYZ     =32,
    CC_RGB2XYZ     =33,
    CC_XYZ2BGR     =34,
    CC_XYZ2RGB     =35,

    CC_BGR2YCrCb   =36,
    CC_RGB2YCrCb   =37,
    CC_YCrCb2BGR   =38,
    CC_YCrCb2RGB   =39,

    CC_BGR2HSV     =40,
    CC_RGB2HSV     =41,

    CC_BGR2Lab     =44,
    CC_RGB2Lab     =45,

    CC_BayerBG2BGR =46,
    CC_BayerGB2BGR =47,
    CC_BayerRG2BGR =48,
    CC_BayerGR2BGR =49,

    CC_BayerBG2RGB =CC_BayerRG2BGR,
    CC_BayerGB2RGB =CC_BayerGR2BGR,
    CC_BayerRG2RGB =CC_BayerBG2BGR,
    CC_BayerGR2RGB =CC_BayerGB2BGR,

    CC_BGR2Luv     =50,
    CC_RGB2Luv     =51,
    CC_BGR2HLS     =52,
    CC_RGB2HLS     =53,

    CC_HSV2BGR     =54,
    CC_HSV2RGB     =55,

    CC_Lab2BGR     =56,
    CC_Lab2RGB     =57,
    CC_Luv2BGR     =58,
    CC_Luv2RGB     =59,
    CC_HLS2BGR     =60,
    CC_HLS2RGB     =61,

    CC_BayerBG2BGR_VNG =62,
    CC_BayerGB2BGR_VNG =63,
    CC_BayerRG2BGR_VNG =64,
    CC_BayerGR2BGR_VNG =65,

    CC_BayerBG2RGB_VNG =CC_BayerRG2BGR_VNG,
    CC_BayerGB2RGB_VNG =CC_BayerGR2BGR_VNG,
    CC_BayerRG2RGB_VNG =CC_BayerBG2BGR_VNG,
    CC_BayerGR2RGB_VNG =CC_BayerGB2BGR_VNG,

    CC_BGR2HSV_FULL = 66,
    CC_RGB2HSV_FULL = 67,
    CC_BGR2HLS_FULL = 68,
    CC_RGB2HLS_FULL = 69,

    CC_HSV2BGR_FULL = 70,
    CC_HSV2RGB_FULL = 71,
    CC_HLS2BGR_FULL = 72,
    CC_HLS2RGB_FULL = 73,

    CC_LBGR2Lab     = 74,
    CC_LRGB2Lab     = 75,
    CC_LBGR2Luv     = 76,
    CC_LRGB2Luv     = 77,

    CC_Lab2LBGR     = 78,
    CC_Lab2LRGB     = 79,
    CC_Luv2LBGR     = 80,
    CC_Luv2LRGB     = 81,

    CC_BGR2YUV      = 82,
    CC_RGB2YUV      = 83,
    CC_YUV2BGR      = 84,
    CC_YUV2RGB      = 85,

    CC_BayerBG2GRAY = 86,
    CC_BayerGB2GRAY = 87,
    CC_BayerRG2GRAY = 88,
    CC_BayerGR2GRAY = 89,

    //YUV 4:2:0 formats family
    CC_YUV2RGB_NV12 = 90,
    CC_YUV2BGR_NV12 = 91,
    CC_YUV2RGB_NV21 = 92,
    CC_YUV2BGR_NV21 = 93,
    CC_YUV420sp2RGB = CC_YUV2RGB_NV21,
    CC_YUV420sp2BGR = CC_YUV2BGR_NV21,

    CC_YUV2RGBA_NV12 = 94,
    CC_YUV2BGRA_NV12 = 95,
    CC_YUV2RGBA_NV21 = 96,
    CC_YUV2BGRA_NV21 = 97,
    CC_YUV420sp2RGBA = CC_YUV2RGBA_NV21,
    CC_YUV420sp2BGRA = CC_YUV2BGRA_NV21,

    CC_YUV2RGB_YV12 = 98,
    CC_YUV2BGR_YV12 = 99,
    CC_YUV2RGB_IYUV = 100,
    CC_YUV2BGR_IYUV = 101,
    CC_YUV2RGB_I420 = CC_YUV2RGB_IYUV,
    CC_YUV2BGR_I420 = CC_YUV2BGR_IYUV,
    CC_YUV420p2RGB = CC_YUV2RGB_YV12,
    CC_YUV420p2BGR = CC_YUV2BGR_YV12,

    CC_YUV2RGBA_YV12 = 102,
    CC_YUV2BGRA_YV12 = 103,
    CC_YUV2RGBA_IYUV = 104,
    CC_YUV2BGRA_IYUV = 105,
    CC_YUV2RGBA_I420 = CC_YUV2RGBA_IYUV,
    CC_YUV2BGRA_I420 = CC_YUV2BGRA_IYUV,
    CC_YUV420p2RGBA = CC_YUV2RGBA_YV12,
    CC_YUV420p2BGRA = CC_YUV2BGRA_YV12,

    CC_YUV2GRAY_420 = 106,
    CC_YUV2GRAY_NV21 = CC_YUV2GRAY_420,
    CC_YUV2GRAY_NV12 = CC_YUV2GRAY_420,
    CC_YUV2GRAY_YV12 = CC_YUV2GRAY_420,
    CC_YUV2GRAY_IYUV = CC_YUV2GRAY_420,
    CC_YUV2GRAY_I420 = CC_YUV2GRAY_420,
    CC_YUV420sp2GRAY = CC_YUV2GRAY_420,
    CC_YUV420p2GRAY = CC_YUV2GRAY_420,

    //YUV 4:2:2 formats family
    CC_YUV2RGB_UYVY = 107,
    CC_YUV2BGR_UYVY = 108,
    //CC_YUV2RGB_VYUY = 109,
    //CC_YUV2BGR_VYUY = 110,
    CC_YUV2RGB_Y422 = CC_YUV2RGB_UYVY,
    CC_YUV2BGR_Y422 = CC_YUV2BGR_UYVY,
    CC_YUV2RGB_UYNV = CC_YUV2RGB_UYVY,
    CC_YUV2BGR_UYNV = CC_YUV2BGR_UYVY,

    CC_YUV2RGBA_UYVY = 111,
    CC_YUV2BGRA_UYVY = 112,
    //CC_YUV2RGBA_VYUY = 113,
    //CC_YUV2BGRA_VYUY = 114,
    CC_YUV2RGBA_Y422 = CC_YUV2RGBA_UYVY,
    CC_YUV2BGRA_Y422 = CC_YUV2BGRA_UYVY,
    CC_YUV2RGBA_UYNV = CC_YUV2RGBA_UYVY,
    CC_YUV2BGRA_UYNV = CC_YUV2BGRA_UYVY,

    CC_YUV2RGB_YUY2 = 115,
    CC_YUV2BGR_YUY2 = 116,
    CC_YUV2RGB_YVYU = 117,
    CC_YUV2BGR_YVYU = 118,
    CC_YUV2RGB_YUYV = CC_YUV2RGB_YUY2,
    CC_YUV2BGR_YUYV = CC_YUV2BGR_YUY2,
    CC_YUV2RGB_YUNV = CC_YUV2RGB_YUY2,
    CC_YUV2BGR_YUNV = CC_YUV2BGR_YUY2,

    CC_YUV2RGBA_YUY2 = 119,
    CC_YUV2BGRA_YUY2 = 120,
    CC_YUV2RGBA_YVYU = 121,
    CC_YUV2BGRA_YVYU = 122,
    CC_YUV2RGBA_YUYV = CC_YUV2RGBA_YUY2,
    CC_YUV2BGRA_YUYV = CC_YUV2BGRA_YUY2,
    CC_YUV2RGBA_YUNV = CC_YUV2RGBA_YUY2,
    CC_YUV2BGRA_YUNV = CC_YUV2BGRA_YUY2,

    CC_YUV2GRAY_UYVY = 123,
    CC_YUV2GRAY_YUY2 = 124,
    //CC_YUV2GRAY_VYUY = CC_YUV2GRAY_UYVY,
    CC_YUV2GRAY_Y422 = CC_YUV2GRAY_UYVY,
    CC_YUV2GRAY_UYNV = CC_YUV2GRAY_UYVY,
    CC_YUV2GRAY_YVYU = CC_YUV2GRAY_YUY2,
    CC_YUV2GRAY_YUYV = CC_YUV2GRAY_YUY2,
    CC_YUV2GRAY_YUNV = CC_YUV2GRAY_YUY2,

    // alpha premultiplication
    CC_RGBA2mRGBA = 125,
    CC_mRGBA2RGBA = 126,

    CC_RGB2YUV_I420 = 127,
    CC_BGR2YUV_I420 = 128,
    CC_RGB2YUV_IYUV = CC_RGB2YUV_I420,
    CC_BGR2YUV_IYUV = CC_BGR2YUV_I420,

    CC_RGBA2YUV_I420 = 129,
    CC_BGRA2YUV_I420 = 130,
    CC_RGBA2YUV_IYUV = CC_RGBA2YUV_I420,
    CC_BGRA2YUV_IYUV = CC_BGRA2YUV_I420,
    CC_RGB2YUV_YV12  = 131,
    CC_BGR2YUV_YV12  = 132,
    CC_RGBA2YUV_YV12 = 133,
    CC_BGRA2YUV_YV12 = 134,

    // Edge-Aware Demosaicing
    CC_BayerBG2BGR_EA = 135,
    CC_BayerGB2BGR_EA = 136,
    CC_BayerRG2BGR_EA = 137,
    CC_BayerGR2BGR_EA = 138,

    CC_BayerBG2RGB_EA = CC_BayerRG2BGR_EA,
    CC_BayerGB2RGB_EA = CC_BayerGR2BGR_EA,
    CC_BayerRG2RGB_EA = CC_BayerBG2BGR_EA,
    CC_BayerGR2RGB_EA = CC_BayerGB2BGR_EA,

    CC_COLORCVT_MAX  = 139
};


/** Sub-pixel interpolation methods */
enum
{
    CC_INTER_NN        =0,
    CC_INTER_LINEAR    =1,
    CC_INTER_CUBIC     =2,
    CC_INTER_AREA      =3,
    CC_INTER_LANCZOS4  =4
};

/** ... and other image warping flags */
enum
{
    CC_WARP_FILL_OUTLIERS =8,
    CC_WARP_INVERSE_MAP  =16
};

/** Shapes of a structuring element for morphological operations
@see MorphShapes, getStructuringElement
*/
enum MorphShapes_c
{
    CC_SHAPE_RECT      =0,
    CC_SHAPE_CROSS     =1,
    CC_SHAPE_ELLIPSE   =2,
    CC_SHAPE_CUSTOM    =100 //!< custom structuring element
};

/** Morphological operations */
enum
{
    CC_MOP_ERODE        =0,
    CC_MOP_DILATE       =1,
    CC_MOP_OPEN         =2,
    CC_MOP_CLOSE        =3,
    CC_MOP_GRADIENT     =4,
    CC_MOP_TOPHAT       =5,
    CC_MOP_BLACKHAT     =6
};

/** Spatial and central moments */
typedef struct CMoments
{
    double  m00, m10, m01, m20, m11, m02, m30, m21, m12, m03; /**< spatial moments */
    double  mu20, mu11, mu02, mu30, mu21, mu12, mu03; /**< central moments */
    double  inv_sqrt_m00; /**< m00 != 0 ? 1/sqrt(m00) : 0 */

#ifdef __cplusplus
    CMoments(){}
    CMoments(const Moments& m)
    {
        m00 = m.m00; m10 = m.m10; m01 = m.m01;
        m20 = m.m20; m11 = m.m11; m02 = m.m02;
        m30 = m.m30; m21 = m.m21; m12 = m.m12; m03 = m.m03;
        mu20 = m.mu20; mu11 = m.mu11; mu02 = m.mu02;
        mu30 = m.mu30; mu21 = m.mu21; mu12 = m.mu12; mu03 = m.mu03;
        double am00 = std::abs(m.m00);
        inv_sqrt_m00 = am00 > DBL_EPSILON ? 1./std::sqrt(am00) : 0;
    }
    operator Moments() const
    {
        return Moments(m00, m10, m01, m20, m11, m02, m30, m21, m12, m03);
    }
#endif
}
CMoments;

/** Hu invariants */
typedef struct CHuMoments
{
    double hu1, hu2, hu3, hu4, hu5, hu6, hu7; /**< Hu invariants */
}
CHuMoments;

/** Template matching methods */
enum
{
    CC_TM_SQDIFF        =0,
    CC_TM_SQDIFF_NORMED =1,
    CC_TM_CCORR         =2,
    CC_TM_CCORR_NORMED  =3,
    CC_TM_CCOEFF        =4,
    CC_TM_CCOEFF_NORMED =5
};

typedef float (C_CDECL * CvDistanceFunction)( const float* a, const float* b, void* user_param );

/** Contour retrieval modes */
enum
{
    CC_RETR_EXTERNAL=0,
    CC_RETR_LIST=1,
    CC_RETR_CCOMP=2,
    CC_RETR_TREE=3,
    CC_RETR_FLOODFILL=4
};

/** Contour approximation methods */
enum
{
    CC_CHAIN_CODE=0,
    CC_CHAIN_APPROX_NONE=1,
    CC_CHAIN_APPROX_SIMPLE=2,
    CC_CHAIN_APPROX_TC89_L1=3,
    CC_CHAIN_APPROX_TC89_KCOS=4,
    CC_LINK_RUNS=5
};

/*
Internal structure that is used for sequential retrieving contours from the image.
It supports both hierarchical and plane variants of Suzuki algorithm.
*/
typedef struct _CvContourScanner* CvContourScanner;

/** Freeman chain reader state */
typedef struct CvChainPtReader
{
    CC_SEQ_READER_FIELDS()
    char      code;
    CPoint   pt;
    schar     deltas[8][2];
}
CvChainPtReader;

/** initializes 8-element array for fast access to 3x3 neighborhood of a pixel */
#define  CC_INIT_3X3_DELTAS( deltas, step, nch )            \
    ((deltas)[0] =  (nch),  (deltas)[1] = -(step) + (nch),  \
     (deltas)[2] = -(step), (deltas)[3] = -(step) - (nch),  \
     (deltas)[4] = -(nch),  (deltas)[5] =  (step) - (nch),  \
     (deltas)[6] =  (step), (deltas)[7] =  (step) + (nch))


/** Contour approximation algorithms */
enum
{
    CC_POLY_APPROX_DP = 0
};

/** @brief Shape matching methods

\f$A\f$ denotes object1,\f$B\f$ denotes object2

\f$\begin{array}{l} m^A_i =  \mathrm{sign} (h^A_i)  \cdot \log{h^A_i} \\ m^B_i =  \mathrm{sign} (h^B_i)  \cdot \log{h^B_i} \end{array}\f$

and \f$h^A_i, h^B_i\f$ are the Hu moments of \f$A\f$ and \f$B\f$ , respectively.
*/
enum ShapeMatchModes
{
    CC_CONTOURS_MATCH_I1  =1, //!< \f[I_1(A,B) =  \sum _{i=1...7}  \left |  \frac{1}{m^A_i} -  \frac{1}{m^B_i} \right |\f]
    CC_CONTOURS_MATCH_I2  =2, //!< \f[I_2(A,B) =  \sum _{i=1...7}  \left | m^A_i - m^B_i  \right |\f]
    CC_CONTOURS_MATCH_I3  =3  //!< \f[I_3(A,B) =  \max _{i=1...7}  \frac{ \left| m^A_i - m^B_i \right| }{ \left| m^A_i \right| }\f]
};

/** Shape orientation */
enum
{
    CC_CLOCKWISE         =1,
    CC_COUNTER_CLOCKWISE =2
};


/** Convexity defect */
typedef struct CvConvexityDefect
{
    CPoint* start; /**< point of the contour where the defect begins */
    CPoint* end; /**< point of the contour where the defect ends */
    CPoint* depth_point; /**< the farthest from the convex hull point within the defect */
    float depth; /**< distance between the farthest point and the convex hull */
} CvConvexityDefect;


/** Histogram comparison methods */
enum
{
    CC_COMP_CORREL        =0,
    CC_COMP_CHISQR        =1,
    CC_COMP_INTERSECT     =2,
    CC_COMP_BHATTACHARYYA =3,
    CC_COMP_HELLINGER     =CC_COMP_BHATTACHARYYA,
    CC_COMP_CHISQR_ALT    =4,
    CC_COMP_KL_DIV        =5
};

/** Mask size for distance transform */
enum
{
    CC_DIST_MASK_3   =3,
    CC_DIST_MASK_5   =5,
    CC_DIST_MASK_PRECISE =0
};

/** Content of output label array: connected components or pixels */
enum
{
  CC_DIST_LABEL_CCOMP = 0,
  CC_DIST_LABEL_PIXEL = 1
};

/** Distance types for Distance Transform and M-estimators */
enum
{
    CC_DIST_USER    =-1,  /**< User defined distance */
    CC_DIST_L1      =1,   /**< distance = |x1-x2| + |y1-y2| */
    CC_DIST_L2      =2,   /**< the simple euclidean distance */
    CC_DIST_C       =3,   /**< distance = max(|x1-x2|,|y1-y2|) */
    CC_DIST_L12     =4,   /**< L1-L2 metric: distance = 2(sqrt(1+x*x/2) - 1)) */
    CC_DIST_FAIR    =5,   /**< distance = c^2(|x|/c-log(1+|x|/c)), c = 1.3998 */
    CC_DIST_WELSCH  =6,   /**< distance = c^2/2(1-exp(-(x/c)^2)), c = 2.9846 */
    CC_DIST_HUBER   =7    /**< distance = |x|<c ? x^2/2 : c(|x|-c/2), c=1.345 */
};


/** Threshold types */
enum
{
    CC_THRESH_BINARY      =0,  /**< value = value > threshold ? max_value : 0       */
    CC_THRESH_BINARY_INV  =1,  /**< value = value > threshold ? 0 : max_value       */
    CC_THRESH_TRUNC       =2,  /**< value = value > threshold ? threshold : value   */
    CC_THRESH_TOZERO      =3,  /**< value = value > threshold ? value : 0           */
    CC_THRESH_TOZERO_INV  =4,  /**< value = value > threshold ? 0 : value           */
    CC_THRESH_MASK        =7,
    CC_THRESH_OTSU        =8, /**< use Otsu algorithm to choose the optimal threshold value;
                                 combine the flag with one of the above CC_THRESH_* values */
    CC_THRESH_TRIANGLE    =16  /**< use Triangle algorithm to choose the optimal threshold value;
                                 combine the flag with one of the above CC_THRESH_* values, but not
                                 with CC_THRESH_OTSU */
};

/** Adaptive threshold methods */
enum
{
    CC_ADAPTIVE_THRESH_MEAN_C  =0,
    CC_ADAPTIVE_THRESH_GAUSSIAN_C  =1
};

/** FloodFill flags */
enum
{
    CC_FLOODFILL_FIXED_RANGE =(1 << 16),
    CC_FLOODFILL_MASK_ONLY   =(1 << 17)
};


/** Canny edge detector flags */
enum
{
    CC_CANNY_L2_GRADIENT  =(1 << 31)
};

/** Variants of a Hough transform */
enum
{
    CC_HOUGH_STANDARD =0,
    CC_HOUGH_PROBABILISTIC =1,
    CC_HOUGH_MULTI_SCALE =2,
    CC_HOUGH_GRADIENT =3
};


/* Fast search data structures  */
struct CvFeatureTree;
struct CvLSH;
struct CvLSHOperations;

/** @} */

#ifdef __cplusplus
}
#endif

#endif
