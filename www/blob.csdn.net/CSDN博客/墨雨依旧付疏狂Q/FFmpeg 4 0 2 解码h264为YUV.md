# FFmpeg 4.0.2 解码h264为YUV - 墨雨依旧付疏狂Q - CSDN博客





2018年08月17日 20:44:26[墨雨依旧付疏狂Q](https://me.csdn.net/hsq1596753614)阅读数：424
个人分类：[FFmpeg 4.0.2](https://blog.csdn.net/hsq1596753614/article/category/7967946)

所属专栏：[FFmpeg 4.0.2](https://blog.csdn.net/column/details/26855.html)









一、FFmpeg 4.0.2解码h264为YUV的流程如下图所示： 
![这里写图片描述](https://img-blog.csdn.net/2018082320490386?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2hzcTE1OTY3NTM2MTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

二、C/C++代码：
```cpp
#include <iostream>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavfilter/avfilter.h>
#include <libavutil/imgutils.h>
};
using namespace std;

int main()
{
    const char *fname = "test.h264";
    // 打开h264文件，并把文件信息存入fctx中 
    int iRes = 0;
    AVFormatContext *fctx = avformat_alloc_context();
    if ((iRes = avformat_open_input(&fctx, fname, NULL, NULL)) != 0)
    {
        cout << "File open failed!" << endl;
        return -1;
    }
    // 寻找视频流信息
    if (avformat_find_stream_info(fctx, NULL) < 0)
    {
        cout << "Stream find failed!\n";
        return -1;
    }
    // dump调试信息
    av_dump_format(fctx, -1, fname, NULL);
    // 打开了视频并且获取了视频流 ，设置视频索引值默认值
    int vindex = -1;
    for (int i = 0; i < fctx->nb_streams; i++)
    {
        if (fctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            vindex = i;
    }
    // 如果没有找到视频的索引，说明并不是一个视频文件
    if (vindex == -1)
    {
        cout << "Codec find failed!" << endl;
        return -1;
    }
    // 分配解码器上下文空间
    AVCodecContext *cctx = avcodec_alloc_context3(NULL);
    // 获取编解码器上下文信息
    if (avcodec_parameters_to_context(cctx, fctx->streams[vindex]->codecpar) < 0)
    {
        cout << "Copy stream failed!" << endl;
        return -1;
    }
    // 查找解码器
    AVCodec *c = avcodec_find_decoder(cctx->codec_id);
    if (!c) {
        cout << "Find Decoder failed!" << endl;
        return -1;
    }
    // 打开解码器
    if (avcodec_open2(cctx, c, NULL) != 0) {
        cout << "Open codec failed!" << endl;
        return -1;
    }
    // 对图形进行宽度上方的裁剪，以便于显示得更好
    struct SwsContext *imgCtx = sws_getContext(cctx->width, cctx->height, cctx->pix_fmt, cctx->width, cctx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    if (!imgCtx) 
    {
        cout << "Get swscale context failed!" << endl;
        return -1;
    }
    AVPacket *pkt = av_packet_alloc();
    AVFrame *fr = av_frame_alloc();
    AVFrame *yuv = av_frame_alloc();
    int vsize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, cctx->width, cctx->height, 1);
    uint8_t *buf = (uint8_t *)av_malloc(vsize);
    av_image_fill_arrays(yuv->data, yuv->linesize, buf, AV_PIX_FMT_YUV420P, cctx->width, cctx->height, 1);
    char errbuf[256] = { 0 };
    FILE *fp_yuv = fopen("test_out.yuv", "wb+");
    while (av_read_frame(fctx, pkt) >= 0) 
    {
        if (pkt->stream_index == vindex) 
        {
            if ((iRes = avcodec_send_packet(cctx, pkt)) != 0)
            {
                cout << "Send video stream packet failed!" << endl;
                av_strerror(iRes, errbuf, 256);
                return -5;
            }
            if ((iRes = avcodec_receive_frame(cctx, fr)) != 0)
            {
                cout << "Receive video frame failed!" << endl;
                av_strerror(iRes, errbuf, 256);
                return -6;
            }
            cout << "decoding the frame " << cctx->frame_number << endl;
            sws_scale(imgCtx, fr->data, fr->linesize, 0, cctx->height, yuv->data, yuv->linesize);
            int y_size = cctx->width*cctx->height;
            fwrite(yuv->data[0], 1, y_size, fp_yuv);        // Y 
            fwrite(yuv->data[1], 1, y_size / 4, fp_yuv);    // U
            fwrite(yuv->data[2], 1, y_size / 4, fp_yuv);    // V
        }
    }
    av_free(buf);
    av_frame_free(&yuv);
    av_frame_free(&fr);
    av_packet_free(&pkt);
    sws_freeContext(imgCtx);
    avcodec_free_context(&cctx);
    avformat_close_input(&fctx);
    avformat_free_context(fctx);

    return getchar();
}
```

三、新增/替换的API函数 

(1) avcodec_parameters_to_context() 

(2) av_image_get_buffer_size() 

(3) av_image_fill_arrays() 

(4) avcodec_send_frame() 

(5) avcodec_receive_packet() 

(6) av_packet_unref()




