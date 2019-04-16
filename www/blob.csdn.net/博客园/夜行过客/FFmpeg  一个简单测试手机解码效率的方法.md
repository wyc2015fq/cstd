# FFmpeg: 一个简单测试手机解码效率的方法 - 夜行过客 - 博客园







# [FFmpeg: 一个简单测试手机解码效率的方法](https://www.cnblogs.com/yongdaimi/p/10690792.html)





先写一个获取当前时间戳的方法

```
long long GetNowMs()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int sec = tv.tv_sec % 360000; // 为了简化计算，否则最后用于计算的ms会很大
    long long t = sec * 1000 + tv.tv_usec / 1000;
    return t;
}
```

然后在每次读取一帧数据之前和解码器解码完成之后添加下列代码

读取一帧数据之前：

```
if (GetNowMs() - start_time >= 3000) {
            LOGI("now decoder fps is %d", frameCount / 3);
            start_time = GetNowMs();
            frameCount = 0;
}

int re = av_read_frame(ic, pkt); // 读取一帧数据
......
```

解码器解码完一帧数据之后，对已解码的帧数进行累加：

```
for (;;) {
            re = avcodec_receive_frame(cc, frame);
            if (re != 0) {
                LOGE("avcodec_receive_frame() failed");
                break;
            }
            if (cc == vc) {
                frameCount++;
            }
}
```

通过已解码的帧数除以中间间隔的时长就可以得到每秒解码的帧数。

手头有两部手机，实际测试结果如下：

三星S6电信版

FFmpeg软解(单线程)： fps = 25-30帧， 耗费CPU：12%
FFmpeg软解(8线程)：fps = 100-120帧，耗费CPU：57-66%
FFmpeg硬解(8线程)：fps = 190-200帧, 耗费CPU：8%

谷歌Nexus6p

FFmpeg软解(单线程)： fps = 30-45帧(不稳定)， 耗费CPU：12%
FFmpeg软解(8线程)：fps = 90-100帧，耗费CPU：75-80%
FFmpeg硬解(8线程)：fps = 55帧, 耗费CPU：4%














