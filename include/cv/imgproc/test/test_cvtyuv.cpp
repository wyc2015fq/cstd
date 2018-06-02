#include "test_precomp.hpp"

using namespace cv;
using namespace std;

#undef RGB
#undef YUV

typedef Vec3b YUV;
typedef Vec3b RGB;

int countOfDifferencies(const CvMat& gold, const CvMat& result, int maxAllowedDifference = 1)
{
    CvMat diff;
    absdiff(gold, result, diff);
    return countNonZero(diff.reshape(1) > maxAllowedDifference);
}

class YUVreader
{
public:
    virtual ~YUVreader() {}
    virtual YUV read(const CvMat& yuv, int row, int col) = 0;
    virtual int channels() = 0;
    virtual CvSize size(CvSize imgSize) = 0;

    virtual bool requiresEvenHeight() { return true; }
    virtual bool requiresEvenWidth() { return true; }

    static YUVreader* getReader(int code);
};

class RGBreader
{
public:
    virtual ~RGBreader() {}
    virtual RGB read(const CvMat& rgb, int row, int col) = 0;
    virtual int channels() = 0;

    static RGBreader* getReader(int code);
};

class RGBwriter
{
public:
    virtual ~RGBwriter() {}

    virtual void write(CvMat& rgb, int row, int col, const RGB& val) = 0;
    virtual int channels() = 0;

    static RGBwriter* getWriter(int code);
};

class GRAYwriter
{
public:
    virtual ~GRAYwriter() {}

    virtual void write(CvMat& gray, int row, int col, const uchar& val)
    {
        gray.at<uchar>(row, col) = val;
    }

    virtual int channels() { return 1; }

    static GRAYwriter* getWriter(int code);
};

class YUVwriter
{
public:
    virtual ~YUVwriter() {}

    virtual void write(CvMat& yuv, int row, int col, const YUV& val) = 0;
    virtual int channels() = 0;
    virtual CvSize size(CvSize imgSize) = 0;

    virtual bool requiresEvenHeight() { return true; }
    virtual bool requiresEvenWidth() { return true; }

    static YUVwriter* getWriter(int code);
};

class RGB888Writer : public RGBwriter
{
    void write(CvMat& rgb, int row, int col, const RGB& val)
    {
        rgb.at<Vec3b>(row, col) = val;
    }

    int channels() { return 3; }
};

class BGR888Writer : public RGBwriter
{
    void write(CvMat& rgb, int row, int col, const RGB& val)
    {
        Vec3b tmp(val[2], val[1], val[0]);
        rgb.at<Vec3b>(row, col) = tmp;
    }

    int channels() { return 3; }
};

class RGBA8888Writer : public RGBwriter
{
    void write(CvMat& rgb, int row, int col, const RGB& val)
    {
        Vec4b tmp(val[0], val[1], val[2], 255);
        rgb.at<Vec4b>(row, col) = tmp;
    }

    int channels() { return 4; }
};

class BGRA8888Writer : public RGBwriter
{
    void write(CvMat& rgb, int row, int col, const RGB& val)
    {
        Vec4b tmp(val[2], val[1], val[0], 255);
        rgb.at<Vec4b>(row, col) = tmp;
    }

    int channels() { return 4; }
};

class YUV420pWriter: public YUVwriter
{
    int channels() { return 1; }
    CvSize size(CvSize imgSize) { return CvSize(imgSize.width, imgSize.height + imgSize.height/2); }
};

class YV12Writer: public YUV420pWriter
{
    void write(CvMat& yuv, int row, int col, const YUV& val)
    {
        int h = yuv.rows * 2 / 3;

        yuv.ptr<uchar>(row)[col] = val[0];
        if( row % 2 == 0 && col % 2 == 0 )
        {
            yuv.ptr<uchar>(h + row/4)[col/2 + ((row/2) % 2) * (yuv.cols/2)] = val[2];
            yuv.ptr<uchar>(h + (row/2 + h/2)/2)[col/2 + ((row/2 + h/2) % 2) * (yuv.cols/2)] = val[1];
        }
    }
};

