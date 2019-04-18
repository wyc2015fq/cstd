# RTSP推流方案调优 - qq_42302962的博客 - CSDN博客
2018年05月26日 10:53:16[Linux高级开发](https://me.csdn.net/qq_42302962)阅读数：572
本项目是一个互联网音频广播得项目，其中APP推音频流到EasyDarwin服务器，云喇叭（音响，使用wifi联网）从EasyDarwin上拉流解码播放，但是在公司里测试时，老是出现播放卡顿的问题（公司网络不太稳定），前后大约耗时3个星期才从根本原因上解决问题。定位解决的过程大致如下：
一、一开始从源码去分析的，因为MP3一帧播放的时长大约是26.12毫秒，但是IOS得APP在填写RTP包的时间戳时舍去了0.12毫秒，直接填写26毫秒，也是起一个定时器，每隔26毫秒去推送一帧数据的，但是如果遇到网络情况不好的情况，那么不断每隔26毫秒去推送，那么会导致网络上的包越来越多，更加导致网络状况差，另一个方面，如果网络状态好时，因为这0.12毫秒的累积，最后也有可能导致拉流端的缓冲被填满的风险。于是做了第一次优化，优化方案就是使用了live555的taskschedule的机制，起了一个延时任务，在该任务里先记下上一帧推送RTP包后的系统时间戳（lastFrameTimestamp），然后记下推送当前帧的系统时间戳（currentTimeStamp），如果currentTimeStamp - lastFrameTimestamp的差值大于一帧数据的播放时长，则提前发送，提前的时间是delaySendTime = frameduration - (currentTimeStamp - g_lastFrameTimestamp - frameduration)，同时把这个差值累积起来（deltaDelayTime），如果该差值达到了一帧数据的播放时长，则立马发送下一帧数据。整个的核心代码如下，当然这也要感谢live555封装了一个如此实用的BasicUsageEnvironment模块，而且是可以单独编译的：
![](https://img-blog.csdn.net/20180526104838403?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQyMzAyOTYy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图1 推流的延时任务函数
二、使用了上面这个方案后，卡顿的现象好了很多，但是在公司里测试时依然发现会出现卡顿，于是继续find the root cause，这时不再怀疑推流方案会有什么问题了，同时也设置了TCP_NODELAY选项，这个时候感觉有点计穷力竭了，不断抓包分析后，某一个突然时刻发现有些RTP包的ACK延时一段时间后才收到，后来经过精确计算发现大约延时40毫秒，敏感的神经告诉我这非常可能就是引起网络拥塞导致音频直播卡顿的原因，于是网上查找为什么会有这个40毫秒的延时确认的现象，于是找到了这篇博客（[再探Linux下的TCP延迟确认机制--TCP_QUICKACK](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Fsctq8888%2Farticle%2Fdetails%2F7398967) ），按照该博客的提醒设置了TCP_QUICKACK选项，但是问题依旧，后来再经过不断的debug，发现该选项没有起作用，该选项在实时操作系统中才会生效，当然中间还试过BBR拥塞算法，但是没有什么改观，于是编译了实时操作系统替换了流媒体服务器的现有操作系统。
最后一测试，大功告成，历时3个星期的调试终于有了结果。说句实在话，这是我debug问题历时最久的一次。也许与工作经历有关吧，技术交流可以入QQ群【554271674】。
以为此记那些不妥协的精神。

