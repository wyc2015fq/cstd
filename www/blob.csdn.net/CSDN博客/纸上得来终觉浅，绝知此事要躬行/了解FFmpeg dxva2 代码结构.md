# 了解FFmpeg dxva2 代码结构 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月26日 15:53:03[boonya](https://me.csdn.net/boonya)阅读数：337








## ffmpeg官方地址

[http://ffmpeg.org/](http://ffmpeg.org/)

## dxva2.h头文件

[http://www.ffmpeg.org/doxygen/3.2/dxva2_8h_source.html](http://www.ffmpeg.org/doxygen/3.2/dxva2_8h_source.html)

### ffmpeg_dxva2.c源文件

[http://www.ffmpeg.org/doxygen/3.2/ffmpeg__dxva2_8c_source.html](http://www.ffmpeg.org/doxygen/3.2/ffmpeg__dxva2_8c_source.html)

### 获取ffmpeg源码
`git clone https://git.ffmpeg.org/ffmpeg.git ffmpeg`
### ffmpeg硬件加速桥梁

[http://www.ffmpeg.org/doxygen/3.2/group__lavc__codec__hwaccel.html](http://www.ffmpeg.org/doxygen/3.2/group__lavc__codec__hwaccel.html)
|## Modules| |
|----|----|
||[Direct3D11](http://www.ffmpeg.org/doxygen/3.2/group__lavc__codec__hwaccel__d3d11va.html)|
|| |
||[DXVA2](http://www.ffmpeg.org/doxygen/3.2/group__lavc__codec__hwaccel__dxva2.html)|
|| |
||[VA API Decoding](http://www.ffmpeg.org/doxygen/3.2/group__lavc__codec__hwaccel__vaapi.html)|
|| |
||[VDA](http://www.ffmpeg.org/doxygen/3.2/group__lavc__codec__hwaccel__vda.html)|
|| |
||[VDPAU Decoder and Renderer](http://www.ffmpeg.org/doxygen/3.2/group__lavc__codec__hwaccel__vdpau.html)|
||VDPAU hardware acceleration has two modules.|
|| |
||[XvMC](http://www.ffmpeg.org/doxygen/3.2/group__lavc__codec__hwaccel__xvmc.html)|

### ffmpeg常用示例

[http://www.ffmpeg.org/doxygen/3.4/examples.html](http://www.ffmpeg.org/doxygen/3.4/examples.html)
- [avio_reading.c](http://www.ffmpeg.org/doxygen/3.4/avio_reading_8c-example.html)
- [decode_audio.c](http://www.ffmpeg.org/doxygen/3.4/decode_audio_8c-example.html)
- [decode_video.c](http://www.ffmpeg.org/doxygen/3.4/decode_video_8c-example.html)
- [demuxing_decoding.c](http://www.ffmpeg.org/doxygen/3.4/demuxing_decoding_8c-example.html)
- [encode_audio.c](http://www.ffmpeg.org/doxygen/3.4/encode_audio_8c-example.html)
- [encode_video.c](http://www.ffmpeg.org/doxygen/3.4/encode_video_8c-example.html)
- [ffhash.c](http://www.ffmpeg.org/doxygen/3.4/ffhash_8c-example.html)
- [filter_audio.c](http://www.ffmpeg.org/doxygen/3.4/filter_audio_8c-example.html)
- [filtering_audio.c](http://www.ffmpeg.org/doxygen/3.4/filtering_audio_8c-example.html)
- [filtering_video.c](http://www.ffmpeg.org/doxygen/3.4/filtering_video_8c-example.html)
- [http_multiclient.c](http://www.ffmpeg.org/doxygen/3.4/http_multiclient_8c-example.html)
- [hw_decode.c](http://www.ffmpeg.org/doxygen/3.4/hw_decode_8c-example.html)
- [metadata.c](http://www.ffmpeg.org/doxygen/3.4/metadata_8c-example.html)
- [muxing.c](http://www.ffmpeg.org/doxygen/3.4/muxing_8c-example.html)
- [qsvdec.c](http://www.ffmpeg.org/doxygen/3.4/qsvdec_8c-example.html)
- [remuxing.c](http://www.ffmpeg.org/doxygen/3.4/remuxing_8c-example.html)
- [resampling_audio.c](http://www.ffmpeg.org/doxygen/3.4/resampling_audio_8c-example.html)
- [scaling_video.c](http://www.ffmpeg.org/doxygen/3.4/scaling_video_8c-example.html)
- [transcode_aac.c](http://www.ffmpeg.org/doxygen/3.4/transcode_aac_8c-example.html)
- [transcoding.c](http://www.ffmpeg.org/doxygen/3.4/transcoding_8c-example.html)

### ffmpeg一般操作流程

1、注册编解码器

2、申明和使用内存上下文变量

3、打开音视频文件，探测音视频文件流

4、打开编、解码器

5、持续编解码将数据放入生产队列处理

6、关闭编、解码器

7、释放内存

### ffmpeg视频硬件加速代码分析

**dxva2.h**定义了dxva的上下文

![](https://img-blog.csdn.net/20180926152347602?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**ffmpeg_dxva2.c**是ffmpeg对dxva2的实现：

定义了支持GPU编码的格式

![](https://img-blog.csdn.net/20180926152639702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

dxva2 上下文结构：

![](https://img-blog.csdn.net/20180926152832820?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

初始化 dxva2:

![](https://img-blog.csdn.net/20180926152950836?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

分配dxva2操作内存：

![](https://img-blog.csdn.net/20180926153144608?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

创建 解码器：

![](https://img-blog.csdn.net/20180926153259775?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

获取解码器配置：

![](https://img-blog.csdn.net/20180926153330264?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

获取buffer:

![](https://img-blog.csdn.net/20180926153435877?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

数据转换：

![](https://img-blog.csdn.net/20180926154305369?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

反初始化dxva2:完成内存释放

![](https://img-blog.csdn.net/20180926153106507?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###  示例程序关注

根据[ffmpeg实现dxva2硬件加速](https://blog.csdn.net/boonya/article/details/82842898)转载一文进行剖析。从最新版本的代码来看，有些跟转载文章上面是不一样的。

从上面源码的结构来看，已经很清晰的反映了代码实现需要的步骤。我们主要关注的是D3D是如何将视频渲染到绑定窗口的。

另外，代码里面多出来的调用方法（实际调用还是ffmpeg_dxva2.c的方法）：

```cpp
int dxva2_retrieve_data_call(AVCodecContext *s, AVFrame *frame)
{
    return dxva2_retrieve_data(s, frame);
}
```

核心代码见函数运行LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)。

注 ：关于内存问题先初始化，使用后及时销毁内存。



