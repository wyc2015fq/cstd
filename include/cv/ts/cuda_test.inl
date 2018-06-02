

#include "opencv2/ts/cuda_test.hpp"
#include <stdexcept>

using namespace cv;
using namespace cuda;
using namespace cvtest;
using namespace testing;
using namespace testing::internal;

namespace perf
{
    CV_EXPORTS void printCudaInfo();
}

namespace cvtest
{
    //////////////////////////////////////////////////////////////////////
    // random generators

    int randomInt(int minVal, int maxVal)
    {
        RNG& rng = TS::ptr()->get_rng();
        return rng.uniform(minVal, maxVal);
    }

    double randomDouble(double minVal, double maxVal)
    {
        RNG& rng = TS::ptr()->get_rng();
        return rng.uniform(minVal, maxVal);
    }

    Size randomSize(int minVal, int maxVal)
    {
        return Size(randomInt(minVal, maxVal), randomInt(minVal, maxVal));
    }

    Scalar randomScalar(double minVal, double maxVal)
    {
        return Scalar(randomDouble(minVal, maxVal), randomDouble(minVal, maxVal), randomDouble(minVal, maxVal), randomDouble(minVal, maxVal));
    }

    img_t randomMat(Size size, int type, double minVal, double maxVal)
    {
        return randomMat(TS::ptr()->get_rng(), size, type, minVal, maxVal, false);
    }

    //////////////////////////////////////////////////////////////////////
    // GpuMat create

    GpuMat createMat(Size size, int type, bool useRoi)
    {
        Size size0 = size;

        if (useRoi)
        {
            size0.width += randomInt(5, 15);
            size0.height += randomInt(5, 15);
        }

        GpuMat d_m(size0, type);

        if (size0 != size)
            d_m = d_m(Rect((size0.width - size.width) / 2, (size0.height - size.height) / 2, size.width, size.height));

        return d_m;
    }

    GpuMat loadMat(const img_t& m, bool useRoi)
    {
        GpuMat d_m = createMat(m.size(), m CC_MAT_TYPE(), useRoi);
        d_m.upload(m);
        return d_m;
    }

    //////////////////////////////////////////////////////////////////////
    // Image load

    img_t readImage(const std::string& fileName, int flags)
    {
        return imread(TS::ptr()->get_data_path() + fileName, flags);
    }

    img_t readImageType(const std::string& fname, int type)
    {
        img_t src = readImage(fname, CV_MAT_CN(type) == 1 ? IMREAD_GRAYSCALE : IMREAD_COLOR);
        if (CV_MAT_CN(type) == 4)
        {
            img_t temp;
            cvtColor(src, temp, COLOR_BGR2BGRA);
            swap(src, temp);
        }
        src.convertTo(src, CV_MAT_DEPTH(type), CV_MAT_DEPTH(type) == CV_32F ? 1.0 / 255.0 : 1.0);
        return src;
    }

    //////////////////////////////////////////////////////////////////////
    // Gpu devices

    bool supportFeature(const DeviceInfo& info, FeatureSet feature)
    {
        return TargetArchs::builtWith(feature) && info.supports(feature);
    }

    DeviceManager& DeviceManager::instance()
    {
        static DeviceManager obj;
        return obj;
    }

    void DeviceManager::load(int i)
    {
        devices_.clear();
        devices_.reserve(1);

        std::ostringstream msg;

        if (i < 0 || i >= getCudaEnabledDeviceCount())
        {
            msg << "Incorrect device number - " << i;
            throw std::runtime_error(msg.str());
        }

        DeviceInfo info(i);

        if (!info.isCompatible())
        {
            msg << "Device " << i << " [" << info.name() << "] is NOT compatible with current CUDA module build";
            throw std::runtime_error(msg.str());
        }

        devices_.push_back(info);
    }

    void DeviceManager::loadAll()
    {
        int deviceCount = getCudaEnabledDeviceCount();

        devices_.clear();
        devices_.reserve(deviceCount);

        for (int i = 0; i < deviceCount; ++i)
        {
            DeviceInfo info(i);
            if (info.isCompatible())
            {
                devices_.push_back(info);
            }
        }
    }

