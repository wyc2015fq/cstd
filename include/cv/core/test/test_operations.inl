

#include "test_precomp.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
#include <numeric>

using namespace cv;
using namespace std;


class CC_OperationsTest : public cvtest::BaseTest
{
public:
    CC_OperationsTest();
    ~CC_OperationsTest();
protected:
    void run(int);

    struct test_excep
    {
        test_excep(const string& _s=string("")) : s(_s) { }
        string s;
    };

    bool SomeMatFunctions();
    bool TestMat();
    template<typename _Tp> void TestType(Size sz, _Tp value);
    bool TestTemplateMat();
    bool TestMatND();
    bool TestSparseMat();
    bool TestVec();
    bool TestMatxMultiplication();
    bool TestMatxElementwiseDivison();
    bool TestSubMatAccess();
    bool TestExp();
    bool TestSVD();
    bool operations1();

    void checkDiff(const img_t& m1, const img_t& m2, const string& s)
    {
        if (cvtest::norm(m1, m2, NORM_INF) != 0) throw test_excep(s);
    }
    void checkDiffF(const img_t& m1, const img_t& m2, const string& s)
    {
        if (cvtest::norm(m1, m2, NORM_INF) > 1e-5) throw test_excep(s);
    }
};

CC_OperationsTest::CC_OperationsTest()
{
}

CC_OperationsTest::~CC_OperationsTest() {}

#define STR(a) STR2(a)
#define STR2(a) #a

#define CHECK_DIFF(a, b) checkDiff(a, b, "(" #a ")  !=  (" #b ")  at l." STR(__LINE__))
#define CHECK_DIFF_FLT(a, b) checkDiffF(a, b, "(" #a ")  !=(eps)  (" #b ")  at l." STR(__LINE__))

#if defined _MSC_VER && _MSC_VER < 1400
#define MSVC_OLD 1
#else
#define MSVC_OLD 0
#endif

template<typename _Tp> void CC_OperationsTest::TestType(Size sz, _Tp value)
{
    Mat_<_Tp> m(sz);
    CC_Assert(m->cols == sz.width && m->rows == sz.height && m CC_MAT_DEPTH() == DataType<_Tp>::depth &&
              m CC_MAT_CN() == DataType<_Tp>::channels &&
              m CC_ELEM_SIZE() == sizeof(_Tp) && m->step == m CC_ELEM_SIZE()*m->cols);
    for( int y = 0; y < sz.height; y++ )
        for( int x = 0; x < sz.width; x++ )
        {
            m(y,x) = value;
        }

    double s = sum(img_t(m) cvReshape(,1))[0];
    CC_Assert( s == (double)sz.width*sz.height );
}

