

#include "test_precomp.hpp"
#include <time.h>

using namespace cv;
using namespace std;

#define sign(a) a > 0 ? 1 : a == 0 ? 0 : -1

#define CORE_EIGEN_ERROR_COUNT 1
#define CORE_EIGEN_ERROR_SIZE  2
#define CORE_EIGEN_ERROR_DIFF  3
#define CORE_EIGEN_ERROR_ORTHO 4
#define CORE_EIGEN_ERROR_ORDER 5

#define MESSAGE_ERROR_COUNT "Matrix of eigen values must have the same rows as source matrix and 1 column."
#define MESSAGE_ERROR_SIZE "Source matrix and matrix of eigen vectors must have the same sizes."
#define MESSAGE_ERROR_DIFF_1 "Accuracy of eigen values computing less than required."
#define MESSAGE_ERROR_DIFF_2 "Accuracy of eigen vectors computing less than required."
#define MESSAGE_ERROR_ORTHO "Matrix of eigen vectors is not orthogonal."
#define MESSAGE_ERROR_ORDER "Eigen values are not sorted in ascending order."

const int COUNT_NORM_TYPES = 3;
const int NORM_TYPE[COUNT_NORM_TYPES] = {NORM_L1, NORM_L2, NORM_INF};

enum TASK_TYPE_EIGEN {VALUES, VECTORS};

class Core_EigenTest: public cvtest::BaseTest
{
public:

    Core_EigenTest();
    ~Core_EigenTest();

protected:

    bool test_values(const img_t& src);												// complex test for eigen without vectors
    bool check_full(int type);													// compex test for symmetric matrix
    virtual void run (int) = 0;													// main testing method

protected:

    float eps_val_32, eps_vec_32;
    float eps_val_64, eps_vec_64;
    int ntests;

    bool check_pair_count(const img_t& src, const img_t& evalues, int low_index = -1, int high_index = -1);
    bool check_pair_count(const img_t& src, const img_t& evalues, const img_t& evectors, int low_index = -1, int high_index = -1);
    bool check_pairs_order(const img_t& eigen_values);											// checking order of eigen values & vectors (it should be none up)
    bool check_orthogonality(const img_t& U);												// checking is matrix of eigen vectors orthogonal
    bool test_pairs(const img_t& src);													// complex test for eigen with vectors

    void print_information(const size_t norm_idx, const img_t& src, double diff, double max_diff);
};

class Core_EigenTest_Scalar : public Core_EigenTest
{
public:
    Core_EigenTest_Scalar() : Core_EigenTest() {}
    ~Core_EigenTest_Scalar();

    virtual void run(int) = 0;
};

class Core_EigenTest_Scalar_32 : public Core_EigenTest_Scalar
{
public:
    Core_EigenTest_Scalar_32() : Core_EigenTest_Scalar() {}
    ~Core_EigenTest_Scalar_32();

    void run(int);
};

class Core_EigenTest_Scalar_64 : public Core_EigenTest_Scalar
{
public:
    Core_EigenTest_Scalar_64() : Core_EigenTest_Scalar() {}
    ~Core_EigenTest_Scalar_64();
    void run(int);
};

class Core_EigenTest_32 : public Core_EigenTest
{
public:
    Core_EigenTest_32(): Core_EigenTest() {}
    ~Core_EigenTest_32() {}
    void run(int);
};

class Core_EigenTest_64 : public Core_EigenTest
{
public:
    Core_EigenTest_64(): Core_EigenTest() {}
    ~Core_EigenTest_64() {}
    void run(int);
};

Core_EigenTest_Scalar::~Core_EigenTest_Scalar() {}
Core_EigenTest_Scalar_32::~Core_EigenTest_Scalar_32() {}
Core_EigenTest_Scalar_64::~Core_EigenTest_Scalar_64() {}

void Core_EigenTest_Scalar_32::run(int)
{
    for (int i = 0; i < ntests; ++i)
    {
        float value = randu<float>();
        img_t src(1, 1, CC_32FC1, Scalar::all((float)value));
        test_values(src);
    }
}

void Core_EigenTest_Scalar_64::run(int)
{
    for (int i = 0; i < ntests; ++i)
    {
        float value = randu<float>();
        img_t src(1, 1, CC_64FC1, Scalar::all((double)value));
        test_values(src);
    }
}

void Core_EigenTest_32::run(int) { check_full(CC_32FC1); }
void Core_EigenTest_64::run(int) { check_full(CC_64FC1); }

Core_EigenTest::Core_EigenTest()
: eps_val_32(1e-3f), eps_vec_32(12e-3f),
  eps_val_64(1e-4f), eps_vec_64(1e-3f), ntests(100) {}
Core_EigenTest::~Core_EigenTest() {}

