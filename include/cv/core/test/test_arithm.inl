#include "test_precomp.hpp"
#include <cmath>

using namespace cv;
using namespace std;

namespace cvtest
{

const int ARITHM_NTESTS = 1000;
const int ARITHM_RNG_SEED = -1;
const int ARITHM_MAX_CHANNELS = 4;
const int ARITHM_MAX_NDIMS = 4;
const int ARITHM_MAX_SIZE_LOG = 10;

struct BaseElemWiseOp
{
    enum { FIX_ALPHA=1, FIX_BETA=2, FIX_GAMMA=4, REAL_GAMMA=8, SUPPORT_MASK=16, SCALAR_OUTPUT=32 };
    BaseElemWiseOp(int _ninputs, int _flags, double _alpha, double _beta,
                   Scalar _gamma=Scalar::all(0), int _context=1)
    : ninputs(_ninputs), flags(_flags), alpha(_alpha), beta(_beta), gamma(_gamma), context(_context) {}
    BaseElemWiseOp() { flags = 0; alpha = beta = 0; gamma = Scalar::all(0); ninputs = 0; context = 1; }
    virtual ~BaseElemWiseOp() {}
    virtual void op(const vector<img_t>&, img_t&, const img_t&) {}
    virtual void refop(const vector<img_t>&, img_t&, const img_t&) {}
    virtual void getValueRange(int depth, double& minval, double& maxval)
    {
        minval = depth < CC_32S ? cvtest::getMinVal(depth) : depth == CC_32S ? -1000000 : -1000.;
        maxval = depth < CC_32S ? cvtest::getMaxVal(depth) : depth == CC_32S ? 1000000 : 1000.;
    }

    virtual void getRandomSize(RNG& rng, vector<int>& size)
    {
        cvtest::randomSize(rng, 2, ARITHM_MAX_NDIMS, cvtest::ARITHM_MAX_SIZE_LOG, size);
    }

    virtual int getRandomType(RNG& rng)
    {
        return cvtest::randomType(rng, _OutputArray::DEPTH_MASK_ALL_BUT_8S, 1,
                                  ninputs > 1 ? ARITHM_MAX_CHANNELS : 4);
    }

    virtual double getMaxErr(int depth) { return depth < CC_32F ? 1 : depth == CC_32F ? 1e-5 : 1e-12; }
    virtual void generateScalars(int depth, RNG& rng)
    {
        const double m = 3.;

        if( !(flags & FIX_ALPHA) )
        {
            alpha = exp(rng.uniform(-0.5, 0.1)*m*2*CC_LOG2);
            alpha *= rng.uniform(0, 2) ? 1 : -1;
        }
        if( !(flags & FIX_BETA) )
        {
            beta = exp(rng.uniform(-0.5, 0.1)*m*2*CC_LOG2);
            beta *= rng.uniform(0, 2) ? 1 : -1;
        }

        if( !(flags & FIX_GAMMA) )
        {
            for( int i = 0; i < 4; i++ )
            {
                gamma[i] = exp(rng.uniform(-1, 6)*m*CC_LOG2);
                gamma[i] *= rng.uniform(0, 2) ? 1 : -1;
            }
            if( flags & REAL_GAMMA )
                gamma = Scalar::all(gamma[0]);
        }

        if( depth == CC_32F )
        {
            img_t fl, db;

            db = img_t(1, 1, CC_64F, &alpha);
            db.convertTo(fl, CC_32F);
            fl.convertTo(db, CC_64F);

            db = img_t(1, 1, CC_64F, &beta);
            db.convertTo(fl, CC_32F);
            fl.convertTo(db, CC_64F);

            db = img_t(1, 4, CC_64F, &gamma[0]);
            db.convertTo(fl, CC_32F);
            fl.convertTo(db, CC_64F);
        }
    }

    int ninputs;
    int flags;
    double alpha;
    double beta;
    Scalar gamma;
    int context;
};


struct BaseAddOp : public BaseElemWiseOp
{
    BaseAddOp(int _ninputs, int _flags, double _alpha, double _beta, Scalar _gamma=Scalar::all(0))
    : BaseElemWiseOp(_ninputs, _flags, _alpha, _beta, _gamma) {}

    void refop(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        img_t temp;
        if( !mask.empty() )
        {
            cvtest::add(src[0], alpha, cvGetSize(src) > 1 ? src[1] : img_t(), beta, gamma, temp, src[0] CC_MAT_TYPE());
            cvtest::copy(temp, dst, mask);
        }
        else
            cvtest::add(src[0], alpha, cvGetSize(src) > 1 ? src[1] : img_t(), beta, gamma, dst, src[0] CC_MAT_TYPE());
    }
};


struct AddOp : public BaseAddOp
{
    AddOp() : BaseAddOp(2, FIX_ALPHA+FIX_BETA+FIX_GAMMA+SUPPORT_MASK, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        if( mask.empty() )
            add(src[0], src[1], dst);
        else
            add(src[0], src[1], dst, mask);
    }
};


struct SubOp : public BaseAddOp
{
    SubOp() : BaseAddOp(2, FIX_ALPHA+FIX_BETA+FIX_GAMMA+SUPPORT_MASK, 1, -1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        if( mask.empty() )
            subtract(src[0], src[1], dst);
        else
            subtract(src[0], src[1], dst, mask);
    }
};


struct AddSOp : public BaseAddOp
{
    AddSOp() : BaseAddOp(1, FIX_ALPHA+FIX_BETA+SUPPORT_MASK, 1, 0, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        if( mask.empty() )
            add(src[0], gamma, dst);
        else
            add(src[0], gamma, dst, mask);
    }
};


struct SubRSOp : public BaseAddOp
{
    SubRSOp() : BaseAddOp(1, FIX_ALPHA+FIX_BETA+SUPPORT_MASK, -1, 0, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        if( mask.empty() )
            subtract(gamma, src[0], dst);
        else
            subtract(gamma, src[0], dst, mask);
    }
};


struct ScaleAddOp : public BaseAddOp
{
    ScaleAddOp() : BaseAddOp(2, FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        scaleAdd(src[0], alpha, src[1], dst);
    }
    double getMaxErr(int depth)
    {
        return depth <= CC_32S ? 2 : depth < CC_64F ? 1e-4 : 1e-12;
    }
};


struct AddWeightedOp : public BaseAddOp
{
    AddWeightedOp() : BaseAddOp(2, REAL_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        addWeighted(src[0], alpha, src[1], beta, gamma[0], dst);
    }
    double getMaxErr(int depth)
    {
        return depth <= CC_32S ? 2 : depth < CC_64F ? 1e-5 : 1e-10;
    }
};

struct MulOp : public BaseElemWiseOp
{
    MulOp() : BaseElemWiseOp(2, FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    void getValueRange(int depth, double& minval, double& maxval)
    {
        minval = depth < CC_32S ? cvtest::getMinVal(depth) : depth == CC_32S ? -1000000 : -1000.;
        maxval = depth < CC_32S ? cvtest::getMaxVal(depth) : depth == CC_32S ? 1000000 : 1000.;
        minval = MAX(minval, -30000.);
        maxval = MIN(maxval, 30000.);
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        multiply(src[0], src[1], dst, alpha);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::multiply(src[0], src[1], dst, alpha);
    }
    double getMaxErr(int depth)
    {
        return depth <= CC_32S ? 2 : depth < CC_64F ? 1e-5 : 1e-12;
    }
};

struct DivOp : public BaseElemWiseOp
{
    DivOp() : BaseElemWiseOp(2, FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        divide(src[0], src[1], dst, alpha);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::divide(src[0], src[1], dst, alpha);
    }
    double getMaxErr(int depth)
    {
        return depth <= CC_32S ? 2 : depth < CC_64F ? 1e-5 : 1e-12;
    }
};

struct RecipOp : public BaseElemWiseOp
{
    RecipOp() : BaseElemWiseOp(1, FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        divide(alpha, src[0], dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::divide(img_t(), src[0], dst, alpha);
    }
    double getMaxErr(int depth)
    {
        return depth <= CC_32S ? 2 : depth < CC_64F ? 1e-5 : 1e-12;
    }
};

struct AbsDiffOp : public BaseAddOp
{
    AbsDiffOp() : BaseAddOp(2, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, -1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        absdiff(src[0], src[1], dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::add(src[0], 1, src[1], -1, Scalar::all(0), dst, src[0] CC_MAT_TYPE(), true);
    }
};

struct AbsDiffSOp : public BaseAddOp
{
    AbsDiffSOp() : BaseAddOp(1, FIX_ALPHA+FIX_BETA, 1, 0, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        absdiff(src[0], gamma, dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::add(src[0], 1, img_t(), 0, -gamma, dst, src[0] CC_MAT_TYPE(), true);
    }
};

struct LogicOp : public BaseElemWiseOp
{
    LogicOp(char _opcode) : BaseElemWiseOp(2, FIX_ALPHA+FIX_BETA+FIX_GAMMA+SUPPORT_MASK, 1, 1, Scalar::all(0)), opcode(_opcode) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        if( opcode == '&' )
            bitwise_and(src[0], src[1], dst, mask);
        else if( opcode == '|' )
            bitwise_or(src[0], src[1], dst, mask);
        else
            bitwise_xor(src[0], src[1], dst, mask);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        img_t temp;
        if( !mask.empty() )
        {
            cvtest::logicOp(src[0], src[1], temp, opcode);
            cvtest::copy(temp, dst, mask);
        }
        else
            cvtest::logicOp(src[0], src[1], dst, opcode);
    }
    double getMaxErr(int)
    {
        return 0;
    }
    char opcode;
};

struct LogicSOp : public BaseElemWiseOp
{
    LogicSOp(char _opcode)
    : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+(_opcode != '~' ? SUPPORT_MASK : 0), 1, 1, Scalar::all(0)), opcode(_opcode) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        if( opcode == '&' )
            bitwise_and(src[0], gamma, dst, mask);
        else if( opcode == '|' )
            bitwise_or(src[0], gamma, dst, mask);
        else if( opcode == '^' )
            bitwise_xor(src[0], gamma, dst, mask);
        else
            bitwise_not(src[0], dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        img_t temp;
        if( !mask.empty() )
        {
            cvtest::logicOp(src[0], gamma, temp, opcode);
            cvtest::copy(temp, dst, mask);
        }
        else
            cvtest::logicOp(src[0], gamma, dst, opcode);
    }
    double getMaxErr(int)
    {
        return 0;
    }
    char opcode;
};

struct MinOp : public BaseElemWiseOp
{
    MinOp() : BaseElemWiseOp(2, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        min(src[0], src[1], dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::min(src[0], src[1], dst);
    }
    double getMaxErr(int)
    {
        return 0;
    }
};

struct MaxOp : public BaseElemWiseOp
{
    MaxOp() : BaseElemWiseOp(2, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        max(src[0], src[1], dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::max(src[0], src[1], dst);
    }
    double getMaxErr(int)
    {
        return 0;
    }
};

struct MinSOp : public BaseElemWiseOp
{
    MinSOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+REAL_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        min(src[0], gamma[0], dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::min(src[0], gamma[0], dst);
    }
    double getMaxErr(int)
    {
        return 0;
    }
};

struct MaxSOp : public BaseElemWiseOp
{
    MaxSOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+REAL_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        max(src[0], gamma[0], dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::max(src[0], gamma[0], dst);
    }
    double getMaxErr(int)
    {
        return 0;
    }
};

struct CmpOp : public BaseElemWiseOp
{
    CmpOp() : BaseElemWiseOp(2, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) { cmpop = 0; }
    void generateScalars(int depth, RNG& rng)
    {
        BaseElemWiseOp::generateScalars(depth, rng);
        cmpop = rng.uniform(0, 6);
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        compare(src[0], src[1], dst, cmpop);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::compare(src[0], src[1], dst, cmpop);
    }
    int getRandomType(RNG& rng)
    {
        return cvtest::randomType(rng, _OutputArray::DEPTH_MASK_ALL_BUT_8S, 1, 1);
    }