bool CC_OperationsTest::TestMat()
{
    try
    {
        img_t one_3x1(3, 1, CC_32F, Scalar(1.0));
        img_t shi_3x1(3, 1, CC_32F, Scalar(1.2));
        img_t shi_2x1(2, 1, CC_32F, Scalar(-1));
        Scalar shift = Scalar::all(15);

        float data[] = { sqrt(2.f)/2, -sqrt(2.f)/2, 1.f, sqrt(2.f)/2, sqrt(2.f)/2, 10.f };
        img_t rot_2x3(2, 3, CC_32F, data);

        img_t res = one_3x1 + shi_3x1 + shi_3x1 + shi_3x1;
        res = img_t(img_t(2 * rot_2x3) * res - shi_2x1) + shift;

        img_t tmp, res2;
        add(one_3x1, shi_3x1, tmp);
        add(tmp, shi_3x1, tmp);
        add(tmp, shi_3x1, tmp);
        gemm(rot_2x3, tmp, 2, shi_2x1, -1, res2, 0);
        add(res2, img_t(2, 1, CC_32F, shift), res2);

        CHECK_DIFF(res, res2);

        img_t mat4x4(4, 4, CC_32F);
        randu(mat4x4, Scalar(0), Scalar(10));

        img_t roi1 = mat4x4(Rect(Point(1, 1), Size(2, 2)));
        img_t roi2 = mat4x4(CRange(1, 3), CRange(1, 3));

        CHECK_DIFF(roi1, roi2);
        CHECK_DIFF(mat4x4, mat4x4(Rect(Point(0,0), mat4x4.size())));

        img_t intMat10(3, 3, CC_32S, Scalar(10));
        img_t intMat11(3, 3, CC_32S, Scalar(11));
        img_t resMat(3, 3, CC_8U, Scalar(255));

        CHECK_DIFF(resMat, intMat10 == intMat10);
        CHECK_DIFF(resMat, intMat10 <  intMat11);
        CHECK_DIFF(resMat, intMat11 >  intMat10);
        CHECK_DIFF(resMat, intMat10 <= intMat11);
        CHECK_DIFF(resMat, intMat11 >= intMat10);
        CHECK_DIFF(resMat, intMat11 != intMat10);

        CHECK_DIFF(resMat, intMat10 == 10.0);
        CHECK_DIFF(resMat, 10.0 == intMat10);
        CHECK_DIFF(resMat, intMat10 <  11.0);
        CHECK_DIFF(resMat, 11.0 > intMat10);
        CHECK_DIFF(resMat, 10.0 < intMat11);
        CHECK_DIFF(resMat, 11.0 >= intMat10);
        CHECK_DIFF(resMat, 10.0 <= intMat11);
        CHECK_DIFF(resMat, 10.0 != intMat11);
        CHECK_DIFF(resMat, intMat11 != 10.0);

        img_t eye =  img_t::eye(3, 3, CC_16S);
        img_t maskMat4(3, 3, CC_16S, Scalar(4));
        img_t maskMat1(3, 3, CC_16S, Scalar(1));
        img_t maskMat5(3, 3, CC_16S, Scalar(5));
        img_t maskMat0(3, 3, CC_16S, Scalar(0));

        CHECK_DIFF(maskMat0, maskMat4 & maskMat1);
        CHECK_DIFF(maskMat0, Scalar(1) & maskMat4);
        CHECK_DIFF(maskMat0, maskMat4 & Scalar(1));

        img_t m;
        m = maskMat4.clone(); m &= maskMat1; CHECK_DIFF(maskMat0, m);
        m = maskMat4.clone(); m &= maskMat1 | maskMat1; CHECK_DIFF(maskMat0, m);
        m = maskMat4.clone(); m &= (2* maskMat1 - maskMat1); CHECK_DIFF(maskMat0, m);

        m = maskMat4.clone(); m &= Scalar(1); CHECK_DIFF(maskMat0, m);
        m = maskMat4.clone(); m |= maskMat1; CHECK_DIFF(maskMat5, m);
        m = maskMat5.clone(); m ^= maskMat1; CHECK_DIFF(maskMat4, m);
        m = maskMat4.clone(); m |= (2* maskMat1 - maskMat1); CHECK_DIFF(maskMat5, m);
        m = maskMat5.clone(); m ^= (2* maskMat1 - maskMat1); CHECK_DIFF(maskMat4, m);

        m = maskMat4.clone(); m |= Scalar(1); CHECK_DIFF(maskMat5, m);
        m = maskMat5.clone(); m ^= Scalar(1); CHECK_DIFF(maskMat4, m);



        CHECK_DIFF(maskMat0, (maskMat4 | maskMat4) & (maskMat1 | maskMat1));
        CHECK_DIFF(maskMat0, (maskMat4 | maskMat4) & maskMat1);
        CHECK_DIFF(maskMat0, maskMat4 & (maskMat1 | maskMat1));
        CHECK_DIFF(maskMat0, (maskMat1 | maskMat1) & Scalar(4));
        CHECK_DIFF(maskMat0, Scalar(4) & (maskMat1 | maskMat1));

        CHECK_DIFF(maskMat0, maskMat5 ^ (maskMat4 | maskMat1));
        CHECK_DIFF(maskMat0, (maskMat4 | maskMat1) ^ maskMat5);
        CHECK_DIFF(maskMat0, (maskMat4 + maskMat1) ^ (maskMat4 + maskMat1));
        CHECK_DIFF(maskMat0, Scalar(5) ^ (maskMat4 | Scalar(1)));
        CHECK_DIFF(maskMat1, Scalar(5) ^ maskMat4);
        CHECK_DIFF(maskMat0, Scalar(5) ^ (maskMat4 + maskMat1));
        CHECK_DIFF(maskMat5, Scalar(5) | (maskMat4 + maskMat1));
        CHECK_DIFF(maskMat0, (maskMat4 + maskMat1) ^ Scalar(5));

        CHECK_DIFF(maskMat5, maskMat5 | (maskMat4 ^ maskMat1));
        CHECK_DIFF(maskMat5, (maskMat4 ^ maskMat1) | maskMat5);
        CHECK_DIFF(maskMat5, maskMat5 | (maskMat4 ^ Scalar(1)));
        CHECK_DIFF(maskMat5, (maskMat4 | maskMat4) | Scalar(1));
        CHECK_DIFF(maskMat5, Scalar(1) | (maskMat4 | maskMat4));
        CHECK_DIFF(maskMat5, Scalar(1) | maskMat4);
        CHECK_DIFF(maskMat5, (maskMat5 | maskMat5) | (maskMat4 ^ maskMat1));

        CHECK_DIFF(maskMat1, min(maskMat1, maskMat5));
        CHECK_DIFF(maskMat1, min(img_t(maskMat1 | maskMat1), maskMat5 | maskMat5));
        CHECK_DIFF(maskMat5, max(maskMat1, maskMat5));
        CHECK_DIFF(maskMat5, max(img_t(maskMat1 | maskMat1), maskMat5 | maskMat5));

        CHECK_DIFF(maskMat1, min(maskMat1, maskMat5 | maskMat5));
        CHECK_DIFF(maskMat1, min(maskMat1 | maskMat1, maskMat5));
        CHECK_DIFF(maskMat5, max(maskMat1 | maskMat1, maskMat5));
        CHECK_DIFF(maskMat5, max(maskMat1, maskMat5 | maskMat5));

        CHECK_DIFF(~maskMat1, maskMat1 ^ -1);
        CHECK_DIFF(~(maskMat1 | maskMat1), maskMat1 ^ -1);

        CHECK_DIFF(maskMat1, maskMat4/4.0);

        /////////////////////////////

        CHECK_DIFF(1.0 - (maskMat5 | maskMat5), -maskMat4);
        CHECK_DIFF((maskMat4 | maskMat4) * 1.0 + 1.0, maskMat5);
        CHECK_DIFF(1.0 + (maskMat4 | maskMat4) * 1.0, maskMat5);
        CHECK_DIFF((maskMat5 | maskMat5) * 1.0 - 1.0, maskMat4);
        CHECK_DIFF(5.0 - (maskMat4 | maskMat4) * 1.0, maskMat1);
        CHECK_DIFF((maskMat4 | maskMat4) * 1.0 + 0.5 + 0.5, maskMat5);
        CHECK_DIFF(0.5 + ((maskMat4 | maskMat4) * 1.0 + 0.5), maskMat5);
        CHECK_DIFF(((maskMat4 | maskMat4) * 1.0 + 2.0) - 1.0, maskMat5);
        CHECK_DIFF(5.0 - ((maskMat1 | maskMat1) * 1.0 + 3.0), maskMat1);
        CHECK_DIFF( ( (maskMat1 | maskMat1) * 2.0 + 2.0) * 1.25, maskMat5);
        CHECK_DIFF( 1.25 * ( (maskMat1 | maskMat1) * 2.0 + 2.0), maskMat5);
        CHECK_DIFF( -( (maskMat1 | maskMat1) * (-2.0) + 1.0), maskMat1);
        CHECK_DIFF( maskMat1 * 1.0 + maskMat4 * 0.5 + 2.0, maskMat5);
        CHECK_DIFF( 1.0 + (maskMat1 * 1.0 + maskMat4 * 0.5 + 1.0), maskMat5);
        CHECK_DIFF( (maskMat1 * 1.0 + maskMat4 * 0.5 + 2.0) - 1.0, maskMat4);
        CHECK_DIFF(5.0 -  (maskMat1 * 1.0 + maskMat4 * 0.5 + 1.0), maskMat1);
        CHECK_DIFF((maskMat1 * 1.0 + maskMat4 * 0.5 + 1.0)*1.25, maskMat5);
        CHECK_DIFF(1.25 * (maskMat1 * 1.0 + maskMat4 * 0.5 + 1.0), maskMat5);
        CHECK_DIFF(-(maskMat1 * 2.0 + maskMat4 * (-1) + 1.0), maskMat1);
        CHECK_DIFF((maskMat1 * 1.0 + maskMat4), maskMat5);
        CHECK_DIFF((maskMat4 + maskMat1 * 1.0), maskMat5);
        CHECK_DIFF((maskMat1 * 3.0 + 1.0) + maskMat1, maskMat5);
        CHECK_DIFF(maskMat1 + (maskMat1 * 3.0 + 1.0), maskMat5);
        CHECK_DIFF(maskMat1*4.0 + (maskMat1 | maskMat1), maskMat5);
        CHECK_DIFF((maskMat1 | maskMat1) + maskMat1*4.0, maskMat5);
        CHECK_DIFF((maskMat1*3.0 + 1.0) + (maskMat1 | maskMat1), maskMat5);
        CHECK_DIFF((maskMat1 | maskMat1) + (maskMat1*3.0 + 1.0), maskMat5);
        CHECK_DIFF(maskMat1*4.0 + maskMat4*2.0, maskMat1 * 12);
        CHECK_DIFF((maskMat1*3.0 + 1.0) + maskMat4*2.0, maskMat1 * 12);
        CHECK_DIFF(maskMat4*2.0 + (maskMat1*3.0 + 1.0), maskMat1 * 12);
        CHECK_DIFF((maskMat1*3.0 + 1.0) + (maskMat1*2.0 + 2.0), maskMat1 * 8);

        CHECK_DIFF(maskMat5*1.0 - maskMat4, maskMat1);
        CHECK_DIFF(maskMat5 - maskMat1 * 4.0, maskMat1);
        CHECK_DIFF((maskMat4 * 1.0 + 4.0)- maskMat4, maskMat4);
        CHECK_DIFF(maskMat5 - (maskMat1 * 2.0 + 2.0), maskMat1);
        CHECK_DIFF(maskMat5*1.0 - (maskMat4 | maskMat4), maskMat1);
        CHECK_DIFF((maskMat5 | maskMat5) - maskMat1 * 4.0, maskMat1);
        CHECK_DIFF((maskMat4 * 1.0 + 4.0)- (maskMat4 | maskMat4), maskMat4);
        CHECK_DIFF((maskMat5 | maskMat5) - (maskMat1 * 2.0 + 2.0), maskMat1);
        CHECK_DIFF(maskMat1*5.0 - maskMat4 * 1.0, maskMat1);
        CHECK_DIFF((maskMat1*5.0 + 3.0)- maskMat4 * 1.0, maskMat4);
        CHECK_DIFF(maskMat4 * 2.0 - (maskMat1*4.0 + 3.0), maskMat1);
        CHECK_DIFF((maskMat1 * 2.0 + 3.0) - (maskMat1*3.0 + 1.0), maskMat1);

        CHECK_DIFF((maskMat5 - maskMat4)* 4.0, maskMat4);
        CHECK_DIFF(4.0 * (maskMat5 - maskMat4), maskMat4);

        CHECK_DIFF(-((maskMat4 | maskMat4) - (maskMat5 | maskMat5)), maskMat1);

        CHECK_DIFF(4.0 * (maskMat1 | maskMat1), maskMat4);
        CHECK_DIFF((maskMat4 | maskMat4)/4.0, maskMat1);

#if !MSVC_OLD
        CHECK_DIFF(2.0 * (maskMat1 * 2.0) , maskMat4);
#endif
        CHECK_DIFF((maskMat4 / 2.0) / 2.0 , maskMat1);
        CHECK_DIFF(-(maskMat4 - maskMat5) , maskMat1);
        CHECK_DIFF(-((maskMat4 - maskMat5) * 1.0), maskMat1);


        /////////////////////////////
        CHECK_DIFF(maskMat4 /  maskMat4, maskMat1);

        ///// Element-wise multiplication

        CHECK_DIFF(maskMat4.mul(maskMat4, 0.25), maskMat4);
        CHECK_DIFF(maskMat4.mul(maskMat1 * 4, 0.25), maskMat4);
        CHECK_DIFF(maskMat4.mul(maskMat4 / 4), maskMat4);
        CHECK_DIFF(maskMat4.mul(maskMat4 / 4), maskMat4);
        CHECK_DIFF(maskMat4.mul(maskMat4) * 0.25, maskMat4);
        CHECK_DIFF(0.25 * maskMat4.mul(maskMat4), maskMat4);

        ////// Element-wise division

        CHECK_DIFF(maskMat4 / maskMat4, maskMat1);
        CHECK_DIFF((maskMat4 & maskMat4) / (maskMat1 * 4), maskMat1);

        CHECK_DIFF((maskMat4 & maskMat4) / maskMat4, maskMat1);
        CHECK_DIFF(maskMat4 / (maskMat4 & maskMat4), maskMat1);
        CHECK_DIFF((maskMat1 * 4) / maskMat4, maskMat1);

        CHECK_DIFF(maskMat4 / (maskMat1 * 4), maskMat1);
        CHECK_DIFF((maskMat4 * 0.5 )/ (maskMat1 * 2), maskMat1);

        CHECK_DIFF(maskMat4 / maskMat4.mul(maskMat1), maskMat1);
        CHECK_DIFF((maskMat4 & maskMat4) / maskMat4.mul(maskMat1), maskMat1);

        CHECK_DIFF(4.0 / maskMat4, maskMat1);
        CHECK_DIFF(4.0 / (maskMat4 | maskMat4), maskMat1);
        CHECK_DIFF(4.0 / (maskMat1 * 4.0), maskMat1);
        CHECK_DIFF(4.0 / (maskMat4 / maskMat1), maskMat1);

        m = maskMat4.clone(); m/=4.0; CHECK_DIFF(m, maskMat1);
        m = maskMat4.clone(); m/=maskMat4; CHECK_DIFF(m, maskMat1);
        m = maskMat4.clone(); m/=(maskMat1 * 4.0); CHECK_DIFF(m, maskMat1);
        m = maskMat4.clone(); m/=(maskMat4 / maskMat1); CHECK_DIFF(m, maskMat1);

        /////////////////////////////
        float matrix_data[] = { 3, 1, -4, -5, 1, 0, 0, 1.1f, 1.5f};
        img_t mt(3, 3, CC_32F, matrix_data);
        img_t mi = mt.inv();
        img_t d1 = img_t::eye(3, 3, CC_32F);
        img_t d2 = d1 * 2;
        MatExpr mt_tr = mt.t();
        MatExpr mi_tr = mi.t();
        img_t mi2 = mi * 2;


        CHECK_DIFF_FLT( mi2 * mt, d2 );
        CHECK_DIFF_FLT( mi * mt, d1 );
        CHECK_DIFF_FLT( mt_tr * mi_tr, d1 );

        m = mi.clone(); m*=mt;  CHECK_DIFF_FLT(m, d1);
        m = mi.clone(); m*= (2 * mt - mt) ;  CHECK_DIFF_FLT(m, d1);

        m = maskMat4.clone(); m+=(maskMat1 * 1.0); CHECK_DIFF(m, maskMat5);
        m = maskMat5.clone(); m-=(maskMat1 * 4.0); CHECK_DIFF(m, maskMat1);

        m = maskMat1.clone(); m+=(maskMat1 * 3.0 + 1.0); CHECK_DIFF(m, maskMat5);
        m = maskMat5.clone(); m-=(maskMat1 * 3.0 + 1.0); CHECK_DIFF(m, maskMat1);
#if !MSVC_OLD
        m = mi.clone(); m+=(3.0 * mi * mt + d1); CHECK_DIFF_FLT(m, mi + d1 * 4);
        m = mi.clone(); m-=(3.0 * mi * mt + d1); CHECK_DIFF_FLT(m, mi - d1 * 4);
        m = mi.clone(); m*=(mt * 1.0); CHECK_DIFF_FLT(m, d1);
        m = mi.clone(); m*=(mt * 1.0 + img_t::eye(m.size(), m CC_MAT_TYPE())); CHECK_DIFF_FLT(m, d1 + mi);
        m = mi.clone(); m*=mt_tr.t(); CHECK_DIFF_FLT(m, d1);

        CHECK_DIFF_FLT( (mi * 2) * mt, d2);
        CHECK_DIFF_FLT( mi * (2 * mt), d2);
        CHECK_DIFF_FLT( mt.t() * mi_tr, d1 );
        CHECK_DIFF_FLT( mt_tr * mi.t(), d1 );
        CHECK_DIFF_FLT( (mi * 0.4) * (mt * 5), d2);

        CHECK_DIFF_FLT( mt.t() * (mi_tr * 2), d2 );
        CHECK_DIFF_FLT( (mt_tr * 2) * mi.t(), d2 );

        CHECK_DIFF_FLT(mt.t() * mi.t(), d1);
        CHECK_DIFF_FLT( (mi * mt) * 2.0, d2);
        CHECK_DIFF_FLT( 2.0 * (mi * mt), d2);
        CHECK_DIFF_FLT( -(mi * mt), -d1);

        CHECK_DIFF_FLT( (mi * mt) / 2.0, d1 / 2);

        img_t mt_mul_2_plus_1;
        gemm(mt, d1, 2, img_t::ones(3, 3, CC_32F), 1, mt_mul_2_plus_1);

        CHECK_DIFF( (mt * 2.0 + 1.0) * mi, mt_mul_2_plus_1 * mi);        // (A*alpha + beta)*B
        CHECK_DIFF( mi * (mt * 2.0 + 1.0), mi * mt_mul_2_plus_1);        // A*(B*alpha + beta)
        CHECK_DIFF( (mt * 2.0 + 1.0) * (mi * 2), mt_mul_2_plus_1 * mi2); // (A*alpha + beta)*(B*gamma)
        CHECK_DIFF( (mi *2)* (mt * 2.0 + 1.0), mi2 * mt_mul_2_plus_1);   // (A*gamma)*(B*alpha + beta)
        CHECK_DIFF_FLT( (mt * 2.0 + 1.0) * mi.t(), mt_mul_2_plus_1 * mi_tr); // (A*alpha + beta)*B^t
        CHECK_DIFF_FLT( mi.t() * (mt * 2.0 + 1.0), mi_tr * mt_mul_2_plus_1); // A^t*(B*alpha + beta)

        CHECK_DIFF_FLT( (mi * mt + d2)*5, d1 * 3 * 5);
        CHECK_DIFF_FLT( mi * mt + d2, d1 * 3);
        CHECK_DIFF_FLT( -(mi * mt) + d2, d1);
        CHECK_DIFF_FLT( (mi * mt) + d1, d2);
        CHECK_DIFF_FLT( d1 + (mi * mt), d2);
        CHECK_DIFF_FLT( (mi * mt) - d2, -d1);
        CHECK_DIFF_FLT( d2 - (mi * mt), d1);

        CHECK_DIFF_FLT( (mi * mt) + d2 * 0.5, d2);
        CHECK_DIFF_FLT( d2 * 0.5 + (mi * mt), d2);
        CHECK_DIFF_FLT( (mi * mt) - d1 * 2, -d1);
        CHECK_DIFF_FLT( d1 * 2 - (mi * mt), d1);

        CHECK_DIFF_FLT( (mi * mt) + mi.t(), mi_tr + d1);
        CHECK_DIFF_FLT( mi.t() + (mi * mt), mi_tr + d1);
        CHECK_DIFF_FLT( (mi * mt) - mi.t(), d1 - mi_tr);
        CHECK_DIFF_FLT( mi.t() - (mi * mt), mi_tr - d1);

        CHECK_DIFF_FLT( 2.0 *(mi * mt + d2), d1 * 6);
        CHECK_DIFF_FLT( -(mi * mt + d2), d1 * -3);

        CHECK_DIFF_FLT(mt.inv() * mt, d1);

        CHECK_DIFF_FLT(mt.inv() * (2*mt - mt), d1);
#endif
    }
    catch (const test_excep& e)
    {
        ts->printf(cvtest::TS::LOG, "%s\n", e.s.c_str());
        ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        return false;
    }
    return true;
}

