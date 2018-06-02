/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef OPENCC_CORE_PRIVATE_HPP
#define OPENCC_CORE_PRIVATE_HPP

#ifndef __OPENCC_BUILD
#  error this is a private header which should not be used from outside of the OpenCV library
#endif

#include "opencv2/core.hpp"
#include "cvconfig.h"

#ifdef HAVE_EIGEN
#  if defined __GNUC__ && defined __APPLE__
#    pragma GCC diagnostic ignored "-Wshadow"
#  endif
#  include <Eigen/Core>
#  include "opencv2/core/eigen.hpp"
#endif

#ifdef HAVE_TBB
#  include "tbb/tbb.h"
#  include "tbb/task.h"
#  undef min
#  undef max
#endif

#if defined HAVE_FP16 && (defined __F16C__ || (defined _MSC_VER && _MSC_VER >= 1700))
#  include <immintrin.h>
#  define CC_FP16 1
#elif defined HAVE_FP16 && defined __GNUC__
#  define CC_FP16 1
#endif

#ifndef CC_FP16
#  define CC_FP16 0
#endif

//! @cond IGNORED

namespace cv
{
#ifdef HAVE_TBB

    typedef tbb::blocked_range<int> BlockedRange;

    template<typename Body> static inline
    void parallel_for( const BlockedRange& range, const Body& body )
    {
        tbb::parallel_for(range, body);
    }

    typedef tbb::split Split;

    template<typename Body> static inline
    void parallel_reduce( const BlockedRange& range, Body& body )
    {
        tbb::parallel_reduce(range, body);
    }

    typedef tbb::concurrent_vector<Rect> ConcurrentRectVector;
#else
    class BlockedRange
    {
    public:
        BlockedRange() : _begin(0), _end(0), _grainsize(0) {}
        BlockedRange(int b, int e, int g=1) : _begin(b), _end(e), _grainsize(g) {}
        int begin() const { return _begin; }
        int end() const { return _end; }
        int grainsize() const { return _grainsize; }

    protected:
        int _begin, _end, _grainsize;
    };

    template<typename Body> static inline
    void parallel_for( const BlockedRange& range, const Body& body )
    {
        body(range);
    }
    typedef std::vector<Rect> ConcurrentRectVector;

    class Split {};

    template<typename Body> static inline
    void parallel_reduce( const BlockedRange& range, Body& body )
    {
        body(range);
    }
#endif

    // Returns a static string if there is a parallel framework,
    // NULL otherwise.
    CC_EXPORTS const char* currentParallelFramework();
} //namespace cv

/****************************************************************************************\
*                                  Common declarations                                   *
\****************************************************************************************/

/* the alignment of all the allocated buffers */
#define  CC_MALLOC_ALIGN    16

/* IEEE754 constants and macros */
#define  CC_TOGGLE_FLT(x) ((x)^((int)(x) < 0 ? 0x7fffffff : 0))
#define  CC_TOGGLE_DBL(x) ((x)^((int64)(x) < 0 ? CC_BIG_INT(0x7fffffffffffffff) : 0))

static inline void* cvAlignPtr( const void* ptr, int align = 32 )
{
    CC_DbgAssert ( (align & (align-1)) == 0 );
    return (void*)( ((size_t)ptr + align - 1) & ~(size_t)(align-1) );
}

static inline int cvAlign( int size, int align )
{
    CC_DbgAssert( (align & (align-1)) == 0 && size < INT_MAX );
    return (size + align - 1) & -align;
}

#ifdef IMG_DEPTH_8U
static inline Size cvGetMatSize( const CvMat* mat )
{
    return Size(mat->cols, mat->rows);
}
#endif

namespace cv
{
CC_EXPORTS void scalarToRawData(const Scalar& s, void* buf, int type, int unroll_to = 0);
}

// property implementation macros

#define CC_IMPL_PROPERTY_RO(type, name, member) \
    inline type get##name() const { return member; }

#define CC_HELP_IMPL_PROPERTY(r_type, w_type, name, member) \
    CC_IMPL_PROPERTY_RO(r_type, name, member) \
    inline void set##name(w_type val) { member = val; }

#define CC_HELP_WRAP_PROPERTY(r_type, w_type, name, internal_name, internal_obj) \
    r_type get##name() const { return internal_obj.get##internal_name(); } \
    void set##name(w_type val) { internal_obj.set##internal_name(val); }

