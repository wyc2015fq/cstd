# ffmpeg编码常见问题排查方法 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月16日 13:05:07[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：310
播放问题排查：
一旦我们遇到视频播放不了，第一件事，就是要找几个别的播放器也播放看看，做一下对比测试，或者对码流做一些基础分析，以便更好的定位问题的源头，而各个平台比较常见的播放/分析工具有如下几个：
**2.1 命令行工具**
ffplay，ffprobe，mediainfo，hls-analyzer 等
**2.2 网页端工具**
[http://www.cutv.com/demo/live_test.swf](http://www.cutv.com/demo/live_test.swf)
[http://www.ossrs.net/players/srs_player.html](http://www.ossrs.net/players/srs_player.html)
**2.3 App 应用**
vlc，vplayer，mxplayer 等
**2.4 Windows 工具**
mp4info，FlvParse，FLVMeta，Elecard StreamEye Studio 等
 ping 一下域名试试，看看是否可以 ping 通，如果 ping 不通，则可能要检查下域名解析的配置了。
**只有音频没有视频，或者只有视频没有音频：**
基于 ffmpeg 的播放器的 `probesize` 设置太小，导致解析码流信息不足

**播放卡顿：**
- 频繁出现缓冲- 播放不够流畅，画面一卡一卡的
其实是指播放器渲染的帧率太低
**如何判断主播网络不好**
**（1）带宽测试**
用带宽测试工具：[Speedtest
 by Ookla - The Global Broadband Speed Test](http://link.zhihu.com/?target=http%3A//www.speedtest.net/) 测试下主播的带宽，如果主播的上行带宽明显小于推流的码率，那么肯定会出现推流帧率不稳定。
[http://www.speedtest.net/](http://www.speedtest.net/)
**（2）统计回调**
一般的推流 SDK 都会统计主播推流的实时视频帧率，如果预设的帧率是 20fps，但是实际的帧率低得很多，比如 5fps，排除手机性能低的原因的话，多半也是网络带宽不足引起的。
**播放设备性能不足：**
- 尽可能选择使用硬解，充分利用 GPU 加速- 如果有多种码流，尽可能在低端机上选择非高清码流- 增大缓冲区，有助于缓解解码不稳定带来的卡顿
可以修改 ffplay 源码，把读取到的每一帧音频、视频的时间戳打印出来看看，这里我给出对 ffplay 的修改 commit 记录，大家可以参考一下：
[print
 the pts of each frame · Jhuster/pili-ffmpeg@4d0476f](http://link.zhihu.com/?target=https%3A//github.com/Jhuster/pili-ffmpeg/commit/4d0476faba5016b291c2eed2c0a2cd6fe303bd50)
为了有效降低 DNS 解析对首开的影响，我们可以提前完成播放域名->IP 地址的解析，并缓存起来，播放的时候，直接传入带 IP 地址的播放地址，从而省去了 DNS 解析的耗时。
如果要支持用 ip 地址播放，是需要修改底层 ffmpeg 源码的，目前七牛的 PLDroidPlayer 就支持这样的播放地址：
URL 格式：“protocol://ip/path?domain=xxxx.com”

所有基于 ffmpeg 的播放器，都会遇到 `avformat_find_stream_info` 这个函数耗时比较久，从而增大了首开时间，该函数主要作用是通过读取一定字节的码流数据，来分析码流的基本信息，如编码信息、时长、码率、帧率等等，它由两个参数来控制其读取的数据量大小和时长，一个是
 probesize，一个是 analyzeduration
减少 probesize 和 analyzeduration 可以有效地减少 `avformat_find_stream_info` 的函数耗时，从而加快首开，但是需要注意的是，设置地太小可能会导致读取的数据量不足，从而无法解析出码流信息，导致播放失败，或者出现只有音频没有视频，只有视频没有音频的问题。
一般图像处理、数据拷贝、编解码带来的延时，都是 ms 级别的，真正会产生比较大延时的地方，一个是互联网上的网络传输延时，另一个就是业务代码中的缓冲区了。
**推流端的数据是怎么 “积累” 起来的 ？**
采集 -> 编码 -> 数据发送 -> [服务器]
当网络产生抖动的时候，“数据发送” 会因此减慢，产生一定的阻塞，从而导致这些数据会被 “积累” 在了推流端的发送缓冲区中。
[服务器]-> 数据接收 -> 解码 -> 渲染
当网络产生抖动的时候，服务器的数据无法 “及时” 地传输到播放端，而由于 TCP 协议的可靠性，所有的数据都会被服务端积累起来，在网络恢复良好的时候，会快速传输到播放端，这些数据会被动地 “积累” 在接收缓冲区中。
**怎么消除业务缓冲区的累积延时呢 ？**
推流端的发送缓冲区，可以在网络恢复良好的时候，快送发送出去，从而消除掉这个累积延时。
播放端的接收缓冲区，可以通过丢帧或者加速播放的方式快速消费掉缓冲区中的数据，从而消除累计延时。
想从本质上解决直播延时问题，还是要换成基于
udp 的私有协议来传输数据。
参考：
[https://zhuanlan.zhihu.com/jhuster](https://zhuanlan.zhihu.com/jhuster)