bool Core_EigenTest::check_pair_count(const img_t& src, const img_t& evalues, int low_index, int high_index)
{
    int n = src->rows, s = sign(high_index);
    if (!( (evalues->rows == n - max<int>(0, low_index) - ((int)((n/2.0)*(s*s-s)) + (1+s-s*s)*(n - (high_index+1)))) && (evalues->cols == 1)))
    {
        std::cout << endl; std::cout << "Checking sizes of eigen values matrix " << evalues << "..." << endl;
        std::cout << "Number of rows: " << evalues->rows << "   Number of cols: " << evalues->cols << endl;
        std::cout << "Size of src symmetric matrix: " << src->rows << " * " << src->cols << endl; std::cout << endl;
        CC_Error(CORE_EIGEN_ERROR_COUNT, MESSAGE_ERROR_COUNT);
        return false;
    }
    return true;
}

bool Core_EigenTest::check_pair_count(const img_t& src, const img_t& evalues, const img_t& evectors, int low_index, int high_index)
{
    int n = src->rows, s = sign(high_index);
    int right_eigen_pair_count = n - max<int>(0, low_index) - ((int)((n/2.0)*(s*s-s)) + (1+s-s*s)*(n - (high_index+1)));

    if (!(evectors->rows == right_eigen_pair_count && evectors->cols == right_eigen_pair_count))
    {
        std::cout << endl; std::cout << "Checking sizes of eigen vectors matrix " << evectors << "..." << endl;
        std::cout << "Number of rows: " << evectors->rows << "   Number of cols: " << evectors->cols << endl;
        std:: cout << "Size of src symmetric matrix: " << src->rows << " * " << src->cols << endl; std::cout << endl;
        CC_Error (CORE_EIGEN_ERROR_SIZE, MESSAGE_ERROR_SIZE);
        return false;
    }

    if (!(evalues->rows == right_eigen_pair_count && evalues->cols == 1))
    {
        std::cout << endl; std::cout << "Checking sizes of eigen values matrix " << evalues << "..." << endl;
        std::cout << "Number of rows: " << evalues->rows << "   Number of cols: " << evalues->cols << endl;
        std:: cout << "Size of src symmetric matrix: " << src->rows << " * " << src->cols << endl; std::cout << endl;
        CC_Error (CORE_EIGEN_ERROR_COUNT, MESSAGE_ERROR_COUNT);
        return false;
    }

    return true;
}

void Core_EigenTest::print_information(const size_t norm_idx, const img_t& src, double diff, double max_diff)
{
    switch (NORM_TYPE[norm_idx])
    {
    case NORM_L1: std::cout << "L1"; break;
    case NORM_L2: std::cout << "L2"; break;
    case NORM_INF: std::cout << "INF"; break;
    default: break;
    }

    cout << "-criteria... " << endl;
    cout << "Source size: " << src->rows << " * " << src->cols << endl;
    cout << "Difference between original eigen vectors matrix and result: " << diff << endl;
    cout << "Maximum allowed difference: " << max_diff << endl; cout << endl;
}

bool Core_EigenTest::check_orthogonality(const img_t& U)
{
    int type = U CC_MAT_TYPE();
    double eps_vec = type == CC_32FC1 ? eps_vec_32 : eps_vec_64;
    img_t UUt; mulTransposed(U, UUt, false);

    img_t E = img_t::eye(U->rows, U->cols, type);

    for (int i = 0; i < COUNT_NORM_TYPES; ++i)
    {
        double diff = cvtest::norm(UUt, E, NORM_TYPE[i]);
        if (diff > eps_vec)
        {
            std::cout << endl; std::cout << "Checking orthogonality of matrix " << U << ": ";
            print_information(i, U, diff, eps_vec);
            CC_Error(CORE_EIGEN_ERROR_ORTHO, MESSAGE_ERROR_ORTHO);
            return false;
        }
    }

    return true;
}

bool Core_EigenTest::check_pairs_order(const img_t& eigen_values)
{
    switch (eigen_values CC_MAT_TYPE())
    {
    case CC_32FC1:
        {
            for (int i = 0; i < (int)(eigen_values CC_MAT_TOTAL() - 1); ++i)
                if (!(eigen_values.at<float>(i, 0) > eigen_values.at<float>(i+1, 0)))
                {
                std::cout << endl; std::cout << "Checking order of eigen values vector " << eigen_values << "..." << endl;
                std::cout << "Pair of indexes with non ascending of eigen values: (" << i << ", " << i+1 << ")." << endl;
                std::cout << endl;
                CC_Error(CORE_EIGEN_ERROR_ORDER, MESSAGE_ERROR_ORDER);
                return false;
            }

            break;
        }

    case CC_64FC1:
        {
            for (int i = 0; i < (int)(eigen_values CC_MAT_TOTAL() - 1); ++i)
                if (!(eigen_values.at<double>(i, 0) > eigen_values.at<double>(i+1, 0)))
                {
                    std::cout << endl; std::cout << "Checking order of eigen values vector " << eigen_values << "..." << endl;
                    std::cout << "Pair of indexes with non ascending of eigen values: (" << i << ", " << i+1 << ")." << endl;
                    std::cout << endl;
                    CC_Error(CORE_EIGEN_ERROR_ORDER, "Eigen values are not sorted in ascending order.");
                    return false;
                }

            break;
        }

    default:;
    }

    return true;
}