class I420Writer: public YUV420pWriter
{
    void write(CvMat& yuv, int row, int col, const YUV& val)
    {
        int h = yuv.rows * 2 / 3;

        yuv.ptr<uchar>(row)[col] = val[0];
        if( row % 2 == 0 && col % 2 == 0 )
        {
            yuv.ptr<uchar>(h + row/4)[col/2 + ((row/2) % 2) * (yuv.cols/2)] = val[1];
            yuv.ptr<uchar>(h + (row/2 + h/2)/2)[col/2 + ((row/2 + h/2) % 2) * (yuv.cols/2)] = val[2];
        }
    }
};

class YUV420Reader: public YUVreader
{
    int channels() { return 1; }
    CvSize size(CvSize imgSize) { return CvSize(imgSize.width, imgSize.height * 3 / 2); }
};

class YUV422Reader: public YUVreader
{
    int channels() { return 2; }
    CvSize size(CvSize imgSize) { return imgSize; }
    bool requiresEvenHeight() { return false; }
};

class NV21Reader: public YUV420Reader
{
    YUV read(const CvMat& yuv, int row, int col)
    {
        uchar y = yuv.ptr<uchar>(row)[col];
        uchar u = yuv.ptr<uchar>(yuv.rows * 2 / 3 + row/2)[(col/2)*2 + 1];
        uchar v = yuv.ptr<uchar>(yuv.rows * 2 / 3 + row/2)[(col/2)*2];

        return YUV(y, u, v);
    }
};


struct NV12Reader: public YUV420Reader
{
    YUV read(const CvMat& yuv, int row, int col)
    {
        uchar y = yuv.ptr<uchar>(row)[col];
        uchar u = yuv.ptr<uchar>(yuv.rows * 2 / 3 + row/2)[(col/2)*2];
        uchar v = yuv.ptr<uchar>(yuv.rows * 2 / 3 + row/2)[(col/2)*2 + 1];

        return YUV(y, u, v);
    }
};

class YV12Reader: public YUV420Reader
{
    YUV read(const CvMat& yuv, int row, int col)
    {
        int h = yuv.rows * 2 / 3;
        uchar y = yuv.ptr<uchar>(row)[col];
        uchar u = yuv.ptr<uchar>(h + (row/2 + h/2)/2)[col/2 + ((row/2 + h/2) % 2) * (yuv.cols/2)];
        uchar v = yuv.ptr<uchar>(h + row/4)[col/2 + ((row/2) % 2) * (yuv.cols/2)];

        return YUV(y, u, v);
    }
};

class IYUVReader: public YUV420Reader
{
    YUV read(const CvMat& yuv, int row, int col)
    {
        int h = yuv.rows * 2 / 3;
        uchar y = yuv.ptr<uchar>(row)[col];
        uchar u = yuv.ptr<uchar>(h + row/4)[col/2 + ((row/2) % 2) * (yuv.cols/2)];
        uchar v = yuv.ptr<uchar>(h + (row/2 + h/2)/2)[col/2 + ((row/2 + h/2) % 2) * (yuv.cols/2)];

        return YUV(y, u, v);
    }
};

class UYVYReader: public YUV422Reader
{
    YUV read(const CvMat& yuv, int row, int col)
    {
        uchar y = yuv.ptr<Vec2b>(row)[col][1];
        uchar u = yuv.ptr<Vec2b>(row)[(col/2)*2][0];
        uchar v = yuv.ptr<Vec2b>(row)[(col/2)*2 + 1][0];

        return YUV(y, u, v);
    }
};

class YUY2Reader: public YUV422Reader
{
    YUV read(const CvMat& yuv, int row, int col)
    {
        uchar y = yuv.ptr<Vec2b>(row)[col][0];
        uchar u = yuv.ptr<Vec2b>(row)[(col/2)*2][1];
        uchar v = yuv.ptr<Vec2b>(row)[(col/2)*2 + 1][1];

        return YUV(y, u, v);
    }
};