bool CC_OperationsTest::SomeMatFunctions()
{
    try
    {
        img_t rgba( 10, 10, CC_8UC4, Scalar(1,2,3,4) );
        img_t bgr( rgba->rows, rgba->cols, CC_8UC3 );
        img_t alpha( rgba->rows, rgba->cols, CC_8UC1 );
        img_t out[] = { bgr, alpha };
        // rgba[0] -> bgr[2], rgba[1] -> bgr[1],
        // rgba[2] -> bgr[0], rgba[3] -> alpha[0]
        int from_to[] = { 0,2, 1,1, 2,0, 3,3 };
        mixChannels( &rgba, 1, out, 2, from_to, 4 );

        img_t bgr_exp( rgba.size(), CC_8UC3, Scalar(3,2,1));
        img_t alpha_exp( rgba.size(), CC_8UC1, Scalar(4));

        CHECK_DIFF(bgr_exp, bgr);
        CHECK_DIFF(alpha_exp, alpha);
    }
    catch (const test_excep& e)
    {
        ts->printf(cvtest::TS::LOG, "%s\n", e.s.c_str());
        ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        return false;
    }
    return true;

}


bool CC_OperationsTest::TestSubMatAccess()
{
    try
    {
        Mat_<float> T_bs(4,4);
        Vec3f cdir(1.f, 1.f, 0.f);
        Vec3f ydir(1.f, 0.f, 1.f);
        Vec3f fpt(0.1f, 0.7f, 0.2f);
        T_bs.setTo(0);
        T_bs(CRange(0,3),CRange(2,3)) = 1.0*img_t(cdir); // wierd OpenCV stuff, need to do multiply
        T_bs(CRange(0,3),CRange(1,2)) = 1.0*img_t(ydir);
        T_bs(CRange(0,3),CRange(0,1)) = 1.0*img_t(cdir.cross(ydir));
        T_bs(CRange(0,3),CRange(3,4)) = 1.0*img_t(fpt);
        T_bs(3,3) = 1.0;
        //std::cout << "[Nav Grok] S frame =" << std::endl << T_bs << std::endl;

        // set up display coords, really just the S frame
        std::vector<float>coords;

        for (int i=0; i<16; i++)
        {
            coords.push_back(T_bs(i));
            //std::cout << T_bs1(i) << std::endl;
        }
        CC_Assert( cvtest::norm(coords, T_bs cvReshape(,1,1), NORM_INF) == 0 );
    }
    catch (const test_excep& e)
    {
        ts->printf(cvtest::TS::LOG, "%s\n", e.s.c_str());
        ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        return false;
    }
    return true;
}