#define CC_IMPL_PROPERTY(type, name, member) CC_HELP_IMPL_PROPERTY(type, type, name, member)
#define CC_IMPL_PROPERTY_S(type, name, member) CC_HELP_IMPL_PROPERTY(type, const type &, name, member)

#define CC_WRAP_PROPERTY(type, name, internal_name, internal_obj)  CC_HELP_WRAP_PROPERTY(type, type, name, internal_name, internal_obj)
#define CC_WRAP_PROPERTY_S(type, name, internal_name, internal_obj) CC_HELP_WRAP_PROPERTY(type, const type &, name, internal_name, internal_obj)

#define CC_WRAP_SAME_PROPERTY(type, name, internal_obj) CC_WRAP_PROPERTY(type, name, name, internal_obj)
#define CC_WRAP_SAME_PROPERTY_S(type, name, internal_obj) CC_WRAP_PROPERTY_S(type, name, name, internal_obj)

/****************************************************************************************\
*                     Structures and macros for integration with IPP                     *
\****************************************************************************************/

#ifdef HAVE_IPP
#include "ipp.h"

#ifndef IPP_VERSION_UPDATE // prior to 7.1
#define IPP_VERSION_UPDATE 0
#endif

#define IPP_VERSION_X100 (IPP_VERSION_MAJOR * 100 + IPP_VERSION_MINOR*10 + IPP_VERSION_UPDATE)

// General define for ipp function disabling
#define IPP_DISABLE_BLOCK 0

#ifdef CC_MALLOC_ALIGN
#undef CC_MALLOC_ALIGN
#endif
#define CC_MALLOC_ALIGN 32 // required for AVX optimization

#define setIppErrorStatus() ipp::setIppStatus(-1, CC_Func, __FILE__, __LINE__)

static inline IppiSize ippiSize(int width, int height)
{
    IppiSize size = { width, height };
    return size;
}

static inline IppiSize ippiSize(const Size & _size)
{
    IppiSize size = { _size.width, _size.height };
    return size;
}

static inline IppiPoint ippiPoint(const Point & _point)
{
    IppiPoint point = { _point.x, _point.y };
    return point;
}

static inline IppiPoint ippiPoint(int x, int y)
{
    IppiPoint point = { x, y };
    return point;
}

static inline IppiBorderType ippiGetBorderType(int borderTypeNI)
{
    return borderTypeNI == BORDER_CONSTANT ? ippBorderConst :
        borderTypeNI == BORDER_WRAP ? ippBorderWrap :
        borderTypeNI == BORDER_REPLICATE ? ippBorderRepl :
        borderTypeNI == BORDER_REFLECT_101 ? ippBorderMirror :
        borderTypeNI == BORDER_REFLECT ? ippBorderMirrorR : (IppiBorderType)-1;
}

static inline IppDataType ippiGetDataType(int depth)
{
    return depth == CC_8U ? ipp8u :
        depth == CC_8S ? ipp8s :
        depth == CC_16U ? ipp16u :
        depth == CC_16S ? ipp16s :
        depth == CC_32S ? ipp32s :
        depth == CC_32F ? ipp32f :
        depth == CC_64F ? ipp64f : (IppDataType)-1;
}

// IPP temporary buffer hepler
template<typename T>
class IppAutoBuffer
{
public:
    IppAutoBuffer() { m_pBuffer = NULL; }
    IppAutoBuffer(int size) { Alloc(size); }
    ~IppAutoBuffer() { Release(); }
    T* Alloc(int size) { m_pBuffer = (T*)ippMalloc(size); return m_pBuffer; }
    void Release() { if(m_pBuffer) ippFree(m_pBuffer); }
    inline operator T* () { return (T*)m_pBuffer;}
    inline operator const T* () const { return (const T*)m_pBuffer;}
private:
    // Disable copy operations
    IppAutoBuffer(IppAutoBuffer &) {}
    IppAutoBuffer& operator =(const IppAutoBuffer &) {return *this;}

    T* m_pBuffer;
};

#else
#define IPP_VERSION_X100 0
#endif

#if defined HAVE_IPP
#if IPP_VERSION_X100 >= 900
#define IPP_INITIALIZER(FEAT)                           \
{                                                       \
    if(FEAT)                                            \
        ippSetCpuFeatures(FEAT);                        \
    else                                                \
        ippInit();                                      \
}
#elif IPP_VERSION_X100 >= 800
#define IPP_INITIALIZER(FEAT)                           \
{                                                       \
    ippInit();                                          \
}
#else
#define IPP_INITIALIZER(FEAT)                           \
{                                                       \
    ippStaticInit();                                    \
}
#endif

