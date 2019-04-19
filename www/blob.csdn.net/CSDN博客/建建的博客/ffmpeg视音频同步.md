# ffmpeg视音频同步 - 建建的博客 - CSDN博客
2017年12月09日 17:03:33[纪建](https://me.csdn.net/u013898698)阅读数：72
- 视音频同步的简单介绍
- DTS 和 PTS
- 计算视频中Frame的显示时间
- 获取Audio clock（audio的播放时长）
- 将视频同步到音频上，实现视音频同步播放
## 视音频同步简单介绍
一般来说，视频同步指的是视频和音频同步，也就是说播放的声音要和当前显示的画面保持一致。想象以下，看一部电影的时候只看到人物嘴动没有声音传出；或者画面是激烈的战斗场景，而声音不是枪炮声却是人物说话的声音，这是非常差的一种体验。
在视频流和音频流中已包含了其以怎样的速度播放的相关数据，视频的帧率（Frame Rate）指示视频一秒显示的帧数（图像数）；音频的采样率（Sample Rate）表示音频一秒播放的样本（Sample）的个数。可以使用以上数据通过简单的计算得到其在某一Frame（Sample）的播放时间，以这样的速度音频和视频各自播放互不影响，在理想条件下，其应该是同步的，不会出现偏差。但，理想条件是什么大家都懂得。如果用上面那种简单的计算方式，慢慢的就会出现音视频不同步的情况。要不是视频播放快了，要么是音频播放快了，很难准确的同步。这就需要一种随着时间会线性增长的量，视频和音频的播放速度都以该量为标准，播放快了就减慢播放速度；播放快了就加快播放的速度。所以呢，视频和音频的同步实际上是一个动态的过程，同步是暂时的，不同步则是常态。以选择的播放速度量为标准，快的等待慢的，慢的则加快速度，是一个你等我赶的过程。
播放速度标准量的的选择一般来说有以下三种：
- 将视频同步到音频上，就是以音频的播放速度为基准来同步视频。视频比音频播放慢了，加快其播放速度；快了，则延迟播放。
- 将音频同步到视频上，就是以视频的播放速度为基准来同步音频。
- 将视频和音频同步外部的时钟上，选择一个外部时钟为基准，视频和音频的播放速度都以该时钟为标准。
## DTS和PTS
上面提到，视频和音频的同步过程是一个你等我赶的过程，快了则等待，慢了就加快速度。这就需要一个量来判断（和选择基准比较），到底是播放的快了还是慢了，或者正以同步的速度播放。在视音频流中的包中都含有DTS和PTS，就是这样的量（准确来说是PTS）。DTS，Decoding Time Stamp，解码时间戳，告诉解码器packet的解码顺序；PTS，Presentation Time Stamp，显示时间戳，指示从packet中解码出来的数据的显示顺序。
视音频都是顺序播放的，其解码的顺序不应该就是其播放的顺序么，为啥还要有DTS和PTS之分呢。对于音频来说，DTS和PTS是相同的，也就是其解码的顺序和解码的顺序是相同的，但对于视频来说情况就有些不同了。
视频的编码要比音频复杂一些，特别的是预测编码是视频编码的基本工具，这就会造成视频的DTS和PTS的不同。这样视频编码后会有三种不同类型的帧：
- I帧 关键帧，包含了一帧的完整数据，解码时只需要本帧的数据，不需要参考其他帧。
- P帧 P是向前搜索，该帧的数据不完全的，解码时需要参考其前一帧的数据。
- B帧 B是双向搜索，解码这种类型的帧是最复杂，不但需要参考其一帧的数据，还需要其后一帧的数据。
I帧的解码是最简单的，只需要本帧的数据；P帧也不是很复杂，值需要缓存上一帧的数据即可，总体来说都是线性，其解码顺序和显示顺序是一致的。B帧就比较复杂了，需要前后两帧的顺序，并且不是线性的，也是造成了DTS和PTS的不同的“元凶”，也是在解码后有可能得不到完整Frame的原因。（[更多I,B,P帧的信息可参考](http://blog.csdn.net/abcjennifer/article/details/6577934)）
假如一个视频序列，要这样显示I B B P，但是需要在B帧之前得到P帧的信息，因此帧可能以这样的顺序来存储I P B B，这样其解码顺序和显示的顺序就不同了，这也是DTS和PTS同时存在的原因。DTS指示解码顺序，PTS指示显示顺序。所以流中可以是这样的：
```
Stream : I P B B
DTS      1 2 3 4
PTS      1 4 2 3
```
通常来说只有在流中含有B帧的时候，PTS和DTS才会不同。
## 计算视频Frame的显示时间
在计算某一帧的显示时间之前，现来弄清楚FFmpeg中的时间单位：时间基（TIME BASE）。在FFmpeg中存在这多个不同的时间基，对应着视频处理的不同的阶段（分布于不同的结构体中）。在本文中使用的是`AVStream`的时间基，来指示Frame显示时的时间戳（timestamp）。
```
/**
    * This is the fundamental unit of time (in seconds) in terms
    * of which frame timestamps are represented.
    *
    */
AVRational time_base;
```
可以看出，`AVStream`中的time_base是以秒为单位，表示frame显示的时间，其类型为`AVRational`。 `AVRational`是一个分数，其声明如下：
```
/**
 * rational number numerator/denominator
 */
typedef struct AVRational{
    int num; ///< numerator
    int den; ///< denominator
} AVRational;
```
num为分子，den为分母。
PTS为一个`uint64_t`的整型，其单位就是`time_base`。表示视频长度的`duration`也是一个`uint64_t`，那么使用如下方法就可以计算出一个视频流的时间长度：
`time(second) = st->duration * av_q2d(st->time_base)`
st为一个AVStream的指针，`av_q2d`将一个`AVRational`转换为双精度浮点数。同样的方法也可以得到视频中某帧的显示时间
`timestamp(second) = pts * av_q2d(st->time_base)`
也就是说，得到了Frame的PTS后，就可以得到该frame显示的时间戳。
### 得到Frame的PTS
通过上面的描述知道，如果有了Frame的PTS就计算出帧的显示的时间。下面的代码展示了在从packet中解码出frame后，如何得到frame的PTS
```
ret = avcodec_receive_frame(video->video_ctx, frame);
if (ret < 0 && ret != AVERROR_EOF)
    continue;
if ((pts = av_frame_get_best_effort_timestamp(frame)) == AV_NOPTS_VALUE)
    pts = 0;
pts *= av_q2d(video->stream->time_base);
pts = video->synchronize(frame, pts);
frame->opaque = &pts;
```
注意，这里的pts是double型，因为将其乘以了time_base，代表了该帧在视频中的时间位置（秒为单位）。有可能存在调用`av_frame_get_best_effort_timestamp`得不到一个正确的PTS，这样的情况放在函数`synchronize`中处理。
```cpp
double VideoState::synchronize(AVFrame *srcFrame, double pts)
{
    double frame_delay;
    if (pts != 0)
        video_clock = pts; // Get pts,then set video clock to it
    else
        pts = video_clock; // Don't get pts,set it to video clock
    frame_delay = av_q2d(stream->codec->time_base);
    frame_delay += srcFrame->repeat_pict * (frame_delay * 0.5);
    video_clock += frame_delay;
    return pts;
}
```
`video_clock`是视频播放到当前帧时的已播放的时间长度。在`synchronize`函数中，如果没有得到该帧的PTS就用当前的`video_clock`来近似，然后更新video_clock的值。
到这里已经知道了video中frame的显示时间了（秒为单位），下面就描述如果得到Audio的播放时间，并以此时间为基准来安排video中显示时间。
## 获取Audio Clock
Audio Clock，也就是Audio的播放时长，可以在Audio时更新Audio Clock。在函数`audio_decode_frame`中解码新的packet，这是可以设置Auddio clock为该packet的PTS
```php
if (pkt.pts != AV_NOPTS_VALUE)
{
    audio_state->audio_clock = av_q2d(audio_state->stream->time_base) * pkt.pts;
}
```
由于一个packet中可以包含多个帧，packet中的PTS比真正的播放的PTS可能会早很多，可以根据Sample Rate 和 Sample Format来计算出该packet中的数据可以播放的时长，再次更新Audio clock 。
```cpp
// 每秒钟音频播放的字节数 sample_rate * channels * sample_format(一个sample占用的字节数)
audio_state->audio_clock += static_cast<double>(data_size) / (2 * audio_state->stream->codec->channels *            
        audio_state->stream->codec->sample_rate);
```
上面乘以2是因为sample format是16位的无符号整型，占用2个字节。
有了Audio clock后，在外面获取该值的时候却不能直接返回该值，因为audio缓冲区的可能还有未播放的数据，需要减去这部分的时间
```cpp
double AudioState::get_audio_clock()
{
    int hw_buf_size = audio_buff_size - audio_buff_index;
    int bytes_per_sec = stream->codec->sample_rate * audio_ctx->channels * 2;
    double pts = audio_clock - static_cast<double>(hw_buf_size) / bytes_per_sec;
    
    return pts;
}
```
用audio缓冲区中剩余的数据除以每秒播放的音频数据得到剩余数据的播放时间，从Audio clock中减去这部分的值就是当前的audio的播放时长。
## 同步
现在有了video中Frame的显示时间，并且得到了作为基准时间的音频播放时长Audio clock ，可以将视频同步到音频了。
- 用当前帧的PTS - 上一播放帧的PTS得到一个延迟时间
- 用当前帧的PTS和Audio Clock进行比较，来判断视频的播放速度是快了还是慢了
- 根据上一步额判断结果，设置播放下一帧的延迟时间。
使用要播放的当前帧的PTS和上一帧的PTS差来估计播放下一帧的延迟时间，并根据video的播放速度来调整这个延迟时间，以实现视音频的同步播放。
具体实现：
```cpp
// 将视频同步到音频上，计算下一帧的延迟时间
// 使用要播放的当前帧的PTS和上一帧的PTS差来估计播放下一帧的延迟时间，并根据video的播放速度来调整这个延迟时间
double current_pts = *(double*)video->frame->opaque;
double delay = current_pts - video->frame_last_pts;
if (delay <= 0 || delay >= 1.0)
    delay = video->frame_last_delay;
video->frame_last_delay = delay;
video->frame_last_pts = current_pts;
// 根据Audio clock来判断Video播放的快慢
double ref_clock = media->audio->get_audio_clock();
double diff = current_pts - ref_clock;// diff < 0 => video slow,diff > 0 => video quick
double threshold = (delay > SYNC_THRESHOLD) ? delay : SYNC_THRESHOLD;
// 调整播放下一帧的延迟时间，以实现同步
if (fabs(diff) < NOSYNC_THRESHOLD) // 不同步
{
    if (diff <= -threshold) // 慢了，delay设为0
        delay = 0;
    else if (diff >= threshold) // 快了，加倍delay
        delay *= 2;
}
video->frame_timer += delay;
double actual_delay = video->frame_timer - static_cast<double>(av_gettime()) / 1000000.0;
if (actual_delay <= 0.010)
    actual_delay = 0.010; 
// 设置一下帧播放的延迟
schedule_refresh(media, static_cast<int>(actual_delay * 1000 + 0.5));
```
`frame_last_pts`和`frame_last_delay`是上一帧的PTS以及设置的播放上一帧时的延迟时间。
- 首先根据当前播放帧的PTS和上一播放帧的PTS估算出一个延迟时间。
- 用当前帧的PTS和Audio clock相比较判断此时视频播放的速度是快还是慢了
- 视频播放过快则加倍延迟，过慢则将延迟设置为0
- frame_timer保存着视频播放的延迟时间总和，这个值和当前时间点的差值就是播放下一帧的真正的延迟时间
- schedule_refresh 设置播放下一帧的延迟时间。
1.视频的pts只精确到整数位，感觉这块是有误差的。
2.音频的时间戳需要减去缓冲区还剩下的数据进行计算
3.将他们的单位转换为毫秒。这要进行减法