bool CC_OperationsTest::TestTemplateMat()
{
    try
    {
        Mat_<float> one_3x1(3, 1, 1.0f);
        Mat_<float> shi_3x1(3, 1, 1.2f);
        Mat_<float> shi_2x1(2, 1, -2);
        Scalar shift = Scalar::all(15);

        float data[] = { sqrt(2.f)/2, -sqrt(2.f)/2, 1.f, sqrt(2.f)/2, sqrt(2.f)/2, 10.f };
        Mat_<float> rot_2x3(2, 3, data);

        Mat_<float> res = img_t(img_t(2 * rot_2x3) * img_t(one_3x1 + shi_3x1 + shi_3x1 + shi_3x1) - shi_2x1) + shift;
        Mat_<float> resS = rot_2x3 * one_3x1;

        Mat_<float> tmp, res2, resS2;
        add(one_3x1, shi_3x1, tmp);
        add(tmp, shi_3x1, tmp);
        add(tmp, shi_3x1, tmp);
        gemm(rot_2x3, tmp, 2, shi_2x1, -1, res2, 0);
        add(res2, img_t(2, 1, CC_32F, shift), res2);

        gemm(rot_2x3, one_3x1, 1, shi_2x1, 0, resS2, 0);
        CHECK_DIFF(res, res2);
        CHECK_DIFF(resS, resS2);


        Mat_<float> mat4x4(4, 4);
        randu(mat4x4, Scalar(0), Scalar(10));

        Mat_<float> roi1 = mat4x4(Rect(Point(1, 1), Size(2, 2)));
        Mat_<float> roi2 = mat4x4(CRange(1, 3), CRange(1, 3));

        CHECK_DIFF(roi1, roi2);
        CHECK_DIFF(mat4x4, mat4x4(Rect(Point(0,0), mat4x4.size())));

        Mat_<int> intMat10(3, 3, 10);
        Mat_<int> intMat11(3, 3, 11);
        Mat_<uchar> resMat(3, 3, 255);

        CHECK_DIFF(resMat, intMat10 == intMat10);
        CHECK_DIFF(resMat, intMat10 <  intMat11);
        CHECK_DIFF(resMat, intMat11 >  intMat10);
        CHECK_DIFF(resMat, intMat10 <= intMat11);
        CHECK_DIFF(resMat, intMat11 >= intMat10);

        CHECK_DIFF(resMat, intMat10 == 10.0);
        CHECK_DIFF(resMat, intMat10 <  11.0);
        CHECK_DIFF(resMat, intMat11 >  10.0);
        CHECK_DIFF(resMat, intMat10 <= 11.0);
        CHECK_DIFF(resMat, intMat11 >= 10.0);

        Mat_<uchar> maskMat4(3, 3, 4);
        Mat_<uchar> maskMat1(3, 3, 1);
        Mat_<uchar> maskMat5(3, 3, 5);
        Mat_<uchar> maskMat0(3, 3, (uchar)0);

        CHECK_DIFF(maskMat0, maskMat4 & maskMat1);
        CHECK_DIFF(maskMat0, Scalar(1) & maskMat4);
        CHECK_DIFF(maskMat0, maskMat4 & Scalar(1));

        Mat_<uchar> m;
        m = maskMat4.clone(); m&=maskMat1; CHECK_DIFF(maskMat0, m);
        m = maskMat4.clone(); m&=Scalar(1); CHECK_DIFF(maskMat0, m);

        m = maskMat4.clone(); m|=maskMat1; CHECK_DIFF(maskMat5, m);
        m = maskMat4.clone(); m^=maskMat1; CHECK_DIFF(maskMat5, m);

        CHECK_DIFF(maskMat0, (maskMat4 | maskMat4) & (maskMat1 | maskMat1));
        CHECK_DIFF(maskMat0, (maskMat4 | maskMat4) & maskMat1);
        CHECK_DIFF(maskMat0, maskMat4 & (maskMat1 | maskMat1));

        CHECK_DIFF(maskMat0, maskMat5 ^ (maskMat4 | maskMat1));
        CHECK_DIFF(maskMat0, Scalar(5) ^ (maskMat4 | Scalar(1)));

        CHECK_DIFF(maskMat5, maskMat5 | (maskMat4 ^ maskMat1));
        CHECK_DIFF(maskMat5, maskMat5 | (maskMat4 ^ Scalar(1)));

        CHECK_DIFF(~maskMat1, maskMat1 ^ 0xFF);
        CHECK_DIFF(~(maskMat1 | maskMat1), maskMat1 ^ 0xFF);

        CHECK_DIFF(maskMat1 + maskMat4, maskMat5);
        CHECK_DIFF(maskMat1 + Scalar(4), maskMat5);
        CHECK_DIFF(Scalar(4) + maskMat1, maskMat5);
        CHECK_DIFF(Scalar(4) + (maskMat1 & maskMat1), maskMat5);

        CHECK_DIFF(maskMat1 + 4.0, maskMat5);
        CHECK_DIFF((maskMat1 & 0xFF) + 4.0, maskMat5);
        CHECK_DIFF(4.0 + maskMat1, maskMat5);

        m = maskMat4.clone(); m+=Scalar(1); CHECK_DIFF(m, maskMat5);
        m = maskMat4.clone(); m+=maskMat1; CHECK_DIFF(m, maskMat5);
        m = maskMat4.clone(); m+=(maskMat1 | maskMat1); CHECK_DIFF(m, maskMat5);

        CHECK_DIFF(maskMat5 - maskMat1, maskMat4);
        CHECK_DIFF(maskMat5 - Scalar(1), maskMat4);
        CHECK_DIFF((maskMat5 | maskMat5) - Scalar(1), maskMat4);
        CHECK_DIFF(maskMat5 - 1, maskMat4);
        CHECK_DIFF((maskMat5 | maskMat5) - 1, maskMat4);
        CHECK_DIFF((maskMat5 | maskMat5) - (maskMat1 | maskMat1), maskMat4);

        CHECK_DIFF(maskMat1, min(maskMat1, maskMat5));
        CHECK_DIFF(maskMat5, max(maskMat1, maskMat5));

        m = maskMat5.clone(); m-=Scalar(1); CHECK_DIFF(m, maskMat4);
        m = maskMat5.clone(); m-=maskMat1; CHECK_DIFF(m, maskMat4);
        m = maskMat5.clone(); m-=(maskMat1 | maskMat1); CHECK_DIFF(m, maskMat4);

        m = maskMat4.clone(); m |= Scalar(1); CHECK_DIFF(maskMat5, m);
        m = maskMat5.clone(); m ^= Scalar(1); CHECK_DIFF(maskMat4, m);

        CHECK_DIFF(maskMat1, maskMat4/4.0);

        Mat_<float> negf(3, 3, -3.0);
        Mat_<float> posf = -negf;
        Mat_<float> posf2 = posf * 2;
        Mat_<int> negi(3, 3, -3);

        CHECK_DIFF(abs(negf), -negf);
        CHECK_DIFF(abs(posf - posf2), -negf);
        CHECK_DIFF(abs(negi), -(negi & negi));

        CHECK_DIFF(5.0 - maskMat4, maskMat1);


        CHECK_DIFF(maskMat4.mul(maskMat4, 0.25), maskMat4);
        CHECK_DIFF(maskMat4.mul(maskMat1 * 4, 0.25), maskMat4);
        CHECK_DIFF(maskMat4.mul(maskMat4 / 4), maskMat4);


        ////// Element-wise division

        CHECK_DIFF(maskMat4 / maskMat4, maskMat1);
        CHECK_DIFF(4.0 / maskMat4, maskMat1);
        m = maskMat4.clone(); m/=4.0; CHECK_DIFF(m, maskMat1);

        ////////////////////////////////

        typedef Mat_<int> TestMat_t;

        const TestMat_t cnegi = negi.clone();

        TestMat_t::iterator beg = negi.begin();
        TestMat_t::iterator end = negi.end();

        TestMat_t::const_iterator cbeg = cnegi.begin();
        TestMat_t::const_iterator cend = cnegi.end();

        int sum = 0;
        for(; beg!=end; ++beg)
            sum+=*beg;

        for(; cbeg!=cend; ++cbeg)
            sum-=*cbeg;

        if (sum != 0) throw test_excep();

        CHECK_DIFF(negi.col(1), negi.col(2));
        CHECK_DIFF(negi.row(1), negi.row(2));
        CHECK_DIFF(negi.col(1), negi.diag());

        if (Mat_<CPoint2f>(1, 1) CC_ELEM_SIZE1() != sizeof(float)) throw test_excep();
        if (Mat_<CPoint2f>(1, 1) CC_ELEM_SIZE() != 2 * sizeof(float)) throw test_excep();
        if (Mat_<CPoint2f>(1, 1) CC_MAT_DEPTH() != CC_32F) throw test_excep();
        if (Mat_<float>(1, 1) CC_MAT_DEPTH() != CC_32F) throw test_excep();
        if (Mat_<int>(1, 1) CC_MAT_DEPTH() != CC_32S) throw test_excep();
        if (Mat_<double>(1, 1) CC_MAT_DEPTH() != CC_64F) throw test_excep();
        if (Mat_<CPoint3d>(1, 1) CC_MAT_DEPTH() != CC_64F) throw test_excep();
        if (Mat_<signed char>(1, 1) CC_MAT_DEPTH() != CC_8S) throw test_excep();
        if (Mat_<unsigned short>(1, 1) CC_MAT_DEPTH() != CC_16U) throw test_excep();
        if (Mat_<unsigned short>(1, 1) CC_MAT_CN() != 1) throw test_excep();
        if (Mat_<CPoint2f>(1, 1) CC_MAT_CN() != 2) throw test_excep();
        if (Mat_<CPoint3f>(1, 1) CC_MAT_CN() != 3) throw test_excep();
        if (Mat_<CPoint3d>(1, 1) CC_MAT_CN() != 3) throw test_excep();

        Mat_<uchar> eye = Mat_<uchar>::zeros(2, 2); CHECK_DIFF(Mat_<uchar>::zeros(Size(2, 2)), eye);
        eye.at<uchar>(Point(0,0)) = 1; eye.at<uchar>(1, 1) = 1;

        CHECK_DIFF(Mat_<uchar>::eye(2, 2), eye);
        CHECK_DIFF(eye, Mat_<uchar>::eye(Size(2,2)));

        Mat_<uchar> ones(2, 2, (uchar)1);
        CHECK_DIFF(ones, Mat_<uchar>::ones(Size(2,2)));
        CHECK_DIFF(Mat_<uchar>::ones(2, 2), ones);

        Mat_<CPoint2f> pntMat(2, 2, CPoint2f(1, 0));
        if(pntMat->stepT() != 2) throw test_excep();

        uchar uchar_data[] = {1, 0, 0, 1};

        Mat_<uchar> matFromData(1, 4, uchar_data);
        const Mat_<uchar> mat2 = matFromData.clone();
        CHECK_DIFF(matFromData, eye cvReshape(,1, 1));
        if (matFromData(Point(0,0)) != uchar_data[0])throw test_excep();
        if (mat2(Point(0,0)) != uchar_data[0]) throw test_excep();

        if (matFromData(0,0) != uchar_data[0])throw test_excep();
        if (mat2(0,0) != uchar_data[0]) throw test_excep();

        Mat_<uchar> rect(eye, Rect(0, 0, 1, 1));
        if (rect->cols != 1 || rect->rows != 1 || rect(0,0) != uchar_data[0]) throw test_excep();

        //Mat_<_Tp>::adjustROI(int,int,int,int)
        //Mat_<_Tp>::cross(const Mat_&) const
        //Mat_<_Tp>::Mat_(const vector<_Tp>&,bool)
        //Mat_<_Tp>::Mat_(int,int,_Tp*,size_t)
        //Mat_<_Tp>::Mat_(int,int,const _Tp&)
        //Mat_<_Tp>::Mat_(Size,const _Tp&)
        //Mat_<_Tp>::mul(const Mat_<_Tp>&,double) const
        //Mat_<_Tp>::mul(const MatExpr_<MatExpr_Op2_<Mat_<_Tp>,double,Mat_<_Tp>,MatOp_DivRS_<img_t> >,Mat_<_Tp> >&,double) const
        //Mat_<_Tp>::mul(const MatExpr_<MatExpr_Op2_<Mat_<_Tp>,double,Mat_<_Tp>,MatOp_Scale_<img_t> >,Mat_<_Tp> >&,double) const
        //Mat_<_Tp>::operator Mat_<T2>() const
        //Mat_<_Tp>::operator MatExpr_<Mat_<_Tp>,Mat_<_Tp> >() const
        //Mat_<_Tp>::operator()(const CRange&,const CRange&) const
        //Mat_<_Tp>::operator()(const Rect&) const

        //Mat_<_Tp>::operator=(const MatExpr_Base&)
        //Mat_<_Tp>::operator[](int) const


        ///////////////////////////////

        float matrix_data[] = { 3, 1, -4, -5, 1, 0, 0, 1.1f, 1.5f};
        Mat_<float> mt(3, 3, matrix_data);
        Mat_<float> mi = mt.inv();
        Mat_<float> d1 = Mat_<float>::eye(3, 3);
        Mat_<float> d2 = d1 * 2;
        Mat_<float> mt_tr = mt.t();
        Mat_<float> mi_tr = mi.t();
        Mat_<float> mi2 = mi * 2;

        CHECK_DIFF_FLT( mi2 * mt, d2 );
        CHECK_DIFF_FLT( mi * mt, d1 );
        CHECK_DIFF_FLT( mt_tr * mi_tr, d1 );

        Mat_<float> mf;
        mf = mi.clone(); mf*=mt;  CHECK_DIFF_FLT(mf, d1);

        ////// typedefs //////

        if (Mat1b(1, 1) CC_ELEM_SIZE() != sizeof(uchar)) throw test_excep();
        if (Mat2b(1, 1) CC_ELEM_SIZE() != 2 * sizeof(uchar)) throw test_excep();
        if (Mat3b(1, 1) CC_ELEM_SIZE() != 3 * sizeof(uchar)) throw test_excep();
        if (Mat1f(1, 1) CC_ELEM_SIZE() != sizeof(float)) throw test_excep();
        if (Mat2f(1, 1) CC_ELEM_SIZE() != 2 * sizeof(float)) throw test_excep();
        if (Mat3f(1, 1) CC_ELEM_SIZE() != 3 * sizeof(float)) throw test_excep();
        if (Mat1f(1, 1) CC_MAT_DEPTH() != CC_32F) throw test_excep();
        if (Mat3f(1, 1) CC_MAT_DEPTH() != CC_32F) throw test_excep();
        if (Mat3f(1, 1) CC_MAT_TYPE() != CC_32FC3) throw test_excep();
        if (Mat1i(1, 1) CC_MAT_DEPTH() != CC_32S) throw test_excep();
        if (Mat1d(1, 1) CC_MAT_DEPTH() != CC_64F) throw test_excep();
        if (Mat1b(1, 1) CC_MAT_DEPTH() != CC_8U) throw test_excep();
        if (Mat3b(1, 1) CC_MAT_TYPE() != CC_8UC3) throw test_excep();
        if (Mat1w(1, 1) CC_MAT_DEPTH() != CC_16U) throw test_excep();
        if (Mat1s(1, 1) CC_MAT_DEPTH() != CC_16S) throw test_excep();
        if (Mat1f(1, 1) CC_MAT_CN() != 1) throw test_excep();
        if (Mat1b(1, 1) CC_MAT_CN() != 1) throw test_excep();
        if (Mat1i(1, 1) CC_MAT_CN() != 1) throw test_excep();
        if (Mat1w(1, 1) CC_MAT_CN() != 1) throw test_excep();
        if (Mat1s(1, 1) CC_MAT_CN() != 1) throw test_excep();
        if (Mat2f(1, 1) CC_MAT_CN() != 2) throw test_excep();
        if (Mat2b(1, 1) CC_MAT_CN() != 2) throw test_excep();
        if (Mat2i(1, 1) CC_MAT_CN() != 2) throw test_excep();
        if (Mat2w(1, 1) CC_MAT_CN() != 2) throw test_excep();
        if (Mat2s(1, 1) CC_MAT_CN() != 2) throw test_excep();
        if (Mat3f(1, 1) CC_MAT_CN() != 3) throw test_excep();
        if (Mat3b(1, 1) CC_MAT_CN() != 3) throw test_excep();
        if (Mat3i(1, 1) CC_MAT_CN() != 3) throw test_excep();
        if (Mat3w(1, 1) CC_MAT_CN() != 3) throw test_excep();
        if (Mat3s(1, 1) CC_MAT_CN() != 3) throw test_excep();

        vector<Mat_<float> > mvf, mvf2;
        Mat_<Vec2f> mf2;
        mvf.push_back(Mat_<float>::ones(4, 3));
        mvf.push_back(Mat_<float>::zeros(4, 3));
        merge(mvf, mf2);
        split(mf2, mvf2);
        CC_Assert( cvtest::norm(mvf2[0], mvf[0], CC_C) == 0 &&
                  cvtest::norm(mvf2[1], mvf[1], CC_C) == 0 );

        {
        img_t a(2,2,CC_32F,1.f);
        img_t b(1,2,CC_32F,1.f);
        img_t c = (a*b.t()).t();
        CC_Assert( cvtest::norm(c, CC_L1) == 4. );
        }

        bool badarg_catched = false;
        try
        {
            img_t m1 = img_t::zeros(1, 10, CC_8UC1);
            img_t m2 = img_t::zeros(10, 10, CC_8UC3);
            m1.copyTo(m2.row(1));
        }
        catch(const Exception&)
        {
            badarg_catched = true;
        }
        CC_Assert( badarg_catched );

        Size size(2, 5);
        TestType<float>(size, 1.f);
        Vec3f val1 = 1.f;
        TestType<Vec3f>(size, val1);
        Matx31f val2 = 1.f;
        TestType<Matx31f>(size, val2);
        Matx41f val3 = 1.f;
        TestType<Matx41f>(size, val3);
        Matx32f val4 = 1.f;
        TestType<Matx32f>(size, val4);
    }
    catch (const test_excep& e)
    {
        ts->printf(cvtest::TS::LOG, "%s\n", e.s.c_str());
        ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        return false;
    }
    return true;
}