    void parseCudaDeviceOptions(int argc, char **argv)
    {
        CommandLineParser cmd(argc, argv,
            "{ cuda_device | -1    | CUDA device on which tests will be executed (-1 means all devices) }"
            "{ h help      | false | Print help info                                                    }"
        );

        if (cmd.has("help"))
        {
            std::cout << "\nAvailable options besides google test option: \n";
            cmd.printMessage();
        }

        int device = cmd.get<int>("cuda_device");
        if (device < 0)
        {
            cvtest::DeviceManager::instance().loadAll();
            std::cout << "Run tests on all supported CUDA devices \n" << std::endl;
        }
        else
        {
            cvtest::DeviceManager::instance().load(device);
            cuda::DeviceInfo info(device);
            std::cout << "Run tests on CUDA device " << device << " [" << info.name() << "] \n" << std::endl;
        }
    }

    //////////////////////////////////////////////////////////////////////
    // Additional assertion

    namespace
    {
        template <typename T, typename OutT> std::string printMatValImpl(const img_t& m, Point p)
        {
            const int cn = m CC_MAT_CN();

            std::ostringstream ostr;
            ostr << "(";

            p.x /= cn;

            ostr << static_cast<OutT>(m.at<T>(p.y, p.x * cn));
            for (int c = 1; c < m CC_MAT_CN(); ++c)
            {
                ostr << ", " << static_cast<OutT>(m.at<T>(p.y, p.x * cn + c));
            }
            ostr << ")";

            return ostr.str();
        }

        std::string printMatVal(const img_t& m, Point p)
        {
            typedef std::string (*func_t)(const img_t& m, Point p);

            static const func_t funcs[] =
            {
                printMatValImpl<uchar, int>, printMatValImpl<schar, int>, printMatValImpl<ushort, int>, printMatValImpl<short, int>,
                printMatValImpl<int, int>, printMatValImpl<float, float>, printMatValImpl<double, double>
            };

            return funcs[m CC_MAT_DEPTH()](m, p);
        }
    }

    void minMaxLocGold(const img_t& src, double* minVal_, double* maxVal_, Point* minLoc_, Point* maxLoc_, const img_t& mask)
    {
        if (src CC_MAT_DEPTH() != CV_8S)
        {
            minMaxLoc(src, minVal_, maxVal_, minLoc_, maxLoc_, mask);
            return;
        }

        // OpenCV's minMaxLoc doesn't support CV_8S type
        double minVal = std::numeric_limits<double>::max();
        Point minLoc(-1, -1);

        double maxVal = -std::numeric_limits<double>::max();
        Point maxLoc(-1, -1);

        for (int y = 0; y < src->rows; ++y)
        {
            const schar* src_row = src.ptr<schar>(y);
            const uchar* mask_row = mask.empty() ? 0 : mask.ptr<uchar>(y);

            for (int x = 0; x < src->cols; ++x)
            {
                if (!mask_row || mask_row[x])
                {
                    schar val = src_row[x];

                    if (val < minVal)
                    {
                        minVal = val;
                        minLoc = Point(x, y);
                    }

                    if (val > maxVal)
                    {
                        maxVal = val;
                        maxLoc = Point(x, y);
                    }
                }
            }
        }

        if (minVal_) *minVal_ = minVal;
        if (maxVal_) *maxVal_ = maxVal;

        if (minLoc_) *minLoc_ = minLoc;
        if (maxLoc_) *maxLoc_ = maxLoc;
    }

    img_t getMat(const img_t* arr)
    {
        if (arr.kind() == _InputArray::CUDA_GPU_MAT)
        {
            img_t m;
            arr.getGpuMat().download(m);
            return m;
        }

        return arr;
    }

