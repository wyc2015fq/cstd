

#include "test_precomp.hpp"
#include <time.h>
#include <limits>
using namespace cv;
using namespace std;

#define CORE_COUNTNONZERO_ERROR_COUNT 1

#define MESSAGE_ERROR_COUNT "Count non zero elements returned by OpenCV function is incorrect."

#define sign(a) a > 0 ? 1 : a == 0 ? 0 : -1

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

class CC_CountNonZeroTest: public cvtest::BaseTest
{
public:
    CC_CountNonZeroTest();
    ~CC_CountNonZeroTest();

protected:
    void run (int);

private:
    float eps_32;
    double eps_64;
    img_t src;
    int current_type;

    void generate_src_data(Size size, int type);
    void generate_src_data(Size size, int type, int count_non_zero);
    void generate_src_stat_data(Size size, int type, int distribution);

    int get_count_non_zero();

    void print_information(int right, int result);
};

CC_CountNonZeroTest::CC_CountNonZeroTest(): eps_32(std::numeric_limits<float>::min()), eps_64(std::numeric_limits<double>::min()), src(img_t()), current_type(-1) {}
CC_CountNonZeroTest::~CC_CountNonZeroTest() {}

void CC_CountNonZeroTest::generate_src_data(Size size, int type)
{
    src cvSetMat(size, CC_MAKETYPE(type, 1));

    for (int j = 0; j < size.width; ++j)
        for (int i = 0; i < size.height; ++i)
            switch (type)
            {
            case CC_8U: { src.at<uchar>(i, j) = randu<uchar>(); break; }
            case CC_8S: { src.at<char>(i, j) = randu<uchar>() - 128; break; }
            case CC_16U: { src.at<ushort>(i, j) = randu<ushort>(); break; }
            case CC_16S: { src.at<short>(i, j) = randu<short>(); break; }
            case CC_32S: { src.at<int>(i, j) = randu<int>(); break; }
            case CC_32F: { src.at<float>(i, j) = randu<float>(); break; }
            case CC_64F: { src.at<double>(i, j) = randu<double>(); break; }
            default: break;
            }
}

void CC_CountNonZeroTest::generate_src_data(Size size, int type, int count_non_zero)
{
    src = img_t::zeros(size, CC_MAKETYPE(type, 1));

    int n = 0; RNG& rng = ts->get_rng();

    while (n < count_non_zero)
    {
        int i = rng.next()%size.height, j = rng.next()%size.width;

        switch (type)
        {
        case CC_8U: { if (!src.at<uchar>(i, j)) {src.at<uchar>(i, j) = randu<uchar>(); n += (src.at<uchar>(i, j) > 0);} break; }
        case CC_8S: { if (!src.at<char>(i, j)) {src.at<char>(i, j) = randu<uchar>() - 128; n += abs(sign(src.at<char>(i, j)));} break; }
        case CC_16U: { if (!src.at<ushort>(i, j)) {src.at<ushort>(i, j) = randu<ushort>(); n += (src.at<ushort>(i, j) > 0);} break; }
        case CC_16S: { if (!src.at<short>(i, j)) {src.at<short>(i, j) = randu<short>(); n += abs(sign(src.at<short>(i, j)));} break; }
        case CC_32S: { if (!src.at<int>(i, j)) {src.at<int>(i, j) = randu<int>(); n += abs(sign(src.at<int>(i, j)));} break; }
        case CC_32F: { if (fabs(src.at<float>(i, j)) <= eps_32) {src.at<float>(i, j) = randu<float>(); n += (fabs(src.at<float>(i, j)) > eps_32);} break; }
        case CC_64F: { if (fabs(src.at<double>(i, j)) <= eps_64) {src.at<double>(i, j) = randu<double>(); n += (fabs(src.at<double>(i, j)) > eps_64);} break; }

        default: break;
        }
    }

}