bool CC_OperationsTest::TestMatND()
{
    int sizes[] = { 3, 3, 3};
    MatND nd(3, sizes, CC_32F);

    return true;
}

bool CC_OperationsTest::TestSparseMat()
{
    try
    {
        int sizes[] = { 10, 10, 10};
        int dims = sizeof(sizes)/sizeof(sizes[0]);
        SparseMat mat(dims, sizes, CC_32FC2);

        if (mat.dims() != dims) throw test_excep();
        if (mat CC_MAT_CN() != 2) throw test_excep();
        if (mat CC_MAT_DEPTH() != CC_32F) throw test_excep();

        SparseMat mat2 = mat.clone();
    }
    catch (const test_excep&)
    {
        ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        return false;
    }
    return true;
}


bool CC_OperationsTest::TestMatxMultiplication()
{
    try
    {
        Matx33f mat(1, 1, 1, 0, 1, 1, 0, 0, 1); // Identity matrix
        CPoint2f pt(3, 4);
        CPoint3f res = mat * pt; // Correctly assumes homogeneous coordinates

        Vec3f res2 = mat*Vec3f(res.x, res.y, res.z);

        if(res.x != 8.0) throw test_excep();
        if(res.y != 5.0) throw test_excep();
        if(res.z != 1.0) throw test_excep();

        if(res2[0] != 14.0) throw test_excep();
        if(res2[1] != 6.0) throw test_excep();
        if(res2[2] != 1.0) throw test_excep();

        Matx44f mat44f(1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1);
        Matx44d mat44d(1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1);
        Scalar s(4, 3, 2, 1);
        Scalar sf = mat44f*s;
        Scalar sd = mat44d*s;

        if(sf[0] != 10.0) throw test_excep();
        if(sf[1] != 6.0) throw test_excep();
        if(sf[2] != 3.0) throw test_excep();
        if(sf[3] != 1.0) throw test_excep();

        if(sd[0] != 10.0) throw test_excep();
        if(sd[1] != 6.0) throw test_excep();
        if(sd[2] != 3.0) throw test_excep();
        if(sd[3] != 1.0) throw test_excep();
    }
    catch(const test_excep&)
    {
        ts->set_failed_test_info(cvtest::TS::FAIL_INVALID_OUTPUT);
        return false;
    }
    return true;
}