#ifdef CVAPI_EXPORTS
#define IPP_INITIALIZER_AUTO                            \
struct __IppInitializer__                               \
{                                                       \
    __IppInitializer__()                                \
    {IPP_INITIALIZER(ipp::getIppFeatures())}        \
};                                                      \
static struct __IppInitializer__ __ipp_initializer__;
#else
#define IPP_INITIALIZER_AUTO
#endif
#else
#define IPP_INITIALIZER
#define IPP_INITIALIZER_AUTO
#endif

#define CC_IPP_CHECK_COND (ipp::useIPP())
#define CC_IPP_CHECK() if(CC_IPP_CHECK_COND)

#ifdef HAVE_IPP

#ifdef CC_IPP_RUN_VERBOSE
#define CC_IPP_RUN_(condition, func, ...)                                   \
    {                                                                       \
        if (ipp::useIPP() && (condition) && (func))                     \
        {                                                                   \
            printf("%s: IPP implementation is running\n", CC_Func);         \
            fflush(stdout);                                                 \
            CC_IMPL_ADD(CC_IMPL_IPP);                                       \
            return __VA_ARGS__;                                             \
        }                                                                   \
        else                                                                \
        {                                                                   \
            printf("%s: Plain implementation is running\n", CC_Func);       \
            fflush(stdout);                                                 \
        }                                                                   \
    }
