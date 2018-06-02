#include "test_precomp.hpp"

using namespace cv;
using namespace std;

TEST(Core_OutputArrayCreate, _1997)
{
    struct local {
        static void create(img_t* arr, Size submatSize, int type)
        {
            int sizes[] = {submatSize.width, submatSize.height};
            arr cvSetMat(sizeof(sizes)/sizeof(sizes[0]), sizes, type);
        }
    };

    img_t mat(Size(512, 512), CC_8U);
    Size submatSize = Size(256, 256);

    ASSERT_NO_THROW(local::create( mat(Rect(Point(), submatSize)), submatSize, mat CC_MAT_TYPE() ));
}

TEST(Core_SaturateCast, NegativeNotClipped)
{
    double d = -1.0;
    unsigned int val = saturate_cast<unsigned int>(d);

    ASSERT_EQ(0xffffffff, val);
}

template<typename T, typename U>
static double maxAbsDiff(const T &t, const U &u)
{
  Mat_<double> d;
  absdiff(t, u, d);
  double ret;
  minMaxLoc(d, NULL, &ret);
  return ret;
}

TEST(Core_OutputArrayAssign, _Matxd_Matd)
{
    img_t expected = (Mat_<double>(2,3) << 1, 2, 3, .1, .2, .3);
    Matx23d actualx;

    {
        img_t* oa(actualx);
        oa.assign(expected);
    }

    img_t actual = (img_t) actualx;

    EXPECT_LE(maxAbsDiff(expected, actual), 0.0);
}

TEST(Core_OutputArrayAssign, _Matxd_Matf)
{
    img_t expected = (Mat_<float>(2,3) << 1, 2, 3, .1, .2, .3);
    Matx23d actualx;

    {
        img_t* oa(actualx);
        oa.assign(expected);
    }

    img_t actual = (img_t) actualx;

    EXPECT_LE(maxAbsDiff(expected, actual), FLT_EPSILON);
}

TEST(Core_OutputArrayAssign, _Matxf_Matd)
{
    img_t expected = (Mat_<double>(2,3) << 1, 2, 3, .1, .2, .3);
    Matx23f actualx;

    {
        img_t* oa(actualx);
        oa.assign(expected);
    }

    img_t actual = (img_t) actualx;

    EXPECT_LE(maxAbsDiff(expected, actual), FLT_EPSILON);
}

TEST(Core_OutputArrayAssign, _Matxd_UMatd)
{
    img_t expected = (Mat_<double>(2,3) << 1, 2, 3, .1, .2, .3);
    UMat uexpected = expected.getUMat(ACCESS_READ);
    Matx23d actualx;

    {
        img_t* oa(actualx);
        oa.assign(uexpected);
    }

    img_t actual = (img_t) actualx;

    EXPECT_LE(maxAbsDiff(expected, actual), 0.0);
}

TEST(Core_OutputArrayAssign, _Matxd_UMatf)
{
    img_t expected = (Mat_<float>(2,3) << 1, 2, 3, .1, .2, .3);
    UMat uexpected = expected.getUMat(ACCESS_READ);
    Matx23d actualx;

    {
        img_t* oa(actualx);
        oa.assign(uexpected);
    }

    img_t actual = (img_t) actualx;

    EXPECT_LE(maxAbsDiff(expected, actual), FLT_EPSILON);
}

TEST(Core_OutputArrayAssign, _Matxf_UMatd)
{
    img_t expected = (Mat_<double>(2,3) << 1, 2, 3, .1, .2, .3);
    UMat uexpected = expected.getUMat(ACCESS_READ);
    Matx23f actualx;

    {
        img_t* oa(actualx);
        oa.assign(uexpected);
    }

    img_t actual = (img_t) actualx;

    EXPECT_LE(maxAbsDiff(expected, actual), FLT_EPSILON);
}


TEST(Core_String, find_last_of__with__empty_string)
{
    String s;
    size_t p = s.find_last_of("q", 0);
    // npos is not exported: EXPECT_EQ(String::npos, p);
    EXPECT_EQ(std::string::npos, p);
}
