

#ifndef OPENCV_CUDA_TEST_UTILITY_HPP
#define OPENCV_CUDA_TEST_UTILITY_HPP

#include <stdexcept>
#include "cvconfig.h"
#include "opencv2/core.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/ts.hpp"

namespace cvtest
{
    //////////////////////////////////////////////////////////////////////
    // random generators

    CV_EXPORTS int randomInt(int minVal, int maxVal);
    CV_EXPORTS double randomDouble(double minVal, double maxVal);
    CV_EXPORTS Size randomSize(int minVal, int maxVal);
    CV_EXPORTS Scalar randomScalar(double minVal, double maxVal);
    CV_EXPORTS CvMat randomMat(Size size, int type, double minVal = 0.0, double maxVal = 255.0);

    //////////////////////////////////////////////////////////////////////
    // GpuMat create

    CV_EXPORTS cuda::GpuMat createMat(Size size, int type, bool useRoi = false);
    CV_EXPORTS cuda::GpuMat loadMat(const CvMat& m, bool useRoi = false);

    //////////////////////////////////////////////////////////////////////
    // Image load

    //! read image from testdata folder
    CV_EXPORTS CvMat readImage(const std::string& fileName, int flags = IMREAD_COLOR);

    //! read image from testdata folder and convert it to specified type
    CV_EXPORTS CvMat readImageType(const std::string& fname, int type);

    //////////////////////////////////////////////////////////////////////
    // Gpu devices

    //! return true if device supports specified feature and gpu module was built with support the feature.
    CV_EXPORTS bool supportFeature(const cuda::DeviceInfo& info, cuda::FeatureSet feature);

    class CV_EXPORTS DeviceManager
    {
    public:
        static DeviceManager& instance();

        void load(int i);
        void loadAll();

        const std::vector<cuda::DeviceInfo>& values() const { return devices_; }

    private:
        std::vector<cuda::DeviceInfo> devices_;
    };

    #define ALL_DEVICES testing::ValuesIn(cvtest::DeviceManager::instance().values())

    //////////////////////////////////////////////////////////////////////
    // Additional assertion

    CV_EXPORTS void minMaxLocGold(const CvMat& src, double* minVal_, double* maxVal_ = 0, Point* minLoc_ = 0, Point* maxLoc_ = 0, const CvMat& mask = CvMat());

    CV_EXPORTS CvMat getMat(const CvMat* arr);

    CV_EXPORTS testing::AssertionResult assertMatNear(const char* expr1, const char* expr2, const char* eps_expr, const CvMat* m1, const CvMat* m2, double eps);

    #define EXPECT_MAT_NEAR(m1, m2, eps) EXPECT_PRED_FORMAT3(cvtest::assertMatNear, m1, m2, eps)
    #define ASSERT_MAT_NEAR(m1, m2, eps) ASSERT_PRED_FORMAT3(cvtest::assertMatNear, m1, m2, eps)

    #define EXPECT_SCALAR_NEAR(s1, s2, eps) \
        { \
            EXPECT_NEAR(s1[0], s2[0], eps); \
            EXPECT_NEAR(s1[1], s2[1], eps); \
            EXPECT_NEAR(s1[2], s2[2], eps); \
            EXPECT_NEAR(s1[3], s2[3], eps); \
        }
    #define ASSERT_SCALAR_NEAR(s1, s2, eps) \
        { \
            ASSERT_NEAR(s1[0], s2[0], eps); \
            ASSERT_NEAR(s1[1], s2[1], eps); \
            ASSERT_NEAR(s1[2], s2[2], eps); \
            ASSERT_NEAR(s1[3], s2[3], eps); \
        }

    #define EXPECT_POINT2_NEAR(p1, p2, eps) \
        { \
            EXPECT_NEAR(p1.x, p2.x, eps); \
            EXPECT_NEAR(p1.y, p2.y, eps); \
        }
    #define ASSERT_POINT2_NEAR(p1, p2, eps) \
        { \
            ASSERT_NEAR(p1.x, p2.x, eps); \
            ASSERT_NEAR(p1.y, p2.y, eps); \
        }

    #define EXPECT_POINT3_NEAR(p1, p2, eps) \
        { \
            EXPECT_NEAR(p1.x, p2.x, eps); \
            EXPECT_NEAR(p1.y, p2.y, eps); \
            EXPECT_NEAR(p1.z, p2.z, eps); \
        }
    #define ASSERT_POINT3_NEAR(p1, p2, eps) \
        { \
            ASSERT_NEAR(p1.x, p2.x, eps); \
            ASSERT_NEAR(p1.y, p2.y, eps); \
            ASSERT_NEAR(p1.z, p2.z, eps); \
        }

    CV_EXPORTS double checkSimilarity(const CvMat* m1, const CvMat* m2);