    double getMaxErr(int)
    {
        return 0;
    }
    int cmpop;
};

struct CmpSOp : public BaseElemWiseOp
{
    CmpSOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+REAL_GAMMA, 1, 1, Scalar::all(0)) { cmpop = 0; }
    void generateScalars(int depth, RNG& rng)
    {
        BaseElemWiseOp::generateScalars(depth, rng);
        cmpop = rng.uniform(0, 6);
        if( depth < CC_32F )
            gamma[0] = cRound(gamma[0]);
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        compare(src[0], gamma[0], dst, cmpop);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::compare(src[0], gamma[0], dst, cmpop);
    }
    int getRandomType(RNG& rng)
    {
        return cvtest::randomType(rng, _OutputArray::DEPTH_MASK_ALL_BUT_8S, 1, 1);
    }
    double getMaxErr(int)
    {
        return 0;
    }
    int cmpop;
};


struct CopyOp : public BaseElemWiseOp
{
    CopyOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+FIX_GAMMA+SUPPORT_MASK, 1, 1, Scalar::all(0)) {  }
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        src[0].copyTo(dst, mask);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        cvtest::copy(src[0], dst, mask);
    }
    int getRandomType(RNG& rng)
    {
        return cvtest::randomType(rng, _OutputArray::DEPTH_MASK_ALL, 1, ARITHM_MAX_CHANNELS);
    }
    double getMaxErr(int)
    {
        return 0;
    }
};


struct SetOp : public BaseElemWiseOp
{
    SetOp() : BaseElemWiseOp(0, FIX_ALPHA+FIX_BETA+SUPPORT_MASK, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>&, img_t& dst, const img_t& mask)
    {
        dst.setTo(gamma, mask);
    }
    void refop(const vector<img_t>&, img_t& dst, const img_t& mask)
    {
        cvtest::set(dst, gamma, mask);
    }
    int getRandomType(RNG& rng)
    {
        return cvtest::randomType(rng, _OutputArray::DEPTH_MASK_ALL, 1, ARITHM_MAX_CHANNELS);
    }
    double getMaxErr(int)
    {
        return 0;
    }
};

template<typename _Tp, typename _WTp> static void
inRangeS_(const _Tp* src, const _WTp* a, const _WTp* b, uchar* dst, size_t total, int cn)
{
    size_t i;
    int c;
    for( i = 0; i < total; i++ )
    {
        _Tp val = src[i*cn];
        dst[i] = (a[0] <= val && val <= b[0]) ? uchar(255) : 0;
    }
    for( c = 1; c < cn; c++ )
    {
        for( i = 0; i < total; i++ )
        {
            _Tp val = src[i*cn + c];
            dst[i] = a[c] <= val && val <= b[c] ? dst[i] : 0;
        }
    }
}

template<typename _Tp> static void inRange_(const _Tp* src, const _Tp* a, const _Tp* b, uchar* dst, size_t total, int cn)
{
    size_t i;
    int c;
    for( i = 0; i < total; i++ )
    {
        _Tp val = src[i*cn];
        dst[i] = a[i*cn] <= val && val <= b[i*cn] ? 255 : 0;
    }
    for( c = 1; c < cn; c++ )
    {
        for( i = 0; i < total; i++ )
        {
            _Tp val = src[i*cn + c];
            dst[i] = a[i*cn + c] <= val && val <= b[i*cn + c] ? dst[i] : 0;
        }
    }
}


static void inRange(const img_t& src, const img_t& lb, const img_t& rb, img_t& dst)
{
    CC_Assert( src CC_MAT_TYPE() == lb CC_MAT_TYPE() && src CC_MAT_TYPE() == rb CC_MAT_TYPE() &&
              src.size == lb.size && src.size == rb.size );
    dst cvSetMat( src.dims, &src.size[0], CC_8U );
    const img_t *arrays[]={&src, &lb, &rb, &dst, 0};
    img_t planes[4];

    NAryMatIterator it(arrays, planes);
    size_t total = planes[0] CC_MAT_TOTAL();
    size_t i, nplanes = it.nplanes;
    int depth = src CC_MAT_DEPTH(), cn = src CC_MAT_CN();

    for( i = 0; i < nplanes; i++, ++it )
    {
        const uchar* sptr = planes[0]->tt.data;
        const uchar* aptr = planes[1]->tt.data;
        const uchar* bptr = planes[2]->tt.data;
        uchar* dptr = planes[3]->tt.data;

        switch( depth )
        {
        case CC_8U:
            inRange_((const uchar*)sptr, (const uchar*)aptr, (const uchar*)bptr, dptr, total, cn);
            break;
        case CC_8S:
            inRange_((const schar*)sptr, (const schar*)aptr, (const schar*)bptr, dptr, total, cn);
            break;
        case CC_16U:
            inRange_((const ushort*)sptr, (const ushort*)aptr, (const ushort*)bptr, dptr, total, cn);
            break;
        case CC_16S:
            inRange_((const short*)sptr, (const short*)aptr, (const short*)bptr, dptr, total, cn);
            break;
        case CC_32S:
            inRange_((const int*)sptr, (const int*)aptr, (const int*)bptr, dptr, total, cn);
            break;
        case CC_32F:
            inRange_((const float*)sptr, (const float*)aptr, (const float*)bptr, dptr, total, cn);
            break;
        case CC_64F:
            inRange_((const double*)sptr, (const double*)aptr, (const double*)bptr, dptr, total, cn);
            break;
        default:
            CC_Error(CC_StsUnsupportedFormat, "");
        }
    }
}


static void inRangeS(const img_t& src, const Scalar& lb, const Scalar& rb, img_t& dst)
{
    dst cvSetMat( src.dims, &src.size[0], CC_8U );
    const img_t *arrays[]={&src, &dst, 0};
    img_t planes[2];

    NAryMatIterator it(arrays, planes);
    size_t total = planes[0] CC_MAT_TOTAL();
    size_t i, nplanes = it.nplanes;
    int depth = src CC_MAT_DEPTH(), cn = src CC_MAT_CN();
    union { double d[4]; float f[4]; int i[4];} lbuf, rbuf;
    int wtype = CC_MAKETYPE(depth <= CC_32S ? CC_32S : depth, cn);
    scalarToRawData(lb, lbuf.d, wtype, cn);
    scalarToRawData(rb, rbuf.d, wtype, cn);

    for( i = 0; i < nplanes; i++, ++it )
    {
        const uchar* sptr = planes[0]->tt.data;
        uchar* dptr = planes[1]->tt.data;

        switch( depth )
        {
        case CC_8U:
            inRangeS_((const uchar*)sptr, lbuf.i, rbuf.i, dptr, total, cn);
            break;
        case CC_8S:
            inRangeS_((const schar*)sptr, lbuf.i, rbuf.i, dptr, total, cn);
            break;
        case CC_16U:
            inRangeS_((const ushort*)sptr, lbuf.i, rbuf.i, dptr, total, cn);
            break;
        case CC_16S:
            inRangeS_((const short*)sptr, lbuf.i, rbuf.i, dptr, total, cn);
            break;
        case CC_32S:
            inRangeS_((const int*)sptr, lbuf.i, rbuf.i, dptr, total, cn);
            break;
        case CC_32F:
            inRangeS_((const float*)sptr, lbuf.f, rbuf.f, dptr, total, cn);
            break;
        case CC_64F:
            inRangeS_((const double*)sptr, lbuf.d, rbuf.d, dptr, total, cn);
            break;
        default:
            CC_Error(CC_StsUnsupportedFormat, "");
        }
    }
}


struct InRangeSOp : public BaseElemWiseOp
{
    InRangeSOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        inRange(src[0], gamma, gamma1, dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::inRangeS(src[0], gamma, gamma1, dst);
    }
    double getMaxErr(int)
    {
        return 0;
    }
    void generateScalars(int depth, RNG& rng)
    {
        BaseElemWiseOp::generateScalars(depth, rng);
        Scalar temp = gamma;
        BaseElemWiseOp::generateScalars(depth, rng);
        for( int i = 0; i < 4; i++ )
        {
            gamma1[i] = MAX(gamma[i], temp[i]);
            gamma[i] = MIN(gamma[i], temp[i]);
        }
    }
    Scalar gamma1;
};


struct InRangeOp : public BaseElemWiseOp
{
    InRangeOp() : BaseElemWiseOp(3, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        img_t lb, rb;
        cvtest::min(src[1], src[2], lb);
        cvtest::max(src[1], src[2], rb);

        inRange(src[0], lb, rb, dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        img_t lb, rb;
        cvtest::min(src[1], src[2], lb);
        cvtest::max(src[1], src[2], rb);

        cvtest::inRange(src[0], lb, rb, dst);
    }
    double getMaxErr(int)
    {
        return 0;
    }
};


struct ConvertScaleOp : public BaseElemWiseOp
{
    ConvertScaleOp() : BaseElemWiseOp(1, FIX_BETA+REAL_GAMMA, 1, 1, Scalar::all(0)), ddepth(0) { }
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        src[0].convertTo(dst, ddepth, alpha, gamma[0]);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::convert(src[0], dst, CC_MAKETYPE(ddepth, src[0] CC_MAT_CN()), alpha, gamma[0]);
    }
    int getRandomType(RNG& rng)
    {
        int srctype = cvtest::randomType(rng, _OutputArray::DEPTH_MASK_ALL, 1, ARITHM_MAX_CHANNELS);
        ddepth = cvtest::randomType(rng, _OutputArray::DEPTH_MASK_ALL, 1, 1);
        return srctype;
    }
    double getMaxErr(int)
    {
        return ddepth <= CC_32S ? 2 : ddepth < CC_64F ? 1e-3 : 1e-12;
    }
    void generateScalars(int depth, RNG& rng)
    {
        if( rng.uniform(0, 2) )
            BaseElemWiseOp::generateScalars(depth, rng);
        else
        {
            alpha = 1;
            gamma = Scalar::all(0);
        }
    }
    int ddepth;
};

struct ConvertScaleFp16Op : public BaseElemWiseOp
{
    ConvertScaleFp16Op() : BaseElemWiseOp(1, FIX_BETA+REAL_GAMMA, 1, 1, Scalar::all(0)), nextRange(0) { }
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        img_t m;
        convertFp16(src[0], m);
        convertFp16(m, dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::copy(src[0], dst);
    }
    int getRandomType(RNG&)
    {
        // 0: FP32 -> FP16 -> FP32
        // 1: FP16 -> FP32 -> FP16
        int srctype = (nextRange & 1) == 0 ? CC_32F : CC_16S;
        return srctype;
    }
    void getValueRange(int, double& minval, double& maxval)
    {
        // 0: FP32 -> FP16 -> FP32
        // 1: FP16 -> FP32 -> FP16
        if( (nextRange & 1) == 0 )
        {
            // largest integer number that fp16 can express exactly
            maxval = 2048.f;
            minval = -maxval;
        }
        else
        {
            // 0: positive number range
            // 1: negative number range
            if( (nextRange & 2) == 0 )
            {
                minval = 0;      // 0x0000 +0
                maxval = 31744;  // 0x7C00 +Inf
            }
            else
            {
                minval = -32768; // 0x8000 -0
                maxval = -1024;  // 0xFC00 -Inf
            }
        }
    }
    double getMaxErr(int)
    {
        return 0.5f;
    }
    void generateScalars(int, RNG& rng)
    {
        nextRange = rng.next();
    }
    int nextRange;
};

struct ConvertScaleAbsOp : public BaseElemWiseOp
{
    ConvertScaleAbsOp() : BaseElemWiseOp(1, FIX_BETA+REAL_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        convertScaleAbs(src[0], dst, alpha, gamma[0]);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::add(src[0], alpha, img_t(), 0, Scalar::all(gamma[0]), dst, CC_8UC(src[0] CC_MAT_CN()), true);
    }
    double getMaxErr(int)
    {
        return 1;
    }
    void generateScalars(int depth, RNG& rng)
    {
        if( rng.uniform(0, 2) )
            BaseElemWiseOp::generateScalars(depth, rng);
        else
        {
            alpha = 1;
            gamma = Scalar::all(0);
        }
    }
};


static void flip(const img_t& src, img_t& dst, int flipcode)
{
    CC_Assert(src.dims == 2);
    dst cvSetMat(cvGetSize(src), src CC_MAT_TYPE());
    int i, j, k, esz = (int)src CC_ELEM_SIZE(), width = src->cols*esz;

    for( i = 0; i < dst->rows; i++ )
    {
        const uchar* sptr = src.ptr(flipcode == 1 ? i : dst->rows - i - 1);
        uchar* dptr = dst.ptr(i);
        if( flipcode == 0 )
            memcpy(dptr, sptr, width);
        else
        {
            for( j = 0; j < width; j += esz )
                for( k = 0; k < esz; k++ )
                    dptr[j + k] = sptr[width - j - esz + k];
        }
    }
}


static void setIdentity(img_t& dst, const Scalar& s)
{
    CC_Assert( dst.dims == 2 && dst CC_MAT_CN() <= 4 );
    double buf[4];
    scalarToRawData(s, buf, dst CC_MAT_TYPE(), 0);
    int i, k, esz = (int)dst CC_ELEM_SIZE(), width = dst->cols*esz;

    for( i = 0; i < dst->rows; i++ )
    {
        uchar* dptr = dst.ptr(i);
        memset( dptr, 0, width );
        if( i < dst->cols )
            for( k = 0; k < esz; k++ )
                dptr[i*esz + k] = ((uchar*)buf)[k];
    }
}


struct FlipOp : public BaseElemWiseOp
{
    FlipOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) { flipcode = 0; }
    void getRandomSize(RNG& rng, vector<int>& size)
    {
        cvtest::randomSize(rng, 2, 2, cvtest::ARITHM_MAX_SIZE_LOG, size);
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        flip(src[0], dst, flipcode);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::flip(src[0], dst, flipcode);
    }
    void generateScalars(int, RNG& rng)
    {
        flipcode = rng.uniform(0, 3) - 1;
    }
    double getMaxErr(int)
    {
        return 0;
    }
    int flipcode;
};

struct TransposeOp : public BaseElemWiseOp
{
    TransposeOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    void getRandomSize(RNG& rng, vector<int>& size)
    {
        cvtest::randomSize(rng, 2, 2, cvtest::ARITHM_MAX_SIZE_LOG, size);
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        transpose(src[0], dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::transpose(src[0], dst);
    }
    double getMaxErr(int)
    {
        return 0;
    }
};

struct SetIdentityOp : public BaseElemWiseOp
{
    SetIdentityOp() : BaseElemWiseOp(0, FIX_ALPHA+FIX_BETA, 1, 1, Scalar::all(0)) {}
    void getRandomSize(RNG& rng, vector<int>& size)
    {
        cvtest::randomSize(rng, 2, 2, cvtest::ARITHM_MAX_SIZE_LOG, size);
    }
    void op(const vector<img_t>&, img_t& dst, const img_t&)
    {
        setIdentity(dst, gamma);
    }
    void refop(const vector<img_t>&, img_t& dst, const img_t&)
    {
        cvtest::setIdentity(dst, gamma);
    }
    double getMaxErr(int)
    {
        return 0;
    }
};

struct SetZeroOp : public BaseElemWiseOp
{
    SetZeroOp() : BaseElemWiseOp(0, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    void op(const vector<img_t>&, img_t& dst, const img_t&)
    {
        dst = Scalar::all(0);
    }
    void refop(const vector<img_t>&, img_t& dst, const img_t&)
    {
        cvtest::set(dst, Scalar::all(0));
    }
    double getMaxErr(int)
    {
        return 0;
    }
};


static void exp(const img_t& src, img_t& dst)
{
    dst cvSetMat( src.dims, &src.size[0], src CC_MAT_TYPE() );
    const img_t *arrays[]={&src, &dst, 0};
    img_t planes[2];

    NAryMatIterator it(arrays, planes);
    size_t j, total = planes[0] CC_MAT_TOTAL()*src CC_MAT_CN();
    size_t i, nplanes = it.nplanes;
    int depth = src CC_MAT_DEPTH();

    for( i = 0; i < nplanes; i++, ++it )
    {
        const uchar* sptr = planes[0]->tt.data;
        uchar* dptr = planes[1]->tt.data;

        if( depth == CC_32F )
        {
            for( j = 0; j < total; j++ )
                ((float*)dptr)[j] = std::exp(((const float*)sptr)[j]);
        }
        else if( depth == CC_64F )
        {
            for( j = 0; j < total; j++ )
                ((double*)dptr)[j] = std::exp(((const double*)sptr)[j]);
        }
    }
}

static void log(const img_t& src, img_t& dst)
{
    dst cvSetMat( src.dims, &src.size[0], src CC_MAT_TYPE() );
    const img_t *arrays[]={&src, &dst, 0};
    img_t planes[2];

    NAryMatIterator it(arrays, planes);
    size_t j, total = planes[0] CC_MAT_TOTAL()*src CC_MAT_CN();
    size_t i, nplanes = it.nplanes;
    int depth = src CC_MAT_DEPTH();

    for( i = 0; i < nplanes; i++, ++it )
    {
        const uchar* sptr = planes[0]->tt.data;
        uchar* dptr = planes[1]->tt.data;

        if( depth == CC_32F )
        {
            for( j = 0; j < total; j++ )
                ((float*)dptr)[j] = (float)std::log(fabs(((const float*)sptr)[j]));
        }
        else if( depth == CC_64F )
        {
            for( j = 0; j < total; j++ )
                ((double*)dptr)[j] = std::log(fabs(((const double*)sptr)[j]));
        }
    }
}

struct ExpOp : public BaseElemWiseOp
{
    ExpOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    int getRandomType(RNG& rng)
    {
        return cvtest::randomType(rng, _OutputArray::DEPTH_MASK_FLT, 1, ARITHM_MAX_CHANNELS);
    }
    void getValueRange(int depth, double& minval, double& maxval)
    {
        maxval = depth == CC_32F ? 50 : 100;
        minval = -maxval;
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        exp(src[0], dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        cvtest::exp(src[0], dst);
    }
    double getMaxErr(int depth)
    {
        return depth == CC_32F ? 1e-5 : 1e-12;
    }
};


struct LogOp : public BaseElemWiseOp
{
    LogOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0)) {}
    int getRandomType(RNG& rng)
    {
        return cvtest::randomType(rng, _OutputArray::DEPTH_MASK_FLT, 1, ARITHM_MAX_CHANNELS);
    }
    void getValueRange(int depth, double& minval, double& maxval)
    {
        maxval = depth == CC_32F ? 50 : 100;
        minval = -maxval;
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        img_t temp;
        cvtest::exp(src[0], temp);
        log(temp, dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        img_t temp;
        cvtest::exp(src[0], temp);
        cvtest::log(temp, dst);
    }
    double getMaxErr(int depth)
    {
        return depth == CC_32F ? 1e-5 : 1e-12;
    }
};


static void cartToPolar(const img_t& mx, const img_t& my, img_t& mmag, img_t& mangle, bool angleInDegrees)
{
    CC_Assert( (mx CC_MAT_TYPE() == CC_32F || mx CC_MAT_TYPE() == CC_64F) &&
              mx CC_MAT_TYPE() == my CC_MAT_TYPE() && mx.size == my.size );
    mmag cvSetMat( mx.dims, &mx.size[0], mx CC_MAT_TYPE() );
    mangle cvSetMat( mx.dims, &mx.size[0], mx CC_MAT_TYPE() );
    const img_t *arrays[]={&mx, &my, &mmag, &mangle, 0};
    img_t planes[4];

    NAryMatIterator it(arrays, planes);
    size_t j, total = planes[0] CC_MAT_TOTAL();
    size_t i, nplanes = it.nplanes;
    int depth = mx CC_MAT_DEPTH();
    double scale = angleInDegrees ? 180/CC_PI : 1;

    for( i = 0; i < nplanes; i++, ++it )
    {
        if( depth == CC_32F )
        {
            const float* xptr = planes[0] img_ptr(float, );
            const float* yptr = planes[1] img_ptr(float, );
            float* mptr = planes[2] img_ptr(float, );
            float* aptr = planes[3] img_ptr(float, );

            for( j = 0; j < total; j++ )
            {
                mptr[j] = sqrt(xptr[j]*xptr[j] + yptr[j]*yptr[j]);
                double a = atan2((double)yptr[j], (double)xptr[j]);
                if( a < 0 ) a += CC_PI*2;
                aptr[j] = (float)(a*scale);
            }
        }
        else
        {
            const double* xptr = planes[0] img_ptr(double, );
            const double* yptr = planes[1] img_ptr(double, );
            double* mptr = planes[2] img_ptr(double, );
            double* aptr = planes[3] img_ptr(double, );

            for( j = 0; j < total; j++ )
            {
                mptr[j] = sqrt(xptr[j]*xptr[j] + yptr[j]*yptr[j]);
                double a = atan2(yptr[j], xptr[j]);
                if( a < 0 ) a += CC_PI*2;
                aptr[j] = a*scale;
            }
        }
    }
}


struct CartToPolarToCartOp : public BaseElemWiseOp
{
    CartToPolarToCartOp() : BaseElemWiseOp(2, FIX_ALPHA+FIX_BETA+FIX_GAMMA, 1, 1, Scalar::all(0))
    {
        context = 3;
        angleInDegrees = true;
    }
    int getRandomType(RNG& rng)
    {
        return cvtest::randomType(rng, _OutputArray::DEPTH_MASK_FLT, 1, 1);
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        img_t mag, angle, x, y;

        cartToPolar(src[0], src[1], mag, angle, angleInDegrees);
        polarToCart(mag, angle, x, y, angleInDegrees);

        img_t msrc[] = {mag, angle, x, y};
        int pairs[] = {0, 0, 1, 1, 2, 2, 3, 3};
        dst cvSetMat(src[0].dims, src[0].size, CC_MAKETYPE(src[0] CC_MAT_DEPTH(), 4));
        mixChannels(msrc, 4, &dst, 1, pairs, 4);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        img_t mag, angle;
        cvtest::cartToPolar(src[0], src[1], mag, angle, angleInDegrees);
        img_t msrc[] = {mag, angle, src[0], src[1]};
        int pairs[] = {0, 0, 1, 1, 2, 2, 3, 3};
        dst cvSetMat(src[0].dims, src[0].size, CC_MAKETYPE(src[0] CC_MAT_DEPTH(), 4));
        mixChannels(msrc, 4, &dst, 1, pairs, 4);
    }
    void generateScalars(int, RNG& rng)
    {
        angleInDegrees = rng.uniform(0, 2) != 0;
    }
    double getMaxErr(int)
    {
        return 1e-3;
    }
    bool angleInDegrees;
};


struct MeanOp : public BaseElemWiseOp
{
    MeanOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+FIX_GAMMA+SUPPORT_MASK+SCALAR_OUTPUT, 1, 1, Scalar::all(0))
    {
        context = 3;
    };
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        dst cvSetMat(1, 1, CC_64FC4);
        dst.at<Scalar>(0,0) = mean(src[0], mask);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        dst cvSetMat(1, 1, CC_64FC4);
        dst.at<Scalar>(0,0) = cvtest::mean(src[0], mask);
    }
    double getMaxErr(int)
    {
        return 1e-5;
    }
};


struct SumOp : public BaseElemWiseOp
{
    SumOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+FIX_GAMMA+SCALAR_OUTPUT, 1, 1, Scalar::all(0))
    {
        context = 3;
    };
    void op(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        dst cvSetMat(1, 1, CC_64FC4);
        dst.at<Scalar>(0,0) = sum(src[0]);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t&)
    {
        dst cvSetMat(1, 1, CC_64FC4);
        dst.at<Scalar>(0,0) = cvtest::mean(src[0])*(double)src[0] CC_MAT_TOTAL();
    }
    double getMaxErr(int)
    {
        return 1e-5;
    }
};