bool CC_OperationsTest::TestMatxElementwiseDivison()
{
    try
    {
        Matx22f mat(2, 4, 6, 8);
        Matx22f mat2(2, 2, 2, 2);

        Matx22f res = mat.div(mat2);

        if(res(0, 0) != 1.0) throw test_excep();
        if(res(0, 1) != 2.0) throw test_excep();
        if(res(1, 0) != 3.0) throw test_excep();
        if(res(1, 1) != 4.0) throw test_excep();
    }
    catch(const test_excep&)
    {
        ts->set_failed_test_info(cvtest::TS::FAIL_INVALID_OUTPUT);
        return false;
    }
    return true;
}


bool CC_OperationsTest::TestVec()
{
    try
    {
        img_t hsvImage_f(5, 5, CC_32FC3), hsvImage_b(5, 5, CC_8UC3);
        int i = 0,j = 0;
        Vec3f a;

        //these compile
        Vec3b b = a;
        hsvImage_f.at<Vec3f>(i,j) = Vec3f((float)i,0,1);
        hsvImage_b.at<Vec3b>(i,j) = Vec3b(Vec3f((float)i,0,1));

        //these don't
        b = Vec3f(1,0,0);
        Vec3b c;
        c = Vec3f(0,0,1);
        hsvImage_b.at<Vec3b>(i,j) = Vec3f((float)i,0,1);
        hsvImage_b.at<Vec3b>(i,j) = a;
        hsvImage_b.at<Vec3b>(i,j) = Vec3f(1,2,3);
    }
    catch(const test_excep&)
    {
        ts->set_failed_test_info(cvtest::TS::FAIL_INVALID_OUTPUT);
        return false;
    }
    return true;
}

