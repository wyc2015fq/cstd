# FFmpeg实现dxva2硬件加速 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月25日 18:06:01[boonya](https://me.csdn.net/boonya)阅读数：468










原文地址：[https://www.cnblogs.com/betterwgo/p/6125507.html](https://www.cnblogs.com/betterwgo/p/6125507.html)



Direct3D device manager，链接：[http://www.cnblogs.com/betterwgo/p/6124588.html](http://www.cnblogs.com/betterwgo/p/6124588.html)

在DirectShow中支持DXVA 2.0，链接：[http://www.cnblogs.com/betterwgo/p/6125351.html](http://www.cnblogs.com/betterwgo/p/6125351.html)

在做dxva2的过程中，参考了许多网上的代码，这些代码又多参考VLC和ffmpeg的例子。

**1.ffmpeg支持dxva2硬件加速的格式**

　　当前我所使用的ffmpeg的版本是3.2，支持dxva2硬件加速的有以下几种文件格式： **AV_CODEC_ID_MPEG2VIDEO、AV_CODEC_ID_H264、AV_CODEC_ID_VC1、AV_CODEC_ID_WMV3、AV_CODEC_ID_HEVC、AV_CODEC_ID_VP9**。ffmpeg识别为这几种格式的文件都可以尝试使用dxva2做硬件加速。但这并不代表是这几种格式的文件就一定支持dxva2硬件加速，因为我就遇到了一个AV_CODEC_ID_HEVC文件在初始化配置dxva2的过程中会失败，PotPlayer在播放这个文件时也不能用dxva2硬件加速。
**2.一些要注意的地方**

　　（1）ffmpeg只实现了dxva2硬件解码的内容。我所翻译的第一篇、第二篇文章的那部分内容除了解码部分，都要由用户自己去实现。这一块颇有一点复杂，不过不用担心，VLC和ffmpeg都有例子可以参考。这一部分的内容需要对以上两篇翻译的内容有所了解才能比较好的理解代码的逻辑。

　　（2）要想真正看到硬件加速的效果，解码后的数据不建议再copy到内存中用CPU进行处理。我一开始就是因为拷贝到吧解码后的数据又copy回内存导致不仅gpu的使用率看不到明显变化，而且CPU的使用率相对于不使用dxva2反而提高了。后来我修改为把解码后的数据直接显示出来，GPU使用率一下子就上去了，CPU使用率也降下来了。
**3.关键代码**

　　由于网上已经有从ffmpeg的例子中分离出来的配置dxva2解码器的代码，所以具体实现起来也相当简单。

（1）头文件ffmpeg_dxva2.h

```
/*
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef FFMPEG_DXVA2_H
#define FFMPEG_DXVA2_H

//#include "windows.h"

extern "C"{
#include "libavcodec/avcodec.h"
#include "libavutil/pixfmt.h"
#include "libavutil/rational.h"
}

enum HWAccelID {
    HWACCEL_NONE = 0,
    HWACCEL_AUTO,
    HWACCEL_VDPAU,
    HWACCEL_DXVA2,
    HWACCEL_VDA,
    HWACCEL_VIDEOTOOLBOX,
    HWACCEL_QSV,
};

typedef struct AVStream AVStream;
typedef struct AVCodecContext AVCodecContext;
typedef struct AVCodec AVCodec;
typedef struct AVFrame AVFrame;
typedef struct AVDictionary AVDictionary;

typedef struct InputStream {
    int file_index;
    AVStream *st;
    int discard;             /* true if stream data should be discarded */
    int user_set_discard;
    int decoding_needed;     /* non zero if the packets must be decoded in 'raw_fifo', see DECODING_FOR_* */
#define DECODING_FOR_OST    1
#define DECODING_FOR_FILTER 2

    AVCodecContext *dec_ctx;
    AVCodec *dec;
    AVFrame *decoded_frame;
    AVFrame *filter_frame; /* a ref of decoded_frame, to be sent to filters */

    int64_t       start;     /* time when read started */
    /* predicted dts of the next packet read for this stream or (when there are
        * several frames in a packet) of the next frame in current packet (in AV_TIME_BASE units) */
    int64_t       next_dts;
    int64_t       dts;       ///< dts of the last packet read for this stream (in AV_TIME_BASE units)

    int64_t       next_pts;  ///< synthetic pts for the next decode frame (in AV_TIME_BASE units)
    int64_t       pts;       ///< current pts of the decoded frame  (in AV_TIME_BASE units)
    int           wrap_correction_done;

    int64_t filter_in_rescale_delta_last;

    int64_t min_pts; /* pts with the smallest value in a current stream */
    int64_t max_pts; /* pts with the higher value in a current stream */
    int64_t nb_samples; /* number of samples in the last decoded audio frame before looping */

    double ts_scale;
    int saw_first_ts;
    int showed_multi_packet_warning;
    AVDictionary *decoder_opts;
    AVRational framerate;               /* framerate forced with -r */
    int top_field_first;
    int guess_layout_max;

    int autorotate;
    int resample_height;
    int resample_width;
    int resample_pix_fmt;

    int      resample_sample_fmt;
    int      resample_sample_rate;
    int      resample_channels;
    uint64_t resample_channel_layout;

    int fix_sub_duration;
    struct { /* previous decoded subtitle and related variables */
        int got_output;
        int ret;
        AVSubtitle subtitle;
    } prev_sub;

    struct sub2video {
        int64_t last_pts;
        int64_t end_pts;
        AVFrame *frame;
        int w, h;
    } sub2video;

    int dr1;

    /* decoded data from this stream goes into all those filters
        * currently video and audio only */
    //InputFilter **filters;
    //int        nb_filters;

    //int reinit_filters;

    /* hwaccel options */
    enum HWAccelID hwaccel_id;
    char  *hwaccel_device;

    /* hwaccel context */
    enum HWAccelID active_hwaccel_id;
    void  *hwaccel_ctx;
    void(*hwaccel_uninit)(AVCodecContext *s);
    int(*hwaccel_get_buffer)(AVCodecContext *s, AVFrame *frame, int flags);
    int(*hwaccel_retrieve_data)(AVCodecContext *s, AVFrame *frame);
    enum AVPixelFormat hwaccel_pix_fmt;
    enum AVPixelFormat hwaccel_retrieved_pix_fmt;

    /* stats */
    // combined size of all the packets read
    uint64_t data_size;
    /* number of packets successfully read for this stream */
    uint64_t nb_packets;
    // number of frames/samples retrieved from the decoder
    uint64_t frames_decoded;
    uint64_t samples_decoded;
} InputStream;


int dxva2_init(AVCodecContext *s, HWND hwnd);
int dxva2_retrieve_data_call(AVCodecContext *s, AVFrame *frame);

#endif /* FFMPEG_DXVA2_H */
```

　　以上代码其实是从ffmpeg中抽出来的。HWAccelID为硬件加速器的ID，在初始化配置解码器的时候会用到，我们实际用的是HWACCEL_DXVA2。InputStream这个结构体水很深，包含了一些在初始化配置中会用到的数据，还包含了一些函数指针，注意这些函数指针的使用。我要说的其实是以下两个函数：

```
int dxva2_init(AVCodecContext *s, HWND hwnd);
int dxva2_retrieve_data_call(AVCodecContext *s, AVFrame *frame);
```

　　函数dxva2_init是初始化配置dxva2解码器的入口，配置工作主要就是由它来完成。在文章最后我会上传整个工程的源码。前两篇翻译的文章的内容几乎都是为它服务的，我上传的源码中的ffmpeg_dxva2.cpp主要就是为了做这一部分工作，当然dxva2_retrieve_data_call也包含在了其中。要想看懂dxva2_init函数的逻辑，你最好看看前面两篇翻译的内容，另外你还需要懂一点D3D渲染的基本知识。

　　函数dxva2_retrieve_data_call用来获得解码后的数据的。如我前面所说，如果不必要，最后不要再把它copy出来，直接用D3D绘制出来就行了，把数据从GPU再copy到内存中会极大的降低GPU的使用率，在我的试验中这样做完全没达到GPU加速的目的，反而是CPU的使用率增高了。所以你在我上传的源码中看到的是直接绘制数据。

```
static int dxva2_retrieve_data(AVCodecContext *s, AVFrame *frame)
{
    LPDIRECT3DSURFACE9 surface = (LPDIRECT3DSURFACE9)frame->data[3];
    InputStream        *ist = (InputStream *)s->opaque;
    DXVA2Context       *ctx = (DXVA2Context *)ist->hwaccel_ctx;

    EnterCriticalSection(&cs);
    //直接渲染
    ctx->d3d9device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    ctx->d3d9device->BeginScene();
    if (m_pBackBuffer)
    {
        m_pBackBuffer->Release();
        m_pBackBuffer = NULL;
    }
    ctx->d3d9device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer);
    GetClientRect(d3dpp.hDeviceWindow, &m_rtViewport);
    ctx->d3d9device->StretchRect(surface, NULL, m_pBackBuffer, &m_rtViewport, D3DTEXF_LINEAR);
    ctx->d3d9device->EndScene();
    ctx->d3d9device->Present(NULL, NULL, NULL, NULL);

    LeaveCriticalSection(&cs);

    return 0;
}
```

（2）实现

　　有了ffmpeg_dxva2.h和ffmpeg_dxva2.cpp这两个文件后实现起来就非常简单了。

　主流程中配置dxva2部分的代码：

```
switch (codec->id)
  {
  case AV_CODEC_ID_MPEG2VIDEO:
  case AV_CODEC_ID_H264:
  case AV_CODEC_ID_VC1:
  case AV_CODEC_ID_WMV3:
  case AV_CODEC_ID_HEVC:
  case AV_CODEC_ID_VP9:
    {
        codecctx->thread_count = 1;  // Multithreading is apparently not compatible with hardware decoding
        InputStream *ist = new InputStream();
        ist->hwaccel_id = HWACCEL_AUTO;
        ist->active_hwaccel_id = HWACCEL_AUTO;
        ist->hwaccel_device = "dxva2";
        ist->dec = codec;
        ist->dec_ctx = codecctx;

        codecctx->opaque = ist;
        if (dxva2_init(codecctx, hWnd) == 0)
        {
            codecctx->get_buffer2 = ist->hwaccel_get_buffer;
            codecctx->get_format = GetHwFormat;
            codecctx->thread_safe_callbacks = 1;

            break;
        }

        bAccel = false;
        break;
    }
  default:
      bAccel = false;
      break;
  }
```

可以看出其中主要就是调用dxva2_init函数。

解码并渲染的代码：

```
if (pkt.stream_index == videoindex)
 {
  int got_picture = 0;

  DWORD t_start = GetTickCount();
  int bytes_used = avcodec_decode_video2(codecctx, picture, &got_picture, &pkt);
  if (got_picture)
  {
      if (bAccel)
      {
          //获取数据同时渲染
          dxva2_retrieve_data_call(codecctx, picture);

          DWORD t_end = GetTickCount();
          printf("dxva2 time using: %lu\n", t_end - t_start);
      }
      else
      {
          //非dxva2情形
          if (img_convert_ctx &&pFrameBGR && out_buffer)
          {
              //转换数据并渲染
              sws_scale(img_convert_ctx, (const uint8_t* const*)picture->data, picture->linesize, 0, codecctx->height, pFrameBGR->data, pFrameBGR->linesize);
              m_D3DVidRender.Render_YUV(out_buffer, picture->width, picture->height);

              DWORD t_end = GetTickCount();
              printf("normal time using: %lu\n", t_end - t_start);
          }
      }

      count++;
  }

  av_packet_unref(&pkt);
 }
```

在dxva2_init函数中其实已经对D3D的渲染进行了配置，所以只需要穿进去窗口句柄，然后调用dxva2_retrieve_data_call函数就可以直接把数据绘制在句柄所对应得窗口上。

源码：[http://download.csdn.net/download/qq_33892166/9698473](http://download.csdn.net/download/qq_33892166/9698473)

工程基于VS2013，需要对ffmpeg有一定了解，对D3D也要有一定的了解。注意在代码中修改要播放的视频的路径，否则控制台退出不正常，VS会卡死的，我也是刚发现有这个问题。最后自己修改一下控制台的代码。直接把调出控制台的代码注释掉也可以正常运行，不过就看不到调试信息了。