struct CountNonZeroOp : public BaseElemWiseOp
{
    CountNonZeroOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+FIX_GAMMA+SCALAR_OUTPUT+SUPPORT_MASK, 1, 1, Scalar::all(0))
    {}
    int getRandomType(RNG& rng)
    {
        return cvtest::randomType(rng, _OutputArray::DEPTH_MASK_ALL, 1, 1);
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        img_t temp;
        src[0].copyTo(temp);
        if( !mask.empty() )
            temp.setTo(Scalar::all(0), mask);
        dst cvSetMat(1, 1, CC_32S);
        dst.at<int>(0,0) = countNonZero(temp);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        img_t temp;
        cvtest::compare(src[0], 0, temp, CMP_NE);
        if( !mask.empty() )
            cvtest::set(temp, Scalar::all(0), mask);
        dst cvSetMat(1, 1, CC_32S);
        dst.at<int>(0,0) = saturate_cast<int>(cvtest::mean(temp)[0]/255*temp CC_MAT_TOTAL());
    }
    double getMaxErr(int)
    {
        return 0;
    }
};


struct MeanStdDevOp : public BaseElemWiseOp
{
    Scalar sqmeanRef;
    int cn;

    MeanStdDevOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+FIX_GAMMA+SUPPORT_MASK+SCALAR_OUTPUT, 1, 1, Scalar::all(0))
    {
        cn = 0;
        context = 7;
    };
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        dst cvSetMat(1, 2, CC_64FC4);
        meanStdDev(src[0], dst.at<Scalar>(0,0), dst.at<Scalar>(0,1), mask);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        img_t temp;
        cvtest::convert(src[0], temp, CC_64F);
        cvtest::multiply(temp, temp, temp);
        Scalar mean = cvtest::mean(src[0], mask);
        Scalar sqmean = cvtest::mean(temp, mask);

        sqmeanRef = sqmean;
        cn = temp CC_MAT_CN();

        for( int c = 0; c < 4; c++ )
            sqmean[c] = sqrt(MAX(sqmean[c] - mean[c]*mean[c], 0.));

        dst cvSetMat(1, 2, CC_64FC4);
        dst.at<Scalar>(0,0) = mean;
        dst.at<Scalar>(0,1) = sqmean;
    }
    double getMaxErr(int)
    {
        CC_Assert(cn > 0);
        double err = sqmeanRef[0];
        for(int i = 1; i < cn; ++i)
            err = MAX(err, sqmeanRef[i]);
        return 3e-7 * err;
    }
};