class YVYUReader: public YUV422Reader
{
    YUV read(const CvMat& yuv, int row, int col)
    {
        uchar y = yuv.ptr<Vec2b>(row)[col][0];
        uchar u = yuv.ptr<Vec2b>(row)[(col/2)*2 + 1][1];
        uchar v = yuv.ptr<Vec2b>(row)[(col/2)*2][1];

        return YUV(y, u, v);
    }
};

class YUV888Reader : public YUVreader
{
    YUV read(const CvMat& yuv, int row, int col)
    {
        return yuv.at<YUV>(row, col);
    }

    int channels() { return 3; }
    CvSize size(CvSize imgSize) { return imgSize; }
    bool requiresEvenHeight() { return false; }
    bool requiresEvenWidth() { return false; }
};

class RGB888Reader : public RGBreader
{
    RGB read(const CvMat& rgb, int row, int col)
    {
        return rgb.at<RGB>(row, col);
    }

    int channels() { return 3; }
};

class BGR888Reader : public RGBreader
{
    RGB read(const CvMat& rgb, int row, int col)
    {
        RGB tmp = rgb.at<RGB>(row, col);
        return RGB(tmp[2], tmp[1], tmp[0]);
    }

    int channels() { return 3; }
};

class RGBA8888Reader : public RGBreader
{
    RGB read(const CvMat& rgb, int row, int col)
    {
        Vec4b rgba = rgb.at<Vec4b>(row, col);
        return RGB(rgba[0], rgba[1], rgba[2]);
    }

    int channels() { return 4; }
};

class BGRA8888Reader : public RGBreader
{
    RGB read(const CvMat& rgb, int row, int col)
    {
        Vec4b rgba = rgb.at<Vec4b>(row, col);
        return RGB(rgba[2], rgba[1], rgba[0]);
    }

    int channels() { return 4; }
};

class YUV2RGB_Converter
{
public:
    RGB convert(YUV yuv)
    {
        int y = MAX(0, yuv[0] - 16);
        int u = yuv[1] - 128;
        int v = yuv[2] - 128;
        uchar r = saturate_cast<uchar>(1.164f * y + 1.596f * v);
        uchar g = saturate_cast<uchar>(1.164f * y - 0.813f * v - 0.391f * u);
        uchar b = saturate_cast<uchar>(1.164f * y + 2.018f * u);

        return RGB(r, g, b);
    }
};

class YUV2GRAY_Converter
{
public:
    uchar convert(YUV yuv)
    {
        return yuv[0];
    }
};

class RGB2YUV_Converter
{
public:
    YUV convert(RGB rgb)
    {
        int r = rgb[0];
        int g = rgb[1];
        int b = rgb[2];

        uchar y = saturate_cast<uchar>((int)( 0.257f*r + 0.504f*g + 0.098f*b + 0.5f) + 16);
        uchar u = saturate_cast<uchar>((int)(-0.148f*r - 0.291f*g + 0.439f*b + 0.5f) + 128);
        uchar v = saturate_cast<uchar>((int)( 0.439f*r - 0.368f*g - 0.071f*b + 0.5f) + 128);

        return YUV(y, u, v);
    }
};