bool CC_OperationsTest::operations1()
{
    try
    {
        CPoint3d p1(1, 1, 1), p2(2, 2, 2), p4(4, 4, 4);
        p1*=2;
        if (!(p1     == p2)) throw test_excep();
        if (!(p2 * 2 == p4)) throw test_excep();
        if (!(p2 * 2.f == p4)) throw test_excep();
        if (!(p2 * 2.f == p4)) throw test_excep();

        CPoint2d pi1(1, 1), pi2(2, 2), pi4(4, 4);
        pi1*=2;
        if (!(pi1     == pi2)) throw test_excep();
        if (!(pi2 * 2 == pi4)) throw test_excep();
        if (!(pi2 * 2.f == pi4)) throw test_excep();
        if (!(pi2 * 2.f == pi4)) throw test_excep();

        Vec2d v12(1, 1), v22(2, 2);
        v12*=2.0;
        if (!(v12 == v22)) throw test_excep();

        Vec3d v13(1, 1, 1), v23(2, 2, 2);
        v13*=2.0;
        if (!(v13 == v23)) throw test_excep();

        Vec4d v14(1, 1, 1, 1), v24(2, 2, 2, 2);
        v14*=2.0;
        if (!(v14 == v24)) throw test_excep();

        Size sz(10, 20);
        if (sz.area() != 200) throw test_excep();
        if (sz.width != 10 || sz.height != 20) throw test_excep();
        if (((CSize)sz).width != 10 || ((CSize)sz).height != 20) throw test_excep();

        Vec<double, 5> v5d(1, 1, 1, 1, 1);
        Vec<double, 6> v6d(1, 1, 1, 1, 1, 1);
        Vec<double, 7> v7d(1, 1, 1, 1, 1, 1, 1);
        Vec<double, 8> v8d(1, 1, 1, 1, 1, 1, 1, 1);
        Vec<double, 9> v9d(1, 1, 1, 1, 1, 1, 1, 1, 1);
        Vec<double,10> v10d(1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

        Vec<double,10> v10dzero;
        for (int ii = 0; ii < 10; ++ii) {
            if (v10dzero[ii] != 0.0)
                throw test_excep();
        }

        img_t A(1, 32, CC_32F), B;
        for( int i = 0; i < A->cols; i++ )
            A.at<float>(i) = (float)(i <= 12 ? i : 24 - i);
        transpose(A, B);

        int minidx[2] = {0, 0}, maxidx[2] = {0, 0};
        double minval = 0, maxval = 0;
        minMaxIdx(A, &minval, &maxval, minidx, maxidx);

        if( !(minidx[0] == 0 && minidx[1] == 31 && maxidx[0] == 0 && maxidx[1] == 12 &&
                  minval == -7 && maxval == 12))
            throw test_excep();

        minMaxIdx(B, &minval, &maxval, minidx, maxidx);

        if( !(minidx[0] == 31 && minidx[1] == 0 && maxidx[0] == 12 && maxidx[1] == 0 &&
              minval == -7 && maxval == 12))
            throw test_excep();

        Matx33f b(1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f);
        img_t c;
        add(img_t::zeros(3, 3, CC_32F), b, c);
        CC_Assert( cvtest::norm(b, c, CC_C) == 0 );

        add(img_t::zeros(3, 3, CC_64F), b, c, NULL, c CC_MAT_TYPE());
        CC_Assert( cvtest::norm(b, c, CC_C) == 0 );

        add(img_t::zeros(6, 1, CC_64F), 1, c, NULL, c CC_MAT_TYPE());
        CC_Assert( cvtest::norm(Matx61f(1.f, 1.f, 1.f, 1.f, 1.f, 1.f), c, CC_C) == 0 );

        vector<CPoint2f> pt2d(3);
        vector<CPoint3d> pt3d(2);

        CC_Assert( img_t(pt2d) cvCheckVector(2) == 3 && img_t(pt2d) cvCheckVector(3) < 0 &&
                   img_t(pt3d) cvCheckVector(2) < 0 && img_t(pt3d) cvCheckVector(3) == 2 );

        Matx44f m44(0.8147f, 0.6324f, 0.9575f, 0.9572f,
                0.9058f, 0.0975f, 0.9649f, 0.4854f,
                0.1270f, 0.2785f, 0.1576f, 0.8003f,
                0.9134f, 0.5469f, 0.9706f, 0.1419f);
        double d = determinant(m44);
        CC_Assert( fabs(d - (-0.0262)) <= 0.001 );

        suf32_t z;
        z.i = 0x80000000;
        CC_Assert( cFloor(z.f) == 0 && cCeil(z.f) == 0 && cRound(z.f) == 0 );
    }
    catch(const test_excep&)
    {
        ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        return false;
    }
    return true;
}


bool CC_OperationsTest::TestExp()
{
    Mat1f tt = Mat1f::ones(4,2);
    Mat1f outs;
    exp(-tt, outs);
    Mat1f tt2 = Mat1f::ones(4,1), outs2;
    exp(-tt2, outs2);
    return true;
}


bool CC_OperationsTest::TestSVD()
{
    try
    {
        img_t A = (Mat_<double>(3,4) << 1, 2, -1, 4, 2, 4, 3, 5, -1, -2, 6, 7);
        img_t x;
        CSVD::solveZ(A,x);
        if( cvtest::norm(A*x, CC_C) > FLT_EPSILON )
            throw test_excep();

        CSVD svd(A, CSVD::FULL_UV);
        if( cvtest::norm(A*svd.vt.row(3).t(), CC_C) > FLT_EPSILON )
            throw test_excep();

        img_t Dp(3,3,CC_32FC1);
        img_t Dc(3,3,CC_32FC1);
        img_t Q(3,3,CC_32FC1);
        img_t U,Vt,R,T,W;

        Dp.at<float>(0,0)=0.86483884f; Dp.at<float>(0,1)= -0.3077251f; Dp.at<float>(0,2)=-0.55711365f;
        Dp.at<float>(1,0)=0.49294353f; Dp.at<float>(1,1)=-0.24209651f; Dp.at<float>(1,2)=-0.25084701f;
        Dp.at<float>(2,0)=0;           Dp.at<float>(2,1)=0;            Dp.at<float>(2,2)=0;

        Dc.at<float>(0,0)=0.75632739f; Dc.at<float>(0,1)= -0.38859656f; Dc.at<float>(0,2)=-0.36773083f;
        Dc.at<float>(1,0)=0.9699229f;  Dc.at<float>(1,1)=-0.49858192f;  Dc.at<float>(1,2)=-0.47134098f;
        Dc.at<float>(2,0)=0.10566688f; Dc.at<float>(2,1)=-0.060333252f; Dc.at<float>(2,2)=-0.045333147f;

        Q=Dp*Dc.t();
        CSVD decomp;
        decomp=CSVD(Q);
        U=decomp.u;
        Vt=decomp.vt;
        W=decomp.w;
        img_t I = img_t::eye(3, 3, CC_32F);

        if( cvtest::norm(U*U.t(), I, CC_C) > FLT_EPSILON ||
            cvtest::norm(Vt*Vt.t(), I, CC_C) > FLT_EPSILON ||
            W.at<float>(2) < 0 || W.at<float>(1) < W.at<float>(2) ||
            W.at<float>(0) < W.at<float>(1) ||
            cvtest::norm(U*img_t::diag(W)*Vt, Q, CC_C) > FLT_EPSILON )
            throw test_excep();
    }
    catch(const test_excep&)
    {
        ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        return false;
    }
    return true;
}

void CC_OperationsTest::run( int /* start_from */)
{
    if (!TestMat())
        return;

    if (!SomeMatFunctions())
        return;

    if (!TestTemplateMat())
        return;

 /*   if (!TestMatND())
        return;*/

    if (!TestSparseMat())
        return;

    if (!TestVec())
        return;

    if (!TestMatxMultiplication())
        return;

    if (!TestMatxElementwiseDivison())
        return;

    if (!TestSubMatAccess())
        return;

    if (!TestExp())
        return;

    if (!TestSVD())
        return;

    if (!operations1())
        return;

    ts->set_failed_test_info(cvtest::TS::OK);
}

TEST(Core_Array, expressions) { CC_OperationsTest test; test.safe_run(); }

class CC_SparseMatTest : public cvtest::BaseTest
{
public:
    CC_SparseMatTest() {}
    ~CC_SparseMatTest() {}
protected:
    void run(int)
    {
        try
        {
            RNG& rng = theRNG();
            const int MAX_DIM=3;
            int sizes[MAX_DIM], idx[MAX_DIM];
            for( int iter = 0; iter < 100; iter++ )
            {
                ts->printf(cvtest::TS::LOG, ".");
                ts->update_context(this, iter, true);
                int k, dims = rng.uniform(1, MAX_DIM+1), p = 1;
                for( k = 0; k < dims; k++ )
                {
                    sizes[k] = rng.uniform(1, 30);
                    p *= sizes[k];
                }
                int j, nz = rng.uniform(0, (p+2)/2), nz0 = 0;
                SparseMat_<int> v(dims,sizes);

                CC_Assert( (int)v.nzcount() == 0 );

                SparseMatIterator_<int> it = v.begin();
                SparseMatIterator_<int> it_end = v.end();

                for( k = 0; it != it_end; ++it, ++k )
                    ;
                CC_Assert( k == 0 );

                int sum0 = 0, sum = 0;
                for( j = 0; j < nz; j++ )
                {
                    int val = rng.uniform(1, 100);
                    for( k = 0; k < dims; k++ )
                        idx[k] = rng.uniform(0, sizes[k]);
                    if( dims == 1 )
                    {
                        CC_Assert( v.ref(idx[0]) == v(idx[0]) );
                    }
                    else if( dims == 2 )
                    {
                        CC_Assert( v.ref(idx[0], idx[1]) == v(idx[0], idx[1]) );
                    }
                    else if( dims == 3 )
                    {
                        CC_Assert( v.ref(idx[0], idx[1], idx[2]) == v(idx[0], idx[1], idx[2]) );
                    }
                    CC_Assert( v.ref(idx) == v(idx) );
                    v.ref(idx) += val;
                    if( v(idx) == val )
                        nz0++;
                    sum0 += val;
                }

                CC_Assert( (int)v.nzcount() == nz0 );

                it = v.begin();
                it_end = v.end();

                for( k = 0; it != it_end; ++it, ++k )
                    sum += *it;
                CC_Assert( k == nz0 && sum == sum0 );

                v.clear();
                CC_Assert( (int)v.nzcount() == 0 );

                it = v.begin();
                it_end = v.end();

                for( k = 0; it != it_end; ++it, ++k )
                    ;
                CC_Assert( k == 0 );
            }
        }
        catch(...)
        {
            ts->set_failed_test_info(cvtest::TS::FAIL_MISMATCH);
        }
    }
};

TEST(Core_SparseMat, iterations) { CC_SparseMatTest test; test.safe_run(); }
