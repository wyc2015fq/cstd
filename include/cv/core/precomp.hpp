

#ifndef __OPENCC_PRECOMP_H__
#define __OPENCC_PRECOMP_H__

#define USE_SSE2  (checkHardwareSupport(CC_CPU_SSE))
#define USE_SSE4_2  (checkHardwareSupport(CC_CPU_SSE4_2))
#define USE_AVX  (checkHardwareSupport(CC_CPU_AVX))
#define USE_AVX2  (checkHardwareSupport(CC_CPU_AVX2))

#include "opencv2/core/hal/hal.hpp"
#include "opencv2/core/hal/intrin.hpp"
#include "opencv2/core/sse_utils.hpp"
#include "opencv2/core/neon_utils.hpp"

#include "arithm_core.hpp"
#include "hal_replacement.hpp"

#ifdef HAVE_TEGRA_OPTIMIZATION
#include "cv1/core/core_tegra.hpp"
#else
#define GET_OPTIMIZED(func) (func)
#endif

namespace cv
{

// -128.f ... 255.f
extern const float g_8x32fTab[];
#define CC_8TO32F(x)  g_8x32fTab[(x)+128]

extern const ushort g_8x16uSqrTab[];
#define CC_SQR_8U(x)  g_8x16uSqrTab[(x)+255]

extern const uchar g_Saturate8u[];
#define CC_FAST_CAST_8U(t)   (assert(-256 <= (t) && (t) <= 512), g_Saturate8u[(t)+256])
#define CC_MIN_8U(a,b)       ((a) - CC_FAST_CAST_8U((a) - (b)))
#define CC_MAX_8U(a,b)       ((a) + CC_FAST_CAST_8U((b) - (a)))

template<> inline uchar OpAdd<uchar>::operator ()(uchar a, uchar b) const
{ return CC_FAST_CAST_8U(a + b); }

template<> inline uchar OpSub<uchar>::operator ()(uchar a, uchar b) const
{ return CC_FAST_CAST_8U(a - b); }

template<> inline short OpAbsDiff<short>::operator ()(short a, short b) const
{ return saturate_cast<short>(std::abs(a - b)); }

template<> inline schar OpAbsDiff<schar>::operator ()(schar a, schar b) const
{ return saturate_cast<schar>(std::abs(a - b)); }

template<> inline uchar OpMin<uchar>::operator ()(uchar a, uchar b) const { return CC_MIN_8U(a, b); }

template<> inline uchar OpMax<uchar>::operator ()(uchar a, uchar b) const { return CC_MAX_8U(a, b); }

typedef void (*BinaryFunc)(const uchar* src1, size_t step1,
                       const uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       void*);

typedef void (*BinaryFuncC)(const uchar* src1, size_t step1,
                       const uchar* src2, size_t step2,
                       uchar* dst, size_t step, int width, int height,
                       void*);

BinaryFunc getConvertFuncFp16(int ddepth);
BinaryFunc getConvertFunc(int sdepth, int ddepth);
BinaryFunc getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CC_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CC_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CC_SPARSE_HASH_RATIO    3

#if defined WIN32 || defined _WIN32
void deleteThreadAllocData();
#endif

inline Size getContinuousSize_( int flags, int cols, int rows, int widthScale )
{
    int64 sz = (int64)cols * rows * widthScale;
    return (flags & CvMat::CONTINUOUS_FLAG) != 0 &&
        (int)sz == sz ? Size((int)sz, 1) : Size(cols * widthScale, rows);
}

inline Size getContinuousSize( const CvMat& m1, int widthScale=1 )
{
    return getContinuousSize_(m1.flags,
                              m1.cols, m1.rows, widthScale);
}

inline Size getContinuousSize( const CvMat& m1, const CvMat& m2, int widthScale=1 )
{
    return getContinuousSize_(m1.flags & m2.flags,
                              m1.cols, m1.rows, widthScale);
}

inline Size getContinuousSize( const CvMat& m1, const CvMat& m2,
                               const CvMat& m3, int widthScale=1 )
{
    return getContinuousSize_(m1.flags & m2.flags & m3.flags,
                              m1.cols, m1.rows, widthScale);
}

inline Size getContinuousSize( const CvMat& m1, const CvMat& m2,
                               const CvMat& m3, const CvMat& m4,
                               int widthScale=1 )
{
    return getContinuousSize_(m1.flags & m2.flags & m3.flags & m4.flags,
                              m1.cols, m1.rows, widthScale);
}

inline Size getContinuousSize( const CvMat& m1, const CvMat& m2,
                               const CvMat& m3, const CvMat& m4,
                               const CvMat& m5, int widthScale=1 )
{
    return getContinuousSize_(m1.flags & m2.flags & m3.flags & m4.flags & m5.flags,
                              m1.cols, m1.rows, widthScale);
}

struct NoVec
{
    size_t operator()(const void*, const void*, void*, size_t) const { return 0; }
};

enum { BLOCK_SIZE = 1024 };

#if defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#else
#define ARITHM_USE_IPP 0
#endif

inline bool checkScalar(const CvMat& sc, int atype, int sckind, int akind)
{
    if( sc.dims > 2 || !sc.isContinuous() )
        return false;
    Size sz = sc.size();
    if(sz.width != 1 && sz.height != 1)
        return false;
    int cn = CC_MAT_CN(atype);
    if( akind == _InputArray::MATX && sckind != _InputArray::MATX )
        return false;
    return sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc CC_MAT_TYPE() == CC_64F && cn <= 4);
}

