# android ffmpeg+OpenGL播放yuv+openSL 快放 慢放 视频播放器 - 建建的博客 - CSDN博客
2018年03月06日 11:32:35[纪建](https://me.csdn.net/u013898698)阅读数：995
这里是完整的音视频播放器，功能如下（[这里有iOS版的](http://blog.csdn.net/m0_37677536/article/details/78769362)）： 
![这里写图片描述](https://img-blog.csdn.net/20171212095746568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbTBfMzc2Nzc1MzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
视频是通过opengl 播放yuv数据，音频是opensl播放。 
app运行流程如下图： 
![这里写图片描述](https://img-blog.csdn.net/20171211175701760?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbTBfMzc2Nzc1MzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
红色虚线内的是一个线程的运行，总共涉及到四个线程。 
java层几乎没有播放器的内容，ffmpeg，opengl，opensl都是在c++层运行，所以可以原封不动的迁移到其他平台，如ios，不过c/c++代码ios程序员看起来不太友好，[有ios版的](http://blog.csdn.net/m0_37677536/article/details/78769362)，ios版的有yuv->RGB->image的方法，有ffmpeg解码播放流程图，解码播放流程一目了然。 
看一下app表现，小米6播放每分钟100M等级的视频： 
![播放每分钟100M等级的视频](https://img-blog.csdn.net/20171212134538948?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbTBfMzc2Nzc1MzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
可以看到cpu使用在10%左右，很省电了有没有100M每分钟，一部电影是十几G的清晰度。 
再看常规清晰度720p： 
![这里写图片描述](https://img-blog.csdn.net/20171212134955020?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbTBfMzc2Nzc1MzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
cpu消耗只有3%左右，这也就是直播清晰度了，这cpu消耗还能说什么。 
看内存优化： 
![这里写图片描述](https://img-blog.csdn.net/20171212135056049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbTBfMzc2Nzc1MzY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
整个播放过程native层内存占用都在15M左右波动，Graphics占用内存也没变化，播放过程中内存释放很到位，没有内存增长。大家可以自行测试。 
这里给大家看下ffmpeg解码代码： 
FFmpeg.h
```
//
// Created by huizai on 2017/11/22.
//
#ifndef FFMPEG_DEMO_FFMPEGDECODER_H
#define FFMPEG_DEMO_FFMPEGDECODER_H
#include "JniDefine.h"
#include "YUV_GL_DATA.h"
extern "C" {
//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
//像素处理
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}
//定义音频重采样后的参数
#define SAMPLE_SIZE 16
#define SAMPLE_RATE 44100
#define CHANNEL     2
typedef struct  FFmpeg{
    int                       totleMs;
//音频播放时间
    int                       aFps;
//视频播放时间
    int                       vFps;
//视频流索引
    int                       videoStreamIndex;
    int                       audioStreamIndex;
    int                       sampleRate;
    int                       sampleSize;
    int                       channel;
//音频贞数据的长度
    int                       pcmDataLength;
    AVFormatContext         * pFormatCtx;
    AVCodecContext          * pVideoCodecCtx;
    AVCodecContext          * pAudioCodecCtx;
    AVFrame                 * pYuv;
    AVFrame                 * pPcm;
    AVCodec                 * pVideoCodec; //视频解码器
    AVCodec                 * pAudioCodec; //音频解码器
    struct SwsContext       * pSwsCtx;
    SwrContext              * pSwrCtx;
    const char              * url;
    AVPacket                  pkt;
    char                    * audioData;
    H264YUV_Frame             yuvFrame;
}Decoder;
int   OpenAndInitDecoder(Decoder * decoder);
int   Read(Decoder * decoder);
int   Decode(Decoder * decoder,AVPacket packet);
int   YuvToGLData(Decoder *decoder);
int   ToPcm(Decoder * decoder,AACFrame * frame);
void  printError(const char* flag,int ret);
double r2d(AVRational r);
#endif //FFMPEG_DEMO_FFMPEGDECODER_H
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
FFmpeg.cpp
```
//
// Created by huizai on 2017/11/22.
//
#include "FFmpeg.h"
int OpenAndInitDecoder(Decoder * decoder){
    //1.注册所有组件
    av_register_all();
    avformat_network_init();
    //2.打开输入视频文件
    //封装格式上下文，统领全局的结构体，保存了视频文件封装格式的相关信息
    decoder->pFormatCtx = avformat_alloc_context();
    int ret = avformat_open_input(&decoder->pFormatCtx, decoder->url, NULL, NULL);
    if (ret != 0) {
        return  ret;
    } else{
        FFLOGI("视频长度：%d",decoder->pFormatCtx->duration);
    }
    decoder->totleMs = (int)(decoder->pFormatCtx->duration/AV_TIME_BASE)*1000;
    avformat_find_stream_info(decoder->pFormatCtx, NULL);
    //分别找到音频视频解码器并打开解码器
    for (int i = 0; i < decoder->pFormatCtx->nb_streams; i++) {
        AVStream *stream = decoder->pFormatCtx->streams[i];
        AVCodec * codec = avcodec_find_decoder(stream->codecpar->codec_id);
        AVCodecContext * codecCtx = avcodec_alloc_context3(codec);
        avcodec_parameters_to_context(codecCtx, stream->codecpar);
        if (codecCtx->codec_type == AVMEDIA_TYPE_VIDEO) {
            printf("video\n");
            decoder->videoStreamIndex = i;
            decoder->pVideoCodec  = codec;
            decoder->pVideoCodecCtx = codecCtx;
            int err = avcodec_open2(decoder->pVideoCodecCtx, decoder->pVideoCodec, NULL);
            if (err != 0) {
                char buf[1024] = {0};
                av_strerror(err, buf, sizeof(buf));
                printf("open videoCodec error:%s", buf);
                return false;
            }
        }
        if (codecCtx->codec_type == AVMEDIA_TYPE_AUDIO) {
            printf("audio\n");
            decoder->audioStreamIndex = i;
            decoder->pAudioCodec  = codec;
            decoder->pAudioCodecCtx = codecCtx;
            int err = avcodec_open2(decoder->pAudioCodecCtx, decoder->pAudioCodec, NULL);
            if (err != 0) {
                char buf[1024] = {0};
                av_strerror(err, buf, sizeof(buf));
                printf("open audionCodec error:%s", buf);
                return false;
            }
            if (codecCtx->sample_rate != SAMPLE_RATE) {
                decoder->sampleRate = codecCtx->sample_rate;
            }
        }
    }
    decoder ->pYuv = av_frame_alloc();
    decoder ->pPcm = av_frame_alloc();
    decoder ->channel = CHANNEL;
    decoder ->sampleSize = SAMPLE_SIZE;
    decoder ->sampleRate = decoder->pAudioCodecCtx->sample_rate;
   // printf("open acodec success! sampleRate:%d  channel:%d  sampleSize:%d fmt:%d\n",
         //  decoder->sampleRate,decoder->channel,decoder->sampleSize,decoder->pAudioCodecCtx->sample_fmt);
    FFLOGI("open acodec success! sampleRate:%d  channel:%d  fmt:%d\n",
           decoder->pAudioCodecCtx->sample_rate,decoder->pAudioCodecCtx->channels,decoder->pAudioCodecCtx->sample_fmt);
    return  ret;
}
int  Read(Decoder * decoder){
    //这里先不加线程锁，在启动多线程的地方统一加锁
    // AVPacket * pkt = malloc(sizeof(AVPacket));
    if (!decoder->pFormatCtx) {
        return -1;
    }
    int err = av_read_frame(decoder->pFormatCtx,&decoder->pkt);
    if (err != 0) {
        printError("av_read_pkt_error:",err);
        if (err == -541478725){
            //读取文件结束
            return -9;
        }
        return  -2;
    }
    return  0;
}
int Decode(Decoder* decoder,AVPacket packet){
    if (!decoder->pFormatCtx) {
        return -1;
    }
    //分配AVFream 空间
    if (decoder->pYuv == NULL) {
        decoder->pYuv = av_frame_alloc();
    }
    if (decoder->pPcm == NULL) {
        decoder->pPcm = av_frame_alloc();
    }
    AVCodecContext * pCodecCtx;
    AVFrame * tempFrame;
    if (packet.stream_index == decoder->videoStreamIndex) {
        pCodecCtx = decoder->pVideoCodecCtx;
        tempFrame = decoder->pYuv;
    }else if (packet.stream_index == decoder->audioStreamIndex){
        pCodecCtx = decoder->pAudioCodecCtx;
        tempFrame = decoder->pPcm;
    }else{
        return -1;
    }
    if (!pCodecCtx) {
        return -1;
    }
    int re = avcodec_send_packet(pCodecCtx,&packet);
    if (re != 0) {
        char errorBuf[1024] = {0};
        av_make_error_string(errorBuf,1024,re);
        FFLOGE("send pkt error:%s",errorBuf);
        return -1;
    }
    re = avcodec_receive_frame(pCodecCtx, tempFrame);
    if (re != 0) {
        char errorBuf[1024] = {0};
        av_make_error_string(errorBuf,1024,re);
        FFLOGE("receive av_data error:%s",errorBuf);
        return -1;
    }
    if (packet.stream_index == decoder->videoStreamIndex) {
        decoder->vFps = (int)((packet.pts * r2d(decoder->pFormatCtx->streams[decoder->videoStreamIndex]->time_base))*1000);
    }else if (packet.stream_index == decoder->audioStreamIndex){
        decoder->aFps = (int)((packet.pts * r2d(decoder->pFormatCtx->streams[decoder->audioStreamIndex]->time_base))*1000);
    }
    FFLOGI("[D]");
    return 0;
}
int Min(int a,int b){
    return a>b?b:a;
}
void copyDecodedFrame(unsigned char *src, unsigned char *dist,int linesize, int width, int height)
{
    width = Min(linesize, width);
    if (sizeof(dist) == 0) {
        return;
    }
    for (int i = 0; i < height; ++i) {
        memcpy(dist, src, width);
        dist += width;
        src += linesize;
    }
}
int   YuvToGLData(Decoder *decoder){
    if (!decoder->pFormatCtx || !decoder->pYuv ||decoder-> pYuv->linesize[0] <= 0) {
        return -1;
    }
    //把数据重新封装成opengl需要的格式
    unsigned int lumaLength= (unsigned int)(decoder->pYuv->height)*(Min(decoder->pYuv->linesize[0], decoder->pYuv->width));
    unsigned int chromBLength=(unsigned int)((decoder->pYuv->height)/2)*(Min(decoder->pYuv->linesize[1], (decoder->pYuv->width)/2));
    unsigned int chromRLength=(unsigned int)((decoder->pYuv->height)/2)*(Min(decoder->pYuv->linesize[2], (decoder->pYuv->width)/2));
    decoder->yuvFrame.luma.dataBuffer = (unsigned char*)malloc(lumaLength);
    decoder->yuvFrame.chromaB.dataBuffer = (unsigned char*)malloc(chromBLength);
    decoder->yuvFrame.chromaR.dataBuffer = (unsigned char*)malloc(chromRLength);
    decoder->yuvFrame.width=(unsigned int)decoder->pYuv->width;
    decoder->yuvFrame.height=(unsigned int)decoder->pYuv->height;
    if (decoder->pYuv->height <= 0) {
        free(decoder->yuvFrame.luma.dataBuffer);
        free(decoder->yuvFrame.chromaB.dataBuffer);
        free(decoder->yuvFrame.chromaR.dataBuffer);
        return -1;
    }
    copyDecodedFrame(decoder->pYuv->data[0],decoder->yuvFrame.luma.dataBuffer,decoder->pYuv->linesize[0],
                     decoder->pYuv->width,decoder->pYuv->height);
    copyDecodedFrame(decoder->pYuv->data[1], decoder->yuvFrame.chromaB.dataBuffer,decoder->pYuv->linesize[1],
                     decoder->pYuv->width / 2,decoder->pYuv->height / 2);
    copyDecodedFrame(decoder->pYuv->data[2], decoder->yuvFrame.chromaR.dataBuffer,decoder->pYuv->linesize[2],
                     decoder->pYuv->width / 2,decoder->pYuv->height / 2);
    return 0;
}
int ToPcm(Decoder * decoder,AACFrame * frame){
    if (!decoder->pFormatCtx || !decoder->pPcm) {
        return -1;
    }
   // printf("sample_rate:%d,channels:%d,sample_fmt:%d,channel_layout:%llu,nb_samples:%d\n",pAudioCodecCtx->sample_rate,pAudioCodecCtx->channels,pAudioCodecCtx->sample_fmt,pAudioCodecCtx->channel_layout,pPcm->nb_samples);
    //音频重采样
    if (decoder->pSwrCtx == NULL) {
        decoder->pSwrCtx = swr_alloc();
        swr_alloc_set_opts(decoder->pSwrCtx,
                           AV_CH_LAYOUT_STEREO,//2声道立体声
                           AV_SAMPLE_FMT_S16,  //采样大小 16位
                           decoder->sampleRate,
                           decoder->pAudioCodecCtx->channel_layout,
                           decoder->pAudioCodecCtx->sample_fmt,// 样本类型
                           decoder->pAudioCodecCtx->sample_rate,
                           0, 0);
        int ret = swr_init(decoder->pSwrCtx);
        char errorBuf[1024] = {0};
        av_make_error_string(errorBuf,1024,ret);
        FFLOGE("swr_init error:%s",errorBuf);
    }
    uint8_t * tempData[1];
    tempData[0] = (uint8_t*)frame->dataBuffer;
    int len = swr_convert(decoder->pSwrCtx, tempData, 10000, (const uint8_t**)decoder->pPcm->data,decoder->pPcm->nb_samples);
    if (len < 0) {
        char errbuf[1024] = {0};
        FFLOGE("swr_convert error:%d",len);
        return 0;
    }
    int outSize = av_samples_get_buffer_size(NULL,
                                             CHANNEL,
                                             len,
                                             AV_SAMPLE_FMT_S16,0);
    frame->length = (unsigned int)outSize;
    FFLOGI("nb_smples:%d,des_smples:%d,outSize:%d",decoder->pPcm->nb_samples,len,outSize);
    return 0;
}
void printError(const char * flag,int ret){
    char errorbuf[1024] = {0};
    av_make_error_string(errorbuf, 1024, ret);
    FFLOGE("%s,ret:%d,%s", flag, ret, errorbuf);
}
double r2d(AVRational r){
    return r.num == 0 || r.den == 0 ? 0.:(double)r.num/(double)r.den;
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
- 204
- 205
- 206
- 207
- 208
- 209
- 210
- 211
- 212
- 213
- 214
- 215
- 216
- 217
- 218
- 219
- 220
- 221
- 222
- 223
- 224
- 225
- 226
- 227
- 228
- 229
可以跟iOS版的对比一下，根本没区别，都是一样的，要不怎么说跨平台技术呢。代码说明可以去看ios版的有简单说明。也可以下载源码debug进行了解。
说明一点，想要做优化的，就是**我这里线程锁加的很粗糙，这样会造成不同线程对资源的等待，会有点点不流畅**，应该精细到每一句就好了，自己去调吧。
**注意！**
想要跑这个代码一点要**配置好ndk编译环境**！要不然跑不起来。 
配置教程： 
[http://blog.csdn.net/m0_37677536/article/details/78557573](http://blog.csdn.net/m0_37677536/article/details/78557573)
源码下载（运行环境AS3.0,编译：gradle-4.1-all）： 
[http://download.csdn.net/download/m0_37677536/10155271](http://download.csdn.net/download/m0_37677536/10155271)
ffmpeg库现在只支持本地或在线MP4格式播放。 
新版库下载（支持所有常见格式播放，目录结构跟代码中版本不一样需要配置）： 
[http://download.csdn.net/download/m0_37677536/10241575](http://download.csdn.net/download/m0_37677536/10241575)
测试视频下载（现在代码是在线播放1080p网络不好会卡）： 
[https://pan.baidu.com/s/1dEWVdAT](https://pan.baidu.com/s/1dEWVdAT)  密码：k4vu 
下载后放到手机跟目录，改一下MainActivity里面的播放地址即可； 
android opengl 播放 yuv： 
[http://blog.csdn.net/m0_37677536/article/details/78783267](http://blog.csdn.net/m0_37677536/article/details/78783267)
android opensl播放 pcm： 
[http://blog.csdn.net/m0_37677536/article/details/79013969](http://blog.csdn.net/m0_37677536/article/details/79013969)
