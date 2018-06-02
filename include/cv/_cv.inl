
#ifndef _CC_INTERNAL_H_
#define _CC_INTERNAL_H_

#if defined _MSC_VER && _MSC_VER >= 1200
    /* disable warnings related to inline functions */
    #pragma warning(disable: 4711 4710 4514)
#endif

#include "cv.h"
#include "cxmisc.h"
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>

typedef unsigned char uchar;
typedef unsigned short ushort;

#ifdef __BORLANDC__
    #define     WIN32
    #define     CC_DLL
    #undef      _CC_ALWAYS_PROFILE_
    #define     _CC_ALWAYS_NO_PROFILE_
#endif

/* helper tables */
extern const uchar icvSaturate8u_cv[];
#define CC_FAST_CAST_8U(t)  (assert(-256 <= (t) || (t) <= 512), icvSaturate8u_cv[(t)+256])
#define CC_CALC_MIN_8U(a,b) (a) -= CC_FAST_CAST_8U((a) - (b))
#define CC_CALC_MAX_8U(a,b) (a) += CC_FAST_CAST_8U((b) - (a))

// -256.f ... 511.f
extern const float icv8x32fTab_cv[];
#define CC_8TO32F(x)  icv8x32fTab_cv[(x)+256]

// (-128.f)^2 ... (255.f)^2
extern const float icv8x32fSqrTab[];
#define CC_8TO32F_SQR(x)  icv8x32fSqrTab[(x)+128]

CC_INLINE  CvDataType icvDepthToDataType(int type);
CC_INLINE  CvDataType icvDepthToDataType(int type)
{
    return (CvDataType)(
            ((((int)cv8u)|((int)cv8s << 4)|((int)cv16u << 8)|
              ((int)cv16s << 12)|((int)cv32s << 16)|((int)cv32f << 20)|
              ((int)cv64f << 24)) >> type*4) & 15);
}

#define CC_HIST_DEFAULT_TYPE CC_32F

CC_EXTERN_C_FUNCPTR(void (C_CDECL * CvWriteNodeFunction)(void* seq,void* node))

#define _CvConvState CvFilterState

typedef struct CPyramid
{
    uchar **ptr;
    CSize *sz;
    double *rate;
    int *step;
    uchar *state;
    int level;
}
CPyramid;

#include "_cvipp.h"
#include "_cvmatrix.h"
#include "_cvgeom.h"
#include "_cvimgproc.h"

// default face cascade
//extern const char* icvDefaultFaceCascade[];

#endif /*_CC_INTERNAL_H_*/