void CC_CountNonZeroTest::generate_src_stat_data(Size size, int type, int distribution)
{
    src cvSetMat(size, CC_MAKETYPE(type, 1));

    double mean = 0.0, sigma = 1.0;
    double left = -1.0, right = 1.0;

    RNG& rng = ts->get_rng();

    if (distribution == RNG::NORMAL)
        rng.fill(src, RNG::NORMAL, Scalar::all(mean), Scalar::all(sigma));
    else if (distribution == RNG::UNIFORM)
        rng.fill(src, RNG::UNIFORM, Scalar::all(left), Scalar::all(right));
}

int CC_CountNonZeroTest::get_count_non_zero()
{
    int result = 0;

    for (int i = 0; i < src->rows; ++i)
        for (int j = 0; j < src->cols; ++j)
        {
            if (current_type == CC_8U) result += (src.at<uchar>(i, j) > 0);
            else if (current_type == CC_8S) result += abs(sign(src.at<char>(i, j)));
            else if (current_type == CC_16U) result += (src.at<ushort>(i, j) > 0);
            else if (current_type == CC_16S) result += abs(sign(src.at<short>(i, j)));
            else if (current_type == CC_32S) result += abs(sign(src.at<int>(i, j)));
            else if (current_type == CC_32F) result += (fabs(src.at<float>(i, j)) > eps_32);
            else result += (fabs(src.at<double>(i, j)) > eps_64);
        }

    return result;
}

void CC_CountNonZeroTest::print_information(int right, int result)
{
    cout << endl; cout << "Checking for the work of countNonZero function..." << endl; cout << endl;
    cout << "Type of img_t: ";
    switch (current_type)
    {
    case 0: {cout << "CC_8U"; break;}
    case 1: {cout << "CC_8S"; break;}
    case 2: {cout << "CC_16U"; break;}
    case 3: {cout << "CC_16S"; break;}
    case 4: {cout << "CC_32S"; break;}
    case 5: {cout << "CC_32F"; break;}
    case 6: {cout << "CC_64F"; break;}
    default: break;
    }
    cout << endl;
    cout << "Number of rows: " << src->rows << "   Number of cols: " << src->cols << endl;
    cout << "True count non zero elements: " << right << "   Result: " << result << endl;
    cout << endl;
}

void CC_CountNonZeroTest::run(int)
{
    const size_t N = 1500;

    for (int k = 1; k <= 3; ++k)
        for (size_t i = 0; i < N; ++i)
        {
        RNG& rng = ts->get_rng();

        int w = rng.next()%MAX_WIDTH + 1, h = rng.next()%MAX_HEIGHT + 1;

        current_type = rng.next()%7;

        switch (k)
        {
        case 1: {
                generate_src_data(Size(w, h), current_type);
                int right = get_count_non_zero(), result = countNonZero(src);
                if (result != right)
                {
                    cout << "Number of experiment: " << i << endl;
                    cout << "Method of data generation: RANDOM" << endl;
                    print_information(right, result);
                    CC_Error(CORE_COUNTNONZERO_ERROR_COUNT, MESSAGE_ERROR_COUNT);
                    return;
                }

                break;
            }

        case 2: {
                int count_non_zero = rng.next()%(w*h);
                generate_src_data(Size(w, h), current_type, count_non_zero);
                int result = countNonZero(src);
                if (result != count_non_zero)
                {
                    cout << "Number of experiment: " << i << endl;
                    cout << "Method of data generation: HALF-RANDOM" << endl;
                    print_information(count_non_zero, result);
                    CC_Error(CORE_COUNTNONZERO_ERROR_COUNT, MESSAGE_ERROR_COUNT);
                    return;
                }

                break;
            }

        case 3: {
                int distribution = randu<uchar>()%2;
                generate_src_stat_data(Size(w, h), current_type, distribution);
                int right = get_count_non_zero(), result = countNonZero(src);
                if (right != result)
                {
                    cout << "Number of experiment: " << i << endl;
                    cout << "Method of data generation: STATISTIC" << endl;
                    print_information(right, result);
                    CC_Error(CORE_COUNTNONZERO_ERROR_COUNT, MESSAGE_ERROR_COUNT);
                    return;
                }

                break;
            }

        default: break;
        }
    }
}

TEST (Core_CountNonZero, accuracy) { CC_CountNonZeroTest test; test.safe_run(); }