    #define EXPECT_MAT_SIMILAR(mat1, mat2, eps) \
        { \
            ASSERT_EQ(mat1 CC_MAT_TYPE(), mat2 CC_MAT_TYPE()); \
            ASSERT_EQ(mat1.size(), mat2.size()); \
            EXPECT_LE(checkSimilarity(mat1, mat2), eps); \
        }
    #define ASSERT_MAT_SIMILAR(mat1, mat2, eps) \
        { \
            ASSERT_EQ(mat1 CC_MAT_TYPE(), mat2 CC_MAT_TYPE()); \
            ASSERT_EQ(mat1.size(), mat2.size()); \
            ASSERT_LE(checkSimilarity(mat1, mat2), eps); \
        }

    //////////////////////////////////////////////////////////////////////
    // Helper structs for value-parameterized tests

    #define CUDA_TEST_P(test_case_name, test_name) \
      class GTEST_TEST_CLASS_NAME_(test_case_name, test_name) \
          : public test_case_name { \
       public: \
        GTEST_TEST_CLASS_NAME_(test_case_name, test_name)() {} \
        virtual void TestBody(); \
       private: \
        void UnsafeTestBody(); \
        static int AddToRegistry() { \
          ::testing::UnitTest::GetInstance()->parameterized_test_registry(). \
              GetTestCasePatternHolder<test_case_name>(\
                  #test_case_name, \
                  ::testing::internal::CodeLocation(\
                      __FILE__, __LINE__))->AddTestPattern(\
                          #test_case_name, \
                          #test_name, \
                          new ::testing::internal::TestMetaFactory< \
                              GTEST_TEST_CLASS_NAME_(\
                                  test_case_name, test_name)>()); \
          return 0; \
        } \
        static int gtest_registering_dummy_ GTEST_ATTRIBUTE_UNUSED_; \
        GTEST_DISALLOW_COPY_AND_ASSIGN_(\
            GTEST_TEST_CLASS_NAME_(test_case_name, test_name)); \
      }; \
      int GTEST_TEST_CLASS_NAME_(test_case_name, \
                                 test_name)::gtest_registering_dummy_ = \
          GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::AddToRegistry(); \
      void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::TestBody() \
      { \
        try \
        { \
          UnsafeTestBody(); \
        } \
        catch (...) \
        { \
          cuda::resetDevice(); \
          throw; \
        } \
      } \
      void GTEST_TEST_CLASS_NAME_(test_case_name, test_name)::UnsafeTestBody()

    #define DIFFERENT_SIZES testing::Values(Size(128, 128), Size(113, 113))

    // Depth

    using perf::MatDepth;

    #define ALL_DEPTH testing::Values(MatDepth(CV_8U), MatDepth(CV_8S), MatDepth(CV_16U), MatDepth(CV_16S), MatDepth(CV_32S), MatDepth(CV_32F), MatDepth(CV_64F))

