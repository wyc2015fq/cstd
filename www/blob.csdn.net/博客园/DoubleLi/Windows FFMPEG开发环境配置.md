# Windows FFMPEG开发环境配置 - DoubleLi - 博客园






1.去FFMPEG网站上下载Dev版本的库，里面有我们需要的头文件和lib文件，然后下载Shared版本的库，里面有我们需要的dll文件

[http://ffmpeg.zeranoe.com/builds/](http://ffmpeg.zeranoe.com/builds/)

![](https://images2017.cnblogs.com/blog/708332/201708/708332-20170812205946304-2072350013.png)
记得区分32位和64位的库，这里碰到一个大坑，就是我下载的是64位的库，但是创建工程的时候选的是32位的工程，导致链接的时候一直报

无法解析的外部符号 _av_register_all。。。（这个因为以前在Linux上使用的都是自己编译出来的库，所以没注意这个坑）


最后通过这个链接解决的

[https://stackoverflow.com/questions/20672777/linker-error-using-ffmpeg-with-visual-studio-2013-express](https://stackoverflow.com/questions/20672777/linker-error-using-ffmpeg-with-visual-studio-2013-express)



2.把Dev库里解压出来的东西拷贝到工程中，Shared库中解压出来的东西拷贝到生成的bin文件目录（如release）

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

G:\source\FFmpegDemo\FFmpegDemo\ffmpeg>
├─inc
│  ├─libavcodec
│  ├─libavdevice
│  ├─libavfilter
│  ├─libavformat
│  ├─libavutil
│  ├─libpostproc
│  ├─libswresample
│  └─libswscale
└─libs
        avcodec.lib
        avdevice.lib
        avfilter.lib
        avformat.lib
        avutil.lib
        postproc.lib
        swresample.lib
        swscale.lib

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

3.右击工程“属性”,“C/C++”——>“附加包含目录”——>加入我们添加进来的头文件的路径

![](https://images2017.cnblogs.com/blog/708332/201708/708332-20170812210052742-1447496335.png)

4.在源码中链接lib文件

```
#pragma comment(lib,"ffmpeg\\libs\\avutil.lib")
#pragma comment(lib,"ffmpeg\\libs\\avformat.lib")
#pragma comment(lib,"ffmpeg\\libs\\avcodec.lib")
#pragma comment(lib,"ffmpeg\\libs\\swscale.lib")
```

源码如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

//main.cpp
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib,"ffmpeg\\libs\\avutil.lib")
#pragma comment(lib,"ffmpeg\\libs\\avformat.lib")
#pragma comment(lib,"ffmpeg\\libs\\avcodec.lib")
#pragma comment(lib,"ffmpeg\\libs\\swscale.lib")

extern "C" 
{

//编码
#include "libavcodec/avcodec.h"
//封装格式处理
#include "libavformat/avformat.h"
//像素处理
#include "libswscale/swscale.h"
};


int main(int argc, char* argv[])
{
    //获取输入输出文件名
    const char *input = "test.mp4";
    const char *output = "test.yuv";

    //1.注册所有组件
    av_register_all();

    //封装格式上下文，统领全局的结构体，保存了视频文件封装格式的相关信息
    AVFormatContext *pFormatCtx = avformat_alloc_context();

    //2.打开输入视频文件
    if (avformat_open_input(&pFormatCtx, input, NULL, NULL) != 0)
    {
        printf("%s", "无法打开输入视频文件");
        return -1;
    }

    //3.获取视频文件信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
    {
        printf("%s", "无法获取视频文件信息");
        return -1;
    }

    //获取视频流的索引位置
    //遍历所有类型的流（音频流、视频流、字幕流），找到视频流
    int v_stream_idx = -1;
    int i = 0;
    //number of streams
    for (; i < pFormatCtx->nb_streams; i++)
    {
        //流的类型
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            v_stream_idx = i;
            break;
        }
    }

    if (v_stream_idx == -1)
    {
        printf("%s", "找不到视频流\n");
        return -1;
    }

    //只有知道视频的编码方式，才能够根据编码方式去找到解码器
    //获取视频流中的编解码上下文
    AVCodecContext *pCodecCtx = pFormatCtx->streams[v_stream_idx]->codec;
    //4.根据编解码上下文中的编码id查找对应的解码
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL)
    {
        printf("%s", "找不到解码器\n");
        return -1;
    }

    //5.打开解码器
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        printf("%s", "解码器无法打开\n");
        return -1;
    }

    //输出视频信息
    printf("视频的文件格式：%s", pFormatCtx->iformat->name);
    printf("视频时长：%d", (pFormatCtx->duration) / 1000000);
    printf("视频的宽高：%d,%d", pCodecCtx->width, pCodecCtx->height);
    printf("解码器的名称：%s", pCodec->name);

    //准备读取
    //AVPacket用于存储一帧一帧的压缩数据（H264）
    //缓冲区，开辟空间
    AVPacket *packet = (AVPacket*)av_malloc(sizeof(AVPacket));

    //AVFrame用于存储解码后的像素数据(YUV)
    //内存分配
    AVFrame *pFrame = av_frame_alloc();
    //YUV420
    AVFrame *pFrameYUV = av_frame_alloc();
    //只有指定了AVFrame的像素格式、画面大小才能真正分配内存
    //缓冲区分配内存
    uint8_t *out_buffer = (uint8_t *)av_malloc(avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
    //初始化缓冲区
    avpicture_fill((AVPicture *)pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

    //用于转码（缩放）的参数，转之前的宽高，转之后的宽高，格式等
    struct SwsContext *sws_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
        pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P,
        SWS_BICUBIC, NULL, NULL, NULL);
    int got_picture, ret;

    FILE *fp_yuv = fopen(output, "wb+");

    int frame_count = 0;

    //6.一帧一帧的读取压缩数据
    while (av_read_frame(pFormatCtx, packet) >= 0)
    {
        //只要视频压缩数据（根据流的索引位置判断）
        if (packet->stream_index == v_stream_idx)
        {
            //7.解码一帧视频压缩数据，得到视频像素数据
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if (ret < 0)
            {
                printf("%s", "解码错误");
                return -1;
            }

            //为0说明解码完成，非0正在解码
            if (got_picture)
            {
                //AVFrame转为像素格式YUV420，宽高
                //2 6输入、输出数据
                //3 7输入、输出画面一行的数据的大小 AVFrame 转换是一行一行转换的
                //4 输入数据第一列要转码的位置 从0开始
                //5 输入画面的高度
                sws_scale(sws_ctx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                    pFrameYUV->data, pFrameYUV->linesize);

                //输出到YUV文件
                //AVFrame像素帧写入文件
                //data解码后的图像像素数据（音频采样数据）
                //Y 亮度 UV 色度（压缩了） 人对亮度更加敏感
                //U V 个数是Y的1/4
                int y_size = pCodecCtx->width * pCodecCtx->height;
                fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);
                fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);
                fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);

                frame_count++;
                printf("解码第%d帧\n", frame_count);
            }
        }

        //释放资源
        av_free_packet(packet);
    }

    fclose(fp_yuv);

    av_frame_free(&pFrame);

    avcodec_close(pCodecCtx);

    avformat_free_context(pFormatCtx);


    return 0;
}
```