YUVreader* YUVreader::getReader(int code)
{
    switch(code)
    {
    case CC_YUV2RGB_NV12:
    case CC_YUV2BGR_NV12:
    case CC_YUV2RGBA_NV12:
    case CC_YUV2BGRA_NV12:
        return new NV12Reader();
    case CC_YUV2RGB_NV21:
    case CC_YUV2BGR_NV21:
    case CC_YUV2RGBA_NV21:
    case CC_YUV2BGRA_NV21:
        return new NV21Reader();
    case CC_YUV2RGB_YV12:
    case CC_YUV2BGR_YV12:
    case CC_YUV2RGBA_YV12:
    case CC_YUV2BGRA_YV12:
        return new YV12Reader();
    case CC_YUV2RGB_IYUV:
    case CC_YUV2BGR_IYUV:
    case CC_YUV2RGBA_IYUV:
    case CC_YUV2BGRA_IYUV:
        return new IYUVReader();
    case CC_YUV2RGB_UYVY:
    case CC_YUV2BGR_UYVY:
    case CC_YUV2RGBA_UYVY:
    case CC_YUV2BGRA_UYVY:
        return new UYVYReader();
    //case CC_YUV2RGB_VYUY = 109,
    //case CC_YUV2BGR_VYUY = 110,
    //case CC_YUV2RGBA_VYUY = 113,
    //case CC_YUV2BGRA_VYUY = 114,
    //    return ??
    case CC_YUV2RGB_YUY2:
    case CC_YUV2BGR_YUY2:
    case CC_YUV2RGBA_YUY2:
    case CC_YUV2BGRA_YUY2:
        return new YUY2Reader();
    case CC_YUV2RGB_YVYU:
    case CC_YUV2BGR_YVYU:
    case CC_YUV2RGBA_YVYU:
    case CC_YUV2BGRA_YVYU:
        return new YVYUReader();
    case CC_YUV2GRAY_420:
        return new NV21Reader();
    case CC_YUV2GRAY_UYVY:
        return new UYVYReader();
    case CC_YUV2GRAY_YUY2:
        return new YUY2Reader();
    case CC_YUV2BGR:
    case CC_YUV2RGB:
        return new YUV888Reader();
    default:
        return 0;
    }
}

RGBreader* RGBreader::getReader(int code)
{
    switch(code)
    {
    case CC_RGB2YUV_YV12:
    case CC_RGB2YUV_I420:
        return new RGB888Reader();
    case CC_BGR2YUV_YV12:
    case CC_BGR2YUV_I420:
        return new BGR888Reader();
    case CC_RGBA2YUV_I420:
    case CC_RGBA2YUV_YV12:
        return new RGBA8888Reader();
    case CC_BGRA2YUV_YV12:
    case CC_BGRA2YUV_I420:
        return new BGRA8888Reader();
    default:
        return 0;
    };
}

RGBwriter* RGBwriter::getWriter(int code)
{
    switch(code)
    {
    case CC_YUV2RGB_NV12:
    case CC_YUV2RGB_NV21:
    case CC_YUV2RGB_YV12:
    case CC_YUV2RGB_IYUV:
    case CC_YUV2RGB_UYVY:
    //case CC_YUV2RGB_VYUY:
    case CC_YUV2RGB_YUY2:
    case CC_YUV2RGB_YVYU:
    case CC_YUV2RGB:
        return new RGB888Writer();
    case CC_YUV2BGR_NV12:
    case CC_YUV2BGR_NV21:
    case CC_YUV2BGR_YV12:
    case CC_YUV2BGR_IYUV:
    case CC_YUV2BGR_UYVY:
    //case CC_YUV2BGR_VYUY:
    case CC_YUV2BGR_YUY2:
    case CC_YUV2BGR_YVYU:
    case CC_YUV2BGR:
        return new BGR888Writer();
    case CC_YUV2RGBA_NV12:
    case CC_YUV2RGBA_NV21:
    case CC_YUV2RGBA_YV12:
    case CC_YUV2RGBA_IYUV:
    case CC_YUV2RGBA_UYVY:
    //case CC_YUV2RGBA_VYUY:
    case CC_YUV2RGBA_YUY2:
    case CC_YUV2RGBA_YVYU:
        return new RGBA8888Writer();
    case CC_YUV2BGRA_NV12:
    case CC_YUV2BGRA_NV21:
    case CC_YUV2BGRA_YV12:
    case CC_YUV2BGRA_IYUV:
    case CC_YUV2BGRA_UYVY:
    //case CC_YUV2BGRA_VYUY:
    case CC_YUV2BGRA_YUY2:
    case CC_YUV2BGRA_YVYU:
        return new BGRA8888Writer();
    default:
        return 0;
    };
}

GRAYwriter* GRAYwriter::getWriter(int code)
{
    switch(code)
    {
    case CC_YUV2GRAY_420:
    case CC_YUV2GRAY_UYVY:
    case CC_YUV2GRAY_YUY2:
        return new GRAYwriter();
    default:
        return 0;
    }
}