inline bool checkScalar(const CvMat* sc, int atype, int sckind, int akind)
{
    if( sc.dims() > 2 || !sc.isContinuous() )
        return false;
    Size sz = sc.size();
    if(sz.width != 1 && sz.height != 1)
        return false;
    int cn = CC_MAT_CN(atype);
    if( akind == _InputArray::MATX && sckind != _InputArray::MATX )
        return false;
    return sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc CC_MAT_TYPE() == CC_64F && cn <= 4);
}

void convertAndUnrollScalar( const CvMat& sc, int buftype, uchar* scbuf, size_t blocksize );

#ifdef CC_COLLECT_IMPL_DATA
struct ImplCollector
{
    ImplCollector()
    {
        useCollection   = false;
        implFlags       = 0;
    }
    bool useCollection; // enable/disable impl data collection

    int implFlags;
    std::vector<int>    implCode;
    std::vector<String> implFun;

    Mutex mutex;
};
#endif

struct CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        device(0), useOpenCL(-1),
//#endif
        useIPP(-1)
#ifdef HAVE_TEGRA_OPTIMIZATION
        ,useTegra(-1)
#endif
#ifdef HAVE_OPENVX
        ,useOpenVX(-1)
#endif
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    int device;
    ocl::Queue oclQueue;
    int useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    int useIPP; // 1 - use, 0 - do not use, -1 - auto/not initialized
#ifdef HAVE_TEGRA_OPTIMIZATION
    int useTegra; // 1 - use, 0 - do not use, -1 - auto/not initialized
#endif
#ifdef HAVE_OPENVX
    int useOpenVX; // 1 - use, 0 - do not use, -1 - auto/not initialized
#endif
};

TLSData<CoreTLSData>& getCoreTlsData();

#if defined(BUILD_SHARED_LIBS)
#if defined WIN32 || defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#elif defined __GNUC__ && __GNUC__ >= 4
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#else
#define CL_RUNTIME_EXPORT
#endif
#else
#define CL_RUNTIME_EXPORT
#endif

extern bool __termination; // skip some cleanups, because process is terminating
                           // (for example, if ExitProcess() was already called)

Mutex& getInitializationMutex();

// TODO Memory barriers?
#define CC_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_VALUE) \
    static TYPE* volatile instance = NULL; \
    if (instance == NULL) \
    { \
        AutoLock lock(getInitializationMutex()); \
        if (instance == NULL) \
            instance = INITIALIZER; \
    } \
    return RET_VALUE;

#define CC_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CC_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CC_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CC_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

}

#endif /*_CXCORE_INTERNAL_H_*/