struct NormOp : public BaseElemWiseOp
{
    NormOp() : BaseElemWiseOp(2, FIX_ALPHA+FIX_BETA+FIX_GAMMA+SUPPORT_MASK+SCALAR_OUTPUT, 1, 1, Scalar::all(0))
    {
        context = 1;
        normType = 0;
    };
    int getRandomType(RNG& rng)
    {
        int type = cvtest::randomType(rng, _OutputArray::DEPTH_MASK_ALL_BUT_8S, 1, 4);
        for(;;)
        {
            normType = rng.uniform(1, 8);
            if( normType == NORM_INF || normType == NORM_L1 ||
                normType == NORM_L2 || normType == NORM_L2SQR ||
                normType == NORM_HAMMING || normType == NORM_HAMMING2 )
                break;
        }
        if( normType == NORM_HAMMING || normType == NORM_HAMMING2 )
        {
            type = CC_8U;
        }
        return type;
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        dst cvSetMat(1, 2, CC_64FC1);
        dst.at<double>(0,0) = norm(src[0], normType, mask);
        dst.at<double>(0,1) = norm(src[0], src[1], normType, mask);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        dst cvSetMat(1, 2, CC_64FC1);
        dst.at<double>(0,0) = cvtest::norm(src[0], normType, mask);
        dst.at<double>(0,1) = cvtest::norm(src[0], src[1], normType, mask);
    }
    void generateScalars(int, RNG& /*rng*/)
    {
    }
    double getMaxErr(int)
    {
        return 1e-6;
    }
    int normType;
};