#elif defined CC_IPP_RUN_ASSERT
#define CC_IPP_RUN_(condition, func, ...)                                   \
    {                                                                       \
        if (ipp::useIPP() && (condition))                               \
        {                                                                   \
            if(func)                                                        \
            {                                                               \
                CC_IMPL_ADD(CC_IMPL_IPP);                                   \
            }                                                               \
            else                                                            \
            {                                                               \
                setIppErrorStatus();                                        \
                CC_Error(Error::StsAssert, #func);                      \
            }                                                               \
            return __VA_ARGS__;                                             \
        }                                                                   \
    }
#else
#define CC_IPP_RUN_(condition, func, ...)                                   \
    if (ipp::useIPP() && (condition) && (func))                         \
    {                                                                       \
        CC_IMPL_ADD(CC_IMPL_IPP);                                           \
        return __VA_ARGS__;                                                 \
    }
#endif
#define CC_IPP_RUN_FAST(func, ...)                                          \
    if (ipp::useIPP() && (func))                                        \
    {                                                                       \
        CC_IMPL_ADD(CC_IMPL_IPP);                                           \
        return __VA_ARGS__;                                                 \
    }
#else
#define CC_IPP_RUN_(condition, func, ...)
#define CC_IPP_RUN_FAST(func, ...)
#endif

#define CC_IPP_RUN(condition, func, ...) CC_IPP_RUN_((condition), (func), __VA_ARGS__)


#ifndef IPPI_CALL
#  define IPPI_CALL(func) CC_Assert((func) >= 0)
#endif

/* IPP-compatible return codes */
typedef enum CvStatus
{
    CC_BADMEMBLOCK_ERR          = -113,
    CC_INPLACE_NOT_SUPPORTED_ERR= -112,
    CC_UNMATCHED_ROI_ERR        = -111,
    CC_NOTFOUND_ERR             = -110,
    CC_BADCONVERGENCE_ERR       = -109,

    CC_BADDEPTH_ERR             = -107,
    CC_BADROI_ERR               = -106,
    CC_BADHEADER_ERR            = -105,
    CC_UNMATCHED_FORMATS_ERR    = -104,
    CC_UNSUPPORTED_COI_ERR      = -103,
    CC_UNSUPPORTED_CHANNELS_ERR = -102,
    CC_UNSUPPORTED_DEPTH_ERR    = -101,
    CC_UNSUPPORTED_FORMAT_ERR   = -100,

    CC_BADARG_ERR               = -49,  //ipp comp
    CC_NOTDEFINED_ERR           = -48,  //ipp comp

    CC_BADCHANNELS_ERR          = -47,  //ipp comp
    CC_BADRANGE_ERR             = -44,  //ipp comp
    CC_BADSTEP_ERR              = -29,  //ipp comp

    CC_BADFLAG_ERR              =  -12,
    CC_DIV_BY_ZERO_ERR          =  -11, //ipp comp
    CC_BADCOEF_ERR              =  -10,

    CC_BADFACTOR_ERR            =  -7,
    CC_BADPOINT_ERR             =  -6,
    CC_BADSCALE_ERR             =  -4,
    CC_OUTOFMEM_ERR             =  -3,
    CC_NULLPTR_ERR              =  -2,
    CC_BADSIZE_ERR              =  -1,
    CC_NO_ERR                   =   0,
    CC_OK                       =   CC_NO_ERR
}
CvStatus;

#ifdef HAVE_TEGRA_OPTIMIZATION
namespace tegra {

CC_EXPORTS bool useTegra();
CC_EXPORTS void setUseTegra(bool flag);

}
#endif

#ifdef ENABLE_INSTRUMENTATION
namespace cv
{
namespace instr
{
struct InstrTLSStruct
{
    InstrTLSStruct()
    {
        pCurrentNode = NULL;
    }
    InstrNode* pCurrentNode;
};

class InstrStruct
{
public:
    InstrStruct()
    {
        useInstr    = false;
        flags       = FLAGS_MAPPING;
        maxDepth    = 0;

        rootNode.m_payload = NodeData("ROOT", NULL, 0, NULL, false, TYPE_GENERAL, IMPL_PLAIN);
        tlsStruct.get()->pCurrentNode = &rootNode;
    }

    Mutex mutexCreate;
    Mutex mutexCount;

    bool       useInstr;
    int        flags;
    int        maxDepth;
    InstrNode  rootNode;
    TLSData<InstrTLSStruct> tlsStruct;
};

class CC_EXPORTS IntrumentationRegion
{
public:
    IntrumentationRegion(const char* funName, const char* fileName, int lineNum, void *retAddress, bool alwaysExpand, TYPE instrType = TYPE_GENERAL, IMPL implType = IMPL_PLAIN);
    ~IntrumentationRegion();

private:
    bool    m_disabled; // region status
    uint64  m_regionTicks;
};

CC_EXPORTS InstrStruct& getInstrumentStruct();
InstrTLSStruct&         getInstrumentTLSStruct();
CC_EXPORTS InstrNode*   getCurrentNode();
}
}

#ifdef _WIN32
#define CC_INSTRUMENT_GET_RETURN_ADDRESS _ReturnAddress()
#else
#define CC_INSTRUMENT_GET_RETURN_ADDRESS __builtin_extract_return_addr(__builtin_return_address(0))
#endif

// Instrument region
#define CC_INSTRUMENT_REGION_META(NAME, ALWAYS_EXPAND, TYPE, IMPL)        ::instr::IntrumentationRegion __instr_region__(NAME, __FILE__, __LINE__, CC_INSTRUMENT_GET_RETURN_ADDRESS, ALWAYS_EXPAND, TYPE, IMPL);
#define CC_INSTRUMENT_REGION_CUSTOM_META(NAME, ALWAYS_EXPAND, TYPE, IMPL)\
    void *__curr_address__ = [&]() {return CC_INSTRUMENT_GET_RETURN_ADDRESS;}();\
    ::instr::IntrumentationRegion __instr_region__(NAME, __FILE__, __LINE__, __curr_address__, false, ::instr::TYPE_GENERAL, ::instr::IMPL_PLAIN);
// Instrument functions with non-void return type
#define CC_INSTRUMENT_FUN_RT_META(TYPE, IMPL, ERROR_COND, FUN, ...) ([&]()\
{\
    if(::instr::useInstrumentation()){\
        ::instr::IntrumentationRegion __instr__(#FUN, __FILE__, __LINE__, NULL, false, TYPE, IMPL);\
        try{\
            auto status = ((FUN)(__VA_ARGS__));\
            if(ERROR_COND){\
                ::instr::getCurrentNode()->m_payload.m_funError = true;\
                CC_INSTRUMENT_MARK_META(IMPL, #FUN " - BadExit");\
            }\
            return status;\
        }catch(...){\
            ::instr::getCurrentNode()->m_payload.m_funError = true;\
            CC_INSTRUMENT_MARK_META(IMPL, #FUN " - BadExit");\
            throw;\
        }\
    }else{\
        return ((FUN)(__VA_ARGS__));\
    }\
}())
// Instrument functions with void return type
#define CC_INSTRUMENT_FUN_RV_META(TYPE, IMPL, FUN, ...) ([&]()\
{\
    if(::instr::useInstrumentation()){\
        ::instr::IntrumentationRegion __instr__(#FUN, __FILE__, __LINE__, NULL, false, TYPE, IMPL);\
        try{\
            (FUN)(__VA_ARGS__);\
        }catch(...){\
            ::instr::getCurrentNode()->m_payload.m_funError = true;\
            CC_INSTRUMENT_MARK_META(IMPL, #FUN "- BadExit");\
            throw;\
        }\
    }else{\
        (FUN)(__VA_ARGS__);\
    }\
}())
// Instrumentation information marker
#define CC_INSTRUMENT_MARK_META(IMPL, NAME, ...) {::instr::IntrumentationRegion __instr_mark__(NAME, __FILE__, __LINE__, NULL, false, ::instr::TYPE_MARKER, IMPL);}

///// General instrumentation
// General OpenCV region instrumentation macro
#define CC_INSTRUMENT_REGION()              CC_INSTRUMENT_REGION_META(__FUNCTION__, false, ::instr::TYPE_GENERAL, ::instr::IMPL_PLAIN)
// Custom OpenCV region instrumentation macro
#define CC_INSTRUMENT_REGION_NAME(NAME)     CC_INSTRUMENT_REGION_CUSTOM_META(NAME,  false, ::instr::TYPE_GENERAL, ::instr::IMPL_PLAIN)
// Instrumentation for parallel_for_ or other regions which forks and gathers threads
#define CC_INSTRUMENT_REGION_MT_FORK()      CC_INSTRUMENT_REGION_META(__FUNCTION__, true,  ::instr::TYPE_GENERAL, ::instr::IMPL_PLAIN);

///// IPP instrumentation
// Wrapper region instrumentation macro
#define CC_INSTRUMENT_REGION_IPP()          CC_INSTRUMENT_REGION_META(__FUNCTION__, false, ::instr::TYPE_WRAPPER, ::instr::IMPL_IPP)
// Function instrumentation macro
#define CC_INSTRUMENT_FUN_IPP(FUN, ...)     CC_INSTRUMENT_FUN_RT_META(::instr::TYPE_FUN, ::instr::IMPL_IPP, status < 0, FUN, __VA_ARGS__)
// Diagnostic markers
#define CC_INSTRUMENT_MARK_IPP(NAME)        CC_INSTRUMENT_MARK_META(::instr::IMPL_IPP, NAME)

///// OpenCL instrumentation
// Wrapper region instrumentation macro
#define CC_INSTRUMENT_REGION_OPENCL()              CC_INSTRUMENT_REGION_META(__FUNCTION__, false, ::instr::TYPE_WRAPPER, ::instr::IMPL_OPENCL)
// OpenCL kernel compilation wrapper
#define CC_INSTRUMENT_REGION_OPENCL_COMPILE(NAME)  CC_INSTRUMENT_REGION_META(NAME, false, ::instr::TYPE_WRAPPER, ::instr::IMPL_OPENCL)
// OpenCL kernel run wrapper
#define CC_INSTRUMENT_REGION_OPENCL_RUN(NAME)      CC_INSTRUMENT_REGION_META(NAME, false, ::instr::TYPE_FUN, ::instr::IMPL_OPENCL)
// Diagnostic markers
#define CC_INSTRUMENT_MARK_OPENCL(NAME)            CC_INSTRUMENT_MARK_META(::instr::IMPL_OPENCL, NAME)
#else
#define CC_INSTRUMENT_REGION_META(...)

#define CC_INSTRUMENT_REGION()
#define CC_INSTRUMENT_REGION_NAME(...)
#define CC_INSTRUMENT_REGION_MT_FORK()

#define CC_INSTRUMENT_REGION_IPP()
#define CC_INSTRUMENT_FUN_IPP(FUN, ...) ((FUN)(__VA_ARGS__))
#define CC_INSTRUMENT_MARK_IPP(...)

#define CC_INSTRUMENT_REGION_OPENCL()
#define CC_INSTRUMENT_REGION_OPENCL_COMPILE(...)
#define CC_INSTRUMENT_REGION_OPENCL_RUN(...)
#define CC_INSTRUMENT_MARK_OPENCL(...)
#endif

//! @endcond

#endif // OPENCC_CORE_PRIVATE_HPP