    AssertionResult assertMatNear(const char* expr1, const char* expr2, const char* eps_expr, const img_t* m1_, const img_t* m2_, double eps)
    {
        img_t m1 = getMat(m1_);
        img_t m2 = getMat(m2_);

        if (m1.size() != m2.size())
        {
            return AssertionFailure() << "Matrices \"" << expr1 << "\" and \"" << expr2 << "\" have different sizes : \""
                                      << expr1 << "\" [" << PrintToString(m1.size()) << "] vs \""
                                      << expr2 << "\" [" << PrintToString(m2.size()) << "]";
        }

        if (m1 CC_MAT_TYPE() != m2 CC_MAT_TYPE())
        {
            return AssertionFailure() << "Matrices \"" << expr1 << "\" and \"" << expr2 << "\" have different types : \""
                                      << expr1 << "\" [" << PrintToString(MatType(m1 CC_MAT_TYPE())) << "] vs \""
                                      << expr2 << "\" [" << PrintToString(MatType(m2 CC_MAT_TYPE())) << "]";
        }

        img_t diff;
        absdiff(m1 cvReshape(,1), m2 cvReshape(,1), diff);

        double maxVal = 0.0;
        Point maxLoc;
        minMaxLocGold(diff, 0, &maxVal, 0, &maxLoc);

        if (maxVal > eps)
        {
            return AssertionFailure() << "The max difference between matrices \"" << expr1 << "\" and \"" << expr2
                                      << "\" is " << maxVal << " at (" << maxLoc.y << ", " << maxLoc.x / m1 CC_MAT_CN() << ")"
                                      << ", which exceeds \"" << eps_expr << "\", where \""
                                      << expr1 << "\" at (" << maxLoc.y << ", " << maxLoc.x / m1 CC_MAT_CN() << ") evaluates to " << printMatVal(m1, maxLoc) << ", \""
                                      << expr2 << "\" at (" << maxLoc.y << ", " << maxLoc.x / m1 CC_MAT_CN() << ") evaluates to " << printMatVal(m2, maxLoc) << ", \""
                                      << eps_expr << "\" evaluates to " << eps;
        }

        return AssertionSuccess();
    }

    double checkSimilarity(const img_t* m1, const img_t* m2)
    {
        img_t diff;
        matchTemplate(getMat(m1), getMat(m2), diff, CC_TM_CCORR_NORMED);
        return std::abs(diff.at<float>(0, 0) - 1.f);
    }

    //////////////////////////////////////////////////////////////////////
    // Helper structs for value-parameterized tests

    vector<MatType> types(int depth_start, int depth_end, int cn_start, int cn_end)
    {
        vector<MatType> v;

        v.reserve((depth_end - depth_start + 1) * (cn_end - cn_start + 1));

        for (int depth = depth_start; depth <= depth_end; ++depth)
        {
            for (int cn = cn_start; cn <= cn_end; ++cn)
            {
                v.push_back(MatType(CV_MAKE_TYPE(depth, cn)));
            }
        }

        return v;
    }

    const vector<MatType>& all_types()
    {
        static vector<MatType> v = types(CV_8U, CV_64F, 1, 4);

        return v;
    }

    void PrintTo(const UseRoi& useRoi, std::ostream* os)
    {
        if (useRoi)
            (*os) << "sub matrix";
        else
            (*os) << "whole matrix";
    }

    void PrintTo(const Inverse& inverse, std::ostream* os)
    {
        if (inverse)
            (*os) << "inverse";
        else
            (*os) << "direct";
    }

    //////////////////////////////////////////////////////////////////////
    // Other

    void dumpImage(const std::string& fileName, const img_t& image)
    {
        imwrite(TS::ptr()->get_data_path() + fileName, image);
    }

    void showDiff(const img_t* gold_, const img_t* actual_, double eps)
    {
        img_t gold = getMat(gold_);
        img_t actual = getMat(actual_);

        img_t diff;
        absdiff(gold, actual, diff);
        threshold(diff, diff, eps, 255.0, CC_THRESH_BINARY);

        namedWindow("gold", WINDOW_NORMAL);
        namedWindow("actual", WINDOW_NORMAL);
        namedWindow("diff", WINDOW_NORMAL);

        imshow("gold", gold);
        imshow("actual", actual);
        imshow("diff", diff);

        waitKey();
    }