struct MinMaxLocOp : public BaseElemWiseOp
{
    MinMaxLocOp() : BaseElemWiseOp(1, FIX_ALPHA+FIX_BETA+FIX_GAMMA+SUPPORT_MASK+SCALAR_OUTPUT, 1, 1, Scalar::all(0))
    {
        context = ARITHM_MAX_NDIMS*2 + 2;
    };
    int getRandomType(RNG& rng)
    {
        return cvtest::randomType(rng, _OutputArray::DEPTH_MASK_ALL_BUT_8S, 1, 1);
    }
    void saveOutput(const vector<int>& minidx, const vector<int>& maxidx,
                    double minval, double maxval, img_t& dst)
    {
        int i, ndims = (int)minidx.size();
        dst cvSetMat(1, ndims*2 + 2, CC_64FC1);

        for( i = 0; i < ndims; i++ )
        {
            dst.at<double>(0,i) = minidx[i];
            dst.at<double>(0,i+ndims) = maxidx[i];
        }
        dst.at<double>(0,ndims*2) = minval;
        dst.at<double>(0,ndims*2+1) = maxval;
    }
    void op(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        int ndims = src[0].dims;
        vector<int> minidx(ndims), maxidx(ndims);
        double minval=0, maxval=0;
        minMaxIdx(src[0], &minval, &maxval, &minidx[0], &maxidx[0], mask);
        saveOutput(minidx, maxidx, minval, maxval, dst);
    }
    void refop(const vector<img_t>& src, img_t& dst, const img_t& mask)
    {
        int ndims=src[0].dims;
        vector<int> minidx(ndims), maxidx(ndims);
        double minval=0, maxval=0;
        cvtest::minMaxLoc(src[0], &minval, &maxval, &minidx, &maxidx, mask);
        saveOutput(minidx, maxidx, minval, maxval, dst);
    }
    double getMaxErr(int)
    {
        return 0;
    }
};


}