YUVwriter* YUVwriter::getWriter(int code)
{
    switch(code)
    {
    case CC_RGB2YUV_YV12:
    case CC_BGR2YUV_YV12:
    case CC_RGBA2YUV_YV12:
    case CC_BGRA2YUV_YV12:
        return new YV12Writer();
    case CC_RGB2YUV_I420:
    case CC_BGR2YUV_I420:
    case CC_RGBA2YUV_I420:
    case CC_BGRA2YUV_I420:
        return new I420Writer();
    default:
        return 0;
    };
}

template<class convertor>
void referenceYUV2RGB(const CvMat& yuv, CvMat& rgb, YUVreader* yuvReader, RGBwriter* rgbWriter)
{
    convertor cvt;

    for(int row = 0; row < rgb.rows; ++row)
        for(int col = 0; col < rgb.cols; ++col)
            rgbWriter->write(rgb, row, col, cvt.convert(yuvReader->read(yuv, row, col)));
}

template<class convertor>
void referenceYUV2GRAY(const CvMat& yuv, CvMat& rgb, YUVreader* yuvReader, GRAYwriter* grayWriter)
{
    convertor cvt;

    for(int row = 0; row < rgb.rows; ++row)
        for(int col = 0; col < rgb.cols; ++col)
            grayWriter->write(rgb, row, col, cvt.convert(yuvReader->read(yuv, row, col)));
}

template<class convertor>
void referenceRGB2YUV(const CvMat& rgb, CvMat& yuv, RGBreader* rgbReader, YUVwriter* yuvWriter)
{
    convertor cvt;

    for(int row = 0; row < rgb.rows; ++row)
        for(int col = 0; col < rgb.cols; ++col)
            yuvWriter->write(yuv, row, col, cvt.convert(rgbReader->read(rgb, row, col)));
}

struct ConversionYUV
{
    explicit ConversionYUV( const int code )
    {
        yuvReader_  = YUVreader :: getReader(code);
        yuvWriter_  = YUVwriter :: getWriter(code);
        rgbReader_  = RGBreader :: getReader(code);
        rgbWriter_  = RGBwriter :: getWriter(code);
        grayWriter_ = GRAYwriter:: getWriter(code);
    }

    ~ConversionYUV()
    {
        if (yuvReader_)
            delete yuvReader_;

        if (yuvWriter_)
            delete yuvWriter_;

        if (rgbReader_)
            delete rgbReader_;

        if (rgbWriter_)
            delete rgbWriter_;

        if (grayWriter_)
            delete grayWriter_;
    }

    int getDcn()
    {
        return (rgbWriter_ != 0) ? rgbWriter_->channels() : ((grayWriter_ != 0) ? grayWriter_->channels() : yuvWriter_->channels());
    }

    int getScn()
    {
        return (yuvReader_ != 0) ? yuvReader_->channels() : rgbReader_->channels();
    }

    CvSize getSrcSize( const CvSize& imgSize )
    {
        return (yuvReader_ != 0) ? yuvReader_->size(imgSize) : imgSize;
    }

    CvSize getDstSize( const CvSize& imgSize )
    {
        return (yuvWriter_ != 0) ? yuvWriter_->size(imgSize) : imgSize;
    }

    bool requiresEvenHeight()
    {
        return (yuvReader_ != 0) ? yuvReader_->requiresEvenHeight() : ((yuvWriter_ != 0) ? yuvWriter_->requiresEvenHeight() : false);
    }

    bool requiresEvenWidth()
    {
        return (yuvReader_ != 0) ? yuvReader_->requiresEvenWidth() : ((yuvWriter_ != 0) ? yuvWriter_->requiresEvenWidth() : false);
    }

    YUVreader*  yuvReader_;
    YUVwriter*  yuvWriter_;
    RGBreader*  rgbReader_;
    RGBwriter*  rgbWriter_;
    GRAYwriter* grayWriter_;
};

