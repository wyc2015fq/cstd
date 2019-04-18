# ffmpeg 库yuv420转jpeg（内存） - tifentan的专栏 - CSDN博客

2017年06月21日 15:21:20[露蛇](https://me.csdn.net/tifentan)阅读数：1843


# ffmpeg 库yuv420转jpeg（内存）

近来实现mjpeg的http实时流，需要yuv420转jpeg。了解了一下主要可以通过ffmpeg或者libjpeg（terbo）实现。 

现想用ffmpeg，网上搜了一下，资料不多。比较热门的是雷神的博客： 
[http://blog.csdn.net/leixiaohua1020/article/details/25346147](http://blog.csdn.net/leixiaohua1020/article/details/25346147)

但有个问题，例子是写文件的，使用到AVFormatContext，封装好的读写，没法从内存里搞出来。 

自己对ffmpeg也是小白一个，只能慢慢查找资料研究。解决方法是自己定义AVCodecContext。
`pCodecCtx = avcodec_alloc_context3(pCodec);`
**这样总算能拿到编码后的buff了。但还有一个问题，在调试雷神的demo的时候发现编码后的图片明显有颜色的差别。**
![编码前](https://img-blog.csdn.net/20170621143913821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![编码后](https://img-blog.csdn.net/20170621143944674?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlmZW50YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

一开始以为是质量的问题，**找了很久也没发现哪里可以调质量(有人知道请告诉我一声)**。 

后来一想不太对，ffmpeg中jpeg编码输入要求YUVJ420P。但**YUVJ420P**和**YUV420P**是不一样的，他们的range有大有小。参考[https://en.wikipedia.org/wiki/YUV](https://en.wikipedia.org/wiki/YUV)

雷神的例子中直接把YUV420P当成YUVJ420P编码可能存在颜色出错。所以解决方法就是 
**1.YUV420P to YUVJ420P**
**2.YUVJ420P to jpeg**

最终加上YUV420P 转 YUVJ420P的代码就可以了。 

完整的代码如下，可下载雷神的demo修改：

```
extern "C"
{
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"  
};
int main(int argc, char* argv[])
{
    AVCodec *pCodec;
    AVCodecContext *pCodecCtx = NULL;
    int i, ret, got_output;
    FILE *fp_in;
    FILE *fp_out;
    AVFrame *pFrame;
    AVPacket pkt;
    int y_size;
    int framecnt = 0;
    struct SwsContext *img_convert_ctx;
    AVFrame *pFrame2;

    char filename_in[] = "cuc_view_480x272.yuv";

    AVCodecID codec_id = AV_CODEC_ID_MJPEG;
    char filename_out[] = "mytest.jpg";

    int in_w = 480, in_h = 272;
    int framenum = 1;

    avcodec_register_all();

    pCodec = avcodec_find_encoder(codec_id);
    if (!pCodec) {
        printf("Codec not found\n");
        return -1;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx) {
        printf("Could not allocate video codec context\n");
        return -1;
    }
    pCodecCtx->bit_rate = 4000000;
    pCodecCtx->width = in_w;
    pCodecCtx->height = in_h;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 11;
    pCodecCtx->gop_size = 75;
    //pCodecCtx->max_b_frames = 0;
    //pCodecCtx->global_quality = 1;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;

    //if (codec_id == AV_CODEC_ID_H264)
    //  av_opt_set(pCodecCtx->priv_data, "preset", "slow", 0);

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec\n");
        return -1;
    }

    pFrame = av_frame_alloc();
    if (!pFrame) {
        printf("Could not allocate video frame\n");
        return -1;
    }
    pFrame->format = pCodecCtx->pix_fmt;
    pFrame->width = pCodecCtx->width;
    pFrame->height = pCodecCtx->height;

    ret = av_image_alloc(pFrame->data, pFrame->linesize, pCodecCtx->width, pCodecCtx->height,
        pCodecCtx->pix_fmt, 16);
    if (ret < 0) {
        printf("Could not allocate raw picture buffer\n");
        return -1;
    }

    pFrame2 = av_frame_alloc();
    if (!pFrame) {
        printf("Could not allocate video frame\n");
        return -1;
    }
    pFrame2->format = AV_PIX_FMT_YUV420P;
    pFrame2->width = pCodecCtx->width;
    pFrame2->height = pCodecCtx->height;

    ret = av_image_alloc(pFrame2->data, pFrame2->linesize, pCodecCtx->width, pCodecCtx->height,
        AV_PIX_FMT_YUV420P, 16);
    if (ret < 0) {
        printf("Could not allocate raw picture buffer\n");
        return -1;
    }

    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUVJ420P, SWS_BICUBIC, NULL, NULL, NULL);

    //Input raw data
    fp_in = fopen(filename_in, "rb");
    if (!fp_in) {
        printf("Could not open %s\n", filename_in);
        return -1;
    }
    //Output bitstream
    fp_out = fopen(filename_out, "wb");
    if (!fp_out) {
        printf("Could not open %s\n", filename_out);
        return -1;
    }

    y_size = pCodecCtx->width * pCodecCtx->height;
    //Encode
    for (i = 0; i < framenum; i++) {
        av_init_packet(&pkt);
        pkt.data = NULL;    // packet data will be allocated by the encoder
        pkt.size = 0;
        //Read raw YUV data
        if (fread(pFrame2->data[0], 1, y_size, fp_in) <= 0 ||       // Y
            fread(pFrame2->data[1], 1, y_size / 4, fp_in) <= 0 ||   // U
            fread(pFrame2->data[2], 1, y_size / 4, fp_in) <= 0) {   // V
            return -1;
        }
        else if (feof(fp_in)) {
            break;
        }
        sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame2->data, pFrame2->linesize, 0, pCodecCtx->height, pFrame->data, pFrame->linesize);

        pFrame->pts = i;
        /* encode the image */
        ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_output);
        if (ret < 0) {
            printf("Error encoding frame\n");
            return -1;
        }
        if (got_output) {
            printf("Succeed to encode frame: %5d\tsize:%5d\n", framecnt, pkt.size);
            framecnt++;
            fwrite(pkt.data, 1, pkt.size, fp_out);
            av_free_packet(&pkt);
        }
    }
    //Flush Encoder
    for (got_output = 1; got_output; i++) {
        ret = avcodec_encode_video2(pCodecCtx, &pkt, NULL, &got_output);
        if (ret < 0) {
            printf("Error encoding frame\n");
            return -1;
        }
        if (got_output) {
            printf("Flush Encoder: Succeed to encode 1 frame!\tsize:%5d\n", pkt.size);
            fwrite(pkt.data, 1, pkt.size, fp_out);
            av_free_packet(&pkt);
        }
    }

    fclose(fp_out);
    avcodec_close(pCodecCtx);
    av_free(pCodecCtx);
    av_freep(&pFrame->data[0]);
    av_frame_free(&pFrame);
    av_frame_free(&pFrame2);
    return 0;
}
```

## 附：yuv颜色range转换原理

一开始我是想自己查查怎么转换range的。找了很久没找到什么例子，最后使用ffmpeg转换了。有时间得补补图像基础。 

实际上我猜想是等比变换的，应该不会太复杂，我看了一下内存里面几个值对比了一下也比较符合。有时间看看ffmpeg的源码验证一下。 

y的range是 [16，235] to [0，255] 

u、v的range是 [16，240] to [0，255]

所以y的等式如下： 

y420-16 / 235-16 = yj420 / 255 

uv的如下： 

uv420-16 / 240-16 = uvj420 / 255 

根据以上求解转换。

从来没这么认真写过博客，感觉耗时甚多。不知道那些写很多博客的大神，还写的很好的是怎么做到的。