bool Core_EigenTest::test_pairs(const img_t& src)
{
    int type = src CC_MAT_TYPE();
    double eps_vec = type == CC_32FC1 ? eps_vec_32 : eps_vec_64;

    img_t eigen_values, eigen_vectors;

    eigen(src, eigen_values, eigen_vectors);

    if (!check_pair_count(src, eigen_values, eigen_vectors))
        return false;

    if (!check_orthogonality (eigen_vectors))
        return false;

    if (!check_pairs_order(eigen_values))
        return false;

    img_t eigen_vectors_t; transpose(eigen_vectors, eigen_vectors_t);

    img_t src_evec(src->rows, src->cols, type);
    src_evec = src*eigen_vectors_t;

    img_t eval_evec(src->rows, src->cols, type);

    switch (type)
    {
    case CC_32FC1:
        {
            for (int i = 0; i < src->cols; ++i)
            {
                img_t tmp = eigen_values.at<float>(i, 0) * eigen_vectors_t.col(i);
                for (int j = 0; j < src->rows; ++j) eval_evec.at<float>(j, i) = tmp.at<float>(j, 0);
            }

            break;
        }

    case CC_64FC1:
        {
            for (int i = 0; i < src->cols; ++i)
            {
                img_t tmp = eigen_values.at<double>(i, 0) * eigen_vectors_t.col(i);
                for (int j = 0; j < src->rows; ++j) eval_evec.at<double>(j, i) = tmp.at<double>(j, 0);
            }

            break;
        }

    default:;
    }

    img_t disparity = src_evec - eval_evec;

    for (int i = 0; i < COUNT_NORM_TYPES; ++i)
    {
        double diff = cvtest::norm(disparity, NORM_TYPE[i]);
        if (diff > eps_vec)
        {
            std::cout << endl; std::cout << "Checking accuracy of eigen vectors computing for matrix " << src << ": ";
            print_information(i, src, diff, eps_vec);
            CC_Error(CORE_EIGEN_ERROR_DIFF, MESSAGE_ERROR_DIFF_2);
            return false;
        }
    }

    return true;
}

bool Core_EigenTest::test_values(const img_t& src)
{
    int type = src CC_MAT_TYPE();
    double eps_val = type == CC_32FC1 ? eps_val_32 : eps_val_64;

    img_t eigen_values_1, eigen_values_2, eigen_vectors;

    if (!test_pairs(src)) return false;

    eigen(src, eigen_values_1, eigen_vectors);
    eigen(src, eigen_values_2);

    if (!check_pair_count(src, eigen_values_2)) return false;

    for (int i = 0; i < COUNT_NORM_TYPES; ++i)
    {
        double diff = cvtest::norm(eigen_values_1, eigen_values_2, NORM_TYPE[i]);
        if (diff > eps_val)
        {
            std::cout << endl; std::cout << "Checking accuracy of eigen values computing for matrix " << src << ": ";
            print_information(i, src, diff, eps_val);
            CC_Error(CORE_EIGEN_ERROR_DIFF, MESSAGE_ERROR_DIFF_1);
            return false;
        }
    }

    return true;
}

bool Core_EigenTest::check_full(int type)
{
    const int MAX_DEGREE = 7;

    srand((unsigned int)time(0));

    for (int i = 0; i < ntests; ++i)
    {
        int src_size = (int)(std::pow(2.0, (rand()%MAX_DEGREE)+1.));

        img_t src(src_size, src_size, type);

        for (int j = 0; j < src->rows; ++j)
            for (int k = j; k < src->cols; ++k)
                if (type == CC_32FC1)  src.at<float>(k, j) = src.at<float>(j, k) = randu<float>();
        else	src.at<double>(k, j) = src.at<double>(j, k) = randu<double>();

        if (!test_values(src)) return false;
    }

    return true;
}

TEST(Core_Eigen, scalar_32) {Core_EigenTest_Scalar_32 test; test.safe_run(); }
TEST(Core_Eigen, scalar_64) {Core_EigenTest_Scalar_64 test; test.safe_run(); }
TEST(Core_Eigen, vector_32) { Core_EigenTest_32 test; test.safe_run(); }
TEST(Core_Eigen, vector_64) { Core_EigenTest_64 test; test.safe_run(); }
