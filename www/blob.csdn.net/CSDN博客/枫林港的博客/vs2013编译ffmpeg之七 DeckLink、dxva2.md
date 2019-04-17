# vs2013编译ffmpeg之七 	DeckLink、dxva2 - 枫林港的博客 - CSDN博客





2018年07月07日 22:47:26[枫林港](https://me.csdn.net/u013601132)阅读数：166








# DeckLink

参考： 
[https://ffmpeg.zeranoe.com/forum/viewtopic.php?f=10&t=1823&p=7530&hilit=DeckLinkAPI.zip&sid=a76011a8eaff6238f26d9c3c64bb5e37#p7530](https://ffmpeg.zeranoe.com/forum/viewtopic.php?f=10&t=1823&p=7530&hilit=DeckLinkAPI.zip&sid=a76011a8eaff6238f26d9c3c64bb5e37#p7530)
[https://pkgs.org/slackware-14.1/slackonly-i486/decklink-sdk-10.1.4-noarch-1_slack.txz.html](https://pkgs.org/slackware-14.1/slackonly-i486/decklink-sdk-10.1.4-noarch-1_slack.txz.html)

Blackmagic Design公司视频录制卡的支持。官网上没有找到对应的api头文件，直接从第一个链接里面下载的，只需要头文件。 

后来发现在官网上可以下载，[https://www.blackmagicdesign.com/cn/support](https://www.blackmagicdesign.com/cn/support)
![这里写图片描述](https://img-blog.csdn.net/20180630001440331?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下载的版本Blackmagic_DeckLink_SDK_10.8.3.zip，解压后用Blackmagic DeckLink SDK 10.8.3\Linux\include下面的头文件。下载时要注册。
# dxva2

`--enable-dxva2`所需的头文件和库文件都在windows系统中，不需要增加额外的文件。 

windows下DXDIAG命令可以看到directx的信息： 
![这里写图片描述](https://img-blog.csdn.net/20180630001957740?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

ffmpeg-3.3用下面的语句测试dxva功能：
```bash
ffmpeg -hwaccel dxva2 -threads 1 -i "a.mp4" -f null - -benchmark
```

会在dxva2_device_create9ex()->IDirect3D9Ex_CreateDeviceEx()这里溢出，不清楚原因。删除ffmpeg-3.3\msvc\MSVC_ffmpeg-3.0\Debug下面的d3d9.dll和dxva2.dll，直接用系统里面的，但还是溢出。 

参考第三方的代码Win32Project1_ffmpeg_dxva2（[http://www.cnblogs.com/betterwgo/p/6125507.html](http://www.cnblogs.com/betterwgo/p/6125507.html)），发现其调用的是IDirect3D9_CreateDevice，而ffmpeg-3.3调用的是：

```
if (dxva2_device_create9ex(ctx, adapter) < 0) {
        // Retry with "classic" d3d9
        err = dxva2_device_create9(ctx, adapter);
        if (err < 0)
            return err;
    }
```

其中dxva2_device_create9ex调用的接口带ex即IDirect3D9Ex_CreateDeviceEx，IDirect3D9Ex_CreateDeviceEx这个函数调用导致溢出。所以解决的办法是直接注释掉if (dxva2_device_create9ex(ctx, adapter) < 0) {，只调用dxva2_device_create9。 

改完后可以运行了，但在程序运行完即将推出的时候报错了： 
![这里写图片描述](https://img-blog.csdn.net/20180630002011828?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM2MDExMzI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这个bug在ffmpeg-3.4.2版本中解决了，ffmpeg-3.3到ffmpeg-3.4.2之间的版本不清楚是具体哪个版本解决的，方法和下面的描述类似。下面分析代码： 

最后在释放资源的时候，调用顺序为av_buffer_pool_uninit()->buffer_pool_free()->buf->free即av_buffer_default_free，av_buffer_default_free调用的是av_free，configure的时候HAVE_ALIGNED_MALLOC定义成1了，av_free分配内存的时候会做对齐。 

通过跟踪buf->free所释放的内存地方，发现这个地址是在pool_alloc_buffer里面赋值的，即先调用pool->alloc2，然后赋值buf->data   = ret->buffer->data，buf->data就是buf->free的第二个参数。 

初始化时调用dxva2_init_pool()->av_buffer_pool_init2()，注册dxva2_pool_alloc到pool->alloc2，后面会用这个函数进行内存分配。ffmpeg-3.3对dxva2_pool_alloc调用了20次（这个值可能和ffmpeg的版本没有关系，没有研究过）。 

再看dxva2_pool_alloc的代码，其调用av_buffer_create()对buf->data进行赋值，即将surfaces_internal[i]的值赋给buf->data，而surfaces_internal[i]显然不是av_xxx函数分配的，所以av_free的时候会报错。 

解决的办法是直接弄个空函数来做释放，调用av_buffer_create的时候将这个函数注册进去（原始的代码av_buffer_create函数的第三个参数是NULL），那么buf->free就是这个dxva2_release_surfaces_internal了：
```
void dxva2_release_surfaces_internal(void *opaque, uint8_t *data)
{   
    return;
}
static AVBufferRef *dxva2_pool_alloc(void *opaque, int size)
{
    AVHWFramesContext      *ctx = (AVHWFramesContext*)opaque;
    DXVA2FramesContext       *s = ctx->internal->priv;
    AVDXVA2FramesContext *hwctx = ctx->hwctx;

    if (s->nb_surfaces_used < hwctx->nb_surfaces) {
        s->nb_surfaces_used++;
        return av_buffer_create((uint8_t*)s->surfaces_internal[s->nb_surfaces_used - 1],
                                sizeof(*hwctx->surfaces), dxva2_release_surfaces_internal, 0, 0);
    }

    return NULL;
}
```

dxva2_release_surfaces_internal里面不需要做什么，因为后面程序会调用dxva2_frames_uninit来释放surfaces_internal[i]。 
`ffmpeg-3.3：ffmpeg -hwaccel dxva2 -threads 1 -i "a.mp4" -f null --benchmark`运行结果如下：

```
Input #0, mpegts, from 'E:\tmp\stream\BenQ.HD.Demo.HDTV.1080i_8M_H2640x21_AAC0x22.mpg':
  Duration: 00:07:02.76, start: 1.000000, bitrate: 8315 kb/s
  Program 1
    Stream #0:0[0x21]: Video: h264 (Main) ([27][0][0][0] / 0x001B), yuv420p(progressive), 1920x1080 [SAR 1:1 DAR 16:9],
14.99 fps, 29.97 tbr, 90k tbn, 29.97 tbc
    Stream #0:1[0x22]: Audio: aac (LC) ([15][0][0][0] / 0x000F), 48000 Hz, stereo, fltp, 127 kb/s
Stream mapping:
  Stream #0:0 -> #0:0 (h264 (native) -> wrapped_avframe (native))
  Stream #0:1 -> #0:1 (aac (native) -> pcm_s16le (native))
Press [q] to stop, [?] for help
Output #0, null, to 'pipe:':
  Metadata:
    encoder         : Lavf57.71.100
    Stream #0:0: Video: wrapped_avframe, nv12(progressive), 1920x1080 [SAR 1:1 DAR 16:9], q=2-31, 200 kb/s, 14.99 fps, 1
4.99 tbn, 14.99 tbc
    Metadata:
      encoder         : Lavc57.89.100 wrapped_avframe
    Stream #0:1: Audio: pcm_s16le, 48000 Hz, stereo, s16, 1536 kb/s
    Metadata:
      encoder         : Lavc57.89.100 pcm_s16le
Error while decoding stream #0:1: Invalid data found when processing input
frame=12670 fps=239 q=-0.0 Lsize=N/A time=00:07:02.82 bitrate=N/A speed=7.97x
video:4751kB audio:79100kB subtitle:0kB other streams:0kB global headers:0kB muxing overhead: unknown
bench: utime=26.255s
bench: maxrss=296192kB
```