    #define DEPTH_PAIRS testing::Values(std::make_pair(MatDepth(CV_8U), MatDepth(CV_8U)),   \
                                        std::make_pair(MatDepth(CV_8U), MatDepth(CV_16U)),  \
                                        std::make_pair(MatDepth(CV_8U), MatDepth(CV_16S)),  \
                                        std::make_pair(MatDepth(CV_8U), MatDepth(CV_32S)),  \
                                        std::make_pair(MatDepth(CV_8U), MatDepth(CV_32F)),  \
                                        std::make_pair(MatDepth(CV_8U), MatDepth(CV_64F)),  \
                                                                                            \
                                        std::make_pair(MatDepth(CV_16U), MatDepth(CV_16U)), \
                                        std::make_pair(MatDepth(CV_16U), MatDepth(CV_32S)), \
                                        std::make_pair(MatDepth(CV_16U), MatDepth(CV_32F)), \
                                        std::make_pair(MatDepth(CV_16U), MatDepth(CV_64F)), \
                                                                                            \
                                        std::make_pair(MatDepth(CV_16S), MatDepth(CV_16S)), \
                                        std::make_pair(MatDepth(CV_16S), MatDepth(CV_32S)), \
                                        std::make_pair(MatDepth(CV_16S), MatDepth(CV_32F)), \
                                        std::make_pair(MatDepth(CV_16S), MatDepth(CV_64F)), \
                                                                                            \
                                        std::make_pair(MatDepth(CV_32S), MatDepth(CV_32S)), \
                                        std::make_pair(MatDepth(CV_32S), MatDepth(CV_32F)), \
                                        std::make_pair(MatDepth(CV_32S), MatDepth(CV_64F)), \
                                                                                            \
                                        std::make_pair(MatDepth(CV_32F), MatDepth(CV_32F)), \
                                        std::make_pair(MatDepth(CV_32F), MatDepth(CV_64F)), \
                                                                                            \
                                        std::make_pair(MatDepth(CV_64F), MatDepth(CV_64F)))

    // Type

    using perf::MatType;

    //! return vector with types from specified range.
    CV_EXPORTS std::vector<MatType> types(int depth_start, int depth_end, int cn_start, int cn_end);

    //! return vector with all types (depth: CV_8U-CV_64F, channels: 1-4).
    CV_EXPORTS const std::vector<MatType>& all_types();

    #define ALL_TYPES testing::ValuesIn(all_types())
    #define TYPES(depth_start, depth_end, cn_start, cn_end) testing::ValuesIn(types(depth_start, depth_end, cn_start, cn_end))

    // ROI

    class UseRoi
    {
    public:
        inline UseRoi(bool val = false) : val_(val) {}

        inline operator bool() const { return val_; }

    private:
        bool val_;
    };

    CV_EXPORTS void PrintTo(const UseRoi& useRoi, std::ostream* os);

    #define WHOLE_SUBMAT testing::Values(UseRoi(false), UseRoi(true))

    // Direct/Inverse

    class Inverse
    {
    public:
        inline Inverse(bool val = false) : val_(val) {}

        inline operator bool() const { return val_; }

    private:
        bool val_;
    };

    CV_EXPORTS void PrintTo(const Inverse& useRoi, std::ostream* os);

    #define DIRECT_INVERSE testing::Values(Inverse(false), Inverse(true))

    // Param class

    #define IMPLEMENT_PARAM_CLASS(name, type) \
        class name \
        { \
        public: \
            name ( type arg = type ()) : val_(arg) {} \
            operator type () const {return val_;} \
        private: \
            type val_; \
        }; \
        inline void PrintTo( name param, std::ostream* os) \
        { \
            *os << #name <<  "(" << testing::PrintToString(static_cast< type >(param)) << ")"; \
        }

    IMPLEMENT_PARAM_CLASS(Channels, int)

    #define ALL_CHANNELS testing::Values(Channels(1), Channels(2), Channels(3), Channels(4))
    #define IMAGE_CHANNELS testing::Values(Channels(1), Channels(3), Channels(4))

    // Flags and enums

    CV_ENUM(NormCode, NORM_INF, NORM_L1, NORM_L2, NORM_TYPE_MASK, NORM_RELATIVE, NORM_MINMAX)

    CV_ENUM(Interpolation, INTER_NEAREST, INTER_LINEAR, INTER_CUBIC, INTER_AREA)

    CV_ENUM(BorderType, BORDER_REFLECT101, BORDER_REPLICATE, BORDER_CONSTANT, BORDER_REFLECT, BORDER_WRAP)
    #define ALL_BORDER_TYPES testing::Values(BorderType(BORDER_REFLECT101), BorderType(BORDER_REPLICATE), BorderType(BORDER_CONSTANT), BorderType(BORDER_REFLECT), BorderType(BORDER_WRAP))

    CV_FLAGS(WarpFlags, INTER_NEAREST, INTER_LINEAR, INTER_CUBIC, WARP_INVERSE_MAP)

    //////////////////////////////////////////////////////////////////////
    // Features2D

    CV_EXPORTS testing::AssertionResult assertKeyPointsEquals(const char* gold_expr, const char* actual_expr, std::vector<KeyPoint>& gold, std::vector<KeyPoint>& actual);

    #define ASSERT_KEYPOINTS_EQ(gold, actual) EXPECT_PRED_FORMAT2(assertKeyPointsEquals, gold, actual)

    CV_EXPORTS int getMatchedPointsCount(std::vector<KeyPoint>& gold, std::vector<KeyPoint>& actual);
    CV_EXPORTS int getMatchedPointsCount(const std::vector<KeyPoint>& keypoints1, const std::vector<KeyPoint>& keypoints2, const std::vector<DMatch>& matches);

    //////////////////////////////////////////////////////////////////////
    // Other

    CV_EXPORTS void dumpImage(const std::string& fileName, const CvMat& image);
    CV_EXPORTS void showDiff(const CvMat* gold, const CvMat* actual, double eps);

    CV_EXPORTS void parseCudaDeviceOptions(int argc, char **argv);
    CV_EXPORTS void printCudaInfo();
}

namespace cv { namespace cuda
{
    CV_EXPORTS void PrintTo(const DeviceInfo& info, std::ostream* os);
}}

#ifdef HAVE_CUDA

#define CV_CUDA_TEST_MAIN(resourcesubdir) \
    CV_TEST_MAIN(resourcesubdir, cvtest::parseCudaDeviceOptions(argc, argv), cvtest::printCudaInfo(), setUseOptimized(false))

#else // HAVE_CUDA

#define CV_CUDA_TEST_MAIN(resourcesubdir) \
    int main() \
    { \
        printf("OpenCV was built without CUDA support\n"); \
        return 0; \
    }

#endif // HAVE_CUDA


#endif // OPENCV_CUDA_TEST_UTILITY_HPP
