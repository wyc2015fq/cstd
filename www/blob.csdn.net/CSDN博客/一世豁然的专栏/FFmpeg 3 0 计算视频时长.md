# FFmpeg 3.0 计算视频时长 - 一世豁然的专栏 - CSDN博客





2017年02月16日 09:11:49[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2115








本博客转载于：[http://www.jianshu.com/p/66b13b2b2979](http://www.jianshu.com/p/66b13b2b2979)






1、问题：`AVFormatContext`和`AVStream`都有
```
duration:
 int64_t
```
字段，调用`av_dump_format()`打印的多媒体文件时长是怎么算出来的呢？


2、av_dump_format输出示例

```
Metadata:
    major_brand     : qt  
    minor_version   : 0
    compatible_brands: qt  
    creation_time   : 2014-01-26 20:14:21
  Duration: 00:00:04.00, start: 0.063733, bitrate: 7987 kb/s
```


3、`AVStream`对于`duration`的描述如下：

```
/**
 * Decoding: duration of the stream, in stream time base.
 * If a source file does not specify a duration, but does specify
 * a bitrate, this value will be estimated from bitrate and file size.
 */
int64_t duration;
```


4、`AVFormatContext`对于`duration`的描述如下：

```
/**
 * Duration of the stream, in AV_TIME_BASE fractional
 * seconds. Only set this value if you know none of the individual stream
 * durations and also do not set any of them. This is deduced from the
 * AVStream values if not set.
 *
 * Demuxing only, set by libavformat.
 */
int64_t duration;
```


5、`int av_read_frame(AVFormatContext *s, AVPacket *pkt);`有如下描述：

```
* pkt->pts, pkt->dts and pkt->duration are always set to correct
 * values in AVStream.time_base units (and guessed if the format cannot
 * provide them). pkt->pts can be AV_NOPTS_VALUE if the video format
 * has B-frames, so it is better to rely on pkt->dts if you do not
 * decompress the payload.
 *
 * @return 0 if OK, < 0 on error or end of file
 */
int av_read_frame(AVFormatContext *s, AVPacket *pkt);
```


6、avformat.h的描述

```
* AVPacket.pts, AVPacket.dts and AVPacket.duration timing information will be
 * set if known. They may also be unset (i.e. AV_NOPTS_VALUE for
 * pts/dts, 0 for duration) if the stream does not provide them. The timing
 * information will be in AVStream.time_base units, i.e. it has to be
 * multiplied by the timebase to convert them to seconds.
```


7、验证


从前述可知，duration以AV_TIME_BASE分数形式表示。若写入文件时不设置AVFormatContext的duration字段值，则从AVStream对应的字段中推断得出。那么，我理解的计算方式为：

```
IF AVFormatContext.duration != AV_NOPTS_VALUE
    time = AVFormatContext.duration * (1.0 * AVStream.num / AVStream.den)
THEN
    time = AVStream.duration * (1.0 * AVStream.num / AVStream.den)
```


time 的组成为123...n，从右往左数三个位为毫秒，剩余值为秒。


简单设计一段粗暴的验证代码：

```
AVStream *videoStream = fmtCtx->streams[videoStreamIndex];
int den = videoStream->time_base.den;
int num = videoStream->time_base.num;
printf("\nfmtContext->duration = %lld, stream->duration = %lld",
       fmtCtx->duration == AV_NOPTS_VALUE ? -1 : fmtCtx->duration ,
       videoStream->duration == AV_NOPTS_VALUE ? -1 : videoStream->duration);
printf("\nstream->numerator = %d, stream->denominator = %d", num, den);
if (fmtCtx->duration != AV_NOPTS_VALUE) {
    printf("\nfmtContext->duration = %lf, fmtContext->duration = %d(s)%3d(ms)",
           fmtCtx->duration * (num * 1.0 / den),
           (int)(fmtCtx->duration * (num * 1.0 / den) / 1000),
           ((int)(fmtCtx->duration * (num * 1.0 / den))) % 1000);
} else {
    printf("\nstream->duration = %lf",
           videoStream->duration * (num * 1.0 / den));
}
```


输入案例：
- 样本1：

Duration: 00:00:16.16

fmtContext->duration = 16160000, stream->duration = -1

stream->numerator = 1, stream->denominator = 1000

fmtContext->duration = 16160.000000, fmtContext->duration = 16(s)160(ms)
- 样本2：

Duration: 00:03:01.39

fmtContext->duration = 181389622, stream->duration = 16325066

stream->numerator = 1, stream->denominator = 90000

// AVFormatContext的计算结果

fmtContext->duration = 2015.440244, fmtContext->duration = 2(s)15(ms)

// AVStream的计算结果

stream->duration = 181.389622
- 样本3：

Duration: 00:00:04.00

fmtContext->duration = 4004967, stream->duration = 123123

stream->numerator = 1, stream->denominator = 30000

// AVFormatContext的计算结果

fmtContext->duration = 133.498900, fmtContext->duration = 0(s)133(ms)

// AVStream的计算结果

stream->duration = 4.104100

从样本2、3可知，当AVStream.duration有值时，AVStream.duration的计算结果近似FFmpeg的计算结果。


8、FFmpeg源码

```
int hours, mins, secs, us;
int64_t duration = fmtCtx->duration + (fmtCtx->duration <= INT64_MAX - 5000 ? 5000 : 0);
secs  = duration / AV_TIME_BASE;
us    = duration % AV_TIME_BASE;
mins  = secs / 60;
secs %= 60;
hours = mins / 60;
mins %= 60;
av_log(NULL, AV_LOG_INFO, "%02d:%02d:%02d.%02d", hours, mins, secs,
       (100 * us) / AV_TIME_BASE);
```


源码以`AV_TIME_BASE`为基准进行处理，定义如下。

```
/**
 * Internal time base represented as integer
 */
#define AV_TIME_BASE            1000000
```


那么，AVFormatContext.duration的描述可能是我理解错了。


9、AV_TIME_BASE、AV_TIME_BASE_Q和pts

`AV_TIME_BASE_Q`定义如下：

```
/**
 * Internal time base represented as fractional value
 */
#define AV_TIME_BASE_Q          (AVRational){1, AV_TIME_BASE}
```

`AVRatioal`定义如下：

```
/**
 * rational number numerator/denominator
 */
typedef struct AVRational{
    int num; ///< numerator
    int den; ///< denominator
} AVRational;
```


分数表示形式不方便计算，就如我前面写了很多冗余代码一样，FFmpeg提供了一个便捷函数。

```cpp
```cpp
/**
 * Convert rational to double.
 * @param a rational to convert
 * @return (double) a
 */
static inline double av_q2d(AVRational a){
    return a.num / (double) a.den;
}
```
```


那么，步骤7验证中计算视频时长可简化为

```
double totalSeconds = videoStream->duration * av_q2d(videoStream->time_base);
printf("totalSeconds = %lf\n", totalSeconds);
```


执行结果为totalSeconds = 181.389622。




