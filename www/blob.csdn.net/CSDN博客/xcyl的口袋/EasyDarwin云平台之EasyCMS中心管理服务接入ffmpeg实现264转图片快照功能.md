# EasyDarwin云平台之EasyCMS中心管理服务接入ffmpeg实现264转图片快照功能 - xcyl的口袋 - CSDN博客
2016年06月28日 11:10:01[xcyl](https://me.csdn.net/cai6811376)阅读数：1063
EasyDarwin一直坚持开源精神，所以我们着手把EasyDarwin中使用的非开源工具替换为开源项目。
**我们将EasyCMS中使用的图片保存替换为FFmpeg。（这里说明Windows版）**
- 首先到ffmpeg官网下载编译好的ffmpeg文件
> 
下载页面：[https://ffmpeg.zeranoe.com/builds/](https://ffmpeg.zeranoe.com/builds/)
![ffmpeg](https://img-blog.csdn.net/20160628111014033)
> 
Shared包含.dll文件，Dev包含.h和.lib文件
将.h/.lib/.dll引入EasyDarwin工程
.h放入EasyDarwin/Include/FFmpeg下
.lib放入EasyDarwin/Lib/FFmpeg/x86下
.dll放入EasyDarwin/Bin/FFmpeg/x86下
- 编写DecoderHelper类
DecoderHelper.h
```
#ifndef _DECODER_HELPER_
#define _DECODER_HELPER_
#define __STDC_CONSTANT_MACROS
extern "C" 
{
#include <inttypes.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
}
class DecoderHelper
{
public:
    DecoderHelper();
    ~DecoderHelper();
    int     SetVideoDecoderParam(int width, int height, int codec, int format);
    int     DecodeVideo(char *inBuff, int inBuffSize, void *yuvBuff, int width, int height);
private:
    void    releaseVideoDecoder();
private:
    AVFormatContext*    _formatContext;
    AVCodecContext*     _videoCodecContext;
    AVFrame*            _videoFrame420;
    struct SwsContext*  _swsContext;
    uint8_t*            _buffYUV420;
    AVPacket            _videoAVPacket;
    uint8_t*            _buffYUV;
    AVFrame*            _avframeYUV;
    AVFrame*            _avframeSWS;
    int                 _codec;
    int                 _width;
    int                 _height;
    int                 _outputFormat;
};
#endif //_DECODER_HELPER_
```
DecoderHelper.cpp
```
#include "DecoderHelper.h"
DecoderHelper::DecoderHelper()
    : _formatContext(NULL),
    _videoCodecContext(NULL),
    _videoFrame420(NULL),
    _buffYUV420(NULL),
    _buffYUV(NULL),
    _avframeYUV(NULL),
    _avframeSWS(NULL),
    _swsContext(NULL),
    _codec(0),
    _width(0),
    _height(0),
    _outputFormat(0)
{
    //Register all the codec
    avcodec_register_all();
    //Register all types of decoding
    av_register_all();
}
DecoderHelper::~DecoderHelper()
{
    releaseVideoDecoder();
}
int DecoderHelper::SetVideoDecoderParam(int width, int height, int codec, int format)
{
    if (_width != width || _height != height || _codec != codec)
    {
        releaseVideoDecoder();
    }
    if (NULL != _videoCodecContext)
    {
        return -1;
    }
    AVCodec *avcodec = avcodec_find_decoder((AVCodecID)codec);
    if (NULL == avcodec)
    {
        return -1;
    }
    _videoCodecContext = avcodec_alloc_context3(avcodec);
    _videoCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    _videoCodecContext->width   = width;
    _videoCodecContext->height  = height;
    int ret = avcodec_open2(_videoCodecContext, avcodec, NULL);
    if (ret < 0)
    {
        goto $fail;
    }
    int numBytes = avpicture_get_size(AV_PIX_FMT_YUV420P, width, height);
    _buffYUV420 = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    _videoFrame420 = av_frame_alloc();
    if (avpicture_fill((AVPicture *)_videoFrame420, _buffYUV420, AV_PIX_FMT_YUV420P,
        _width, _height) < 0)
    {
    }
    av_init_packet(&_videoAVPacket);
    _width = width;
    _height = height;
    _codec = codec;
    _outputFormat = format;
    return 0;
$fail:
    {
        return -1;
    }
}
int DecoderHelper::DecodeVideo(char *inBuff, int inBuffSize, void *yuvBuff, int width, int height)
{
    if (NULL == inBuff)         return -1;
    if (1 > inBuffSize)         return -1;
    if (NULL == yuvBuff)        return -1;
    if (NULL == _videoCodecContext)     return -2;
    _videoAVPacket.size = inBuffSize;
    _videoAVPacket.data = (uint8_t*)inBuff;
    int frameFinished = 0;
    int nDecode = avcodec_decode_video2(_videoCodecContext, _videoFrame420, &frameFinished, &_videoAVPacket);//(uint8_t*)pInBuffer, inputSize);
    if (nDecode < 0)    return -3;
    if (!frameFinished) return -4;
    if  (width != _width || height != _height)
    {
        if (NULL != _avframeYUV)
        {
            av_frame_free(&_avframeYUV);
            _avframeYUV = NULL;
        }
        if (NULL != _swsContext)
        {
            sws_freeContext(_swsContext);
            _swsContext = NULL;
        }
        _width = width;
        _height = height;
    }
    if (NULL == _avframeYUV)
    {
        int numBytes = avpicture_get_size((AVPixelFormat)_outputFormat, width, height);
        _avframeYUV = av_frame_alloc();
    }
    if (NULL == _avframeYUV)        return -5;
    if (avpicture_fill((AVPicture *)_avframeYUV, (uint8_t*)yuvBuff, (AVPixelFormat)_outputFormat,
        width, height) < 0)
    {
        return -1;
    }
    if (NULL == _swsContext)
    {
        _swsContext = sws_getCachedContext(_swsContext, _videoCodecContext->width, _videoCodecContext->height, (AVPixelFormat)AV_PIX_FMT_YUV420P, 
            width, height, (AVPixelFormat)_outputFormat, SWS_BICUBIC, NULL, NULL, NULL);
    }
    if (NULL == _swsContext)        return -1;
    int ret = sws_scale(_swsContext, _videoFrame420->data, _videoFrame420->linesize, 0, _videoCodecContext->height, 
        _avframeYUV->data, _avframeYUV->linesize);
    return 0;
}
void DecoderHelper::releaseVideoDecoder()
{
    if (NULL != _videoFrame420)
    {
        av_frame_free(&_videoFrame420);
        _videoFrame420 = NULL;
    }
    if (NULL != _buffYUV420)
    {
        av_free(_buffYUV420);
        _buffYUV420 = NULL;
    }
    if (NULL != _avframeSWS)
    {
        av_frame_free(&_avframeSWS);
        _avframeSWS = NULL;
    }
    if (NULL != _avframeYUV)
    {
        av_frame_free(&_avframeYUV);
        _avframeYUV = NULL;
    }
    if (NULL != _buffYUV)
    {
        av_free(_buffYUV);
        _buffYUV = NULL;
    }
    if (NULL != _swsContext)
    {
        sws_freeContext(_swsContext);
        _swsContext = NULL;
    }
    if (NULL != _videoCodecContext)
    {
        avcodec_close(_videoCodecContext);
        av_free(_videoCodecContext);
        _videoCodecContext = NULL;
    }
}
```
- 使用
```
DecoderHelper decoderHelper;
decoderHelper.SetVideoDecoderParam();
decoderHelper.DecodeVideo();
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：288214068
Copyright © EasyDarwin.org 2012-2015
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
