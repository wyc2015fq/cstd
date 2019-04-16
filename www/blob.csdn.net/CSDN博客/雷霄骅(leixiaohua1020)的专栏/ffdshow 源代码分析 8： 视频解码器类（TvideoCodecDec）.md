# ffdshow 源代码分析 8： 视频解码器类（TvideoCodecDec） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月13日 00:06:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：35











注：写了一系列的有关ffdshow对解码器的封装的代码，列表如下：
[ffdshow 源代码分析 6： 对解码器的dll的封装（libavcodec）](http://blog.csdn.net/leixiaohua1020/article/details/15493329)
[ffdshow 源代码分析 7： libavcodec视频解码器类（TvideoCodecLibavcodec）](http://blog.csdn.net/leixiaohua1020/article/details/15493521)
[ffdshow 源代码分析 8： 视频解码器类（TvideoCodecDec）](http://blog.csdn.net/leixiaohua1020/article/details/15493743)
[ffdshow 源代码分析 9： 编解码器有关类的总结](http://blog.csdn.net/leixiaohua1020/article/details/15493961)


==========



![](https://img-blog.csdn.net/20140616103824187)

前面两篇文章介绍了ffdshow中libavcodec的封装类Tlibavcodec，以及libavcodec的解码器类TvideoCodecLibavcodec：

[ffdshow 源代码分析 6： 对解码器的dll的封装（libavcodec）](http://blog.csdn.net/leixiaohua1020/article/details/15493329)

[ffdshow 源代码分析 7： 解码器类（TvideoCodecLibavcodec）](http://blog.csdn.net/leixiaohua1020/article/details/15493521)

其中libavcodec的解码器类TvideoCodecLibavcodec通过调用Tlibavcodec中的方法实现了libavcodec的dll中方法的调用；而它继承了TvideoCodecDec，本文正是要分析它继承的这个类。



TvideoCodecDec是所有视频解码器共有的父类。可以看一下它的继承关系：

![](https://img-blog.csdn.net/20131111235459359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



可见，除了TvideoCodecLibavcodec继承了TvideoCodecDec之外，还有好几个类继承了TvideoCodecDec，比如说：TvideoCodecLibmpeg2，TvideoCodecXviD4等等…。突然来了兴趣，我们可以看一下其他的解码器类的定义是什么样的。

TvideoCodecLibmpeg2定义如下：

```cpp
/* 
 *雷霄骅 
 *leixiaohua1020@126.com 
 *中国传媒大学/数字电视技术 
 */ 
#ifndef _TVIDEOCODECLIBMPEG2_H_
#define _TVIDEOCODECLIBMPEG2_H_

#include "TvideoCodec.h"
#include "libmpeg2/include/mpeg2.h"

class Tdll;
struct Textradata;
class TccDecoder;
//libmpeg2解码器
class TvideoCodecLibmpeg2 : public TvideoCodecDec
{
private:
    Tdll *dll;
    uint32_t (*mpeg2_set_accel)(uint32_t accel);
    mpeg2dec_t* (*mpeg2_init)(void);
    const mpeg2_info_t* (*mpeg2_info)(mpeg2dec_t *mpeg2dec);
    mpeg2_state_t (*mpeg2_parse)(mpeg2dec_t *mpeg2dec);
    void (*mpeg2_buffer)(mpeg2dec_t *mpeg2dec, const uint8_t *start, const uint8_t *end);
    void (*mpeg2_close)(mpeg2dec_t *mpeg2dec);
    void (*mpeg2_reset)(mpeg2dec_t *mpeg2dec, int full_reset);
    void (*mpeg2_set_rtStart)(mpeg2dec_t *mpeg2dec, int64_t rtStart);
    int (*mpeg2_guess_aspect)(const mpeg2_sequence_t * sequence, unsigned int * pixel_width, unsigned int * pixel_height);

    mpeg2dec_t *mpeg2dec;
    const mpeg2_info_t *info;
    bool wait4Iframe;
    int sequenceFlag;
    REFERENCE_TIME avgTimePerFrame;
    TffPict oldpict;
    Textradata *extradata;
    TccDecoder *ccDecoder;
    Tbuffer *buffer;
    uint32_t oldflags;
    bool m_fFilm;
    int SetDeinterlaceMethod(void);

    void init(void);
    HRESULT decompressI(const unsigned char *src, size_t srcLen, IMediaSample *pIn);

protected:
    virtual bool beginDecompress(TffPictBase &pict, FOURCC infcc, const CMediaType &mt, int sourceFlags);

public:
    TvideoCodecLibmpeg2(IffdshowBase *Ideci, IdecVideoSink *Isink);
    virtual ~TvideoCodecLibmpeg2();

    static const char_t *dllname;
    virtual int getType(void) const {
        return IDFF_MOVIE_LIBMPEG2;
    }
    virtual int caps(void) const {
        return CAPS::VIS_QUANTS;
    }

    virtual void end(void);
    virtual HRESULT decompress(const unsigned char *src, size_t srcLen, IMediaSample *pIn);
    virtual bool onSeek(REFERENCE_TIME segmentStart);
    virtual HRESULT BeginFlush();
};

#endif
```





TvideoCodecXviD4定义如下：

```cpp
/* 
 *雷霄骅 
 *leixiaohua1020@126.com 
 *中国传媒大学/数字电视技术 
 */ 
#ifndef _TVIDEOCODECXVID4_H_
#define _TVIDEOCODECXVID4_H_

#include "TvideoCodec.h"

class Tdll;
struct Textradata;
//xvid解码器
class TvideoCodecXviD4 : public TvideoCodecDec
{
private:
    void create(void);
    Tdll *dll;
public:
    TvideoCodecXviD4(IffdshowBase *Ideci, IdecVideoSink *IsinkD);
    virtual ~TvideoCodecXviD4();
    int (*xvid_global)(void *handle, int opt, void *param1, void *param2);
    int (*xvid_decore)(void *handle, int opt, void *param1, void *param2);
    int (*xvid_plugin_single)(void *handle, int opt, void *param1, void *param2);
    int (*xvid_plugin_lumimasking)(void *handle, int opt, void *param1, void *param2);
    static const char_t *dllname;
private:
    void *enchandle, *dechandle;
    int psnr;
    TffPict pict;
    Tbuffer pictbuf;
    static int me_hq(int rd3), me_(int me3);
    Textradata *extradata;
    REFERENCE_TIME rtStart, rtStop;
protected:
    virtual bool beginDecompress(TffPictBase &pict, FOURCC infcc, const CMediaType &mt, int sourceFlags);
    virtual HRESULT flushDec(void);
public:
    virtual int getType(void) const {
        return IDFF_MOVIE_XVID4;
    }
    virtual int caps(void) const {
        return CAPS::VIS_QUANTS;
    }

    virtual HRESULT decompress(const unsigned char *src, size_t srcLen, IMediaSample *pIn);
};

#endif
```



从以上这2种解码器类的定义，我们可以看出一些规律，比如说：

> 1. 都有Tdll *dll这个变量，用于加载视频解码器的dll
2. 都有beginDecompress()函数，用于初始化解码器
3. 都有decompress()函数，用于解码





好了，闲话不说，回归正题，来看一下这些解码器共有的父类：TvideoCodecDec

```cpp
//具体 视频 解码器的父类，存一些公共信息
class TvideoCodecDec : virtual public TvideoCodec, virtual public TcodecDec
{
protected:
    bool isdvdproc;
    comptrQ<IffdshowDecVideo> deciV;
    IdecVideoSink *sinkD;
    TvideoCodecDec(IffdshowBase *Ideci, IdecVideoSink *Isink);
    Rational guessMPEG2sar(const Trect &r, const Rational &sar2, const Rational &containerSar);

    class TtelecineManager
    {
    private:
        TvideoCodecDec* parent;
        int segment_count;
        int pos_in_group;
        struct {
            int fieldtype;
            int repeat_pict;
            REFERENCE_TIME rtStart;
        } group[2]; // store information about 2 recent frames.
        REFERENCE_TIME group_rtStart;
        bool film;
        int cfg_softTelecine;
    public:
        TtelecineManager(TvideoCodecDec* Iparent);
        void get_timestamps(TffPict &pict);
        void get_fieldtype(TffPict &pict);
        void new_frame(int top_field_first, int repeat_pict, const REFERENCE_TIME &rtStart, const REFERENCE_TIME &rtStop);
        void onSeek(void);
    } telecineManager;

public:
    static TvideoCodecDec* initDec(IffdshowBase *deci, IdecVideoSink *Isink, AVCodecID codecId, FOURCC fcc, const CMediaType &mt);

    virtual ~TvideoCodecDec();

    virtual int caps(void) const {
        return CAPS::NONE;
    }
    virtual bool testMediaType(FOURCC fcc, const CMediaType &mt) {
        return true;
    }
    virtual void forceOutputColorspace(const BITMAPINFOHEADER *hdr, int *ilace, TcspInfos &forcedCsps) {
        *ilace = 0; //cspInfos of forced output colorspace, empty when entering function
    }
    enum {SOURCE_REORDER = 1};
    virtual bool beginDecompress(TffPictBase &pict, FOURCC infcc, const CMediaType &mt, int sourceFlags) = 0;
    virtual HRESULT decompress(const unsigned char *src, size_t srcLen, IMediaSample *pIn) = 0;
    virtual bool onDiscontinuity(void) {
        return false;
    }
    virtual HRESULT onEndOfStream(void) {
        return S_OK;
    }

    unsigned int quantsDx, quantsStride, quantsDy, quantBytes, quantType;
    //QP表
	void *quants;
    uint16_t *intra_matrix, *inter_matrix;
	//计算平均QP
    float calcMeanQuant(void);
	//画运动矢量
    virtual bool drawMV(unsigned char *dst, unsigned int dx, stride_t stride, unsigned int dy) const {
        return false;
    }
    virtual const char* get_current_idct(void) {
        return NULL;
    }
    virtual int useDXVA(void) {
        return 0;
    };

    virtual void setOutputPin(IPin * /*pPin*/) {}
};
```



TvideoCodecDec这个类中，还定义了一个类TtelecineManager。这种在类里面再定义一个类的方式还是不太多见的。TtelecineManager这个类的作用还没有研究，先不管它。

可以看出，TvideoCodecDec类的定义并不复杂，最主要的变量有如下几个，这几个变量都是子类中会用到的：

> comptrQ<IffdshowDecVideo>deciV：重要性不言而喻，回头介绍
IdecVideoSink *sinkD：重要性不言而喻，回头介绍
void *quants：QP表（为什么要存在这里还没搞清）

TvideoCodecDec类定义了几个函数：

> initDec()：初始化解码器（重要）
calcMeanQuant()：计算平均QP（为什么要在这里计算还没搞清）



TvideoCodecDec类还定义了一些纯虚函数，作为接口，这些函数的实现都在TvideoCodecDec的子类中完成【这几个函数是最重要的】：

> beginDecompress();
decompress();



TvideoCodecDec类中最重要的函数只有一个，就是initDec()，作用主要是初始化解码器。其他的很多函数大多只是定义了一个名称，并没有实现，因为都是打算在具体各种解码器类中再进行实现的。

看一下initDec()的代码：

```cpp
TvideoCodecDec* TvideoCodecDec::initDec(IffdshowBase *deci, IdecVideoSink *sink, AVCodecID codecId, FOURCC fcc, const CMediaType &mt)
{
    // DXVA mode is a preset setting
    switch (codecId) {
        case AV_CODEC_ID_H264:
            if (deci->getParam2(IDFF_filterMode) & IDFF_FILTERMODE_VIDEODXVA) {
                if (deci->getParam2(IDFF_dec_DXVA_H264)) {
                    codecId = CODEC_ID_H264_DXVA;
                } else {
                    return NULL;
                }
            }
            break;
        case AV_CODEC_ID_VC1:
        case CODEC_ID_WMV9_LIB:
            if (deci->getParam2(IDFF_filterMode) & IDFF_FILTERMODE_VIDEODXVA) {
                if (deci->getParam2(IDFF_dec_DXVA_VC1)) {
                    codecId = CODEC_ID_VC1_DXVA;
                } else {
                    return NULL;
                }
            }
            break;
        default:
            break;
    }

    TvideoCodecDec *movie = NULL;

    if (is_quicksync_codec(codecId)) {
        movie = new TvideoCodecQuickSync(deci, sink, codecId);
    } else if (lavc_codec(codecId)) {
        movie = new TvideoCodecLibavcodec(deci, sink);
    } else if (raw_codec(codecId)) {
        movie = new TvideoCodecUncompressed(deci, sink);
    } else if (wmv9_codec(codecId)) {
        movie = new TvideoCodecWmv9(deci, sink);
    } else if (codecId == CODEC_ID_XVID4) {
        movie = new TvideoCodecXviD4(deci, sink);
    } else if (codecId == CODEC_ID_LIBMPEG2) {
        movie = new TvideoCodecLibmpeg2(deci, sink);
    } else if (codecId == CODEC_ID_AVISYNTH) {
        movie = new TvideoCodecAvisynth(deci, sink);
    } else if (codecId == CODEC_ID_H264_DXVA || codecId == CODEC_ID_VC1_DXVA) {
        movie = new TvideoCodecLibavcodecDxva(deci, sink, codecId);
    } else {
        return NULL;
    }
    if (!movie) {
        return NULL;
    }
    if (movie->ok && movie->testMediaType(fcc, mt)) {
        movie->codecId = codecId;
        return movie;
    } else if (is_quicksync_codec(codecId)) {
        // QuickSync decoder init failed, revert to internal decoder.
        switch (codecId) {
            case CODEC_ID_H264_QUICK_SYNC:
                codecId = AV_CODEC_ID_H264;
                break;
            case CODEC_ID_MPEG2_QUICK_SYNC:
                codecId = CODEC_ID_LIBMPEG2;
                break;
            case CODEC_ID_VC1_QUICK_SYNC:
                codecId = CODEC_ID_WMV9_LIB;
                break;
            default:
                ASSERT(FALSE); // this shouldn't happen!
        }

        delete movie;

        // Call this function again with the new codecId.
        return initDec(deci, sink, codecId, fcc, mt);
    } else {
        delete movie;
        return NULL;
    }
}
```



这个函数的功能还是比较好理解的，根据CodecID的不同，创建不同的解码器（从TvideoCodecLibavcodec，TvideoCodecXviD4，TvideoCodecLibmpeg2这些里面选择）。



虽然不知道用途是什么，但是我们可以顺便看一下计算平均QP的函数，就是把quants1指向的QP表里面的数据求了一个平均值：

```cpp
//计算平均QP
float TvideoCodecDec::calcMeanQuant(void)
{
    if (!quants || !quantsDx || !quantsDy) {
        return 0;
    }
    unsigned int sum = 0, num = quantsDx * quantsDy;
    unsigned char *quants1 = (unsigned char*)quants;
    for (unsigned int y = 0; y < quantsDy; y++)
        for (unsigned int x = 0; x < quantsDx; x++) {
            sum += quants1[(y * quantsStride + x) * quantBytes];
        }
    return float(sum) / num;
}
```














