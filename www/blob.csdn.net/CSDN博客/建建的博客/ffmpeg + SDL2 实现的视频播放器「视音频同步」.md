# ffmpeg + SDL2 实现的视频播放器「视音频同步」 - 建建的博客 - CSDN博客
2017年11月17日 14:10:24[纪建](https://me.csdn.net/u013898698)阅读数：240
个人分类：[ffmpeg学习](https://blog.csdn.net/u013898698/article/category/6692312)

日期：2016.10.8 
作者：isshe 
github：github.com/isshe 
邮箱：i.sshe@outlook.com 
平台：ubuntu16.04 64bit
## 前言
- 这个程序使用的视音频同步方法是视频同步音频。接下来大概还会学习其他方法，不过下一步应该是先完善功能，实现暂停，播放之类的。
- 这个版本中是用的是较新的两个解码函数avcodec_send_packet(), avcode_receive_frame()。如果旧版本没有，就换回avcodec_decode_video2()即可。
以下写着”问题:”的，如果懂请回答，谢谢。
## 程序结构图
![这里写图片描述](https://img-blog.csdn.net/20161008214718975)
- 主线程开一个线程专门负责读packet放到两个队列。
- 然后主线程开音频设备。
- 主线程开新线程给刷新函数(refresh_func)
- 然后主线程回去等待事件发生。
## 同步的思路
视频同步音频的方法的实质是：如果视频慢了就延时短点，如果快了就延时长点。
大体思路是：获取音频时间，获取视频帧显示时间，比较两个时间判断快了还是慢了，在上一帧的延时上做相应增减。
- 
以音频为基准的话，如何获取音频的当前播放时间(cur_audio_clock)？
> - 用av_p2d()把时基(time_base)转换成double类型，再乘以当前packet.pts即可得到当前packet的播放时间(audio_clock)。
- 因为一个packet中可能有多帧数据，所以获取的audio_clock大概只是前面帧的时间戳。 
后面的通过计算获得： 
- a.计算每秒播放的字节(bytes)数x，
- b.获取当前缓冲区index。
- c. 用index/x即可得到额外时间。
- d. 用audio_clock + index/x 即可算出当前播放时间。
- x的计算方法：样本率 * 每个样本大小 * 通道数。
- 图示：
- ![这里写图片描述](https://img-blog.csdn.net/20161009090857962)
- 
又如何获取视频的当前播放时间(cur_frame_pts)？
> - 当前packet.pts * time_base + extra_delay 
当前帧的显示时间戳 * 时基 +额外的延迟时间。 
extra_delay = repeat_pict / (2*fps) (从官网看来的)
- 
如何调整视频？
> - 用当前帧pts减去上一帧的pts得到上一帧的延时。
- 对比上面获得的两个时间（视频当前时间、音频当前时间）判断是快了还是慢了。 
当 cur_frame_pts - cur_audio_clock > 0 说明快了，小于就慢了。 
设置一个最大容忍值和一个最小容忍值。 
以上一帧延时为基础，当慢太多就减小延时(delay), 当快太多就加大延时(delay)。
- 
以上就是个人对视频同步音频方法的大体理解 , 如有不对请指出，感谢!
## 主要部分分析
### 1. 获取音频的当前播放时间
- 获取当前packet的时间
```
if (packet.pts != AV_NOPTS_VALUE)
     {
        ps->audio_clock = packet.pts * av_q2d(ps->paudio_stream->time_base);
     }
```
- 1
- 2
- 3
- 4
> 
1.av_q2d()是Convert rational to double. 
AVRational结构有两个成员： 
分子：int nun 
分母：int den
- 获取当前音频播放时间
```
double get_audio_clock(PlayerState *ps)
{
    long long bytes_per_sec = 0;
    double cur_audio_clock = 0.0;
    double cur_buf_pos = ps->audio_buf_index;
    //每个样本占2bytes。16bit
    bytes_per_sec = ps->paudio_stream->codec->sample_rate
                * ps->paudio_codec_ctx->channels * 2;
    cur_audio_clock = ps->audio_clock +
                    cur_buf_pos / (double)bytes_per_sec;
    return cur_audio_clock;
}
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
> - sample_rate：采样率（例如44100）
- channels：通道数
- 2代表2bytes。每个样本的大小(format指定)
- 获取当前音频播放时间：audio_clock + packet已经播放的时间. 
另一种获取当前时间的方法： 
audio_clock+整个packet能播的时间 - 还没播的时间。 (我参考的代码就是这种方法。)
### 获取视频frame的显示时间
- 
概念:
- PTS：显示时间戳。
- DTS：解码时间戳。
- 时基(time_base): 相当于一个单位。（大概是类似与s(秒）之类的) 
- ffmpeg中每个阶段有不同是时基。
- 代码中用到两个来调整时间，分别是:AVStream、AVCodecContext的time_base（都是以秒为单位，转换的时候要注意，SDL_Delay用的是毫秒， SDL_AddTimer用的是毫秒， av_gettime()用的是微秒）「1s = 1000ms = 1000000us」.
- 
获取packet.pts并调整
```
double get_frame_pts(PlayerState *ps, AVFrame *pframe)
{
    double pts = 0.0;
    double frame_delay = 0.0;
    pts = av_frame_get_best_effort_timestamp(pframe);
    if (pts == AV_NOPTS_VALUE)      //???
    {
        pts = 0;
    }
    pts *= av_q2d(ps->pvideo_stream->time_base);
    if (pts != 0)
    {
        ps->video_clock = pts;  
    }
    else
    {
        pts = ps->video_clock;
    }
    //更新video_clock
    //这里用的是AVCodecContext的time_base
    //extra_delay = repeat_pict / (2*fps), 这个公式是在ffmpeg官网手册看的
    frame_delay = av_q2d(ps->pvideo_stream->codec->time_base);
    frame_delay += pframe->repeat_pict / (frame_delay * 2);
    ps->video_clock += frame_delay;
    return pts;
}
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
> - 以中间的av_q2d为界，av_q2d以及以上代码为计算frame的pts。 
当获取不到的时候，就用av_q2d以下的代码来设置。 
问题：什么时候下会获取不到呢？
### 同步调整
- 同步就是快了就加延时（delay），慢了就减延时（delay）。
- 下面代码中，两个函数都是自定义的。 
- get_frame_pts()：获取当前帧的显示时间戳。
- get_delay()：获取延时时间。注意返回的是一个double类型，返回的时间是 s(秒)。给SDL_Delay()用的时候要*1000。
- (本来尝试在get_delay()内部转换单位，返回一个整型，但是相对比较麻烦，容易出错。)
```
pts = get_frame_pts(ps, pframe);
     //ps中用cur_frame_pts是为了减少get_delay()的参数
     ps->cur_frame_pts = pts; //*(double *)pframe.opaque;
     ps->delay = get_delay(ps) * 1000 + 0.5;
```
- 1
- 2
- 3
- 4
- get_delay()函数代码
```
double get_delay(PlayerState *ps)
{
    double      ret_delay = 0.0;
    double      frame_delay = 0.0;
    double      cur_audio_clock = 0.0;
    double      compare = 0.0;
    double      threshold = 0.0;
    //这里的delay是秒为单位， 化为毫秒：*1000
    //获取两帧之间的延时
    frame_delay = ps->cur_frame_pts - ps->pre_frame_pts;
    if (frame_delay <= 0 || frame_delay >= 1.0)
    {
        frame_delay = ps->pre_cur_frame_delay;
    }
    //两帧之间的延时存到统筹结构
    ps->pre_cur_frame_delay = frame_delay;
    ps->pre_frame_pts = ps->cur_frame_pts;
    cur_audio_clock = get_audio_clock(ps);
    //compare < 0 说明慢了， > 0说明快了
    compare = ps->cur_frame_pts - cur_audio_clock;
    //设置阀值, 是一个正数，最小阀值取它的负数。
    //这里设阀值为两帧之间的延迟，
    threshold = frame_delay;
    //SYNC_THRESHOLD ? frame_delay : SYNC_THRESHOLD;
    if (compare <= -threshold)      //慢， 加快速度， 
    {
        ret_delay = frame_delay / 2;  //这里用移位更好快
    }
    else if (compare >= threshold)  //快了，就在上一帧延时的基础上加长延时
    {
        ret_delay = frame_delay * 2; //这里用移位更好快
    }
    else
    {
        ret_delay = frame_delay;
    }
    return ret_delay;
}
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
## 所遇到的问题
### 1. ffmpeg共享内存导致的错误
> - 本来的实现是用一个新线程负责解码(packet->frame)， 解码后把frame放入队列，像把packet放入队列那样。 
这里会有一个画面闪烁或者严重丢帧的问题。 
原因就是每次新建的frame入队的时候，会共享内存， 这一个frame覆盖前一个frame。 
- 解决办法有： 
a. 把队列长度设为1。每次只解码一帧到队列，取了以后再解。 
b. 最容易想到的肯定是找不共享内存的方法了.
- 尝试过不共享内存的方法有： 
av_frame_clone(), 
av_frame_alloc()以后av_frame_ref()「这个看了手册，但不理解，只是用了」 
av_malloc()后用memcpy() 
av_frame_alloc后用memcpy(). 
都失败了。然后用a的方法实现了同步后，重写这部分代码，把frame队列删了。
## 参考资料
- 博文：[http://www.cnblogs.com/wangguchangqing/p/5900426.html](http://www.cnblogs.com/wangguchangqing/p/5900426.html)
- ffmepg在线手册：[http://ffmpeg.org/doxygen/2.1/frame_8h.html#acb9d9082c10e625e51a68caceabd4458](http://ffmpeg.org/doxygen/2.1/frame_8h.html#acb9d9082c10e625e51a68caceabd4458)
- SDL在线手册：[http://wiki.libsdl.org/SDL_OpenAudioDevice](http://wiki.libsdl.org/SDL_OpenAudioDevice)
## 代码下载
Linux版：[http://download.csdn.net/detail/i_scream_/9648046](http://download.csdn.net/detail/i_scream_/9648046)
Windows版：[http://download.csdn.net/detail/i_scream_/9648373](http://download.csdn.net/detail/i_scream_/9648373)
(window版用的ffmpeg比较老，使用的是老的解码函数avcodec_decode_audio4(), avcodec_decode_video2()) 
题外话：
- 接触ffmpeg已经8天了，搭环境用了两天，因为不知道怎么才算对，搭好了编译下载的程序不过，又再搭如此反复。最后还是看雷神的视频才懂，真的感谢他。
- 学习这几个程序，一次次重写，学习到了统筹结构很重要，也很好用。
- 还学习到，当出现错误的时候， 
- 如果是有不懂的函数，结构，要去查询，理解再继续。
- 如果函数之类都没问题，是逻辑之类问题，就认真看程序，去想，思考。
- 而不要盲目地一个个去试错。