typedef Ptr<cvtest::BaseElemWiseOp> ElemWiseOpPtr;
class ElemWiseTest : public ::testing::TestWithParam<ElemWiseOpPtr> {};

TEST_P(ElemWiseTest, accuracy)
{
    ElemWiseOpPtr op = GetParam();

    int testIdx = 0;
    RNG rng((uint64)cvtest::ARITHM_RNG_SEED);
    for( testIdx = 0; testIdx < cvtest::ARITHM_NTESTS; testIdx++ )
    {
        vector<int> size;
        op->getRandomSize(rng, size);
        int type = op->getRandomType(rng);
        int depth = CC_MAT_DEPTH(type);
        bool haveMask = (op->flags & cvtest::BaseElemWiseOp::SUPPORT_MASK) != 0 && rng.uniform(0, 4) == 0;

        double minval=0, maxval=0;
        op->getValueRange(depth, minval, maxval);
        int i, ninputs = op->ninputs;
        vector<img_t> src(ninputs);
        for( i = 0; i < ninputs; i++ )
            src[i] = cvtest::randomMat(rng, size, type, minval, maxval, true);
        img_t dst0, dst, mask;
        if( haveMask )
            mask = cvtest::randomMat(rng, size, CC_8U, 0, 2, true);

        if( (haveMask || ninputs == 0) && !(op->flags & cvtest::BaseElemWiseOp::SCALAR_OUTPUT))
        {
            dst0 = cvtest::randomMat(rng, size, type, minval, maxval, false);
            dst = cvtest::randomMat(rng, size, type, minval, maxval, true);
            cvtest::copy(dst, dst0);
        }
        op->generateScalars(depth, rng);

        op->refop(src, dst0, mask);
        op->op(src, dst, mask);

        double maxErr = op->getMaxErr(depth);
        ASSERT_PRED_FORMAT2(cvtest::MatComparator(maxErr, op->context), dst0, dst) << "\nsrc[0] ~ " <<
            cvtest::MatInfo(!src.empty() ? src[0] : img_t()) << "\ntestCase #" << testIdx << "\n";
    }
}


INSTANTIATE_TEST_CASE_P(Core_Copy, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::CopyOp)));
INSTANTIATE_TEST_CASE_P(Core_Set, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::SetOp)));
INSTANTIATE_TEST_CASE_P(Core_SetZero, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::SetZeroOp)));
INSTANTIATE_TEST_CASE_P(Core_ConvertScale, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::ConvertScaleOp)));
INSTANTIATE_TEST_CASE_P(Core_ConvertScaleFp16, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::ConvertScaleFp16Op)));
INSTANTIATE_TEST_CASE_P(Core_ConvertScaleAbs, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::ConvertScaleAbsOp)));

INSTANTIATE_TEST_CASE_P(Core_Add, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::AddOp)));
INSTANTIATE_TEST_CASE_P(Core_Sub, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::SubOp)));
INSTANTIATE_TEST_CASE_P(Core_AddS, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::AddSOp)));
INSTANTIATE_TEST_CASE_P(Core_SubRS, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::SubRSOp)));
INSTANTIATE_TEST_CASE_P(Core_ScaleAdd, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::ScaleAddOp)));
INSTANTIATE_TEST_CASE_P(Core_AddWeighted, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::AddWeightedOp)));
INSTANTIATE_TEST_CASE_P(Core_AbsDiff, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::AbsDiffOp)));


INSTANTIATE_TEST_CASE_P(Core_AbsDiffS, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::AbsDiffSOp)));

INSTANTIATE_TEST_CASE_P(Core_And, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::LogicOp('&'))));
INSTANTIATE_TEST_CASE_P(Core_AndS, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::LogicSOp('&'))));
INSTANTIATE_TEST_CASE_P(Core_Or, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::LogicOp('|'))));
INSTANTIATE_TEST_CASE_P(Core_OrS, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::LogicSOp('|'))));
INSTANTIATE_TEST_CASE_P(Core_Xor, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::LogicOp('^'))));
INSTANTIATE_TEST_CASE_P(Core_XorS, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::LogicSOp('^'))));
INSTANTIATE_TEST_CASE_P(Core_Not, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::LogicSOp('~'))));

INSTANTIATE_TEST_CASE_P(Core_Max, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::MaxOp)));
INSTANTIATE_TEST_CASE_P(Core_MaxS, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::MaxSOp)));
INSTANTIATE_TEST_CASE_P(Core_Min, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::MinOp)));
INSTANTIATE_TEST_CASE_P(Core_MinS, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::MinSOp)));

INSTANTIATE_TEST_CASE_P(Core_Mul, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::MulOp)));
INSTANTIATE_TEST_CASE_P(Core_Div, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::DivOp)));
INSTANTIATE_TEST_CASE_P(Core_Recip, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::RecipOp)));

INSTANTIATE_TEST_CASE_P(Core_Cmp, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::CmpOp)));
INSTANTIATE_TEST_CASE_P(Core_CmpS, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::CmpSOp)));

INSTANTIATE_TEST_CASE_P(Core_InRangeS, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::InRangeSOp)));
INSTANTIATE_TEST_CASE_P(Core_InRange, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::InRangeOp)));

INSTANTIATE_TEST_CASE_P(Core_Flip, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::FlipOp)));
INSTANTIATE_TEST_CASE_P(Core_Transpose, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::TransposeOp)));
INSTANTIATE_TEST_CASE_P(Core_SetIdentity, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::SetIdentityOp)));

INSTANTIATE_TEST_CASE_P(Core_Exp, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::ExpOp)));
INSTANTIATE_TEST_CASE_P(Core_Log, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::LogOp)));

INSTANTIATE_TEST_CASE_P(Core_CountNonZero, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::CountNonZeroOp)));
INSTANTIATE_TEST_CASE_P(Core_Mean, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::MeanOp)));
INSTANTIATE_TEST_CASE_P(Core_MeanStdDev, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::MeanStdDevOp)));
INSTANTIATE_TEST_CASE_P(Core_Sum, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::SumOp)));
INSTANTIATE_TEST_CASE_P(Core_Norm, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::NormOp)));
INSTANTIATE_TEST_CASE_P(Core_MinMaxLoc, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::MinMaxLocOp)));
INSTANTIATE_TEST_CASE_P(Core_CartToPolarToCart, ElemWiseTest, ::testing::Values(ElemWiseOpPtr(new cvtest::CartToPolarToCartOp)));


