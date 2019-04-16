# FFmpeg 4.0.2 实现YUV文件scale大小变换 - 墨雨依旧付疏狂Q - CSDN博客





2018年08月30日 22:54:45[墨雨依旧付疏狂Q](https://me.csdn.net/hsq1596753614)阅读数：139
所属专栏：[FFmpeg 4.0.2](https://blog.csdn.net/column/details/26855.html)









```cpp
/*
 * 功能：实现YUV文件scale大小变换
 * FFmpeg：4.0.2
*/
#include <iostream>
extern "C"
{
#include <libswscale/swscale.h>
#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
};
using namespace std;

int YUVFileScale(const char *srcYUV, int nSrcW, int nSrcH, const char *dstYUV, int nDstW, int nDstH)
{
    // 初始化输入源文件
    FILE *fp_input = NULL;
    fopen_s(&fp_input, srcYUV, "rb");
    if (fp_input == NULL)
    {
        cout << "error when open file input.yuv" << endl;
        return -1;
    }
    // 初始化输出目标文件
    FILE *fp_output = NULL;
    fopen_s(&fp_output, dstYUV, "wb");
    if (fp_output == NULL)
    {
        cout << "error when open file output.yuv" << endl;
        return -1;
    }
    // 源缓冲区
    int src_bufferSize = nSrcW * nSrcH * 3 / 2;
    uint8_t *src_bufferPtr = (uint8_t *)malloc(src_bufferSize * 2);
    if (src_bufferPtr == NULL)
    {
        cout << "error when malloc for src_bufferPtr" << endl;
        return -1;
    }
    // 目标缓冲区
    int dst_bufferSize = nDstW * nDstH * 3 / 2;
    uint8_t *dst_bufferPtr = (uint8_t *)malloc(dst_bufferSize * 2);
    if (dst_bufferPtr == NULL)
    {
        cout << "error when malloc for dst_bufferPtr" << endl;
        return -1;
    }
    // 分配空间
    int m_pVideoOutput_Zoomsize = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, nDstW, nDstH, 1);
    uint8_t *m_pVideoOutput_ZoomBuf = (uint8_t *)malloc(m_pVideoOutput_Zoomsize * 3 * sizeof(char)); //最大分配的空间，能满足yuv的各种格式  
    // 循环读取YUV文件中的每一帧                                                                                            // YUV scale大小变换
    int num = 1;
    while (!feof(fp_input))
    {
        int ret = fread(src_bufferPtr, 1, src_bufferSize, fp_input);
        if (ret < src_bufferSize)
        {
            cout << "read data over" << endl;
            break;
        }
        cout << "read one frame num = " << num++ << endl;
        //------------ scale变换 ------------------------------------------------------------------------------------------------------------------------------
        AVFrame *p_input_frame = av_frame_alloc();
        av_image_fill_arrays(p_input_frame->data, p_input_frame->linesize, src_bufferPtr, AV_PIX_FMT_YUV420P, nSrcW, nSrcH, 1);
        AVFrame *p_output_frame = av_frame_alloc();
        av_image_fill_arrays(p_output_frame->data, p_output_frame->linesize, m_pVideoOutput_ZoomBuf, AV_PIX_FMT_YUV420P, nDstW, nDstH, 1);
        struct SwsContext* m_pSwsContext = sws_getContext(nSrcW, nSrcH, AV_PIX_FMT_YUV420P, nDstW, nDstH, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
        if (NULL == m_pSwsContext)
        {
            cout << "ffmpeg get context error!" << endl;
            return -1;
        }
        sws_scale(m_pSwsContext, p_input_frame->data, p_input_frame->linesize, 0, nSrcH, p_output_frame->data, p_output_frame->linesize);
        sws_freeContext(m_pSwsContext);
        memcpy(dst_bufferPtr, m_pVideoOutput_ZoomBuf, m_pVideoOutput_Zoomsize);
        //-----------------------------------------------------------------------------------------------------------------------------------------------------
        fwrite(dst_bufferPtr, 1, m_pVideoOutput_Zoomsize, fp_output);
        if (num > 500)
            break;
    }
    // 释放内存
    free(m_pVideoOutput_ZoomBuf);
    fclose(fp_input);
    fclose(fp_output);
    free(src_bufferPtr);
    free(dst_bufferPtr);
    return 1;
}

int main()
{
    const char *srcYUV = "input.yuv";
    int nSrcW = 352, nSrcH = 288;
    const char *dstYUV = "output.yuv";
    int nDstW = 640, nDstH = 480;
    YUVFileScale(srcYUV, nSrcW, nSrcH, dstYUV, nDstW, nDstH);
    return getchar();
}
```