CC_ENUM(YUVCVTS, CC_YUV2RGB_NV12, CC_YUV2BGR_NV12, CC_YUV2RGB_NV21, CC_YUV2BGR_NV21,
                 CC_YUV2RGBA_NV12, CC_YUV2BGRA_NV12, CC_YUV2RGBA_NV21, CC_YUV2BGRA_NV21,
                 CC_YUV2RGB_YV12, CC_YUV2BGR_YV12, CC_YUV2RGB_IYUV, CC_YUV2BGR_IYUV,
                 CC_YUV2RGBA_YV12, CC_YUV2BGRA_YV12, CC_YUV2RGBA_IYUV, CC_YUV2BGRA_IYUV,
                 CC_YUV2RGB_UYVY, CC_YUV2BGR_UYVY, CC_YUV2RGBA_UYVY, CC_YUV2BGRA_UYVY,
                 CC_YUV2RGB_YUY2, CC_YUV2BGR_YUY2, CC_YUV2RGB_YVYU, CC_YUV2BGR_YVYU,
                 CC_YUV2RGBA_YUY2, CC_YUV2BGRA_YUY2, CC_YUV2RGBA_YVYU, CC_YUV2BGRA_YVYU,
                 CC_YUV2GRAY_420, CC_YUV2GRAY_UYVY, CC_YUV2GRAY_YUY2,
                 CC_YUV2BGR, CC_YUV2RGB, CC_RGB2YUV_YV12, CC_BGR2YUV_YV12, CC_RGBA2YUV_YV12,
                 CC_BGRA2YUV_YV12, CC_RGB2YUV_I420, CC_BGR2YUV_I420, CC_RGBA2YUV_I420, CC_BGRA2YUV_I420)

typedef ::testing::TestWithParam<YUVCVTS> Imgproc_ColorYUV;

TEST_P(Imgproc_ColorYUV, accuracy)
{
    int code = GetParam();
    RNG& random = theRNG();

    ConversionYUV cvt(code);

    const int scn = cvt.getScn();
    const int dcn = cvt.getDcn();
    for(int iter = 0; iter < 30; ++iter)
    {
        CvSize sz(random.uniform(1, 641), random.uniform(1, 481));

        if(cvt.requiresEvenWidth())  sz.width  += sz.width % 2;
        if(cvt.requiresEvenHeight()) sz.height += sz.height % 2;

        CvSize srcSize = cvt.getSrcSize(sz);
        CvMat src = CvMat(srcSize.height, srcSize.width * scn, CC_8UC1).reshape(scn);

        CvSize dstSize = cvt.getDstSize(sz);
        CvMat dst = CvMat(dstSize.height, dstSize.width * dcn, CC_8UC1).reshape(dcn);
        CvMat gold(dstSize, CC_8UC(dcn));

        random.fill(src, RNG::UNIFORM, 0, 256);

        if(cvt.rgbWriter_)
            referenceYUV2RGB<YUV2RGB_Converter>  (src, gold, cvt.yuvReader_, cvt.rgbWriter_);
        else if(cvt.grayWriter_)
            referenceYUV2GRAY<YUV2GRAY_Converter>(src, gold, cvt.yuvReader_, cvt.grayWriter_);
        else if(cvt.yuvWriter_)
            referenceRGB2YUV<RGB2YUV_Converter>  (src, gold, cvt.rgbReader_, cvt.yuvWriter_);

        cvtColor(src, dst, code, -1);

        EXPECT_EQ(0, countOfDifferencies(gold, dst));
    }
}