TEST(Core_ArithmMask, uninitialized)
{
            RNG& rng = theRNG();
            const int MAX_DIM=3;
            int sizes[MAX_DIM];
            for( int iter = 0; iter < 100; iter++ )
            {
                int dims = rng.uniform(1, MAX_DIM+1);
                int depth = rng.uniform(CC_8U, CC_64F+1);
                int cn = rng.uniform(1, 6);
                int type = CC_MAKETYPE(depth, cn);
                int op = rng.uniform(0, depth < CC_32F ? 5 : 2); // don't run binary operations between floating-point values
                int depth1 = op <= 1 ? CC_64F : depth;
                for (int k = 0; k < MAX_DIM; k++)
                {
                    sizes[k] = k < dims ? rng.uniform(1, 30) : 0;
                }
                SCOPED_TRACE(format("iter=%d dims=%d depth=%d cn=%d type=%d op=%d depth1=%d dims=[%d; %d; %d]",
                                         iter,   dims,   depth,   cn,   type,   op,   depth1, sizes[0], sizes[1], sizes[2]));

                img_t a(dims, sizes, type), a1;
                img_t b(dims, sizes, type), b1;
                img_t mask(dims, sizes, CC_8U);
                img_t mask1;
                img_t c, d;

                rng.fill(a, RNG::UNIFORM, 0, 100);
                rng.fill(b, RNG::UNIFORM, 0, 100);

                // [-2,2) range means that the each generated random number
                // will be one of -2, -1, 0, 1. Saturated to [0,255], it will become
                // 0, 0, 0, 1 => the mask will be filled by ~25%.
                rng.fill(mask, RNG::UNIFORM, -2, 2);

                a.convertTo(a1, depth1);
                b.convertTo(b1, depth1);
                // invert the mask
                compare(mask, 0, mask1, CMP_EQ);
                a1.setTo(0, mask1);
                b1.setTo(0, mask1);

                if( op == 0 )
                {
                    add(a, b, c, mask);
                    add(a1, b1, d);
                }
                else if( op == 1 )
                {
                    subtract(a, b, c, mask);
                    subtract(a1, b1, d);
                }
                else if( op == 2 )
                {
                    bitwise_and(a, b, c, mask);
                    bitwise_and(a1, b1, d);
                }
                else if( op == 3 )
                {
                    bitwise_or(a, b, c, mask);
                    bitwise_or(a1, b1, d);
                }
                else if( op == 4 )
                {
                    bitwise_xor(a, b, c, mask);
                    bitwise_xor(a1, b1, d);
                }
                img_t d1;
                d.convertTo(d1, depth);
                EXPECT_LE(cvtest::norm(c, d1, CC_C), DBL_EPSILON);
            }

            Mat_<uchar> tmpSrc(100,100);
            tmpSrc = 124;
            Mat_<uchar> tmpMask(100,100);
            tmpMask = 255;
            Mat_<uchar> tmpDst(100,100);
            tmpDst = 2;
            tmpSrc.copyTo(tmpDst,tmpMask);
}

TEST(Multiply, FloatingPointRounding)
{
    img_t src(1, 1, CC_8UC1, Scalar::all(110)), dst;
    Scalar s(147.286359696927, 1, 1 ,1);

    multiply(src, s, dst, 1, CC_16U);
    // with CC_32F this produce result 16202
    ASSERT_EQ(dst.at<ushort>(0,0), 16201);
}

TEST(Core_Add, AddToColumnWhen3Rows)
{
    img_t m1 = (Mat_<double>(3, 2) << 1, 2, 3, 4, 5, 6);
    m1.col(1) += 10;

    img_t m2 = (Mat_<double>(3, 2) << 1, 12, 3, 14, 5, 16);

    ASSERT_EQ(0, countNonZero(m1 - m2));
}

TEST(Core_Add, AddToColumnWhen4Rows)
{
    img_t m1 = (Mat_<double>(4, 2) << 1, 2, 3, 4, 5, 6, 7, 8);
    m1.col(1) += 10;

    img_t m2 = (Mat_<double>(4, 2) << 1, 12, 3, 14, 5, 16, 7, 18);

    ASSERT_EQ(0, countNonZero(m1 - m2));
}

TEST(Core_round, CvRound)
{
    ASSERT_EQ(2, cRound(2.0));
    ASSERT_EQ(2, cRound(2.1));
    ASSERT_EQ(-2, cRound(-2.1));
    ASSERT_EQ(3, cRound(2.8));
    ASSERT_EQ(-3, cRound(-2.8));
    ASSERT_EQ(2, cRound(2.5));
    ASSERT_EQ(4, cRound(3.5));
    ASSERT_EQ(-2, cRound(-2.5));
    ASSERT_EQ(-4, cRound(-3.5));
}


typedef testing::TestWithParam<Size> Mul1;

TEST_P(Mul1, One)
{
    Size size = GetParam();
    img_t src(size, CC_32FC1, Scalar::all(2)), dst,
            ref_dst(size, CC_32FC1, Scalar::all(6));

    multiply(3, src, dst);

    ASSERT_EQ(0, cvtest::norm(dst, ref_dst, NORM_INF));
}

INSTANTIATE_TEST_CASE_P(Arithm, Mul1, testing::Values(Size(2, 2), Size(1, 1)));

class SubtractOutputMatNotEmpty : public testing::TestWithParam< std::tr1::tuple<Size, perf::MatType, perf::MatDepth, bool> >
{
public:
    Size size;
    int src_type;
    int dst_depth;
    bool fixed;

    void SetUp()
    {
        size = std::tr1::get<0>(GetParam());
        src_type = std::tr1::get<1>(GetParam());
        dst_depth = std::tr1::get<2>(GetParam());
        fixed = std::tr1::get<3>(GetParam());
    }
};

TEST_P(SubtractOutputMatNotEmpty, Mat_Mat)
{
    img_t src1(size, src_type, Scalar::all(16));
    img_t src2(size, src_type, Scalar::all(16));

    img_t dst;

    if (!fixed)
    {
        subtract(src1, src2, dst, NULL, dst_depth);
    }
    else
    {
        const img_t fixed_dst(size, CC_MAKE_TYPE((dst_depth > 0 ? dst_depth : CC_16S), src1 CC_MAT_CN()));
        subtract(src1, src2, fixed_dst, NULL, dst_depth);
        dst = fixed_dst;
        dst_depth = fixed_dst CC_MAT_DEPTH();
    }

    ASSERT_FALSE(dst.empty());
    ASSERT_EQ(src1.size(), dst.size());
    ASSERT_EQ(dst_depth > 0 ? dst_depth : src1 CC_MAT_DEPTH(), dst CC_MAT_DEPTH());
    ASSERT_EQ(0, countNonZero(dst cvReshape(,1)));
}

TEST_P(SubtractOutputMatNotEmpty, Mat_Mat_WithMask)
{
    img_t src1(size, src_type, Scalar::all(16));
    img_t src2(size, src_type, Scalar::all(16));
    img_t mask(size, CC_8UC1, Scalar::all(255));

    img_t dst;

    if (!fixed)
    {
        subtract(src1, src2, dst, mask, dst_depth);
    }
    else
    {
        const img_t fixed_dst(size, CC_MAKE_TYPE((dst_depth > 0 ? dst_depth : CC_16S), src1 CC_MAT_CN()));
        subtract(src1, src2, fixed_dst, mask, dst_depth);
        dst = fixed_dst;
        dst_depth = fixed_dst CC_MAT_DEPTH();
    }

    ASSERT_FALSE(dst.empty());
    ASSERT_EQ(src1.size(), dst.size());
    ASSERT_EQ(dst_depth > 0 ? dst_depth : src1 CC_MAT_DEPTH(), dst CC_MAT_DEPTH());
    ASSERT_EQ(0, countNonZero(dst cvReshape(,1)));
}

TEST_P(SubtractOutputMatNotEmpty, Mat_Mat_Expr)
{
    img_t src1(size, src_type, Scalar::all(16));
    img_t src2(size, src_type, Scalar::all(16));

    img_t dst = src1 - src2;

    ASSERT_FALSE(dst.empty());
    ASSERT_EQ(src1.size(), dst.size());
    ASSERT_EQ(src1 CC_MAT_DEPTH(), dst CC_MAT_DEPTH());
    ASSERT_EQ(0, countNonZero(dst cvReshape(,1)));
}

