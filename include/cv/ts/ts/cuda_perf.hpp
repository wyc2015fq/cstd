

#ifndef OPENCV_CUDA_PERF_UTILITY_HPP
#define OPENCV_CUDA_PERF_UTILITY_HPP

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ts/ts_perf.hpp"
#include "cvconfig.h"

namespace perf
{
    #define ALL_BORDER_MODES BorderMode::all()
    #define ALL_INTERPOLATIONS Interpolation::all()

    CV_ENUM(BorderMode, BORDER_REFLECT101, BORDER_REPLICATE, BORDER_CONSTANT, BORDER_REFLECT, BORDER_WRAP)
    CV_ENUM(Interpolation, INTER_NEAREST, INTER_LINEAR, INTER_CUBIC, INTER_AREA)
    CV_ENUM(NormType, NORM_INF, NORM_L1, NORM_L2, NORM_HAMMING, NORM_MINMAX)

    enum { Gray = 1, TwoChannel = 2, BGR = 3, BGRA = 4 };
    CV_ENUM(MatCn, Gray, TwoChannel, BGR, BGRA)

    #define CUDA_CHANNELS_1_3_4 testing::Values(MatCn(Gray), MatCn(BGR), MatCn(BGRA))
    #define CUDA_CHANNELS_1_3 testing::Values(MatCn(Gray), MatCn(BGR))

    #define GET_PARAM(k) std::tr1::get< k >(GetParam())

    #define DEF_PARAM_TEST(name, ...) typedef ::perf::TestBaseWithParam< std::tr1::tuple< __VA_ARGS__ > > name
    #define DEF_PARAM_TEST_1(name, param_type) typedef ::perf::TestBaseWithParam< param_type > name

    DEF_PARAM_TEST_1(Sz, Size);
    typedef perf::Size_MatType Sz_Type;
    DEF_PARAM_TEST(Sz_Depth, Size, perf::MatDepth);
    DEF_PARAM_TEST(Sz_Depth_Cn, Size, perf::MatDepth, MatCn);

    #define CUDA_TYPICAL_MAT_SIZES testing::Values(perf::sz720p, perf::szSXGA, perf::sz1080p)

    #define FAIL_NO_CPU() FAIL() << "No such CPU implementation analogy"

    #define CUDA_SANITY_CHECK(mat, ...) \
        do{ \
            CvMat gpu_##mat(mat); \
            SANITY_CHECK(gpu_##mat, ## __VA_ARGS__); \
        } while(0)

    #define CPU_SANITY_CHECK(mat, ...) \
        do{ \
            CvMat cpu_##mat(mat); \
            SANITY_CHECK(cpu_##mat, ## __VA_ARGS__); \
        } while(0)

    CV_EXPORTS CvMat readImage(const std::string& fileName, int flags = IMREAD_COLOR);

    struct CvtColorInfo
    {
        int scn;
        int dcn;
        int code;

        CvtColorInfo() {}
        explicit CvtColorInfo(int scn_, int dcn_, int code_) : scn(scn_), dcn(dcn_), code(code_) {}
    };
    CV_EXPORTS void PrintTo(const CvtColorInfo& info, std::ostream* os);

    CV_EXPORTS void printCudaInfo();

    CV_EXPORTS void sortKeyPoints(std::vector<KeyPoint>& keypoints, InputOutputArray _descriptors = noArray());

#ifdef HAVE_CUDA
    #define CV_PERF_TEST_CUDA_MAIN(modulename) \
        int main(int argc, char **argv)\
        {\
            const char * impls[] = { "cuda", "plain" };\
            CV_PERF_TEST_MAIN_INTERNALS(modulename, impls, perf::printCudaInfo())\
        }
#else
    #define CV_PERF_TEST_CUDA_MAIN(modulename) \
        int main(int argc, char **argv)\
        {\
            const char * plain_only[] = { "plain" };\
            CV_PERF_TEST_MAIN_INTERNALS(modulename, plain_only)\
        }
#endif
}

#endif // OPENCV_CUDA_PERF_UTILITY_HPP