    namespace
    {
        bool keyPointsEquals(const KeyPoint& p1, const KeyPoint& p2)
        {
            const double maxPtDif = 1.0;
            const double maxSizeDif = 1.0;
            const double maxAngleDif = 2.0;
            const double maxResponseDif = 0.1;

            double dist = norm(p1.pt - p2.pt);

            if (dist < maxPtDif &&
                fabs(p1.size - p2.size) < maxSizeDif &&
                abs(p1.angle - p2.angle) < maxAngleDif &&
                abs(p1.response - p2.response) < maxResponseDif &&
                p1.octave == p2.octave &&
                p1.class_id == p2.class_id)
            {
                return true;
            }

            return false;
        }

        struct KeyPointLess : std::binary_function<KeyPoint, KeyPoint, bool>
        {
            bool operator()(const KeyPoint& kp1, const KeyPoint& kp2) const
            {
                return kp1.pt.y < kp2.pt.y || (kp1.pt.y == kp2.pt.y && kp1.pt.x < kp2.pt.x);
            }
        };
    }

    testing::AssertionResult assertKeyPointsEquals(const char* gold_expr, const char* actual_expr, std::vector<KeyPoint>& gold, std::vector<KeyPoint>& actual)
    {
        if (gold.size() != actual.size())
        {
            return testing::AssertionFailure() << "KeyPoints size mistmach\n"
                                               << "\"" << gold_expr << "\" : " << gold.size() << "\n"
                                               << "\"" << actual_expr << "\" : " << actual.size();
        }

        std::sort(actual.begin(), actual.end(), KeyPointLess());
        std::sort(gold.begin(), gold.end(), KeyPointLess());

        for (size_t i = 0; i < gold.size(); ++i)
        {
            const KeyPoint& p1 = gold[i];
            const KeyPoint& p2 = actual[i];

            if (!keyPointsEquals(p1, p2))
            {
                return testing::AssertionFailure() << "KeyPoints differ at " << i << "\n"
                                                   << "\"" << gold_expr << "\" vs \"" << actual_expr << "\" : \n"
                                                   << "pt : " << testing::PrintToString(p1.pt) << " vs " << testing::PrintToString(p2.pt) << "\n"
                                                   << "size : " << p1.size << " vs " << p2.size << "\n"
                                                   << "angle : " << p1.angle << " vs " << p2.angle << "\n"
                                                   << "response : " << p1.response << " vs " << p2.response << "\n"
                                                   << "octave : " << p1.octave << " vs " << p2.octave << "\n"
                                                   << "class_id : " << p1.class_id << " vs " << p2.class_id;
            }
        }

        return ::testing::AssertionSuccess();
    }

    int getMatchedPointsCount(std::vector<KeyPoint>& gold, std::vector<KeyPoint>& actual)
    {
        std::sort(actual.begin(), actual.end(), KeyPointLess());
        std::sort(gold.begin(), gold.end(), KeyPointLess());

        int validCount = 0;

        for (size_t i = 0; i < gold.size(); ++i)
        {
            const KeyPoint& p1 = gold[i];
            const KeyPoint& p2 = actual[i];

            if (keyPointsEquals(p1, p2))
                ++validCount;
        }

        return validCount;
    }

    int getMatchedPointsCount(const std::vector<KeyPoint>& keypoints1, const std::vector<KeyPoint>& keypoints2, const std::vector<DMatch>& matches)
    {
        int validCount = 0;

        for (size_t i = 0; i < matches.size(); ++i)
        {
            const DMatch& m = matches[i];

            const KeyPoint& p1 = keypoints1[m.queryIdx];
            const KeyPoint& p2 = keypoints2[m.trainIdx];

            if (keyPointsEquals(p1, p2))
                ++validCount;
        }

        return validCount;
    }

    void printCudaInfo()
    {
        perf::printCudaInfo();
    }
}


void cuda::PrintTo(const DeviceInfo& info, std::ostream* os)
{
    (*os) << info.name();
}