TEST_P(SubtractOutputMatNotEmpty, Mat_Scalar)
{
    img_t src(size, src_type, Scalar::all(16));

    img_t dst;

    if (!fixed)
    {
        subtract(src, Scalar::all(16), dst, NULL, dst_depth);
    }
    else
    {
        const img_t fixed_dst(size, CC_MAKE_TYPE((dst_depth > 0 ? dst_depth : CC_16S), src CC_MAT_CN()));
        subtract(src, Scalar::all(16), fixed_dst, NULL, dst_depth);
        dst = fixed_dst;
        dst_depth = fixed_dst CC_MAT_DEPTH();
    }

    ASSERT_FALSE(dst.empty());
    ASSERT_EQ(cvGetSize(src), dst.size());
    ASSERT_EQ(dst_depth > 0 ? dst_depth : src CC_MAT_DEPTH(), dst CC_MAT_DEPTH());
    ASSERT_EQ(0, countNonZero(dst cvReshape(,1)));
}

TEST_P(SubtractOutputMatNotEmpty, Mat_Scalar_WithMask)
{
    img_t src(size, src_type, Scalar::all(16));
    img_t mask(size, CC_8UC1, Scalar::all(255));

    img_t dst;

    if (!fixed)
    {
        subtract(src, Scalar::all(16), dst, mask, dst_depth);
    }
    else
    {
        const img_t fixed_dst(size, CC_MAKE_TYPE((dst_depth > 0 ? dst_depth : CC_16S), src CC_MAT_CN()));
        subtract(src, Scalar::all(16), fixed_dst, mask, dst_depth);
        dst = fixed_dst;
        dst_depth = fixed_dst CC_MAT_DEPTH();
    }

    ASSERT_FALSE(dst.empty());
    ASSERT_EQ(cvGetSize(src), dst.size());
    ASSERT_EQ(dst_depth > 0 ? dst_depth : src CC_MAT_DEPTH(), dst CC_MAT_DEPTH());
    ASSERT_EQ(0, countNonZero(dst cvReshape(,1)));
}

TEST_P(SubtractOutputMatNotEmpty, Scalar_Mat)
{
    img_t src(size, src_type, Scalar::all(16));

    img_t dst;

    if (!fixed)
    {
        subtract(Scalar::all(16), src, dst, NULL, dst_depth);
    }
    else
    {
        const img_t fixed_dst(size, CC_MAKE_TYPE((dst_depth > 0 ? dst_depth : CC_16S), src CC_MAT_CN()));
        subtract(Scalar::all(16), src, fixed_dst, NULL, dst_depth);
        dst = fixed_dst;
        dst_depth = fixed_dst CC_MAT_DEPTH();
    }

    ASSERT_FALSE(dst.empty());
    ASSERT_EQ(cvGetSize(src), dst.size());
    ASSERT_EQ(dst_depth > 0 ? dst_depth : src CC_MAT_DEPTH(), dst CC_MAT_DEPTH());
    ASSERT_EQ(0, countNonZero(dst cvReshape(,1)));
}

TEST_P(SubtractOutputMatNotEmpty, Scalar_Mat_WithMask)
{
    img_t src(size, src_type, Scalar::all(16));
    img_t mask(size, CC_8UC1, Scalar::all(255));

    img_t dst;

    if (!fixed)
    {
        subtract(Scalar::all(16), src, dst, mask, dst_depth);
    }
    else
    {
        const img_t fixed_dst(size, CC_MAKE_TYPE((dst_depth > 0 ? dst_depth : CC_16S), src CC_MAT_CN()));
        subtract(Scalar::all(16), src, fixed_dst, mask, dst_depth);
        dst = fixed_dst;
        dst_depth = fixed_dst CC_MAT_DEPTH();
    }

    ASSERT_FALSE(dst.empty());
    ASSERT_EQ(cvGetSize(src), dst.size());
    ASSERT_EQ(dst_depth > 0 ? dst_depth : src CC_MAT_DEPTH(), dst CC_MAT_DEPTH());
    ASSERT_EQ(0, countNonZero(dst cvReshape(,1)));
}

TEST_P(SubtractOutputMatNotEmpty, Mat_Mat_3d)
{
    int dims[] = {5, size.height, size.width};

    img_t src1(3, dims, src_type, Scalar::all(16));
    img_t src2(3, dims, src_type, Scalar::all(16));

    img_t dst;

    if (!fixed)
    {
        subtract(src1, src2, dst, NULL, dst_depth);
    }
    else
    {
        const img_t fixed_dst(3, dims, CC_MAKE_TYPE((dst_depth > 0 ? dst_depth : CC_16S), src1 CC_MAT_CN()));
        subtract(src1, src2, fixed_dst, NULL, dst_depth);
        dst = fixed_dst;
        dst_depth = fixed_dst CC_MAT_DEPTH();
    }

    ASSERT_FALSE(dst.empty());
    ASSERT_EQ(src1.dims, dst.dims);
    ASSERT_EQ(src1.size, dst.size);
    ASSERT_EQ(dst_depth > 0 ? dst_depth : src1 CC_MAT_DEPTH(), dst CC_MAT_DEPTH());
    ASSERT_EQ(0, countNonZero(dst cvReshape(,1)));
}

INSTANTIATE_TEST_CASE_P(Arithm, SubtractOutputMatNotEmpty, testing::Combine(
    testing::Values(Size(16, 16), Size(13, 13), Size(16, 13), Size(13, 16)),
    testing::Values(perf::MatType(CC_8UC1), CC_8UC3, CC_8UC4, CC_16SC1, CC_16SC3),
    testing::Values(-1, CC_16S, CC_32S, CC_32F),
    testing::Bool()));

TEST(Core_FindNonZero, singular)
{
    img_t img(10, 10, CC_8U, Scalar::all(0));
    vector<Point> pts, pts2(10);
    findNonZero(img, pts);
    findNonZero(img, pts2);
    ASSERT_TRUE(pts.empty() && pts2.empty());
}

TEST(Core_BoolVector, support)
{
    std::vector<bool> test;
    int i, n = 205;
    int nz = 0;
    test.resize(n);
    for( i = 0; i < n; i++ )
    {
        test[i] = theRNG().uniform(0, 2) != 0;
        nz += (int)test[i];
    }
    ASSERT_EQ( nz, countNonZero(test) );
    ASSERT_FLOAT_EQ((float)nz/n, (float)(mean(test)[0]));
}

TEST(MinMaxLoc, Mat_IntMax_Without_Mask)
{
    Mat_<int> mat(50, 50);
    int iMaxVal = numeric_limits<int>::max();
    mat.setTo(iMaxVal);

    double min, max;
    Point minLoc, maxLoc;

    minMaxLoc(mat, &min, &max, &minLoc, &maxLoc, img_t());

    ASSERT_EQ(iMaxVal, min);
    ASSERT_EQ(iMaxVal, max);

    ASSERT_EQ(Point(0, 0), minLoc);
    ASSERT_EQ(Point(0, 0), maxLoc);
}

TEST(Normalize, regression_5876_inplace_change_type)
{
    double initial_values[] = {1, 2, 5, 4, 3};
    float result_values[] = {0, 0.25, 1, 0.75, 0.5};
    img_t m(Size(5, 1), CC_64FC1, initial_values);
    img_t result(Size(5, 1), CC_32FC1, result_values);

    normalize(m, m, 1, 0, NORM_MINMAX, CC_32F);
    EXPECT_EQ(0, cvtest::norm(m, result, NORM_INF));
}

TEST(MinMaxLoc, regression_4955_nans)
{
    img_t one_mat(2, 2, CC_32F, Scalar(1));
    minMaxLoc(one_mat, NULL, NULL, NULL, NULL);

    img_t nan_mat(2, 2, CC_32F, Scalar(std::numeric_limits<float>::quiet_NaN()));
    minMaxLoc(nan_mat, NULL, NULL, NULL, NULL);
}

TEST(Subtract, scalarc1_matc3)
{
    int scalar = 255;
    img_t srcImage(5, 5, CC_8UC3, Scalar::all(5)), destImage;
    subtract(scalar, srcImage, destImage);

    ASSERT_EQ(0, norm(img_t(5, 5, CC_8UC3, Scalar::all(250)), destImage, NORM_INF));
}

TEST(Subtract, scalarc4_matc4)
{
    Scalar sc(255, 255, 255, 255);
    img_t srcImage(5, 5, CC_8UC4, Scalar::all(5)), destImage;
    subtract(sc, srcImage, destImage);

    ASSERT_EQ(0, norm(img_t(5, 5, CC_8UC4, Scalar::all(250)), destImage, NORM_INF));
}