TEST_P(Imgproc_ColorYUV, roi_accuracy)
{
    int code = GetParam();
    RNG& random = theRNG();

    ConversionYUV cvt(code);

    const int scn = cvt.getScn();
    const int dcn = cvt.getDcn();
    for(int iter = 0; iter < 30; ++iter)
    {
        CvSize sz(random.uniform(1, 641), random.uniform(1, 481));

        if(cvt.requiresEvenWidth())  sz.width  += sz.width % 2;
        if(cvt.requiresEvenHeight()) sz.height += sz.height % 2;

        int roi_offset_top = random.uniform(0, 6);
        int roi_offset_bottom = random.uniform(0, 6);
        int roi_offset_left = random.uniform(0, 6);
        int roi_offset_right = random.uniform(0, 6);

        CvSize srcSize = cvt.getSrcSize(sz);
        CvMat src_full(srcSize.height + roi_offset_top + roi_offset_bottom, srcSize.width + roi_offset_left + roi_offset_right, CC_8UC(scn));

        CvSize dstSize = cvt.getDstSize(sz);
        CvMat dst_full(dstSize.height  + roi_offset_left + roi_offset_right, dstSize.width + roi_offset_top + roi_offset_bottom, CC_8UC(dcn), Scalar::all(0));
        CvMat gold_full(dst_full.size(), CC_8UC(dcn), Scalar::all(0));

        random.fill(src_full, RNG::UNIFORM, 0, 256);

        CvMat src = src_full(Range(roi_offset_top, roi_offset_top + srcSize.height), Range(roi_offset_left, roi_offset_left + srcSize.width));
        CvMat dst = dst_full(Range(roi_offset_left, roi_offset_left + dstSize.height), Range(roi_offset_top, roi_offset_top + dstSize.width));
        CvMat gold = gold_full(Range(roi_offset_left, roi_offset_left + dstSize.height), Range(roi_offset_top, roi_offset_top + dstSize.width));

        if(cvt.rgbWriter_)
            referenceYUV2RGB<YUV2RGB_Converter>  (src, gold, cvt.yuvReader_, cvt.rgbWriter_);
        else if(cvt.grayWriter_)
            referenceYUV2GRAY<YUV2GRAY_Converter>(src, gold, cvt.yuvReader_, cvt.grayWriter_);
        else if(cvt.yuvWriter_)
            referenceRGB2YUV<RGB2YUV_Converter>  (src, gold, cvt.rgbReader_, cvt.yuvWriter_);

        cvtColor(src, dst, code, -1);

        EXPECT_EQ(0, countOfDifferencies(gold_full, dst_full));
    }
}

INSTANTIATE_TEST_CASE_P(cvt420, Imgproc_ColorYUV,
    ::testing::Values((int)CC_YUV2RGB_NV12, (int)CC_YUV2BGR_NV12, (int)CC_YUV2RGB_NV21, (int)CC_YUV2BGR_NV21,
                      (int)CC_YUV2RGBA_NV12, (int)CC_YUV2BGRA_NV12, (int)CC_YUV2RGBA_NV21, (int)CC_YUV2BGRA_NV21,
                      (int)CC_YUV2RGB_YV12, (int)CC_YUV2BGR_YV12, (int)CC_YUV2RGB_IYUV, (int)CC_YUV2BGR_IYUV,
                      (int)CC_YUV2RGBA_YV12, (int)CC_YUV2BGRA_YV12, (int)CC_YUV2RGBA_IYUV, (int)CC_YUV2BGRA_IYUV,
                      (int)CC_YUV2GRAY_420, (int)CC_RGB2YUV_YV12, (int)CC_BGR2YUV_YV12, (int)CC_RGBA2YUV_YV12,
                      (int)CC_BGRA2YUV_YV12, (int)CC_RGB2YUV_I420, (int)CC_BGR2YUV_I420, (int)CC_RGBA2YUV_I420,
                      (int)CC_BGRA2YUV_I420));

INSTANTIATE_TEST_CASE_P(cvt422, Imgproc_ColorYUV,
    ::testing::Values((int)CC_YUV2RGB_UYVY, (int)CC_YUV2BGR_UYVY, (int)CC_YUV2RGBA_UYVY, (int)CC_YUV2BGRA_UYVY,
                      (int)CC_YUV2RGB_YUY2, (int)CC_YUV2BGR_YUY2, (int)CC_YUV2RGB_YVYU, (int)CC_YUV2BGR_YVYU,
                      (int)CC_YUV2RGBA_YUY2, (int)CC_YUV2BGRA_YUY2, (int)CC_YUV2RGBA_YVYU, (int)CC_YUV2BGRA_YVYU,
                      (int)CC_YUV2GRAY_UYVY, (int)CC_YUV2GRAY_YUY2));
